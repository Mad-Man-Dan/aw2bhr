#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073E0C.
 * sub_08073E0C @ 0x08073E0C
 */

#include "hardware.h"
#include "proc.h"
/* MATCHED, wave 75, 388/388 configured bytes. Short-lived fixed-register
 * locals preserve the ROM's first-sample destination, min/max copy order,
 * odd-path cursor accumulator, and final DMA-control copy. Empty read/write
 * constraints keep those copies distinct without emitting instructions.
 * The .rodata relocation at 0x081CC034 is this unit's force-address word for
 * gUnknown_0202FDE4 and is placed by the promotion metadata. */

/* Historical derivation: after a wave-55 handback to W55-E, W55-A's diagnosis
 * below is CORRECT and W55-A's draft is unchanged; the handback's own diagnosis
 * was wrong, and that refutation is the useful result.
 *
 * THE HANDBACK CLAIMED: "the draft wrote a file-local literal where the ROM
 * references the existing global gUnknown_081CC034", citing the reloc pair
 *     original   ec: R_ARM_ABS32 gUnknown_081CC034
 *     candidate  e8: R_ARM_ABS32 .rodata
 * and predicting that naming the global collapses a 199-byte diff to a few
 * bytes. REFUTED, and the prediction is backwards. This draft ALREADY names
 * the global honestly -- it writes `gUnknown_0202FDE4`, and 0x081CC034 is this
 * unit's own -fforce-addr copy of &gUnknown_0202FDE4, exactly as
 * src/decomp/c_08073658.c records for 0x081CC028 and 0x081CC024/2C. The
 * `.rodata` reloc IS the correct output of the honest spelling; the promotion
 * carries it as a rodata entry and tools/split_rodata.py carves the duplicate.
 * There is nothing to fix in the data spelling and no symbol to declare.
 *
 * THE TELL THAT DISTINGUISHES THE TWO CASES, since the brief's new "#1 cause
 * of misses" screen cannot: compare the FULL pool word lists, then check the
 * SIZE. Here the two lists are identical in symbol, order and count -- the only
 * difference is the first entry's spelling -- and the candidate is 4 bytes
 * SHORT. A genuine local-literal defect ADDS a word or changes the list; an
 * honest -fforce-addr word does neither. When the lists match and the size does
 * not, the pool is a symptom and the missing bytes are CODE. `first difference
 * at +0xc` is the confirmation: +0xc is the first `ldr rN,[pc,#X]`, so the
 * entire 199-byte "diff" and the 47.7% score are the positional shadow of two
 * missing instructions ~0xa0 bytes later.
 *
 * W55-E then re-measured the real residual (the min/max block W55-A names) with
 * nine more spellings in a reduced probe that reproduces W55-A's shape exactly.
 * All nine fail, and they fail informatively -- see the agbcc MIN/MAX chapter
 * added to docs/agbcc-codegen.md (wave 55). In short: `a >= b ? a : b` is
 * folded to MAX_EXPR, whose expansion compares the TARGET COPY, so no spelling
 * of it can produce the ROM's THIRD register holding `va` and shared by both
 * compares; and any spelling that breaks the fold (a distinct condition
 * variable, `(u32)` casts on the condition only) drops to the COND_EXPR path,
 * which presets the ACCUMULATOR instead of the value and is 4 instructions per
 * group, not 6. Additionally ruled out this pass: `int va` (10 insns, and the
 * second group loses its preset entirely), nested `?:` (6 insns/group),
 * `(va < maxa) ? maxa : va` and `((ca = va) < maxa) ? maxa : va` (both fold
 * back to the identical MAX_EXPR output). `u8 va` produces the right INSTRUCTION
 * COUNT but puts the extra copy at the DEFINITION rather than in the min/max
 * block, and costs a fourth high register.
 *
 * Original W55-A note follows.
 *
 * PARKED at 4 bytes, wave 55 (W55-A). Everything outside the four min/max
 * accumulator updates is byte-exact, including register allocation. The ROM
 * spends ONE EXTRA `adds rN, rV, #0` per value: it copies va (r3) into r2 and
 * compares r2 in BOTH the max and the min, and copies vb (r4) into r1 and
 * compares r1 in both -- i.e. the compare operand has its own pseudo, distinct
 * from the value that feeds the two ternary presets. This candidate compares
 * the home register in the first of each pair and the preset copy in the
 * second, so it is 2 instructions / 4 bytes short.
 *
 * Ruled out by probe: `u8 va` (creates the copy but at the DEFINITION, and
 * spills two values to the stack); `>`/`<` instead of `>=`/`<=` (gives bhi/blo
 * where the ROM has bhs/bls); `if (va > maxa) maxa = va;` (3 insns per group,
 * not 6); an explicit second local for the compare operand
 * (`ca = va; maxa = (ca >= maxa) ? va : maxa;`) -- copy propagation collapses it
 * and flips the ternary to preset the ACCUMULATOR instead, 4 insns per group.
 *
 * Everything else in this function is settled and byte-exact, and three of the
 * four things that got it there are written up in docs/agbcc-codegen.md
 * (wave 55): the `int t; t -= 0x80; (u8)t` split that produces `subs #0x80`
 * rather than `add #0x80`; the pointer-vs-address-constant reading of
 * gUnknown_086141FC; and the wrap `proc->unk58 = i + 1; ... = i + 1 - 0x630`,
 * which must keep the OLD value live so combine folds the pool word to -0x62F
 * (and -0x550 in the else arm) rather than -0x630.
 *
 * PROMOTION NEEDS A .rodata WORD PLACED: 0x081CC034 holds 0x0202FDE4, this
 * unit's own -fforce-addr copy of &gUnknown_0202FDE4 (same class as the
 * 0x081CC024/28/2C words recorded in c_08073658.c).
 *
 * The PCM oscilloscope: two channels of struct SoundInfo::pcmBuffer are walked
 * one sample per scanline, the top channel becomes REG_WIN1H via DMA0 (X1 =
 * (sample >> 1) - 0x20, X2 = 0xF0), and each channel's peak-to-peak range is
 * published in gUnknown_0202FDE8 / gUnknown_0202FDEA. proc->unk58 is the read
 * cursor, wrapping modulo 0x630; when gGameClock is odd the whole render
 * is skipped and the cursor just advances a frame's worth (0xE0). */

