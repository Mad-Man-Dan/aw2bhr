#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049360.
 * sub_08049360 @ 0x08049360
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"

/* ShopScreen_Loop @ 0x08049360, 1480 bytes, THUMB.
 *
 * The unit-list proc's per-frame state machine: a twelve-case switch on
 * gUnknown_084C30F8->unk834, ending in a common "redraw if the list is not
 * empty" tail.
 *
 * gUnknown_084C30F8 and gpKeySt are both reached through agbcc's own
 * -fforce-addr words (0x0812A164 holds 0x084C30F8, 0x0812A168 holds
 * 0x03002EE0 which IS gpKeySt), so naming the real symbols is what produces
 * the double load. gUnknown_0812A160 is NOT one of those: its ROM word is
 * 0x00590005 and the function hands the ADDRESS to sub_0808B6E8 as a copy
 * source, so it is a data template and must be named -- see W56-G.
 */
void ShopScreen_Loop(ProcPtr proc)
{
    u16 steps[2];
    int flag = 0;
    u16 i;

    sub_0808B6E8(steps, gUnknown_0812A160, 4);

    if (gUnknown_084C30F8->unk83c != 0)
    {
        if (ShopScreen_StepOffsetToZero() != 0)
            return;
        ShopScreen_StartMessage(gUnknown_084C30E8[0]);
        gUnknown_084C30F8->unk834 = 0xb;
    }

    switch (gUnknown_084C30F8->unk834)
    {
    case 0:
        if (gUnknown_084C30F8->unk836 != 0)
            ShopList_HandleInput();
        if (gUnknown_084C30F8->unk030 != 0)
            break;
        if (gUnknown_084C30F8->unk836 != 0)
        {
            if ((gpKeySt->pressed & 2) != 0)
                flag = 1;
        }
        else
        {
            if ((gpKeySt->unk0c & 2) != 0)
                ShopScreen_EndMessage();
            if (sub_08019260())
                break;
            flag = 1;
        }
        if (flag == 1)
        {
            gDispIo.disp_ct.win0_enable = 0;
            gUnknown_084C30F8->unk834 = 0xa;
            break;
        }
        if (gUnknown_084C30F8->unk836 == 0)
            return;
        if ((gpKeySt->pressed & 1) == 0)
            break;
        gUnknown_084C30F8->unk834++;
        break;

    case 1:
        if (ShopScreen_StepOffsetToZero() != 0)
            break;
        ShopScreen_StartMessage(
            gUnknown_0849EDB0[gUnknown_02028E1C[gUnknown_084C30F8->unk01e]]
                .unk14);
        gUnknown_084C30F8->unk834++;
        break;

    case 2:
        if ((gpKeySt->unk0c & 2) != 0)
        {
            ShopScreen_EndMessage();
            gUnknown_084C30F8->unk834 = 8;
            break;
        }
        if (sub_08019260())
            break;
        gUnknown_084C30F8->unk83b = 0;
        if (gUnknown_03002EE4 == 1)
        {
            ShopScreen_StartMessage(gUnknown_084C3028);
            gUnknown_084C30F8->unk834 = 7;
            break;
        }
        if (gUnknown_084C30F8->unk028
            < gUnknown_0849EDB0[gUnknown_02028E1C[gUnknown_084C30F8->unk01e]]
                  .unk04)
        {
            ShopScreen_StartMessage(gUnknown_084C3008);
            gUnknown_084C30F8->unk834 = 7;
            break;
        }
        ShopScreen_StartMessage(gUnknown_084C2FE8);
        gUnknown_084C30F8->unk834++;
        break;

    case 3:
        if (sub_08019260())
            break;
        gUnknown_084C30F8->unk83b = 0x2d;
        gUnknown_084C30F8->unk02c
            = gUnknown_084C30F8->unk028
            - gUnknown_0849EDB0[gUnknown_02028E1C[gUnknown_084C30F8->unk01e]]
                  .unk04;
        GrantShopItem(gUnknown_02028E1C[gUnknown_084C30F8->unk01e]);
        DrawShopItemRow(0,
                     (gUnknown_084C30F8->unk01e - gUnknown_084C30F8->unk020) * 2
                         + 7,
                     gBG0TilemapBuffer,
                     gUnknown_02028E1C[gUnknown_084C30F8->unk01e], 4);
        BG_EnableSyncBG0();
        gUnknown_084C30F8->unk834++;
        break;

    case 4:
        PlayMusicOrSfx2(0x6b);
        gUnknown_084C30F8->unk834++;
        /* fallthrough */
    case 5:
        if ((gGameClock & 1) == 0)
            break;
        gUnknown_084C30F8->unk028 -= steps
            [gUnknown_0849EDB0[gUnknown_02028E1C[gUnknown_084C30F8->unk01e]]
                     .unk04
                 > 999
                 ? 1
                 : 0];
        if (gUnknown_084C30F8->unk028 < gUnknown_084C30F8->unk02c)
        {
            gUnknown_084C30F8->unk028 = gUnknown_084C30F8->unk02c;
            TrySpendBattleMapPoints(
                gUnknown_0849EDB0[gUnknown_02028E1C[gUnknown_084C30F8->unk01e]]
                    .unk04);
            FillTilemapRect(gBG0TilemapBuffer, 7, 0xf, 0x17, 4, 0);
            PlayMusicOrSfx2(0x6c);
            gUnknown_084C30F8->unk834++;
        }
        gUnknown_084C30F8->unk835 = 1;
        BG_EnableSyncBG0();
        break;

    case 6:
        if (ShopScreen_StepOffsetToMinus38() != 0)
            break;
        if (gUnknown_084C30F8->unk83b != 0)
        {
            gUnknown_084C30F8->unk83b--;
            break;
        }
        for (i = gUnknown_084C30F8->unk01e; i < gUnknown_084C30F8->unk836; i++)
        {
            gUnknown_02028E1C[i] = gUnknown_02028E1C[i + 1];
            if (gUnknown_02028E1C[i + 1] == 0xff)
            {
                gUnknown_084C30F8->unk836--;
                if (gUnknown_084C30F8->unk020 != 0)
                {
                    if (gUnknown_084C30F8->unk836 - gUnknown_084C30F8->unk020
                        <= 2)
                    {
                        gUnknown_084C30F8->unk020--;
                        if (gUnknown_084C30F8->unk01e
                            != gUnknown_084C30F8->unk836)
                            gUnknown_084C30F8->unk01e--;
                    }
                }
                if (gUnknown_084C30F8->unk01e >= gUnknown_084C30F8->unk836)
                    gUnknown_084C30F8->unk01e--;
                gUnknown_084C30F8->unk030 = 1;
                ShopList_StepScroll();
                gUnknown_084C30F8->unk834++;
                break;
            }
        }
        if (gUnknown_084C30F8->unk836 == 0)
            gUnknown_084C30F8->unk834 = 9;
        break;

    case 7:
        if ((gpKeySt->unk0c & 2) != 0)
            ShopScreen_EndMessage();
        if (sub_08019260())
            break;
        gUnknown_084C30F8->unk834 = 8;
        break;

    case 8:
        if (ShopScreen_StepOffsetToMinus38() != 0)
            break;
        RedrawSelectedShopRow();
        gUnknown_084C30F8->unk834 = 0;
        break;

    case 9:
        if (ShopScreen_StepOffsetToZero() != 0)
            break;
        ShopScreen_StartMessage(gUnknown_084C3048);
        gUnknown_084C30F8->unk834 = 0;
        break;

    case 10:
        if (gUnknown_084C30F8->unk836 == gUnknown_084C30F8->unk837)
        {
            if (ShopScreen_StepOffsetToZero() != 0)
                break;
            gUnknown_084C30F8->unk83a = gGameClock & 3;
            ShopScreen_StartMessage(gUnknown_084C30E8[gUnknown_084C30F8->unk83a]);
        }
        gUnknown_084C30F8->unk834 = 0xb;
        break;

    case 11:
        if (gUnknown_084C30F8->unk838 != 0)
            sub_080152C0((s32)gUnknown_084C3118, 0);
        Proc_Break(proc);
        return;
    }

    if (gUnknown_084C30F8->unk836 != 0)
        ShopScreen_DrawCursorSprites(2,
                     (gUnknown_084C30F8->unk01e - gUnknown_084C30F8->unk020)
                             * 16
                         + 0x39);
}

asm(".global sub_08049360\n.thumb_set sub_08049360, ShopScreen_Loop\n");
