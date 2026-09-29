
# Wave 92 (W92-A)

Verified inheritance: 796 / 804 (-8), 15.42%, first difference +0xc. Every
W89-I figure reproduces.

## REFUTED: naming the four target globals directly (the wave-92 brief's plan)

The brief told this batch to replace `(*pE4)`, `**pDC`, `(*pD8)` and
`gUnknown_081360E0` with `gUnknown_03004580`, `gUnknown_0300453C`,
`gUnknown_020298E0` and `gUnknown_085D6A48`, on the grounds that the 0x08136
cells are agbcc's own -fforce-addr address words. Measured, one try_match:

    W89-I fixpoint                      796   -8   15.42%
    all four named directly             756  -48   11.82%

A 40-byte regression. The honest spelling lets agbcc fold the base address into
every use, which deletes the per-use re-chase that W88-C installed on purpose.
W88-C's `c_local` bind of the pointer OBJECT remains the closest spelling, and
W85-C's reading of these four cells stands. This axis is now closed from both
ends: the bare pointer-object reference (W88-C, adds a .rodata word and a third
load) and the target global (here, deletes the second load).

This is the opposite result from sub_080359A4, sub_0802F588 and sub_0802F6A0 in
the same wave, where naming the global directly was worth 4 to 18 points. The
discriminator is what the loaded value is USED AS: in those three the chased
word holds the address of a scalar or an array that is then indexed, and agbcc
rebuilds the same chain; here it holds a base that every use would fold into a
displacement.

## MOVED: the side variable is read as if it were volatile

`-fno-force-mem` was the one flag the wave-91 sweep found for this function
(size-exact at 26.62%), which says the missing 8 bytes are memory operands our
build holds in registers and the original re-reads. W89-A's volatile probe was
aimed at gUnknown_03001FBC, where it loses the `ldrsh` because combine will not
fold a volatile MEM into a sign_extend. gUnknown_0300453C is a `u16` read with
`ldrh`, so that objection does not apply to it, and nobody had tried it.

    W89-I fixpoint                      796   -8   15.42%
    volatile read of gUnknown_0300453C  800   -4   17.79%   <- kept

Half the size gap in one edit, and the first movement on this function since
wave 89. The draft carries it as a file-local
`#define SIDE (*(volatile u16 *)&gUnknown_0300453C)` because the shared
declaration in include/unknown-globals.h must not be retyped unilaterally --
42 promoted files read that symbol. IF THE ORCHESTRATOR IS WILLING TO RE-VERIFY
THOSE 42 FILES, the honest fix is to declare gUnknown_0300453C `volatile u16`
in the header and drop the macro; the evidence here says the original treated
it as volatile.

REFUTED in the same wave: extending the volatile to the OTHER route to the same
variable (`pDC`, i.e. `**pDC` after the `03004580[side^1][2] == 1` test) by
retyping the local to `volatile u16 *const *`. 868 bytes, +68, 14.22%. Only the
direct reads want it.

## Still open

-4 bytes. Re-test `-fno-force-mem` from the new fixpoint before assuming the
remaining four bytes are the same mechanism.

## -fno-force-mem, re-measured from the new fixpoint (the pending question)

    before the volatile read   796  -8   15.42%    with -fno-force-mem: 804  +0  26.62%
    after  the volatile read   800  -4   17.79%    with -fno-force-mem: 808  +8  21.06%

The flag used to close the gap exactly and now OVERSHOOTS it by the same four
bytes it used to supply. So the volatile read is the source-level equivalent of
what -fforce-mem was doing to gUnknown_0300453C, and that half of the residual
is accounted for. The remaining -4 bytes are a different mechanism; do not
reach for this flag again.

# History moved out of the draft (waves 37-89)

These notes were comment blocks in sub_08050FF8.c. They are kept verbatim; the
standing rule is that wave numbers, percentages and lists of failed spellings
live here and not in source.

/* WAVE 85 (W85-C).  CORRECTION to the wave-37 note, found by reading the full
 * target listing: the four 0x08136 words are NOT -fforce-addr constants.  The
 * ROM reaches each of them with a DOUBLE load -- `ldr r4,=gUnknown_081360E0 ;
 * ldr r2,[r4] ; ... adds r0,r0,r2 ; ldrh r0,[r0,#4]` -- and uses the loaded
 * value as a BASE, which a force-addr word never is.  They are four const
 * pointer objects (declared in include/unknown-globals.h next to the
 * gUnknown_081360A0/A4/A8 cells), and the code chases them:
 *   - gUnknown_081360E4 (holds 0x03004580) is the row-pointer spelling of
 *     gUnknown_03004580: EVERY 03004580 access in the ROM goes through it
 *     (there is no `=gUnknown_03004580` pool word anywhere in the function),
 *     with the column offset folded onto the chased value (+2 for [1], +4 for
 *     [2]).
 *   - gUnknown_081360D8 (holds 0x020298E0) serves ONLY the three .unk8c
 *     accesses (0xD arm, else arm, increment); the .unk16/.unk00/.unk18
 *     accesses keep the direct `=gUnknown_020298E0` spelling.
 *   - gUnknown_081360E0 (holds 0x085D6A48) serves ONLY the `unk04 == 0` test;
 *     the x-sum's .unk08 read keeps the direct `=gUnknown_085D6A48` word.
 *   - gUnknown_081360DC (holds 0x0300453C) takes over from the direct spelling
 *     at the `03004580[side^1][2] == 1` test: every side read from there to
 *     the end of the function is `*gUnknown_081360DC` (the ROM reassigns r8
 *     from &0300453C to &081360DC at exactly that point), while the six reads
 *     before it stay direct.
 */

