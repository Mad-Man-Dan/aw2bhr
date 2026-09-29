#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08079618.
 * sub_08079618 @ 0x08079618, sub_0807974C @ 0x0807974C, sub_08079B04 @ 0x08079B04, sub_08079EA4 @ 0x08079EA4, sub_08079FAC @ 0x08079FAC, sub_0807A0C4 @ 0x0807A0C4
 */

#include "proc.h"
struct Unk8079618
{
    /* 00 */ STRUCT_PAD(0x00, 0x2c);
    /* 2c */ int unk2c;
    /* 30 */ STRUCT_PAD(0x30, 0x34);
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4c);
    /* 4c */ s16 unk4c;
};
#include "hardware.h"
struct Unk807974C
{
    /* 00 */ STRUCT_PAD(0x00, 0x38);
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ int unk60;
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
    /* 6a */ s16 unk6a;
};
struct Unk8079B04
{
    /* 00 */ STRUCT_PAD(0x00, 0x38);
    /* 38 */ int unk38;
    /* 3c */ STRUCT_PAD(0x3c, 0x58);
    /* 58 */ int unk58;
    /* 5c */ int unk5c;
    /* 60 */ int unk60;
    /* 64 */ STRUCT_PAD(0x64, 0x6a);
    /* 6a */ s16 unk6a;
};
/* PARKED -- see data/parked.json. Everything below is believed correct; the
 * blocker is that `sub_08079B38` has no global symbol in either build (the
 * splitter merged 0x08079B38 into sub_08079B04), so this cannot be promoted and
 * trymatch cannot resolve the call. Nothing here is a codegen question.
 *
 * A proc body. The affine denominator is the same `d != 0 ? d : 2` guard as the
 * promoted c_08027B68.c, but with d = 0x200 - proc->unk34, which agbcc strength-
 * reduces to `cmp unk34, #0x200` -- so the ROM's `movs r1,#2; cmp r2,r4; beq;
 * subs r1,r4,r2` is that one conditional expression and not a range test.
 * The angle is the literal 0 here, unlike WorldMapReticle_ShrinkLoop: no `ands #0xff`.
 */

struct Unk8079EA4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ int unk30;
    /* 34 */ int unk34;
    /* 38 */ STRUCT_PAD(0x38, 0x40);
    /* 40 */ int unk40;
    /* 44 */ STRUCT_PAD(0x44, 0x4c);
    /* 4c */ u16 unk4c;
    /* 4e */ STRUCT_PAD(0x4e, 0x52);
    /* 52 */ u16 unk52;
};
/* PARKED at 99.29%, SIZE EXACT (280/280). The residual is TWO bytes and it is
 * the immediate field of ONE `bl`. This function is BELIEVED CORRECT as C.
 *
 * THE RESIDUAL IS IDENTICAL TO ITS SIBLING ResultsSubScreen_SlideOutShowCo_Loop's, byte for byte and
 * cause for cause. Confirmed this wave (W52-E) by running both diffs together:
 *
 *     original    bl 3ec <_08079B38>       (no relocation)
 *     candidate   bl 0 <sub_08079B38>      + R_ARM_THM_CALL sub_08079B38
 *
 * 0x08079B38 is a genuine function entry that lives INSIDE another function's
 * body as far as the split is concerned:
 *
 *     asm/code-0806CFC8.s:26214      b _08079B38
 *     asm/code-0806CFC8.s:26228   _08079B38:
 *     asm/code-0806CFC8.s:26648      bl _08079B38
 *     asm/code-0806CFC8.s:26773      bl _08079B38
 *     asm/code-0806CFC8.s:26914      bl _08079B38
 *
 * All three call sites are in the SAME assembly file, so in the original build
 * the `bl` was resolved by the assembler with the offset baked in and emitted
 * no relocation at all. A per-function `try_match` compiles exactly one
 * function, so its call to sub_08079B38 is necessarily external and
 * necessarily carries a R_ARM_THM_CALL. **No C source spelling can close
 * this** -- it is a property of the compilation unit, not of the source.
 *
 * NOT a park to be re-attacked at the C level, and NOT a candidate for the
 * permuter. The two remaining bytes disappear the moment the whole
 * ResultsSubScreen_CountUpScores_Loop.s unit is built as one translation unit, because the assembler
 * then resolves the branch locally. `trymatch --unit` is the oracle that can
 * gate it; that needs drafts for every function in the unit. Still missing as
 * of this wave: ResultsSubScreen_CountUpScores_Loop, sub_08079B04. (ResultsSubScreen_RankStamp_Loop, ResultsSubScreen_SlideOutShowCo_Loop and
 * this one are drafted.)
 *
 * The underlying cause is a SPLITTER artefact worth fixing centrally:
 * tools/split_asm.py did not recognise 0x08079B38 as a function start,
 * because what precedes it is two bytes of `b _08079B38` in front of the
 * previous function's 13-word literal pool rather than a normal epilogue. It
 * therefore merged it into sub_08079B04 and no global symbol exists at that
 * address in either build. Do not "fix" this by declaring `_08079B38` -- that
 * is a false match that fails to link. */

