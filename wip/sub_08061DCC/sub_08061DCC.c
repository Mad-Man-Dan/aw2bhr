/* WAVE 89 (W89-H). THE `c_local` AXIS IS RE-OPENED AND HALF THE REMAINDER IS
 * GONE. Draft moves from 2 instructions short of the ROM to 1. The wave-37
 * `c_local` negative was measured against an allocation that no longer exists,
 * exactly as W89-D predicted, and the corrected form of the lever is:
 *
 *   BIND THE SYMBOL'S ADDRESS TO A LOCAL **AND LEAVE THE FIRST REFERENCE
 *   BARE**. The bind is what creates the second pseudo; the bare reference is
 *   what keeps the pool word. Both halves are load-bearing.
 *
 *   pa = &gUnknown_0816DB08;                 <- creates the cross-block pseudo
 *   if (p->unk04_0 < (*(*(pa = &gUnknown_0816DB08)))[3])  <- BARE, block-local pseudo
 *   ...
 *   if (q >= (*(*pa))[2])                     <- the later use, via the local
 *
 * emits the ROM's `ldr r0,=X / add r6,r0,#0` -- the pool address materialised
 * into a LOW SCRATCH and copied into its callee-saved register. The entry block
 * goes from 9 instructions to the ROM's 10, with the ROM's instruction multiset.
 *
 * MEASURED (two compile_probe calls, seven spellings, one try_match):
 *   v1  pa AND pb bound, first reference of each left BARE ......... best
 *   v2  only pa bound ............................. same count as v1; the copy
 *       is produced by the pa bind ALONE (isolation), and binding pb is
 *       byte-neutral here -- but v1 orders block 2 as the ROM does
 *       (`ldr` pool word first, then `ldrb` index) and v2 does not.
 *   v3  only pb bound ............. NO copy anywhere; identical to the old draft
 *   v4  both bound and used for EVERY reference, no bare reference left ......
 *       push mask grows to {r4,r5,r6,r7,lr}. This IS the wave-37 negative, and
 *       its cause is now named: it is W89-C's "leave at least one bare
 *       reference" rule, not the bind.
 *   w4  the bare read forced AHEAD of the bind with a temp ......... REGRESSION,
 *       the copy DISAPPEARS (`ldr r6,=X` direct) and the entry block is back to
 *       9 instructions. The bind must come FIRST in the SOURCE even though the
 *       copy is EMITTED after the loads -- the copy's position is the
 *       compiler's, and trying to author it destroys it.
 *   w5  pb bound inside the if body .............. same count, block 2 ordered
 *       `ldrb` before `ldr`; further from the ROM than v1.
 *   w6  the fold-proof mask moved to the FIRST reference ........... same count.
 *       W89-D's "which side carries it does not matter" still holds after the
 *       bind is added.
 *
 * WHAT IS LEFT: exactly ONE instruction, the ROM's `adds r5,r3,#0` in block 2 --
 * the same copy pattern for `&gUnknown_0816DB0C` that the pa bind just bought
 * for `&gUnknown_0816DB08`. Binding pb does not buy it because &Y never has to
 * survive a call, so the allocator is content to leave it in a low register,
 * where the ROM spends a callee-saved one on it. The blocker is the one W89-D
 * named: this draft has FIVE cross-block values (index, &X, &Y, param, and the
 * shared `0x5c` constant) where the ROM has FOUR, because the ROM
 * rematerialises `movs #0x5c` at BOTH multiplies and this draft hoists one
 * `mov r5,#0x5c` and copies the index into the multiply's destination instead.
 * A constant pseudo that dies at the multiply is tied to the multiply's
 * destination; a live one cannot be.
 *
 * AND THE `0x5c` PSEUDO IS NOT REACHABLE BY THE FIFTH SPLITTER. gcse refuses to
 * unify a bare CONST_INT (`want_to_gcse_p` returns 0 for CONST_INT), and a mask
 * on a constant is folded before cse ever numbers it -- measured on
 * sub_08035170 this wave, where a shift-mask-shift over a `u8` local holding a
 * constant offset was byte-identical to the unmasked source. The fifth
 * splitter needs a NARROW VARIABLE operand whose value range lets combine
 * discharge the mask. `idx * 0x5c` has one (the index, and W89-D already spent
 * it); the constant does not.
 */