/* WAVE 88 (W88-B).  THIS DRAFT HAD NOT COMPILED SINCE WAVE 86 AND NOBODY HAD
 * NOTICED -- three waves of "parked at 21.5%" was a score for source that no
 * oracle could build.  Wave 86 (W86-C) retyped gUnknown_085643A8 from
 * `struct Unk085643A8 { const s16 unk00[2]; const s16 unk04[2]; }` to
 * `extern const s16 gUnknown_085643A8[][2]` in include/unknown-globals.h,
 * which turned this file's `gUnknown_085643A8.unk04[m]` into
 * "request for member `unk04' in something not a structure or union".
 *
 * Fixed here to the array spelling W86-C proved is the ROM's form:
 * `.unk04[m]` -> `[1][m]` (member offset 4 == row 1 of s16[2]), and the
 * redundant `(const s16 (*)[2])&gUnknown_085643A8` cast in the else arm
 * dropped now that the declared type is already `const s16 [][2]`.
 *
 * LESSON FOR THE ORCHESTRATOR, worth more than the score: a header retype in
 * one agent's batch can silently break an unrelated open draft, and NOTHING in
 * the pipeline catches it -- per-function `trymatch` is only run on the
 * functions a wave batches, and a park entry records the last score, not the
 * last build. A cheap sweep that just COMPILES every open draft after each
 * wave's header edits would have caught this in wave 86. */

/* WAVE 88 (W88-C).  W88-B's residual 1 ("the candidate chases one pointer level
 * too many") and residual 2 ("two .rodata force-addr words the ROM lacks") ARE
 * ONE FACT, and it is fixed. They were never two bugs: the extra `ldr` at each
 * site IS the load through the force-addr word.
 *
 * MECHANISM (measured, and it generalises -- see the doc chapter). A BARE
 * reference to a `T *const` pointer OBJECT read from more than one basic block
 * makes agbcc park the OBJECT'S OWN ADDRESS in a `.rodata` address constant and
 * reach it with an extra indirection:
 *     candidate   .LC8: .word gUnknown_081360E4      (in .rodata)
 *                 .text pool holds `.word .LC8`, every use is
 *                 `mov r1,r9 / ldr r0,[r1] / ldr r1,[r0]`     -- THREE insns
 *     ROM         .text pool holds `.word gUnknown_081360E4`, every use is
 *                 `mov r1,sl / ldr r2,[r1]`                   -- TWO insns
 * The candidate emitted FOUR such words (gUnknown_0300451C, _081360E4,
 * _081360DC, _081360D8), not the two W88-B counted.
 *
 * FIX: the old `c_local` workaround applied to the POINTER OBJECT ITSELF --
 * bind the object's ADDRESS to a local and read through it:
 *     u16 (*const *pE4)[8] = &gUnknown_081360E4;   ... (*pE4)[i][j]
 * The address then reaches the allocator as an ordinary source pseudo, agbcc
 * materialises it once with a plain `ldr rN,=sym` pool word and re-loads the
 * VALUE at each use -- exactly the ROM. Applied to all four, ALL FOUR .rodata
 * words disappear and the .text pool becomes the ROM's. gUnknown_081360E0 is
 * referenced ONCE and never needed it: the trigger is multi-block reference
 * count, not the type.
 *
 * SECOND FIX, same wave: THE ROM BINDS E4's COLUMN BASES. Where the candidate
 * folded the column offset into the load displacement (`ldrh r1,[r0,#0x2]`),
 * the ROM forms it as its own runtime add off the chased value -- `adds r0,#2`
 * for column 1 and, decisively, `adds r7,r2,#4` for column 2 KEPT IN r7 AND
 * REUSED AT TWO SITES. A reused column base in a callee-saved register cannot
 * come from a displacement. The two-statement address form reproduces it:
 *     e4 = *pE4;  c2 = (u16 *)e4 + 2;  ... c2[idx * 8] ...
 * which emits `add r6,r2,#4 / lsl r1,r0,#4 / add r0,r1,r6 / ldrh r4,[r0]`,
 * byte-for-byte the ROM. Same lever as sub_0803CFA4's `+2` this wave: a
 * constant offset that the store/load insn can absorb is folded below the tree
 * level, so NO spelling of a single expression reaches it -- only splitting the
 * address across two statements does.
 *
 * STILL OPEN, and both are named source constructs, not ties:
 *   - THE X-SUM's COLUMN 1 IS STILL A DISPLACEMENT. `((u16 *)*pE4 + 1)[i]`
 *     inline does NOT split (fold collapses it back); it needs its own two
 *     statements, like the if-block. MEASURED: adding `dc = *pDC; e4 = *pE4;
 *     cp = (u16 *)e4 + 1;` before the x-sum DOES produce the ROM's
 *     `add r7,r1,#0x2` there AND fixes the tail (the ROM keeps the chased
 *     `*pDC` POINTER in r7 and re-reads `ldrh r1,[r7]`, which `dc = *pDC;`
 *     reproduces) -- but in that configuration agbcc force-addrs
 *     gUnknown_020298E0 instead (a new `.LC2`), because freeing a register
 *     changes which symbol crosses the multi-block threshold. The two fixes
 *     are in tension through register pressure; that tension is the next
 *     wave's target, not a tie.
 *   - The constant 1 lands in a hi register (`mov r2,#1 / mov sl,r2 /
 *     mov r2,sl`) where the ROM has `movs r6,#1 / eors r0,r6` -- two extra
 *     instructions, downstream of the same pressure. */

