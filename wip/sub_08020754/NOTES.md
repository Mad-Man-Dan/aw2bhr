# sub_08020754 -- wave 91 (W91-A)

Draft unchanged: 220 of 208 bytes (+12), 27.9%, first difference +0xc.

## The pre-registered member-form test: NEGATIVE (byte-identical)

This draft was ALREADY a member form -- through its own local
`struct Unk20754Map` (width/height/unit at 0x51A/cell at 0x1432/rowOffset), not
raw byte offsets. Respelled through the shared `struct Map`
(`((struct Map *)gUnknown_08499590)->terrain[...]`, `->unitUnk[...]`,
`->rowOffset[y]`) and again through `gMap`: both compile to a `_cand.bin`
`cmp`-identical to the wave-90 draft. Nothing to gain on this axis.

## What the -da dumps say the residual is (new this wave)

`python tools/rtldump.py sub_08020754 --flags="-da -dp"` on the struct-Map
spelling. In `dump.cse`, the `= 0` arm (code_label 234) already uses reg 47
(the loop-top `rowOffset[y] + x`), and the `= 1` arm (code_label 260)
recomputes from a fresh load of gUnknown_08499590. The `= 0` arm is NOT the
fall-through of anything CSE can walk through -- it starts with a label -- so
CSE reached it by `-fcse-skip-blocks`: the `unit == 0` jump branches AROUND the
unit-type block (which has no other entry) to a single-use label, and
cse_end_of_basic_block follows such a jump (`AROUND`), invalidating only what
the skipped block sets. The `= 1` label has two uses (the 0x13 test and the
range test), so no path can follow into it.

The ROM is the mirror image: `= 1` reuses r2, `= 0` recomputes with a reload
of the global. So the ROM's source must, at CSE time, (a) deny the skip-blocks
path into the `= 0` arm (its label had a second use, or the type-check block
had a second entry) AND (b) give the `= 1` label one use preceded by a barrier.
The park's older text ("CSE stays on the fall-through, so the `= 0` arm keeps
the value") names the wrong mechanism: it is skip_blocks, not fall-through.

## Measured this wave (all against the struct-Map draft)

- k1: negated tests each `goto one;`, then `= 0; continue; one: = 1;` --
  byte-identical to the draft (27.9%, +12).
- k2: nested `if (terrain test) { if (unit == 0) goto zero; if (type test)
  { zero: = 0; continue; } } = 1;` -- byte-identical.
- k4: `... && !(unit != 0 && (u8)(type - 0x10) <= 4)` -- byte-identical.
- k3: `if (cond) goto zero; = 1; continue; zero: = 0;` -- +8, 45.7%, but it
  puts the `= 1` arm first, recomputes there, and grows the frame to
  `sub sp,#8`. Wrong direction.


# Wave 93 (W93-C) -- best.c is the already-rejected k3 shape

Draft unchanged at 220/208 (+12), 26.36%, first difference +0xc.
`best.c` renamed `best.c.wrongc`.

best.c is, statement for statement:

    if (cond) goto zero;
    dst[...] = 1;
    continue;
  zero:
    dst[...] = 0;

which this file's own "Measured this wave" section already records as k3:
"+8, 45.7%, but it puts the `= 1` arm first, recomputes there, and grows the
frame to `sub sp,#8`. Wrong direction." Measured again this wave: 43.98%,
size+8, first difference +0xa against the draft's +0xc, so it diverges
EARLIER. It is correct C and it is going the wrong way; its higher
percentage is entirely the 4 bytes it gives back on a draft that is already
over size.

The residual is unchanged: which arm CSE recomputes `rowOffset[y] + x` on.

## wave 97 (W97-L)
Base unchanged (26.36%, +12). Lever 1 (respell ONE identical expression) applied to the "= 0" arm's index, six
spellings (spellings.py): `dst[x + row]`, `*(dst + x + row)`, `(u16)(row + x)`, `row - ~x + 1` all 26.36% +12 (the
compiler canonicalises them back to the same expression, no new value number); `*(volatile u16 *)&row[y]` 24.54% +8;
`*(volatile u32 *)&gUnknown_08499590` as the map pointer in that arm 28.77% +4 but it makes BOTH arms recompute
(reload of the map pointer, row table and an extra literal word) -- wrong direction (ROM: "= 0" arm recomputes,
"= 1" arm reuses the loop-top sum). Mechanism unchanged from the wave-41/87 reading: the ROM's "= 0" block begins
at a label (the `unit == 0` branch target), so cse starts a fresh block there and re-derives the address; the
"= 1" arm stays inside the condition's block. A value-numbering respelling cannot create that; only a block
structure change can, and the jump-pass merge of duplicated `= 1` stores blocks the nested form.
Proposed summary tried: + "respelling the 0-arm index (4 pure commutations, a volatile row read, a volatile map
pointer read) does not split it".

## wave 97 (W97-V)
Base: levers 4a-31 (branch polarity: the `dst = 0` arm is the else of the negated condition). wrongc OK. 26.36% +12 -> 43.98% +8. Permuter run 1 only 44.23% via a macro-expanded `new_var = rowOffset` (rejected: unreadable, +0.25%); draft left at the 43.98% levers form. Residual: +8 bytes. ROM keeps dst in r5 and the gMap pointer in sl (one `mov sl, r0`) with only one stack slot; ours keeps dst in sl and spills the map pointer to [sp,#0], so one more live value than the ROM.
