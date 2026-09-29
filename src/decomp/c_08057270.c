#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08057270.
 * sub_08057270 @ 0x08057270
 */

/* SetUpBattleAnimDisplay @ 0x08057270 */

#include "hardware.h"

/* The three globals reached through a `-fforce-addr` .rodata address constant
 * are gDispIo (0x030030CC), gUnknown_03004504 (0x03004504) and
 * gUnknown_0300200C (0x0300200C) -- confirmed by dereferencing the ROM words at
 * 0x08136164 / 0x08136168 / 0x0813616C, which hold exactly those addresses. The
 * honest spelling is used throughout and agbcc parks all three in this unit's
 * own .rodata, so promotion must carry the three rodata entries.
 *
 * The three shadow writes at the top all need the SCALAR cast rather than
 * `.raw`: the union member store drops the `adds r0, rN, #0` copy that the ROM
 * has on each of them (hardware.h's own note on BgCntBuf predicts this).
 *
 * gUnknown_03004504's byte is loaded ONCE for the bit0/bit7 pair and then
 * RELOADED after ResetWindowShadows for the bit0/bit2/bit3 tree, which is the
 * ordinary "non-const memory is dropped at every bl" reload and not a volatile
 * tell. The two `orrs` per byte of gUnknown_030030A4 are separate 1-bit field
 * writes; a scalar `|= 3` would fold to one.
 */
void SetUpBattleAnimDisplay(void)
{
    *(u16 *)&gDispIo.disp_ct = 0x7F60;
    *(u16 *)&gUnknown_03002B6C = 0x400;
    *(u16 *)&gUnknown_03001FE8 = 0x505;
    SetBg2Bg3ControlBySide(gUnknown_0300450C);

    if (!gUnknown_03004504.bit0)
    {
        gUnknown_03001FF8 = 0;
        gUnknown_03001418 = 0;
        gUnknown_03002B34 = 0;
        gUnknown_03002F18 = 0;
        gUnknown_030030A0 = 0;
        gUnknown_03002000 = 0;
    }
    else if (!gUnknown_03004504.bit7)
    {
        gUnknown_030030A0 = 0xFFF8;
        gUnknown_0300200C = 0xFFF8;
    }

    gUnknown_03001400 = 0;
    gUnknown_0300200C = 0;

    ResetWindowShadows();

    gDispIo.disp_ct.win0_enable = 1;
    gDispIo.disp_ct.win1_enable = 1;

    gUnknown_03002B40 = 0;
    gUnknown_03002B4C = 0x77;
    gUnknown_03002EFC = 0;
    gUnknown_03002B44 = 0xA0;
    gUnknown_03002B68 = 0x79;
    gUnknown_030024E4 = 0xF0;
    gUnknown_03002B30 = 0;
    gUnknown_030020B8 = 0xA0;

    gUnknown_030030A4.bits.win0_enable_bg0 = 1;
    gUnknown_030030A4.bits.win0_enable_bg1 = 1;
    gUnknown_030030A4.bits.win1_enable_bg0 = 1;
    gUnknown_030030A4.bits.win1_enable_bg1 = 1;

    if (gUnknown_03004504.bit0)
    {
        if (gUnknown_03004504.bit2)
        {
            if (gUnknown_03004504.bit3)
            {
                gUnknown_030030A4.bits.win0_enable_bg2 = 1;
                gUnknown_030030A4.bits.win0_enable_obj = 1;
                gUnknown_030030A4.bits.win0_enable_blend = 1;
            }
            else
            {
                gUnknown_030030A4.bits.win1_enable_bg3 = 1;
                gUnknown_030030A4.bits.win1_enable_obj = 1;
                gUnknown_030030A4.bits.win1_enable_blend = 1;
            }
        }
        else
        {
            EnableBattleAnimWindowLayers();
        }
    }
    else
    {
        EnableBattleAnimWindowLayers();
    }

    FlushLCDControl();
}
asm(".global sub_08057270\n.thumb_set sub_08057270, SetUpBattleAnimDisplay\n");
