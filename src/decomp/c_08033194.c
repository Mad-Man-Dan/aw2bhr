#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033194.
 * sub_08033194 @ 0x08033194
 */

#include "proc.h"
#include "hardware.h"
struct Unk33194Proc
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ s16 unk64;
};

void LinkMultiboot_Init(struct Unk33194Proc *proc)
{
    struct Unk08062FB8 *p;

    gUnknown_030032DC = gUnknown_08090D5C[0];
    gUnknown_03003F28 = gUnknown_08090D5C[1] - gUnknown_08090D5C[0];
    gUnknown_03003F44 = gUnknown_08090D5C[0] + 0x2b0;

    p = (struct Unk08062FB8 *)gUnknown_03003F70;
    p->unk28 = gUnknown_08090D5C[0];
    p->unk4b = 0;

    sub_08062FB8(p);

    gUnknown_0849B018->unk0a[0] = 4;
    gUnknown_0849B018->unk06 = 0;
    gUnknown_0849B018->unk09 = 0;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 0, 0);

    proc->unk64 = 0;
}
asm(".global sub_08033194\n.thumb_set sub_08033194, LinkMultiboot_Init\n");