/* WAVE 89 (W89-A).  ALL SCORES UNDER THE `configured` PROFILE.  Snapshot of
 * W88-C's fixpoint in work/sub_08050FF8/w89-start.c.
 *
 * THE PROMPT'S PRE-REGISTERED HYPOTHESIS WAS REFUTED, AND THE REFUTATION IS
 * THE USEFUL RESULT.  Wave 89 was told: the ROM re-loads gUnknown_03001FBC's
 * address from the pool at four sites where the candidate caches it in r8, and
 * the `static inline` re-read lever (wave 88) should force the re-derivation.
 * Both halves are wrong.
 *
 * 1. THE CANDIDATE'S PER-SITE INSTRUCTION COUNT WAS ALREADY THE ROM'S. At
 *    every gUnknown_03001FBC site after the first, the ROM emits
 *    `ldr r2,=sym / movs r0,#0 / ldrsh r1,[r2,r0]` and the candidate emits
 *    `mov r0,r8 / mov r2,#0 / ldrsh r1,[r0,r2]`. Three THUMB insns either way,
 *    two bytes each. The r8 cache costs ONE instruction in total -- the
 *    `mov r8,r0` in the prologue -- and NOT one per site. The whole "four
 *    re-loads vs a cached pointer" reading of this residual counts a
 *    difference of REGISTER NAME as a difference of SIZE.
 *
 * 2. THE `static inline` LEVER MAKES IT WORSE, NOT BETTER (measured, one
 *    probe). `static __inline__ __typeof__(gUnknown_03001FBC) fbc(void)
 *    { return gUnknown_03001FBC; }` called at all seven sites DOES delete the
 *    `mov r8,r0`, giving a prologue with the ROM's exact instruction count --
 *    and then cse merges the SCALED INDEX across the unk2c/unk30/unk34 stores,
 *    which the ROM recomputes at each one. Net about FIFTEEN instructions
 *    SHORT on a candidate that was already short. The wave-88 chapter's
 *    reachable set is "re-read a pool word AFTER A CALL"; sites 2-5 here have
 *    no call between them, so the lever never applied.
 *
 * 3. A VOLATILE READ (`#define FBC (*(volatile s16 *)&gUnknown_03001FBC)`)
 *    DOES reproduce the ROM's per-site re-derivation of the value and of all
 *    the arithmetic derived from it -- ten insns per store, exactly the ROM's
 *    ten -- but it emits `ldrh / lsl #16 / asr #16` where the ROM has
 *    `movs #0 / ldrsh`, because combine will not fold a volatile MEM into a
 *    sign_extend. Byte-neutral against the plain draft at those sites and a
 *    strict loss in the prologue. NOT the answer, and recorded here so nobody
 *    re-runs it.
 *
 * WHAT THE ROM'S RE-LOADS ACTUALLY MEAN -- READ THE ADDRESS AND THE VALUE
 * SEPARATELY. At the unk2c/unk30 pair the ROM loads `ldr r2,=sym` ONCE and
 * reuses r2 across BOTH stores while re-loading the value and recomputing
 * `v*3*32` at each. So cse DOES share the address there; the later "re-loads"
 * are reload rematerialising a `ldr rN,=sym` whose pseudo LOST its hard
 * register. That is the wave-17 diagnostic read backwards: the ROM holds the
 * address in a dying scratch because it has MORE live values than the
 * candidate, not because the source respells the reference. The prologue
 * `mov r8,r0` is therefore the LAST symptom of the -16 bytes, exactly as the
 * brief's "a prologue difference is the last symptom to appear and the first
 * one you see" says. DO NOT SPEND ANOTHER WAVE ON gUnknown_03001FBC's
 * SPELLING; spend it on the missing instructions.
 *
 * WHAT MOVED: W88-C's own two-statement address split, applied to the TWO
 * COLUMN-1 SITES IT LEFT. W88-C fixed column 2 (`c2 = (u16 *)e4 + 2`) and
 * wrote that the x-sum's column 1 "needs its own two statements" but banked
 * neither that nor the `unk04 == 0` test's. Both are done here:
 *     e4  = *pE4;  c1  = (u16 *)e4 + 1;   before the `unk04 == 0` test
 *     e4b = *pE4;  c1b = (u16 *)e4b + 1;  before the x-sum
 * Each emits the ROM's `adds r1,#2` as its own instruction where the draft had
 * folded it into `ldrh r1,[r0,#0x2]`. SEPARATE LOCALS FOR THE X-SUM ARE
 * DELIBERATE: the ROM re-chases *pE4 and re-adds #2 there rather than reusing
 * the if-block's value, and reusing one local across both would make one pseudo
 * with a live range spanning the calls (the wave-17 punctuation rule).
 *
 * REMAINING, in the order they are worth attacking:
 *   - THE `+2` AND THE INDEX SHIFT ARE EMITTED IN THE OPPOSITE ORDER TO THE
 *     ROM at both new sites (ROM `lsls r0,r5,#4` then `adds r1,#2`; candidate
 *     `add r1,r1,#0x2` then `lsl r0,r3,#0x4`). Same instructions, same bytes,
 *     wrong order -- so the ROM reads the SIDE before it forms the column
 *     base. Binding the side to its own local ahead of the column bind is the
 *     next thing to try. This is the identical residual sub_0803CFA4 carries.
 *   - THE ROM SETS UP sb=&_081360D8, r8=&_081360DC AND sl=&_081360E4 INSIDE
 *     the `unk04 == 0` block (four insns) where the candidate defers D8/DC to
 *     the following block (three insns). Move `pDC`/`pD8`'s binds above the
 *     `unk04 == 0` test to match.
 *   - THREE DISTINCT CONSTANT 1s. The ROM materialises `movs r5,#1` and
 *     `movs r7,#1` in the first block and a THIRD, `movs r6,#1`, at
 *     _080511A6, reusing r6 for both `^1`s in the if-block. The candidate
 *     merges the late one with the early one and pays `mov r2,#1 / mov r8,r2 /
 *     mov r3,r8` for it. This is wave-80's "same register, different block =
 *     one variable" read in reverse: THREE registers in three blocks = three
 *     source constants.
 *
 * AND THE 8 BYTES: THE POINTER-OBJECT BINDS BELONG INSIDE THE `unk04 == 0`
 * BLOCK. The ROM sets up sb=&_081360D8, r8=&_081360DC and sl=&_081360E4 after
 * the test expression is computed but BEFORE the conditional branch; the draft
 * bound pDC/pD8 after the if/else MERGE. No statement position exists between
 * an `if`'s condition and its branch -- so make one, by binding the test's
 * value to its own local:
 *     t = ((struct Unk85D6A48Row *)gUnknown_081360E0)[c1[side * 8]].unk04;
 *     pDC = &gUnknown_081360DC;
 *     pD8 = &gUnknown_081360D8;
 *     if (t == 0) ...
 * -16 BYTES -> -8 BYTES, 12.2% -> 14.8%, IN ONE EDIT. First movement on this
 * function since wave 37. See the doc chapter.
 *
 * MEASURED PROGRESSION, all `configured`:
 *     W88-C fixpoint (w89-start.c)      788  -16   11.9%
 *     + the two column-1 splits          788  -16   12.2%
 *     + the bind ordering (w89a-final.c) 796   -8   14.8%
 *
 * IGNORE `best.json` ON THIS FUNCTION. It reads 21.52% and `best.c` is the
 * 2,687-byte wave-37 file that HAS NOT COMPILED SINCE WAVE 86 (W88-B found the
 * header retype that broke it). That percentage describes source no oracle can
 * build. The draft's real score is the one in the table above.
 *
 * THE REMAINING RESIDUAL IS ONE ALLOCATION FACT, NOW FULLY NAMED. The ROM
 * keeps &gUnknown_0300453C in the HI register r8 and pays `mov rLow,r8` at
 * every one of its six reads, and keeps &gUnknown_020298E0 in r9 the same way;
 * the candidate keeps both in LOW registers and reads them directly, one
 * instruction shorter per read. That is the SAME FACT as the prologue's
 * `mov r8,r0`: in the ROM &gUnknown_03001FBC LOSES the callee-saved register
 * to &gUnknown_0300453C and is rematerialised, while in the candidate it WINS
 * r8. By the wave-17 tie-break (equal reference counts, first-created pseudo
 * wins) the candidate is behaving CORRECTLY -- both are read six times and
 * gUnknown_03001FBC is read first, in sub_0801566C's argument. The ROM inverts
 * the tie. The question for wave 90 is what gives gUnknown_0300453C the
 * heavier allocno -- NOT how to respell gUnknown_03001FBC, whose three
 * candidate levers are all measured and refuted above.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

/* WAVE 89 (W89-E). ALL SCORES UNDER THE `configured` PROFILE, re-measured.
 *
 *     W89-A's final (w89e-start.c)        796  -8  14.8%  677/796 differ
 *     + the byte-offset index binds       796  -8  15.4%  672/796 differ
 *
 * IGNORE `best so far: 21.5% (kept in best.c)`. best.c is still the wave-37
 * fossil and will not update until the draft passes 21.5%. The draft is the
 * deliverable.
 *
 * CONFIRMED AND BANKED: THE SHARED `+2`-VERSUS-INDEX-SHIFT ORDER IS CLOSED, and
 * the lever is a new one, not a reordering. Eight spellings of the column-1
 * reference across two compile_probes fall into exactly TWO classes and nothing
 * in between:
 *   - a BIND statement (`c1 = (u16 *)e4 + 1;`) always emits `adds #2` BEFORE
 *     the index shift -- and binding the SIDE first (W89-A's proposal) moves
 *     the side's LOAD up but leaves the shift after the `adds #2`; binding the
 *     ELEMENT index (`idx = side * 8`) is byte-identical to it, because combine
 *     folds the bind's *8 with the subscript's *2 at the use and the bind emits
 *     nothing at all.
 *   - ANY inline form of the `+1` -- 2-D subscript `e4[side][1]`,
 *     constant-in-index `b[side*8+1]`, index-first sum
 *     `*(side*8 + ((u16*)e4 + 1))`, fully inline -- DELETES the add into
 *     `ldrh [r0,#0x2]`. (W88's addressing-mode bound, on four more tree forms.)
 * The two instructions come from two different phases: `adds #2` is the bind
 * STATEMENT's expansion, pinned to that statement; the shift is the REFERENCE's
 * index operand, pinned to the reference.
 *
 * WHAT REACHES IT: BIND THE ALREADY-SCALED *BYTE* OFFSET, so the reference has
 * no scaling left to apply and the shift is materialised at the bind:
 *     off = gUnknown_0300453C * 16;      /+ NOT * 8 +/
 *     c1  = (u16 *)e4 + 1;
 *     ... *(u16 *)((u8 *)c1 + off) ...
 * Emits the ROM's order exactly: ldr / ldrh / lsls #4 / adds #2 / adds / ldrh
 * with no displacement. Applied at BOTH column-1 sites. Residue at the site is
 * only `adds r0,r0,r1` vs `adds r0,r1,r0` -- W80-D's commutative operand order,
 * a different axis. Chapter in docs/agbcc-codegen.md.
 *
 * REFUTED: THE PROMPT'S "THE ROM INVERTS THE WAVE-17 TIE" FRAMING. The ROM does
 * not invert a tie; it never enters &gUnknown_03001FBC into the competition.
 * Read the ROM's own prologue: `ldr r1,[pc,#336]` puts &gUnknown_03001FBC in a
 * LOW SCRATCH that DIES at the `bl sub_0801566C`, and only AFTER the call are
 * &gUnknown_020298E0 and &gUnknown_0300453C materialised into r9 and r8. There
 * is no pair of equally-ranked call-crossing allocnos competing for one
 * register. The candidate's prologue `mov r8,r0` exists because ITS
 * &gUnknown_03001FBC is one pseudo shared ACROSS the call; the ROM's is not.
 *
 * SO THE QUESTION FOR WAVE 90, RESTATED: what makes the address used at
 * `sub_0801566C(gUnknown_03001FBC, &oam)` fail to reach the four
 * `gUnknown_03001470[gUnknown_03001FBC]` stores after the call? That is a
 * cse/gcse unification across a `bl`, not an allocation tie -- and by W89-C's
 * own partition it is the ONE splitter class with a live lever (`static inline`
 * splits a memory bind, and a `bl` kills memory), not the dead one.
 *
 * WHICH ALSO EXPLAINS W89-A'S static-inline LOSS: it applied the helper at ALL
 * SEVEN sites, deleting the very pre-call reference that must stay bare (W89-C's
 * "leave at least one bare reference" trap), after which cse merged the scaled
 * index across the unk2c/unk30/unk34 stores. THE UNTESTED SPELLING IS THE
 * HELPER AT THE POST-CALL SITES ONLY, line 44's reference left bare. One
 * try_match; I ran out of budget before it.
 *
 * NOT RE-RUN (W89-A measured these; do not repeat): static inline at all seven
 * sites; the volatile read (loses the `ldrsh` -- combine will not fold a
 * volatile MEM into a sign_extend); the "four re-loads vs a cached pointer"
 * reading (the r8 cache costs ONE instruction in the whole function).
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero flips). */

