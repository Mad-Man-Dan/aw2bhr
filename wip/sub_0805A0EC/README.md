# sub_0805A0EC

0x0805A0EC, 380 bytes, THUMB, parked.

Best score so far: 99.0% (best.c).

## What it does

AI: lists candidate tiles for the active unit. The candidates are reachable tiles whose terrain belongs to the current army and whose terrain type matches the unit's type in a table. It skips tiles that hold another unit. It also skips the active unit's own tile when byte 0x0B of that unit is 5. Each entry gets x, y and a score: the tile's movement value. The code also adds a bonus when another unit is on the tile, but that branch can never run because those tiles were already skipped. The list ends with 0xFFFF.

## How close it is

Compiles to the right size. 2 bytes of code differ: a zero is stored from a different register than in the original.

## What is left

Find a way of writing the store of zero that puts the zero in r0 and still puts the next copy in r2. Each spelling tried so far gets one of the two right and the other wrong.

## Already tried

- Writing the map reads as struct Map members: no change. Writing the row pointer as a member reorders an addition and is worse.
- Zero as a literal, as a variable, set early or late: each fixes one register and breaks the other.
- The automatic permuter: 15 minutes (about 35,000 tries) in this round and several runs before it, with no improvement.
- Compiling with the gcse pass turned off: 16 bytes too short.

## Files

- `sub_0805A0EC.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `NOTES.md`: working notes
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Why it is parked

PARKED Wave 70 at exact size 380/380 with exactly 4 code bytes different. ROM's zero store uses r0 and the verified draft uses r3; a literal obtains r0 but rotates the following row-pointer copy away from ROM r2. Two chained permuters plus literal, chained, late, pointer, width, deleted-anchor and fixed-low-register variants cannot satisfy both decisions simultaneously. Residual is one coupled allocno tie. W77-M 2026-08-18: re-confirmed at 380/380 and 4 reported bytes, of which only TWO are code (+0xd5/+0xd6, `movs r0,#0; strh r0,[r5,#2]` against the same pair in r3); +0x34 and +0x164 are addend bytes of the equivalent .rodata alias and are not residual. No new axis was attempted -- this entry already eliminates the source axis across waves 62, 66 and 70 and the residual is a bare two-pseudo allocation tie. Wave 80 (W80-F), dead-statement transfer, three compile_probes, no try_match spent, still 380/380 and 2 code bytes: literal zero plus a dead z = 0 before the p binding is identical to the plain literal (movs r0 but mov r3,ip) -- cse does not substitute a register for a literal's own register, so a dead CONSTANT set is not reusable the way a dead computation is; the W80-A dead-int form on props[off] rewrites the selector block (worse); literal zero plus a live pp/pp2 copy of props+off to inflate r3's reload use count leaves mov r3,ip. Refined residual: the permuter's long-lived new_var3 zero occupies r3 so reload takes r2 for the ip copy; with the literal r3 is free and sorts first. The ROM has the zero in r0 AND something live in r3 across the store, and no construct measured supplies that value. W83-B 2026-08-26: configured re-verdict 380/380, 98.9%, 4 reported bytes (+0x34/+0x164 addend bytes non-residual, +0xd5/+0xd6 zero-store pair) -- unchanged. Grep rule applied: promoted c_08085F94.c writes the window-adjacent pair `proc->unk58 = gUnknown_03005990[gUnknown_0300596C] + gUnknown_03005980; proc->unk5c = ...;` but its spellings address a different statement set and transfer nothing. No probe spent; batch budget went to the permuted sibling.

WAVE 84 (W84-C): a chained --current campaign did NOT reproduce the wave-83 win pattern -- both ~13.7k/14.5k-iteration runs were killed by environment-level interrupts partway (not tool timeouts; identical invocations ran to completion for sibling targets) after reporting nothing above the starting ceiling. best.c audited binary-equal to draft. The zero-store r0-vs-r3 tie stands.

