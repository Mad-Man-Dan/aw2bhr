#include "global.h"
#include "hardware.h"

/* Wave 80 (W80-D): the classified `cmp r6,#64` hunk is CLOSED by the W80-C
 * fold rule, not by a statement split. `gUnknown_084C36E4[t] + 1 +
 * gUnknown_030044E0->unk65` is `(VAR + CON) + ARG1`, which fold's
 * `associate:` rewrites to `table[t] + (unk65 + 1)` -- so the table address
 * is expanded first and the member load is deferred, exactly the draft's
 * hunk. Writing `gUnknown_030044E0->unk65 + 1 + gUnknown_084C36E4[t]` is the
 * same rule the other way round: it folds to `unk65 + (table[t] + 1)`, whose
 * expansion forms the unk65 ADDRESS first, then the table load plus one, then
 * the deferred unk65 load and the add -- the ROM's instruction order, byte
 * for byte (compile_probe h1). The wave-77 header below said "the operand
 * order of the sum itself cannot move it, since the ROM loads the table byte
 * first and adds 1 to it" -- that reads the emitted order back into the
 * source, and fold makes the emitted order the OPPOSITE of the source order.
 *
 * Wave 77 (W77-C): configured baseline is 924/920 (+4), 58.8%, 379 differing
 * bytes -- a large improvement on the wave-66 +16, and the launch brief's "-2"
 * is wrong in sign as well as size.
 *
 * The two `R_ARM_ABS32 .rodata` relocations against the ROM's
 * gUnknown_0812A284 / gUnknown_0812A288 are CORRECT and are not part of the
 * residual: they are this unit's own -fforce-addr words, and the promotion
 * carries them (see the wave-55 note below).
 *
 * ONE HUNK CLASSIFIED, not fixed. In the `cmp r6, #64` block the ROM forms the
 * `unk65` member address FIRST (`ldr r4,[r7,#0]; ldr r2,[r4,#0]; adds r1,r2,#0;
 * adds r1,#101`) and only then the table address (`ldr r0,[pc,#64];
 * adds r0,r6,r0`), while this draft hoists the table address above the pointer
 * global's load entirely (`ldr r0,[pc,#72]; adds r6,r6,r0` before
 * `ldr r4,[r7,#0]`). Instruction COUNT is equal, so this hunk is not the +4.
 * It is the W43-I axis ("a pointer global inside an ADDRESS expression is
 * loaded LATE -- give it its own statement") on the line
 * `gUnknown_084C36E4[t] + 1 + gUnknown_030044E0->unk65 > ...`; the operand
 * order of the sum itself cannot move it, since the ROM loads the table byte
 * first and adds 1 to it, which is what the current spelling already says. */
