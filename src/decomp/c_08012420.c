#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08012420.
 * sub_08012420 @ 0x08012420
 */

#include "hardware.h"

/* The VBlank flush: every display register that has an IWRAM shadow, pushed to
 * the hardware in one straight line. 364 bytes, no branches, no calls.
 *
 * The statement ORDER here is the ROM's, and it is readable straight off the
 * absolute-address arithmetic rather than guessed: MOSAIC is loaded as a pool
 * word and the four BGxCNT writes come off it with `subs r1, #0x44` and three
 * `adds r1, #2`, which only happens if MOSAIC is written first. Every store is
 * to a volatile lvalue, so agbcc may not reorder them.
 *
 * Six of the source globals are volatile, and both halves of that were found
 * here rather than assumed -- see the notes in hardware.h and on
 * gUnknown_03002B20 in unknown-globals.h. Without them this comes out 16 bytes
 * short: the eight scroll writes share one load of each origin instead of
 * reloading it, and the last two register addresses get CSEd against the
 * leftover BGxOFS cursor instead of being loaded as fresh pool words.
 */

void FlushDisplayRegisters(void)
{
    REG_DISPCNT = *(u16 *)&gDispIo.disp_ct;
    REG_DISPSTAT = gUnknown_030020B4.raw;
    REG_BG0HOFS = gUnknown_03001FF8 - gUnknown_030030D0;
    REG_BG0VOFS = gUnknown_03001418 - gUnknown_03002B20;
    REG_BG1HOFS = gUnknown_03002B34 - gUnknown_030030D0;
    REG_BG1VOFS = gUnknown_03002F18 - gUnknown_03002B20;
    REG_BG2HOFS = gUnknown_030030A0 - gUnknown_030030D0;
    REG_BG2VOFS = gUnknown_03001400 - gUnknown_03002B20;
    REG_BG3HOFS = gUnknown_0300200C - gUnknown_030030D0;
    REG_BG3VOFS = gUnknown_03002000 - gUnknown_03002B20;
    REG_MOSAIC = gUnknown_030030C4;
    REG_BG0CNT = gUnknown_03002B6C.raw;
    REG_BG1CNT = gUnknown_03001FE8.raw;
    REG_BG2CNT = gUnknown_030030B4.raw;
    REG_BG3CNT = gUnknown_0300251C.raw;
    REG_BLDCNT = gUnknown_030030E0.raw;
    REG_BLDALPHA = gUnknown_03002020 + (gUnknown_03002B28 << 8);
    REG_BLDY = gUnknown_03001FFC;
    REG_BG2AFFIN(0) = gUnknown_030024D0[0];
    REG_BG2AFFIN(1) = gUnknown_030024D0[1];
    REG_BG2AFFIN(2) = gUnknown_030024D0[2];
    REG_BG2AFFIN(3) = gUnknown_030024D0[3];
    REG_BG3AFFIN(0) = gUnknown_03003020[0];
    REG_BG3AFFIN(1) = gUnknown_03003020[1];
    REG_BG3AFFIN(2) = gUnknown_03003020[2];
    REG_BG3AFFIN(3) = gUnknown_03003020[3];
}
asm(".global sub_08012420\n.thumb_set sub_08012420, FlushDisplayRegisters\n");
