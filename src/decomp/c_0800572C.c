#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800572C.
 * sub_0800572C @ 0x0800572C, sub_080057EC @ 0x080057EC
 */

#include "hardware.h"

/* The three 0x0808D7xx "globals" asm/ names in this block are NOT globals: they
 * are agbcc's own -fforce-addr pool words (the 0x0808D6DC..0x0808D8A8 run
 * documented in include/unknown-globals.h). 0x0808D7B8 holds 0x0200B204 and
 * 0x0808D7BC/0x0808D7C0/0x0808D7C4/0x0808D7D0/0x0808D7D4 hold 0x0200B0B0 or
 * 0x0200B204 -- read straight out of baserom.gba. So the honest spelling is to
 * name gDesignRoomName / gActiveMap directly and let the build place
 * the pool word.
 *
 * The reload of the pool word after every `bl` (`ldr r4, [r6]`) is what
 * -fforce-addr does on its own; nothing in the source has to reproduce it.
 *
 * `lsls #0x1c; lsrs #0x1e` on the BGCNT shadow is bits 3:2, i.e.
 * gUnknown_03002B6C.bits.chr_block, and `<< 14` + 0x06000000 turns the char
 * base block into its VRAM address. */
void sub_0800572C(void)
{
    InitTilePool(0, (void *)(0x06000000 + (gUnknown_03002B6C.bits.chr_block << 14)), 0x2FC, 10);
    LoadTilePoolGraphic(9);
    if (LoadDesignRoomName(0, gDesignRoomName) != 1)
        sub_08004D74(0, 0);
    else
        sub_08004D90(0, 0, gDesignRoomName);
    if (LoadDesignRoomName(1, gDesignRoomName) != 1)
        sub_08004D74(0, 1);
    else
        sub_08004D90(0, 1, gDesignRoomName);
    if (LoadDesignRoomName(2, gDesignRoomName) != 1)
        sub_08004D74(0, 2);
    else
        sub_08004D90(0, 2, gDesignRoomName);
    gActiveMap->menuCursorX = 0x15;
    gActiveMap->menuCursorY = 0x20;
}

/* NO local for the slot id -- `(s8)gActiveMap->designSlot` is written TWICE,
 * once in the test and once in the value arm, and that is the whole difference
 * between the two forms of a signed-byte load. Bound to an `s8` (or `int`)
 * local the read is `ldrb; lsls #24; asrs #24`; read twice and CSEd it becomes
 * `movs rN,#0x10; ldrsb rN,[rM,rN]`, and the copy `adds r0,r1,#0` in front of
 * the `(u16)` shift pair appears with it. Same size either way, 20 of 76 bytes
 * different -- see the note added to docs/agbcc-codegen.md.
 *
 * unk10 stays `u8`: sub_0800520C hands it to LoadDesignRoomName's `int` parameter
 * with a bare `ldrb` and matches, and an `s8` member would give `ldrsb` there.
 * Declaring the member `s8` changes NOTHING here (probed with a struct cast),
 * so the cast is the source's and not the type's. */
void sub_080057EC(void)
{
    sub_08019F2C(gUnknown_08488494, 2, 4, 0,
                 (s8)gActiveMap->designSlot < 0 ? 0 : (s8)gActiveMap->designSlot);
    sub_0800572C();
    gActiveMap->state = 3;
}
