#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804018C.
 * sub_0804018C @ 0x0804018C
 */

#include "proc.h"
/* The argument is a POINTER, not the `s32` this was promoted with. Two callers
 * settle it and neither is expressible as an integer: WriteBackBattleUnit does
 * `ldr r3,[r5]; adds r0,r3,#0` on a struct it has just been storing through,
 * and AiExecuteDestroyUnit passes `gUnknown_030040D8`, the dereferenced struct-pointer
 * global. They are different object types, so `void *` and not either struct.
 * Byte-neutral -- the parameter is only ever `str`-ed -- and re-verified with
 * trymatch. */
struct UnkProc0849FB44
{
    u8 filler_00[0x4c];
    void *unk_4c;
};

void StartUnitDestroyed(void *arg0)
{
    ((struct UnkProc0849FB44 *)(Proc_Start(gUnknown_0849FB44, PROC_TREE_3)))->unk_4c = arg0;
}
asm(".global sub_0804018C\n.thumb_set sub_0804018C, StartUnitDestroyed\n");
