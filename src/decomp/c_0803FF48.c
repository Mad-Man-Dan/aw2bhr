#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803FF48.
 * sub_0803FF48 @ 0x0803FF48
 */

#include "proc.h"
struct Unk3FF48Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x50);
    /* 50 */ int unk50;
    /* 54 */ int unk54;
};

/* Sets the generation counter, sweeps every existing instance of the script
 * through ExplosionEffect_EndIfStale (which ends the ones whose stashed counter is stale),
 * then starts a fresh blocking instance and stamps the same counter into it.
 * a1 and a2 have to survive both calls, which is what costs the r8/sb pair. */
void StartExplosionEffect(int a1, int a2, int a3, ProcPtr parent)
{
    struct Unk3FF48Proc *proc;

    gUnknown_030044D4 = a3;
    Proc_ForEach(gUnknown_0849FB04, ExplosionEffect_EndIfStale);
    proc = Proc_StartBlocking(gUnknown_0849FB04, parent);
    proc->unk50 = 0;
    proc->unk2c = a1;
    proc->unk30 = a2;
    proc->unk54 = a3;
}
asm(".global sub_0803FF48\n.thumb_set sub_0803FF48, StartExplosionEffect\n");
