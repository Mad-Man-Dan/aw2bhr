# Notes for `src/unit.c`

Source: [`src/unit.c`](../../src/unit.c). These comments were moved verbatim from the C file; line numbers refer to the version before extraction.

## Note 1 — original line 5

````c
/* Unit/CO code, contiguous from 0x08042B70 through 0x08043630.
 * The preceding gap is undecompiled; the following function uses inline assembly.
 * Preserve function order for the original ROM layout. */
````

## Note 2 — original line 22

````c
/* gUnknown_030044B0 is declared `u8 []`, so the word store at +8 needs a cast.
 * The BASE must be bound to a local first: writing
 * `*(u32 *)(gUnknown_030044B0 + 8)` folds the 8 into the pool word as
 * `gUnknown_030044B0+0x8` and stores at `[r2]`, where the ROM has a clean
 * `gUnknown_030044B0` pool word and `str r0, [r2, #8]`. Same instruction
 * count -- the difference is entirely in the relocation addend.
 *
 * The element address is recomputed for the second store because the first
 * `strb` kills gPlayers's MEM (it is a non-const pointer global). */
````

## Note 3 — original line 54

````c
/*
 * GetUnitCostWithCoBonus, GetUnitMovementWithCoBonus, GetUnitFiringRangeWithCoBonus and GetUnitVisionWithCoBonus are named per
 * Xenesis's AW2 Subroutine List: "Gathers Price multipliers of CO",
 * "Collects Mov Range + CO Boosts", "Collects Max Firing Range + CO Boosts"
 * and "Gathers Vision Total of CO" respectively. GetUnitAttackWithCoBonus/GetUnitDefenceWithCoBonus
 * aren't separately cited there, but they're structurally identical to the
 * cited Mov/Range/Vision trio one function over (base stat + CO bonus, same
 * `+ 0x64` bias), so they're named by that analogy: attack and defence are
 * the two stats the cited trio's family (GetCoAttackBonus.../GetCoRangeBonus
 * in src/unit.c) doesn't already cover. The old sub_XXXXXXXX
 * symbols are kept as linker aliases below so every other unit keeps
 * resolving them unchanged.
 */
````

## Note 4 — original line 68

````c
/* The parent argument doubles as a tree NUMBER and a real ProcPtr, and the
 * signed `cmp #7` is what tells them apart -- hence the `(int)` cast, which a
 * pointer comparison would not give.
 *
 * The arms are written `<= 7` with Proc_Start first, so the ROM's `bgt` reaches
 * the else. That is the ORDINARY sense: these two arms REJOIN, and for
 * rejoining arms agbcc branches on the NEGATED condition to the else and leaves
 * the then inline. It is the opposite of what the same source shape does when
 * both arms `return` (see sub_08017CF0), where the else goes inline instead.
 *
 * unk64/unk66 are reached by advancing the base (`adds r0,#0x64` then
 * `adds r0,#2`) because 0x64 is past `strh`'s 5-bit displacement; unk2c/unk30
 * stay in the displacement, which is why only the halfword pair walks. */
````

## Note 5 — original line 83

````c
/* 00 */
````

## Note 6 — original line 84

````c
/* 2c */
````

## Note 7 — original line 85

````c
/* 30 */
````

## Note 8 — original line 86

````c
/* 34 */
````

## Note 9 — original line 87

````c
/* 4a */
````

## Note 10 — original line 88

````c
/* 4c */
````

## Note 11 — original line 89

````c
/* 64 */
````

## Note 12 — original line 90

````c
/* 66 */
````

## Note 13 — original line 109

````c
/* The two bounds are bound to LOCALS, and that is what the ROM's compares need.
 * Written as literals, agbcc's tree-level `fold` canonicalises `>= 2` to `> 1`
 * and, knowing unk18 is a u8, picks unsigned branches -- `cmp #3; bhi` /
 * `cmp #1; bls`. Via int locals the comparison is typed `int` at tree level, so
 * neither the fold nor the range-narrowing fires, and constant propagation
 * still puts both values back in the `cmp` immediates. Exactly the lever
 * docs/agbcc-codegen.md records for sub_08044BA0 (wave 26, W26-B).
 *
 * Binding both is necessary: `hi` alone leaves the second compare unsigned. */
````

## Note 14 — original line 134

````c
/* Both parameters are `int`, which corrects the `(u16, u8)` this function was
 * declared with -- see the note in unknown-functions.h. The prologue is a bare
 * `adds r4, r0, #0; adds r5, r1, #0` and both values live across two calls; the
 * narrow declaration adds four PROMOTE_MODE narrowing instructions. */
````

## Note 15 — original line 145

````c
/* The `lsls #4; subs; lsls #2` triple is the 0x3c stride of
 * struct PlayerStruct, and gPlayers is a pointer to it, so the pool
 * word is dereferenced once before the index is added. */
