#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077B74.
 * sub_08077B74 @ 0x08077B74
 */

#include "proc.h"
#include "hardware.h"
/* Wave 35 (W35-B). An Interpolate-driven curtain: `t` sweeps 4..0x1e over the
 * five frames and the visible width is `0x1e - t` clamped to the proc's own
 * +0x4c. `gUnknown_030030E0.bits.effect = 3` is the all-ones `orr #0xc0` with no
 * AND, the form unknown-globals.h already records for that field.
 *
 * The clamp's LIMIT is bound to its own local: the ROM issues the +0x4c `ldrsh`
 * BEFORE it computes `0x1e - t`, which `if (n > proc->unk4c)` cannot produce --
 * that spelling computes n first and loads the field at the compare. */
struct Unk77B74Proc
{
    /* 00 */ u8 filler_00[0x44];
    /* 44 */ int unk44;
    /* 48 */ s16 unk48;
    /* 4a */ s16 unk4a;
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
};

void WorldMapMapPreview_OpenLoop(struct Unk77B74Proc *proc)
{
    int t;
    int n;
    int m;
    int zero;

    t = Interpolate(4, 0x1E, proc->unk48, proc->unk44, 5);

    m = proc->unk4c;
    n = 0x1E - t;

    if (n > m)
        n = m;

    SetBgScrollShadow(0, 0, 0);

    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = proc->unk44;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x1D;

    WorldMapMissionInfo_PutSprites(0, 0xA8 - gUnknown_0300064C);

    zero = 0;
    CpuFastSet(&zero, gBG1TilemapBuffer, 0x01000140);

    sub_08071900(gUnknown_08551A04 + 0x100,
                 gBG1TilemapBuffer + (proc->unk4a * 32 + t), n, proc->unk4e);
    BG_EnableSyncBG1();

    if (proc->unk44 > 4)
    {
        proc->unk44 = 0;
        PlayMusicOrSfx2(0x67);
        Proc_Break(proc);
    }
    else
    {
        proc->unk44++;
    }
}
asm(".global sub_08077B74\n.thumb_set sub_08077B74, WorldMapMapPreview_OpenLoop\n");
