# sub_0801D390

0x0801D390, 856 bytes, THUMB, parked.

Best score so far: 8.8%, -4 bytes.

## What it does

Runs the command script of one animated object (a 0x4C-byte record in gUnknown_0200E438) until it reaches a wait or an end command. Commands include wait, jump, show a frame, set a field and end. a2 picks the pass: 1 counts down waits without drawing, 0 draws.

## How close it is

Still 4 bytes short (852 against 856) and unchanged this wave. The whole difference is at the top of the loop, where the original copies the 0xFFFFF000 mask through two extra registers before the AND. Both builds use the same amount of stack, so those copies have no variable behind them.

## What is left

Find C that makes the compiler copy the mask constant twice before the AND, as the original does. The same fix should also close sub_0801DCD4, which has exactly this difference.

## Already tried

- Seventeen spellings of the mask and the switch value (operand order, `~0xFFF` or `-0x1000`, the mask or the value in a local inside or outside the loop, `&=` forms, re-reading `*p`, different types for v): all give one direct AND. Writing the mask as `0xFFFFF000` also makes it unsigned, which turns every case comparison unsigned.
- Two chained mask locals, nested inline identity functions, locals pinned to a register, and empty or volatile barriers around them: all compile to the same single AND.
- The permuter for 600 seconds: nothing better.
- A `do { } while` loop with an extra `if (p == NULL) return;` guard (kept in best.c): right size and 63% of bytes matching, but only because the redundant guard adds 4 bytes; it moves the first literal pool and is further from the original.
- Dropping the `end` flag that nothing sets: 6 bytes shorter, so the flag stays.

## Files

- `sub_0801D390.c`: the current draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at 852/856 (-4). The whole residual is the shared loop-top mask routed through two missing copies. Seventeen mask spellings, chained locals, fixed-register qualifiers, barriers, inline identity calls and long permuter searches are ruled out.

### Wave 93

- **result:** no change; 8.76% at -4 bytes, first difference +0x10
- **best_c_NOT_adopted:** The wave-93 brief instructed that best.c (62.97%, size-exact) was 'the only sane base if it is correct C'. It IS correct C -- the honest while loop rewritten as if (p == NULL) return; do { } while (p != NULL); -- but it is not a better base, and the instruction is wrong. The guard is redundant, the loop's own test already covers it, and it compiles to exactly the 4 bytes this function is short. The whole residual stays at +0x10 in both forms and the guarded form additionally moves the first literal pool about 40 bytes, so it is further from the ROM's layout, not closer. This is the brief's own 'a score that rose because the SIZE changed is not progress' case, and wave 56 had already recorded it in the draft's header. The honest while draft is kept.
- **frame_lever_ruled_out:** Wave 93's frame-slot lever (an ordinary local never creates a stack slot, a volatile local always does) was checked here because the function is 4 bytes short and a volatile local costs about that. It does not apply: BOTH frames are sub sp, #24, so the ROM allocates exactly the same number of addressable locals. The two missing copies are register-to-register with no object behind them, and volatile cannot produce them without adding a slot the ROM has not got. No probe spent.
- **residual_reread:** ROM: ldrh r1,[r7] / ldr r4,[pc,#44] / adds r0,r4,#0 / adds r2,r0,#0 / ands r2,r1. Candidate: ldrh r1,[r7] / ldr r2,[pc,#40] / ands r2,r1. Three pseudos for the mask in the ROM (r4 -> r0 -> r2), one in the candidate; ands is destructive, so each copy implies the previous pseudo is still live afterwards. A pseudo-COUNT fact with no frame object and no instruction selection in it, which is why seventeen respellings have all folded to one AND.
- **permuter_REJECTED:** 900 s x 4 threads, the first run from the honest draft, reported IMPROVED 8.76% -> 56.19% at the exact 856 bytes. REJECTED as wrong C. The only semantic change is one read-before-set: new_var = (struct Unk0801D390Bits *)&e->unk30; was bound inside the 0x2000-family arm and then used for the SAME unk00_c test in the 0xF000 / 0xF00 arm, where it is never assigned. The two arms are mutually exclusive cases of one switch, and being inside the while loop does not save it because the first iteration can take 0xF000 directly. Output kept as w93-perm1-5619.c.wrongc.
- **draft_recovery_warning:** work/ is not tracked by git and no copy of the draft had been taken before the run, because the draft was being KEPT rather than replaced -- which is exactly when the brief's 'copy work/<fn>/<fn>.c before any permuter run' gets skipped. The draft was reconstructed by reverting the three edits (declaration, assignment, two uses) and re-measures at 8.76%, size -4, first +0x10, the recorded numbers to the digit.

</details>
