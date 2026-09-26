# sub_08010EF8 — parked, 64 bytes, SIZE-EXACT, 79.7%

Carried forward from wave 42 (W42-C). Wave 51 (W51-J) re-derived it from the
assembly with the existing draft set aside, reached the same body independently,
and then measured three spellings the earlier note had not covered. The body is
not in question; **one instruction's position is**.

## Remaining diff — 13 of 64 bytes, all positional

    ROM         ldr r6,[pc,#40]                <- preheader, r6 = 0x0808DF8C
                _08010F0A: ... bl __umodsi3
                ldr r1,[r6] ; ldrh r1,[r1]     <- BOTH loads inside the loop

    candidate   ldr r0,[pc,#40] ; ldr r6,[r0]  <- both hoisted
                .L3: ... bl __umodsi3
                ldrh r1,[r6]                   <- only the ldrh in the loop

Same instruction count, hence the exact size. The ROM re-reads the pointer from
0x0808DF8C on every iteration; every candidate so far hoists that read.

## What wave 51 measured that is NEW

The earlier note recorded only SCALAR spellings and concluded that any non-const
spelling "overshoots to four levels". That is true of scalars but **not of the
array form**, and the array form is the closest shape anyone has produced:

| declaration | address handling | the middle `ldr` |
|---|---|---|
| `u16 *const gUnknown_0808DF8C;` (draft) | inline pool, correct | **hoisted** |
| `u16 *gUnknown_0808DF8C[];` | inline pool, **correct** | **hoisted** |
| `u16 *volatile gUnknown_0808DF8C[];` | force-addr, 4 levels | hoisted |
| `u16 *gUnknown_0808DF8C;` (scalar, non-const) | force-addr, 4 levels | hoisted |

The two force-addr rows reproduce the wave-42 finding and can be dropped from
future consideration. **The `u16 *gUnknown_0808DF8C[]` row is the new one**: a
non-const, non-volatile ARRAY of pointers keeps the plain inline pool word — no
`.rodata` force-addr word, no fourth level — so it is exactly one LICM decision
away from the ROM, and it is the shape the next attempt should start from.

## The axis, stated precisely

`const`ness and `volatile`ness are NOT the lever, and this is the correction to
the wave-42 note: every one of the four rows above hoists the pointer load,
including two that are neither const nor unchanging. agbcc decided the
`strh r1,[r5]` in the loop body cannot alias a static symbol's storage, so the
load is loop-invariant in all of them. `volatile` does not rescue it because
agbcc reaches for `-fforce-addr` first and the extra level costs more than the
hoist saves.

**So the question is not how to spell the global. It is what makes agbcc treat
the store as a possible alias of it.** Nothing about the declaration of
`gUnknown_0808DF8C` can answer that; the next thing to vary is `dest` — the
store's own pointer — since the aliasing decision is made against the STORE, not
the load.

Not a permuter case: the difference is instruction ORDER across a loop
boundary, not a register assignment.

## Wave 58 (W58-A): the array row is confirmed, and the axis is now closed

Re-measured `u16 *g[];` (non-const, non-volatile) and `u16 *const g[];` side by
side in one probe. **They are byte-identical to each other and to the scalar
draft** — both emit the plain inline pool word and both hoist `ldr r6,[r0]` into
the preheader. So the wave-51 row is right that the array form keeps the correct
pool word, and right that it does not fix the hoist; it is not "one LICM decision
away" in any way a declaration can reach.

What that rules out is bigger than the row. This loop **contains two calls**
(`__umodsi3`, `__udivsi3`) and a store through a pointer parameter, and agbcc
hoists a non-const, non-volatile global load out of it anyway. Any theory that
the ROM's reload is explained by call-clobbering or by store aliasing is dead:
the two compilations differ in neither. Ten spellings across three waves now
land on the same instruction stream.

**Classification: constant placement, and specifically an LICM decision, which
is instruction ORDER across a loop boundary — the same unreachable family as
`sub_080373F0`'s block layout.** No permuter case (order across a loop, not a
register), and no declaration case (all ten spellings measured). The one axis
never tested is whether the ROM word at 0x0808DF8C is agbcc's own force-addr
slot for `gUnknown_0300308C` rather than a pointer object; if it is, the source
is the honest `gUnknown_0300308C[0]` and the missing instruction is agbcc
choosing force-addr for a constant subscript, which wave 42 showed cannot be
talked into it either.

## Settled, do not re-derive

- The body: prologue, the `dest + x + 0x20` start cell, both libgcc calls, the
  digit arithmetic, the u16 re-narrowing and the exit test are all byte-exact.
- The symbol: 0x0808DF8C holds 0x0300308C; +4 and +8 both hold 0x03001408, and
  0x0808DF98 onward is glyph data (0x00033300, 0x00330030, 0x03300033), so the
  three pointer words are a short run and not a long table.
- `sub_08010F38` reads the same gUnknown_0300308C at +0 and +2 through a plain
  inline pool word and MATCHED, so the two spellings coexist in one original
  `.c` and the choice is per (function, symbol).
- `extern u16 *const gUnknown_0808DF8C;` in include/unknown-globals.h exists
  only for this draft and should go with it if this stays parked.

# Wave 92 (W92-A)

NOT RE-MEASURED BEYOND `tools/drafts.py bases`, which confirms both `best.c` and
the draft at 79.69%, size-exact, first difference +0x11. The wave-79 row that
says "treat this entry as closed" is correct and I did not spend a try_match
against it.

WHAT THE WAVE-92 BRIEF ASKED FOR IS ALREADY IN THE RULED-OUT LIST. It told this
batch to "fix the pool-word spelling" by naming gUnknown_0300308C directly. That
is the first entry under `axes_ruled_out`: the honest spelling is one
instruction SHORT, because agbcc then puts 0x0300308C straight in the text pool
and the loop body keeps only the `ldrh`.

ONE NEW OBSERVATION, and it is narrower than another declaration sweep. Three
other functions in this same batch (sub_080359A4, sub_0802F588, sub_0802F6A0)
DID close part of their gap by naming the global directly, and in all three
agbcc answered with the very chain this function needs -- a text pool word
pointing at a `.rodata` word that holds the global's address, then two loads.
So agbcc can be made to emit it.

The mechanism is already settled in docs/agbcc-codegen.md ("The `.rodata`
force-addr word is made by GCSE's PRE", wave 91 W91-B): at expand EVERY global
access goes through the pool as `(set P (symbol_ref .LCn))` then
`(set Q (mem/u P))`, the pair collapses to a plain `ldr rQ,=g` when P has ONE
use, and the `.LCn` word survives only when P has SEVERAL -- which is what
gcse's PRE creates. That chapter even names this function's shape as a case
where PRE fires: "a loop containing a call". This loop contains two
(__umodsi3, __udivsi3).

SO THE OPEN QUESTION IS NOT "WHICH DECLARATION". It is why, with the honest
spelling, P ends up with one use here when PRE has a partially redundant
occurrence to unify and a call in the loop to make the pseudo live. That is a
question for `-da`/`-dp` dumps of this function's `.gcse` pass, comparing the
`.LC` pseudo's use count against sub_0805A9AC's (where the chapter traced the
insns). `-fno-gcse` is already in the swept flag set and did not move this
function, which is itself a datum: if the word were a PRE insertion we would
expect the flag to change something here.
