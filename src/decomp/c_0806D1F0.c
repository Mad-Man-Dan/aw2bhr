#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D1F0.
 * sub_0806D1F0 @ 0x0806D1F0
 */

/* NOT a Proc: +0x26 and +0x28 fall inside PROC_HEADER (proc_mark/proc_flags
 * and proc_lockCnt), so this pointer parameter is some other object.  Only the
 * two halfwords are known; the rest is filler. */
struct Unk6D1F0
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
};

void RulesScreenRuleOptionEnter_Init(struct Unk6D1F0 *a)
{
    gUnknown_08580934->unk2d++;
    a->unk26 = 0x10;
}
asm(".global sub_0806D1F0\n.thumb_set sub_0806D1F0, RulesScreenRuleOptionEnter_Init\n");
