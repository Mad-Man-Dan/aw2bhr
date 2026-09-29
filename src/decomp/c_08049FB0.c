#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049FB0.
 * sub_08049FB0 @ 0x08049FB0, sub_08049FD4 @ 0x08049FD4
 */

/* `lsls #0x14` ... `asrs #0x10` is the s16 conversion of `b * 16 + 18` FUSED
 * with the multiply: (x << 16) >> 16 sign-extends the low half, and `b * 16`
 * shifted left another 16 is `b << 20`, with `movs r0,#0x90; lsls r0,#0xd`
 * being the constant 18 << 16. It is not a shift in the source and the
 * conversion comes from SetMapCursorDisplayPosition's declared `s16` second parameter. */
void LanguageSelect_CursorInit(void)
{
    LoadCursorSpriteGraphics();
    SetMapCursorDisplayPosition(0x60, gUnknown_02028E40 * 16 + 18);
}
asm(".global sub_08049FB0\n.thumb_set sub_08049FB0, LanguageSelect_CursorInit\n");

/* LanguageSelect_CursorInit's twin on EaseMapCursorAndDraw instead of SetMapCursorDisplayPosition -- same
 * `gUnknown_02028E40 * 16 + 18` argument and the same s16 conversion fused into
 * it. See LanguageSelect_CursorInit. */
void LanguageSelect_CursorStep(void)
{
    EaseMapCursorAndDraw(0x60, gUnknown_02028E40 * 16 + 18, 3);
}
asm(".global sub_08049FD4\n.thumb_set sub_08049FD4, LanguageSelect_CursorStep\n");