WAVE86: WAVE 86 (W86-A): (1) wave-84's killed chain re-run full length from w84-start.c: 300 s, 14,942 iterations, ended on its own deadline, nothing above the ceiling -- a REAL negative now, not an abort. (2) W80-F's premise 'the ROM has something live in r3 across the store' is REFUTED by tracing the ROM: nothing is live in r3 there. (3) The residual is an exact two-way TRADE measured off the .s across four spellings in one unit: a long-lived zero local puts the zero in r3 (wrong) and the ip-copy scratch in r2 (right); a literal, a later-dead live copy, or a LATE-bound zero local all put the zero in r0 (right) and the ip copy in r3 (wrong); the ROM wants r0 AND r2. (4) NEW FACT: it is the zero local's LIVE-RANGE START, not its existence, that pins it to r3 -- `new_var3 = 0;` immediately before the store compiles byte-identically to a literal, so wave 66's 'late-binding' and 'literal' measurements were the same experiment. Configured, 380/380, 2 real code bytes (+0xd5/+0xd6), unchanged. WAVE 90 (W90-B): ported to PR #3 names, PORT-EXACT (98.9%, 2 code bytes at +0xd5/+0xd6 plus 2 .rodata addend bytes). Two 900 s permuter runs with the fixed scorer found nothing: undirected from the port (9,368 iterations) and directed PERM_RANDOMIZE over the tail region from the literal-zero variant (9,699). MECHANISM (from a -dg dump, tools/rtldump.py): both registers are RELOAD registers, handed out round-robin in insn order over {r0,r1,r2,r3}. Port: key reload r2, the spilled zero's reload r3, the ip (rowp) copy r2. Literal: the zero is a local-alloc pseudo in r0 and the ip copy, the next reload after the key's r2, gets r3. The ROM needs the round-robin pointer one step later than either candidate: one more counted reload between the key test and the zero store, or one fewer before the ip copy (e.g. the key compare's `ldr r2,[sp]` not being a reload). Byte-neutral: copying key to a block-local before the compare (cse removes it). A rewrite in sibling sub_0805A388's gMap-> spelling is -24 bytes, a different shape. See work/sub_0805A0EC/NOTES.md.

### Wave 91

W91-B, pre-registered struct Map member test: NEGATIVE. The ->height/->width bounds and ->terrain/->unit/(u8 *)->rowOffset through the q cast are BYTE-IDENTICAL (98.95%). rowp = (u8 *)&((struct Map *)q)->rowOffset[y] gives 95.79%, because the member builds (y*2 + 0x417A) + q and the ROM's (q + 0x417A) + t order needs the byte pointer. Member access off the global with no q (cast or gMap) is 33.68%: the row address hoists into the outer loop. A 900 s permuter run from --current (35,635 iterations) found nothing. Under a temporary -O2 -fno-gcse profile this draft is -16. Residual unchanged: 2 code bytes, the zero-store reload tie. work/sub_0805A0EC/NOTES.md.

### Wave 92

W92-C: unchanged 98.95%, 2 code bytes at +0xd5/+0xd6 (+0x34/+0x164 are .rodata alias addends, not residual). 900 s permuter from --current, 14,676 iterations: nothing. MECHANISM SHARPENED TO ONE DIRECTION: W90-B offered two ('one more reload between the key test and the zero store, OR one fewer before the ip copy'). For the literal-zero form only the second is consistent -- with the literal the zero is a local_alloc pseudo in r0 (ROM-correct) and the ip copy is the NEXT reload after the key test's, landing one register high. So the ROM's round-robin cursor is one step BEHIND this draft's at the ip copy, and the lever is one FEWER reload BEFORE the key test, not an extra one after it. Everything before +0xd5 is byte-exact, so the reload the ROM does not spend must be on an insn this draft also emits -- most plausibly one of the two high-to-low `mov rLO, r8` reads.

### Wave 93

W93-F: one 900 s run from the draft under the old permuter scoring returned nothing; not yet run under the length-penalty scorer.

### Wave 94

W94-B: 900 s / 4 threads / --current under the length-penalised scorer (AW2_PENALTY_SIZE=1000), the first run here under the fixed objective. 34,891 iterations, 1,112 errors, ZERO improving candidates -- nothing beat the draft's objective score of 120, nothing was verified, the draft was not touched (re-checked: 380/380, 98.95%, first difference +0x34; only the +0xd5/+0xd6 pair is code). The scorer fix cannot help this park and the reason generalises: the draft is already size-exact, so the new length term is zero for it and for every size-exact neighbour, and the objective is exactly as blind to a two-byte register-numbering residual as before. The remaining work is the reload-ordering read the wave-92 note sets up (one FEWER scratch handed out before the key test), not another search.

### Wave 96

