# Notes for `src/battle.c`

Source: [`src/battle.c`](../../src/battle.c). These comments were moved verbatim from the C file; line numbers refer to the version before extraction.

## Note 1 — original line 3

````c
/* Battle damage and resolution, 0x08024ABC–0x08025308.
 * The consecutive pool words at 0x08090A34–0x08090A44 belong to this unit. */
````

## Note 2 — original line 12

````c
/* 0x00 */
````

## Note 3 — original line 13

````c
/* 0x04 */
````

## Note 4 — original line 14

````c
/* 0x0a */
````

## Note 5 — original line 15

````c
/* 0x0c */
````

## Note 6 — original line 16

````c
/* 0x10 */
````

## Note 7 — original line 17

````c
/* 0x12 */
````

## Note 8 — original line 18

````c
/* 0x18 */
````

## Note 9 — original line 139

````c
/* struct Unk030013D0 lives in include/unknown-globals.h -- see the note there
 * for how each member was measured. It is shared rather than local because
 * this function's prototype names it and sub_08024E60 calls it. */
````

## Note 10 — original line 191

````c
/* Normalises a pair of signed counters at +8: if BOTH are non-positive the
 * larger becomes 1 and the other 0; otherwise each negative one is clamped to
 * 0. The two clamps are shared -- the `a` clamp falls THROUGH into the `b`
 * clamp, which is why the b test sits outside the outer `if`.
 *
 * The member is s16 and that is measured: every read is
 * `movs rI,#8; ldrsh rD,[rB,rI]`, the s16-OBJECT tell. A u16 member compared
 * `> 0` would give `ldrh` and an unsigned branch. The record itself is not
 * modelled -- only +8 is reached here -- so the struct stays local to this
 * translation unit rather than going into unknown-globals.h. */
````

## Note 11 — original line 202

````c
/* WAVE 36 (W36-A): the local struct is GONE and the parameters are the shared
 * struct Unk030013D0. CalcBattleDamage's call site settles it -- it passes
 * gUnknown_030013D0 and gUnknown_030013B0, the same two records it hands to
 * sub_08024A2C / SelectBattleWeapon / sub_08024C58 / sub_08024E60 -- and the choice
 * is byte-visible at that caller, because two tags naming one symbol split
 * agbcc's -fforce-addr pool word in two where the ROM has one. The `s16 unk08`
 * measured below IS Unk030013D0's unk08; this function still matches
 * byte-for-byte after the change. See include/unknown-functions.h. */
````

## Note 12 — original line 296

````c
/* 0x00 */
````

## Note 13 — original line 297

````c
/* 0x04 */
````

## Note 14 — original line 298

````c
/* 0x08 */
````

## Note 15 — original line 299

````c
/* 0x0a */
````

## Note 16 — original line 345

````c
/* Family F000 (tools/families.py): `push {lr}; ldr r0,=X; bl S;
 * pop {r0}; bx r0` -- a one-line forwarder. `pop {r0}` is the void epilogue
 * per docs/agbcc-codegen.md, so the callee's result is discarded and this
 * returns nothing. The exemplar is src/decomp/c_080733B8.c.
 */
````

## Note 17 — original line 352

````c
/* The odd one out of this batch: the pool word is a RAM address
 * (0x030013D0), so the global is NOT const -- sub_0802505C writes through it.
 * It is declared `u8 []` rather than a struct: only the ADDRESS is used here,
 * and `gArray` and `&gScalar` both give the clean pool word, so nothing in
 * this function discriminates. The offsets that are known are recorded on the
 * declaration in include/unknown-globals.h.
 */
````

## Note 18 — original line 365

````c
/* THREE parameters, and the third is invisible here: CalcBattleDamage reads r2 as
 * a pointer (`ldrh [r2]`, `ldrh [r2,#2]`) and this function never writes r2, so
 * it forwards a parameter it does not otherwise touch -- the pass-through
 * wrapper case in docs/agbcc-codegen.md, settled on the callee's prologue.
 * sub_080251D8 reads r0 the same way, which is why the then-arm's `bl` has no
 * argument setup at all.
 *
 * a1 and a2 are `int`: a2 is tested raw (`cmp r1,#0` with no entry narrowing,
 * so no PROMOTE_MODE), and the `lsls #0x10; asrs #0x10` pair on each is the
 * CONVERSION to CalcBattleDamage's s16 parameters at the call. */
````

## Note 19 — original line 446

````c
/* F010: `push {lr}; ldr r0,=g1; bl S1; ldr r0,=g2; bl S2; pop {r0}; bx r0` --
 * two statements, each with its own pool word, result of each discarded.
 * src/decomp/c_08044924.c is the matched exemplar.
 * Both calls go to the same callee -- sub_080252E8, a bare `bx lr` stub -- and
 * the two pool words are DIFFERENT symbols (0x030013D0 then 0x030013B0), so
 * this is not the 'agbcc emits two loads of one symbol' case. They are the two
 * instances of one RAM record; source order is D0 first, which is not address
 * order. */
````
