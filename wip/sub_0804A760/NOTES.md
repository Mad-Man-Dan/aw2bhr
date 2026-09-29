# sub_0804A760

## wave 96

Base: the wave-80 draft with the register pins (`sub_0804A760.w96-start.c`, 59.09%, size +4, first diff +0xae). Now `sub_0804A760.c` = `sub_0804A760.w96-gp2.c`: **920/920, 73 of 920 bytes differ, 92.1%, first diff +0xd8.**

What moved it (59.1% -> 92.1%, size +4 -> exact): the ROM reloads the pointer global through the compiler's `.rodata` cell IN EACH ARM of the key `do { } while` loop (`ldr r0,[r7]; ldr r1,[r0]`), while the draft hoisted the load out of the loop (a fresh `ldr r5,=gUnknown_030044E0` literal, the old +4). Binding the cell's address to a local, `struct Unk030044E0 **gp = &gUnknown_030044E0;`, and RE-ASSIGNING it (`gp = &gUnknown_030044E0;`) at the top of each of the increment and decrement arms, then reading the increment/decrement arms through `(*gp)`, keeps the loop from hoisting the chase (loop.c sees a different pseudo set in every arm). Binding once before the loop is the wrong half: it still hoists (57.3%, size -4, and `gp` declared at the top of the function moves the prologue).

Mechanism: this is the pre-registered early-cell / late-pool split from the link-cable and force-addr blocks seen from the LOOP side. The cell load is a loop-invariant memory read; move_movables hoists it when one pseudo carries it across several arms (savings >= 2). A separate address bind per arm gives each arm its own single-use pseudo (savings 1, below the threshold for a loop that contains a call), which is the ROM's per-arm reload.

Negatives:
- Testing `unk1e == 0` through `(*gp)` in the else arm (either as `else if` or as a nested if after `gp = ...`) brings the hoist BACK (+4, 67.2%): the test and the decrement then share one pseudo inside the arm. A second pointer `gq` for the else arm is no better (59.1%).
- The current draft tests `gUnknown_030044E0->unk1e == 0` with the struct's u8 member (the ROM tests the s16 view with `ldrsh` after a shared `ldrh`). This is a semantic no-op for the range 0..14 but is NOT the original's read; it is the price of avoiding the hoist so far. Open.

Left (diff against the ROM, 73 bytes):
1. the decrement arm test above (0x24a..), plus the after-loop `ldr r5` copy (`adds r4,r5,#0` in ours, ROM keeps r5) which follows from it.
2. `adds r0,r6,r0` (ROM) vs `adds r6,r6,r0` (ours) at 0x112: the pinned `t` (asm("r6")). The unpinned draft is worse; the pin is a costume.
3. the `unk66` hunk at 0x158: ROM `movs r4,#0` placement and r0/r3 assignment for `c` (pin on r3).
4. `movs r0,r0` vs `movs r4,r0` at 0xd8.
Permuter: see below.

Proposed summary:
- does: on-screen keyboard input handler; moves the cursor with the d-pad, wraps rows and columns, jumps to a key on shoulder buttons and plays the click sound
- status: 92% at exact size; first difference at +0xd8
- left: the decrement arm compares the cursor column as a signed halfword after a shared load; the register pins on t and c hide two allocation choices
- tried: per-arm re-bind of the address cell in the key loop (the lever), once-only bind (hoists), bind at function top (moves the prologue), second pointer for the else arm

## wave 97 (W97-Q)

Base: `sub_0804A760.w97-start.c` (= w96-gp2, 92.07%, size exact, first diff +0xd8). `sub_0804A760.c` is restored to it (score-best); the better-shaped candidate is `w97-q.c` (67.97%, size +4, but only ~4 sites differ; the score is low only because +4 shifts every later byte).

Moved: the decrement-arm test. Writing the else arm as its own block with an if/else STATEMENT read entirely through `(*gp)` (`gp = &gUnknown_030044E0; if (V(*gp)->unk1e == 0) V(*gp)->unk1e = 0xe; else V(*gp)->unk1e = V(*gp)->unk1e - 1;`, w97-a.c) reproduces the ROM arm byte for byte (`ldrh r2` / `ldrsh` / `bne` / `movs #14` / `subs r0,r2,#1`), no hoist. The w96 note "test through (*gp) brings the hoist back" is not what the +4 is: the +4 is the post-loop copy `adds r4,r5,#0` (below) plus 2 bytes of pool padding. The ternary spelling (w97-b) gets the arm order wrong (beq/subs first), as wave 80 said.

Second: the unk66 arm used the pinned `c` (r3). Give it its own local (`k`: `k = unk66 + 1; k &= 0xff; k |= 0x80;`, w97-q.c) and the arm matches the ROM except the zero constant sits in r1 where the ROM has r4.

Left in w97-q.c (all else identical): (1) `adds r6,r6,r0` vs ROM `adds r0,r6,r0` (t pin); (2) zero for unk67 in r1 vs ROM r4; (3) after the key loop the ROM keeps the reloaded cell value in r5 for the whole post-loop switch, ours makes a second pseudo `adds r4,r5,#0` for the three cases 0x40/0x24/0x25 (case 0x23 keeps r5): this is the +4. Removing the pin on u, c or t individually does not remove the copy (w97-m,o,r). Cases stored through `(*gp)` (w97-k, frame +4), gp re-assigned after the loop (w97-l, 62%), while-cond through gp (w97-w/x/y) all worse.
Unpinning c alone gives size-exact 920 at 68.8% (w97-t): the tail then differs (`lsls #5` vs `lsls #21; lsrs #16`), so c's pin is needed for the tail.

Proposed summary:
- does: on-screen keyboard input handler (d-pad cursor, row/column wrap, jump keys, click sound)
- status: 92% at exact size in the committed draft; a shape-correct variant (w97-q.c) has only three residual sites but is 4 bytes long
- left: after the key loop the ROM reuses one register for the cell value in all four switch cases; ours copies it for three; also t pin and a zero constant register
- tried: decrement arm as if/else through the pointer (fixes the arm), separate local for the unk66 arm (fixes that hunk), unpinning u/c/t, routing later reads through the pointer local
