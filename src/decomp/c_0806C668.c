#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C668.
 * sub_0806C668 @ 0x0806C668
 */

#include "hardware.h"
#include "proc.h"
/* Starts the gUnknown_08581A80 child, gives it one of two OAM blobs and matching
 * x, then runs the same blend/window setup src/decomp/c_0806C700.c does with two
 * different coefficients (0 and 0x10 here, 0x10 and 8 there).
 *
 * The `movs r2,#0` that zeroes the child's +0x58 is the SAME register the mask
 * chain below reuses -- `subs r2,#0x21` turns it into ~0x20 for both
 * target-enable clears. That is ordinary cse, not a shared variable. */
struct Unk0806C668Proc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
};
struct Unk08581A80Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x24];
    /* 0x54 */ u16 *unk54;
    /* 0x58 */ int unk58;
};

void StartCreditsResultSprite(struct Unk0806C668Proc *proc)
{
    struct Unk08581A80Proc *p;

    p = Proc_Start(gUnknown_08581A80, proc);

    if (proc->unk38 != 0)
    {
        p->unk2c = 0;
        p->unk54 = gUnknown_08581A5E;
    }
    else
    {
        p->unk2c = 8;
        p->unk54 = gUnknown_08581A44;
    }

    p->unk58 = 0;

    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = gUnknown_03001FFC;

    *(u16 *)&gUnknown_030030E0 = *(u16 *)&gUnknown_030030E0 & 0xFFE0;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0xF00;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
}
asm(".global sub_0806C668\n.thumb_set sub_0806C668, StartCreditsResultSprite\n");
