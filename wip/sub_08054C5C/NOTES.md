# sub_08054C5C -- wave 90, W90-A

Start: 92.5%, 560/560, 42 differing bytes, first difference +0x13
(`w90-start.c`). End: **93.4%, 37 differing bytes**, same first difference
(`sub_08054C5C.c` == `w90-perm1-934.c`).

| run | search | iterations | result |
|---|---|---|---|
| perm-w90-1 | undirected, 900 s, from 92.5% | 11,558 | kept 93.4%: `t = sub_08055058(a[side], b[side], c[side], i = d[side ^ 1]);` |
| perm-w90-2 | undirected, 900 s, chained from 93.4% | 16,819 | best permuter score 2300 (base 2420) is 69.3% by bytes, so it was rejected |
| perm-w90-3 | directed (`w90-directed.perm.txt`: PERM_LINESWAP over the declarations + PERM_RANDOMIZE over the setup loop), 900 s | 13,564 | nothing better than 2420 |

The kept change reuses the loop counter `i` (dead at that point) as a temp for
the fourth argument. It preserves the semantics: `i` is reassigned before its
next use. It moves 5 bytes; the W80 residual is otherwise unchanged:

- The setup loop's counter is born in r3 and copied to r7 (`adds r7, r3, #0`)
  where the ROM has it in r6 from the start (`movs r6, #0`). This is the
  wave-78 `t = i` size costume that W80 recorded. Without the copy the
  function is 568/560.
- The third stack address (sp+10, spilled to [sp+40]) is derived as
  `adds r0, #2` off the sp+8 value already in r0. The ROM computes all three
  (sp+12, sp+6, sp+10) from sp, and builds the first one in r3 because its
  counter isn't there.
- The table base goes in r6 and gUnknown_085D6A52 in sb. The ROM has the base
  in r7 and gUnknown_085D6A48 in sb, which is the W80 allocno order.

W83-D's zero-trip do/while promotion and the seven-profile sweep stay ruled
out (see the parked entry).

## wave 97 (W97-G)

Base: `sub_08054C5C.c` (== w90-perm1-934.c), 93.39% size-exact, first diff +0x13. Unchanged.

Pre-registered hypothesis (a shared side/other-side index recomputed by the ROM) did NOT hold: the whole residual is
in the setup loop preheader (nothing after +0x13 differs except register names from the r6/r7 swap), not in the
call groups.

Probes (trymatch):
- Drop the `t = i` copy (index `i` directly) while KEEPING `i = d[side ^ 1]` as the 4th argument: 560/560
  size-exact but 75.5%. Preheader gets `movs rN,#0` for the counter in r7 (ROM r6), keeps `adds r0,#2` for sp+10,
  and the tail argument costs `ldrh r7; adds r3,r7,#0` (ROM loads straight into r3). So the `i =` reuse is what
  makes the no-copy form size-exact, but it adds a copy the ROM does not have.
- No-copy without `i =`: 568/560 (+8), 16%. With `u = d[side^1]` instead of `i =`: 568, 16.4%. Only reuse of the
  dead loop counter keeps the size; a fresh pseudo does not.
Conclusion: unchanged allocno-order residual (counter reg, sp+10 derivation, A48/A52 in r9). No new lever.

Proposed summary: does = builds per-side setup tables for two sides, then runs the setup chain for the active side
and the other side if they differ. status = 93.4% size-exact, all differences in the loop preheader register
assignment. left = counter in r7 not r6, sp+10 derived off sp+8, wrong table base in r9. tried = see above plus
waves 78-90.

## wave 97 (W97-AA)
Base unchanged (93.39%; draft snapshot `sub_08054C5C.w97aa-start.c`). Tried this wave's copy-back step in place of the `t = i` costume:
- `for (i = 0; i < 2; ) { ...use i...; nx = i + 1; i = nx; }` with the `i = d[side ^ 1]` tail reuse: 560 size-exact but 69.29%
  (first +0x13). The counter is a plain u16 with no `t = i` copy, and the setup loop is now the ROM's shape, but the counter lands in r7
  and the table base in r6 (ROM r6 / r7), `&b[1]` is still derived `adds r0,#2` from `&b[0]`, and the tail loads `ldrh r7; adds r3,r7,#0`.
  So the copy-back step gives the size the costume gave, without the costume, and leaves exactly the counter/base allocno swap.
- The same with `t = i` kept (copy-back + costume): 93.39%, byte-identical to the draft. Step at the top (`i = nx` in the for clause): 564 bytes.
- Replacing `i = d[side^1]` in the tail with `nx = ...`, `(t = ...)` or the bare read: 572 / 572 / 568 bytes, 15%: only reuse of the loop
  counter keeps the size, as before.
Proposed left: counter r7/base r6 swap in the setup preheader (allocno order), sp+10 derivation, tail copy.

Permuter (W97-AA, foreground, 500-560 s, 2 threads, from the current draft): NO-IMPROVEMENT.
