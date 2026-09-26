# sub_08050FF8

0x08050FF8, 804 bytes, THUMB, parked.

Best score so far: 17.8%, -4 bytes (best.c).

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
- `best.c`: the closest attempt, when it is not the draft
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

</details>
