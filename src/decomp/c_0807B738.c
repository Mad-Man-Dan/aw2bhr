#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B738.
 * sub_0807B738 @ 0x0807B738, sub_0807B760 @ 0x0807B760
 */

struct Proc7B738
{
    u8 filler_00[0x30];
    int unk_30;
};
#include "hardware.h"
#include "proc.h"

/* Wave 48 (W48-E): retyped from `struct Proc7B738 *` to ProcPtr so the
 * function can be DECLARED in include/unknown-functions.h -- MatchSummaryPanel_Hold_Loop
 * tail-calls it with its own view of the same proc, and a file-local struct
 * tag cannot appear in a shared prototype. Byte-identical either way; the
 * cast is where the type now lives. */
void sub_0807B738(ProcPtr proc)
{
    PutSprite(0, 0x71, (((struct Proc7B738 *)proc)->unk_30 + 1) * 8, gUnknown_0848B690, 0x1064);
}

void MatchSummary_WaitForA_Loop(ProcPtr proc)
{
    if (gpKeySt->pressed & 1)
    {
        Proc_Start(gUnknown_0861604C, proc);
        Proc_Break(proc);
    }
}
asm(".global sub_0807B760\n.thumb_set sub_0807B760, MatchSummary_WaitForA_Loop\n");