struct Unk8079FACProc
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[0x08];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x04];
    /* 0x52 */ u16 unk52;
};
/* PARKED at 99.18%, SIZE EXACT (244/244), and the TWO differing bytes are the
 * immediate field of ONE `bl`. This function is BELIEVED CORRECT as C.
 *
 * CONFIRMED THIS WAVE (W52-E) BY DIFFING IT AGAINST ITS SIBLING: ResultsSubScreen_WaitForButton_Loop
 * has the IDENTICAL two-byte residual, same callee, same cause. Whatever else
 * is true of these two functions, they stand or fall together, and neither one
 * has anything open at the C level.
 *
 *     original    bl 3ec <_08079B38>       (no relocation)
 *     candidate   bl 0 <sub_08079B38>      + R_ARM_THM_CALL sub_08079B38
 *
 * 0x08079B38 is a genuine function entry that the split placed inside another
 * function's body:
 *
 *     asm/code-0806CFC8.s:26214      b _08079B38
 *     asm/code-0806CFC8.s:26228   _08079B38:
 *     asm/code-0806CFC8.s:26648      bl _08079B38
 *     asm/code-0806CFC8.s:26773      bl _08079B38
 *     asm/code-0806CFC8.s:26914      bl _08079B38
 *
 * All three call sites are in the SAME assembly file, so in the original build
 * the assembler resolved the branch locally with the offset baked in and
 * emitted no relocation. A per-function `try_match` compiles exactly one
 * function, so its call is necessarily external and necessarily relocated.
 * **No C source spelling can close this** -- it is a property of the
 * compilation unit, not of the source. Do not send it to the permuter, and do
 * not "fix" it by naming the callee `_08079B38`: that is a false match that
 * fails to link, because no global symbol exists at that address in either
 * build.
 *
 * The two bytes disappear the moment the whole ResultsSubScreen_CountUpScores_Loop.s unit is built as
 * one translation unit. `trymatch --unit` is the oracle that can gate it, and
 * that needs a draft for every function in the unit. Drafted: ResultsSubScreen_RankStamp_Loop,
 * ResultsSubScreen_WaitForButton_Loop, this one. Still missing: ResultsSubScreen_CountUpScores_Loop, sub_08079B04.
 * The root cause is a tools/split_asm.py artefact -- it did not recognise
 * 0x08079B38 as a function start, because what precedes it is two bytes of
 * `b _08079B38` in front of the previous function's 13-word literal pool
 * rather than a normal epilogue.
 *
 * Two things converted here that are worth carrying:
 *   - gUnknown_03001400 is VOLATILE. This function writes it and tests
 *     `(s16)gUnknown_03001400` in the very next statement; a non-volatile
 *     global is store-forwarded and the ROM's `ldrh` reload disappears, which
 *     was 4 bytes (the load plus the alignment word it pulls in). The
 *     qualifier is now on the declaration and all nine promoted users were
 *     re-verified byte-for-byte.
 *   - `d = proc->unk34 - 0x58;` must be its OWN statement. Written inline as
 *     `proc->unk40 - (proc->unk34 - 0x58)`, fold associates it into
 *     `(proc->unk40 + 0x58) - proc->unk34` and the two `subs` come out with
 *     their operands swapped. A separate statement blocks the association.
 *
 * Also settled: `PutResultsBanner(proc, v)` takes the Interpolate result that is
 * already in r1 -- the ROM spends no instruction on it, which is why the
 * result is bound to a local and not re-read from +0x34 (the NEXT call,
 * sub_08079B38, does re-read it). */
