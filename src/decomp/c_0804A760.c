#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A760.
 * sub_0804A760 @ 0x0804A760
 */

#include "hardware.h"
/*
 * NameEntry_HandleInput -- input handler for the on-screen character grid.
 *
 * The grid is 15 columns by 6 rows. gUnknown_030044E0 holds the cursor
 * (+0x1e column, +0x20 row), and GetNameEntryGridChar maps a cell index
 * (row * 15 + column) to the key under it: 0x23, 0x24, 0x25 and 0x40 are
 * command keys, anything else is a character.
 *
 *   1. Finish any pending mode in +0x63 (states 1-3).
 *   2. Start, A and B: confirm, enter a character, or delete one.
 *   3. The D-pad moves the cursor with wrap-around, skipping over the
 *      remaining cells of a multi-cell command key.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - The column at +0x1e is a halfword here, so it is read through
 *     struct Unk030044E0View (the same view as c_0804A260.c).
 *   - `unk65 + 1 + table[t]` is written in that order; the compiler folds the
 *     sum so the loads come out in the original's order.
 *   - The column increment is a `?:` but the decrement is an if/else.
 *   - The `unk66` mask is applied to a local in separate statements, which
 *     keeps the `& 0xff` the original has.
 *   - Three locals are pinned to registers; no unpinned spelling has been
 *     found that gives the same register choice.
 *   - The outer repeat is a labelled goto loop, as in the original.
 *   - gUnknown_030044E0 and gpKeySt are reached through this unit's own
 *     address words in .rodata (0x0812A284 and 0x0812A288).
 */

/* Second view of the object gUnknown_030044E0 points at, for the HALFWORD at
 * +0x1e that the shared struct spells `u8 unk1e` plus filler. Identical to
 * c_0804A260.c's, deliberately: that file documents why the shared model cannot
 * be retyped and why a bare `*(s16 *)&...` cast makes agbcc reload the global
 * pointer where a cast through a struct does not. */
struct Unk030044E0View
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x41 - 0x20];
    /* 0x41 */ u8 unk41[0x17];
};

void NameEntry_HandleInput(void)
{
    register int flag asm("r8");
    int t;
    register int u asm("r4");
    u16 i;
    u8 *dst;
    register int c asm("r3");
    struct Unk030044E0 **gp;
    int k;
    int z;
    struct Unk030044E0 **gq;
    struct Unk030044E0 **gr;

    flag = 0;
    t = GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);

    switch (gUnknown_030044E0->unk63)
    {
    case 1:
        gUnknown_030044E0->unk63 = flag;
        break;
    case 2:
        if (gUnknown_03002EE4 == 1)
        {
            gUnknown_030044E0->unk63 = flag;
            break;
        }
        dst = (u8 *)gUnknown_030044E0->unk58;
        TrimTrailingFullWidthSpaces(gUnknown_030044E0->unk2c);
        for (i = 0; i < gUnknown_030044E0->unk5f; i++)
            dst[i] = gUnknown_030044E0->unk2c[i];
        ClearSlotScriptCallback(gUnknown_03001FBC);
        return;
    case 3:
        if (gUnknown_03002EE4 != 1)
        {
            ClearSlotScriptCallback(gUnknown_03001FBC);
            return;
        }
        gUnknown_030044E0->unk63 = flag;
        break;
    }

    switch (gpKeySt->pressed & 0xf)
    {
    case 8:
        ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 0xe;
        gUnknown_030044E0->unk20 = 5;
        NameEntry_Confirm();
        return;
    case 1:
        switch (t)
        {
        case 0x23:
            NameEntry_Confirm();
            return;
        default:
            if (gUnknown_030044E0->unk65 + 1 + gUnknown_084C36E4[t] > gUnknown_030044E0->unk60
             || gUnknown_030044E0->unk5d > gUnknown_030044E0->unk5f - 1)
            {
                PlayMusicOrSfx2(0x68);
                return;
            }
            PlayMusicOrSfx2(0x65);
            NameEntry_TypeChar();
            RedrawNameEntryText(0);
            gUnknown_030044E0->unk5d++;
            return;
        case 0x40:
            k = gUnknown_030044E0->unk66 + 1;
            z = 0;
            k &= 0xff;
            k |= 0x80;
            gUnknown_030044E0->unk66 = k;
            gUnknown_030044E0->unk67 = z;
            return;
        case 0x24:
            PlayMusicOrSfx2(0x65);
            gUnknown_030044E0->unk63 = 3;
            RedrawNameEntryText(0);
            StartEventScript(gUnknown_084C3A5C);
            return;
        case 0x25:
            if (gUnknown_030044E0->unk5d != 0)
            {
                gUnknown_030044E0->unk5d--;
                NameEntry_DeleteChar();
                RedrawNameEntryText(0);
                PlayMusicOrSfx2(0x66);
                return;
            }
            PlayMusicOrSfx2(0x68);
            return;
        }
    case 2:
        if (gUnknown_030044E0->unk5d != 0)
        {
            gUnknown_030044E0->unk5d--;
            NameEntry_DeleteChar();
            RedrawNameEntryText(0);
            PlayMusicOrSfx2(0x66);
        }
        return;
    }

    if ((gpKeySt->held & 0xf0) == 0)
        return;

