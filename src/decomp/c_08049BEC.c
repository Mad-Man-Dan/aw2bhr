#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08049BEC.
 * sub_08049BEC @ 0x08049BEC
 */

#include "proc.h"
/* Claims the next of N stacked entries: the running counter gUnknown_02028E3D
 * becomes this proc's slot index, the slot fixes its y, and the counter is then
 * post-incremented.
 *
 * unk22 is SIGNED: it is written from a `ldrb` and read straight back with
 * `movs r5,#0x22; ldrsh r0,[r1,r5]`, the register-offset ldrsh that is Thumb's
 * only signed-halfword load. The two zeroes (r3 for the two `strb`s, r4 for the
 * `strh`) are materialised together at the top, ahead of everything. */
struct Unk08049BECProc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x02];
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
    /* 0x27 */ u8 filler_27[0x01];
    /* 0x28 */ u8 unk28;
};

void DefeatFlow_BannerPiece_Init(struct Unk08049BECProc *proc)
{
    proc->unk22 = gUnknown_02028E3D;
    proc->unk1e = proc->unk22 * 0x20 + 0x38;

    gUnknown_084C3240->unk20 = 0x40;
    gUnknown_02028E3D++;

    proc->unk26 = 0x50;
    proc->unk28 = 0;

    gUnknown_084C3240->unk2a = 0;

    proc->unk24 = 0;
}
asm(".global sub_08049BEC\n.thumb_set sub_08049BEC, DefeatFlow_BannerPiece_Init\n");
