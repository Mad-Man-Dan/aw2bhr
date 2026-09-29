#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080445A8.
 * sub_080445A8 @ 0x080445A8
 */

#include "proc.h"
/* Ends the proc when the current army's entry has no +0x04 payload, otherwise
 * plays its cue and arms the two proc counters. */
struct Unk080445A8Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ u16 unk68;
};

void CoPowerUnitEffects_Init(struct Unk080445A8Proc *proc)
{
    ClearObjAffineSlots();

    if (gUnknown_084A0090[gPlayers[gUnknown_030033EC].co]
            .power[gPlayers[gUnknown_030033EC].coActivationMode - 1].animationCondition == NULL)
    {
        Proc_End(proc);
    }
    else
    {
        StartCoPowerAnimation(gUnknown_030033EC);
        proc->unk68 = 1;
        proc->unk64 = 0;
    }
}
asm(".global sub_080445A8\n.thumb_set sub_080445A8, CoPowerUnitEffects_Init\n");
