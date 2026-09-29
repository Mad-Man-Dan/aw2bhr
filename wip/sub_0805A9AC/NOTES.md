
## Wave 91 (W91-B)

Member form: NEGATIVE, byte-identical. The draft already reads every map
field through a local `struct Map5A9AC` cast. Respelled as `struct Map`
(`->danger/->rowOffset/->unit/->terrain/->width/->height`, and `->move` for
the sub_0801F92C argument), and as `gMap->`: identical, 37.57% / +28 with the
volatile unit view and 35.66% without it.

Mechanism of the +28, from a -da dump (rtl-w91/, source w91-v3.c):
- The surplus `.LC1` (the force-addr word for gUnknown_08499590) is GCSE
  PRE. gcse inserts `(set (reg 482) (symbol_ref .LC1))` after the n-loop
  init (insn 987) and turns every in-loop map access into a copy of 482. With
  482 multi-use, combine cannot fold the `.LC`/(mem) pair into a plain
  `ldr =gUnknown_08499590`.
- The in-loop map-ADDRESS pseudo 148 (16 refs / 160 insns, priority
  4*16/160) then narrowly beats `i` (reg 30, 25 refs / 256 insns,
  4*25/256) for sl. `i` spills (+4 frame) and the cascade follows. In the ROM
  the address pseudo LOSES. It is rematerialised per use from REG_EQUIV as a
  plain `ldr rX,=gUnknown_08499590` in whatever reload register comes next
  (r7, r2, r3, r0 at the four sites; that is the tell), and `i` keeps sl.
  A priority tie decided by about 2%: a permuter case.
- The ROM DOES use gcse here: its prologue has three `.rodata` words
  (0x0816D9B4/B8/BC). Under -fno-gcse none appear, and the draft is +8.

900 s permuter from --current (perm-w91-1.log; 27,828 iterations, 773
improving candidates). SIZE-EXACT candidates exist for the first time:
output-10764-1 at 50.68% and output-9844-1 at 40.98%, plus several at -8 and
-12. 10764 binds `new_var = (struct Map5A9AC *)gUnknown_08499590;` at the TOP
of the j body and reads some of the rowOffset / danger indexes through it.
9844 hoists the width into a `u16` read before the gUnknown_030013EC call.
w91-perm10764-clean.c is a readable transcription of 10764. It compiles to
the same instruction stream as the permuter's source.c (checked by rtldump).
Next: chain a permuter run from it.

Permuter outcome (after the run finished): permute.py kept output-8096-1
(67.76% but +4). It wraps most of the n-loop body in `do { ... } while (0)`,
so the body's `continue`s now leave the do and run the tail. That is
SEMANTICALLY WRONG C and was discarded. Its valid half (`vp =
&gUnknown_08499590` per i iteration, used for the volatile unit view) is +32
on the old draft and +8 on 10764: worse. THE DRAFT IS NOW
w91-perm10764-clean.c (verified with trymatch: 732/732, 50.68%, first
difference +0xa). The old +28 draft is w91-start.c.

## wave 96

Base: the 50.68% draft (`sub_0805A9AC.w96-start.c`, first +0xa, frame `sub sp,#40` vs ROM #36 because `i` spills).
Un-binding, measured over all 32 subsets of the five `new_var->` sites (one unit, size + try_match):
- Un-binding ONLY the last one (the post-loop `bv = ...` read after the n-loop, where `new_var` was a stale bind
  from the j loop) gives the ROM's frame (`sub sp,#36`), size 728 (-4), first difference moves +0xa -> +0x3e
  (score drops 50.7 -> 42.3 because the size shift moves later bytes). Kept as the draft and as `w96-m16.c`.
  Mechanism: that use kept the map-address pseudo alive across the whole n-loop, which is what beat `i` for sl.
- Un-binding the first two sites (unk2D5A guard, danger index) changes nothing (same bytes).
- Un-binding site 3 or 4 (the `unk1432` sites) is worse (+8 / first +0xa again).
- Un-binding all five and deleting the bind: 760 (+28), frame 44.
- Removing the volatile read (plain cast at that site): 724 (-8), same frame.
Residual after the kept change: the running best (`best.raw`) sits in r9/r4 with a `mov r4,r9; ands; orrs; mov r9,r4`
round-trip where the ROM keeps it in r6; the map address is held in r8 (`mov r8,r1; mov r6,r8`) where the ROM
re-derives `ldr r7,=gUnknown_08499590; ldr r2,[r7]` per site (rule 1: re-derive form); and the 4 missing bytes.
NOT yet tried: re-assigning a pointer-to-cell local at the top of each of the four arms (rule 1 form).
Proposed summary: left: map address held in a hi register (ROM re-derives per use); `best` in r9 not r6; -4 bytes.
tried += new_var unbound at the post-loop use (frame now matches).
Rule-1 re-derive probe (one unit, on the kept form): rebinding `gp = &gUnknown_08499590` per use inside the j body
(expression form `(*(gp = (struct Map5A9AC **)&g))`): all sites 736 and frame 40 (worse); only the first site 728, frame 36 (= kept form);
volatile site only 728 frame 36; unk1432 sites only 732... no variant reaches 732 with frame 36. The re-derive form does not transfer here
because the ROM's per-site `ldr r7,=gUnknown_08499590; ldr r3,[r7]` uses the plain pool word, not a rodata cell.

## wave 97 (W97-X)

Base: levers.py `3a-206+5a-339` (loop step through a copy `nj = j + 1; ... j = nj`, plus the `unk1432` tile read bound to a local before the sub_08026FD0 call), hand-written into the draft (old draft `sub_0805A9AC.w97x-start.c`, the nj-only form `sub_0805A9AC.w97x-nj.c` is the draft). 42.35% -4 -> 65.16% size-exact (732), first difference still +0xa, frame `sub sp,#40` (ROM #36: nj takes a slot).
The `tile` local alone changes nothing (65.03% without it). wrongc says WRONG on this family (extra sub_08042D1C call on seed 117, args (0x41DA63EB, 0x66)): a1 = 0x41DA63EB indexes `t[a1]` far out of range, so the emulated stack contents differ between two frames and the read differs; I believe this is a false positive but could not prove it. The old w96-start draft gets the same verdict against the current base, and the base against itself is OK. Treat 65% as unconfirmed until someone reads seed 117.
Without nj (va: j++) the frame matches (`#36`) but size is 728 and first diff +0x3e. The 4 bytes are the ROM's `adds r4,#1` step vs the draft's stack-resident nj; the ROM has j in r4 with no copy, so the +4 is elsewhere (the map address held in r8 where the ROM reloads `ldr rN,=gUnknown_08499590` at each site, and the j-loop bound reload is a plain pool load in the ROM).

## wave 97 (orchestrator check)
The W97-X draft (65.16%) is equivalent C by reading: nj copy-back step (int) and `u8 tile` for a u8 array element passed to a u8 parameter. wrongc WRONG on seed 117 comes from an out-of-range random index reading stack memory whose layout differs between the two frames (0x28 vs 0x24), not from the source change. Kept.
