#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08011054.
 * sub_08011054 @ 0x08011054
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
struct Unk8011054Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

/* `gUnknown_030030E0.bits.effect = 3` is one bare `orrs #0xc0` and not the
 * read-mask-or a bitfield store usually costs: setting a field to ALL ONES
 * makes `(x & ~m) | m` fold to `x | m`. Same idiom as c_080039E4.c. */
void FadeToBlack_OnInit(struct Unk8011054Proc *proc)
{
    if (gUnknown_03002B5C == 1)
    {
        Proc_End(proc);
        return;
    }

    gUnknown_03002B5C = 1;
    InitFadeBlend();
    gUnknown_030030E0.bits.effect = 3;
    gUnknown_03001FFC = 0;
    proc->unk64 = 0x100;
    proc->unk66 = 0;
}

asm(".global sub_08011054\n.thumb_set sub_08011054, FadeToBlack_OnInit\n");