````

## Note 16 — original line 154

````c
/* GetUnitAttackWithCoBonus's twin over GetCoDefenceBonus. */
````

## Note 17 — original line 161

````c
/* The one-argument call is the LEFT operand of the `+`: agbcc evaluates it
 * first and parks the result in r4, then builds the three-argument call. */
````

## Note 18 — original line 169

````c
/* GetUnitMovementWithCoBonus's twin over GetUnitBaseFiringRange / GetCoRangeBonus. */
````

## Note 19 — original line 176

````c
/* The floor is written `if (n > 1) return n; else return 1;` and not the other
 * way round: both arms return, so agbcc emits the ELSE inline and branches to
 * the THEN on the condition being true, which is the ROM's `bgt` over an inline
 * `movs r0, #1`.
 *
 * gPlaySt.unk2c is reached with `adds r0, #0x2c` on the base rather
 * than an `ldrb` displacement because 0x2c is past `ldrb`'s 5-bit field -- that
 * is addressing, not a member-array tell. */
````

## Note 20 — original line 217

````c
/* `int`, not `u8`: a u8 return re-narrows the callee's result with
 * `lsls #24; lsrs #24`, which the original does not have. */
````

## Note 21 — original line 224

````c
/* `int`, not the `u8` this was promoted with. Its own bytes cannot tell --
 * it is a leaf ending in `bx lr` and `ldrb` zero-extends either way -- but
 * sub_08042DFC forwards the result straight out with no `lsls #24; lsrs #24`,
 * and a u8-returning callee makes the caller re-narrow. The call site is the
 * only evidence there is, and it says 32 bits.
 */
````

## Note 22 — original line 235

````c
/* Named per Xenesis's AW2 Subroutine List: "Gathers Luck Total of CO". The
 * old GetCoLuckBonus symbol is kept as a linker alias below so every other
 * unit keeps resolving it unchanged. */
````

## Note 23 — original line 249

````c
/* Player-level wrapper for GetCoLuckBonus. The old GetPlayerCoLuckBonus symbol is
 * kept as a linker alias below so every other unit keeps resolving it
 * unchanged. */
````

## Note 24 — original line 260

````c
/* Named per Xenesis's AW2 Subroutine List: "Gathers Negative Luck Total of
 * CO". The old GetCoNegativeLuckBonus symbol is kept as a linker alias below so every
 * other unit keeps resolving it unchanged. */
````

## Note 25 — original line 274

````c
/* Player-level wrapper for GetCoNegativeLuckBonus. The old GetPlayerCoNegativeLuckBonus
 * symbol is kept as a linker alias below so every other unit keeps
 * resolving it unchanged. */
````

## Note 26 — original line 301

````c
/* The second argument is never used, but it stays in the signature: the
 * forwarder next door loads gPlayers[a1].unk1e and passes it, and
 * that load is in the original's bytes. */
````

## Note 27 — original line 318

````c
/* The second argument is never used, but it stays in the signature: the
 * forwarder next door loads gPlayers[a1].unk1e and passes it, and
 * that load is in the original's bytes. */
````

## Note 28 — original line 336

````c
/* Named per Xenesis's AW2 Subroutine List: "Gathers Counterattack Bonus
 * Total of CO". The old GetCoCounterattackBonus symbol is kept as a linker alias below
 * so every other unit keeps resolving it unchanged. */
````

## Note 29 — original line 350

````c
/* Player-level wrapper for GetCoCounterattackBonus. The old GetPlayerCoCounterattackBonus
 * symbol is kept as a linker alias below so every other unit keeps
 * resolving it unchanged. */
````

## Note 30 — original line 377

````c
/* The per-army repair/income multiplier, applied as a percentage: the byte at
 * gUnknown_085D5ABC[c].unk1e[e][d] scales GetCoAttackBonus's result, which is
 * itself a percentage that the `+ 100` turns into a multiplier.  A zero
 * product is raised to 1, so a matchup that "works at all" never rounds away
 * to nothing, but a zero scale byte short-circuits to 0 before that.
 *
 * The return is `int` and NOT the u16 unknown-functions.h used to declare:
 * a u16-returning agbcc function narrows its own result in the epilogue and
 * this one does not.  See the note on the declaration.
 *
 * `base` is its own statement, evaluated unconditionally ahead of the
 * `v > 0` test -- the ROM does the `bl` and the `+ 0x64` before the `cmp`.
 */
````

## Note 31 — original line 410