/* WAVE 89 (W89-D). THE RECOMPUTED MULTIPLY IS REPRODUCED. Draft moves from
 * 128/136 (-8) to SIZE-EXACT 136/136. Full record: work/sub_08061DCC/W89-notes.md.
 *
 * The prompt's premise for this park -- "the ROM re-reads a value FROM MEMORY,
 * so wave 88's `static inline` helper is in scope" -- IS WRONG HERE and I did
 * not spend a probe on the helper. Both the ROM and the old draft re-load the
 * pool word at the join; the value they DISAGREE about is `p->unk00 * 0x5c`,
 * which is an address-arithmetic pseudo. By W89-C's own bound the helper is
 * inert on exactly that. What DOES reach it is W87's redundant-mask lever,
 * which attacks cse's VALUE NUMBERING, and no wave before 87 could have tried
 * it. Wave 87's own note here ("the index-spelling axis is five deep and
 * exhausted") was true only of VALUE-equal respellings: every one of those five
 * spells the same value, and cse merges them. The lever that works does not
 * create a second value, it creates a second EXPRESSION for one value.
 *
 * MEASURED (three compile_probe calls, nine spellings, two try_match):
 *   - `[p->unk00 & 0xff]` at either reference: BYTE-IDENTICAL to the old draft.
 *     The AND is folded before cse ever numbers the MULT, so a bare mask is NOT
 *     a value-splitter for a value already known to fit. This BOUNDS W87's
 *     lever: it needs the SHIFT-MASK-SHIFT, not an AND that fold can discharge.
 *   - `[(((u32)p->unk00 << 24) & 0xff000000) >> 24]` on the SECOND reference:
 *     the win. cse sees a different expression, so the product is no longer
 *     available at the join; combine then proves the shift-mask-shift redundant
 *     and deletes it, costing no instruction. The join block now matches the
 *     ROM instruction for instruction --
 *       ldr r2,[r5] / movs r1,#0x5c / muls r1,r3,r1 / adds r1,r1,r2
 *     -- and the push mask becomes {r4,r5,r6,lr} as the ROM has it.
 *   - The same form on the FIRST reference instead, on BOTH references with
 *     different shift counts, and bound to a local declared inside the `if`
 *     body: all three BYTE-IDENTICAL to the second-reference form. Which SIDE
 *     carries it does not matter; that there are two expressions does.
 *   - Bound to a local BEFORE the `if` (`t = <masked>;`): +1 instruction (138
 *     bytes). It buys the ROM's `adds r3,r2,#0` index copy but pays for it by
 *     routing the FIRST multiply through the copy too, where the ROM keeps the
 *     raw load for the first and the copy for the second. Rejected on size.
 *   - Binding the quotient to a `u8` local (below) reorders the post-call block
 *     to the ROM's `lsls / lsrs / ldr r1,[r6] / ldr r1,[r1] / ldrb`, where the
 *     inline form hoisted both loads above the two shifts. Same size, one more
 *     block exact. Declared inside the `if` body or at function top: identical.
 *
 * WHAT IS LEFT, and it is now ONE mechanism rather than the old three:
 * the ROM materialises each pool ADDRESS into a low scratch and COPIES it to
 * its callee-saved register (`ldr r2,=A / ... / adds r6,r2,#0` and
 * `ldr r3,=B / ... / adds r5,r3,#0`), where this draft loads each address
 * straight into the callee-saved register. Copy-after-computation is gcse's
 * `pre_insert_copies` signature: in the ROM the later reference was left to
 * gcse PRE, in the draft cse had already merged it, so gcse had nothing to
 * insert a copy for. [W89-H: CONFIRMED, and the source lever is the bind above.
 * Half of it is now reproduced.]
 * Net: the draft is two instructions short of the ROM's code and gets exactly
 * four bytes back as literal-pool alignment padding the ROM does not need, so
 * `size: match` here is the wave-77 `sub_08045FC8` costume -- do not read it as
 * a two-byte residual. [W89-H: now ONE instruction short, two bytes of padding.]
 */
#include "global.h"

