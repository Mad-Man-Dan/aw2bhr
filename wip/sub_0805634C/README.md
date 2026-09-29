# sub_0805634C

0x0805634C, 364 bytes, THUMB, parked.

Best score so far: 81.3% (best.c).

## What it does

Builds side `b`'s sort keys from ROM tables, adjusting the keys of up to five particular slots, then sorts them with sub_08056638(b). Does nothing if `c` is 0.

## How close it is

Compiles to the right size (364 bytes) with 79.4% of bytes identical. The compare row has its own variable (`side = b;`) and the D2A table's address is bound for the second loop's read only. What is left: the original's frame is 8 bytes where the draft's is 4, and it holds gUnknown_020298E0 in sl and adds 0x1a at run time.

## What is left

In the second loop the original loads the bare addresses of gUnknown_020298E0 and gUnknown_08551D22 and adds the member offsets (unk1a, and column 3) at run time before adding the varying index; every spelling so far folds the offset into the pool word or into the load. Separate `p = base; p += K;` statements produce the right instructions, but they must live exactly as long as the original's values: before the loop they get spilled, and inside it the compiler folds them back.

## Already tried

- Member, flat and pointer spellings (`gUnknown_020298E0[b].unk1a[j]`, `rec->unk1a[b * 72 + j]`, `gUnknown_08551D22[x][3]`, `c3 = gUnknown_08551D22[0] + 3`): the offset is folded into the pool word.
- A local holding the table's address, before or inside either loop: the whole address is moved out of the loop and a shared index computation is lost, or the offset goes into the load.
- Wrapper structs around the tables (a leading filler, or an array of row structs): the offset still folds, or goes into the load.
- Separate `p = base; p += K;` statements before the loops: the adjusted pointers are spilled; inside the loop body: folded back to base + K.
- Writing the `+` operands of the first loop the other way round: an extra force-addr word for gUnknown_0300450C appears; the draft's order is required.

## Files

- `sub_0805634C.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 70 at 360/364 (-4). First loop and all semantics are settled; the second loop needs two addresses emitted as bare symbol plus runtime constant before the varying offset. Separate p=base; p+=K statements prove that spelling for +0x1a and +6, but before-loop placement spills the adjusted pointers while body placement const-propagates back to base+K. Existing direct/member/pointer forms are documented in the draft; the remaining axis is lifetime/CSE, not transcription.

### Wave 95

No probe run. Reading the pre-registered chapters (two-statement split, member-array giv order) against the record: the wave-70 park already ran the two-statement `p = base; p += K;` split, before the loop (spills, kills the inner CSE) and in the body (const-propagates back). The chapters' successful splits use a base that is a LOAD (`e4 = *pE4`) or a two-element row read; here the base is a bare SYMBOL_REF, so cse folds base+K back. Nothing in the chapters gives a non-constant base for gUnknown_020298E0 / gUnknown_08551D22. Left at the wave-70 draft (360/364).
(draft left at the wave-70 draft.)

### Wave 96

Base: wave-70 draft unchanged (38.19%, -4, first diff +0xa). Pre-registered "try un-binding" had nothing to un-bind (the draft has no address binds). Read the diff instead:
- First diff +0xa: the ROM keeps `c` in r8 (`mov r8,r2` straight from the zero-extended argument) and b*2 in r7; the draft puts `c` in r7 and b*2 in r8. Pure global-alloc priority between c and b*2; no source lever found.
- The ROM has TWO spill slots (`sub sp,#8`: [sp]=b*0x6c, [sp+4]=b*0x90) and hoists `&gUnknown_030045A0[b ^ 1]` into r4 at the top of each outer-loop pass, then reuses it for both table reads. The draft has one slot and recomputes `b ^ 1` (`movs r1,#1; eors; lsls; ldr; adds`) inside the taken branch. Binding `u16 *q = &gUnknown_030045A0[b ^ 1];` at the top of the outer body and reading `*q` for one or both reads: 29.95%, still -4, first diff still +0xa (worse: the local adds a pseudo without moving c/b*2). So the missing copies are that hoist; it needs LICM to hoist a computation out of a conditional block, which a source bind does not reproduce.
- The ROM also inlines `gUnknown_020298E0 + 0x1a` as `mov r0,sl; adds r0,#0x1a` per inner iteration (sl holds the bare symbol), i.e. `unk1a[j]` reached through the struct symbol, as the draft spells it, but the draft folds the +0x1a into a pool word.
Not permuter-run this wave (one slot; 08057164 ran instead).
Proposed status: unchanged; left = register roles of c/b*2 in loop 1 and the outer-loop hoist of the b^1 table address.

### Wave 97

wave 97 (W97-S)
Base: draft unchanged (79.40%, size+0, first diff +0xA). Tried (spellings.py): struct-pointer bind of gUnknown_020298E0[b] before/inside the i loop, `u16 *k = ...unk1a` bind before/inside, byte-offset spellings of the unk1a read: 32-36% (+4/+8), or byte-identical for the offset forms (the +0x1a stays folded into the pool word). Separate copies of b (lever 1): `s2 = b` for the 0x90-record read alone +8, `s3 = b` for gUnknown_020298EC alone +8/53%, BOTH copies gives frame `sub sp,#8` like the ROM (two spill slots) but -4 bytes, 56.6%, first diff +0x1E (stores b*0x6c twice to [sp]); so the two-slot frame is reachable, the bare-symbol-in-sl / +0x1a-at-run-time is not. Draft restored.
Proposed status: unchanged; tried adds "separate copies of b for the record read and the 0x98 row read reach the ROM's two-slot frame but lose 4 bytes".

</details>
