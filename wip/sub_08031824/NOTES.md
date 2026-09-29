# sub_08031824 — working notes

Size-exact (292 bytes), 8 bytes differ, first difference at +0x14. The whole
instruction stream is the original's; three temporary-register picks differ.

## Where the three differences are

| site | original | draft |
| --- | --- | --- |
| first copy loop, reading the record pointer out of its high register | `mov r1, r8` | `mov r0, r8` |
| binding the address of `gUnknown_020280C0` | `ldr r1, [pc, #84]` | `ldr r0, [pc, #84]` |
| set-up of the 5-byte copy | `ldr r3, ...` (twice) | `ldr r1, ...` (twice) |

At each site a value that lives in a high register has to pass through a low
one for a single instruction. The original picks a higher low register than
the draft does, which means some other value was still in use there.

## Wave 92 (W92-C)

Measured with the compiler's own register-allocation dump (`tools/rtldump.py
sub_08031824 --flags=-dg`, written to `work/sub_08031824/rtl-w92/`):

- The dump prints `Spilling for insn 11. / Spilling reg 0.` and the same for
  insn 26, insn 64 and insn 167 — the four low registers the three sites use.
  So the picks are made when the compiler hands out **scratch registers**, by
  the cost of taking each one away from whatever is living in it, not by the
  earlier allocation pass. Raising the cost of taking r0 at those points is the
  only lever: some value must be living in r0 and still be needed afterwards.
- **Separate counters for the three header loops** (`i`, `i2`, `i3` in place of
  one reused `i`): byte-identical, 8 bytes, 97.3%, same first difference. The
  counters are coalesced back into one, so this is not a way to add pressure.
- **Binding `gUnknown_020280C0` before `j = 0` instead of after**: worse, 12
  bytes differ (95.9%). Confirms the existing note that the bind has to follow
  the counter's initialisation.
- Binding the first loop's destination to a pointer local does not compile:
  the member is volatile, so `&p->unk00[i]` cannot be assigned to a `u8 *`.

Still open, and unchanged from wave 90: find the value the original kept in r0
across the first copy loop and the `gUnknown_020280C0` bind, and in r0/r1/r2
across the 5-byte copy set-up.

## wave 97 (W97-M)
Base: the 97.26% draft, unchanged. No improvement. Hypotheses (pre-registered: the r0/r1 scratch pick is moved by respelling how the 5-byte copy's two addresses
or the `c` bind are formed) did not hold. Measured with tools/spellings.py (size 292 every time, first diff always +0x14):
- 5-byte copy: `i = 4` before/after the src/dst binds 93.8/97.3%; dst before src 95.2%; `(int)gUnknown_020280D4 + j*0x1c` order 95.6%;
  shared `off2` 92.8% (first diff moves to +0x67, worse); ascending `for (i = 0; i < 5; i++)` 97.26% (byte-identical); pointer arithmetic on u8* 95.6%.
- `c = gUnknown_020280C0` bind: before `j = 0` 97.26%; `&gUnknown_020280C0[0]` 95.9%; cast, a dead `d5 = 0`, a trailing `i = 0`, a dead `v2 = 0` before it: all byte-identical 97.26%.
So the scratch pick is insensitive to every statement-order/spelling around it; the wave-92 finding (the pick is made in reload's scratch choice) stands.
Proposed summary: unchanged (status 97.26% size-exact; left: three scratch-register picks r0/r1 vs r1/r3).

## wave 97 (W97-PG)
Permuter chain: 1 link, 97.26% -> 97.26%, NO-IMPROVEMENT.
