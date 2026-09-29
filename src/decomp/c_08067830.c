#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067830.
 * sub_08067830 @ 0x08067830
 */

#include "hardware.h"
#include "proc.h"
/* The zoom step of the 0x08067 screen: accumulate unk3c into unk5c, carry a
 * quarter of that into unk58, and rebuild the BG2 affine matrix from it.
 *
 * The 0x14-byte stack frame is `struct BgAffineSrcData` -- two words, four
 * halfwords and a trailing halfword, in that order -- the same identification
 * src/decomp/c_080678EC.c records, and gUnknown_030024D0 is cast rather than
 * retyped for the same reason.
 *
 * Both `/ 4`s are SIGNED (`cmp #0; bge; adds #3; asrs #2`), and the second is
 * one expression feeding two stores: the ROM computes `asrs r1,r0,#2` once and
 * `strh`s it to both sx and sy. */
struct Unk08067830Proc
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ s32 unk3c;
    /* 0x40 */ u8 filler_40[0x18];
    /* 0x58 */ s32 unk58;
    /* 0x5c */ s32 unk5c;
    /* 0x60 */ s32 unk60;
};

void IntroBgZoom_Loop(struct Unk08067830Proc *proc)
{
    struct BgAffineSrcData src;

    if (proc->unk60 != 0)
    {
        proc->unk5c += proc->unk3c;
        proc->unk58 += proc->unk5c / 4;

        src.texX = 0x7800;
        src.texY = 0x5000;
        src.scrX = 0x78;
        src.scrY = 0x50;
        src.sx = proc->unk58 / 4;
        src.sy = proc->unk58 / 4;
        src.alpha = 0;

        BgAffineSet(&src, (struct BgAffineDstData *)gUnknown_030024D0, 1);
    }
}
asm(".global sub_08067830\n.thumb_set sub_08067830, IntroBgZoom_Loop\n");
