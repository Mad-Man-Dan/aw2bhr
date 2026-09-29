#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DF60.
 * sub_0801DF60 @ 0x0801DF60, sub_0801DF74 @ 0x0801DF74
 */

/* gUnknown_03002520 is the 0x400-byte OAM shadow; this pair of accessors
 * reaches it as raw bytes, so the index is a byte offset masked to 0x3FF.
 */
u8 GetOamShadowByte(int offset)
{
    u8 *p = (u8 *)gUnknown_03002520;

    return p[offset & 0x3FF];
}
asm(".global sub_0801DF60\n.thumb_set sub_0801DF60, GetOamShadowByte\n");

/* The `volatile` is load-bearing: a volatile QImode store does not absorb the
 * u8 parameter's zero-extension, which is where the leading `lsl #24; lsr #24`
 * comes from. Without it agbcc folds the narrowing into the `strb` and the
 * function comes out four bytes short.
 */
void SetOamShadowByte(int offset, u8 value)
{
    volatile u8 *p = (volatile u8 *)gUnknown_03002520;

    p[offset & 0x3FF] = value;
}
asm(".global sub_0801DF74\n.thumb_set sub_0801DF74, SetOamShadowByte\n");
