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

## Wave 94 (W94-A) - both imported drafts rejected, draft unchanged at 54.65%

**Vesly's two files are wrong C and are quarantined** as
`vesly-best.c.wrongc` and `vesly-sub_080607E8.c.wrongc`. The defect is not the
wave-92 duplicated call (there is none): it is `(char) b` as the second
argument of `sub_08025CC8`. `b = p->unk01 + 4` ranges 4..259, so the cast
truncates it; agbcc's `char` is unsigned here, so the cast compiles to
`lsl #0x18 / lsr #0x18` where the ROM has `lsls #0x10 / asrs #0x10`. The file
is size-exact only because three errors cancel: the truncation adds an
instruction pair, an extra `add r0, r2, #0` adds one, and the missing
`movs r0, #0` for `unk09` removes one.

Three axes re-tested on the current (wave-77 goto-loop) base, since all three
were originally measured on the pre-wave-77 base. All still negative:

- `b = p->unk01; b += 4;` - the dead first set is eliminated, so `b` is still
  a single-set pseudo and the narrowing still folds; it also moves the loop
  counter into r8. Worse.
- `u16 b` - byte-identical.
- `(s16) b` written as an explicit cast at the call - byte-identical.

**That last one bounds the wave-15 rule.** "An explicit narrowing cast expands
to a shift pair that survives where the implicit conversion folds" holds only
where the narrowing is NOT provably redundant. `b` is a single-set pseudo with
`nonzero_bits <= 0x1ff`, so combine folds a 16-bit sign extension of it
whatever the source spelling is. For the ROM's shifts to survive, `b` must be
a pseudo whose range agbcc cannot bound - not a cast.

Permuter run 1 from the 54.65% base reported 79.65% size-exact and the form is
WRONG C, quarantined as `w94-perm1-7965.c.wrongc`. It inserts
`band = c; i = band;` before `u->unk0a = i;`. The stored value is right (`c` is
0 in that branch) but it clobbers `band`, the loop-invariant schedule row, and
resets the loop counter `i` to 0 - a unit created at column 1 makes the loop
repeat column 1 for ever. No uninitialised-read check can see this.

What that form does establish, and it is worth having: the right size (172) is
reachable from this draft by adding two register copies inside the loop body,
which agrees with the recorded three-instruction residual.

### Residual

164/172 (-8), 54.65%, first difference +0xa - the mask shuffle's high register
(ROM `sb`, candidate `r8`), which follows from the missing instructions in the
`sub_08025CC8` call block rather than being independent.

## wave 97 (W97-G)

Base: draft (54.65%, 164/172). `best.c` (79.65%) is WRONG C and stays rejected: it writes `band = c; i = band;`
inside the loop body, so `band` (loop-invariant, used for `band * 3 + i` next iteration) and the loop counter `i`
are both clobbered with the known-zero `c`. Its size-exact score comes from that clobber, not a valid twin.

Residual re-read: (1) the ROM keeps `lsls #16; asrs #16` on `b` at the sub_08025CC8 call; the draft drops it
(b = u8 field + 4, so nonzero_bits proves it fits). (2) `movs r0,#0; strb r0,[r4,#9]` vs draft `strb r6,[r4,#9]`
(cse substitutes the known-zero `c`).

Probes (trymatch, all restore to the draft): `b = p->unk01; b += 4;` -> 168 (-4) but 30% (i moves to r8, ext still
folded); `(s8)` on the field -> 168; `*(u8 *)((u8 *)p + 1) + 4`, `(u16)(...)`, `(u32)p->unk01 + 4`: byte-identical
to the draft (164). Two-set b does not restore the extension either, so the fold is not reg_n_sets-gated here.
Unresolved: what makes the ROM's `b` non-provable (value not from a ldrb+4 chain visible to nonzero_bits).

Proposed summary: does = spawns up to three factory units in a row at the map slot's row from the factory schedule.
status = 54.65% draft, 8 bytes short. left = missing sign extension on the y argument, `unk09 = 0` uses the zero
register instead of a literal. tried = see above plus waves 92/94; best.c is wrong C (clobbers band and i).

## wave 97 (W97-Y)
Base: draft (54.65%, -8). levers.py's 81.4% (`s8` copies of d at both uses) is WRONG: the third argument of sub_08025CC8 becomes 0xFFFFFF8B for d >= 0x80 (wrongc, confirmed by reading); no other lever beat the draft.
Probes for the missing sign extension on b (`s16 bs = b` at the call, `s16 b`, `u16 b`, `(s16)(...)` on the assignment or at the call): all 164 bytes, the extension stays folded (nonzero_bits proves b fits); `s16 b` plus a separate `s16 bs; bs = b;` gives 168 at 30.8%. So the fold is not a copy-count or type issue on b.
Permuter (3 links, 500 s each): the kept "improvements" (63.4%, 65.7%, size 172) are PADDING, not progress: `new_var = i <= 2; if (new_var) goto` and a do { } while (0) around the loop make agbcc emit `movs r0,#0 / movs r0,#1 / cmp r0,#0 / bne` at the loop end (visible in the diff), replacing the ROM's `ble`. Rejected; draft restored. A size-exact score on this function is a padding artefact until the tail `ble` is reproduced.
Residual unchanged from W97-G: b's `lsls/asrs` before the call, `movs r0,#0` for unk09, and the sb/r9 band allocation.
