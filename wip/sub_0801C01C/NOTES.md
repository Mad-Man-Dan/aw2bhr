## wave 95

Base: sub_0801C01C.w95-start.c (27.6%, size -8). Best sound draft now sub_0801C01C.c = permuter run 1 output (70.69%, size exact, first difference +0x10); saved as sub_0801C01C.w95-perm1-out.c.

What moved it:
- Hand: computing the `(u16)pair` high half as its own variable and reading a4.unk04 (z) before x/y matched the ROM's order of temporaries (27.6 -> 28.4%), still -8.
- Permuter run 1 (900 s, from the hand draft): 28.4 -> 70.69%, size 116 exact. Read: the kept form splits the a4.unk00 load, the mask constant, the pair-to-long-long copy and the `<< 6 << 10` shift into extra pseudos (new_var*); statements are equivalent C (mask BEFORE the >> 16, same z/x/y).
- Permuter run 2: 70.69 -> 78.45% but WRONG C: it swapped `pair >>= 16; pair &= mask` to shift first, then mask with the sign-extended int constant, which zeroes the high half x should carry. Discarded (kept as sub_0801C01C.w95-perm2-out-WRONG.c). Not chained further.

Negatives: `int w = a4.unk04; ... (u16)w` sinks the load to the call (z must be computed before x/y); `u32` pre-shifted hi/lo temporaries and a `t` intermediate for z fold to the same code.
Parameter-width question: callers pass s16 values (c_08022DD4.c) and sub_08022BB8 matched with u16 first two parameters left alone, so int-vs-u16 was not needed.
Residual: register choice; ROM keeps lo<<16 in sb and the pair in r5/r6, our build uses low registers.
Proposed summary: does = as before; status = "116 bytes, size exact, 70.7%; only register allocation differs"; left = "ROM parks lo<<16 in r9 and the pair halves in r5/r6"; tried = above plus the wrong shift-first form.

## wave 97

Base: `sub_0801C01C.c` (70.69%, size-exact, first difference +0x10). `best.c` (78.45%) was NOT adopted: it is the wave-95 run-2 form (`pair >>= 16` before `pair &= mask`), which zeroes the high half `x` needs. Checked by re-inserting the mask-first order into best.c's other changes: the result scores 70.69% again, so best.c's whole gain came from the wrong shift-first order and nothing in it is a valid twin.

Moved: permuter run 1 (900 s x 2 threads) 70.69 -> **75.00%**, size-exact. The change is one statement pair: `new_var = (((u32) new_var2) << 6) << 10; new_var3 = (((u32) (a1 & 0x1FF)) << 16) | new_var;`. That is the pre-shifted low word as a separate statement, and it REUSES `new_var` (the already-declared mask variable) as the scratch. Audited: `new_var` holds the mask only before `pair &= new_var`, so the reuse is value-preserving. The lever is pseudo sharing: a fresh `u32 new_var5` for the same statement (my hand probe) changed nothing, the shared int scratch did. Run 2 from the 75.00% file: NO-IMPROVEMENT.

Residual: register choice only. The ROM copies a2 to r5 and keeps the pair halves in r5/r6, with `lo << 16` parked in sb; ours parks the zero high half in r9 and uses r4/r2. First difference still +0x10.

Proposed summary: does = as before; status = "116 bytes, size exact, 75.0%; only register allocation differs"; left = "the original keeps the zero high half and pair in r5/r6 and lo<<16 in r9"; tried = the shared-scratch statement split, the shift-first form (wrong C, high half lost), separate scratch variable.