key_loop:
    {
        if (gpKeySt->repeated & 0x30)
        {
            u = GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);
            switch (u)
            {
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x40:
                break;
            default:
                u = 0;
                break;
            }

            do
            {
                if (gpKeySt->repeated & 0x10)
                {
                    gq = &gUnknown_030044E0;
                    ((struct Unk030044E0View *)(*gq))->unk1e = (((struct Unk030044E0View *)(*gq))->unk1e > 0xd) ? 0 : ((struct Unk030044E0View *)(*gq))->unk1e + 1;
                }
                else
                {
                    gq = &gUnknown_030044E0;
                    if (((struct Unk030044E0View *)(*gq))->unk1e == 0)
                        ((struct Unk030044E0View *)(*gq))->unk1e = 0xe;
                    else
                        ((struct Unk030044E0View *)(*gq))->unk1e = ((struct Unk030044E0View *)(*gq))->unk1e - 1;
                }
            }
            while (GetNameEntryGridChar((*(gp = &gUnknown_030044E0))->unk20 * 15
                + ((struct Unk030044E0View *)(*gp))->unk1e) == u);

            switch (GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15
                + gUnknown_030044E0->unk1e))
            {
            case 0x40:
                ((struct Unk030044E0View *)(*gp))->unk1e = 2;
                break;
            case 0x24:
                ((struct Unk030044E0View *)(*gp))->unk1e = 5;
                break;
            case 0x25:
                ((struct Unk030044E0View *)(*gp))->unk1e = 9;
                break;
            case 0x23:
                ((struct Unk030044E0View *)(*gp))->unk1e = 0xd;
                break;
            }
            flag = 1;
        }

        if (gpKeySt->repeated & 0xc0)
        {
            if (gpKeySt->repeated & 0x80)
                gUnknown_030044E0->unk20 = (gUnknown_030044E0->unk20 > 4)
                    ? 0 : gUnknown_030044E0->unk20 + 1;
            else if (gUnknown_030044E0->unk20 == 0)
                gUnknown_030044E0->unk20 = 5;
            else
                gUnknown_030044E0->unk20 = gUnknown_030044E0->unk20 - 1;
            flag = 1;
        }
    }

    if (gUnknown_030044E0->unk5c != 0)
        goto after_key_loop;
    if (GetNameEntryGridChar(gUnknown_030044E0->unk20 * 15
        + gUnknown_030044E0->unk1e) == 0x24)
        goto key_loop;

after_key_loop:

    if (flag == 1)
    {
        if (t == 0x40 || t == 0x24 || t == 0x25 || t == 0x23)
        {
            c = 0;
            if (t != 0x40)
            {
                c = 1;
                if (t != 0x24)
                {
                    c = 2;
                    if (t != 0x25)
                        c = 3;
                }
            }
            ApplyPaletteExt(gUnknown_0812B21C, (u16)((c + 0x10) * 0x20), 0x20);
        }
        gUnknown_030044E0->unk22 = 0x1e;
        gUnknown_030044E0->unk24 = 7;
        PlayMusicOrSfx2(0x67);
    }
}
asm(".global sub_0804A760\n.thumb_set sub_0804A760, NameEntry_HandleInput\n");
