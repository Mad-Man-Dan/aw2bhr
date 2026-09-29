#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08027428.
 * SupplyIconEffect_Init @ 0x08027428
 */

#include "proc.h"
struct Unk27428Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x44);
    /* 44 */ s16 unk44;
    /* 46 */ STRUCT_PAD(0x46, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ STRUCT_PAD(0x58, 0x64);
    /* 64 */ s16 unk64;
};

/* MATCHED. Byte-for-byte the same function as sub_080272C4 -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void SupplyIconEffect_Init(struct Unk27428Proc *proc)
{
    proc->unk50 = AP_Create(gUnknown_08112614, 1, 1);
    proc->unk50->unk22 = 0x31CA;

    LoadPopupIcon(proc->unk2c, proc->unk30, proc->unk54, proc->unk50);

    proc->unk64 = 0x200;
    proc->unk44 = 0xFFFF;
}
asm(".global sub_08027428\n.thumb_set sub_08027428, SupplyIconEffect_Init\n");
