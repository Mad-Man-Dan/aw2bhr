#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078150.
 * sub_08078150 @ 0x08078150, sub_08078168 @ 0x08078168, sub_08078180 @ 0x08078180
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"

int Campaign_WHILE_08078151(void)
{
    return Proc_Find(gUnknown_086147FC) != 0;
}

int IsWorldMapForResumeRunning(void)
{
    return Proc_Find(gUnknown_0861485C) != 0;
}
asm(".global sub_08078168\n.thumb_set sub_08078168, IsWorldMapForResumeRunning\n");

int IsWorldMapAfterMissionRunning(void)
{
    return Proc_Find(gUnknown_08614894) != 0;
}
asm(".global sub_08078180\n.thumb_set sub_08078180, IsWorldMapAfterMissionRunning\n");

asm(".global sub_08078150\n.thumb_set sub_08078150, Campaign_WHILE_08078151\n");
