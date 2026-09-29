#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800C874.
 * CountProperties @ 0x0800C874, CountPropertiesOfType @ 0x0800C8A0, RegisterArmyHqs @ 0x0800C8D8, sub_0800C958 @ 0x0800C958, sub_0800C9E8 @ 0x0800C9E8
 */

/* Counts the live entries in the gProperty list: 4-byte records whose
 * first byte is 0 when the slot is empty and 0xFF when the list ends.
 *
 * The list bound and the sentinel are TWO exits and their order is readable
 * straight off the bottom of the loop -- `adds r1,#4; cmp r1,r3; bgt` runs
 * BEFORE the 0xFF re-check -- so this is a bounded `for` whose body `break`s
 * on the sentinel, not a `while (p->flags != 0xFF)`.  The 0xFF test ahead of
 * the loop is that same `break` rotated into a guard, and the `movs r0,#0xb6;
 * lsls #1` preheader (0x16C == 0x5C * 4) is strength_reduce's giv limit, not
 * source.
 *
 * A leaf with no `push`: the counter, the pointer giv and the limit fit in
 * r0..r3. */
int CountProperties(void)
{
    int i;
    int n;

    n = 0;

    for (i = 0; i <= 0x5B; i++)
    {
        if (gProperty[i].flags == 0xFF)
            break;
        if (gProperty[i].flags != 0)
            n++;
    }

    return n;
}

asm(".global sub_0800C874\n.thumb_set sub_0800C874, CountProperties\n");

/* CountProperties with one more conjunct: the same bounded scan of
 * gProperty, counting only the live records whose flags byte equals
 * the caller's.  The `cmp r0,#0` survives beside `cmp r0,r3` even though a
 * zero argument would make it redundant, so the emptiness test really is
 * written separately in the source and is not folded into the equality.
 *
 * The extra `push {r4, lr}` over CountProperties is the parameter occupying r3,
 * which pushes the giv limit into r4. */
u32 CountPropertiesOfType(int a)
{
    int i;
    int n;

    n = 0;

    for (i = 0; i <= 0x5B; i++)
    {
        if (gProperty[i].flags == 0xFF)
            break;
        if (gProperty[i].flags != 0 && gProperty[i].flags == a)
            n++;
    }

    return n;
}
asm(".global sub_0800C8A0\n.thumb_set sub_0800C8A0, CountPropertiesOfType\n");

int RegisterArmyHqs(void)
{
    int i;
    int n;

    n = 0;

    for (i = 0; i <= 3; i++)
    {
        gActiveMap->hqX[i] |= 0xFF;
        gActiveMap->hqY[i] |= 0xFF;
    }

    for (i = 0; i <= 0x5B; i++)
    {
        if (gProperty[i].flags == 0xFF)
            break;
        if (gProperty[i].flags != 0)
        {
            switch (gProperty[i].flags)
            {
            case 0x28:
            case 0x48:
            case 0x68:
            case 0x88:
                SetArmyHq(gProperty[i].flags, gProperty[i].x,
                             gProperty[i].y);
                n++;
                break;
            }
        }
    }

    return n;
}

asm(".global sub_0800C8D8\n.thumb_set sub_0800C8D8, RegisterArmyHqs\n");

/* Totals three CountPropertiesOfType counts for whichever id the caller names, after
 * checking the id is currently valid.  Undeclared before this wave.
 *
 * The three ids per arm are the base id's nibble variants -- 0x2E/0x2A/0x2B for
 * 0x28, and the same +6/+2/+3 pattern for 0x48, 0x68 and 0x88 -- so the four
 * arms are one shape over a different base, which is why gcc cross-jumps their
 * tails: the final `bl CountPropertiesOfType; adds r5, r5, r0` is emitted ONCE and all
 * four arms branch to it with the id already in r0.  That merge is the
 * compiler's, not the source's; the arms are written out in full.
 *
 * An accumulator, not a three-term sum -- `adds r5, r0, #0` then `adds r5, r5,
 * r0` -- the same shape sub_0800C6A8 needs and for the same reason.
 *
 * The two out-parameters GetArmyHq fills are never read: this function only
 * wants its yes/no, and the -1 it returns on failure is distinct from the 0 a
 * valid-but-empty id gives. */
int sub_0800C958(int a)
{
    int x;
    int y;
    int n;

    if (GetArmyHq(a, &x, &y) == 0)
        return -1;

    n = 0;

    switch (a)
    {
    case 0x28:
        n = CountPropertiesOfType(0x2E);
        n += CountPropertiesOfType(0x2A);
        n += CountPropertiesOfType(0x2B);
        break;
    case 0x48:
        n = CountPropertiesOfType(0x4E);
        n += CountPropertiesOfType(0x4A);
        n += CountPropertiesOfType(0x4B);
        break;
    case 0x68:
        n = CountPropertiesOfType(0x6E);
        n += CountPropertiesOfType(0x6A);
        n += CountPropertiesOfType(0x6B);
        break;
    case 0x88:
        n = CountPropertiesOfType(0x8E);
        n += CountPropertiesOfType(0x8A);
        n += CountPropertiesOfType(0x8B);
        break;
    }

    return n;
}

/* Asks sub_0800C958 about all four ids, falling back to CountArmyUnits(team) when
 * an id is not currently registered, and answers whether ALL four are present
 * and more than one of them is non-empty.
 *
 * Four copies of one block, with the team number 1..4 pairing with the ids
 * 0x28/0x48/0x68/0x88 in order.  Each block keeps its own result in a
 * callee-saved register (r8, r7, r6, r4) because all four are still live at the
 * final `&&` chain -- that is what the `mov r7, r8` push is for, not a loop.
 *
 * The first block's `movs r5, #1` where the other three have `adds r5, #1` is
 * CSE: n is provably 0 there, so the increment folds to a constant, and gcc
 * then reuses that same register for the `a = 1` beside it.  The source is the
 * same `n++` in all four.
 *
 * The tail is the preset-and-overwrite form -- `movs r0,#0`, the tests, then
 * `movs r0,#1` with NO unconditional branch between them -- so it is an
 * assignment to one result variable, not `if (...) return 1; else return 0;`. */
int sub_0800C9E8(void)
{
    int n;
    int a;
    int b;
    int c;
    int d;
    int r;

    n = 0;

    a = sub_0800C958(0x28);
    if (a == 0)
    {
        if (CountArmyUnits(1) > 0)
        {
            n++;
            a = 1;
        }
    }
    else if (a > 0)
    {
        n++;
    }

    b = sub_0800C958(0x48);
    if (b == 0)
    {
        if (CountArmyUnits(2) > 0)
        {
            n++;
            b = 1;
        }
    }
    else if (b > 0)
    {
        n++;
    }

    c = sub_0800C958(0x68);
    if (c == 0)
    {
        if (CountArmyUnits(3) > 0)
        {
            n++;
            c = 1;
        }
    }
    else if (c > 0)
    {
        n++;
    }

    d = sub_0800C958(0x88);
    if (d == 0)
    {
        if (CountArmyUnits(4) > 0)
        {
            n++;
            d = 1;
        }
    }
    else if (d > 0)
    {
        n++;
    }

    r = 0;
    if (a != 0 && b != 0 && c != 0 && d != 0 && n > 1)
        r = 1;

    return r;
}
