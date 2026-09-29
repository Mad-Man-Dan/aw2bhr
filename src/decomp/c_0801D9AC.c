#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D9AC.
 * sub_0801D9AC @ 0x0801D9AC, sub_0801D9E4 @ 0x0801D9E4, sub_0801DA14 @ 0x0801DA14
 */

/* Position plus offset, in 8.8 fixed point, written out as two s16. The
 * `cmp #0; bge; adds #0xff` ahead of the `asrs #8` is a SIGNED DIVIDE by 256
 * rounding toward zero -- a plain `>> 8` on the same s32 gives the bare `asrs`
 * with no correction. */
void GetSpriteScriptDrawPosition(int a, s16 *x, s16 *y)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[a];

    *x = (p->unk14 + p->unk0c) / 256;
    *y = (p->unk18 + p->unk10) / 256;
}
asm(".global sub_0801D9AC\n.thumb_set sub_0801D9AC, GetSpriteScriptDrawPosition\n");

/* GetSpriteScriptDrawPosition's offset-only half -- see the note there for the divide. */
void GetSpriteScriptPosition(int a, s16 *x, s16 *y)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[a];

    *x = p->unk14 / 256;
    *y = p->unk18 / 256;
}
asm(".global sub_0801D9E4\n.thumb_set sub_0801D9E4, GetSpriteScriptPosition\n");

/* GetSpriteScriptDrawPosition's position-only half, and the exact inverse of
 * src/decomp/c_0801D98C.c, which writes the same two members as `x << 8`. */
void GetSpriteScriptOffset(int a, s16 *x, s16 *y)
{
    struct Unk0200E438 *p = &gUnknown_0200E438[a];

    *x = p->unk0c / 256;
    *y = p->unk10 / 256;
}
asm(".global sub_0801DA14\n.thumb_set sub_0801DA14, GetSpriteScriptOffset\n");
