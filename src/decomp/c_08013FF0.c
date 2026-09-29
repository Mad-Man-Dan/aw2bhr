#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013FF0.
 * sub_08013FF0 @ 0x08013FF0
 */

/* unk1e is SIGNED: the zero test is `ldrsh`, which a u16 would not produce.
 * The decrement reads it back `ldrh` because the store truncates anyway.
 * No global here, so the parameter's type stays local to this file.
 */
struct Unk8013FF0
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u8 filler_0c[0x12];
    /* 0x1e */ s16 unk1e;
};

void TextPause_Loop(struct Unk8013FF0 *s)
{
    if (s->unk1e == 0)
        s->unk08 = 0;

    s->unk1e--;
}
asm(".global sub_08013FF0\n.thumb_set sub_08013FF0, TextPause_Loop\n");
