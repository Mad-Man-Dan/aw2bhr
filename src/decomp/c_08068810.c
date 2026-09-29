#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08068810.
 * sub_08068810 @ 0x08068810
 */

#include "proc.h"
#include "hardware.h"
/* StartIntroCoNameBanner's seven-argument sibling -- same 0x0858xxxx proc script setup,
 * same gUnknown_085D3DD0 -> gTextTable -> sub_08068038 graphics chain.
 * Diffed against StartIntroCoNameBanner rather than predicted: the proc fields written
 * are +0x29, +0x30, +0x32, +0x2a, then +0x38, +0x39, +0x4e and +0x4f, where the
 * shorter one writes +0x4d and +0x4f. The store to +0x32 reaches through the
 * +0x29 chain (`adds r0,#9`) because the halfword store to +0x30 uses r7's own
 * displacement and does not disturb it.
 *
 * The `(tbl = gUnknown_085D3DD0)[a1]` binding is the same lever as in
 * StartIntroCoNameBanner; see the note there. */
struct Unk68810Proc
{
    /* 0x00 */ u8 filler_00[0x29];
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
    /* 0x2b */ u8 filler_2b[0x05];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 filler_33[0x05];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3a */ u8 filler_3a[0x14];
    /* 0x4e */ u8 unk4e;
    /* 0x4f */ u8 unk4f;
};

void StartIntroCoNameWobble(int a1, int a2, int a3, int a4, u16 a5, u8 a6, ProcPtr parent)
{
    struct Unk68810Proc *proc;
    const struct CoData *tbl;

    proc = Proc_Start(gUnknown_08581264, parent);
    proc->unk29 = a1;
    proc->unk30 = a5;
    proc->unk32 = a6;
    ApplyPalettes(gUnknown_0817DA18, a6 + 0x10, 1);
    proc->unk2a = sub_08068038(gTextTable[(tbl = gUnknown_085D3DD0)[a1].nameIndex], a5);
    proc->unk38 = a2;
    proc->unk39 = a3;
    proc->unk4e = a4;
    proc->unk4f = 0;
}
asm(".global sub_08068810\n.thumb_set sub_08068810, StartIntroCoNameWobble\n");
