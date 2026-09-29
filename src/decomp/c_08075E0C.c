#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075E0C.
 * sub_08075E0C @ 0x08075E0C, sub_08075E3C @ 0x08075E3C
 */

#include "proc.h"
struct Unk08075E0C
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ s16 unk4c;
};
struct Unk08075E3C
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ int unk54;
    /* 0x58 */ int unk58;
};

void WorldMapMissionClear_FadeInLoop(struct Unk08075E0C *proc)
{
    StepBank15WhiteFade(proc->unk4c, 0x10);
    EnablePaletteSync();

    proc->unk4c++;

    if (proc->unk4c > 0x10)
        Proc_Break(proc);
}
asm(".global sub_08075E0C\n.thumb_set sub_08075E0C, WorldMapMissionClear_FadeInLoop\n");

void WorldMapMissionClear_OnEnd(struct Unk08075E3C *proc)
{
    ColorWorldMapSection(proc->unk58);
    RegisterDataMove(gUnknown_08614280, (void *)0x0600F000, 0x1000);
    AP_Delete(proc->unk54);
}
asm(".global sub_08075E3C\n.thumb_set sub_08075E3C, WorldMapMissionClear_OnEnd\n");
