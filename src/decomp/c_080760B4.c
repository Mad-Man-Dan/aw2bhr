#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080760B4.
 * sub_080760B4 @ 0x080760B4
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* The VRAM address handed to InitTilePool is built from the live BG control
 * shadow: `lsls #0x1c; lsrs #0x1e` is the 2-bit field at bit 2 -- chr_block --
 * read out of a 32-bit `ldr`, which is the width hardware.h records this shadow
 * being used at for bitfield access. `lsls #0xe` then scales it by the 0x4000
 * char-block stride and `movs #0xc0; lsls #0x13` is the 0x6000000 VRAM base.
 *
 * +0x3a is reached by materialising its address (`adds r2, r4, #0; adds r2,
 * #0x3a`) because THUMB's `strb` immediate form stops at 0x1f, not because it
 * is a separate object. The single `movs r1, #0` serves both word stores. */
struct Unk80760B4
{
    /* 0x00 */ u8 filler_00[0x3a];
    /* 0x3a */ s8 unk3a;
    /* 0x3b */ u8 filler_3b[0x1];
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
};

void WorldMapNationPanel_Init(struct Unk80760B4 *proc)
{
    sub_0801F114();
    InitTilePool(1,
                 (void *)(0x6000000 + gUnknown_03002B6C.bits.chr_block * 0x4000),
                 0x29, 1);
    LoadTilePoolGraphic(0x3E);
    LoadTilePoolGraphic(0x3F);
    LoadTilePoolGraphic(0x40);
    LoadTilePoolGraphic(0x41);
    LoadTilePoolGraphic(0x42);

    proc->unk40 = 0;
    proc->unk3a = 1;
    proc->unk3c = 0;
}

asm(".global sub_080760B4\n.thumb_set sub_080760B4, WorldMapNationPanel_Init\n");

extern struct ProcCmd WM_Listener_WHILE_EXISTS_08614314[];
extern void WorldMapNationPanel_Setup(void);
extern void WorldMapNationPanel_SlideInLoop(void);
extern void WorldMapNationPanel_WatchLoop(void);
extern void WorldMapNationPanel_SlideOutLoop(void);

struct ProcCmd CONST_DATA ProcScr_WM_Listener[] =
{
    PROC_2A,
    PROC_2A,
    PROC_YIELD,
    PROC_CALL(WorldMapNationPanel_Init),
PROC_LABEL(0),
    PROC_WHILE_EXISTS(WM_Listener_WHILE_EXISTS_08614314),
    PROC_CALL(WorldMapNationPanel_Setup),
    PROC_REPEAT(WorldMapNationPanel_SlideInLoop),
    PROC_REPEAT(WorldMapNationPanel_WatchLoop),
    PROC_REPEAT(WorldMapNationPanel_SlideOutLoop),
    PROC_GOTO(0),
    PROC_END,
};

asm(".global gUnknown_08614460\n.set gUnknown_08614460, ProcScr_WM_Listener\n");
