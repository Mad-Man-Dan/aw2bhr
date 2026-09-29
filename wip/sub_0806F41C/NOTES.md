# sub_0806F41C — wave 92 (W92-B)

**15.38% at +4 bytes -> 61.36% SIZE-EXACT (308/308).** The structural residual
that parked this function since wave 36 is gone; what is left is a register-number
tie. The permuter did it, from the first run ever made on this function.

## What was wrong and what fixed it

The draft kept the address of `proc->unk38` live across the first if/else where
the ROM recomputes it. That cost a fifth callee-saved register, which pushed
`flag` into a high register, which forced its `!= 0` to come out as a branch
instead of the ROM's branchless `negs / orrs / lsrs` — one fact with three
symptoms, and it also made the candidate four bytes long.

`perm-w92-1.log` (900 s, 4 threads, base score 5960): no byte match, but it
improved the draft to 61.36% at the ROM's exact size, from
`permuter/output-3240-1/source.c`. Two of its edits are load-bearing and both
are recorded in the source's "Why the C looks odd" note:

    flag = 0;
    flag = sub_0803CBD8(0x22) != flag;

    new_var = proc->unk38;
    ApplyPaletteExt(tbl[proc->unk38].unk08, 0, tbl[new_var].unk0c << 5);

The prologue now pushes four callee-saved registers, not five, so the
`mov r7, r8 / push {r7}` pair is gone and `flag` is branchless. **Do not fold
either statement back into one** (the standing wave-59 rule).

## It is semantically sound — checked, unlike the other permuter result this wave

The candidate was read before being kept, because the same run of the tool on
sub_080607E8 produced a mutation that was not sound. Here everything checks out:

  * `SetWinEnable(0, 0, 0)` had been expanded into its three field writes.
    `include/hardware.h` defines it as a macro that is exactly those three
    writes, so the expansion is token-identical. The macro call has been put
    back and the result re-verified.
  * `new_var` is `s8` and is assigned `proc->unk38`, also `s8`, **before** its
    use — no use-before-assignment, unlike the artefacts in this function's
    neighbours.
  * `flag = 0; flag = ... != flag;` computes the same boolean.
  * The rest was brace and formatting churn.

Re-verified after tidying to project style: 308 bytes, 61.36%, unchanged.

## The pool-word premise in the brief is false here

The brief said gUnknown_0816E808 is the compiler's own pointer word and that the
table at 0x085828DC should be named directly as an offset into
gUnknown_08582764. The ROM listing says otherwise: the pool word is
`=gUnknown_0816E808`, the word at 0x0816E808 holds 0x085828DC, and the ROM
dereferences that pointer object twice — `ldr r1,[r2]`, then after copying the
address with `adds r3,r2,#0`, `ldr r6,[r3]`. Naming the table directly would
delete a load.

The size-exact result settles it from the other side: with the declaration
`extern struct Unk0816E808Entry *gUnknown_0816E808;` the candidate now has the
ROM's exact byte count, so the load count is right. The 0x10 record size is
right too — the ROM scales the index with `lsls #4`. The parked entry's `left`
field has been corrected.

## The wave-89 bind lever, tested and refuted here

The ROM's `ldr r2,=X / ... / adds r3,r2,#0` is the shape wave 89 reproduced on
sub_08061DCC by binding a symbol's address to a local while leaving the first
reference bare, and this draft had the two halves in the opposite order. Both
hoists were measured before the permuter run: bind above the bare unk0d test,
8.33%; bind above sub_0806E210 as well, 9.62%. Size moved in neither, so the
hoist only shuffled registers. W66-M's finding stands — gcse reunifies the two
`proc + 0x38` address computations however the source spells them — and the
permuter reached the same end by another route.

## Residual and the next step

41 of 308 bytes... in fact 119 of 308 differ, but the first difference at +0x4 is
a register number: the ROM holds the proc pointer in r5 and the 0x7f mask in r6,
the candidate has them the other way round. Everything downstream follows from
that swap. This is a pure allocation tie on a size-exact function, which is the
case wave 59 closed by **chaining permuter runs, each from the previous best**
(87.3 -> 94.3 -> 98.2 -> match).

