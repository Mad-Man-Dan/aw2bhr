#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08041820.
 * sub_08041820 @ 0x08041820
 */

#include "proc.h"
struct Unk41820Proc
{
    /* 00 */ u8 filler_00[0x4c];
    /* 4c */ void *unk4c;
    /* 50 */ u8 filler_50[0x04];
    /* 54 */ void *unk54;
    /* 58 */ u8 filler_58[0x0c];
    /* 64 */ s16 unk64;
};

void StartAttackOnInventionAt(int a1, int a2, int a3)
{
    u8 c = a3;
    struct Unk02028360 *ent = FindLivingInventionTargetAt(a1, a2);
    struct Unk41820Proc *proc;
    u8 *st;
    u8 *lim;
    int hp;

    if (ent == NULL)
    {
        StartAttackOnPipeSeamAt(a1, a2, c);
        return;
    }

    st = gUnknown_030044B0;
    *(u32 *)(st + 8) = gUnknown_03001FD4;

    CalcBattleDamageVsStructure(gUnknown_03003F38);

    hp = ent->unk04;
    lim = (u8 *)gBattleAttacker;

    if (hp < *(s16 *)(lim + 0x14))
        ent->unk04 = 0;
    else
        ent->unk04 = hp - lim[0x14];

    ResetDisplayEffects();

    proc = Proc_Start(gUnknown_0849FE78, PROC_TREE_3);
    proc->unk54 = gUnknown_030040D8;
    proc->unk4c = ent;
    proc->unk64 = c;
}
asm(".global sub_08041820\n.thumb_set sub_08041820, StartAttackOnInventionAt\n");
