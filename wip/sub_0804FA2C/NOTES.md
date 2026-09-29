# sub_0804FA2C — wave 93 (W93-B)

## New base: 72.94% at the EXACT size, up from 45.91% at +4

`best.c` / `recovered.c` (identical) scored 72.94% at 632 bytes — the ROM's size
— where the draft was 636 (+4) at 45.91%. The size delta closing is the real
result; the percentage follows it. Diffed against the draft and it IS equivalent
C. Three changes, all of them address binds or dead anchors:

- `new_var` (renamed `sideData`) is assigned `gUnknown_03004580` before the
  `sub_08015608` call and read twice afterwards, in `sub_08057D44`'s second
  argument and in the last statement's index. It holds the array's ADDRESS,
  which is a constant, so the values loaded through it at each read are exactly
  the values the draft's `gUnknown_03004580[...]` loads at the same point. An
  intervening call cannot change the address, only the contents, and the reads
  have not moved.
- `meta` (renamed `sideDataAnchor`) is assigned the same address inside the
  `tileNum` statement's subscript, via a comma operator, and never read. It is a
  dead allocno anchor — the documented wave-17 lever, creating the pseudo at a
  point no statement boundary can reach.
- the draft's four write-only locals `r1 r2 r3 r4`, one per position store,
  became a single `row` assigned four times. All five spellings are dead stores;
  collapsing four pseudos into one is an allocation change, not a semantic one
  (and per the wave-17 "binding locals are punctuation" rule it is the kind of
  change that moves bytes here).

Adopted; the draft is backed up as `sub_0804FA2C.w93-start.c` and the adopted
base as `w93-base-7294.c`. Re-measured after the renames: **72.94%, size-exact,
first difference +0x80**, identical to the pre-rename score.

## What this does to the recorded history

The parked entry warns that "best.c holds an unrelated variant scoring 72.9%,
and that number has twice been quoted as this function's state". It is not
unrelated: it is the draft plus the two address binds and the collapsed row
local, and it is worth the +4 the function had been carrying for four waves.

It also changes the standing diagnosis. The residual was recorded as
`&gUnknown_03004580` being materialised one instruction too early into the wrong
register, with the twins' comma-anchor fix (`sub_0804D290`, `sub_0804DCA8`)
measured as WORSE here — 652 bytes at 16.6% for the full form, 636 at 26.7% for
the anchor alone. Both of those measurements were taken with the four separate
`r1..r4` locals in place. With them collapsed to one, an anchor of the same kind
is worth 27 points and the exact size. The twins' lever was not wrong here; it
was being measured against a draft carrying three extra dead pseudos.

## Residual

632/632, 171 of 632 bytes differ, first difference at +0x80 — moved from +0xc,
so the divergence now starts well past the prologue and the tile-number
multiply the old residual described.

## Permuter, 900 s x 4 threads from the 72.94% base: 72.94% -> 75.16%, still size-exact

Kept in `sub_0804FA2C.c`; the base is `w93-base-7294.c` and the run's output is
also saved as `w93-perm1-7516.c`. One mutation, audited as equivalent C:

**a `do { ... } while (0);` wrapped around the first eleven statements**, from
`oam.hFlip` through the `.unk06` position store, leaving the final `.y` store
outside it. There is no `break` or `continue` in the body, so it runs exactly
once and nothing is reordered. Equivalent.

This is the statement-boundary/join lever, and it is worth noting that a
`do { } while (0)` is what the twins' promoted fix uses too — the parked entry
records the twins' form as 20 bytes too long HERE, but that measurement wrapped
the `sub_08057D44` statement. Wrapping the eleven statements BEFORE the last
position store is worth 2.2 points and keeps the exact size.

## Residual after the run

632/632, 157 of 632 bytes differ, 75.16%, first difference at +0x80.

## Anchor position swept against the new residual — already optimal

