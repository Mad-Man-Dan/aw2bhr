#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080398D0.
 * sub_080398D0 @ 0x080398D0
 */

#include "proc.h"
struct Unk398D0Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54;
};

/* One of six random lines for the army's terrain type. `lsls #6; adds; lsls #2`
 * is the 0x104 stride of struct CoData and `adds r4, #0x20` on the bare
 * table symbol -- kept live in r4 across the __umodsi3 call -- is the
 * member-array hoist for unk20[], the same idiom GetArmyCoPowerName uses for unk38.
 *
 * __umodsi3 rather than __modsi3 because GetNextRandomNumber returns u32. */

u8 ShowRandomCoPowerQuote(ProcPtr procPtr)
{
    struct Unk398D0Proc *proc = procPtr;

    ShowCoQuote(gUnknown_085D3DD0[gPlayers[proc->unk54].co]
                     .copQuote[GetNextRandomNumber() % 6]);

    return 0;
}
asm(".global sub_080398D0\n.thumb_set sub_080398D0, ShowRandomCoPowerQuote\n");
