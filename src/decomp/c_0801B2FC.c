#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801B2FC.
 * sub_0801B2FC @ 0x0801B2FC
 */

/* Rebuilds the whole save-slot table: clears the 16 words at
 * gUnknown_0200CC88.slotGeneration, then for each of the 16 slots loads it with
 * ReadAndValidateSaveSector and records its id/size/flags, tracking the NEWEST valid slot
 * (`best`) by the u32 at buf+8 with buf[0xc] as the tie-break. The winner's
 * 16-word block at buf+0x10 is copied over gUnknown_0200CC88.slotGeneration and
 * gUnknown_0200CD08 gets its counter + 1.
 *
 * With no valid slot at all it calls sub_0801ADC8 (unless a1 is set) and
 * stops. Otherwise FindNewestCompleteSave(0) picks the active slot; 0xffff
 * means "none", which invalidates every entry. The final pass re-selects among the slots
 * flagged 0x10 by the smallest unk30 and reloads that one.
 *
 * WAVE 74: configured MATCH, 444/444 bytes. Reusing `j` for the initial clear
 * loop gives its countdown the ROM's r2 without disturbing `j`'s later uses.
 * Assigning `t = 0xff` inside the final loop preserves the ROM's hoisted mask,
 * and prebinding unk00 preserves the settled address allocation. The remaining
 * printed relocation names resolve to equal addresses. See NOTES.md.
 */

struct Unk1B2FCBuf
{
    /* 0x000 */ u8 filler_000[0x08];
    /* 0x008 */ u32 unk008;
    /* 0x00c */ u8 unk00c;
    /* 0x00d */ u8 unk00d;
    /* 0x00e */ u8 filler_00e[0x02];
    /* 0x010 */ u32 unk010[0x10];
    /* 0x050 */ u8 filler_050[0xf9f];
    /* 0xfef */ u8 unkfef[0x10];
    /* 0xfff */ u8 unkfff;
};
#define BUF ((struct Unk1B2FCBuf *)gUnknown_02002000)

void ScanSaveSectors(int a1)
{
    int i;
    int j;
    int best;
    u32 t;
    u8 *unk00;

    best = -1;

    for (j = 0; j < 0x10; j++)
        gUnknown_0200CC88.slotGeneration[j] = 0;

    gUnknown_0200CD08 = 0;

    for (i = 0; i < 0x10; i++)
    {
        unk00 = &gUnknown_0200CC38.unk00[i];
        gUnknown_0200CC38.unk10[i] = 0xff;
        gUnknown_0200CC38.unk00[i] = -1;
        gUnknown_0200CC38.unk20[i] = 0;

        if (ReadAndValidateSaveSector(i) == 0)
        {
            gUnknown_0200CC88.sectorGeneration[i] = BUF->unk008;
            gUnknown_0200CC38.unk10[i] = BUF->unk00d;
            *unk00 = BUF->unk00d;
            gUnknown_0200CC38.unk30[i] = BUF->unk00c;

            if (best == -1
                || (best > -1
                    && (gUnknown_0200CC88.sectorGeneration[best] < BUF->unk008
                        || (gUnknown_0200CC88.sectorGeneration[best] == BUF->unk008
                            && gUnknown_0200CC38.unk30[best] > BUF->unk00c))))
            {
                best = i;

                for (j = 0; j < 0x10; j++)
                    gUnknown_0200CC88.slotGeneration[j] = BUF->unk010[j];

                gUnknown_0200CD08 = BUF->unk008 + 1;
            }
        }

        gUnknown_0200CC38.unk40[i] = BUF->unkfff;
    }

    if (best == -1)
    {
        if (a1 == 0)
            sub_0801ADC8();

        return;
    }

    best = FindNewestCompleteSave(0);

    if (best == 0xffff)
    {
        for (i = 0; i < 0x10; i++)
        {
            gUnknown_0200CC38.unk10[i] |= 0xff;
            gUnknown_0200CC38.unk00[i] |= 0xff;
        }

        if (a1 == 0)
            sub_0801ADC8();

        return;
    }

    j = best;

    for (i = 0; i < 0x10; i++)
    {
        if (gUnknown_0200CC38.unk20[i] & 0x10)
        {
            if (gUnknown_0200CC38.unk30[i] < gUnknown_0200CC38.unk30[j])
                j = i;
        }
    }

    ReadAndValidateSaveSector(j);

    for (i = 0; i < 0x10; i++)
    {
        gUnknown_0200CC38.unk10[i] = BUF->unkfef[i];
        t = 0xff;
        gUnknown_0200CC38.unk00[i] = t & BUF->unkfef[i];
    }
}
asm(".global sub_0801B2FC\n.thumb_set sub_0801B2FC, ScanSaveSectors\n");
