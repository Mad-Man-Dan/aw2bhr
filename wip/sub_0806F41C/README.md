# sub_0806F41C

0x0806F41C, 308 bytes, THUMB, parked.

Best score so far: 87.0%.

## What it does

Rebuilds the screen for one entry of a 16-byte screen table, chosen by the proc: it turns the windows off, ends related procs, and either loads the entry's graphics or restarts the gUnknown_08582CAC proc.

## How close it is

Compiles to the right size (308 bytes) with 87.0% of bytes identical; wrapping the else arm in `do { } while (0)` moved it from 82.8%. What is left: the pool-address register (r2 against r0) and the original's unfolded +4 / +8 adds in the else arm.

## What is left

Chain another permuter run from the current draft, which is wave 59's recipe for exactly this residual (87.3 -> 94.3 -> 98.2 -> match on that function, where a single run from the first draft found nothing). Snapshot the draft first; the run that produced the current one is in work/sub_0806F41C/perm-w92-1.log and its untidied output is w92-perm-keep.c. Do NOT fold either of the two load-bearing statements back into one -- the split assignment to flag, and the subscript that goes through new_var -- and do NOT name the table at 0x085828DC directly: gUnknown_0816E808 is a real pointer object in ROM that the ROM dereferences twice, and the size-exact result confirms the load count is right.

## Already tried

- Binding tbl inside the unk0d test: fixes the order of one load but costs a second high register; worse.
- Two separate pointer locals for the two uses of proc->unk38: the compiler merges them back; byte-identical.
- Dropping tbl and naming the table at all five uses: 12 bytes too long, 8.8%.
- Reaching unk38 through a byte pointer or an integer address: byte-identical.
- Volatile views of unk38 after the branch: force reloads, but in a different form, and the old address is still kept.
- Wave 92: hoisting the tbl bind above the bare unk0d test, the wave-89 leave-one-bare shape: 8.33%, still +4 bytes. Hoisting it above sub_0806E210 as well: 9.62%, still +4. Size moved in neither, so the hoist only shuffled registers; gcse reunifies the two proc + 0x38 address computations however they are spelled (W66-M), and the permuter reached the fix by another route.
- Wave 92: naming the table at 0x085828DC instead of gUnknown_0816E808 was NOT tried and should not be -- the ROM's pool word is =gUnknown_0816E808, the word there holds 0x085828DC, and the ROM loads through it twice; the candidate is now size-exact with the pointer-object declaration, which settles the load count.

## Files

- `sub_0806F41C.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Best so far

61.36% identical, SIZE-EXACT (308/308)

### What still differs

A register-number tie on a size-exact candidate. First difference at +0x4: the ROM keeps the proc pointer in r5 and the 0x7f mask in r6, the candidate swaps them, and everything downstream follows. The wave-36 chain -- address of proc->unk38 held across the if/else, fifth saved register, flag in a high register, branch instead of negs/orrs/lsrs -- is RESOLVED.

### Notes

Improved in wave 92 from 15.38% at +4 bytes to 61.36% size-exact. Two statements in the draft are load-bearing and are documented in the source's own note: flag is assigned zero and then reassigned from the comparison against itself, and the last argument's subscript goes through new_var instead of reading proc->unk38 a third time.

### Wave 92

WAVE 92 (W92-B): no movement (15.38%, +4). THE POOL-WORD PREMISE IS FALSE HERE. The ROM listing and baserom agree: this function's pool word is =gUnknown_0816E808 (0x0816E808), the word at 0x0816E808 holds 0x085828DC, and the ROM dereferences that pointer object TWICE -- ldr r1,[r2], then, after copying the address with adds r3,r2,#0, ldr r6,[r3]. Naming the table directly would delete a load, so extern struct Unk0816E808Entry *gUnknown_0816E808 is correct, and so is the 0x10 record size (the ROM scales the index with lsls #4). The left field, which recommended extern struct Unk0816E808Entry gUnknown_085828DC[], has been corrected. THE WAVE-89 BIND LEVER DOES NOT TRANSFER. The ROM's ldr r2,=X / ... / adds r3,r2,#0 is the shape wave 89 reproduced on sub_08061DCC by binding the symbol's address to a local while leaving the first reference bare, and this draft has the two halves in the opposite order: bare read first, bind after. Hoisting the bind above the bare unk0d test gives 8.33%, still +4; hoisting it above sub_0806E210 as well gives 9.62%, still +4. Size moved in neither, so the hoist only shuffled registers, and the reason is already in this entry -- W66-M measured that gcse reunifies the two proc + 0x38 address computations whatever the source does. PERMUTER: the first run ever on this function (900 s, 4 threads, from the draft), base score 5960, search reached about 2980, with several size-exact and size-4 candidates left in work/sub_0806F41C/permuter/output-*/ for the next wave to read. OUTCOME: the permuter closed the structural residual. 900 s, 4 threads, base score 5960, no byte match, but it improved the draft from 15.38% at +4 bytes to 61.36% SIZE-EXACT from permuter/output-3240-1/source.c. The candidate was READ before being kept, because the same tool's result on sub_080607E8 this wave was not sound: here it is. Its SetWinEnable expansion is token-identical (hardware.h defines the macro as exactly those three field writes) and the macro call was put back and re-verified; new_var is s8, assigned from the s8 proc->unk38 BEFORE its use, so there is no use-before-assignment; and flag = 0 followed by flag = ... != flag computes the same boolean. The prologue now pushes four callee-saved registers instead of five and the flag test is branchless, so the whole wave-36 chain is resolved. Re-verified after tidying to project style. NEXT: chain a second permuter run from this draft (wave 59's recipe); it was not started only to avoid leaving a run alive into the orchestrator's verification pass.

### Wave 97

wave 97
Base: wave-92 draft (61.36%, 308 B). Now 82.79% size-exact, first diff still +0x4 (r5/r6 swap); snapshot `sub_0806F41C.w97-perm1-start.c`.
- The wave-92 draft's pool word for `gUnknown_0816E808` was the compiler's `.rodata` force-addr word (double load). Fix: read the table into a bare local first (`t = gUnknown_0816E808;`, used for the first `unk0d` test), THEN bind `pp = &gUnknown_0816E808;` and take the later table pointer as `tbl = *pp`. The pool word becomes the plain `=gUnknown_0816E808` (61.4 -> 80.2%). Binding pp BEFORE the first read (or at the top) instead: 304 B / 11% (frame grows); binding after but reading `tbl = *pp` vs `*(pp = &..)`: unchanged.
- `{ u8 v = raw8; raw8 = v | 0x80; }` in the set arm (instead of `raw8 = raw8 | 0x80`) restores the ROM's `movs r2,#128; orrs r1,r2; strb` shape: 80.2 -> 82.8%. The same for the `& 0x7f` arm: 304 B, worse. `|=` unchanged.
- Residual: proc in r6 / 0x7f mask in r5 (ROM has them swapped) and the copy of the pool address (`adds r3,r2,#0`) lands right after the load in the ROM but before the index computation here (the ROM loads through r2, the draft through r0). Permuter chain from this base, 900 s x1: NO-IMPROVEMENT.
Proposed summary: left=register swap proc/mask (r5/r6) and where the pool-address copy is placed; tried=pool-word bind orders, arm store spellings, permuter.

wave 97 (W97-AA)
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

</details>
