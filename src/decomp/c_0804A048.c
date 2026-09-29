#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A048.
 * sub_0804A048 @ 0x0804A048, sub_0804A0A0 @ 0x0804A0A0
 */

#include "hardware.h"

/* The `movs r0,#0x7f; ands` is a BITFIELD clear of DISPCNT bit 7, not a plain
 * mask on a u8 -- gDispIo.disp_ct is struct DispCnt and forced_blank is that
 * bit. A 1-bit clear whose complement fits in an 8-bit immediate needs no
 * `mov #N; neg`, which is why this one lacks the usual bitfield-store shape. */
void sub_0804A048(void)
{
    ResetWindowShadows();
    SetupBackgrounds(gUnknown_0849D16C);
    EnableVBlankInterrupt();
    FlushLCDControl();
    gDispIo.disp_ct.forced_blank = 0;
    ClearBg0Tilemap();
    ClearBg1Tilemap();
    ClearBg2Tilemap();
    sub_080616F0();
    BG_EnableSyncBG0();
    BG_EnableSyncBG1();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    sub_080152EC(gUnknown_084C3814, 0);
}

/* A switch, not an if-chain: the `cmp #0x40; beq / cmp #0x40; bgt` pair is
 * gcc's binary dispatch over the three cases 1, 0x40 and 0x80. The two
 * gUnknown_02028E40 arms share the `& 3; strb` tail by cross-jumping, which is
 * why the 0x80 arm ends in a bare `b` into the middle of the 0x40 arm.
 *
 * gUnknown_0200C420.unk08 is left a plain u8 with explicit masks rather than
 * retyped as a bitfield container: MarkProfileSaved already reads bit 0 of the
 * same byte as a flag, and the mask spelling reproduces both this write and
 * sub_0804A124's `>> 6` read exactly. */
void LanguageSelect_Loop(void)
{
    switch (gpKeySt->pressed)
    {
    case 1:
        sub_0803B4DC(0x71);
        gUnknown_0200C420.unk08 = (gUnknown_0200C420.unk08 & 0x3f)
                                | (gUnknown_02028E40 << 6);
        ClearSlotScriptCallback(gUnknown_03001FBC);
        sub_0804A010();
        break;
    case 0x80:
        sub_0803B4DC(0x67);
        gUnknown_02028E40 = (gUnknown_02028E40 + 1) & 3;
        break;
    case 0x40:
        sub_0803B4DC(0x67);
        gUnknown_02028E40 = (gUnknown_02028E40 - 1) & 3;
        break;
    }
}
asm(".global sub_0804A0A0\n.thumb_set sub_0804A0A0, LanguageSelect_Loop\n");
