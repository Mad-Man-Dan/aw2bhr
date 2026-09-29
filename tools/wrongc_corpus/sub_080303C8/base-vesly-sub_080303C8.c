#include "global.h"
#include "hardware.h"

/* Wave 74 (W74-C): fresh configured baseline is 420/428 (-8), 343 differing
 * bytes (18.0%), first difference +0x0e.  The faithful pointer-rooted volatile
 * forms were tested separately at both RMW sites.  `volatile u16 *p; *p |=
 * mask` with an int asm barrier compiles to 416/428 at either site, and
 * loading the value through p, barriering it, then storing `value | 0xffff`
 * compiles to 412/428.  Both lose the wanted RMW instructions rather than
 * fixing the residual.  The nonvolatile COMPONENT_REF plus int-mask barrier
 * at only the first site reaches 424/428 but changes the allocation and scores
 * worse (15.2%); the same experiment at the second site is 416/428.  The two
 * contexts remain distinct, and the honest 420-byte draft below is retained.
 * Declaration-local `int hi = 0x8000` again kept 420/428 and improved only one
 * byte (342 differing), confirming that the OR grouping is independent.
 *
 * Wave 72 (W72-C): a nonvolatile overlay plus an inline-asm literal barrier
 * can reproduce the first site's single-read RMW instruction sequence, but
 * applying it at both sites materialises a second 0xffff instead of reusing
 * the ROM's long-lived value, spills acc, and regresses the whole unit.  The
 * two sites therefore require distinct allocation contexts; the 420/428
 * configured fixpoint below is restored.
 *
 * Wave 65 (W65-O): the credible `int hi = 0x8000` fold barrier was retested
 * in the complete function.  It reproduces the ROM's OR grouping exactly but
 * remains 420/428 overall; both volatile `unk210 |= 0xFFFF` sites still carry
 * one extra dead `ldrh`.  The two residual mechanisms are independent.
 *
 * Wave 62 (W62-A): current configured total is 420/428 (-8; code delta -10).
 * The local-hi fold barrier did not change total size, and nonvolatile overlay
 * spellings for the two RMW sites either stayed at 420 or fell to 412 by
 * folding away the wanted OR.  -O1 and no-force profiles diverge globally.
 *
 * PARKED (wave 49, W49-B) at +8 bytes on 428. NOT a "could not work out how"
 * park: the control flow, every type, every constant and both loops are
 * settled and byte-exact. What remains is TWO narrow codegen idioms, each
 * costing the same few bytes at two sites. Listed with what was ruled out so
 * the next agent does not re-run any of it.
 *
 * It also needs its .rodata pool words placed once it closes:
 *   "rodata": ["0x08090CBC", "0x08090CC0", "0x08090CC4"]
 * All three are agbcc -fforce-addr address constants, dereferenced in
 * baserom.gba: 0x08090CBC -> 0849B018, 0x08090CC0 -> 02023894,
 * 0x08090CC4 -> 0849B01C. None of them is a global.
 *
 * ---- REMAINING DIFF 1: the OR chain, worth -2 bytes (mine is SHORTER) ----
 *
 * ROM:  r1 = 0x3FF & ~KEYINPUT
 *       r0 = unk00 << 10
 *       r2 = 0x8000                  (movs #0x80; lsls #8; adds r2,r4,#0)
 *       r0 |= r2                     <- constant joins the SHIFT
 *       r1 |= r0
 *       r1 |= unk02 << 13
 * Mine: r1 = 0x3FF & ~KEYINPUT
 *       r1 |= 0x8000                 <- constant floats to the ACCUMULATOR
 *       r1 |= unk00 << 10
 *       r1 |= unk02 << 13
 *
 * agbcc's `fold` reassociates the constant out of the `(unk00 << 10) | 0x8000`
 * group no matter how it is parenthesised. Ruled out by compile_probe, all
 * measured, none of them worth retrying:
 *   - the plain single expression: floats it AND widens it to 0xffff8000,
 *     costing an extra pool word (the widening happens because gcc knows the
 *     result feeds a HImode store);
 *   - assigning the whole chain to an `int` temp first: stops the WIDENING
 *     (0x8000 is now built inline with movs/lsls, as the ROM does) but still
 *     floats it. This is the version below and is the closest;
 *   - `v |= ...` in three statements: floats it, and earlier;
 *   - `(unk00 | 0x20) << 10`: ORs before the shift;
 *   - `+ 0x8000` instead of `| 0x8000`: bits 10..25 and bit 15 overlap, so gcc
 *     cannot prove disjointness and emits `add`, where the ROM has `orrs`.
 * The ONE spelling that reproduced the ROM's grouping exactly was binding the
 * constant to a local (`int hi = 0x8000;`) -- a fold barrier. It is left out
 * because it is not credible original source; if nothing better is found it is
 * a legitimate fallback and it is known to work.
 *
 * ---- REMAINING DIFF 2: `unk210 |= 0xFFFF`, +2 bytes at each of 2 sites ----
 *
 * ROM:  ldrh; ldr =0xFFFF; orrs; strh          (ONE read)
 * Mine: ldrh; ldrh; ldr =0xFFFF; orrs; strh    (TWO reads)
 *
 * The extra read is the volatile-store dead load. The puzzle is that unk210 is
 * definitely volatile -- the `unk210 = acc` store 20 instructions later carries
 * its own dead `ldrh`, and the `return unk210` after it re-reads rather than
 * reusing `acc` -- yet the ROM's two read-modify-writes do NOT carry it. So
 * agbcc appears to suppress the dead load when the stored value already
 * contains a load of the same MEM, and something about this draft's spelling
 * does not trigger that. Ruled out by probe: `x |= c`, `x = x | c`, and going
 * through a bound `struct Unk0849B01C *` all give two reads. Note that unk210
 * cannot simply be non-volatile: `(u16)x | 0xFFFF` would then fold to a
 * constant store and the ROM's `orrs` would disappear entirely.
 *
 * ---- Everything else is settled ----
 *
 * gUnknown_02023894 needed `volatile` (see its note in unknown-globals.h) --
 * a CORRECTNESS fix, not just codegen: without it the handshake spin loop's
 * load is hoisted and the loop can never exit. The tempting hypothesis that
 * the loop's other, already-volatile read (unk04) would block the hoist was
 * tested and is FALSE.
 * `acc` must be initialised at its declaration, not before the loop: the ROM
 * zeroes r8 in the prologue, ahead of both early-outs, and that placement is
 * also what puts it in a callee-saved register.
 * The two `goto`s are the ROM's shared tail -- the early-out path falls into
 * the gpKeySt return that the bad-packet path branches to. */

