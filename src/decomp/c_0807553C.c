#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807553C.
 * sub_0807553C @ 0x0807553C
 */

#include "hardware.h"
#include "proc.h"
/* MATCHED (wave 38, W38-E) via decomp-permuter, 66 iterations from the 96.3%
 * hand draft. Parked since an earlier wave at size-exact with 6 bytes differing.
 *
 * THE RESIDUAL WAS THE ALLOCNO TIE-BREAK BETWEEN x AND y, and the lever is
 * REUSING ONE LOCAL AS THE SCRATCH FOR THE OTHER'S COMPUTATION:
 *
 *     y = a1 - 0x10;        <- y's pseudo is created FIRST, holding x's value
 *     x = (u16)y;
 *     y = (u16)(a2 - 0x10);
 *
 * rather than the symmetric `x = (u16)(a1 - 0x10); y = (u16)(a2 - 0x10);`,
 * which puts x in r5 and y in r4 -- the ROM's assignment reversed. The earlier
 * park had ruled out declaring y before x and moving `proc`'s declaration, and
 * was right that DECLARATION order is not the lever: gcc 2.9 creates these
 * pseudos at first REFERENCE. Naming y first in an assignment is what moves the
 * reference, and no reordering of the declarations can reach it.
 *
 * This is the wave-37 "reuse an existing local rather than a fresh name inverts
 * depending on the exit test" rule showing up as a lever rather than a
 * preference, and it is also the confirmation that the permuter DOES reach
 * pure register-allocation residuals -- see docs/agbcc-codegen.md.
 *
 * StartWorldMapSelectionFrame is the re-entry twin (src/decomp/c_0807548C.c): identical
 * coordinate head -- see that file for why the parameters are s16 and the
 * locals are s16-with-an-explicit-(u16) -- and an identical blend tail, but it
 * Proc_FINDs the already-running gUnknown_086143B8 proc instead of starting
 * one, tests its third argument against 1 rather than 2, and slides the
 * existing +0x2a/+0x2c pair up into +0x2e/+0x30 before writing the new target
 * into +0x32/+0x34: the old destination becomes the new origin, which is what
 * makes c_08075368.c's Interpolate arms read +0x2e..+0x34 as two endpoint
 * pairs. Those two extra field-to-field copies, which StartWorldMapSelectionFrame does not
 * have, are exactly what consumed the r1 scratch and shifted the tie-break.
 */
struct Unk807553C
{
    /* 0x00 */ u8 filler_00[0x2a];
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u16 unk32;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u8 filler_36[0x2];
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
};

void MoveWorldMapSelectionFrame(s16 a1, s16 a2, int a3)
{
    struct Unk807553C *proc;
    s16 x;
    s16 y;

    y = a1 - 0x10;
    x = (u16)y;
    y = (u16)(a2 - 0x10);

    if (a3 != 1)
    {
        x = 0xFFE2;
        y = 0xFFC0;
    }

    proc = Proc_Find(gUnknown_086143B8);

    proc->unk2e = proc->unk2a;
    proc->unk30 = proc->unk2c;
    proc->unk32 = -x;
    proc->unk34 = -y;
    proc->unk38 = a3;
    proc->unk3c = 0;

    gUnknown_030030E0.bits.effect = 1;
    gUnknown_03002020 = 0xC;
    gUnknown_03002B28 = 0x10;
    gUnknown_03001FFC = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 2;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x1800;
}
asm(".global sub_0807553C\n.thumb_set sub_0807553C, MoveWorldMapSelectionFrame\n");