#include "global.h"
struct Unk807A0C4
{
    /* 0x00 */ u8 filler_00[0x34];
    /* 0x34 */ int unk34;
    /* 0x38 */ u8 filler_38[0x04];
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[0x08];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ u8 filler_4e[0x04];
    /* 0x52 */ u16 unk52;
};

void ResultsSubScreen_LabelsSlideIn_Loop(struct Unk8079618 *proc)
{
    int off;
    int i;

    PutResultsBanner(proc, 0);

    off = DivRem(Div((u16)proc->unk38, 3), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_0822AC60 + off), 0x2b8, 2);

    proc->unk38++;

    proc->unk34 = Interpolate(4, 0, 0xb4, proc->unk2c, 0x28);

    PutSprite(0, proc->unk34 - 0xac, 0x50, gUnknown_08615C20, 0x5300);
    PutSprite(0, proc->unk34 - 0xac, 0x60, gUnknown_08615C20, 0x5310);
    PutSprite(0, proc->unk34 - 0xac, 0x70, gUnknown_08615C20, 0x5320);
    PutSprite(0, proc->unk34 - 0xac, 0x80, gUnknown_08615C2E, 0x52fc);
    PutSprite(0, proc->unk34 - 0xac, 0x88, gUnknown_08615C20, 0x5330);

    for (i = 0; i < 3; i++)
        PutSprite(0, proc->unk34 - 0x24, i * 16 + 0x50, gUnknown_0848B6C6, 0x52d0);

    PutSprite(0, proc->unk34 - 0x24, 0x88, gUnknown_0848B6C6, 0x52d0);

    if (proc->unk2c > 0x27)
    {
        if (proc->unk4c > 7)
        {
            proc->unk4c = 0;
            Proc_Break(proc);
        }

        proc->unk4c++;
    }
    else
    {
        proc->unk2c++;
    }
}
asm(".global sub_08079618\n.thumb_set sub_08079618, ResultsSubScreen_LabelsSlideIn_Loop\n");

