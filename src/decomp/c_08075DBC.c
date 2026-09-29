#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075DBC.
 * sub_08075DBC @ 0x08075DBC
 */

#include "proc.h"
/* The gUnknown_081D1504 row index is `unk64 - 6` scaled by 0x10 HALFWORDS: the
 * ROM's `lsls #5` is the 0x20-byte stride of that table seen through
 * CopyToPaletteBufferNoSync's `u16 *` parameter, not a byte offset.
 *
 * Both +0x4c and +0x64 are reached by materialising the address first (`adds
 * r0, r4, #0; adds r0, #0x64`) because THUMB's `strh`/`ldrsh` immediate forms
 * do not reach that far -- `strh` stops at 0x3e and `ldrsh` has no immediate
 * form at all. Neither is a separate object; both are ordinary fields. */
struct Unk8075DBC
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
    /* 0x4e */ u8 filler_4e[0xe];
    /* 0x5c */ int unk5c;
    /* 0x60 */ u8 filler_60[0x4];
    /* 0x64 */ s16 unk64;
};

void WorldMapMissionClear_RestoreInit(struct Unk8075DBC *proc)
{
    if (proc->unk5c == 0)
        Proc_End(proc);

    CopyToPaletteBufferNoSync(gUnknown_081D1504 + (proc->unk64 - 6) * 0x10, 0x1E0, 0x20);

    proc->unk4c = 0;

    ColorFadeInit();
    LoadFadeComponentsFromPalette(0xF, -1);
    EnablePaletteSync();
}
asm(".global sub_08075DBC\n.thumb_set sub_08075DBC, WorldMapMissionClear_RestoreInit\n");