The residual at +0x80 is a missing pool-word materialisation. The ROM loads
THREE pool words before the index shift and parks the third in r8; the candidate
loads two and pays for it with two register copies at the multiply:

    ROM        ldr r5,=A / ldr r3,=B / mov r8,r3 / lsls r0,#4 / ldr r1,=C / adds r0,r0,r1
               ... ldr r3,=D / ldrh r0,[r3] / muls r0,r1
    candidate  ldr r5,=A /              lsls r0,#4 / ldr r3,=B / adds r0,r0,r3
               ... ldr r3,=D / ldrh r0,[r3] / adds r3,r1,#0 / muls r3,r0 / adds r0,r3,#0

So `&gUnknown_03004580`'s pseudo still needs to be created one position earlier.
Three further positions for the comma anchor were measured:

    inside the subscript, where the base has it ... 75.16%  size+0  first +0x80
    inside the CAST's operand .................... 75.16%  size+0  first +0x80  (identical code)
    as its own statement before `oam.tileNum` .... 75.16%  size+0  first +0x7e  (first diff EARLIER)
    in the FIRST operand of the `+` .............. 36.23%  size -4 first +0xc   (much worse)

The base's position is the optimum of the four, and moving the anchor into the
first operand loses the exact size as well. The lever that remains is not the
anchor's position but whatever makes agbcc keep the value in r8 across the index
computation instead of rematerialising at the multiply. Note the ROM's `muls
r0, r1` is bare: the two copies in the candidate are downstream of the
allocation, not an operand-order choice (the parked entry already records that
both operand orders compile identically).

## wave 96

Base: sub_0804FA2C.c (75.16%, size-exact, first diff +0x80). Draft is now `sub_0804FA2C.w96-single.c` = the old draft with the two address binds merged into ONE variable (`sideData`, assigned inside the tileNum subscript by the comma anchor; the later `sideData = gUnknown_03004580;` statement is gone) and the sub_08057D44 second argument spelled `*(u16 *)((u8 *)sideData + 6 + side * 16)`. Score unchanged (75.16%): both are byte-neutral. Old draft kept as `.w96-start.c`.

Reading of the ROM (target.s): ONE pseudo holds &gUnknown_03004580 (`ldr r3,=word; mov r8,r3`, placed right after the 085D6A48 word and BEFORE the row shift), and is used with immediates: `mov r1,r8; adds r1,#6` (sub_08057D44 argument, [side][3]) and `movs r2,#0xa; add r2,r8; mov sb,r2` (the [5] read, kept in sb and reused by the four position stores). So the ROM spells both reads as base + constant + side*16, i.e. the same "+N first" form as sub_080506B0. The pool word order confirms it: the ROM's 03004580 word sits before 03004582; the draft's sits after 0300450C.

Negatives:
- Converting the four `[side][5]` reads to `(u8 *)sideData + 10 + side*16`: 628 bytes (-4), 41.8%, first diff +0xc. Converting only the first, or only the fourth: +4 / +24 bytes, first diff +0xc. Any change to the count of by-name references to gUnknown_03004580 moves the start of the function (the pool word order shifts), so the [5] form cannot be tried without also getting the early pseudo.
- Merged single variable: byte-neutral. cse propagates the constant address into the later uses (a pseudo set once to a symbol_ref is replaced by the symbol), so the held register the ROM has (r8 across the call) is not created. The comma anchor alone creates the early pool word in the old draft only because the write is dead there and survives as a separate pseudo.
Not run: the permuter (75% base, prior 900s x4 run in wave 93 found only the do/while).

Proposed summary:
- does: sets up the cursor sprite for a side and slot, positions it from the position table and starts its effect
- status: 75% at exact size; first difference at +0x80
- left: the ROM holds &gUnknown_03004580 in one register from before the tile-number multiply and adds +6 / +10 to it for two later reads; the draft rebuilds the address at each read
- tried: comma anchor (kept), do/while wrap (kept), single merged variable, +6/+10 byte-offset spellings of the later reads (see NOTES)
