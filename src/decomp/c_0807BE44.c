#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807BE44.
 * sub_0807BE44 @ 0x0807BE44, sub_0807BE70 @ 0x0807BE70, sub_0807BE90 @ 0x0807BE90
 */

#include "proc.h"
struct Proc7BE44
{
    u8 filler_00[0x4c];
    s16 unk_4c;
    u8 filler_4e[0x66 - 0x4e];
    s16 unk_66;
};
struct Proc7BE70
{
    u8 filler_00[0x60];
    int unk_60;
};
#include "hardware.h"
struct Proc7BE90
{
    u8 filler_00[0x68];
    u16 unk_68;
};

void MissionTitleLetters_Reveal_Loop(struct Proc7BE44 *proc)
{
    MissionTitleLetters_PutRevealingLetters(proc);
    if (proc->unk_66 >= proc->unk_4c * 10)
        Proc_Break(proc);
}
asm(".global sub_0807BE44\n.thumb_set sub_0807BE44, MissionTitleLetters_Reveal_Loop\n");

void MissionTitleLetters_BarExpand_Loop(struct Proc7BE70 *proc)
{
    MissionTitleLetters_PutLetterRow(proc);
    MissionTitleLetters_PutMissionLabelAndBar(proc);
    if (proc->unk_60 > 0xe)
        Proc_Break(proc);
}
asm(".global sub_0807BE70\n.thumb_set sub_0807BE70, MissionTitleLetters_BarExpand_Loop\n");

void MissionTitleLetters_Hold_Loop(struct Proc7BE90 *proc)
{
    MissionTitleLetters_PutLetterRow(proc);
    MissionTitleLetters_PutFinalLabelAndBar(proc);
    proc->unk_68++;
    if (proc->unk_68 == 0x1e0 || (gpKeySt->pressed & 1))
        Proc_Start(gUnknown_08616570, proc);
}
asm(".global sub_0807BE90\n.thumb_set sub_0807BE90, MissionTitleLetters_Hold_Loop\n");
