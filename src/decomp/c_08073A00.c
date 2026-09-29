#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073A00.
 * sub_08073A00 @ 0x08073A00
 */

#include "hardware.h"
#include "proc.h"
/* Wave 57 (W57-D). MATCHED (was parked at 72.4%, size exact, since wave 35).
 *
 * PROMOTION NEEDS A .rodata POOL WORD PLACED:
 *   "rodata": ["0x081CC030"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py.
 *
 * THE WHOLE RESIDUAL WAS THE SECOND LOOP'S INDEX FORM, and it is worth
 * generalising (docs/agbcc-codegen.md, wave 57).  The inherited draft wrote the
 * second clear as a RELATIVE offset:
 *
 *     for (i = 0; i < 0xa0 * 2; i++)
 *         gUnknown_0202F8DC[0xa0 * 2 + i] = 0;
 *
 * and this file writes it as an ABSOLUTE range:
 *
 *     for (i = 0xa0 * 2; i < 0xa0 * 4; i++)
 *         gUnknown_0202F8DC[i] = 0;
 *
 * Both emit the SAME loop -- base + 0x280, count 0x140, check_dbra_loop's
 * `subs; cmp #0; bne` -- but only the absolute form leaves the shared `0` for
 * the eight u8 stores to be materialised at the FIRST `strb`, in r1, which is
 * the ROM.  With the relative form agbcc defines that zero at the second loop's
 * EXIT instead, and everything else in the diff followed from that one def:
 * the early zero takes r3, so `proc` cannot have r3 and moves to r4, the
 * -fforce-addr base moves r4 -> r5, and the prologue becomes
 * `push {r4, r5, lr}` instead of `push {r4, lr}`.  64 of 232 bytes differed and
 * every one of them was that cascade -- the instruction stream and the pool
 * order were already correct.
 *
 * So: a low byte-identity score on a size-exact candidate can be ONE register
 * too many, and the fix can be an index spelling that changes no instruction.
 * The wave-35 note's two ruled-out axes (separate `i`/`j`, explicit walking
 * pointers) were both real dead ends -- separate loop variables do remove the
 * early zero but then pin the second loop's exhausted down-counter -- but the
 * axis that mattered, relative vs absolute subscript, had not been tried.
 *
 * gUnknown_081CC030 in the asm is NOT a global: the ROM word there is
 * 0x0202F8DC, i.e. agbcc's own -fforce-addr address constant for
 * gUnknown_0202F8DC.  Naming the array directly, as here, reproduces the
 * two-level load in the first loop and the direct symbol load in the second;
 * the `.rodata` relocation in the diff is the expected form.
 *
 * PolygonWipe_HBlankHandler is an HBlank handler installed through SetHBlankInterruptHandler; it is now
 * declared in include/unknown-functions.h. */

struct Unk73A00Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

void PolygonWipe_Init(struct Unk73A00Proc *proc)
{
    int i;

    for (i = 0; i < 0xa0 * 2; i++)
        gUnknown_0202F8DC[i] = 0;

    for (i = 0xa0 * 2; i < 0xa0 * 4; i++)
        gUnknown_0202F8DC[i] = 0;

    proc->unk58 = -4;

    gUnknown_0202FDDC = gUnknown_0202FDE4 = gUnknown_0202F8DC;
    gUnknown_0202FDE0 = gUnknown_0202F8DC + 0xa0 * 2;

    gDispIo.disp_ct.win0_enable = 1;
    gDispIo.disp_ct.win1_enable = 1;
    gDispIo.disp_ct.objwin_enable = 0;

    gUnknown_03002B40 = 0;
    gUnknown_03002EFC = 0;
    gUnknown_03002B4C = 0;
    gUnknown_03002B44 = 0xa0;
    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0;
    gUnknown_030024E4 = 0;
    gUnknown_030020B8 = 0xa0;

    gUnknown_030030A4.bits.win0_enable_blend = 1;
    gUnknown_030030A4.bits.win1_enable_blend = 1;
    gUnknown_030030DC.bits.win1_enable_blend = 1;

    SetHBlankInterruptHandler((int)PolygonWipe_HBlankHandler);
}
asm(".global sub_08073A00\n.thumb_set sub_08073A00, PolygonWipe_Init\n");
