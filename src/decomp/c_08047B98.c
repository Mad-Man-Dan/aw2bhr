#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08047B98.
 * sub_08047B98 @ 0x08047B98
 */

/* Repaint the whole panel for record `p`: refresh from p->unk1e, clear the two
 * text windows of *gBG0TilemapBuffer, blank the two halfwords at +0x6e and
 * +0xae, redraw the p->unk1f + 1 field and flush.
 *
 * gBG0TilemapBuffer is `u16 *`, so +0x6e and +0xae are elements 0x37 and 0x57;
 * both are past `strh`'s imm5 * 2 limit, which is what turns the second into
 * the `adds r0, #0x40` continuation of the first address rather than its own
 * pool word.
 *
 * The zero in r4 is one value doing four jobs -- both FillTilemapRect sixth
 * arguments, both `strh`s and DrawTallNumberRightAligned's sixth -- which is what pays for
 * r4 being callee-saved here. */

struct Unk08047B98
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 unk1f;
};

void RefreshUnitList(struct Unk08047B98 *p)
{
    sub_08047190(p, p->unk1e);
    FillTilemapRect(gBG0TilemapBuffer, 0xf, 1, 1, 2, 0);
    FillTilemapRect(gBG0TilemapBuffer, 1, 7, 0x1c, 0xc, 0);
    gBG0TilemapBuffer[0x37] = 0;
    gBG0TilemapBuffer[0x57] = 0;
    DrawTallNumberRightAligned(0x18, 1, gBG0TilemapBuffer, p->unk1f + 1, 0x8000, 0);
    DrawUnitListRows(p);
    BG_EnableSyncBG0();
}
asm(".global sub_08047B98\n.thumb_set sub_08047B98, RefreshUnitList\n");
