# sub_08068A00

## wave 97

Base: existing draft (25.51%, 176 B, -20), kept as `sub_08068A00.w97-start.c`. Draft now: the four dead arms
(0x62, 0x58, 0x4e, 0x26) each hold `proc->unk2c += 0; break;` -> 196 B SIZE-EXACT, 44.9%, first diff still +0x6.
- A no-op statement on the switched field keeps all nine case nodes through expansion and is deleted later; a
  `goto done;`, `do { } while (0);` or bare `break;` arm does not (six pivots, 176 B). Unlike wave 36's dead store to
  an unused local, the `+= 0` on the field itself survives.
- Residual: tree is rooted at 0x60 (agbcc's nine-node root, index 4), the ROM roots at 0x4e (index 2). Case RANGES
  count 2 in balance_case_nodes' cost and move the root left (`case 0 ... 1` + `case 0x26 ... 0x27` roots at 0x58,
  +4 B, 9.5%), but the ROM tests plain values, so the ROM's root needs three range-cost nodes among its first three
  without range emission. Not found.
- `+= 0` is probably not the original source; it is a probe that gets the size right.
Proposed summary: does=cutscene step by countdown; status=size-exact, tree root differs; left=ROM roots its compare tree
at 0x4e, draft at 0x60; tried=empty arms, grouped labels, default placements, dead stores, goto arms, `+= 0` arms, ranges.
