#include "global.h"

/* WAVE 88 (W88-B). Status `asm`, never parked; no prior notes, no header
 * comment -- the draft was the only record.
 *
 * BASELINE (w88-start.c): 54.5%, size -4, pool 33/34 under `configured`.
 *
 * THE MISSING POOL WORD IS FIXED, and the mechanism was NOT an allocation tie.
 * The ROM emits the constant 0x4C3 TWICE (one pool word for the `a3 == 1` arm,
 * one for the else arm). The candidate emitted it once and derived the first
 * with a runtime `subs r2, #6` off a still-live 0x4C9. The cause was
 * EVALUATION ORDER at the two stores above it: the ROM loads the stored VALUE
 * first and the address constant second, so the 0x4C9 pseudo dies at its store
 * and cse has nothing to reuse; the candidate computed the address first,
 * keeping 0x4C9 live across the store and into the `if`. Binding the stored
 * value to a local --
 *     w = buf[5];      a2[0x4C9] = w;
 *     w = buf[0] + 1;  a2[0x4CA] = w;
 * -- restores the ROM's order, kills the reuse and emits the second pool word.
 *
 * Also fixed: the LAST copy loop's base. The ROM hoists `a2 + 0x4C4` as a
 * preheader bind and indexes `r2 + i`; the candidate built `(i + a2) + 0x4C4`
 * inside the loop. Binding it AFTER `i = 0` (not before) puts it in the ROM's
 * preheader position -- [i=0][a2+0x4C4][&gUnknown_03003FF3].
 *
 * WAVE 88 (W88-C) claimed two further fixes. BOTH ARE REGRESSIONS -- see the
 * WAVE 89 (W89-A) block at the end of this file. Its notes are kept below for
 * the record because its REFUTATION of the COMPONENT_REF spelling is sound and
 * independently useful; only the two edits it banked are wrong.
 *
 * A. (W88-C) "The same `a2 + 0x4C4` bind was missing on the *FIRST* zero loop,
 *    and writing it exactly like the last loop reproduces the ROM."
 *    -- WRONG. See W89-A: the two loops are NOT symmetric in the ROM.
 *
 * B. (W88-C) THE INNER STORE'S `+2`. The ROM builds the destination as its own
 *    address with the CONSTANT FIRST (`lsls r1,r1,#1 / adds r4,r7,#2 /
 *    adds r4,r4,r1`) and does NOT use a store displacement; the candidate folds
 *    the +2 into `strh r0,[r1,#2]`.
 *    MEASURED AND REFUTED (this part still stands, do not re-run): spelling the
 *    destination as a COMPONENT_REF on a cast struct whose u16 array member
 *    sits at offset 2 -- `((struct { u8 filler_00[2]; u16 cells[1]; } *)a2)
 *    ->cells[k++]` -- STILL folds the +2 into the displacement. W80-D's "only a
 *    COMPONENT_REF forms (ptr + c) first" does NOT reach a member offset the
 *    store insn can absorb: that fold happens in addressing-mode selection,
 *    below the tree form, so no tree spelling of a SINGLE EXPRESSION reaches
 *    it. That bound on W80-D is real.
 *    W88-C then banked the TWO-STATEMENT SPLIT (`n = k; k++; d = a2 + 2;
 *    d = d + n * 2; *(u16 *)d = ...;`) as the fix. See W89-A: it does emit the
 *    ROM's three-insn address form, but it COSTS 8 BYTES NET elsewhere.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

/* WAVE 89 (W89-A). ALL SCORES UNDER THE `configured` PROFILE.
 *
 * THE HEADLINE: W88-C'S RECORDED RESULT IS A `best.json` MISREAD, AND ITS TWO
 * BANKED EDITS ARE AN 8-BYTE REGRESSION. This is the exact trap the brief
 * warns about ("`best so far: X%` is best.c's score, not the draft's").
 *
 *   - `work/sub_0803CFA4/best.c` is BYTE-IDENTICAL to `w88c-start.c`, the
 *     snapshot W88-C took of W88-B's fixpoint BEFORE its first probe, and
 *     `best.json` records that source at 660 bytes, `exact_size: true`,
 *     65.15%. So W88-B's fixpoint was ALREADY size-exact at 65.15%; W88-B's
 *     "size -4" describes its own BASELINE, not its result, and W88-C read
 *     that -4 as its own starting point.
 *   - W88-C's two edits (the first-zero-loop bind and the two-statement
 *     address split) never beat 65.15%, so `best.c` was never updated -- and
 *     W88-C wrote `best.json`'s untouched numbers into its notes as the final
 *     draft's. The draft W88-C left on disk compiles to 652 bytes: MINUS 8,
 *     not size-exact. Measured directly from `_cand.bin` after re-running
 *     `try_match` on the on-disk draft, and visible in the diff as the switch
 *     jump table starting at 0x174 where the ROM's starts at 0x17c, with the
 *     whole tail carrying that same 8-byte shift through to the final
 *     `R_ARM_ABS32 gUnknown_03003FF3` (candidate 0x288, ROM 0x290).
 *   - Both edits are reverted here. THE FRAME IS NOT EVIDENCE FOR THE SPLIT
 *     EITHER: `add sp,#36` is present in this reverted body too, so W88-C's
 *     "the frame grows to 0x24 and size-exactness falls out of the same edit"
 *     is a coincidence of two things that were already true.
 *
 * WHY W88-C'S "PROCESS POINT" INVERTED. W88-C's stated lesson was: when a
 * lever fixes one instance of a repeated shape, apply it to every other
 * instance in the same function. Applied to the two `a2 + 0x4C4` zero loops
 * that is a REGRESSION -- the ROM binds the base in the LAST loop and does NOT
 * bind it in the FIRST. The two loops differ in what is live across them: the
 * last loop also carries `&gUnknown_03003FF3` in its preheader, so the base
 * bind has a preheader to sit in; the first loop's preheader is empty and the
 * bind has nothing to hold its position, so it is folded back. A repeated
 * SHAPE is not a repeated ALLOCATION CONTEXT.
 *
 * REMAINING RESIDUAL from here (the size-exact 65.15% body):
 *   - Loop-counter allocation. The ROM keeps k in r5 and y in sb and spills
 *     the destination address to [sp,#0x20]; the candidate keeps the address
 *     in a register and spills y instead, reloading it inside the body. Same
 *     number of spill slots, different choice of what lives in one.
 *   - The mid-body re-chase of the pointer global IS reproduced
 *     (`mov r3,sl / ldr r0,[r3] / ldr r5,[r0]`, the W86-F force-addr reload).
 *     The `*((u8 *)gPlayers) += 0;` line is what defeats cse's merge
 *     of the two chases and costs zero bytes -- do NOT delete it as dead code.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

/* WAVE 89 (W89-E). ALL SCORES UNDER THE `configured` PROFILE. Re-measured, not
 * inherited: 660 bytes, size-exact, 65.2%, 230 of 660 differ, first difference
 * at +0xa. `best.c` is byte-identical to this draft.
 *
 * CORRECTION TO THE W89-A BLOCK ABOVE. It says "`add sp,#36` is present in this
 * reverted body too". IT IS NOT. This body emits `sub sp,#32`; THE ROM EMITS
 * `sub sp,#36`. W88-C's conclusion was still wrong and W89-A's revert was still
 * right, but the frame is an OPEN 4-byte difference, not something already true.
 *
 * THE RESIDUAL AT +0xa IS THE FRAME SIZE ITSELF -- classification, which is what
 * this batch asked for. The first two hunks are `sub sp,#36` vs `sub sp,#32`,
 * and then a transposition of the `a2` parameter copy against the `a3` sub-word
 * prologue conversion (ROM: copy a1 / convert+spill a3 / copy a2; candidate:
 * copy a1 / copy a2 / convert+spill a3). The transposition is the SECOND-ORDER
 * consequence of the slot count, not an independent fact.
 *   KIND: allocation -- THE ROM CARRIES ONE MORE 4-BYTE STACK SLOT, i.e. one
 *   more value live across the region that owns [sp,#0x20]. Not a bare allocno
 *   tie (a slot COUNT is source-changeable), but see the refutation below.
 *   This is the wave-57 sub_08073A00 pattern: A PROLOGUE DIFFERENCE IS THE LAST
 *   SYMPTOM TO APPEAR AND THE FIRST ONE YOU SEE. Nothing at the top of this
 *   function is wrong; do not spend a wave on the parameter copies.
 *
 * REFUTED, one try_match: THE TYPED DESTINATION BIND. Pre-registered mechanism
 * was that binding the inner store's destination would both emit the ROM's
 * three-insn address form AND supply the live value that buys the extra slot.
 * By probe `d = (u16 *)(a2 + 2); d[k++] = v;` DOES emit the ROM's form
 * (`lsl / add r0,#0x2 / lsr / add / strh [r1]`) and is cheaper than W88-C's
 * three-statement u8* split; `((u16 *)a2)[k + 1]` and the present spelling both
 * fold to `strh r2,[r1,#0x2]`. IN SITU IT IS A 20-BYTE REGRESSION: 680 bytes
 * (+20), 19.4%, 532 of 660 differing. It does not buy the slot -- the frame
 * stays 32 -- it pushes the a2 PARAMETER into hi r8 (`mov r8,r1`) and every
 * later reference pays `add rN,r8`. Reverted; this body is the 65.2% fixpoint.
 *
 * THIRD MEASURED INSTANCE OF ONE TRAP ON THIS FUNCTION (W88-C's split, W88-C's
 * first-zero-loop bind, this): reproducing the ROM's instruction shape at one
 * site is NOT evidence the source construct is right, because the construct's
 * cost is paid in the allocator somewhere else. Measure the whole function.
 *
 * FOR WAVE 90: find the value the ROM keeps live across the inner loop that
 * this candidate does not, and use THE FRAME (36 vs 32) as the meter -- the
 * score is frozen by the +0xa difference and will not move until the slot count
 * matches. Do NOT manufacture an address local to create the pressure; measured
 * above, the allocator spends it on a hi-register parameter instead.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

void sub_0803CFA4(const void *a1, u8 *a2, u8 a3)
{
    u8 buf[8];
    u8 tbl[8];
    u16 i;
    u16 k;
    u16 ctr;
    int x;
    int y;
    int idx;
    int v;
    u8 t;
    u8 w;
    u8 *p;
    u8 *q;
    u8 *rows;
    u8 *cells;
    struct Unit *e;
    sub_0803CC84(a2 + 0x4B2, a1);

    a2[0x4C2] = 0;
    a2[0] = *(u16 *)gUnknown_08499590;
    a2[1] = *(u16 *)(gUnknown_08499590 + 2);

    for (i = 0; i <= 4; i++)
        a2[0x4C4 + i] = 0;

    for (i = 0; i <= 5; i++)
        buf[i] = 0;

    for (i = 0; i <= 4; i++)
        tbl[gUnknown_03003FF3[i]] = (i - 1) << 6;

    ctr = 0;
    k = 0;

    for (y = 0; y < *(u16 *)(gUnknown_08499590 + 2); y++)
    {
        for (x = 0; x < *(u16 *)gUnknown_08499590; x++)
        {
            p = gUnknown_08499590;
            rows = p + 0x417A;
            idx = *(u16 *)(rows + y * 2) + x;
            cells = p + 0x12;
            v = cells[idx];

            if (v != 0)
            {
                e = &gUnknown_08499594[v];
                a2[0x4CB + k] = tbl[(v >> 6) + 1] | e->type;
            }
            else
            {
                a2[0x4CB + k] = 0;
            }

            p = gUnknown_08499590;
            rows = p + 0x417A;
            idx = *(u16 *)(rows + y * 2) + x;
            cells = p + 0xA22;
            *(u16 *)(a2 + 2 + k++ * 2) = ((u16 *)cells)[idx];
            *((u8 *)gPlayers) += 0;

            idx = *(u16 *)(rows + y * 2) + x;
            cells = p + 0x1432;
            t = cells[idx];

            switch (t & 0x1f)
            {
            case 6:
            case 8:
            case 10:
            case 11:
            case 14:
            case 20:
                buf[5]++;
                buf[t >> 5]++;
                break;
            }

            if ((t & 0x1f) == 8)
                ctr++;
        }
    }

    for (i = 0; i <= 4; i++)
    {
        if (buf[0] < buf[i])
            buf[0] = buf[i];
    }

    w = buf[5];
    a2[0x4C9] = w;
    w = buf[0] + 1;
    a2[0x4CA] = w;

    if (a3 == 1)
        a2[0x4C3] = ctr;
    else
        a2[0x4C3] = 0;

    i = 0;
    q = a2 + 0x4C4;

    for (; i <= 4; i++)
        q[i] = gUnknown_03003FF3[i];
}

/* WAVE 89 (W89-G). ALL SCORES UNDER THE `configured` PROFILE, re-measured.
 * Snapshot in work/sub_0803CFA4/w89g-start.c; this body restored to it and
 * re-verified: size match (660 bytes), 65.2%, 230 of 660 differ, +0xa, frame
 * `sub sp,#32` against the ROM's `sub sp,#36`. Every W89-E figure reproduces.
 *
 * THE SLOT'S OCCUPANT IS NAMED, read off the ROM's stack traffic. The ROM
 * touches FIVE slots (0x10, 0x14, 0x18, 0x1c, 0x20); this candidate touches
 * FOUR (0x10, 0x14, 0x18, 0x1c) AT THE SAME OFFSETS IN THE SAME ORDER. The
 * extra slot is [sp,#0x20], the highest, i.e. the last reload allocated. Its
 * entire traffic is ONE store and ONE load with NO bl between them:
 *
 *     adds r1, r5, #0                       @ old k copied out
 *     adds r0,r1,#1 / lsls #0x10 / lsrs r5  @ k = (u16)(k+1)
 *     lsls r1,r1,#1 / adds r4,r7,#2 / adds r4,r4,r1   @ dest = a2 + 2 + k*2
 *     str  r4, [sp, #0x20]                  @ SPILL
 *     ... twelve instructions of RHS, inline ...
 *     ldr  r1, [sp, #0x20] / strh r0, [r1]  @ RELOAD, store
 *
 * SO IT IS NEITHER A SPILLED PARAMETER NOR A CALL RESULT. It is the inner
 * store's DESTINATION ADDRESS, spilled by reload because the ROM computes the
 * address first and then evaluates a twelve-instruction RHS INLINE, exhausting
 * the low registers before the store. This candidate never spills it because
 * `idx` and `cells` are bound by their own statements BEFORE the store, so when
 * the store expands, the RHS is two instructions and the destination survives.
 *
 * MEASURED AND REFUTED (1/2) -- INLINE THE RHS, AND THE DIRECTION IS INVERTED.
 * Pre-registered from the reading above: drop the idx/cells binds so the
 * destination must stay live across the full RHS and force the spill. Result:
 * 656 bytes (-4), 28.9%, AND THE FRAME WENT DOWN TO `sub sp,#28` -- one slot
 * FEWER, not one more.
 *   THE FRAME TRACKS THE NUMBER OF SIMULTANEOUSLY LIVE SOURCE BINDS, AND
 *   INLINING MOVES IT DOWN. A ROM frame LARGER than the candidate's can never
 *   be bought by removing binds, and this candidate sits BELOW the ROM on that
 *   axis: the original source holds one MORE value bound here, not fewer.
 *
 * MEASURED AND REFUTED (2/2) -- THE SHARED ROW-ADDRESS BIND. The ROM forms
 * r3 = p_value + 0x417A + y*2 inside the store statement and REUSES r3 in the
 * next statement (ldrh r0,[r3] again for the 0x1432 read) where this draft
 * recomputes it, so the ROM visibly carries one more live address. Binding it
 * (rowp = rows + y * 2, then *(u16 *)rowp + x at both sites) is BYTE-NEUTRAL:
 * 660 size-exact, 65.2%, 230 of 660 differ, frame still 32. cse had already
 * shared the address, so the bind adds no live value -- W87's "binding a
 * symbol's ADDRESS is byte-neutral when redundant". Reverted.
 *
 * WHAT THE ALLOCATION DIFFERENCE ACTUALLY IS, FOR WAVE 90. The hi registers
 * name it:
 *   ROM        k lives in r5 (low, callee-saved); ip HOLDS y * 2, and
 *              rows + y*2 is re-formed inside the loop off a re-chased base
 *              (mov r1,sl / ldr r0,[r1] / ldr r2,[r0]) -- TWO values live
 *              across the body, the chased base in r2 and y*2 in ip.
 *   candidate  ip HOLDS k (a hi scratch); LICM has hoisted the WHOLE address
 *              rows + y*2 into ONE register -- only ONE value live.
 * So the ROM's extra live value across the inner loop is y * 2 HELD SEPARATELY
 * FROM THE ROW BASE, and the extra stack slot is the downstream consequence of
 * that pressure. That is why W89-E's destination bind (adding an address)
 * regressed: the ROM does not have an extra address, it has the composite row
 * address NOT hoisted.
 *
 * PRE-REGISTERED FOR WAVE 90: defeat LICM's hoist of the composite rows + y*2
 * so the loop re-forms it from a base plus a separately-held y*2, as the ROM
 * does. MEASURE THE FRAME (32 -> 36), not the score, which stays frozen at
 * 65.2% until the slot count matches. Do NOT add an address local (W89-E,
 * -20 bytes) and do NOT remove binds (W89-G, the frame drops to 28).
 *
 * Still true: *((u8 *)gPlayers) += 0; defeats cse's merge of the two
 * pointer-global chases and costs zero bytes -- do NOT delete it as dead code.
 * Toolchain axis not re-swept (W81-E: zero exit-0 flips across 155 drafts). */

