#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039650.
 * sub_08039650 @ 0x08039650
 */

#include "proc.h"
struct Unk39650Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x54);
    /* 0x54 */ int unk54; /* an army index: it is what indexes
                           * gPlayers[] here and in CoPowerSequence_Activate */
    /* 0x58 */ int unk58;
};

/* `lsls #4; subs; lsls #2` is the x0x3c stride of struct PlayerStruct -- an
 * array index, not hand-rolled arithmetic.
 *
 * The proc is also StartCoPowerScript's THIRD argument, which is why the incoming
 * pointer is copied into r2 before anything else: r2 is the third argument
 * register. Drop that argument and agbcc keeps the base in r0, swaps r2 and r3
 * between the proc and the table base, and misses by five bytes in the same
 * 36. */

void CoPowerSequence_StartPowerScript(struct Unk39650Proc *proc)
{
    StartCoPowerScript(gPlayers[proc->unk54].co, proc->unk58, proc);
}
asm(".global sub_08039650\n.thumb_set sub_08039650, CoPowerSequence_StartPowerScript\n");
