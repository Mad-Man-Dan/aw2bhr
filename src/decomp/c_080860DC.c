#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080860DC.
 * MapSelectList_HandleInput @ 0x080860DC
 */

#include "hardware.h"
#include "proc.h"
/* Input handler for the campaign-map list screen: up/down move the cursor,
 * A picks, B backs out, L/R page through the armies.
 *
 * The list is read as the struct member `gUnknown_02027F74.unk04[p->unk58]`,
 * not through `q = (u8 *)&gUnknown_02027F74; q += 4;`. The pointer local
 * tied gcse's shared address pseudo for &gUnknown_02027F74 to a register
 * preference for r4, and that preference reached gUnknown_02027F74.unk37 in
 * both clamp windows below (unk37 got r4 instead of r1, the array byte r1
 * instead of r3). With the member spelling, both L/R arms use the one plain
 * `p->unk58 = p->unk5c + gUnknown_03005980;`, as the ROM's identical windows
 * say they should.
 *
 * Load-bearing, from earlier waves:
 *  - `GetMapPreviewScrollY() + p->unk4e * 0x100`, not `<< 8`: the multiply keeps
 *    the signed HImode operand (`ldrsh`); the shift is narrowed to `ldrh`.
 *  - `*(u16 *)&p->unk4e = w;` with `w = 0xFFFF;` in an int local, in both
 *    arms, so the shared `strh` cross-jumps.
 *  - the `size = 2; MapSelectList_SetupWindows(); PlayMusicOrSfx2(0x67)` tail written out at
 *    the end of both of the first two arms, not once after the if-chain.
 * Promotion needs "rodata": ["0x081D9410", "0x081D9414", "0x081D9418",
 * "0x081D941C", "0x081D9420"]. */

struct Unk80860DCProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x18];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
};

