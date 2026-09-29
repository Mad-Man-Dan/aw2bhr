
# Wave 92 (W92-A)

    inherited draft                              280  +0  40.36%
    + the three globals named directly           280  +0  40.71%
    + n computed after the cursor read           280  +0  41.79%
    + the byte count typed u16 in the prototype  280  +0  45.00%   <- kept

## The three 0x08090C8x words are agbcc's own, and the honest spelling emits them

0x08090C80 / C84 / C88 hold 0x0300410C, 0x02025818 and 0x030040CC -- the write
cursor, the send ring and the read cursor. They are one -fforce-addr address
block with addends 0, 4, 8, not a ROM table of pointers, and the draft's
`*gUnknown_08090C80` / `((struct Unk0802F588Ring *)gUnknown_08090C84)->ring[i]`
spellings were reproducing by hand what the compiler emits by itself. Naming
gUnknown_0300410C, gUnknown_02025818 and gUnknown_030040CC directly gives the
ROM's three-level chain for the early references AND the ROM's direct pool
words for the later ones -- agbcc makes that split itself, and it is the same
split the ROM shows (`ldr =gUnknown_08090C84; ldr` early, `ldr =gUnknown_02025818`
from the first checksum store onward).

This also disposes of the wave-49 note about a "dead ldrh no pointer spelling
produces". The dead load before each store belongs to gUnknown_02025818's
`volatile u16 []` declaration; reached by its own name, every store in the
function gets it, and the local ring struct is unnecessary.

## The second parameter is a u16, and that is where 3 points were

The ROM's `lsls r1,r1,#16` sits in the prologue, five instructions before the
`lsrs r6,r1,#17` that uses it. A cast written at the use emits both shifts
together at the use; only a sub-word PARAMETER puts the first one in the
prologue's own insn group, where combine then merges the entry's `lsrs #16`
with the source's `>> 1` into a single `lsrs #17`. So the parameter is `u16`,
not `int`, and include/unknown-functions.h now says so.

BYTE-NEUTRAL AT EVERY CALLER, and checked rather than assumed: all ten call
sites pass a small literal (4, 0x1a, 0x84), so the narrowing costs no
instruction. sub_0802FA64, sub_080309AC, sub_08030D84, sub_080319AC,
sub_08031A24 and sub_080320AC were re-run through trymatch after the retype and
all six still MATCH.

## Statement order

The ROM reads the write cursor BEFORE computing the halfword count, so
`chk = 0; cur = gUnknown_0300410C; n = a2 >> 1;` in that order. The count's
starting value still has to be assigned in two statements (`sum = 0x4fff;
sum = n + sum;`): folding it into one is 4 bytes long and 11 points worse.

## The residual

Size-exact, allocation. Two visible facts, both downstream of register choice:
the dead `ldrh` before the 0x4FFF store picks the register holding 0x4FFF, so
our build reloads the constant where the ROM keeps it live; and in the second
cursor advance the ROM parks both `cur * 2` and `cur + 1` in hi registers where
we keep `cur * 2` low. The two cancel in size.

## Permuter, run 1 (900 s, 4 threads, chained from the 45.00% draft)

    45.00% -> 82.14%, still size-exact 280/280, first difference +0x10

Kept from work/sub_0802F588/permuter/output-1500-1, spliced. Two changes, both
behaviour-preserving, and both checked by hand before keeping them:

  * `new_var = 0x4fff; gUnknown_02025818[cur] = new_var;` -- the marker
    constant copied into a local before the store. This is the fix for the
    residual the hand analysis had already named: the dead `ldrh` in front of
    that store was picking the register holding 0x4FFF, so our build reloaded
    the constant from the pool where the original keeps it live.
  * `chk = n; return chk;` -- the returned count copied into the checksum
    accumulator, which is dead by that point. `n` is `a2 >> 1` on a u16
    parameter, so at most 0x7FFF, and `chk` is u16: the copy is lossless and
    the returned value is unchanged. It is a register-assignment device, not
    something the original is likely to have written.