void ResultsSubScreen_CountUpScores_Loop(struct Unk807974C *proc)
{
    int off;

    off = DivRem(Div((u16)proc->unk38, 3), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_0822AC60 + off), 0x2b8, 2);

    proc->unk38++;

    PutResultsBanner(proc, 0);

    PutSprite(0, 8, 0x50, gUnknown_08615C20, 0x5300);
    PutSprite(0, 8, 0x60, gUnknown_08615C20, 0x5310);
    PutSprite(0, 8, 0x70, gUnknown_08615C20, 0x5320);
    PutSprite(0, 8, 0x80, gUnknown_08615C2E, 0x52fc);
    PutSprite(0, 8, 0x88, gUnknown_08615C20, 0x5330);

    PutSprite(0, 0x90, 0x50, gUnknown_0848B6C6, 0x52d0);
    PutSprite(0, 0x90, 0x60, gUnknown_0848B6C6, 0x52d0);
    PutSprite(0, 0x90, 0x70, gUnknown_0848B6C6, 0x52d0);
    PutSprite(0, 0x90, 0x88, gUnknown_0848B6C6, 0x52d0);

    if (proc->unk64 < proc->unk58)
        proc->unk64++;

    if (proc->unk66 < proc->unk5c)
        proc->unk66++;

    if (proc->unk68 < proc->unk60)
        proc->unk68++;

    if (proc->unk64 > 0x63)
        PutSprite(0, 0x68, 0x50, gUnknown_0848B690,
                  (DivRem(Div(proc->unk64, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk64 > 9)
        PutSprite(0, 0x74, 0x50, gUnknown_0848B690,
                  (DivRem(Div(proc->unk64, 10), 10) * 4 + 0x2a8) | 0x5000);

    PutSprite(0, 0x80, 0x50, gUnknown_0848B690,
              (DivRem(proc->unk64, 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk66 > 0x63)
        PutSprite(0, 0x68, 0x60, gUnknown_0848B690,
                  (DivRem(Div(proc->unk66, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk66 > 9)
        PutSprite(0, 0x74, 0x60, gUnknown_0848B690,
                  (DivRem(Div(proc->unk66, 10), 10) * 4 + 0x2a8) | 0x5000);

    PutSprite(0, 0x80, 0x60, gUnknown_0848B690,
              (DivRem(proc->unk66, 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk68 > 0x63)
        PutSprite(0, 0x68, 0x70, gUnknown_0848B690,
                  (DivRem(Div(proc->unk68, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk68 > 9)
        PutSprite(0, 0x74, 0x70, gUnknown_0848B690,
                  (DivRem(Div(proc->unk68, 10), 10) * 4 + 0x2a8) | 0x5000);

    PutSprite(0, 0x80, 0x70, gUnknown_0848B690,
              (DivRem(proc->unk68, 10) * 4 + 0x2a8) | 0x5000);

    if (proc->unk4c > 0x6b)
    {
        if (DivRem(proc->unk4c, 2) != 0 && gUnknown_03002B28 != 0)
        {
            gUnknown_03002020++;
            gUnknown_03002B28--;
        }

        if (proc->unk6a > 0x63)
            PutSpriteExt(0, 0x68, 0x488, gUnknown_0848B690,
                         (DivRem(Div(proc->unk6a, 100), 10) * 4 + 0x280) | 0x5000);

        if (proc->unk6a > 9)
            PutSpriteExt(0, 0x74, 0x488, gUnknown_0848B690,
                         (DivRem(Div(proc->unk6a, 10), 10) * 4 + 0x280) | 0x5000);

        PutSpriteExt(0, 0x80, 0x488, gUnknown_0848B690,
                     (DivRem(proc->unk6a, 10) * 4 + 0x280) | 0x5000);
    }

    if (proc->unk64 != proc->unk58 || proc->unk66 != proc->unk5c
        || proc->unk68 != proc->unk60)
    {
        if ((proc->unk4c & 1) != 0)
            PlayMusicOrSfx2(0x7c);
    }

    if (proc->unk4c > 0xaf)
    {
        gUnknown_03002020 = 0x10;
        gUnknown_03002B28 = 8;
        PlayMusicOrSfx2(0x79);
        Proc_Break(proc);
    }

    proc->unk4c++;
}
asm(".global sub_0807974C\n.thumb_set sub_0807974C, ResultsSubScreen_CountUpScores_Loop\n");

/* The slide-in twin of ResultsSubScreen_CountUpScores_Loop @ 0x0807974C: the same results panel with
 * every row's Y displaced by a scroll offset `a`, and each row suppressed once
 * it would have slid past its own threshold. No PutResultsBanner call, no
 * gUnknown_03002B28 drain and no Proc_Break -- those belong to the static
 * version.
 *
 * THE ENTRY POINT IS 0x08079B38, NOT 0x08079B04. The 52 bytes at 0x08079B04
 * (`b _08079B38`, alignment, twelve pool words) are ResultsSubScreen_CountUpScores_Loop's literal
 * pool, which agbcc emits after that function's `bx r0` and inside this unit's
 * next function. tools/split_asm.py read the `b` as a function start. Compiled
 * as one translation unit with ResultsSubScreen_CountUpScores_Loop in address order, the label
 * `sub_08079B04` lands at 0x08079B38 and the whole 2,668-byte unit reproduces
 * -- which is also what turns the three callers' `bl` into the local,
 * unrelocated branch the ROM has.
 */
static void sub_08079B04(ProcPtr proc, u32 a)
{
    int i;
    int d;
    int off;

    off = DivRem(Div((u16)((struct Unk8079B04 *)proc)->unk38, 3), 0x10) * 2;
    ApplyPaletteExt((u16 *)((u8 *)gUnknown_0822AC60 + off), 0x2b8, 2);

    ((struct Unk8079B04 *)proc)->unk38++;

    if (a <= 0x60)
        PutSprite(0, 8, 0x50 - a, gUnknown_08615C20, 0x5300);

    if (a <= 0x70)
        PutSprite(0, 8, 0x60 - a, gUnknown_08615C20, 0x5310);

    if (a <= 0x80)
        PutSprite(0, 8, 0x70 - a, gUnknown_08615C20, 0x5320);

    if (a <= 0x90)
        PutSprite(0, 8, 0x80 - a, gUnknown_08615C2E, 0x52fc);

    if (a <= 0x98)
        PutSprite(0, 8, 0x88 - a, gUnknown_08615C20, 0x5330);

    /* `d` MUST be a local assigned INSIDE the if-block. Written inline as
     * `i * 0x10 - (a - 0x50)` the whole value is linear in `i`, so
     * strength_reduce turns it into a third induction variable, eliminates
     * `i` entirely and check_dbra_loop reverses the counter -- +8 bytes and
     * one extra callee-saved register, which then pushes `proc` out of r7.
     * Binding the subtrahend conditionally makes it a separate pseudo, the
     * giv is no longer recognised, and the ROM's `lsls; adds; subs; subs`
     * comes back exactly. */
    for (i = 0; i < 3; i++)
    {
        if (a <= i * 0x10 + 0x60)
        {
            d = a - 0x50;
            PutSprite(0, 0x90, i * 0x10 - d, gUnknown_0848B6C6, 0x52d0);
        }
    }

    if (a <= 0x98)
        PutSprite(0, 0x90, 0x88 - a, gUnknown_0848B6C6, 0x52d0);

    if (((struct Unk8079B04 *)proc)->unk58 > 0x63 && a <= 0x60)
        PutSprite(0, 0x68, 0x50 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk58, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk58 > 9 && a <= 0x60)
        PutSprite(0, 0x74, 0x50 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk58, 10), 10) * 4 + 0x2a8) | 0x5000);

    if (a <= 0x60)
        PutSprite(0, 0x80, 0x50 - a, gUnknown_0848B690,
                  (DivRem(((struct Unk8079B04 *)proc)->unk58, 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk5c > 0x63 && a <= 0x70)
        PutSprite(0, 0x68, 0x60 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk5c, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk5c > 9 && a <= 0x70)
        PutSprite(0, 0x74, 0x60 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk5c, 10), 10) * 4 + 0x2a8) | 0x5000);

    if (a <= 0x70)
        PutSprite(0, 0x80, 0x60 - a, gUnknown_0848B690,
                  (DivRem(((struct Unk8079B04 *)proc)->unk5c, 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk60 > 0x63 && a <= 0x80)
        PutSprite(0, 0x68, 0x70 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk60, 100), 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk60 > 9 && a <= 0x80)
        PutSprite(0, 0x74, 0x70 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk60, 10), 10) * 4 + 0x2a8) | 0x5000);

    if (a <= 0x80)
        PutSprite(0, 0x80, 0x70 - a, gUnknown_0848B690,
                  (DivRem(((struct Unk8079B04 *)proc)->unk60, 10) * 4 + 0x2a8) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk6a > 0x63 && a <= 0x98)
        PutSprite(0, 0x68, 0x88 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk6a, 100), 10) * 4 + 0x280) | 0x5000);

    if (((struct Unk8079B04 *)proc)->unk6a > 9 && a <= 0x98)
        PutSprite(0, 0x74, 0x88 - a, gUnknown_0848B690,
                  (DivRem(Div(((struct Unk8079B04 *)proc)->unk6a, 10), 10) * 4 + 0x280) | 0x5000);

    if (a <= 0x98)
        PutSprite(0, 0x80, 0x88 - a, gUnknown_0848B690,
                  (DivRem(((struct Unk8079B04 *)proc)->unk6a, 10) * 4 + 0x280) | 0x5000);
}

void ResultsSubScreen_RankStamp_Loop(struct Unk8079EA4Proc *proc)
{
    PutResultsBanner(proc, 0);
    sub_08079B04(proc, 0);

    proc->unk34 = Interpolate(0, 0, 0x100, proc->unk30, 8);

    SetObjAffine(1,
                 Div(COS_Q12(0) * 16,
                     proc->unk34 == 0x200 ? 2 : 0x200 - proc->unk34),
                 Div(-SIN_Q12(0) * 16,
                     proc->unk34 == 0x200 ? 2 : 0x200 - proc->unk34),
                 Div(SIN_Q12(0) * 16,
                     proc->unk34 == 0x200 ? 2 : 0x200 - proc->unk34),
                 Div(COS_Q12(0) * 16,
                     proc->unk34 == 0x200 ? 2 : 0x200 - proc->unk34));

    PutSpriteExt(0, 0x288, (proc->unk40 + 0x38) | 0x700, gUnknown_0848B6A0,
                 ((gUnknown_08615E40[proc->unk52] + 4) << 12) | 0x240);

    if (proc->unk30 > 7)
    {
        proc->unk4c = 0;
        Proc_Break(proc);
    }

    proc->unk30++;
}
asm(".global sub_08079EA4\n.thumb_set sub_08079EA4, ResultsSubScreen_RankStamp_Loop\n");

void ResultsSubScreen_WaitForButton_Loop(struct Unk8079FACProc *proc)
{
    PutResultsBanner(proc, 0);
    sub_08079B04(proc, 0);

    PutSpriteExt(0, 0xA8, (proc->unk40 + 0x58) | 0x400, gUnknown_0848B6A0,
                 ((gUnknown_08615E44[proc->unk52] + 4) << 12) | 0x240);

    proc->unk4c++;

    if (proc->unk4c > 0x3B)
        proc->unk4c = 0x3C;

    if ((gpKeySt->pressed & 9) && proc->unk4c == 0x3C)
    {
        proc->unk4c = 0;

        gUnknown_03001400 = 0xFF10;
        gDispIo.disp_ct.bg2_enable = 0;

        DrawWindowBackgroundOnBg2(0, 0xE, 0x1E, 6);

        gUnknown_030030E0.bits.effect = 1;

        gUnknown_03002020 = 0x10;
        gUnknown_03002B28 = 8;
        gUnknown_03001FFC = gUnknown_03001FFC;

        gUnknown_030030E0.raw &= 0xFFE0;
        gUnknown_030030E0.bits.target1_enable_bd = 0;
        gUnknown_030030E0.raw = (gUnknown_030030E0.raw & 0xE0FF) | 0x800;
        gUnknown_030030E0.bits.target2_enable_bd = 0;

        Proc_Break(proc);
    }
}
asm(".global sub_08079FAC\n.thumb_set sub_08079FAC, ResultsSubScreen_WaitForButton_Loop\n");

void ResultsSubScreen_SlideOutShowCo_Loop(struct Unk807A0C4 *proc)
{
    int v;
    int d;

    v = Interpolate(4, 0, 0xF0, proc->unk4c, 0x30);
    proc->unk34 = v;
    PutResultsBanner(proc, v);
    sub_08079B04(proc, proc->unk34);

    if (proc->unk34 <= proc->unk40 + 0x98)
    {
        d = proc->unk34 - 0x58;
        PutSpriteExt(0, 0xA8, ((proc->unk40 - d) & 0xFF) | 0x400,
                     gUnknown_0848B6A0,
                     ((gUnknown_08615E48[proc->unk52] + 4) << 12) | 0x240);
    }

    gUnknown_03001400 = proc->unk34 - 0xF0;

    if ((s16)gUnknown_03001400 > -0x30)
        gDispIo.disp_ct.bg2_enable = 1;

    if (proc->unk4c > 0x1F)
    {
        proc->unk3c = Interpolate(4, -0x40, 0x40, proc->unk4c - 0x20, 0x10);
        sub_08043C28(proc->unk3c & 0x1FF, 0xA0, 0xB000, 0, 0);
    }

    if (proc->unk4c > 0x2F)
    {
        ResultsScreen_ShowVictoryQuote();
        Proc_Break(proc);
    }

    proc->unk4c++;
}
asm(".global sub_0807A0C4\n.thumb_set sub_0807A0C4, ResultsSubScreen_SlideOutShowCo_Loop\n");