/* WAVE 89 (W89-G). ALL SCORES UNDER THE `configured` PROFILE, re-measured
 * before and after. Snapshot in work/sub_08050FF8/w89g-start.c.
 *
 * VERIFIED INHERITANCE: 796 bytes, -8, 15.4%, 672 of 796 differ, first
 * difference at +0xc. Every W89-E figure reproduces exactly.
 *
 * REFUTED -- THE HELPER AT THE POST-CALL SITES ONLY. This was the one probe
 * W89-E left unrun, and the reason this batch existed.
 * `static __inline__ __typeof__(gUnknown_03001FBC) fbc(void)
 * { return gUnknown_03001FBC; }` called at the SIX post-call sites (unk28,
 * unk2c, unk30, unk34, sub_08015608's argument, sub_08050528's argument), with
 * the pre-call sub_0801566C(gUnknown_03001FBC, &oam) reference left BARE
 * exactly as W89-C's "leave at least one bare reference" trap requires:
 *
 *     W89-E fixpoint (w89g-start.c)   796  -8   15.4%  672/796 differ
 *     + helper at post-call sites     764  -40  10.2%  682/764 differ
 *
 * A 32-BYTE REGRESSION -- the same size W89-A measured for the all-seven-sites
 * spelling. Draft restored and re-verified at 796 / -8 / 15.4%.
 *
 * LEAVING THE PRE-CALL REFERENCE BARE DOES NOT RESCUE THE HELPER. W89-C's
 * bare-reference trap is about the -fforce-addr .rodata word mechanism; it is
 * not what costs the bytes here. The three adjacent gUnknown_03001470[...]
 * stores are ALL post-call, so they route through the helper together whether
 * or not a seventh reference stays bare, and cse merges across them regardless.
 *
 * THE RE-FRAMING, WORTH MORE THAN THE PROBE. W89-A and W89-E both treated the
 * ADDRESS of gUnknown_03001FBC as the object of this residual. Measured twice
 * now (all seven sites, and post-call only), THE ADDRESS IS NOT WHERE THE BYTES
 * ARE: every spelling that makes the address re-derive at each use also lets
 * cse merge the derived SCALED INDEX across the unk2c/unk30/unk34 stores, and
 * the candidate comes out 30-40 bytes SHORTER rather than closer. The ~15
 * missing instructions are THE SCALED-INDEX RECOMPUTATIONS AT THOSE THREE
 * STORES -- a cse VALUE unification of three identical computations, not an
 * address unification, not a gcse-across-a-bl fact, and not an allocation tie.
 * W89-E's reading of the ROM prologue stands (the pre-call address genuinely
 * dies at the bl); the inference that the helper reaches this residual does not.
 *
 * PRE-REGISTERED FOR WAVE 90, never run here: W87's REDUNDANT-MASK LEVER
 * (((v << 22 & 0xffc00000) >> 22)) APPLIED TO THE INDEX at the three stores. By
 * W89-C's partition the splitter for a VALUE cse is volatile, and W89-A's
 * volatile probe put the volatile on the FBC READ (which loses the ldrsh --
 * combine will not fold a volatile MEM into a sign_extend); that is a
 * measurement about the read, not the index. The mask lever attacks cse's value
 * numbering directly and combine deletes the AND, so it costs nothing.
 *
 * DO NOT SPEND WAVE 90 ON gUnknown_03001FBC'S ADDRESS AGAIN. Four levers
 * against it are now measured and refuted: static inline at all seven sites
 * (W89-A), static inline at the post-call sites only (W89-G), the volatile read
 * (W89-A), and the bare reference (W88-C). Toolchain axis not re-swept
 * (W81-E: 155 drafts x 7 profiles, zero exit-0 flips). */

