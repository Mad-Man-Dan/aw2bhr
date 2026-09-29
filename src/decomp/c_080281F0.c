#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080281F0.
 * DebugSettingsMenu_Loop @ 0x080281F0
 */

/* THIS IS A MATCH pending `.rodata` placement -- read the addends before
 * believing `matched: false` (wave 36, W36-M).
 * 500/500 bytes, SIZE MATCH, 98.4% identical, and every single instruction is
 * the ROM's. All 8 differing bytes are relocation ADDENDS, and they line up
 * exactly with the ROM's own `.rodata` layout offset from 0x08090AC4:
 *      .rodata+0x00 -> gUnknown_08090AC4   the { "OFF", "ON" } table
 *      .rodata+0x10 -> gUnknown_08090AD4   "O"
 *      .rodata+0x14 -> gUnknown_08090AD8   " MAP:%02d"
 *      .rodata+0x20 -> gUnknown_08090AE4   "SNOW:%s"
 *      .rodata+0x28 -> gUnknown_08090AEC   "SAKU:%s"
 *      .rodata+0x30 -> gUnknown_08090AF4   &gpKeySt        (force-addr)
 *      .rodata+0x34 -> gUnknown_08090AF8   &gUnknown_03001FBC
 *      .rodata+0x38 -> gUnknown_08090AFC   &gUnknown_03001470
 *      .rodata+0x3c -> gUnknown_08090B00   &gPlaySt
 * `trymatch` resolves a force-addr word automatically (it reads the word's value
 * out of baserom.gba) but cannot do that for a string, which has no address to
 * check -- hence the false negative. Promotion needs the whole run
 * 0x08090AC4-0x08090B03 carved as this function's rodata.
 * SETTLED: the two `switch`es really are switches (gcc's balanced tree,
 * `cmp #1/beq; cmp #1/bgt; cmp #0/beq; b default`), gUnknown_08499F4C is a
 * `const u16 []` of six UNSIGNED bounds read in lower/upper pairs, and
 * gUnknown_03001FBC takes an ORDINARY inline pool word in the first arm (one
 * reference) and the force-addr word everywhere else (many) -- one global, two
 * codegens, one honest spelling. */
#include "hardware.h"
static const char *const sNames[] = { "OFF", "ON" };

void DebugSettingsMenu_Loop(void)
{
    struct Unk03001470 *ent;
    int v;

    if (gpKeySt->pressed & 9)
    {
        EndSlotScriptAt(gUnknown_03001FBC);
        return;
    }

    if (gpKeySt->pressed & DPAD_UP)
    {
        ent = &gUnknown_03001470[gUnknown_03001FBC];
        v = (u16)ent->unk1e;

        if (ent->unk1e > 0)
            ent->unk1e = v - 1;
    }

    if (gpKeySt->pressed & DPAD_DOWN)
    {
        ent = &gUnknown_03001470[gUnknown_03001FBC];
        v = (u16)ent->unk1e;

        if (ent->unk1e <= 1)
            ent->unk1e = v + 1;
    }

    if (gpKeySt->repeated & DPAD_LEFT)
    {
        switch (gUnknown_03001470[gUnknown_03001FBC].unk1e)
        {
        case 0:
            if (gUnknown_08499F4C[0] < gPlaySt.mapID)
                gPlaySt.mapID--;
            break;

        case 1:
            if (gUnknown_08499F4C[2] < gPlaySt.weather)
                gPlaySt.weather--;
            break;

        case 2:
            if (gUnknown_08499F4C[4] < gPlaySt.fog)
                gPlaySt.fog--;
            break;
        }
    }

    if (gpKeySt->repeated & DPAD_RIGHT)
    {
        switch (gUnknown_03001470[gUnknown_03001FBC].unk1e)
        {
        case 0:
            if (gUnknown_08499F4C[1] > gPlaySt.mapID)
                gPlaySt.mapID++;
            break;

        case 1:
            if (gUnknown_08499F4C[3] > gPlaySt.weather)
                gPlaySt.weather++;
            break;

        case 2:
            if (gUnknown_08499F4C[5] > gPlaySt.fog)
                gPlaySt.fog++;
            break;
        }
    }

    DebugPrintf(8, (s16)(gUnknown_03001470[gUnknown_03001FBC].unk1e * 2 + 14), "O");
    DebugPrintf(10, 14, " MAP:%02d", gPlaySt.mapID);
    DebugPrintf(10, 16, "SNOW:%s", sNames[gPlaySt.weather]);
    DebugPrintf(10, 18, "SAKU:%s", sNames[gPlaySt.fog]);
}

asm(".global sub_080281F0\n.thumb_set sub_080281F0, DebugSettingsMenu_Loop\n");
