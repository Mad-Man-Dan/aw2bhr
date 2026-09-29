#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C7DC.
 * sub_0801C7DC @ 0x0801C7DC
 */

/* PARKED ON THE PIPELINE, NOT ON THE DECOMPILATION (wave 42, W42-F).
 *
 * trymatch reports `+2` and simultaneously `bytes: 0 of 154 differ (100.0%
 * identical)` with `relocs: match`. The whole of the residual is a trailing
 * `.short 0x0000`: AP_PutAnimFrameAtTime is 154 bytes, THUMB code ending 2 mod 4 leaves
 * `.text` short of its four-byte section alignment, and trymatch reads the
 * section with `objcopy --only-section=.text`, so the pad is inside the window
 * it compares. Nothing writable in C moves it. Same class as OptionsMenu_DeleteUsability --
 * see the wave-24 chapter in docs/agbcc-codegen.md, amended by wave 42.
 *
 * The ROM spends those two bytes the same way. baserom.gba at 0x0801C876 reads
 * `00 00`, and the next function's `push {r4-r7,lr}` sits at the 4-aligned
 * 0x0801C878. Unlike OptionsMenu_DeleteUsability the pad has NO symbol of its own -- this
 * function is the LAST in asm/code.s, whose final two lines are `.align 2, 0`,
 * so the padding is a section boundary rather than an inter-function stub.
 * Promotion therefore only has to carve the function; the promoted unit's own
 * trailing `.align 2, 0` supplies the same two bytes.
 *
 * DO NOT go looking for another spelling. Any edit that changes the size by two
 * is wrong by two the other way.
 *
 * Two derivations in here were the whole difficulty.
 *
 * 1. THE THIRD CASE LABEL. The dispatch is `cmp #1; beq` / `cmp #1; bgt` /
 *    `cmp #0; beq` -- three compares with the bound spelled literally, which is
 *    the switch tell. But a switch on {0, 1} alone does NOT produce it: with two
 *    case nodes balance_case_nodes leaves the list linear, the root is the
 *    LOWEST case, and because `p[1]` is unsigned the low-bound test folds away,
 *    giving `cmp #0; beq` / `cmp #1; beq` -- two compares, 4 bytes short
 *    (measured at 96.8%). A THIRD case node makes balance_case_nodes split, the
 *    root becomes case 1 with case 0 as its left child and no right child, and
 *    emit_case_nodes emits exactly the ROM's three compares.
 *    The third label's VALUE is not recoverable from the ROM: everything above 1
 *    goes to the shared block via the `bgt`, so the node emits no code of its
 *    own. `case 2` is the smallest value that works and its body is written out
 *    rather than fallen through, because that is the spelling that was verified.
 *    Any value > 1 with an identical body gives the same bytes.
 *
 * 2. THE CURSOR IS THE VARIABLE THAT IS COMPUTED, AND `start` IS THE COPY.
 *    The ROM does `adds r2, r0, r1` then `adds r5, r2, #0`, i.e. it builds the
 *    walking cursor first and saves a copy of it. Writing it the other way round
 *    (`start = ...; for (p = start; ...)`) swaps the two registers and reorders
 *    the `movs r1, #0` -- 5 bytes differ, everything else already exact. Hence
 *    `p = ...; start = p;` and a `while` rather than a `for` on the first loop.
 *
 * The `lsrs #1; lsls #1` pairs are NOT `& ~1` masks. `a1` is `const u16 *`, so
 * they are a genuine `>> 1` of a byte offset into a halfword index followed by
 * the compiler's own scale-by-2 for the pointer addition -- i.e. plain
 * `a1 + (a1[0] >> 1)`, exactly what the brief's "count lsls/lsrs pairs as
 * shifts first" rule says to try before inventing a mask.
 *
 * The return is `(u8)`: `lsls r5, r1, #0x18` before the call and `lsrs r0, r5,
 * #0x18` after it are the two halves of one zero-extend-from-byte, split
 * because the value has to survive PutSpriteExt in a callee-saved register.
 */

int AP_PutAnimFrameAtTime(const u16 *a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    const u16 *base;
    const u16 *tbl;
    const u16 *start;
    const u16 *p;
    int sum;
    int ret;

    base = a1 + (a1[0] >> 1);
    tbl = a1 + (a1[1] >> 1);

    p = tbl + (tbl[a2] >> 1);
    start = p;

    sum = 0;
    while (p[0] != 0)
    {
        sum += p[0];
        p += 2;
    }

    switch (p[1])
    {
    default:
        a3 = DivRem(a3, sum);
        ret = 1;
        break;
    case 2:
        a3 = DivRem(a3, sum);
        ret = 1;
        break;
    case 0:
        if (a3 > sum)
        {
            a3 = sum;
            ret = 0;
        }
        else
        {
            ret = 1;
        }
        break;
    case 1:
        return 0;
    }

    for (p = start; p[0] != 0; p += 2)
    {
        a3 -= p[0];
        if (a3 <= 0)
            break;
    }

    PutSpriteExt(a7, a4, a5, (u16 *)(base + (base[p[1]] >> 1)), a6);
    return (u8)ret;
}
asm(".global sub_0801C7DC\n.thumb_set sub_0801C7DC, AP_PutAnimFrameAtTime\n");
