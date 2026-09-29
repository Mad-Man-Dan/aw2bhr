#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D348.
 * sub_0801D348 @ 0x0801D348
 */

/* A two-way dispatch on bit 0 of argument 1. The `long long` seventh argument
 * is not a guess: the pair of adjacent word loads carries agbcc's own "created
 * by thumb_load_double_from_address" note and lands in QueueSpriteRequest's declared
 * 64-bit slot, which forces the 8-byte alignment that puts it at +0x24. */
/* Wave 56, W56-H. The last parameter was retyped `u16` -> `s16`. The witness is
 * the ONE caller, sub_0801D390: it loads gUnknown_0200E438[i].unk38 with
 * `movs r6,#0x38; ldrsh r0,[r1,r6]` before both `str r0,[sp,#0x10]` stack pushes.
 * A `u16` parameter makes agbcc zero-extend, which is the encodable
 * `ldrh r0,[r1,#0x38]` -- 2 bytes shorter at each of the two call sites and 4
 * bytes short overall. This body stays byte-for-byte identical either way
 * (PROMOTE_MODE zero-extends the stack slot regardless of signedness and the
 * `(s16)` cast at the use is unchanged), re-verified by trymatch. */
void SubmitSpriteScriptFrame(int a, int b, int c, int d, int e, int f, long long g, s16 h)
{
    if (a & 1)
        QueueSpriteRequestSimple(e, b, c, d, f);
    else
        QueueSpriteRequest(e, b, c, d, g, (s16)h);
}
asm(".global sub_0801D348\n.thumb_set sub_0801D348, SubmitSpriteScriptFrame\n");