struct Unk73E0CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x58);
    /* 58 */ int unk58;
};

void SoundScope_Loop(struct Unk73E0CProc *proc)
{
    int i;
    int y;
    int t;
    u32 maxb;
    u32 maxa;
    u32 minb;
    u32 mina;
    u32 va;
    u32 vb;
    register u32 ca asm("r2");
    register u32 cb asm("r1");

    gUnknown_0202FDE4 = gUnknown_0202F8DC;

    gDispIo.disp_ct.win1_enable = 1;

    gUnknown_03002B68 = 0;
    gUnknown_03002B30 = 0;
    gUnknown_030024E4 = 0xF0;
    gUnknown_030020B8 = 0xA0;

    maxb = 0;
    maxa = 0;
    minb = 0xFF;
    mina = 0xFF;

    if ((gGameClock & 1) == 0)
    {
        for (y = 0; y <= 0xDF; y++)
        {
            i = proc->unk58;

            if (y <= 0x9F)
            {
                {
                    register int first asm("r1");
                    first = gUnknown_086141FC->pcmBuffer[0][i + 0x630];
                    first -= 0x80;
                    va = (u8)first;
                }

                t = gUnknown_086141FC->pcmBuffer[0][i];
                t -= 0x80;
                vb = (u8)t;

                ((u16 *)gUnknown_0202FDE4)[y] = (((va >> 1) - 0x20) << 8) | 0xF0;

                {
                    register u32 result asm("r0");
                    result = va;
                    asm("" : "+r" (result));
                    ca = va;
                    maxa = (ca < maxa) ? maxa : result;
                }
                {
                    register u32 result asm("r1");
                    result = va;
                    asm("" : "+r" (result));
                    mina = (ca > mina) ? mina : result;
                }
                {
                    register u32 result asm("r0");
                    result = vb;
                    asm("" : "+r" (result));
                    cb = vb;
                    maxb = (cb < maxb) ? maxb : result;
                }
                minb = (cb > minb) ? minb : vb;
            }

            proc->unk58 = i + 1;

            if (proc->unk58 > 0x62F)
                proc->unk58 = i + 1 - 0x630;
        }

        gUnknown_0202FDE8 = (maxa - mina) & 0xFF;
        gUnknown_0202FDEA = (maxb - minb) & 0xFF;
    }
    else
    {
        register int old asm("r2");
        old = proc->unk58;

        proc->unk58 = old + 0xE0;

        if (proc->unk58 > 0x62F)
        {
            register int wrap asm("r0");
            wrap = old + 0xE0 - 0x630;
            proc->unk58 = wrap;
        }
    }

    REG_DMA0CNT_H = 0;
    REG_DMA0SAD = (u32)gUnknown_0202FDE4;
    REG_DMA0DAD = (u32)&REG_WIN1H;
    REG_DMA0CNT_L = 1;
    {
        register u32 control asm("r3");
        register u32 output asm("r0");
        control = 0xA240;
        asm("" : "+r" (control));
        output = control;
        REG_DMA0CNT_H = output;
    }
}
asm(".global sub_08073E0C\n.thumb_set sub_08073E0C, SoundScope_Loop\n");