/* PARKED, wave 55 (W55-H).
 * WAVE 66 ADDENDUM: spelling the outer repetition as the ROM-shaped labeled
 * goto loop reduced the configured draft from 948/920 (+28) to 936/920 (+16).
 * The explicit nested palette-index if-chain now reproduces the ROM's 0,1,2,3
 * assignment order. The remaining loop region is +18 bytes while the palette
 * argument is 4 bytes short (missing the ROM's copy plus u16 truncation), with
 * final alignment accounting for the net +16. int/u16/u8 `c` and a separate
 * u16 palette local all compiled identically.
 *
 * Historical wave-55 note follows: one attempt -- ran out of wave budget, not out of
 * ideas. 920 bytes; the prologue, both switches, every arm of the inner
 * `t` switch, the inner do/while and the whole 0x30 block came out matching on
 * the first try. Everything below is a READOUT, not a guess: it was taken off
 * the ROM before the draft was written and none of it was contradicted by the
 * diff. The next agent should start from this body, not from the assembly.
 *
 * THE VOCABULARY, all of it settled and already in the headers this wave:
 *  - gUnknown_0812A284 is the -fforce-addr word for &gUnknown_030044E0, and
 *    gUnknown_0812A288 is the one for &gpKeySt (unknown-globals.h already says
 *    no gUnknown_03002EE0 can be declared). Name both targets directly; the
 *    promotion needs "rodata": ["0x0812A284", "0x0812A288"].
 *  - gUnknown_030044E0 is NOT cached in a local. The ROM re-loads it through
 *    the force-addr word at essentially every statement, which is what writing
 *    `gUnknown_030044E0->...` everywhere produces.
 *  - +0x1e is a HALFWORD here (`strh`, `ldrh` and `ldrsh` all appear) while the
 *    shared struct spells it `u8 unk1e` on the strength of sub_0804A64C's
 *    `ldrb`. Do NOT retype the shared struct -- c_0804A260.c already worked
 *    this out and the answer is the view struct below, copied from that file
 *    verbatim. The `ldrb` at the top is agbcc narrowing the halfword load
 *    because sub_0804A18C takes u8, so the shared spelling is correct THERE and
 *    the view spelling is correct for the stores. Both appear in this body and
 *    both were right.
 *  - One member reads two ways in one statement: `ldrh` for the arithmetic and
 *    `movs rI,#0x1e; ldrsh` for the compare. That is one `s16` member with the
 *    load width picked per use, not two members.
 *  - sub_0804A18C's argument is `unk20 * 15 + unk1e` and appears FOUR times.
 *    It is the single most repeated shape in the function.
 *
 * REMAINING DIFF -- `size: candidate is 948 bytes, original is 920 (+28)`.
 * Not a multiple of 4, so this is NOT a pool-word-count problem: it is
 * instruction layout, in two spots, both in the tail. Everything before
 * `_0804AA70` is aligned.
 *
 * 1. THE OUTER LOOP-BACK. The ROM's `_0804AA70` is
 *      if (unk5c != 0) goto after;
 *      if (sub_0804A18C(...) != 0x24) goto after;
 *      goto _0804A946;
 *    i.e. the ENTIRE key block from `_0804A946` down is one do/while whose
 *    condition is `unk5c == 0 && sub_0804A18C(...) == 0x24`. This body spells
 *    it that way and gcc still laid the second conjunct out in the wrong place,
 *    so the candidate is longer. The `_0804A946` target is the
 *    `gpKeySt->unk02 & 0x30` test, NOT the `& 0xf0` guard above it -- the
 *    `& 0xf0` guard is outside the loop and is only tested once. Getting the
 *    loop's TOP right is the whole of this defect; suspect a `goto`-shaped
 *    source here, since the doc's "A GOTO LOOP IS INVISIBLE TO loop.c" chapter
 *    describes exactly the layout the ROM has.
 *
 * 2. THE PALETTE INDEX. Two sub-defects, both small:
 *    (a) the `c` chain comes out `movs #2` before `movs #3` in the ROM and
 *        `movs #3; bne; movs #2` here -- an if/else-chain arm inversion, see
 *        the "BLANK arm of an `if` emitted FIRST" chapter.
 *    (b) the ROM keeps the `(u16)` truncation (`lsls #21; lsrs #16`) where this
 *        body's is elided to a bare `lsls #5`, so agbcc could prove `c` small
 *        here and could not there. Read it as `(u16)((c + 0x10) * 0x20)`; the
 *        cast IS in the original. Fixing (a) may well fix (b), since the
 *        inverted chain is what let gcc track c's constants.
 *
 * NOT tried: nothing. This is one attempt.
 */

/* Second view of the object gUnknown_030044E0 points at, for the HALFWORD at
 * +0x1e that the shared struct spells `u8 unk1e` plus filler. Identical to
 * c_0804A260.c's, deliberately: that file documents why the shared model cannot
 * be retyped and why a bare `*(s16 *)&...` cast makes agbcc reload the global
 * pointer where a cast through a struct does not. */
struct Unk030044E0View
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x41 - 0x20];
    /* 0x41 */ u8 unk41[0x17];
};