void MapSelectList_HandleInput(ProcPtr procp)
{
    struct Unk80860DCProc *p = procp;
    int g;
    int w;

    if ((p->unk4e != 0 || p->unk52 != 0) && p->unk4c <= 7)
        return;

    if (p->unk2c <= 0x10)
        return;

    if (gpKeySt->repeated & 0x40)
    {
        if (p->unk58 <= gUnknown_02027F74.unk36)
            return;

        p->unk58--;

        if (gPlaySt.gameMode == 2)
        {
            RefreshMapSelectSidePanels(p->unk58);

            if (gUnknown_03005930 > 1 || p->unk5c == 0)
                gUnknown_03005930--;
            else
            {
                p->unk5c--;
                p->unk30 = 1;
            }

            gUnknown_03005900 = p->unk5c;
        }
        else
        {
            if (gUnknown_03005980 > 1 || p->unk5c == 0)
                gUnknown_03005980--;
            else
            {
                p->unk5c--;
                p->unk30 = 1;
            }

            gUnknown_03005990[gUnknown_0300596C] = p->unk5c;
        }

        if (p->unk4e != 0 || p->unk52 != 0)
        {
            ClearBg1Tilemap();
            BG_EnableSyncBG1();
            gUnknown_03002F18 = GetMapPreviewScrollY() + p->unk4e * 0x100;
            p->unk4c = 8;
            p->unk52 = 0;
        }
        else
            p->unk4c = 0;

        w = 0xFFFF;
        *(u16 *)&p->unk4e = w;
        gUnknown_03001FE8.bits.size = 2;
        MapSelectList_SetupWindows();
        PlayMusicOrSfx2(0x67);
        return;
    }
    else if (gpKeySt->repeated & 0x80)
    {
        if (p->unk58 >= gUnknown_02027F74.unk37)
            return;

        p->unk58++;

        if (gPlaySt.gameMode == 2)
        {
            RefreshMapSelectSidePanels(p->unk58);

            if (gUnknown_03005930 < (g = gUnknown_03005928) - 2
                || p->unk5c == gUnknown_02027F74.unk37 - g + 1)
                gUnknown_03005930++;
            else
            {
                p->unk5c++;
                p->unk30 = 1;
            }

            gUnknown_03005900 = p->unk5c;
        }
        else
        {
            if (gUnknown_03005980 < (g = gUnknown_03005928) - 2
                || p->unk5c == gUnknown_02027F74.unk37 - g + 1)
                gUnknown_03005980++;
            else
            {
                p->unk5c++;
                p->unk30 = 1;
            }

            gUnknown_03005990[gUnknown_0300596C] = p->unk5c;
        }

        if (p->unk4e != 0 || p->unk52 != 0)
        {
            ClearBg1Tilemap();
            BG_EnableSyncBG1();
            gUnknown_03002F18 = GetMapPreviewScrollY() + p->unk4e * 0x100;
            p->unk4c = 8;
            p->unk52 = 0;
        }
        else
            p->unk4c = 0;

        *(u16 *)&p->unk4e = 1;
        gUnknown_03001FE8.bits.size = 2;
        MapSelectList_SetupWindows();
        PlayMusicOrSfx2(0x67);
        return;
    }
    else if ((gpKeySt->pressed & 1) && p->unk4e == 0
             && Proc_Find(gUnknown_084892C4) == 0)
    {

        if (!(u8)sub_0803CA54(gUnknown_02027F74.unk04[p->unk58]))
            return;

        if (gPlaySt.gameMode == 2)
            gUnknown_03005900 = p->unk5c;
        else
            gUnknown_03005990[gUnknown_0300596C] = p->unk5c;

        SetMapId(gUnknown_02027F74.unk04[p->unk58]);
        sub_0803BD54();
        PlayMusicOrSfx2(0x71);
        RemoveMapPreviewPaletteHook();
        Proc_Start(gUnknown_08616CCC, p);
        p->unk4e = 1;
        p->unk4c = 0x1E;
        Proc_Break(p);
        return;
    }
    else if ((gpKeySt->pressed & 2) && Proc_Find(gUnknown_084892C4) == 0
             && Proc_Find(gUnknown_08616CCC) == 0)
    {
        PlayMusicOrSfx2(0x66);
        sub_0803BD60();
        RemoveMapPreviewPaletteHook();
        Proc_Start(gUnknown_08616CCC, p);
        p->unk4e = 1;
        p->unk4c = 0x1E;
        Proc_Break(p);
        return;
    }
    else if (gPlaySt.gameMode == 2)
    {
        if (!(gpKeySt->pressed & 0x100))
            return;

        gUnknown_0200C420.unk0c = 1 - gUnknown_0200C420.unk0c;

        if (gUnknown_0200C420.unk0c != 0)
            PlayMusicOrSfx2(0x65);
        else
            PlayMusicOrSfx2(0x66);

        ToggleMapRecordsPanel(p->unk58, p);
        return;
    }
    else if (gpKeySt->pressed & 0x220)
    {
        gUnknown_03005990[gUnknown_0300596C] = p->unk5c;

        gUnknown_0300596C--;
        if (gUnknown_0300596C <= 1)
            gUnknown_0300596C = 8;

        while (!(u8)BuildMapListForCategory(gUnknown_0300596C))
        {
            gUnknown_0300596C--;
            if (gUnknown_0300596C <= 1)
                gUnknown_0300596C = 8;
        }

        p->unk5c = gUnknown_03005990[gUnknown_0300596C];

        if (gUnknown_03005980 > gUnknown_02027F74.unk37)
            gUnknown_03005980 = gUnknown_02027F74.unk37;

        p->unk58 = p->unk5c + gUnknown_03005980;

        if (p->unk4e != 0 || p->unk52 != 0)
        {
            ClearBg1Tilemap();
            BG_EnableSyncBG1();
            gUnknown_03002B34 = GetMapPreviewScrollX() + (p->unk52 << 8);
            p->unk4c = 8;
            p->unk4e = 0;
        }
        else
            p->unk4c = 0;

        p->unk2c = 0;
        p->unk52 = 0xFFFF;
        gUnknown_03001FE8.bits.size = 1;
        MapSelectList_SetupWindows();
        PlayMusicOrSfx2(0x76);
        return;
    }
    else if (gpKeySt->pressed & 0x110)
    {
        gUnknown_03005990[gUnknown_0300596C] = p->unk5c;

        gUnknown_0300596C++;
        if (gUnknown_0300596C > 8)
            gUnknown_0300596C = 2;

        while (!(u8)BuildMapListForCategory(gUnknown_0300596C))
        {
            gUnknown_0300596C++;
            if (gUnknown_0300596C > 8)
                gUnknown_0300596C = 2;
        }

        p->unk5c = gUnknown_03005990[gUnknown_0300596C];

        if (gUnknown_03005980 > gUnknown_02027F74.unk37)
            gUnknown_03005980 = gUnknown_02027F74.unk37;

        p->unk58 = p->unk5c + gUnknown_03005980;

        if (p->unk4e != 0 || p->unk52 != 0)
        {
            ClearBg1Tilemap();
            BG_EnableSyncBG1();
            gUnknown_03002B34 = GetMapPreviewScrollX() + (p->unk52 << 8);
            p->unk4c = 8;
            p->unk4e = 0;
        }
        else
            p->unk4c = 0;

        p->unk2c = 0;
        p->unk52 = 1;
        gUnknown_03001FE8.bits.size = 1;
        MapSelectList_SetupWindows();
        PlayMusicOrSfx2(0x76);
    }
}
asm(".global sub_080860DC\n.thumb_set sub_080860DC, MapSelectList_HandleInput\n");
