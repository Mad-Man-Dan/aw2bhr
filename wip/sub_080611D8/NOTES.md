# sub_080611D8 — wave 93 (W93-E): the return-path park is broken open

Start of wave: 75.00%, size-exact (304/304), first difference at **+0x10**.
After one 900 s permuter run: **93.42%**, size-exact, first difference at
**+0xde**. This function had never had a permuter run.

## What moved it

The park's residual was exactly this: the ROM keeps **two separate `return 1`
tails** and one shared `return 0` placed last, while every structured spelling
merged the two `return 1` tails instead. Six hand spellings had been tried
across waves 70–92 (nesting the call chain, inverting the final test, a single
exit through a result variable, explicit labels, switch and do-while forms) and
none of them reached it.

The lever is a **local holding the success value**, so that the two sites are
not the same expression when the late `jump2` cross-jumping pass looks at them:

    r = 1;
    if (out[0] == 9999) return 0;
    gUnknown_085766E4[best].unk03 = 0xfe;
    return r;              /* r is provably 1 here, folds to movs r0,#1 */
  alt:
    r = sub_08061668(out);
    if (r != 0) return r;  /* r is NOT provably 1 here */
    return 0;

With `return 1;` written at **both** sites the two tails are identical at
`jump2` and get cross-jumped into one, which is the whole 76-byte residual.
Reverting only the second one (`w93-p1-clean.c`) drops the function straight
back to 75.00% / first +0x10 — measured, not assumed. So the non-constant
return is load-bearing and is the thing to keep.

## Why the non-constant return is correct C, not a matching hack

`sub_08061668` is promoted (`src/decomp/c_08061668.c`) and returns only 0 or 1.
On the `if (r != 0)` arm the only value `r` can hold is 1, so `return r;` and
`return 1;` are the same function. The compiler cannot prove that, which is
precisely why it stops merging the tails.

## Audited and cleaned

The permuter also mutated `u8 t` to `char t`. Measured both: byte-identical
(93.42%, size+0, first +0xde either way), which incidentally confirms plain
`char` is unsigned under agbcc for this target. `u8` is kept, since
`gUnknown_085D5ABC[].deployLocation` is `u8` (`include/unit.h:235`).

Nothing else in the kept form changes a statement's meaning: no variable is
read before it is set, no call is duplicated or dropped, no store crosses a
call. The rest of the diff against the wave-start draft is brace style and the
permuter's reflow of the `||` chain into parenthesised pairs.

## Where it stands

Residual is now the last ~20 bytes, from +0xde. Files: `perm-w93-1.log` (the
run that found it), `sub_080611D8.w93-start.c` (the 75.00% wave-start draft),
`sub_080611D8.w93-r1.c` (this 93.42% form), `w93-p1-clean.c` (the measured
negative above).

## Second run

A chained 900 s run from the 93.42% form (`perm-w93-2.log`, run under the new
length-penalty scorer) found nothing better. The draft is left at the 93.42%
form. The last ~20 bytes from +0xde are a hand problem, not a search problem.
