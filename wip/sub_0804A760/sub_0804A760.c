#include "global.h"
#include "hardware.h"

/* Wave 80 (W80-D). Four named constructs closed in one round, each read off
 * the instruction-level diff and confirmed by compile_probe before the
 * verdict; the remaining residual is the fifth, named at the end.
 *
 *  1. THE `cmp r6,#64` HUNK IS THE W80-C FOLD RULE, NOT A STATEMENT SPLIT.
 *     `gUnknown_084C36E4[t] + 1 + gUnknown_030044E0->unk65` is
 *     `(VAR + CON) + ARG1`, which fold's `associate:` rewrites to
 *     `table[t] + (unk65 + 1)`: the table address is expanded first and the
 *     member load is deferred -- the wave-77 hunk. Writing
 *     `gUnknown_030044E0->unk65 + 1 + gUnknown_084C36E4[t]` folds to
 *     `unk65 + (table[t] + 1)`, whose expansion forms the unk65 ADDRESS
 *     first, then the table load plus one, then the deferred unk65 load and
 *     the add -- the ROM's order, instruction for instruction. The wave-77
 *     note "the operand order of the sum itself cannot move it, since the ROM
 *     loads the table byte first" read the EMITTED order back into the
 *     source; fold makes the emitted order the opposite of the written one.
 *
 *  2. THE SECOND `?:` OF EACH PAIR IS AN if/else STATEMENT. `x == 0 ? K :
 *     x - 1` and `x != 0 ? x - 1 : K` both come out `beq L; subs; b; L: movs
 *     #K` (then-arm falls into the store); the ROM's `bne L; movs #K; b; L:
 *     subs` is what `if (x == 0) x = K; else x = x - 1;` emits -- two stores
 *     cross-jumped at the strh. The FIRST of each pair (`x > N ? 0 : x + 1`)
 *     is a genuine `?:` (the four-store if/else form puts `movs #0` first).
 *     So the original mixes a conditional expression for the increment with
 *     an if/else for the decrement, in both the unk1e and the unk20 pair.
 *
 *  3. `unk66`'s `& 0xff` SURVIVES ONLY AS A MULTI-SET LOCAL. Every single-set
 *     spelling (`((x + 1) & 0xff) | 0x80` inline, a `u8` local, a `(u8)`
 *     cast) lets nonzero_bits prove the mask redundant for the QImode store,
 *     and the `| 0x80` is then re-canonicalised as `| -0x80` (`movs; negs;
 *     adds`). `c = x + 1; c &= 0xff; c |= 0x80; x = c;` keeps the mask
 *     (wave-57 rule: nonzero_bits is tracked only for a single-set pseudo).
 *
 *  4. THE `register ... asm("rN")` PINS WERE WRONG. In the `case 1` default
 *     arm the ROM writes the table sum into r0 (`adds r0, r6, r0`) where the
 *     pinned draft wrote it into r6 (`adds r6, r6, r0`): local-alloc ties an
 *     add's destination to a DYING PSEUDO input (the table address, r0); a
 *     hard-register variable is never a pseudo, so the pin hands the dest
 *     the hard reg instead. A ROM that keeps t's register across a dying use
 *     proves t was an ordinary pseudo. MEASURED: removing all four pins with
 *     fixes 1-3 in place is 924/920, 12.4%, first difference +0x9 -- flag
 *     lands in r7, the three `unk63 = flag` arms cross-jump into one store,
 *     and everything after renumbers. The pins are therefore a COSTUME for
 *     an allocation construct nobody has found (whatever gives flag r8 and
 *     t r6 without a pin); they are kept only because the pinned draft is
 *     the closer measurement, not because they are source.
 *
 *  5. REMAINING (the +4): in the key `do { } while (sub_0804A18C(...) == u)`
 *     loop the ROM reloads the gUnknown_030044E0 force-addr word through r7
 *     in EACH arm (`ldr r0,[r7]; ldr r1,[r0]`) and once more after the strh
 *     (`ldr r5,[r7]`, which then serves the whole post-loop switch). This
 *     draft's loop hoists the word as a fresh literal (`ldr r5, =gUnknown_
 *     030044E0` -- a NEW pool word, the +4) before the loop and uses r5 in
 *     both arms, then reloads via r7 after the loop. Mechanism: the two arm
 *     loads are one expression, PRE merges them into one pseudo above the
 *     key test, the movable then has savings >= 2 and clears
 *     move_movables' threshold, and it is re-emitted from its REG_EQUAL
 *     constant. The ROM's loads stayed separate (savings 1 each, below the
 *     threshold for a loop with a call). Untested: which source keeps the
 *     two arms' loads distinct -- probes c1 (draft `?:`) and c2 (four
 *     stores) both hoist in a reduced function.
 */
/* Wave 77 (W77-C): configured baseline is 924/920 (+4), 58.8%, 379 differing
 * bytes -- a large improvement on the wave-66 +16, and the launch brief's "-2"
 * is wrong in sign as well as size.
 *
 * The two `R_ARM_ABS32 .rodata` relocations against the ROM's
 * gUnknown_0812A284 / gUnknown_0812A288 are CORRECT and are not part of the
 * residual: they are this unit's own -fforce-addr words, and the promotion
 * carries them (see the wave-55 note below).
 */
/* PARKED, wave 55 (W55-H).
 * WAVE 66 ADDENDUM: spelling the outer repetition as the ROM-shaped labeled
 * goto loop reduced the configured draft from 948/920 (+28) to 936/920 (+16).
 * The explicit nested palette-index if-chain now reproduces the ROM's 0,1,2,3
 * assignment order.
 *
 * THE VOCABULARY, all of it settled and already in the headers:
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
 *  - The outer loop-back `_0804AA70` is `if (unk5c != 0) goto after; if
 *    (sub_0804A18C(...) != 0x24) goto after; goto key_loop;` -- a goto-shaped
 *    loop that loop.c cannot see, spelled below exactly that way.
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
    struct Unk030044E0 **gp;

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
            c = gUnknown_030044E0->unk66 + 1;
            c &= 0xff;
            c |= 0x80;
            gUnknown_030044E0->unk66 = c;
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
                {
                    gp = &gUnknown_030044E0;
                    ((struct Unk030044E0View *)(*gp))->unk1e = (((struct Unk030044E0View *)(*gp))->unk1e > 0xd) ? 0 : ((struct Unk030044E0View *)(*gp))->unk1e + 1;
                }
                else if (gUnknown_030044E0->unk1e == 0)
                {
                    gp = &gUnknown_030044E0;
                    ((struct Unk030044E0View *)(*gp))->unk1e = 0xe;
                }
                else
                {
                    gp = &gUnknown_030044E0;
                    ((struct Unk030044E0View *)(*gp))->unk1e = ((struct Unk030044E0View *)(*gp))->unk1e - 1;
                }
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
            else if (gUnknown_030044E0->unk20 == 0)
                gUnknown_030044E0->unk20 = 5;
            else
                gUnknown_030044E0->unk20 = gUnknown_030044E0->unk20 - 1;
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
