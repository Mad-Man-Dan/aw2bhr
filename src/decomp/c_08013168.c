#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013168.
 * sub_08013168 @ 0x08013168
 */

#include "hardware.h"
#include "proc.h"
/*
 * WhiteFlash_Init -- the start-up body of the gUnknown_0848936C process: clear its
 * three word fields and set the blend registers up.
 *
 * unk64 chooses the starting blend level in gUnknown_03001FFC -- 0x10 when it
 * is 0, otherwise 0. Either way the blend effect becomes 2 and
 * gUnknown_03002020 and gUnknown_03002B28 are cleared. The blend control word
 * then names the targets: all four backgrounds, the sprites and the backdrop as
 * the first blend target, and none of them as the second.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - Both arms of the `if` repeat `effect = 2` and the two zero stores. Lifted
 *     above the `if` they are emitted once, and the original has them twice.
 */
struct Unk08013168Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ u32 unk54;
    /* 58 */ u32 unk58;
    /* 5c */ u32 unk5c;
    /* 60 */ STRUCT_PAD(0x60, 0x64);
    /* 64 */ s16 unk64;
};

void WhiteFlash_Init(struct Unk08013168Proc *proc)
{
    proc->unk54 = 0;
    proc->unk58 = 0;
    proc->unk5c = 0;

    if (proc->unk64 == 0)
    {
        gUnknown_030030E0.bits.effect = 2;
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = 0x10;
    }
    else
    {
        gUnknown_030030E0.bits.effect = 2;
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = 0;
    }

    gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xffe0) | 0x1f;
    gUnknown_030030E0.raw = gUnknown_030030E0.raw & 0xe0ff;
    gUnknown_030030E0.bits.target1_enable_bd = 1;
    gUnknown_030030E0.bits.target2_enable_bd = 0;
}
asm(".global sub_08013168\n.thumb_set sub_08013168, WhiteFlash_Init\n");