/* WAVE 89 (W89-I). ALL SCORES UNDER THE `configured` PROFILE, re-measured, not
 * inherited: 796 bytes against 804 (-8), 15.4%, 672 of 796 differ, first
 * difference at +0xc. Every W89-E / W89-G figure reproduces. Snapshot in
 * work/sub_08050FF8/w89i-start.c. THIS BODY IS UNCHANGED BY ME -- no try_match
 * spent, because the probe below removed the reason to spend one.
 *
 * REFUTED: THE FIFTH SPLITTER (W89-D's fold-proof mask) AT THE THREE STORES.
 * Both halves of the pre-registered hypothesis are false.
 *
 *  1. THERE IS NO SHARED INDEX TO SPLIT. This draft ALREADY recomputes the
 *     scaled index at unk2c, unk30 and unk34 -- each store emits its own
 *     `mov rN,r8 / movs rM,#0 / ldrsh / lsls #1 / adds / lsls #5`, which is the
 *     ROM's sequence instruction for instruction (the ROM differs only in
 *     reaching &gUnknown_03001FBC by a pool `ldr` and in holding
 *     &gUnknown_03001470 in `sl` where this body uses `ip`). The intervening
 *     `str` kills the memory value, so cse cannot merge them. W89-G's "the ~15
 *     missing instructions are the scaled-index recomputations" was a
 *     measurement of the STATIC INLINE drafts -- there the read becomes a
 *     register value with no memory dependence, which is exactly why every
 *     helper spelling came out 30-40 bytes SHORT. Counted from the diff's hunk
 *     headers this candidate is short by 2/1/1/2 lines in four hunks and EQUAL
 *     in the last four: six lines, not fifteen.
 *  2. A FOLD-PROOF MASK ON A MEMORY-LOADED VALUE IS ABSORBED BY THE LOAD.
 *     One compile_probe, an 8-bit fold-proof mask on the unk30 index and a
 *     16-bit one on unk34: the AND neither survives to cse nor dies in combine
 *     -- combine folds it INTO the load, giving `ldrb r1,[r3]` and
 *     `ldrh r1,[r3]` in place of `movs r2,#0 / ldrsh r1,[r0,r2]`. One
 *     instruction SHORTER per site, wrong load width, on a body already 8 bytes
 *     short. Chapter appended to docs/agbcc-codegen.md bounding W89-D: the mask
 *     reaches a REGISTER value whose nonzero_bits already fit it, never a bare
 *     narrow-global read.
 *
 * WHAT THE RESIDUAL IS, read off the diff rather than inferred. This candidate
 * holds FOUR addresses in long-lived registers (r5=&gUnknown_0300453C,
 * r8=&gUnknown_03001FBC, r9=&gUnknown_020298E0, ip=&gUnknown_03001470); THE ROM
 * HOLDS THREE (r8=&gUnknown_0300453C, r9=&gUnknown_020298E0,
 * sl=&gUnknown_03001470) and rematerialises &gUnknown_03001FBC from the pool at
 * each group of uses. Holding one fewer address pushes the ROM's
 * &gUnknown_0300453C out of the low callee-saved bank into r8, where it pays a
 * `mov rLow,r8` at each of its ~6 reads; this body reads `ldrh rN,[r5]`
 * directly. That is where the ~5 instructions are, and it is ONE fact.
 * Read forwards: THE ROM BINDS THE SIDE VALUE WHERE THIS BODY BINDS THE SIDE
 * ADDRESS.
 *
 * FIVE LEVERS ARE NOW MEASURED AND REFUTED AGAINST gUnknown_03001FBC's
 * SPELLING -- do not re-run any of them: the `static inline` helper at all seven
 * sites (-15 insns); the helper at the six post-call sites with the pre-call
 * reference bare (764 bytes, -40, 10.2%); the `volatile` read (loses the
 * `ldrsh`; combine will not fold a volatile MEM into a sign_extend); the "four
 * re-loads versus a cached pointer" reading (the r8 cache costs ONE instruction
 * in the whole function); and the fold-proof mask (above). Every lever that
 * removes the hold also removes the memory dependence that keeps the three
 * store indices apart. THE NEXT LEVER MUST REMOVE THE HOLD WHILE LEAVING THE
 * REFERENCE A BARE MEMORY READ -- or, more promisingly, must attack the LOW
 * bank instead of the hold, giving r4-r7 a fifth competing value so that
 * &gUnknown_0300453C is forced into a hi register as it is in the ROM.
 *
 * Toolchain axis not re-swept (W81-E: 155 drafts x 7 profiles, zero exit-0
 * flips). `best so far: 21.5%` remains the wave-37 fossil; ignore it. */

