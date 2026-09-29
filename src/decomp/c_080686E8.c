#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080686E8.
 * sub_080686E8 @ 0x080686E8
 */

#include "proc.h"
#include "hardware.h"
/* Starts the 0x08581210 proc and loads the graphics that go with it.
 *
 * The `(tbl = gUnknown_085D3DD0)[a2]` binding is not decoration -- it is the
 * whole difference. Written plainly as `gTextTable[gUnknown_085D3DD0[a2]
 * .unk00]`, agbcc folds `base + a2 * 0x104` into the memory address and
 * force_reg's the symbol LATE, emitting `ldr =gUnknown_085D3DD0` after the
 * index shifts. The ROM loads both table addresses up front. Assigning the base
 * to a local inside the outer subscript creates the symbol's pseudo at the head
 * of the outer PLUS's second operand -- after `ldr =gTextTable`, before
 * the shifts -- which is exactly the ROM's order. Four other spellings (pointer
 * arithmetic, a `(u8 *)base + a2 * 0x104` cast, a hoisted element pointer, a
 * whole-function base local) were probed and all place the load elsewhere. */
struct Unk686E8Proc
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x05];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x1a];
    /* 0x4d */ u8 unk4d;
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};

void StartIntroCoNameBanner(int a1, int a2, u16 a3, u8 a4, ProcPtr parent)
{
    struct Unk686E8Proc *proc;
    const struct CoData *tbl;

    proc = Proc_Start(gUnknown_08581210, parent);
    proc->unk29 = a2;
    proc->unk4d = a1;
    proc->unk30 = a3;
    proc->unk32 = a4;
    ApplyPalettes(gUnknown_0817DA18, a4 + 0x10, 1);
    proc->unk2a = sub_08068038(gTextTable[(tbl = gUnknown_085D3DD0)[a2].nameIndex], a3);
    proc->unk4f = 0;
}
asm(".global sub_080686E8\n.thumb_set sub_080686E8, StartIntroCoNameBanner\n");
