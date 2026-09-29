#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080722B8.
 * sub_080722B8 @ 0x080722B8
 */

#include "proc.h"
struct Unk722B8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4c */ void (*unk4c)(void);
    /* 50 */ STRUCT_PAD(0x50, 0x54);
    /* 54 */ int unk54;
};

/* StartFadeCore. Both dispatch words are bound to locals as their own
 * statements: the ROM loads each function pointer BEFORE it materialises that
 * call's arguments, which is not what an inline
 * `gUnknown_081CBF68[kind].unkNN(...)` gives -- there the argument setup comes
 * first (measured, 71.1%). `_call_via_r2` / `_call_via_r1` give the arities. */
void StartFadeCore(int kind, int speed, ProcPtr parent, void (*onDone)(void))
{
    struct Unk722B8Proc *proc;
    ProcPtr (*start)(const struct ProcCmd *, ProcPtr);
    void (*step)(int);
    int n;

    start = gUnknown_081CBF68[kind].unk00;
    proc = start(gUnknown_08613EE4, parent);

    proc->unk54 = speed;
    proc->unk4c = onDone;

    n = speed >> 4;

    if (n == 0)
        n = 1;

    step = gUnknown_081CBF68[kind].unk04;
    step((s8)(n * gUnknown_081CBF68[kind].unk08));
}
asm(".global sub_080722B8\n.thumb_set sub_080722B8, StartFadeCore\n");
