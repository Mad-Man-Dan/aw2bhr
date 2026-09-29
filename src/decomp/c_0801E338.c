#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801E338.
 * sub_0801E338 @ 0x0801E338
 */

/* Fills the sprite-request ring entry at gUnknown_03002510 and hands it to
 * sub_0801A718, advancing the cursor only if that did not return -1. The
 * unmasked twin of sub_0801ECE8.
 *
 * THE SIXTH PARAMETER IS `volatile` AND IS READ INTO A LOCAL. That pair is the
 * whole -4 this function was parked on from wave 33 to wave 41, and it is a
 * pair -- each half alone was measured and refuted in an earlier wave:
 *   - wave 36 tried `int t; t = a6; ... .unk0a = t;` with a plain `int`
 *     parameter. Copy propagation deletes the local before combine runs and the
 *     output is byte-identical to the direct store.
 *   - wave 40 tried volatile-flavoured USE-SITE expressions (`a6 & 0xFFFF`,
 *     `a6 * 1`) with no local. combine removes the redundant operation and
 *     narrows the stack load into the `strh` in the same pass.
 * Qualifying the PARAMETER volatile is what stops copy propagation from
 * deleting the local, so the read survives as a full-word `ldr [sp,#0x28]` in
 * the assign_parms block at the top -- which is exactly where the ROM has it --
 * and a6 gets the pseudo and the third callee-saved hi register that the
 * prologue and epilogue save. Every narrow retype also creates the pseudo but
 * loads it with `ldrsh`/`ldrh`; volatile is the only thing that keeps the
 * width.
 *
 * Top-level qualifiers on a parameter are ignored when checking type
 * compatibility, so this needs no change to the prototype in
 * include/unknown-functions.h and no caller is affected. */
int QueueSpriteRequest(int a1, int a2, int a3, int a4, long long a5, volatile int a6)
{
    int t;

    t = a6;
    gUnknown_0200ED20[gUnknown_03002510].unk00 = a2;
    gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
    gUnknown_0200ED20[gUnknown_03002510].unk04 = a4;
    gUnknown_0200ED20[gUnknown_03002510].unk08 = 0;
    gUnknown_0200ED20[gUnknown_03002510].unk0c = a5;
    gUnknown_0200ED20[gUnknown_03002510].unk0a = t;

    if (sub_0801A718(&gUnknown_0200ED20[gUnknown_03002510], a1) == -1)
        return 1;

    gUnknown_03002510++;
    return 0;
}
asm(".global sub_0801E338\n.thumb_set sub_0801E338, QueueSpriteRequest\n");
