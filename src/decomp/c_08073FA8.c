#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073FA8.
 * sub_08073FA8 @ 0x08073FA8, sub_08073FF4 @ 0x08073FF4
 */

#include "proc.h"
/* The per-frame step of the proc StartBgScrollAnimator starts: four channels, each a
 * 16.8 position pair at +0x30 stepped by a velocity pair at +0x40, gated by
 * one bit of the +0x2c mask. The value handed to SetBgScrollShadow is the
 * position's integer part narrowed to a signed byte and then widened again
 * for that function's u16 parameters.
 *
 * The mask must be bound to a local. Written inline, `proc->unk2C & (1 << i)`
 * is folded by `fold` into `(proc->unk2C >> i) & 1` -- `asr r0, r0, r5;
 * movs r1, #1; ands` -- because the AND feeds a comparison against zero. A
 * VAR_DECL is opaque to that transform, so the shift stays on the constant
 * and lands ahead of the load, exactly as the ROM has it. Verified with
 * compile_probe both ways. */

struct Unk73FA8Vec
{
    /* 00 */ u16 x;
    /* 02 */ u16 y;
};
struct Unk73FA8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2C);
    /* 2C */ int unk2C;
    /* 30 */ struct Unk73FA8Vec pos[4];
    /* 40 */ struct Unk73FA8Vec vel[4];
};
/* Starts gUnknown_08614220 and seeds it: a word at +0x2c and two parallel
 * halfword arrays 0x10 apart, the first cleared and the second copied out of
 * the caller's buffer. The `movs r2, #7` / `bge` counter is eight passes --
 * agbcc's reversal of an ascending `for (i = 0; i < 8; i++)`, so the loop
 * runs 0..7 in the source.
 *
 * One induction variable serves both arrays: the ROM walks r1 from
 * proc + 0x30 and reaches the second array with the displacement 0x10, which
 * is what two 8-entry u16 arrays at 0x30 and 0x40 compile to. The source
 * pointer is stepped independently. BgScrollAnimator_Loop reads the same 0x30/0x40
 * bytes as four PAIRS of halfwords; both readings describe the same memory
 * and each function is spelled the way its own accesses read out. */

struct Unk73FF4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2C);
    /* 2C */ int unk2C;
    /* 30 */ u16 unk30[8];
    /* 40 */ u16 unk40[8];
};

void BgScrollAnimator_Loop(struct Unk73FA8Proc * proc)
{
    int i;
    int mask;

    for (i = 0; i < 4; i++)
    {
        mask = 1 << i;

        if (proc->unk2C & mask)
        {
            proc->pos[i].x += proc->vel[i].x;
            proc->pos[i].y += proc->vel[i].y;

            SetBgScrollShadow(i, (s8)(proc->pos[i].x >> 8), (s8)(proc->pos[i].y >> 8));
        }
    }
}
asm(".global sub_08073FA8\n.thumb_set sub_08073FA8, BgScrollAnimator_Loop\n");

void StartBgScrollAnimator(int a, const void * src, ProcPtr parent)
{
    struct Unk73FF4Proc * proc = Proc_Start(gUnknown_08614220, parent);
    const u16 * in = src;
    int i;

    proc->unk2C = a;

    for (i = 0; i < 8; i++)
    {
        proc->unk30[i] = 0;
        proc->unk40[i] = in[i];
    }
}
asm(".global sub_08073FF4\n.thumb_set sub_08073FF4, StartBgScrollAnimator\n");
