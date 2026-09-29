#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074AAC.
 * sub_08074AAC @ 0x08074AAC
 */

#include "proc.h"
/* The first argument is a POINTER into ROM, not the `s32` this was promoted
 * with, and WorldMapReturn_RevealBonusMissions is decisive rather than merely suggestive: it walks
 * four 4-byte records based at gUnknown_08615974 and calls
 * `StartWorldMapMarkerReveal(&record[i][2], parent)` -- an INTERIOR address formed as
 * `gUnknown_08615974 + 2 + i*4`, which no integer parameter would be spelled
 * as. The other three callers agree: WorldMapReturn_RevealMissions passes a word loaded out of
 * gUnknown_0861500C, and WorldMapReturn_RevealMissions08To0A / WorldMapReturn_RevealMissions10To12 pass gUnknown_08615984 and
 * gUnknown_08615988 by name. Everything so passed is a 0xFF-terminated u8 id
 * list (checked against baserom.gba), hence `const u8 *`.
 * Byte-neutral -- the value is only `str`-ed -- and re-verified with trymatch. */
struct Unk8074AAC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ const u8 *unk_2c;
    /* 0x30 */ s32 unk_30;
};

void StartWorldMapMarkerReveal(const u8 *arg, ProcPtr parent)
{
    struct Unk8074AAC * proc;

    gUnknown_0202FDFC.unk10 = 0;

    proc = Proc_StartBlocking(gUnknown_086142E4, parent);

    proc->unk_2c = arg;
    proc->unk_30 = 0;
}
asm(".global sub_08074AAC\n.thumb_set sub_08074AAC, StartWorldMapMarkerReveal\n");