u16 sub_080303C8(void)
{
    int acc = 0;
    int v;
    int i;

    if (gPlaySt.savingEnabled == 0)
        goto reset;
    if (gUnknown_0849B018->unk01 != 2)
        goto reset;

    if (gUnknown_0849B018->unk06 != 0)
        while (gUnknown_02023894 == 0 && gUnknown_0849B018->unk04 != 2)
            ;

    REG_IME = 0;
    gUnknown_02023894 = 0;
    REG_IME = 1;
    sub_080301E8();

    v = (~REG_KEYINPUT & 0x3FF)
      | ((gUnknown_0849B01C->unk00 << 10) | 0x8000)
      | (gUnknown_0849B01C->unk02 << 13);
    gUnknown_0849B01C->unk06 = v;

    for (i = 0; i < 4; i++)
    {
        if (sub_0802F460(i))
        {
            if (gUnknown_0849B01C->unk208[i] == 0xFFFF
             || gUnknown_0849B01C->unk208[i] == 0x5FFF
             || gUnknown_0849B01C->unk208[i] == 0x7FFF
             || !(gUnknown_0849B01C->unk208[i] & 0x8000))
            {
                gUnknown_0849B01C->unk210 |= 0xFFFF;
                goto keys;
            }
            if (((gUnknown_0849B01C->unk208[i] & 0x1C00) >> 10) != gUnknown_0849B01C->unk00)
            {
                gUnknown_0849B018->unk1b++;
                gUnknown_0849B01C->unk210 |= 0xFFFF;
                return 0;
            }
            acc |= gUnknown_0849B01C->unk208[i] & 0x3FF;
        }
    }

    gUnknown_0849B01C->unk210 = acc;
    gUnknown_0849B018->unk1b = 0;
    return gUnknown_0849B01C->unk210;

reset:
    gUnknown_0849B018->unk1b = 0;
keys:
    return gpKeySt->previous;
}






