Base: unchanged draft (98.95% size+0, first +0x34; only +0xd5/+0xd6 real).
Hypothesis (move the zero local's first assignment, not its declaration) tested with the
one-unit harness: 8 placements of `new_var3 = 0;` (after rowp / cells / first guard / props /
third guard / after the volatile block / top / after off), read off the `.s`:
- after rowp, top, after off: zero in r3, ip copy in r2 (current draft; the trade's first half)
- after cells, first guard, third guard: zero in a hi reg copied down (`mov r2,sl` / `mov r2,r8`), ip copy r3
- after the volatile block (just before p bind): `movs r0,#0`, ip copy r3 (= literal; second half)
- after `props = q + 0x12`: zero HELD in sl and copied to r0 (`mov r0,sl; strh r0`) with ip copy in r2 -
  the ROM's two registers, but the zero is held in a hi register instead of `movs r0,#0`. try_match:
  21.58%, pool changes (an extra .rodata word appears, the 0816D980 alias is lost). Not a match.
So the live-range START does not compose the trade: the placements between give either a hi-reg
hold (more copies) or fall to one of the two known halves. Hypothesis refuted as a way to a match;
the two-way trade stands.
Proposed summary: does: lists candidate tiles for the active unit. status: 98.95%, 2 real code bytes.
left: zero store uses r3 instead of r0 (or the next row-pointer copy lands in r3). tried: zero local
placed at eight points, literal zero, dead extras, permuter runs (~50k iterations total).

### Wave 97

wave 97
Base: unchanged draft (98.95%, size-exact; `sub_0805A0EC.w97-start.c`). No improvement.

RTL read (`-da` on the literal-zero form, `sub_0805A0EC.w97-lit.c`): with a literal `out->v = 0` the zero is a plain
`(set (reg 0) (const_int 0))` (insn 285, global-alloc chose r0, same as the ROM). The copy that goes wrong is a RELOAD:
insn 569 `(set (reg 3) (reg ip))` for `ldrh r0,[ip+4]` -- the row pointer got ip from global alloc and reload must
bring it to a low register to use it as a base. The reload insns in order are 560 (r2 <- key slot), 563 (r6), 566 (r0,
the volatile byte compare), 569 (r3). The spill-reg list is 0,1,3,2 (in order of "Spilling reg"), and reload's choice is
round-robin from the last one handed out: after the r0 of 566 the next free one in that list is r3, the ROM's is r2. So
the ROM's previous hand-out was r3 (or the list order differs); i.e. one more reload scratch (a r3) must be consumed
between the compare byte and the store, or the compare byte's reload must use r3 rather than r0.
Measured with the one-unit harness (build/probe/w97h.py): literal zero with the `p`/`new_var2` bind removed, moved
before/after the store, the store spelled through a `s16 *` cast, and the volatile compare swapped or spelled with the long
index: all `mov r3,ip` (unchanged). The long-lived zero local is what removes r3 from the round robin, hence r2 (the known trade).
Untried: a construct that costs the compare byte an r3 reload (a second volatile read, or reading `*new_var` twice).
Proposed summary: status 98.95% size-exact, 2 code bytes; left: row-pointer copy register (r3 vs r2) when the zero is a literal; tried: eight zero-local placements, literal, volatile-compare respellings; mechanism is reload's round-robin over spill regs 0,1,3,2.

wave 97 (second pass)
Base: literal-zero form (`sub_0805A0EC.w97-lit.c`, 98.42%, size-exact, first +0x34). Draft `sub_0805A0EC.c` unchanged (98.95%).
Hypothesis (from the lead): a construct that costs the volatile compare an r3 reload takes r3 first, so the row-pointer reload gets r2.
Measured in one unit (`w97-var.c`, spellings.py), all with the literal zero:
- both tests folded into one `if (props[off] == *new_var && tbl[props[off]].unk0b == 5) continue;`: 93.95%.
- `p` bind removed (use `p->unk00` directly): 98.42% (unchanged). `u = tbl + props[off]` spelling: 98.42%. `{ s16 z = 0; out->v = z; }`: 98.42%.
- `out->v = 0;` moved before the `p` bind: 96.32%.
- `u = &tbl[*new_var]` (compare and index share ONE volatile read): 36.84% (-8 bytes; the ROM has the second props[off] load).
- `int v = *new_var; if (props[off] == v)`: 94.74%.
None changes which scratch register the row-pointer reload gets. Not matched; the two-way trade stands.
Proposed summary addition (tried): folded compare, p bind removed, zero as block-local s16, store moved above the p bind, shared volatile read.

wave 97 (W97-W)
Alias lever (gMap vs gUnknown_08499590) does not apply: the ROM has ONE force-addr word (gUnknown_0816D97C) for the address of gUnknown_08499590, held in sl and reused at every site, so the source used one name. Probes (`w97w0/1/2.c`, one of the three bare uses renamed gMap; gMap is `struct Map *` so it needs a cast): 22.75% +20, 61.20% +4, one compile fail (type). Draft unchanged (98.95%).

wave 97 (W97-PG)
Permuter chain: 1 link (540s), 98.95% -> 98.95%, NO-IMPROVEMENT. Draft unchanged.

</details>
