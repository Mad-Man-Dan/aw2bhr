#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080272C4.
 * sub_080272C4 @ 0x080272C4
 */

#include "proc.h"
struct Unk272C4Proc
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

/* MATCHED, and the first of a byte-identical pair with SupplyIconEffect_Init. Allocates
 * a sprite through AP_Create, stashes it at +0x50, sets its unk22 attribute
 * word and seeds three proc fields.
 *
 * Nothing here needed a probe -- the proc layout falls straight out of the
 * displacements and AP_Create / AP_SwitchAnimation were already typed. The one
 * readout is LoadPopupIcon's arity: four parameters, of which the fourth is the
 * AP_Create result on its way to AP_SwitchAnimation's `struct Unk0801C210 *`. */
void sub_080272C4(struct Unk272C4Proc *proc)
{
    proc->unk50 = AP_Create(gUnknown_08112614, 1, 1);
    proc->unk50->unk22 = 0x31CA;

    LoadPopupIcon(proc->unk2c, proc->unk30, proc->unk54, proc->unk50);

    proc->unk64 = 0x200;
    proc->unk44 = 0xFFFF;
}
