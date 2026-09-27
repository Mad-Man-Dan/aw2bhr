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
