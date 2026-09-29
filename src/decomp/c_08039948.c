#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039948.
 * sub_08039948 @ 0x08039948
 */

#include "proc.h"
/* Installs the palette for the proc's slot, then picks one of four (unk2c,
 * unk30) travel offsets from the slot's unk14 tag and kicks off the matching
 * scroll. Cases 0 and 1 share their tail call and agbcc cross-jumps them into
 * one `bl LoadCoPowerPanelTiles`; cases 2 and 3 keep their own, which is the compiler's
 * choice and not a source-level difference -- all four arms are written the
 * same way here.
 *
 * `str r1,[r5,#0x30]` in case 0 stores the switch value itself because agbcc
 * knows r1 is 0 on that edge; `proc->unk30 = 0;` is the right spelling. */
struct Unk39948Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x2c);
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ STRUCT_PAD(0x34, 0x54);
    /* 0x54 */ int unk54;
};

void CoPowerPanel_Init(struct Unk39948Proc *proc)
{
    int i;

    i = proc->unk54;

    ApplyPaletteExt(&gUnknown_080A36A8[i * 0x10], 0x100, 0x20);
    BG_EnableSyncBG0();

    switch (gUnknown_085D3DD0[i].unk14)
    {
    case 0:
        proc->unk2c = 0x18;
        proc->unk30 = 0;
        LoadCoPowerPanelTiles(0x2b0, 8);
        break;

    case 1:
        proc->unk2c = -0x18;
        proc->unk30 = 0;
        LoadCoPowerPanelTiles(0x2b0, 8);
        break;

    case 2:
        proc->unk2c = 0;
        proc->unk30 = 0x18;
        CoPowerPanelNoOp(0x2b0, 8);
        break;

    case 3:
        proc->unk2c = 0;
        proc->unk30 = -0x18;
        CoPowerPanelNoOp(0x2b0, 8);
        break;
    }
}
asm(".global sub_08039948\n.thumb_set sub_08039948, CoPowerPanel_Init\n");
