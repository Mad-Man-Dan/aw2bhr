
## wave 97 (W97-L)
Base: previous draft (28.8%, +8, `sub sp,#4` spill of i+1) kept as sub_08026290.w97L-start.c. New source
(sub_08026290.c, = vc.c): bind the address of the current army's CO byte before the retry loop
(`ci = &gPlaySt.co[i]; ... *ci = v;`), inner loop still on bare `gPlaySt.aiControlled[j]` / `gPlaySt.co[j]`.
Result: size-exact (176), no frame, push list identical, prologue and outer-loop shape now the ROM's, but the score
FALLS to 19.3% because the pool words differ (`&gPlaySt.co[i]` folds to a `gPlaySt+0x3d` literal, `ldr r0,=0x3d`, where
the ROM does `adds r6,r2,#0; adds r6,#0x3d; adds r7,r5,r6` from a bare word; and n lands in r7 not r8).
Negatives (measured with spellings.py): binding `ai` at the top of the outer body and using it in the inner loop
(-4, frame 8), binding `co = gPlaySt.co; ci = co + i` (size-exact, frame 8), both bound (-20), binding ai and co
inside the if AFTER the store (-8, frame 4), `ai`+`co`+`ci` all bound in the first spelling (-20).
Next: ROM's r6 = base+0x3d is a real pointer variable (`co`) alive across the inner loop and r7 = &co[i]; a
spelling that keeps `co` a pointer WITHOUT the frame is still needed (the frame comes from i+1 being spilled once
`co` and `ci` are both live).
Proposed summary tried: + "binding &co[i] before the retry loop removes the frame and the +8 but folds the address
into a gPlaySt+0x3d literal".

## wave 97 (W97-V)
Base: levers 5d-73_1cp-57 (`s8 lv0 = i` copy for the aiControlled store, a do{}while(0) around the retry body); wrongc OK (warning is only the `while (0)` literal). 19.32% -> 46.59% size-exact. Permuter run 1 -> 57.95% (`ci = &gPlaySt.co[i]` moved into the retry loop body after the call; value-equal), run 2 nothing. Residual: ROM reloads the gPlaySt pool word inside the outer loop (plain literal) after the early `.rodata` force-addr word (the W95-B split construct); ours hoists both base+56 and base+61. Negative: binding `ai = gPlaySt.aiControlled` at loop top and `co` after the test (the ROM's apparent shape: aiBase in sl, coBase in r6) -> 156-160 bytes (-16..-20), 6-7%: agbcc folds them and drops the held registers.

## wave 97 (W97-Z)
Base unchanged (`sub_08026290.c`, 57.95% size-exact; old draft saved as `sub_08026290.w97z-start.c`). Scratch probes only:
`w97z.c` (pointer binds `g`, `ai`, `co`: 160 bytes, 6-7%), `w97z2.c` (inner-loop temps: 31.8% -4 / 52.3% / 20.2% +12),
`w97z3.c` (alias `gUnknown_03003FC0` at entry / outer head / inner loop, five assignments: 184-192 bytes, 12-37%).
All negative. Mechanism found for the ROM's split (see the last chapter of docs/agbcc-codegen.md): PRE's copy `N = P`
gives the entry word its second use; loop.c pass 2 (26-insn limit) decides whether the `mem/u N` load leaves the inner loop;
N then loses allocation and the surviving use becomes a plain literal. The draft differs in that N wins a register (`sl`)
and the entry base is reused for the hoisted `+0x38`. Untried: enlarge the inner loop past 26 insns at loop time with insns
that vanish later while lowering N's priority. Proposed `tried` addition: "aliasing the global's second name at the entry,
outer loop or inner loop, and pointer binds, do not reproduce the pool split".
Follow-up (W97-Z): `-dL` on the draft: outer loop pass 1 (74 insns) hoists insns 41/44 (the `mem/u N` load and its `+0x38`, `savings 2, life 15`) to the preheader; the ROM re-derives both every outer iteration. Reading: loop.c moves them because `threshold*savings*life >= insns` (26*2*15). To keep them in the loop the pseudo's life must be ~1 insn (26*2*1 = 52 < 74). Not reached from source yet.
