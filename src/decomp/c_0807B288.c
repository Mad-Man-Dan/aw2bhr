#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B288.
 * sub_0807B288 @ 0x0807B288, sub_0807B2B8 @ 0x0807B2B8
 */

struct Proc7B288
{
    u8 filler_00[0x30];
    int unk_30;
    u8 filler_34[0x54 - 0x34];
    int unk_54;
    u8 filler_58[0x64 - 0x58];
    s16 unk_64;
};
#include "proc.h"
/* The proc's +0x30 is `ldrsh` here but `str`-ed as a word by MatchSummaryPanel_Init two
 * functions earlier. Each file models the object with only the offsets it
 * touches, so the local layouts differ deliberately -- the load width in this
 * body is the fact, not the store width in the other one. */

struct Proc7B2B8
{
    u8 filler_00[0x30];
    s16 unk_30;
    u8 filler_32[0x54 - 0x32];
    int unk_54;
    u8 filler_58[0x64 - 0x58];
    s16 unk_64;
};

void MatchSummaryPanel_Init(struct Proc7B288 *proc)
{
    if (gPlaySt.gameMode == 3)
    {
        proc->unk_30 = 9;
        proc->unk_64 = 4;
    }
    else
    {
        proc->unk_30 = 7;
        proc->unk_64 = 9;
    }
    proc->unk_54 = 3;
}
asm(".global sub_0807B288\n.thumb_set sub_0807B288, MatchSummaryPanel_Init\n");

void MatchSummaryPanel_GrowWindow_Loop(struct Proc7B2B8 *proc)
{
    DrawWindowBackgroundOnBg2(8 - proc->unk_54, proc->unk_30, proc->unk_54 * 2 + 1, proc->unk_64);
    if (proc->unk_54 > 7)
        Proc_Break(proc);
    proc->unk_54++;
}
asm(".global sub_0807B2B8\n.thumb_set sub_0807B2B8, MatchSummaryPanel_GrowWindow_Loop\n");
