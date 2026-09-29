# sub_08039588

## Wave 93 (W93-D) -- the hoist ORDER is solved; only the constant merge is left

Reading the table entry into a local as the FIRST statement of the search loop
body fixes the preheader ORDER, which was half of residual (a):

    for (k = 0; gUnknown_08090F30[k] != 0; k++) {
        c = gUnknown_08090F30[k];
        dst = j * 0x100 + 0x6140;
        if (str[i] == c) { ... break; }
    }

With `c` first the preheader comes out `ldr r3, =gUnknown_08090F30` THEN the
`j << 8` computation -- the ROM's order. Without it the two are reversed. The
leading read costs nothing, because the ROM loads tbl[k] into a register there
anyway. GENERAL LEVER, worth reusing: LICM emits its hoists in the order the
invariants appear in the loop body, so a leading reference to the value you
want hoisted FIRST puts it first.

What is left is only the constant merge: the preheader gets `lsl r4, r4, #8`
and the `+ 0x6140` is folded into the use as a single `=0x6016140` pool word.
164 bytes (-8) at 43.0%.

`-fno-cse-follow-jumps` does NOT prevent the merge (measured on this spelling:
43.0%, -8, unchanged). That is expected in hindsight: gcc lays the if-body out
as the FALL-THROUGH of the inverted compare, so the def and the use sit on one
cse path without any jump being followed. Breaking it needs a JOIN -- a label
with two or more predecessors -- between the def and the use inside the loop,
and no C construct that survives the `jump` pass creates one here.

The impasse is now exactly stated:
  * the def must be INSIDE the inner loop and BEFORE any conditional branch in
    the body, or LICM will not hoist it. Everything after the `if` is
    `maybe_never`, which is why the bottom-of-body spelling keeps both
    constants but never moves.
  * the def must be OUT of the use's fall-through path, or cse reassociates
    0x6140 with 0x06010000.
Every position in the body satisfies exactly one of the two.

Draft unchanged: size-exact 172 bytes, 87.2%.

## wave 96

Base: `sub_08039588.c` (87.2%, size-exact, first diff +0x17); confirmed the parked residual (ROM hoists `dst` after the
zero-trip guard and table base; draft computes it before the guard, j/dst in r3/r4 instead of the shared r4).
Pre-registration (same LICM first-use family as sub_08037A78) NOT confirmed: the def has to be inside the inner loop to
hoist, and every form that puts it there merges the constants (-8). Probed: fold-proof mask on j (`((u32)j<<16 &
0xffff0000)>>16`) in the def, in the use inline, and in the reordered-constant use: 40.7% / 43.0% (-8), the mask does not
split cse's merge of `0x06010000 + dst` because j is re-derived (not a held narrow operand) here; def in the `for`
condition as `k=0; a[k]!=0 && (dst=..,1)`: 6.4%; `(dst=..., a[k]!=0)`: 39.7% +12; def in the increment clause: 19.8%. No match.
Residual unchanged: the def cannot be both out of the use's EBB (pool words) and an inner-loop invariant (hoist).

## wave 97 (W97-U)

Base: `sub_08039588.c` (87.21%, size-exact, first diff +0x17), unchanged. Pre-registered hypothesis (a constant merged
across the loop is a lever-1/lever-5 case) NOT confirmed. Probed with spellings.py (all 43.02% -8 unless noted):
`dst` removed and the address written inline in the call as `(u8 *)(j*0x100 + 0x6140) + 0x06010000`,
`(u32)(...) + 0x06010000`, `0x06010000 + (u32)(...)`, `(j<<8)` form, `(u8*)0x06010000 + (...)`: all fold back to the one
`=0x6016140` word (cse folds `(x + C1) + C2` however the cast is placed). `dst` typed u32 / `(j<<8)`: same. `c = tbl[k]`
first then `dst = ..` in the inner loop: same. Copy-back step `nj = j + 1; ... j = nj;` (before dst / after dst / at the
top of the outer body): 35.2% / 36.4% / 24.4% at +4 (adds the copy the ROM has but moves the guard). `nj` before the inner
loop with the address inline: 51.7% -4 (best of the new probes, still short). A `vram = (u8*)0x06010000` local used as
`vram + dst`: 75.0% size-exact, first diff +0xC (the base gets held, worse than the draft).
Mechanism note: the ROM keeps `j+1` (r6) computed before the zero-trip guard and the `dst` sum after it, i.e. both are
loop.c hoists, and the two constants stay separate words. That needs the sum's def inside the loop AND out of the use's
cse path; no spelling tried does both.
Proposed summary tried: "inline / casted / u32 spellings of the VRAM address all fold to one constant word; copy-back
step for j moves the guard but not the hoist".

## wave 97 (W97-AA)
Base unchanged (87.21%). Checked the twin lead: the ROM pool here has only three words (gUnknown_08090F30, 0x00006140, 0x06010000);
neither 0x6140 nor 0x06010000 is a neighbour symbol (no asm/ symbol at 0x0601xxxx, VRAM is not a linked object), so the sub_08073228
trick (name a second symbol at offset 0) has nothing to name. Re-read the diff: the whole residual is the ROM computing
`j*0x100 + 0x6140` AFTER the zero-trip guard and reusing j's register (r4) for it (j+1 kept in r6 across), while ours computes it before the
guard into r4 with j in r3. No new probes beyond W97-U's list.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.