/* PARKED, wave 37 (W37-K3), re-measured wave 59 (W59-E), wave 87 (W87-D).
 * History before wave 89, kept because its ruled-out axes are still binding:
 *
 * What the ROM does that the pre-wave-89 draft did not:
 *
 *   at _08061E16   ldr r2,[r5] / movs r1,#0x5c / muls r1,r3,r1 / adds r1,r1,r2
 *   old draft      ldr r1,[r3] / adds r1,r2,r1
 *
 * i.e. the ROM RECOMPUTES `0x5c * p->unk00` in the second arm and keeps the
 * bare `p->unk00` alive instead (the `adds r3,r2,#0` copy), where the old draft
 * kept the PRODUCT alive across the merge. THIS HALF IS NOW FIXED (wave 89).
 *
 * Axes ruled out, one try_match each:
 *   - honest spelling (`gUnknown_03004784[3]`, `gUnknown_085D5ABC[..]`) -- loses
 *     the .rodata indirection ENTIRELY, 16 bytes short. Measured cause, and it
 *     is new: -fforce-addr's reroute is settled after CSE has merged the
 *     references, and a pointer global read twice around `bl __divsi3` collapses
 *     to ONE reference because a libcall does not clobber memory. Probes f1..f4
 *     in that wave confirm two references in two basic blocks are NOT enough on
 *     their own -- docs/agbcc-codegen.md's "referenced by name from more than
 *     one basic block" rule is necessary, not sufficient.
 *   - `c_local`: `pa = &gUnknown_0816DB08`, `pb = &gUnknown_0816DB0C` bound to
 *     locals. Hoists BOTH pool loads into the entry block and CSEs harder --
 *     120 bytes, strictly worse. **RE-OPENED AND PARTLY OVERTURNED IN WAVE 89
 *     (W89-H): that measurement used the locals for EVERY reference, which
 *     leaves no bare reference and costs a fifth callee-saved register. With
 *     the first reference left BARE the same bind is the lever, not a
 *     regression. See the W89-H block at the top.**
 *   - a local for the index, and the same local written as an EMBEDDED
 *     assignment inside the first subscript (`g[t = p->unk00]`). Byte-identical;
 *     so are `t = p->unk00;` before the `if` and `u = p->unk00;` inside the join
 *     block (wave 87). All five are VALUE-equal respellings -- see the wave-89
 *     note above for why that whole class was always going to fail.
 *   - `const` on the two pool declarations: byte-identical, i.e. no effect.
 *   - the W81-C / W86-G two-definition join re-derivation (wave 87): refuted,
 *     and correctly so -- W81-C needs definitions that DISAGREE BY VALUE across
 *     paths, and both arms of the `||` need the same index.
 *
 * WAVE 59 (W59-E): do not run `mcp permute` on this draft without a snapshot --
 * the wave-59 breakage stranded raw header-expanded output in the draft file.
 * (The wrapper was fixed after wave 61.)
 *
 * Everything else is settled and should not be re-derived:
 *   - the parameter is a unit record with struct Unk08499594's layout; the
 *     bitfields are what produce the ROM's SImode `movs #8; negs; ands` mask.
 *     A hand-written `(p->unk09 & ~7) | K` narrows to `movs #0xf8; ands` and is
 *     one instruction shorter -- measured, so the field really is a bitfield.
 *   - `[r4,#4]` is read BOTH as a byte with a 7-bit field and as a halfword
 *     masked 0xf0 << 3 = 0x780: one u16 bitfield accessed at two widths, not
 *     two separate members. `[r4,#9]` is a 3-bit field at bits 0-2.
 *   - `volatile` on the two pool words is what restores the ROM's
 *     re-dereference at every use (+8 bytes over the plain declaration). It is
 *     a reproduction device, not a claim about the object.
 *   - the `||` short-circuit, the `(u8)` cast on the quotient, `100 * x`, the
 *     SIGNED `__divsi3` (not `__udivsi3`), the real `muls` by 0x5c, and the
 *     shared `orrs; strb` tail (cross-jumping) are all exact.
 */

struct Unk8061DCC
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x03];
    /* 0x04 */ u16 unk04_0:7;
               u16 unk04_7:4;
               u16 unk05_3:5;
    /* 0x06 */ u8 unk06_0:7;
               u8 unk06_7:1;
    /* 0x07 */ u8 filler_07[0x02];
    /* 0x09 */ u8 unk09_0:3;
               u8 unk09_3:5;
};

void sub_08061DCC(struct Unk8061DCC *p)
{
    u8 **volatile *pa;
    struct UnitType *volatile *pb;

    if (p->unk04_0 < (*(*(pa = &gUnknown_0816DB08)))[3])
    {
        p->unk09_0 = 2;
        return;
    }

    pb = &gUnknown_0816DB0C;
    if ((*pb)[p->unk00].maxAmmo == 0 || p->unk04_7 != 0)
    {
        u8 q = 100 * p->unk06_0 / (*pb)[(((u32)p->unk00 << 24) & 0xff000000) >> 24].maxFuel;

        if (q >= (*(*pa))[2])
            return;
    }

    p->unk09_0 = 1;
}
