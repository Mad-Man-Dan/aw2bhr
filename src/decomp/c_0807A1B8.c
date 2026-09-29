#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807A1B8.
 * sub_0807A1B8 @ 0x0807A1B8
 */

#include "proc.h"
#include "hardware.h"
struct Unk0807A1B8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4C);
    /* 4C */ s16 unk4c; /* `ldrsh` off a zero index register, and the guard
                         * `> 0x30` is a signed compare after `lsls #0x10;
                         * asrs #0x10`, so a SIGNED halfword. */
};

/* Four sprites in a row, then a fade-in counter that saturates at 0x30 and
 * gates the exit on A/START.
 *
 * The loop counts UP in the source: `i` is used only by the exit test, so
 * strength_reduce turns the two `i * 0x40` terms into their own accumulators
 * (r6, r5) and check_dbra_loop then reverses what is left into `subs; cmp #0;
 * bge`. The preheader's `movs r6,#0xa0; lsls #1` / `movs r5,#8; rsbs` are those
 * giv inits, not source.
 *
 * gUnknown_081D92F8 in the disassembly is NOT a global: the ROM word there
 * holds 0x0848B6A0, i.e. it is agbcc's own `-fforce-addr` address constant for
 * gUnknown_0848B6A0. Naming the array directly reproduces it, and the promotion
 * carries "rodata": ["0x081D92F8"].
 *
 * Only bg2_enable is cleared -- `movs r0,#5; rsbs` is the single mask ~4. */
void ResultsVersus_WaitForButton_Loop(struct Unk0807A1B8Proc *proc)
{
    int i;

    for (i = 0; i < 4; i++)
        PutSprite(0, (i * 0x40 - 8) & 0x1FF, 0x30, gUnknown_0848B6A0,
                  0x4000 | (0x140 + i * 0x40));

    proc->unk4c++;

    if (proc->unk4c > 0x30)
        proc->unk4c = 0x30;

    if ((gpKeySt->pressed & 9) && proc->unk4c == 0x30)
    {
        proc->unk4c = 0;
        gUnknown_03001400 = 0xFF10;
        gDispIo.disp_ct.bg2_enable = 0;
        DrawWindowBackgroundOnBg2(0, 0xE, 0x1E, 6);
        Proc_Break(proc);
    }
}
asm(".global sub_0807A1B8\n.thumb_set sub_0807A1B8, ResultsVersus_WaitForButton_Loop\n");
