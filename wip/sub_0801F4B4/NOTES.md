## wave 95

Base: sub_0801F4B4.w95-start.c (58.6%, size -4). Draft unchanged so far.

Read of the ROM: the 0x08090928 word is a compiler pool word holding &gUnknown_0300409C. r4 holds ITS address (`ldr r4,=gUnknown_08090928`), every ordinary cursor access is `ldr rX,[r4]` then a load through that, and r1 = `adds r1,r4,#0` at the swap-merge and at the inner-loop bottom is a second pseudo `pp` holding the word's address, defined twice (merge + back edge) and used only at the loop head (`ldr r0,[r1]; ldr r0,[r0]; ldrb r0,[r0,#2]`, once in the pre-loop empty test, once as switch discriminant). r1 cannot live across the calls, hence the redefinition at the bottom.

Probe: declared `struct Unk300409C **const gUnknown_08090928;` in the header and wrote `pp = &gUnknown_08090928` at the merge and at the loop bottom, reading `(***pp).unk02` for the empty test and the switch. NEGATIVE: 584 bytes (+12) and the prologue changed (first difference +0x12): naming the word as a real symbol makes the compiler load its address through a second pool word (`mov r7,sl; ldr r1,[r7]` at the bottom) instead of reusing r4. Header edit reverted. The name-the-word form does not reproduce a copy of the existing force-addr register; the pp spelling needs an address VALUE that CSE shares with the force-addr word of the bare global, which only the bare global itself provides.
Not run: permuter chain (queue was full behind sub_0801C01C / sub_0802216C / sub_0801E9B0).
