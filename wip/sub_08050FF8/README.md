# sub_08050FF8

0x08050FF8, 804 bytes, THUMB, parked.

Best score so far: 25.0%.

## What it does

Sets up the current sprite object for one side and slot, taking its tile number from the other side's record, then computes the sprite's position and places it. When the other side's unit has a certain flag it also plays an alternating effect through sub_0803B48C.

## How close it is

Compiles 4 bytes short (800 against 804). The percentage is low because the difference starts in the first instructions and the size difference shifts everything after it, so read the diff rather than the score.

## What is left

4 bytes. Half of the old 8-byte gap was the side variable, which the original re-reads from memory at every use; that is fixed. The rest is a different mechanism and has not been identified.

## Already tried

- Reading the side variable through a volatile pointer so it is re-read at each use: 4 bytes better and kept. This is what the earlier -fno-force-mem finding was pointing at.
- Naming the four globals the draft reaches through compiler-made cells directly: 40 bytes worse. The cells have to be chased the way the draft does.
- Making the second route to the side variable volatile as well: 68 bytes too long.
- Five ways of respelling the sprite-object global (earlier waves): all measured, none helps.

## Files

- `sub_08050FF8.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

Worked across several waves without a match; the full record is the draft's header comment and the notes files in work/sub_08050FF8/.

### Wave 92

- **agent:** W92-A
- **measured:** 800/804 (-4), 17.79%, first difference +0xc, up from 796/804 (-8), 15.42%.
- **moved:** A volatile re-read of gUnknown_0300453C. The wave-91 flag sweep found -fno-force-mem makes this function size-exact, which says the missing bytes are memory operands our build holds in registers and the original re-reads. W89-A's volatile probe was aimed at gUnknown_03001FBC, where a volatile MEM cannot fold into a sign_extend and the ldrsh is lost; gUnknown_0300453C is read with ldrh, so that objection does not apply and nobody had tried it. Half the size gap in one edit, and the first movement since wave 89.
- **header_question_for_the_orchestrator:** The draft carries this as a file-local macro over a cast, because 42 promoted files read gUnknown_0300453C and a shared declaration must not be retyped unilaterally. If those 42 can be re-verified, the honest fix is to declare gUnknown_0300453C volatile u16 in include/unknown-globals.h and drop the macro.
- **refuted:** - The wave-92 brief's own plan for this function -- naming gUnknown_03004580, gUnknown_0300453C, gUnknown_020298E0 and gUnknown_085D6A48 directly instead of the pE4/pDC/pD8 binds. 756 bytes (-48), 11.82%: a 40-byte regression, because the honest spelling folds the base into every use and deletes the per-use re-chase W88-C installed deliberately. This axis is now closed from both ends, the bare pointer-object reference (W88-C) and the target global (here).
- Extending the volatile to the other route to the same variable (retyping the pDC local to volatile u16 *const *): 868 bytes (+68), 14.22%. Only the direct reads want it.
- **next:** Re-run -fno-force-mem from the new fixpoint and say whether the last 4 bytes are the same mechanism.

### Wave 95

Base: existing draft (800, -4, 17.8%), kept as `sub_08050FF8.w95-start.c`; draft unchanged.
- Lever 1 (per-block temp for a derived value) did not apply: the ROM's copies here are the `side * 144` index shared by two stores in the FIRST statement, not a saved-register copy.
- Finding: in the first statement (`gUnknown_02029906[SIDE][gUnknown_020298E0[SIDE].unk16 - 1] = 1`) the ROM reads the side ONCE and shares `side*144` between both subscripts. Naming the plain (non-volatile) global at BOTH positions of that statement (rest stays volatile) reproduces that statement's instructions exactly (single `ldrh`, `lsls #3; adds; lsls #4`, `adds r0,r1,r2`, `ldrh [r0,#22]`). But total size falls to 792 (-12) and the score to 15.2%, first difference still +0xc. Plain global at only ONE of the two positions is byte-neutral (800, 17.8%).
- Plain global everywhere: 796 (-8), 15.4%.
- Lever 2 (mixed bare / bound): this IS the mixed form for the side reads (statement 1 bare, others volatile); it fixes statement 1 but loses 8 bytes elsewhere, so not size-exact. Not transferred as a size fix.
- Residual unchanged: ROM holds &gUnknown_0300453C in r8, &gUnknown_020298E0 in r9 and reloads &gUnknown_03001FBC per group; ours holds the 03001FBC address in r6.
Proposed tried-line: "plain (non-volatile) side read in statement 1 only: that statement then matches but the function is 12 bytes short".

