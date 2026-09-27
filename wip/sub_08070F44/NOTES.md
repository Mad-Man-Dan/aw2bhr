# sub_08070F44 — CgbModVol, sound library (104 bytes)

## Wave 93 (W93-A): 68.27% -> 75.00% under `--profile old-agbcc`, size-exact

The park said "the instruction stream and the instruction order are identical —
only the register numbers differ". That was wrong; two order facts were left.

1. **The multiply's operands are the other way round.** The ROM loads unk06
   (+0x06) BEFORE envelopeGoal (+0x0a) and copies unk06 into the product
   register. agbcc evaluates the SECOND operand of a multiply first, so
   `chan->unk06 * chan->envelopeGoal` emits them in the ROM's opposite order.
   Writing `chan->envelopeGoal * chan->unk06` is worth 73.08 -> 75.00.
2. **The tail re-uses the two volume locals for the pan mask.** 68.27 -> 73.08.
   This was already sitting in `best.c` and nobody had installed it. It works
   because it lengthens those locals' live ranges. Its statement order is the
   reverse of the ROM's load order, and that is correct — putting the panMask
   read first is 71.15%.

Byte-neutral, measured: re-using the locals for the multiply's operands too;
declaring `left` before `right`; `right >> 1` instead of `right / 2`.

## Residual: 26 of 104 bytes, one hard register

The ROM keeps `chan` in r1 and the `rightVolume << 24` temporary in r2; the
candidate has them swapped, and the sum's operand ownership follows from that.

Read as global.c's `allocno_compare`, priority
`floor_log2(refs)*refs/live_length` with ties broken by the LOWER allocno
number: `chan` is the parameter, so its allocno is created first and wins any
tie — that is the ROM. For the candidate to lose, the `<<24` temporary's
priority has to be strictly higher, which it is: 3 refs over 7 insns against
chan's function-long range. The lever would lengthen that temporary's range or
shorten chan's, and neither is spellable — the temporary is agbcc's own
zero-extension of a volatile QImode load.

## Tooling gap that blocks the obvious next step

`tools/permute.py` calls `agbenv.flags(name)` with no profile argument
(permute.py:209), while `agbenv.flags(fn, profile)` does take one. This
function has no `compiler-overrides.json` entry, so a permuter run would score
against the DEFAULT toolchain while the function needs old_agbcc, and nothing
it found would transfer. A `--profile` passthrough would make this function
permutable — which matters, because the twin case this wave, sub_08073480
(size-exact, pure register-name residual), CLOSED on a 900 s permuter run.

## Wave 93 (W93-F): the profile gap is closed, and the answer is negative

`tools/permute.py` now takes `--profile`, so this function can be searched
under the compiler it actually needs. It was, for the first time:

    python tools/permute.py sub_08070F44 --profile old-agbcc --seconds 900 \
        --threads 4 --current

43,742 iterations, 47 outputs, the best 40 checked against the real bytes.
**Nothing beat the starting point.** The permuter's own baseline line confirms
the draft independently: 75.00%, size-exact, first difference at +0x2.

### The search objective points the wrong way on this function

This is the useful result, and it is not a near miss — it is an inversion.
The starting point scores 2100 on the permuter's objective. Every one of the
40 candidates checked scored *better* on that objective (620 to 1940) and
matched *worse* on the real bytes:

    permuter score   real match
      620 (best)       45.19%, 4 bytes short
      820              56.73%, size-exact
     1240              11.54%, size-exact
     1900              62.50%, size-exact
     2100 (the draft)  75.00%, size-exact

The objective rewards shrinking the function, and this function's residual is
one register name in a body that is already the right length, so every step
the search calls progress moves away from the answer. Chaining more runs
cannot help: the direction is wrong, not the distance.

So the lever the wave-93 notes above hoped for does not exist as the tool is
scored today. Either the objective needs a size-exactness term strong enough
to dominate on a 104-byte body, or this function needs the allocator read
directly. Do not spend another undirected run here.

One note on the older record: the parked entry credits a permuter run "with
the older compiler" in an earlier wave, reaching 73%. That cannot be what it
says it is — before this wave `permute.py` had no way to select a compiler and
always used the configured one. Treat that line as a default-compiler run.

## Wave 94 (W94-B): the fixed scorer keeps the length, and still does not find it

First campaign under the length-penalised objective (`AW2_PENALTY_SIZE=1000`),
`--profile old-agbcc --current` from the 75.00% draft. 32,175 iterations,
40 outputs kept.

**Every one of the 40 was re-verified by hand afterwards**, one at a time,
against the real bytes under `old-agbcc` (the run's own verify phase had been
contaminated — see the incident note below — so its numbers were thrown away).
Result: best candidate **69.23%**, and nothing beat the draft.

The scorer fix did change the search, measurably:

- **40 of 40 kept outputs are size-exact.** Under the old objective W93-F's
  best candidate was 4 bytes short and the whole top of the list was shorter
  than the target. The length drift is gone.
- **The inversion is not.** All 40 score BETTER on the permuter objective
  (1340-2080 against the draft's 2100) and match WORSE on the bytes. So
  holding the length fixed is necessary and not sufficient: with the length
  constant the objective still ranks a different register assignment above the
  right one, because a register mismatch and an instruction it happens to line
  up with weigh the same to it.

Read together with W93-F, this closes the undirected search on this function
for a second, independent reason. The residual is a two-allocno priority tie
(see above); the randomizer's moves do not reach it and the objective cannot
rank it.

### Incident: two concurrent permute.py runs on one function strand cpp output

Worth knowing because it produces exactly the corruption the wave brief warns
about, from a direction the brief does not mention.

A backgrounded launch wrapper reported "failed, exit 1" while the
`tools/permute.py` it started was still running. A second run was then started
on the same function, writing the same log name and sharing
`work/<fn>/permuter/`. Both reached their verify phase together. Verify leaves
each candidate in `work/<fn>/<fn>.c` while trymatch judges it, and reads the
form to restore (`orig`, permute.py:564) at the START of verify -- so run B
read run A's header-expanded candidate as "the draft" and faithfully restored
203 KB of `cpp -P` output over the real draft, while printing "restored
unchanged".

Two consequences: (1) every percentage in a raced verify phase is unreliable,
because the file being compiled may be the other run's candidate; (2) the draft
is silently replaced by expanded source that still compiles.

So: never run two permuters on one function, and do not trust a launch
wrapper's exit status as evidence that the run died -- check that
`work/<fn>/permuter/permuter.log` has stopped growing over a minute, not
over ten seconds (it writes one snapshot a minute by design).