/* WAVE 89 (W89-I). ALL SCORES UNDER THE `configured` PROFILE, re-measured:
 * size match (660 bytes), 230 of 660 differ (65.2%), first difference at +0xa,
 * frame `sub sp,#32` against the ROM's `sub sp,#36`. Snapshot in
 * work/sub_0803CFA4/w89i-start.c. THIS BODY IS UNCHANGED BY ME -- no try_match
 * spent; both probes below left the meter where it was.
 *
 * REFUTED WITHOUT A PROBE: W89-G's LICM FRAMING. It reads "LICM hoists the whole
 * composite address rows + y*2 into ONE register for the candidate while the ROM
 * keeps y*2 in ip and re-forms the address inside the loop". THERE IS NO HOIST
 * TO DEFEAT. In `trymatch.py --diff` both bodies re-chase the pointer object,
 * both recompute y*2 INSIDE the body, both re-form p + 0x417a + y*2 there, and
 * both park y*2 for the second group (ROM `mov ip,r2`, candidate
 * `adds r4,r1,#0`). gUnknown_08499590 is `extern u8 *` -- a pointer OBJECT --
 * so the load is a MEM the loop's own stores kill and loop.c cannot hoist it in
 * either build. The ROM's `ip` park is a within-body cse, not a preheader hoist.
 * So the goto-loop mechanism has no target and the fifth splitter has nothing to
 * split; neither was run and neither should be.
 *
 * MEASURED AND REFUTED (1/2): DESTINATION-BEFORE-RHS, i.e. W88's two-statement
 * split applied ALONE (W89-A reverted it together with the first-zero-loop bind
 * and never isolated the two). `n = k; k++; d = a2 + 2; d = d + n * 2;` ahead of
 * the p/rows/idx/cells group DOES emit the ROM's order exactly --
 * `add r2,r7,#0x2 / lsl r1,r1,#1 / add r2,r2,r1` produced before the chase, with
 * `n = k` as its own copy -- AND THE FRAME STAYS 32. No spill; r2 survives in a
 * register. The split is not what buys the fifth slot, and W89-A's revert of it
 * costs nothing on this axis. Not banked (it also moves `sl` off the force-addr
 * .LC0 word onto the outer counter, which the ROM does not do).
 *
 * MEASURED AND REFUTED (2/2): AN EXPLICIT `yo = y * 2;` OUTER-LOOP LOCAL, from
 * the reading that the ROM's ip holds y*2 and its k holds a LOW register. Half
 * works: the bind lands in the ROM's position (`lsl r0,r1,#1 / mov ip,r0` in the
 * outer body, `add r0,r0,ip` at all three uses). The half that matters does not:
 * k goes to hi r8 not r5, the outer counter y is evicted to [sp,#0x1c] and
 * reloaded, the store folds back to `strh r0,[r1,#0x2]`, AND THE FRAME IS STILL
 * 32. The registers TRADE, they do not accumulate.
 *
 * THE FRAME HAS NOW RESISTED FIVE SOURCE EDITS ACROSS THREE AGENTS, and only
 * REMOVING binds ever moved it (W89-G's inline: 28). NO SOURCE BIND ADDED TO
 * THIS FUNCTION HAS EVER MOVED THE FRAME UP. Chapter appended to
 * docs/agbcc-codegen.md. What is left is the one fact the rest follows from:
 * THE ROM'S k LIVES IN LOW CALLEE-SAVED r5 AND THIS BODY'S LIVES IN ip.
 * With r5 spent on k the ROM has one fewer low scratch in the body, reload
 * spills the store destination, the fifth slot appears and the frame grows 4.
 * That is an allocation RANK question about one global.c allocno spanning both
 * loops -- the wave-80 bare-tie class. Give it ONE pre-registered mechanism, and
 * MEASURE k's REGISTER in a free compile_probe: the frame and the score are
 * both frozen behind it.
 *
 * Still true: `*((u8 *)gPlayers) += 0;` compiles to NOTHING but defeats
 * cse's merge of the two pointer-global chases -- confirmed again in both probes
 * -- do NOT delete it as dead code. The pool word at b8 relocating against
 * `.rodata` where the ROM names gUnknown_08091144 is agbcc's own force-addr word
 * for gUnknown_08499590 (.LC0), the expected honest spelling, not a residual.
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */
