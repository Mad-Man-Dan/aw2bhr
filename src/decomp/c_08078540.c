#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078540.
 * sub_08078540 @ 0x08078540
 */

#include "proc.h"
struct UnkProc8615AAC
{
    u8 filler_00[0x54];
    void *unk_54;
};

void StartBlockingEventScript(void *arg, ProcPtr parent)
{
    ((struct UnkProc8615AAC *)Proc_StartBlocking(gUnknown_08615AAC, parent))->unk_54 = arg;
}
asm(".global sub_08078540\n.thumb_set sub_08078540, StartBlockingEventScript\n");