## Header-level volatile ruled out (orchestrator, after wave 92)

Declaring `extern volatile u16 gUnknown_0300453C;` in include/unknown-globals.h
makes agbcc crash (`toplev.c:1074: Internal compiler error`) on the promoted
src/decomp/c_08052CA4.c. Keep the file-local `SIDE` macro in this draft.

## wave 95

Base: existing draft (800, -4, 17.8%), kept as `sub_08050FF8.w95-start.c`; draft unchanged.
- Lever 1 (per-block temp for a derived value) did not apply: the ROM's copies here are the `side * 144` index shared by two stores in the FIRST statement, not a saved-register copy.
- Finding: in the first statement (`gUnknown_02029906[SIDE][gUnknown_020298E0[SIDE].unk16 - 1] = 1`) the ROM reads the side ONCE and shares `side*144` between both subscripts. Naming the plain (non-volatile) global at BOTH positions of that statement (rest stays volatile) reproduces that statement's instructions exactly (single `ldrh`, `lsls #3; adds; lsls #4`, `adds r0,r1,r2`, `ldrh [r0,#22]`). But total size falls to 792 (-12) and the score to 15.2%, first difference still +0xc. Plain global at only ONE of the two positions is byte-neutral (800, 17.8%).
- Plain global everywhere: 796 (-8), 15.4%.
- Lever 2 (mixed bare / bound): this IS the mixed form for the side reads (statement 1 bare, others volatile); it fixes statement 1 but loses 8 bytes elsewhere, so not size-exact. Not transferred as a size fix.
- Residual unchanged: ROM holds &gUnknown_0300453C in r8, &gUnknown_020298E0 in r9 and reloads &gUnknown_03001FBC per group; ours holds the 03001FBC address in r6.
Proposed tried-line: "plain (non-volatile) side read in statement 1 only: that statement then matches but the function is 12 bytes short".

