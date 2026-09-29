#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BF90.
 * sub_0801BF90 @ 0x0801BF90, sub_0801BFB8 @ 0x0801BFB8
 */

#include "proc.h"
/* The draw half of the proc that StartSpriteRefresher starts: every field that
 * function writes is read straight back out here and handed to PutSprite. The
 * two are a producer/consumer pair over the same layout, which is the
 * discriminating use the field types would otherwise lack -- neither function
 * alone proves unk54 holds a pointer.
 *
 * unk50 is SIGNED: `movs r2, #0; ldrsh r4, [r1, r2]` off a zero index register,
 * where an unsigned field would have been a plain `ldrh` with a displacement.
 * unk52 and unk54 are past the 31-byte `ldrh` displacement limit relative to
 * the anchor agbcc chose, hence the `adds r0, #0x52` chain. */
struct Unk1BF90Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ u32 unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x50);
    /* 50 */ s16 unk50;
    /* 52 */ u16 unk52;
    /* 54 */ u16 *unk54;
};
/* Starts the sprite proc SpriteRefresher_OnIdle draws, seeding all five of its fields and
 * returning it. The same script gUnknown_0848B418 appears in TWO pool words
 * because each arm of the parent test loads it independently -- that
 * duplication is the if/else, not two different scripts.
 *
 * SIX parameters, and the last two are the reason the widths are settled: they
 * arrive on the stack at [sp,#0x14] and [sp,#0x18] and are read with a plain
 * `ldr` at the point of use. Declared u16, agbcc narrows both in the PROLOGUE
 * instead, which costs an extra callee-saved register and the `mov r7, r8`
 * spill that goes with it. The `strh` at +0x50 and +0x52 is the field's width,
 * not the parameter's -- those are independent facts.
 *
 * The branch sense is `!= 0` with the parent arm inline: agbcc branches on the
 * inverted condition to the ELSE block here, so `beq` reaching the PROC_TREE_3
 * arm makes that arm the else. */
struct Unk1BFB8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ u32 unk2c;
    /* 30 */ u32 unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x50);
    /* 50 */ s16 unk50;
    /* 52 */ u16 unk52;
    /* 54 */ u16 *unk54;
};

void SpriteRefresher_OnIdle(struct Unk1BF90Proc *proc)
{
    PutSprite(proc->unk50, proc->unk2c, proc->unk30, proc->unk54, proc->unk52);
}
asm(".global sub_0801BF90\n.thumb_set sub_0801BF90, SpriteRefresher_OnIdle\n");

ProcPtr StartSpriteRefresher(ProcPtr parent, int oam, u32 a, u32 b, u16 *gfx, int pal)
{
    struct Unk1BFB8Proc *proc;

    if (parent != NULL)
        proc = Proc_Start(gUnknown_0848B418, parent);
    else
        proc = Proc_Start(gUnknown_0848B418, PROC_TREE_3);

    proc->unk2c = a;
    proc->unk30 = b;
    proc->unk50 = oam;
    proc->unk54 = gfx;
    proc->unk52 = pal;
    return proc;
}
asm(".global sub_0801BFB8\n.thumb_set sub_0801BFB8, StartSpriteRefresher\n");
