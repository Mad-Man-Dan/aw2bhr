
## wave 95

No probe run. Reading the pre-registered chapters (two-statement split, member-array giv order) against the record: the wave-70 park already ran the two-statement `p = base; p += K;` split, before the loop (spills, kills the inner CSE) and in the body (const-propagates back). The chapters' successful splits use a base that is a LOAD (`e4 = *pE4`) or a two-element row read; here the base is a bare SYMBOL_REF, so cse folds base+K back. Nothing in the chapters gives a non-constant base for gUnknown_020298E0 / gUnknown_08551D22. Left at the wave-70 draft (360/364).
(draft left at the wave-70 draft.)

## wave 96
Base: wave-70 draft unchanged (38.19%, -4, first diff +0xa). Pre-registered "try un-binding" had nothing to un-bind (the draft has no address binds). Read the diff instead:
- First diff +0xa: the ROM keeps `c` in r8 (`mov r8,r2` straight from the zero-extended argument) and b*2 in r7; the draft puts `c` in r7 and b*2 in r8. Pure global-alloc priority between c and b*2; no source lever found.
- The ROM has TWO spill slots (`sub sp,#8`: [sp]=b*0x6c, [sp+4]=b*0x90) and hoists `&gUnknown_030045A0[b ^ 1]` into r4 at the top of each outer-loop pass, then reuses it for both table reads. The draft has one slot and recomputes `b ^ 1` (`movs r1,#1; eors; lsls; ldr; adds`) inside the taken branch. Binding `u16 *q = &gUnknown_030045A0[b ^ 1];` at the top of the outer body and reading `*q` for one or both reads: 29.95%, still -4, first diff still +0xa (worse: the local adds a pseudo without moving c/b*2). So the missing copies are that hoist; it needs LICM to hoist a computation out of a conditional block, which a source bind does not reproduce.
- The ROM also inlines `gUnknown_020298E0 + 0x1a` as `mov r0,sl; adds r0,#0x1a` per inner iteration (sl holds the bare symbol), i.e. `unk1a[j]` reached through the struct symbol, as the draft spells it, but the draft folds the +0x1a into a pool word.
Not permuter-run this wave (one slot; 08057164 ran instead).
Proposed status: unchanged; left = register roles of c/b*2 in loop 1 and the outer-loop hoist of the b^1 table address.

## wave 96 (result): 38.19% -> 79.40%, size-exact (364/364), first diff +0xa
Three permuter runs (900 s, 2 threads, chained). Kept, all reviewed by reading the diff against the start:
- Run 1 (38.2 -> 75.6): `side = b;` (an `unsigned int` copy) in the outer loop body, with `gUnknown_0202980A[side][i]` for the compare only. Lever 1 (two variables): the compare row is its own pseudo, separate from the store row `b`. Value-identical, correct C.
- Run 2 (75.6 -> 79.4): bind the column-0 table `d2a = gUnknown_08551D2A;` (`const u16 (*)[5]`) and use it for the LOOP-2 read only, leaving loop 1's read bare (lever 2, mixed bind, transferred: it works on a table base read across two loops); plus a dead `three = 3` assigned inline as the column subscript (`gUnknown_08551D22[..][three = 3]`), which is byte-relevant: dropping it costs 2.2 points (77.2%). It gives the constant its own pseudo. Correct C, if odd. An empty `if (1) {}` in the permuter's output was byte-neutral and is dropped.
- Run 3 (79.4 -> 81.3%) REJECTED: made `three` a `volatile` local and dropped `side`. Kept as `sub_0805634C.w96-perm3-volatile.c.wrongc`. Draft is run 2's result, locals renamed (`new_var` -> `d2a`, `new_var2` -> `three`), re-scored 79.40%.
Residual: `sub sp,#8` in the ROM against `#4` (the ROM spills BOTH b*0x6c and b*0x90 to the frame, the draft only the first), and the ROM holds the bare `gUnknown_020298E0` in sl and adds `#26` at run time (`mov r0,sl; adds r0,#0x1a`) where the draft folds the offset into a pool word (`.word 0x1a` addend on gUnknown_020298E0). Binding `struct Unk020298E0 *e = gUnknown_020298E0;` before or inside the outer loop and reading `e[b].unk1a[j]`: 60.4%, -4 (regresses; the bind removes the second spill slot the ROM needs).
Proposed status: register roles inside loop 2 (two frame slots, base symbol held with the +0x1a added at run time). Next step for a later wave: permute again from the current draft but reject any output that adds a volatile.

## wave 97 (W97-S)
Base: draft unchanged (79.40%, size+0, first diff +0xA). Tried (spellings.py): struct-pointer bind of gUnknown_020298E0[b] before/inside the i loop, `u16 *k = ...unk1a` bind before/inside, byte-offset spellings of the unk1a read: 32-36% (+4/+8), or byte-identical for the offset forms (the +0x1a stays folded into the pool word). Separate copies of b (lever 1): `s2 = b` for the 0x90-record read alone +8, `s3 = b` for gUnknown_020298EC alone +8/53%, BOTH copies gives frame `sub sp,#8` like the ROM (two spill slots) but -4 bytes, 56.6%, first diff +0x1E (stores b*0x6c twice to [sp]); so the two-slot frame is reachable, the bare-symbol-in-sl / +0x1a-at-run-time is not. Draft restored.
Proposed status: unchanged; tried adds "separate copies of b for the record read and the 0x98 row read reach the ROM's two-slot frame but lose 4 bytes".
