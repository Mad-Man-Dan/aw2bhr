#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F2F4.
 * sub_0803F2F4 @ 0x0803F2F4
 */

/* DeathRayFire_StartBeam's twin: same three calls over a different blob/palette pair,
 * a sound cue in the middle, and different wrap offsets and object id. */
struct UnkF2F4Sub
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ void *unk24;
};
struct UnkF2F4Ret
{
    /* 0x00 */ u8 filler_00[0x50];
    /* 0x50 */ struct UnkF2F4Sub *unk50;
};
struct UnkF2F4Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
};

void VolcanoFire_StartEruption(struct UnkF2F4Proc *proc)
{
    Decompress(gUnknown_08115A78, gUnknown_0200FC50);
    ApplyPaletteExt(gUnknown_081169B0, 0x2A0, 0x20);
    PlayMusicOrSfx2(0x1D8);
    ((struct UnkF2F4Ret *)APProc_Create(gUnknown_081161CC,
                 (proc->unk2c * 16 - gMap->scrollX + 0x20) & 0x1FF,
                 (proc->unk30 * 16 - gMap->scrollY + 8) & 0xFF,
                 0x51CA, 0, 1))->unk50->unk24 = gUnknown_0200FC50;
}
asm(".global sub_0803F2F4\n.thumb_set sub_0803F2F4, VolcanoFire_StartEruption\n");