````c
/*
 * Named per Xenesis's AW2 Subroutine List: "Attack Value of CO" / "Defence
 * Value of CO" / "Movement Bonus of CO" / "Range Bonus of CO" for the four
 * functions in this family respectively -- matches the `varies` s16 index K
 * (0/1/2/3) documented below, which selects which of the four stats to read
 * out of the same per-CO/power-state/unit-class table. Parameters named from
 * that: `a` indexes gUnknown_085D3DD0 (a CO), `b` indexes its `.unk38` (a CO
 * Power state), `c` indexes gUnknown_085D5ABC (a unit type) both directly and
 * via GetUnitCombatClassColumn. The old sub_XXXXXXXX symbols are kept as
 * linker aliases below so every other unit keeps resolving them unchanged.
 */
````

## Note 32 — original line 422

````c
/* Family F049: four copies of one body that differ ONLY in the s16 index K
 * (0, 1, 2, 3 -- `movs rN, #0/2/4/6` at the three `ldrsh` sites), exactly as
 * data/families.json `varies` says.
 *
 * Three things had to be read off the ROM rather than guessed:
 *
 * 1. struct CoModeData.unk24 is an array of POINTERS. The ROM does
 *    `ldr r7,[r3]` on the table word and then `ldrsh r0,[r7,r2]` through the
 *    result. 0x24 + 8*4 == 0x44 fills the record exactly, and the two indices
 *    in use (gUnknown_085D5ABC[c].unk18, and GetUnitCombatClassColumn's 5/6/7) span it.
 *
 * 2. Both lookups happen BEFORE the gPlaySt.coAbilities early-out. That is
 *    not scheduling -- agbcc will not hoist a load above a branch -- so the
 *    two assignments really are the first two statements, unlike the F038
 *    accessors in c_08042E2C.c where the early-out comes first.
 *
 * 3. gUnknown_085D5ABC is NOT const, and that is what the second
 *    `ldrb r0,[r6,#0x18]` proves. With the header's old `const` the load is
 *    CSEd across `bl GetUnitCombatClassColumn`, the value rather than the address lives in
 *    r6, and the function is two bytes short. See the note on the declaration
 *    in include/unknown-globals.h.
 *
 * The dead `ldr r7, =gUnknown_085D3DD0` at the top and the folded pool word
 * `gUnknown_085D3DD0 + 0x5c` (which asm/ symbolises as gUnknown_085D3E2C) both
 * fall straight out of the plain member spelling under -fforce-addr; neither
 * is something to reproduce by hand. */
````

## Note 33 — original line 466

````c
/* See GetCoAttackBonus above for the shared analysis this family relies on. */
````

## Note 34 — original line 485

````c
/* See GetCoAttackBonus above for the shared analysis this family relies on. */
````

## Note 35 — original line 504

````c
/* See GetCoAttackBonus above for the shared analysis this family relies on. */
````

## Note 36 — original line 528

````c
/*
 * Named per Xenesis's AW2 Subroutine List: GetCoCostBonus = "Gathers Cost
 * Bonus of CO. Input of CO ID, Power Status and Unit ID. Outputs cost
 * bonuses in r0", GetCoVisionBonus = "Gathers vision bonus of CO". The old
 * sub_XXXXXXXX symbols are kept as linker aliases below so every other unit
 * keeps resolving them unchanged.
 */
````

## Note 37 — original line 536

````c
/* Family F038, the shape of the dozen accessors in src/decomp/c_08042E2C.c.
 * The address arithmetic `(a * 65 + b * 17) * 4` is already encoded by
 * struct CoData (0x104) holding struct CoModeData unk38[3] (0x44),
 * so the only thing to read out of the assembly is the member: the ROM's
 * `adds r0, #0x4c` is 0x38 + 0x14, i.e. unk14.
 *
 * `movs r1, #0; ldrsh r0, [r0, r1]` is not a variation on the matched siblings
 * -- every member of F038 uses it, including GetCoLuckBonus/E84/FC4 -- it is just
 * how an s16 member is reached once the offset has been folded onto the base.
 * No re-narrowing after the load, so the return is `int`.
 *
 * THREE parameters, not the two this was first promoted with. The third is
 * unused in this member's body, so the body cannot tell you -- the evidence is
 * the call site: GetUnitCostWithCoBonus (wave 26, byte-exact) materialises `adds r2, r5,
 * #0` immediately before the bl, which only an argument produces, and the
 * sibling GetCoRangeBonus takes and uses the same three. An unused trailing
 * parameter is byte-neutral here; re-verified with trymatch after the change. */
````

## Note 38 — original line 562

````c
/* Family F038, the twin of GetCoCostBonus one member down: the ROM's
 * `adds r0, #0x44` is 0x38 + 0x0c, i.e. unk0c. See the note on GetCoCostBonus. */
````

## Note 39 — original line 576

