#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080130DC.
 * sub_080130DC @ 0x080130DC, sub_0801311C @ 0x0801311C
 */

#include "proc.h"
/* The three halfwords land at +0x64/+0x66/+0x68 of the proc, and the ROM says
 * what reads them back: gUnknown_0848936C's second ProcCmd is PROC_CMD_ONEND
 * with dataPtr &WhiteFlash_OnEnd, and WhiteFlash_OnEnd's whole body is gated on an
 * `ldrsh` of +0x68. So these are one proc's fields and not a scratch array.
 *
 * The `adds r0, #2` chain between the stores is what a run of separate
 * halfword members gives once the base has been bumped past strh's 5-bit
 * scaled offset limit; it is not an array subscript. */
struct Unk130DCProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x64);
    /* 64 */ u16 unk64;
    /* 66 */ u16 unk66;
    /* 68 */ u16 unk68;
};
#include "hardware.h"
/* The ONEND handler of gUnknown_0848936C -- the ROM says so: that script's
 * second ProcCmd is PROC_CMD_ONEND with dataPtr 0x0801311D. StartWhiteFlash is
 * the starter that fills +0x64/+0x66/+0x68, and this reads +0x68 back.
 *
 * Same shape as the promoted BlendToWhite_Init with every value zeroed instead of
 * set: `effect = 0` rather than 2, `raw & 0xffe0` with no `| 0x1f`, and
 * `target1_enable_bd = 0` rather than 1. That last one is why the ROM has
 * `movs r0, #0x21; rsbs r0, r0, #0` -- agbcc materialises the 32-bit mask
 * ~0x20 = 0xFFFFFFDF as the negation of 0x21, not as a `movs r0, #0xDF`.
 *
 * BlendToWhite_Init needed a `do { } while (0)` around the raw read-modify-write to
 * stop agbcc hoisting the trailing proc-field store into it. There is no
 * trailing store here, so the barrier is unnecessary and the plain statement
 * matches. */
struct Unk0801311C
{
    /* 0x00 */ u8 filler_00[0x68];
    /* 0x68 */ s16 unk68;
};

void StartWhiteFlash(int a, int b, int c, ProcPtr parent)
{
    struct Unk130DCProc *proc;

    if (parent != 0)
        proc = Proc_StartBlocking(gUnknown_0848936C, parent);
    else
        proc = Proc_Start(gUnknown_0848936C, PROC_TREE_3);

    proc->unk64 = a;
    proc->unk66 = b;
    proc->unk68 = c;
}
asm(".global sub_080130DC\n.thumb_set sub_080130DC, StartWhiteFlash\n");

void WhiteFlash_OnEnd(struct Unk0801311C *proc)
{
    if (proc->unk68 != 0)
    {
        gUnknown_030030E0.bits.effect = 0;
        gUnknown_03002020 = 0;
        gUnknown_03002B28 = 0;
        gUnknown_03001FFC = 0;
        gUnknown_030030E0.raw = gUnknown_030030E0.raw & 0xffe0;
        gUnknown_030030E0.bits.target1_enable_bd = 0;
    }
}
asm(".global sub_0801311C\n.thumb_set sub_0801311C, WhiteFlash_OnEnd\n");
