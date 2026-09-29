
## wave 95

Base: existing draft (352/360, -8, 41.9%). Pre-registered hypothesis (wave-94 chapter: the running x must be something the compiler cannot bound) NOT settled in its favour.
- Fold-proof mask on the mask operand, `(((u32)x << 16) & 0xffff0000) >> 16) & 0x1ff` with `u16 x`: 348 (-12), 41.4%. Folds like the earlier spellings (combine discharges it, then the shift pair is gone, nothing is shared with the increment).
- `u32 x` with the increment written as the shift pair `x = ((x << 16) + 0x100000) >> 16` and the mask operand `((x << 16) >> 16) & 0x1ff`: 348 (-12), 43.1% (higher score only because the size moved; first difference still +0xa).
- Mechanism: x's two defs (0x6a and the truncated increment) let nonzero_bits prove x <= 0xffff, so any truncation of x in the mask is deleted by combine; the ROM's shared `lsls r5,r1,#16` therefore needs a definition of x that is NOT single-range, and none is available from the constants in this loop.
Proposed summary left: loop 3 keeps x<<16 once per pass in the ROM; every spelling that keeps the truncation lets combine delete it. Not permuter-run (size -8).
