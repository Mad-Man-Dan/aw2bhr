#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013D4C.
 * sub_08013D4C @ 0x08013D4C, sub_08013D64 @ 0x08013D64
 */

struct Unk8013D4C
{
    u8 filler_00[0x28];
    u16 *unk28;
    u8 filler_2c[4];
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
};

u16 *TextWriterGetCursorTilemapPtr(struct Unk8013D4C *s)
{
    return s->unk28 + s->unk32 + s->unk33 * 32;
}
asm(".global sub_08013D4C\n.thumb_set sub_08013D4C, TextWriterGetCursorTilemapPtr\n");

u16 *TextWriterGetOriginTilemapPtr(struct Unk8013D4C *s)
{
    return s->unk28 + s->unk30 + s->unk31 * 32;
}
asm(".global sub_08013D64\n.thumb_set sub_08013D64, TextWriterGetOriginTilemapPtr\n");
