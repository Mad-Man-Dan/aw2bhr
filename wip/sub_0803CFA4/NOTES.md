# sub_0803CFA4 notes

## Wave 94 (W94-B): first permuter run. Two "improvements", both wrong C

`--current`, 900 s, 4 threads, under the length-penalised scorer. The run
reported a chain of improvements and kept the last: **65.15% -> 73.18%,
size-exact**. It is rejected, as `w94b-perm1-k-dropped.c.wrongc`. Two
independent errors in one file:

- `a2[0x4CB + k] = tbl[(v >> 6) + 1] | e->type;` became
  `a2[0x4CB + 0] = ...` in the non-zero arm only. Every cell with a unit on it
  writes that unit's byte to the same first slot instead of its own, and the
  `else` arm still uses `k`, so the two arms disagree.
- `unsigned char new_var = (k++) * 2;` for the halfword store's offset. `k * 2`
  reaches about 0x4C0 on a full map, so an 8-bit offset wraps. (The buffer one
  caller passes is 0x724 bytes.)

Neither is visible to any automatic check: every variable is set before it is
read, and the length does not change.

A second candidate, at **73.48%**, was refused by `permute.py`'s own
uninitialised-read guard ("reads `cells` before setting it") -- but
`trymatch.record_best` had already written it into `best.c`, so `best.c` and
`best.json` were left recommending a file the permuter had just rejected.
`best.c` has been reset to the draft and the refused form kept as
`best.c.wrongc`. **Check `best.json`'s `origin` after any permuter run: a
"permuter" origin on a function whose draft did not change means best.c holds a
candidate that was refused or superseded.**

### The one honest idea in those candidates is a negative

Both candidates bound the halfword store's address to its own pointer before the
store, which is a correct transformation on its own and close to what the park's
`left` line asks for. On its own it is **35.39% and 4 bytes too long**
(`w94b-store-ptr.c`):

    dst = (u16 *)(a2 + 2 + k++ * 2);
    *dst = ((u16 *)cells)[idx];

So the whole 8-point gain the run reported came from the wrong-C parts, not from
the address split. The frame is still 32 bytes where the original reserves 36,
and the per-cell counter is still in a high register.

## wave 97 (W97-M)
Base: current draft (65.15%, size-exact, frame 0x20 vs ROM 0x24). Un-binding probes (spellings.py, frame meter): dropping the `w` binds -4 bytes (54.7%); dropping the
`q` bind (inline `a2[0x4C4+i]`) byte-identical; deleting `*((u8 *)gPlayers) += 0` 64.55% (still needed, 0 bytes); replacing `rows` by the inline sum at the 3rd read +20;
inlining `e->type` +16. None moves the frame to 0x24; the frame still only ever shrinks when a bind is removed. The brief's "-1 copy, un-bind first" does not match
the measured state: this draft is size-exact and one spill slot SHORT, not one copy over.
