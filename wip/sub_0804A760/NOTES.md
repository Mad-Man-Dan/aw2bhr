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
