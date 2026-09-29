# sub_0807F434

## wave 97

Base: the old draft (17.74%, 248 B, +8; kept as sub_0807F434.w97-start.c). Now **240 B size-exact, 91.67%**, first diff +0xF
(three register renumberings, nothing structural). Final source is work/sub_0807F434/sub_0807F434.c.

What moved it (each measured, in order):
1. `i + 1` in the second loop's call argument respelled `-~i` (17.74% +8 -> 85.42% size-exact). cse shared the `i+1` with the
   loop step and computed it at the top of the body, which took the register for a hoisted `.rodata` address word; `-~i` is
   not seen equal by cse but combine folds it to `adds r2, r5, #1` after the call, as the ROM has it. `(i + 2) - 1`,
   `(i * 2 + 2) / 2`, `(i + 3) - 2`, `i - (-1)` all fold in a pass before cse and stay 17.74%. `(s16)i + 1` / `(u8)i + 1`
   work on sharing but add an `lsls/asrs` pair (+4). A plain or s16 local copy `tn = i` did not help (+8 / +16).
2. Binding `p` no longer inside the Decompress argument: `Decompress(gUnknown_08234B10, gUnknown_0200FC50);` then loops that
   name gUnknown_0200FC50 directly (85.42 -> 87.92 ; with `for (j = 0, p = ...` 89.17).
3. The copy loop in the sibling sub_0807E980's spelling (`for (k = 0, nv = 0; ...) { x = 0x06015000 + j*0x800 + nv;
   CpuFastSet(&gUnknown_0200FC50[j*0x100 + k*0x400], (void *)x, 0x40); nv += 0x100; }`) -> 91.67%.

Residual (all register names, no size change): the ROM keeps the Decompress source word in r7 and the buffer word in r6;
we get them swapped (r6/r7). The outer counter `j` is in r5 in the ROM and in r2 for us, and the source/dest add order in
the loop preheader differs. Spellings of the Decompress buffer argument (`p` bound before, `&buf[0]`, `(void *)`, `+ 0`)
are byte-identical at 91.67%, and a separate `src = p + j*0x100` bind or an inline dest expression did not change it.

Proposed summary: does = as before; status = "size-exact (240), 92% of bytes; only register numbering differs";
left = "which of two hoisted address words gets r6 vs r7 and the outer counter's register"; tried = the above.

Update (end of wave 97): final draft is **94.17%, 240 B size-exact**. Permuter runs: 91.67 -> 93.33 (`new_var = &i` in the first
loop, un-shares `i * 12` -- valid C, no frame change) -> 93.75 (`j = 0; proc->unk4c = j;`) and the destination written inline
in the CpuFastSet call -> 94.17; a third run found nothing. Remaining 14 bytes: the two shifts `j<<8` / `j<<11` are in the
opposite order and `ldr r0,=0x06015000; adds r4,r1,r0` comes before `movs r6,#7` where the ROM has it after; final zero
`movs r4,#0` is in r5 for us. Reordering the operands of the src/dest index sums does not move them.
