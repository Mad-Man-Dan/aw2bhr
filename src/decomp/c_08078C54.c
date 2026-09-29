#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078C54.
 * sub_08078C54 @ 0x08078C54, sub_08078CC8 @ 0x08078CC8
 */

#include "hardware.h"

struct Unk8078C54
{
    u8 filler_00[0x68];
    u16 unk_68;
};
struct Unk8078CC8
{
    u8 filler_00[0x68];
    u16 unk_68;
};

void BeginFadeToBlackFrom6(struct Unk8078C54 * proc)
{
    gUnknown_030030E0.bits.effect = 3;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 6;

    *(vu16 *)&gUnknown_030030E0 = (*(vu16 *)&gUnknown_030030E0 & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    proc->unk_68 = 0;
}
asm(".global sub_08078C54\n.thumb_set sub_08078C54, BeginFadeToBlackFrom6\n");

void sub_08078CC8(struct Unk8078CC8 * proc)
{
    gUnknown_030030E0.bits.effect = 2;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0;
    gUnknown_03001FFC = 6;

    *(vu16 *)&gUnknown_030030E0 = (*(vu16 *)&gUnknown_030030E0 & 0xFFE0) | 0x1F;
    gUnknown_030030E0.bits.target1_enable_bd = 1;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030DC.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    proc->unk_68 = 0;
}