That chain is the pre-registered next step and was not started only to avoid
leaving a permuter running into the orchestrator's verification pass — a stray
run can overwrite the draft while it verifies. To continue safely:

    cp work/sub_0806F41C/sub_0806F41C.c work/sub_0806F41C/<snapshot>.c
    python tools/permute.py sub_0806F41C --seconds 900 --threads 4 --current \
        > work/sub_0806F41C/perm-w92-2.log 2>&1

`work/sub_0806F41C/sub_0806F41C.w92-start.c` is the 15.38% draft this wave began
with, and `w92-perm-keep.c` is the permuter's untidied output.

## Pool words owned

    _0806F484 -> gUnknown_0816E808   (the pointer object, not the table)

plus gDispIo, gUnknown_08582AF4, gUnknown_03002B6C, gUnknown_08582CAC,
gUnknown_08499578 and 0x01000010.

## wave 97

Base: wave-92 draft (61.36%, 308 B). Now 82.79% size-exact, first diff still +0x4 (r5/r6 swap); snapshot `sub_0806F41C.w97-perm1-start.c`.
- The wave-92 draft's pool word for `gUnknown_0816E808` was the compiler's `.rodata` force-addr word (double load). Fix: read the table into a bare local first (`t = gUnknown_0816E808;`, used for the first `unk0d` test), THEN bind `pp = &gUnknown_0816E808;` and take the later table pointer as `tbl = *pp`. The pool word becomes the plain `=gUnknown_0816E808` (61.4 -> 80.2%). Binding pp BEFORE the first read (or at the top) instead: 304 B / 11% (frame grows); binding after but reading `tbl = *pp` vs `*(pp = &..)`: unchanged.
- `{ u8 v = raw8; raw8 = v | 0x80; }` in the set arm (instead of `raw8 = raw8 | 0x80`) restores the ROM's `movs r2,#128; orrs r1,r2; strb` shape: 80.2 -> 82.8%. The same for the `& 0x7f` arm: 304 B, worse. `|=` unchanged.
- Residual: proc in r6 / 0x7f mask in r5 (ROM has them swapped) and the copy of the pool address (`adds r3,r2,#0`) lands right after the load in the ROM but before the index computation here (the ROM loads through r2, the draft through r0). Permuter chain from this base, 900 s x1: NO-IMPROVEMENT.
Proposed summary: left=register swap proc/mask (r5/r6) and where the pool-address copy is placed; tried=pool-word bind orders, arm store spellings, permuter.

## wave 97 (W97-AA)
Started from the wave-97 draft (82.79%); installed the levers.py chain-best change, cleaned: `do { } while (0)` around the six-statement
else arm (proc restart / CpuFastSet / Proc_Goto). 82.79% -> 87.01%, size-exact, first difference +0x04 -> +0x41: the proc (r5) / 0x7f mask
(r6) swap is GONE. wrongc: OK (400 seeds). Chain-best's dead `__typeof__ lv0` declaration was dropped (no effect). Snapshot of the
previous file: `sub_0806F41C.w97aa-start.c`.
Residual (40 bytes): (1) the ROM loads the table pointer into r2 and copies it to r3 AFTER the unk0d load (`ldr r2,=pool; ldr r1,[r2]; ...
adds r3,r2,#0`); ours loads through r0 and copies right after the load; (2) the ROM builds `tbl+4` / `tbl+8` as separate adds before adding the
scaled index for the Decompress unk04 and palette unk08 reads (`adds r1,r6,#4; adds r0,r0,r1; ldr r0,[r0]`); ours folds them into `ldr r0,[r0,#4]`.
Five spellings of (2) (`*(u8 **)((u8 *)tbl + 4 + idx*16)`, index-first sum, either or both reads) are byte-identical to the plain member read:
combine folds the constant into the load address whatever the source order. Proposed left: pool-address register (r2 vs r0) and the
un-folded +4/+8 in the else arm.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.