void sub_0804A760(void)
{
    register int flag asm("r8");
    register int t asm("r6");
    register int u asm("r4");
    u16 i;
    u8 *dst;
    register int c asm("r3");

    flag = 0;
    t = sub_0804A18C(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);

    switch (gUnknown_030044E0->unk63)
    {
    case 1:
        gUnknown_030044E0->unk63 = flag;
        break;
    case 2:
        if (gUnknown_03002EE4 == 1)
        {
            gUnknown_030044E0->unk63 = flag;
            break;
        }
        dst = (u8 *)gUnknown_030044E0->unk58;
        sub_08013034(gUnknown_030044E0->unk2c);
        for (i = 0; i < gUnknown_030044E0->unk5f; i++)
            dst[i] = gUnknown_030044E0->unk2c[i];
        sub_08015C30(gUnknown_03001FBC);
        return;
    case 3:
        if (gUnknown_03002EE4 != 1)
        {
            sub_08015C30(gUnknown_03001FBC);
            return;
        }
        gUnknown_030044E0->unk63 = flag;
        break;
    }

    switch (gpKeySt->pressed & 0xf)
    {
    case 8:
        ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 0xe;
        gUnknown_030044E0->unk20 = 5;
        sub_0804A6D8();
        return;
    case 1:
        switch (t)
        {
        case 0x23:
            sub_0804A6D8();
            return;
        default:
            if (gUnknown_030044E0->unk65 + 1 + gUnknown_084C36E4[t] > gUnknown_030044E0->unk60
             || gUnknown_030044E0->unk5d > gUnknown_030044E0->unk5f - 1)
            {
                sub_0803B4DC(0x68);
                return;
            }
            sub_0803B4DC(0x65);
            sub_0804A64C();
            sub_0804A1E4(0);
            gUnknown_030044E0->unk5d++;
            return;
        case 0x40:
            gUnknown_030044E0->unk66 = ((gUnknown_030044E0->unk66 + 1) & 0xff) | 0x80;
            gUnknown_030044E0->unk67 = 0;
            return;
        case 0x24:
            sub_0803B4DC(0x65);
            gUnknown_030044E0->unk63 = 3;
            sub_0804A1E4(0);
            sub_080193B0(gUnknown_084C3A5C);
            return;
        case 0x25:
            if (gUnknown_030044E0->unk5d != 0)
            {
                gUnknown_030044E0->unk5d--;
                sub_0804A68C();
                sub_0804A1E4(0);
                sub_0803B4DC(0x66);
                return;
            }
            sub_0803B4DC(0x68);
            return;
        }
    case 2:
        if (gUnknown_030044E0->unk5d != 0)
        {
            gUnknown_030044E0->unk5d--;
            sub_0804A68C();
            sub_0804A1E4(0);
            sub_0803B4DC(0x66);
        }
        return;
    }

    if ((gpKeySt->held & 0xf0) == 0)
        return;

key_loop:
    {
        if (gpKeySt->repeated & 0x30)
        {
            u = sub_0804A18C(gUnknown_030044E0->unk20 * 15 + gUnknown_030044E0->unk1e);
            switch (u)
            {
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x40:
                break;
            default:
                u = 0;
                break;
            }

            do
            {
                if (gpKeySt->repeated & 0x10)
                    ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e =
                        (((struct Unk030044E0View *)gUnknown_030044E0)->unk1e > 0xd)
                            ? 0
                            : ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e + 1;
                else
                    ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e =
                        (((struct Unk030044E0View *)gUnknown_030044E0)->unk1e == 0)
                            ? 0xe
                            : ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e - 1;
            }
            while (sub_0804A18C(gUnknown_030044E0->unk20 * 15
                + gUnknown_030044E0->unk1e) == u);

            switch (sub_0804A18C(gUnknown_030044E0->unk20 * 15
                + gUnknown_030044E0->unk1e))
            {
            case 0x40:
                ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 2;
                break;
            case 0x24:
                ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 5;
                break;
            case 0x25:
                ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 9;
                break;
            case 0x23:
                ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 0xd;
                break;
            }
            flag = 1;
        }

        if (gpKeySt->repeated & 0xc0)
        {
            if (gpKeySt->repeated & 0x80)
                gUnknown_030044E0->unk20 = (gUnknown_030044E0->unk20 > 4)
                    ? 0 : gUnknown_030044E0->unk20 + 1;
            else
                gUnknown_030044E0->unk20 = (gUnknown_030044E0->unk20 == 0)
                    ? 5 : gUnknown_030044E0->unk20 - 1;
            flag = 1;
        }
    }

    if (gUnknown_030044E0->unk5c != 0)
        goto after_key_loop;
    if (sub_0804A18C(gUnknown_030044E0->unk20 * 15
        + gUnknown_030044E0->unk1e) == 0x24)
        goto key_loop;

after_key_loop:

    if (flag == 1)
    {
        if (t == 0x40 || t == 0x24 || t == 0x25 || t == 0x23)
        {
            c = 0;
            if (t != 0x40)
            {
                c = 1;
                if (t != 0x24)
                {
                    c = 2;
                    if (t != 0x25)
                        c = 3;
                }
            }
            ApplyPaletteExt(gUnknown_0812B21C, (u16)((c + 0x10) * 0x20), 0x20);
        }
        gUnknown_030044E0->unk22 = 0x1e;
        gUnknown_030044E0->unk24 = 7;
        sub_0803B4DC(0x67);
    }
}
