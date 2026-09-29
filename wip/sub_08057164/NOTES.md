# sub_08057164 — wave 93 (W93-E)

Draft unchanged as the base: MISMATCH 20.90%, size+0 (268/268), first
difference at +0xe.

## Two spellings measured and rejected (no `try_match` spent)

Both were scored with `tools/drafts.py bases`, which compiles every `.c` in the
folder, so neither touched the draft.

- `w93-literal.c` — no binding locals at all: `gUnknown_0855203C[a * 10 +
  y * 5 + i]` and the other two tables written out in full, no `tbl`, no `idx`.
  **21.32%, size +4 (272), first difference +0xa.** Four bytes too long and the
  divergence starts EARLIER than the draft's. The percentage rose only because
  the extra bytes moved the tail; it is not progress.
- `w93-idxonly.c` — one element counter `k = a * 10 + y * 5` set before the
  loop, read by all three table arms and stepped `k++` at the bottom of the
  body, with `tbl` removed. **21.27%, size+0, first difference +0xa.**

The second is the shape the ROM reads like (one element index stepping by 1,
each table scaling it at the use), and it is still worse by the only measure
that counts here. `drafts.py bases` names it the base on percentage alone and
prints its own caution about the earlier first difference; the caution is
right. **Do not adopt either file.**

So the park's "left" line is now bounded on both sides: binding the table
pointer forms the address GIV, and *removing every binding local* does not stop
it either. The GIV is formed off the bare `SYMBOL_REF` base just as readily as
off a pointer pseudo, which means no arrangement of the index expression alone
reaches it.

## Permuter

Never run before this wave. See `perm-w93-*.log` in this folder.

## Wave 94 (W94-B): first permuter run. One "improvement", and it is wrong C

`--current`, 900 s, 4 threads, under the length-penalised scorer. The run
reported an improvement and kept it: **20.90% -> 30.22%, size-exact**. It is
rejected, and the file is kept as `w94b-perm1-idx-clobber.c.wrongc`.

What it did: it split `idx = a * 10 + y * 5;` into `idx = 10; idx = (a * idx)
+ (y * 5);` (harmless), and then, inside the loop body, **reassigned the same
variable**:

    idx = a * 5;
    e[i].unk00 = gUnknown_085521DC[((y * 55) + idx) + i];

`idx` is the index the three `unk1a` table reads use at the TOP of the body. So
on the first pass it is `a * 10 + y * 5` as intended, and on passes 2 to 5 it is
`a * 5`: four of the five entries read the wrong element of gUnknown_0855203C /
gUnknown_08551F60 / gUnknown_08551E84. Every read is of a variable that was
assigned, so `-Wuninitialized` cannot see it, and the length is unchanged, so
the size guard cannot either. Only reading the body catches this one.

**A screen that does work on this function:** the draft's first difference is
at +0xe, and this candidate's is at **+0xa** -- earlier -- while its score rose.
The wave-93 negatives (`w93-literal.c`, `w93-idxonly.c`) have the same
signature: higher percentage, first difference +0xa. On this function a
candidate that moves the first difference from +0xe to +0xa is reorganising the
loop's opening rather than fixing it, and that is where the wrong-C forms live.

Draft restored and re-verified: 268/268, 20.90%, first difference +0xe.

## wave 96
Base: draft unchanged (20.90%, size+0, first diff +0xe). One 900 s permuter run (2 threads): reported IMPROVED 20.90 -> 26.87%, first diff +0xa (earlier), and it is WRONG C: it introduced `idx = 0; e[i].unk02 = idx; e[i].unk06 = idx;` at the bottom of the body, which clobbers `idx` (the per-side table index) for passes 2-5 of the loop. Kept as `sub_08057164.w96-perm1-idx0-clobber.c.wrongc`; draft restored. Same signature as the wave-94 rejection (`idx` reassigned inside the loop, first diff moving earlier).
Reading the ROM again: the ROM keeps `tbl` (0855203C) in sl as a register and steps the element index `idx + i` as its own +1 walker (r7) with an explicit `lsls #1; add sl` at each of its three uses, and `e` as a +0x24 walker (r5), `i` in sb, `b` in ip, `y` in r6, `&gUnknown_03004582[y]` spilled at [sp] (hoisted row address), `y*5` at [sp+4]. The draft folds `tbl + (idx+i)*2` into one address walker (steps by 2) — the GIV noted in wave 93. Also tried `for (...; i++, idx++)` with `tbl[idx]` reads: 20.2%, +4 bytes, first diff +0xc. No lever found to keep the idx+i giv unmerged with the base symbol.
Proposed status: unchanged; left = whole-loop register roles and the unmerged idx+i walker.

## wave 97 (W97-Y)
Base: draft (20.90%, 268). levers.py's 30.97% `5b-94+5b-32` is WRONG C though wrongc says OK: it assigns `idx = (b*10 + y*5) + i` inside the loop's else arm, so idx (the a-side index for passes 2-5) is clobbered. Rejected.
Separate-walker lever (W96-C's lead): `j = idx;` before the loop, `tbl[j]`/`gUnknown_08551F60[j]`/`gUnknown_08551E84[j]` for the three unk1a reads, `j++` at the loop end -> 272 bytes (+4), 20.96%. Copy-back forms (`nx = i + 1; j++; i = nx;`, `j` copy-back, i and j orders) are byte-identical: loop.c still reduces `j` and eliminates `i` (the exit test becomes a compare on the walker), so the ROM's separate `i` in sb (`movs r0,#1; add sb,r0`) is not reproduced by any step form.
Permuter (3 links of ~500 s from the walker form, each wrongc OK and diffed): the kept edits are a do { } while (0) around the unk1a if/else chain and `j++` moved inside a second do-while; size back to 268 exactly, 32.46%, first diff +0xa. This is size-exact only by another shape: frame is `sub sp, #4` against the ROM's `#12` and `tbl + j` is still one pointer walker (`ldr r7,[sp]`), so it is not the ROM's structure.
Proposed status: 32.5% (size-exact, frame #4 vs #12); left = ROM keeps tbl in sl, idx+i as its own +1 walker in r7, i in sb, and two spilled row/product slots; no source spelling tried keeps i alive next to the idx walker.