````c
/*
 * Not itself Xenesis-documented, but named for what its caller
 * (GetCoAttackBonus and siblings, src/unit.c) uses it for:
 * mapping a unit's 3-valued combat-class tag onto which column of the
 * per-CO/power-state stat table to read. The old sub_XXXXXXXX symbol is
 * kept as a linker alias below so every other unit keeps resolving it
 * unchanged.
 */
````

## Note 40 — original line 585

````c
/* Maps the 3-valued tag at +0x0e onto a column index. `int`, not u8: its
 * caller GetCoAttackBonus does `lsls r0, r0, #2` straight off the `bl` with no
 * re-narrowing.
 *
 * The local has to be `u32`. The whole shape hangs on it: an UNSIGNED
 * comparison gives `cmp #1; bls` for the first test, a signed one gives `ble`
 * -- one bit of encoding, no size difference. And a `switch` statement does
 * not reach this at all in any case order (four spellings probed): it expands
 * to two linear `cmp/beq` equality tests, never the range test plus a single
 * equality test the ROM has, even though the two come out the same 36 bytes.
 */
````

## Note 41 — original line 611

````c
/* The pointer parameter's +0x00 is a `struct Unit *` into the unit
 * table and +0x06 an s16.  `(p->unk00 - gUnits) >> 6` is the
 * 0-based army number -- the table is grouped 64 entries to an army, the same
 * `>> 6` unknown-globals.h already records -- and the `+ 1` makes it the
 * 1-based one GetPlayerSpecialAbilities indexes by.  The whole `sub/lsl/add/neg/asr #8`
 * chain is agbcc's EXACT division for the pointer difference (multiply by
 * -inverse(3), negate, then one `asr` that carries both the /4 of the exact
 * division and the >> 6) -- it is one operator, not five.
 */
````

## Note 42 — original line 622

````c
/* 0x00 */
````

## Note 43 — original line 623

````c
/* 0x04 */
````

## Note 44 — original line 624

````c
/* 0x06 */
````

## Note 45 — original line 652

````c
/*
 * GetUnitBaseCost is named per Xenesis's AW2 Subroutine List: "Subroutine that
 * gets the unit's base cost". The other three in this family aren't
 * separately cited there, but each feeds exactly one Xenesis-named "+ CO
 * Boosts" total in src/unit.c (base movement into
 * GetUnitMovementWithCoBonus, base firing range into
 * GetUnitFiringRangeWithCoBonus, base vision into
 * GetUnitVisionWithCoBonus), so they're named by that structural analogy
 * rather than a direct citation. The old sub_XXXXXXXX symbols are kept as
 * linker aliases below so every other unit keeps resolving them unchanged.
 */
````

## Note 46 — original line 664

````c
/* All four return `int`, not the u8/u16 they were first promoted with. The
 * bodies are bare ldrb/ldrh member loads and are byte-identical either way, so
 * the width was a body-side guess with no oracle. Wave 26 produced the first
 * promoted callers and they settle it: GetUnitCostWithCoBonus does `bl GetUnitBaseCost;
 * adds r6, r0, #0` and multiplies with r6 later -- no re-narrowing after the
 * bl with the value used, which a u16 return cannot produce (agbcc re-narrows
 * a narrow-returning callee's result at every call site). GetUnitMovementWithCoBonus,
 * GetUnitFiringRangeWithCoBonus and GetUnitVisionWithCoBonus show the same for the other three. Settled
 * from the callers; all four re-verified byte-exact after the change. */
````

## Note 47 — original line 704

````c
/* A `switch` over 19 sprite kinds that nudges the (x, y) it was handed, picks
 * an animation id and puts one sprite. Cases 4 and 8..11 are absent -- their
 * jump-table slots point at the tail, which is also the default, and the id
 * that reaches sub_0801C7DC in those cases is the selector itself.
 *
 * Case 0 reads the unit id under the pixel position: the s16 scroll fields
 * (gMap->scrollX / scrollY) are added before the `>> 4` that turns a pixel
 * coordinate into a cell coordinate, and the cell is gMap->unit[idx]. They are
 * a different pair from the tile-granularity camX / camY.
 *
 * `id` is REUSED as the 4-or-0 flag in case 0, rather than a `flag` local:
 * a separate local lands the constant straight in r2 and the original spends
 * `adds r2, r4, #0` copying it out of the switch variable's register. The
 * rowOffset load is its own statement with the column computed after it, which
 * keeps the frame at `push {r4, r5, r6, lr}`.
 */
````

## Note 48 — original line 802

````c
/* Picks the corner an on-screen box should hang off: the screen is split at
 * x = 0xd0 and y = 0x80 and the sprite id is bumped by 1, 2 or 3 for three of
 * the four quadrants, leaving the top-left quadrant unchanged.  All three
 * comparisons are signed, so all three parameters are `int`.
 */
````
