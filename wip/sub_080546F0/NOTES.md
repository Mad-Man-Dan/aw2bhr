
## wave 95

Base: the existing draft (`sub_080546F0.w95-start.c`, unchanged, 26.6%, +8). Not improved.
Only hand probes, no permuter (not size-exact; permuter reserved for the size-exact drafts).

- Counter TYPES are not the seed: `u8 i` -864 (-196), `u8 i,j` -228, `int i; u8 j` +12, `s16 i` +32, `u32 i,j` +8 (same as int), `int i; s16 j` +20.
- The frame difference is confirmed as pure allocation: draft and ROM are the same code up to the first
  loop, but the draft puts `i` in r6 and the 0x030045A0 pointer in r7, the ROM the reverse (i in r7, pointer r6).
- `gUnknown_02029664 = 0` as its own byte zero (ROM: `movs r0,#0; strb r0,[r1]`, a second zero): a
  `volatile u8` store and a `u8 tmp8 = 0` temp are both byte-neutral (+8, 26.6%). The byte zero is not
  authorable by a temp: cse still merges it with the halfword zero, or the statement is unchanged.
- One distinct temp per block (W95-A lever): does not apply. There is no reused compare/bound variable here;
  the shared object is the loop counter itself, which the wave-72 split table already measured. Transfer: NO.

Proposed summary: status +8 bytes, 26.6%; left unchanged (the ROM keeps the shared counter in r7 and
the sl/sb/r8 pointer set differs); tried gains: counter types (six spellings), byte-zero temp / volatile store.
