#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F2B8.
 * sub_0803F2B8 @ 0x0803F2B8
 */

#include "proc.h"
struct UnkF2B8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x4c);
    /* 4c */ int unk4c;
};

/* r1 is never written before the bl, so the parent is this wrapper's own
 * fourth parameter arriving via r3. */
void StartVolcanoFire(int a, int b, int c, ProcPtr parent)
{
    struct UnkF2B8Proc *proc = Proc_StartBlocking(gUnknown_0849F888, parent);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk4c = c;
}
asm(".global sub_0803F2B8\n.thumb_set sub_0803F2B8, StartVolcanoFire\n");
