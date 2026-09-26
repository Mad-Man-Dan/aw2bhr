# sub_08084C14

0x08084C14, 816 bytes, THUMB, parked.

Best score so far: 75.9% (best.c).

## What it does

Per-frame handler of a proc-driven menu with five pages and a selected army. It runs the current page's update, then handles keys: Up/Down change page, Left/Right cycle the army (reloading that army's CO graphics), A or B closes the menu, and R on the last page opens another screen.

## How close it is

One small unfaithfulness first: the draft's `u8 v` truncates the decremented army number to 8 bits, where the original truncates only the army-count call's result (harmless for the values 1 to 4 it holds). Otherwise it compiles to the right size (816 bytes) with every statement, branch, jump table and pool word right; 75.9% of bytes match and the rest is mostly which scratch register each value lands in.

## What is left

Rewrite the Left-key arm so only the army-count result is narrowed (for example a cast on the call instead of `u8 v`) and check it stays the right size. Then re-read the scratch-register differences directly: the old reading that the original keeps the constant 1 in a saved register across the arms is wrong, it does so at one point only.

## Already tried

- The shared default case written last: the jump table's entries come out in the other order; default-first is kept.
- An if/else with two stores in the Left-key arm: the original has one store after a `?:`.
- `p[0x27] = -1` instead of storing 0xFFFF through a u16: loads the wrong constant.
- Dropping the permuter's `t = gPlayers` binding and the empty `do { } while (0)` on case 0: two arms merge and 36 bytes are lost; both are kept.
- A volatile read to reproduce the original's unused load before the 0xFFFF store: right size, but worse (72.8%).
- Pinning the constant 1 to a fixed register: 32 bytes too long, and the proc pointer moves register.
- Automatic permuter, three runs: 39.8% to 75.9%; a 79.2% result held only on header-expanded output, not on the real draft.

## Files

- `sub_08084C14.c`: the current draft
- `best.c`: the closest attempt, when it is not the draft
- `target.s`: the original assembly

## Technical history

<details>
<summary>The full record from `data/parked.json`: every attempt, with compiler detail.</summary>

### Notes

PARKED Wave 71 at exact size 816/816, 75.9%. Every statement, call and pool word is present; residual is scratch allocation around the shared constant and established odd CFG spellings. Prior permuter-derived candidates and header-regression variants are rejected.

WAVE86: WAVE 86 (W86-D, constant-twin axis): twin c_080849C8.c (six shared callees, the shared `unk01 == 2 ? sub_0802490C(...) : sub_080248F8()` idiom) is a same-VOCABULARY neighbour, not a shape twin (520B straight-line init vs 816B jump-table key handler). Its construct -- `register ProcPtr x asm("r9")` alias of a value used at every call site -- transplanted onto the 'shared constant 1 in r8' hunk REGRESSED HARD: 816 -> 848 (+32), every store paid a `mov r1,r8`; restored. This shows the wave-54 park wording 'the shared constant 1 ... out of r8' is a MIS-DESCRIPTION: the ROM holds 1 in r8 at ONE point only and materialises it elsewhere some other way -- re-read that hunk before trusting the entry. Also: a register-asm pin is not a neutral experiment on a function whose parameter competes for callee-saved registers (it cost a register and moved p). Configured, 816/816 size-exact, 75.9% draft, unchanged.

</details>
