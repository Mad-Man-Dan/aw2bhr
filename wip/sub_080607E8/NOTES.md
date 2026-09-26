# sub_080607E8 — wave 92 (W92-B)

Draft unchanged at **54.65%, 164 bytes against 172 (-8)**. The residual is now
read off the ROM instruction for instruction, and it is exactly four
instructions. A 900 s permuter run and five hand spellings were measured; the
permuter's "improvement" was rejected as wrong C.

## The residual, in full

    ROM                                   draft
    mov r2, r8                            adds r1, r7, #0   (or mov r1, r9)
    lsls r1, r2, #16                        -- absent --
    asrs r1, r1, #16                        -- absent --
    bl sub_08025CC8                       bl sub_08025CC8
    adds r4, r0, #0                       adds r4, r0, #0
    movs r0, #0                             -- absent --
    strb r0, [r4, #9]                     strb r6, [r4, #9]
    strb r6, [r4, #10]                    strb r6, [r4, #10]

Two defects, and the arithmetic closes exactly:

  * the ROM narrows `b` to 16 bits before passing it as sub_08025CC8's second
    argument, three instructions where the draft copies in one — **+4 bytes**;
  * the ROM materialises a fresh zero for `u->unk09 = 0` where the draft reuses
    the register holding `c`, which the `c == 0` guard proved zero — **+2 bytes**;
  * with those six bytes the code reaches 170, and the literal pool's alignment
    then needs the ROM's `.short 0x0000` — **+2 bytes**, giving 172.

So the missing 8 bytes are fully accounted for and nothing else in the function
differs. Everything after the two defects is byte-exact.

The narrowing is requested: `sub_08025CC8` is prototyped `(s16, s16, s16)`. The
draft's compiler deletes it because `b = p->unk01 + 4` is a zero-extended byte
plus a constant, so combine can prove 23 sign-bit copies. The first argument
`a + i` keeps its narrowing in both, and that is the contrast that names the
mechanism: `i` is a loop counter with no range combine can use, so `a + i` is
unprovable and the narrowing survives. To reproduce the ROM, `b` has to be
unprovable the same way, and no spelling found this wave makes it so.

The last pool word's relocation prints as a difference — original
`gUnknown_030046B4`, candidate `gFactoryUnitSchedule` — and is not one:
`aw2bhr.map` puts gFactoryUnitSchedule at 0x030046b4. Same address, two names.

## Spellings measured this wave (all negative)

    b split into `b = p->unk01; b += 4;` ................  30.23%, size -4
    `u->unk0a` re-read from the map instead of from c ...  16.11%, size +8
    both of those together .............................  17.39%, size +12
    the map cell's address bound to a local, unk0a re-read
      through it, on top of the split ...................  25.57%, size +4
    the two stores swapped in source order .............  byte-identical to the split

**Correcting the parked entry on the split.** It recorded that splitting `b`'s
definition "gets 4 bytes back", which is true of the size and false of the
reason. Compiling it and reading the diff shows the narrowing is *still* absent —
the split only moves `b` from a high register into r7, so the copy becomes
`adds r1, r7, #0` instead of `mov r1, r9`, one instruction either way. The four
bytes come from an unrelated register shuffle earlier in the loop. Splitting `b`
does not reach the mechanism and is not a partial fix; the draft, which keeps
more of the ROM's instruction stream, is the better base despite scoring higher.
Swapping the two stores is byte-neutral, so store order is not a lever either.

## The permuter result was rejected, and why it looked good

`perm-w92-1.log`: 900 s, 4 threads, from the draft. It reported the draft
improved from 54.65% to 80.81% at the ROM's exact size and kept that source.
Reading the mutation: it inserted a second `p = sub_0803E354(7);` **inside the
loop body**. `p` is dead after `a` and `b` are read, so the assignment is dead,
but the call cannot be deleted, and `movs r0, #7 / bl sub_0803E354` is 8 bytes —
precisely the deficit. Every instruction after it then lands on the ROM's
address and the score jumps.

The ROM's loop body contains no such call. The candidate would call
sub_0803E354 four extra times per invocation, so it is not merely unlikely
source, it is a behaviour change. Rejected, draft restored, and `best.c` /
`best.json` reset (they had kept the 80.81% fossil).

This is the second time in this batch that a large score gain was pure size
realignment — the other was `-fno-force-mem` on sub_08061DCC. **Both fooled an
automated ranker in the same way: on a function that is short of the ROM's size,
anything that adds the missing number of bytes anywhere realigns the whole tail
and multiplies the percentage.** The permuter's objective and the flag sweep's
ranking are both vulnerable to it. A candidate that gains size should be checked
for *which* instructions it added before its score is believed.
