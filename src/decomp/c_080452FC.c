#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080452FC.
 * sub_080452FC @ 0x080452FC, sub_08045358 @ 0x08045358
 */

#include "proc.h"
/* The two coordinates are DECLARED LOCALS, not argument expressions: the ROM
 * narrows both to u8 before it materialises the flag, which is the order a
 * declaration list gives and not the order argument setup would. */
struct Unk452FC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u8 unk2c;
    /* 0x2d */ u8 filler_2d[0x0f];
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
};
#include "hardware.h"
struct Unk45358Proc
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u8 *unk4c;
    /* 0x50 */ u16 *unk50;
};

void CoPowerUnitSparkle_Loop(struct Unk452FC *proc)
{
    u8 x;
    u8 y;
    u8 flag;

    if (FindSlotScript((s32)gUnknown_0849A00C) == -1)
    {
        x = proc->unk3c;
        y = proc->unk40;
        flag = 0;

        if (gPlayers[proc->unk2c].coActivationMode == 2)
            flag = 1;

        StartUnitSparkleEffect(x, y, flag);
        Proc_Break(proc);
    }
}
asm(".global sub_080452FC\n.thumb_set sub_080452FC, CoPowerUnitSparkle_Loop\n");

/* `gUnknown_03002B6C.bits.chr_block * 0x4000` is the promoted c_08013C00.c
 * idiom; only the base differs (0x06005600 rather than 0x06000000). */
void CoPowerOverlay_Init(struct Unk45358Proc *proc)
{
    Decompress(gUnknown_08112704, (void *)(gUnknown_03002B6C.bits.chr_block * 0x4000 + 0x06005600));
    Decompress(proc->unk4c, gBG0TilemapBuffer);
    AddToHalfwords(gBG0TilemapBuffer, 0x800, 0x82b0);
    ApplyPaletteExt(proc->unk50, 0x100, 0x20);
    BG_EnableSyncBG0();
}
asm(".global sub_08045358\n.thumb_set sub_08045358, CoPowerOverlay_Init\n");
