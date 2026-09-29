#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D4C0.
 * sub_0806D4C0 @ 0x0806D4C0
 */

/* Same object as RulesScreenRuleOptionEnter_Init -- see the note there; +0x26/+0x28 rule out a
 * Proc. */
struct Unk6D4C0
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u16 unk28;
};

void RulesScreenArmyColumnEnter_Init(struct Unk6D4C0 *a)
{
    gUnknown_08580934->unk2d++;
    a->unk26 = 0x10;
    a->unk28 = 0xf0;
}
asm(".global sub_0806D4C0\n.thumb_set sub_0806D4C0, RulesScreenArmyColumnEnter_Init\n");