## wave 96

Base: unchanged draft (17.79%, size -4, first diff +0xc). Restored after one probe.

Pre-registered classification (ROM 31 hi-register moves vs draft 26 in the disassembly; the screen's 22 vs 13 counts `adds rX,rY,#0` copies too): the ROM's extra copies are NOT copies of one register. They are copies from THREE held hi registers, each holding one plain pool address for the whole body: r8 = &gUnknown_0300453C (about 10 `mov rX,r8`, early; later re-pointed at the cell gUnknown_081360DC), sb = &gUnknown_020298E0 (about 6, later the cell 081360D8), sl = &gUnknown_03001470 (about 7, later the cell 081360E4). So it is neither pure (a) nor pure (b): it is (b) three times over, with the holds set up by `ldr rX,=sym; mov hi,rX` before the first use. The draft holds 0300453C in the LOW register r7 (volatile read, no copies) and gUnknown_03001FBC in r6, where the ROM re-loads 03001FBC from the pool at every use (`ldr r1,=; movs r2,#0; ldrsh r0,[r1,r2]`) and holds nothing for it. That is why the first difference is +0xc: the draft's r6 hold of 03001FBC is the first thing the ROM does not have.

Probe: re-assigning `pE4 = &gUnknown_081360E4;` before each of the later cell reads (the lever that fixed the sub_0804A760 key loop by giving each arm its own pseudo): 816 bytes (+12), 18.4%, first diff +0xc. Wrong direction here: the ROM HOLDS the cell address in sl across the whole body (`mov r1,sl; ldr r2,[r1]` five times), it does not re-derive it. The loop-hoist mechanism of sub_0804A760 does not apply because this function has no loop.
Open lever: whatever stops the compiler holding gUnknown_03001FBC (used 8 times, no pseudo in the ROM) while it holds the three others in hi registers. wave 89's static-inline reader deleted the hold but cost more elsewhere.

## wave 97 (W97-X)

Base: levers.py candidate `2-58_5a-419` adopted (old draft kept as `sub_08050FF8.w97x-start.c`): `s16 t` plus the `unk08` index bound to a `u16 lv0` before the x sum. 17.79% -4 -> 25.00% size-exact (804), frame `sub sp #8` equal to the ROM's, first difference still +0xc. wrongc OK.
Probes (`var2.c`, VARIANT 0-3, binds placed AFTER the sub_0801566C call as the ROM does): binding &gUnknown_0300453C to a volatile pointer 24.9%; plus &gUnknown_020298E0 18.4%; plus &gUnknown_03001470 792 (-12) 14.6%. Binding before the call is worse (23.9 / 17.4 / 16.3). Binding the three held addresses explicitly does not reproduce the ROM's holds: the extra binds change the allocation of the cell pointers pDC/pD8 (they land in r8/r9/sl and the ROM's arrangement differs from +0xc on).
Not resolved: the ROM also holds constants 1 (r5, r7) and 0 (r3) live from the first store; not investigated.

## wave 97 (W97-Z)
Base unchanged (25.00%, size-exact 804, first diff +0xC). This function is NOT the `.rodata` word split: the draft has no `.LC` word for gUnknown_03001FBC, and the ROM's `ldr rX,=gUnknown_03001FBC` at each use is a plain constant pseudo that lost the register contest (reload rematerialises it). The draft lets it win r6. The ROM's three held hi registers are gUnknown_020298E0 (sb), gUnknown_0300453C (r8) and gUnknown_03001470 (sl); the draft holds 020298E0, 0300453C and the slot-table via `ip` only briefly. The ROM also holds the constant 1 in TWO registers (r5 for `SIDE ^ 1`, r7 for the `= 1` stores); the draft shares one (r4). One more long-lived value in the ROM is the likely reason FBC loses.
Measured (spellings.py): slot table bound to a local `sl` for the four `.unk28/2c/30/34` stores: 14.8% -8. Separate `u16 one1/one2` for the stores: identical. First/last FBC use through a local: identical / 24.5%. `SIDE ^ x1` with `int x1 = 1`: 23.13% (the `1` becomes a separate register but nothing else follows). All negative. The pseudo-count difference (two 1s) is not reachable by folding-proof source so far.
