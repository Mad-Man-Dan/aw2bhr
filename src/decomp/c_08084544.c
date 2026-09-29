#include "global.h"
#include "proc.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08084544.
 * sub_08084544 @ 0x08084544
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

struct Unk8084544
{
    u8 filler_00[0x48];
    u16 unk48;
};

void MainMenuCarouselBg_Loop(struct Unk8084544 *proc)
{
    if ((proc->unk48 & 1) == 0)
    {
        gUnknown_0300200C++;
        gUnknown_03002000--;
    }

    gUnknown_03001FF8++;
    proc->unk48++;
}

asm(".global sub_08084544\n.thumb_set sub_08084544, MainMenuCarouselBg_Loop\n");

struct ProcCmd CONST_DATA ProcScr_MainMenuC4[] =
{
    PROC_REPEAT(MainMenuCarouselBg_Loop),
    PROC_END,
};

asm(".global gUnknown_08616A58\n.set gUnknown_08616A58, ProcScr_MainMenuC4\n");