Both are left exactly as the permuter wrote them.

CONTRAST WITH sub_080359A4 IN THE SAME WAVE, worth recording because it cuts
both ways: here the best permuter SCORE (1500 against a 4140 base) was also the
best byte identity, while on sub_080359A4 the best scores verified 20 points
BELOW a candidate that barely improved on the base. The score is not reliably
wrong, it is just not the verdict -- harvest every output.

## Permuter, run 2 (chained from the 82.14% draft) -- RESULT NOT KEPT, AND WHY

    82.14% size-exact  ->  86.62% but FOUR BYTES TOO LONG

`tools/permute.py` keeps whichever candidate has the highest percentage and
does not look at the size delta, so it installed the +4 form over a size-exact
draft. Restored to the size-exact 82.14% draft; the rejected candidate is kept
at `work/sub_0802F588/w92-perm2-plus4.c` and at
`work/sub_0802F588/permuter/output-1240-1`.

THIS IS THE SECOND TIME THIS EXACT FUNCTION HAS BEEN CAUGHT BY IT. The wave-74
entry already records "an older permuter result ... scores higher by position
but is 4 bytes too long" as a rejected candidate. A positional percentage on a
draft that is the wrong size says where the divergence starts, not how much is
left, and a candidate that cannot be the right size cannot match.

The rejected candidate is still worth reading, because its two changes point
somewhere:

  * `sum = 0; chk = sum;` in place of `chk = 0;` -- the two accumulators
    initialised from one another. `sum` is overwritten with `n + 0x4fff`
    before it is read, so this is behaviour-preserving.
  * `new_var2 = 0x1ff; cur = (cur + 1) & new_var2;` inside the payload loop --
    binding the mask to a local. THIS is what costs the four bytes, and it is
    aimed at something real: the ROM materialises 0x1FF once and keeps it in a
    register (`ldr r1,=0x1ff; mov r8,r1`), reusing it at every cursor advance.
    A spelling that gets the ROM's single materialisation WITHOUT the extra
    instruction is the next thing to try on this function.

FOR THE TOOLING: permute.py's keep rule should require the candidate's size
delta to be no worse than the base's, or at least refuse to replace a
size-exact draft with one that is not.

## wave 96

Base: existing draft (82.14%, size-exact), kept as `sub_0802F588.w96-start.c`; draft unchanged.
The diff is two hi-register assignments swapped: the ROM keeps `chk` in r9 (zeroed in the prologue) and the ring base in ip
(`mov ip,r0` after `ldr r0,[cell]`); the draft has them the other way round, and the cascade moves cur*2 / cur+1 and the mask.
The final `chk = n; ... return chk;` copy is not in the ROM (it returns `adds r0,r6,#0`, i.e. n itself), but removing it (`return n;`)
drops to 60.7%: the copy is what keeps `chk` off the register the ROM leaves alone (see the header comment), so the +2 copies of the
pre-registration are NOT this one.
Negatives: declaring `chk` before `sum` and moving `cur` in the declaration list are byte-identical. Permuter, one 900 s run:
82.14 -> 82.50% but the kept change is WRONG C (`cur = chk; ... return cur;` stores n into the write cursor); rejected and the draft restored.
Proposed summary: does = writes one packet into the send ring; status = 82.1%, size-exact; left = chk (r9) vs ring base (ip) swapped;
tried = decl order, return n, one permuter run (its only gain was wrong C).

## wave 97 (W97-S)
Draft unchanged (82.14%). best.c (86.62%) is +4 bytes, not progress. Tried by spellings.py: `chk = 0` moved to just before the first loop 22.9% +8; `chk = chk - prod - 1` 43.7% +4; s16 chk 41% +8; s16 sum 17.7% +8; swapping the sum/chk statements in the loop 77.5% size-exact; `chk = 0` placed after `cur = ...` 77.5%. None moves chk to r9.
