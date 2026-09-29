#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AFCC.
 * sub_0804AFCC @ 0x0804AFCC
 */

/* gUnknown_0812A2A8 is NOT an object: the word at 0x0812A2A8 holds 0x030030A0,
 * so it is agbcc's -fforce-addr address constant for the u16 gUnknown_030030A0.
 * Declared `u16 *` it cost one indirection per read; that declaration has been
 * retired from include/unknown-globals.h.
 *
 * unk2a is read BOTH ways in one statement -- `ldrsh` for the `> 0xa0` test and
 * `ldrh` for the `- 4` stored straight back -- which is what proves it an s16
 * object rather than a u16 one. */
void NameEntry_SlideOut_Step(void)
{
    int t;

    gUnknown_03001FF8 -= 10;
    gUnknown_030030A0 -= 10;

    if (gUnknown_030044E0->unk2a > 0xa0)
        gUnknown_030044E0->unk2a = gUnknown_030044E0->unk2a - 4;
    else
        gUnknown_030044E0->unk2a = 0xa0;

    t = ((gUnknown_030030A0 - 0x138) >> 3) + 5;
    t &= 0x1f;

    FillTilemapRect(gBG0TilemapBuffer, t, 0, 3, 0x14, 0);
    FillTilemapRect(gBG2TilemapBuffer, t, 0, 3, 0x14, 0x360);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();

    if (t == 0)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}
asm(".global sub_0804AFCC\n.thumb_set sub_0804AFCC, NameEntry_SlideOut_Step\n");
