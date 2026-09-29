#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802042C.
 * sub_0802042C @ 0x0802042C
 */

/* Traces a path downhill through the gUnknown_03003340 cost overlay from (x, y),
 * appending one direction code per step to *p and handing the run to
 * RevertMovementScript to reverse in place. Each step samples the four orthogonal
 * neighbours (0 = +x, 1 = -x, 2 = -y, 3 = +y; off-map reads as 0xff), takes the
 * minimum, collects every neighbour tied for it, and breaks the tie with
 * GetNextRandomNumber. `dir` survives across steps because the switch has no default
 * arm -- a tie count of 0 leaves the previous direction standing.
 *
 * The neighbour stores and the step switch are both written 0, 1, 3, 2; that is
 * the block order in the ROM for both, so it is source order and not a
 * scheduling artefact.
 *
 * The loop test goes through the `s8` local `c`: the ROM reads the cell with
 * `ldrb` and then sign-extends with `lsls #0x18; asrs #0x18` before `cmp #0`,
 * and a written-out `(s8)cell != 0` cannot produce that -- fold strips a
 * same-width sign change out of an equality test. A signed-char LOCAL does:
 * PROMOTE_MODE parks it zero-extended, so every signed read of it re-extends.
 *
 * The four neighbour samples MUST be if/else and not `?:`, and this was worth
 * 28 bytes -- the whole residual. A ternary gives one merged store, so agbcc
 * keeps a single `&nb[0]` in a register and reaches every element with a
 * displacement; written as two statements each arm materialises `sp + 8 + 2k`
 * from scratch, which is what the ROM does. This is the store-address twin of
 * the "one local where the original had N" rule. */
void GenerateBestMovementScript(int x, int y, u8 *p)
{
    s16 sel[4];
    s16 nb[4];
    u8 *start;
    s16 best;
    s16 n;
    s16 dir;
    s8 c;
    int i;

    start = p;
    dir = 0;
    gUnknown_03004074 = gUnknown_03003340[y][x];
    c = gUnknown_03003340[y][x];

    while (c != 0)
    {
        if (x + 1 == gMap->width)
            nb[0] = 0xff;
        else
            nb[0] = gUnknown_03003340[y][x + 1];
        if (x == 0)
            nb[1] = 0xff;
        else
            nb[1] = gUnknown_03003340[y][x - 1];
        if (y + 1 == gMap->height)
            nb[3] = 0xff;
        else
            nb[3] = gUnknown_03003340[y + 1][x];
        if (y == 0)
            nb[2] = 0xff;
        else
            nb[2] = gUnknown_03003340[y - 1][x];

        best = 0x100;
        n = 0;

        for (i = 0; i < 4; i++)
            if (best > nb[i])
                best = nb[i];

        for (i = 0; i < 4; i++)
            if (best == nb[i])
                sel[n++] = i;

        switch (n)
        {
        case 1:
            dir = sel[0];
            break;
        case 2:
            dir = sel[(GetNextRandomNumber() >> 14) & 1];
            break;
        case 3:
            dir = sel[GetNextRandomNumber() % 3];
            break;
        case 4:
            dir = sel[GetNextRandomNumber() & 3];
            break;
        }

        *p++ = dir;

        switch (dir)
        {
        case 0:
            x = x + 1;
            break;
        case 1:
            x = x - 1;
            break;
        case 3:
            y = y + 1;
            break;
        case 2:
            y = y - 1;
            break;
        }

        c = gUnknown_03003340[y][x];
    }

    RevertMovementScript(start, p);
}
asm(".global sub_0802042C\n.thumb_set sub_0802042C, GenerateBestMovementScript\n");
