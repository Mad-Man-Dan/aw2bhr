#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A6F0.
 * sub_0806A6F0 @ 0x0806A6F0
 */

#include "hardware.h"
#include "proc.h"
/* Sets up the pan/zoom proc StartMeteorImpact just started: clears the top palette
 * row, records a start and an end position in 12-bit fixed point, derives the
 * per-frame step for each axis by dividing the span by the 0x33-frame duration,
 * and hands the proc to gUnknown_085815D0 (whose MeteorImpactGlow_Loop reads unk5a back
 * through its own +0x2c -- see src/decomp/c_0806A534.c).
 *
 * The divides are `__divsi3`, not Div, so they are the plain `/` operator.
 * unk5a is a genuine s16 OBJECT: both call sites reload it with the
 * `movs rI, #0; ldrsh r1, [r6, rI]` register-offset form rather than reusing
 * the 0x33 just stored.
 *
 * The clear loop is ASCENDING in the source even though the ROM counts r1 down
 * from 0xf and walks the pointer backwards from &gPal[0x14f]. The preheader is
 * what proves it: agbcc writes the source's own init FIRST, then LICM's hoists,
 * then strength_reduce's giv inits -- and here `movs r1, #0xf` lands THIRD,
 * after the hoisted `movs r2, #0` and after the `=gPal` load, so it was written
 * by check_dbra_loop and not by the source. Spelling the loop descending puts
 * `movs r1, #0xf` first and costs 6 bytes.
 *
 * The proc is reached through a CAST rather than a `struct ... *proc = a1;`
 * binding. The binding is an executable statement, so agbcc emits its
 * `adds r5, r0, #0` at the first statement of the body, AFTER the entry saves
 * of r1 and r2; the ROM saves all three in parameter order, which only happens
 * when r0 is used directly as a parameter. */

struct Unk806A6F0
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ int unk48;
    /* 0x4c */ int unk4c;
    /* 0x50 */ u8 filler_50[0x08];
    /* 0x58 */ u16 unk58;
    /* 0x5a */ s16 unk5a;
    /* 0x5c */ u8 filler_5c[0x02];
    /* 0x5e */ u16 unk5e;
    /* 0x60 */ u16 unk60;
};
struct Unk806A6F0Sub
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ void *unk2c;
};
#define PROC ((struct Unk806A6F0 *)a1)

void SetupMeteorImpact(ProcPtr a1, int a2, int a3)
{
    struct Unk806A6F0Sub *sub;
    int i;

    RegisterDataMove(gUnknown_0817B150, (void *)0x06013940, 0x800);
    ApplyPaletteExt(gUnknown_0817B950, 0x260, 0x20);

    for (i = 0; i < 0x10; i++)
        gPal[0x140 + i] = 0;

    PROC->unk38 = a2 << 12;
    PROC->unk3c = a3 << 12;
    PROC->unk30 = (0xf0 - a2) << 12;
    PROC->unk34 = (a3 - 0x80) << 12;
    PROC->unk58 = 0x200;
    PROC->unk5a = 0x33;
    PROC->unk40 = PROC->unk30;
    PROC->unk44 = PROC->unk34;
    PROC->unk48 = (PROC->unk38 - PROC->unk40) / PROC->unk5a;
    PROC->unk4c = (PROC->unk3c - PROC->unk44) / PROC->unk5a;

    sub = Proc_Start(gUnknown_085815D0, a1);
    sub->unk2c = a1;

    PROC->unk5e = gMap->scrollX;
    PROC->unk60 = gMap->scrollY;
}
asm(".global sub_0806A6F0\n.thumb_set sub_0806A6F0, SetupMeteorImpact\n");