### Wave 96

Base: unchanged draft (17.79%, size -4, first diff +0xc). Restored after one probe.

Pre-registered classification (ROM 31 hi-register moves vs draft 26 in the disassembly; the screen's 22 vs 13 counts `adds rX,rY,#0` copies too): the ROM's extra copies are NOT copies of one register. They are copies from THREE held hi registers, each holding one plain pool address for the whole body: r8 = &gUnknown_0300453C (about 10 `mov rX,r8`, early; later re-pointed at the cell gUnknown_081360DC), sb = &gUnknown_020298E0 (about 6, later the cell 081360D8), sl = &gUnknown_03001470 (about 7, later the cell 081360E4). So it is neither pure (a) nor pure (b): it is (b) three times over, with the holds set up by `ldr rX,=sym; mov hi,rX` before the first use. The draft holds 0300453C in the LOW register r7 (volatile read, no copies) and gUnknown_03001FBC in r6, where the ROM re-loads 03001FBC from the pool at every use (`ldr r1,=; movs r2,#0; ldrsh r0,[r1,r2]`) and holds nothing for it. That is why the first difference is +0xc: the draft's r6 hold of 03001FBC is the first thing the ROM does not have.

Probe: re-assigning `pE4 = &gUnknown_081360E4;` before each of the later cell reads (the lever that fixed the sub_0804A760 key loop by giving each arm its own pseudo): 816 bytes (+12), 18.4%, first diff +0xc. Wrong direction here: the ROM HOLDS the cell address in sl across the whole body (`mov r1,sl; ldr r2,[r1]` five times), it does not re-derive it. The loop-hoist mechanism of sub_0804A760 does not apply because this function has no loop.
Open lever: whatever stops the compiler holding gUnknown_03001FBC (used 8 times, no pseudo in the ROM) while it holds the three others in hi registers. wave 89's static-inline reader deleted the hold but cost more elsewhere.

### Wave 97

wave 97 (W97-X)
Base: levers.py candidate `2-58_5a-419` adopted (old draft kept as `sub_08050FF8.w97x-start.c`): `s16 t` plus the `unk08` index bound to a `u16 lv0` before the x sum. 17.79% -4 -> 25.00% size-exact (804), frame `sub sp #8` equal to the ROM's, first difference still +0xc. wrongc OK.
Probes (`var2.c`, VARIANT 0-3, binds placed AFTER the sub_0801566C call as the ROM does): binding &gUnknown_0300453C to a volatile pointer 24.9%; plus &gUnknown_020298E0 18.4%; plus &gUnknown_03001470 792 (-12) 14.6%. Binding before the call is worse (23.9 / 17.4 / 16.3). Binding the three held addresses explicitly does not reproduce the ROM's holds: the extra binds change the allocation of the cell pointers pDC/pD8 (they land in r8/r9/sl and the ROM's arrangement differs from +0xc on).
Not resolved: the ROM also holds constants 1 (r5, r7) and 0 (r3) live from the first store; not investigated.

wave 97 (W97-Z)
Base unchanged (25.00%, size-exact 804, first diff +0xC). This function is NOT the `.rodata` word split: the draft has no `.LC` word for gUnknown_03001FBC, and the ROM's `ldr rX,=gUnknown_03001FBC` at each use is a plain constant pseudo that lost the register contest (reload rematerialises it). The draft lets it win r6. The ROM's three held hi registers are gUnknown_020298E0 (sb), gUnknown_0300453C (r8) and gUnknown_03001470 (sl); the draft holds 020298E0, 0300453C and the slot-table via `ip` only briefly. The ROM also holds the constant 1 in TWO registers (r5 for `SIDE ^ 1`, r7 for the `= 1` stores); the draft shares one (r4). One more long-lived value in the ROM is the likely reason FBC loses.
Measured (spellings.py): slot table bound to a local `sl` for the four `.unk28/2c/30/34` stores: 14.8% -8. Separate `u16 one1/one2` for the stores: identical. First/last FBC use through a local: identical / 24.5%. `SIDE ^ x1` with `int x1 = 1`: 23.13% (the `1` becomes a separate register but nothing else follows). All negative. The pseudo-count difference (two 1s) is not reachable by folding-proof source so far.

</details>
