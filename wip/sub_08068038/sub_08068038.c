#include "global.h"

/* PARKED at 36.6%, +4 bytes (176 vs 172). Wave 59, W59-E.
 * First difference at +0xc, in the frame.
 *
 * Renders a string one glyph at a time: for each character of `str`, scan the
 * gUnknown_08581160 table (stride 8: `chr` at +0, `data` at +4) from the
 * position held in the GLOBAL cursor gUnknown_0202F208 until the glyph matches
 * either exactly or with 0x20 added, decompress it to 0x06010000 + i * 512 +
 * a2 * 32, reset the cursor and advance. A 0xff `chr` is the end-of-table
 * sentinel and falls back to entry 12's data. Returns the number of glyphs
 * emitted. THE SCAN CURSOR REALLY LIVES IN MEMORY -- it is loaded at the top of
 * every inner iteration and stored back on increment; no local reproduces that.
 *
 * WHAT IS ALREADY BYTE-EXACT IN PLACE: the whole control-flow skeleton --
 * the outer test, the inner scan loop, the `goto advance` re-entry, the
 * short-circuit `||`, the CROSS-JUMP MERGE that gives both arms one shared
 * `bl Decompress`, the `ldr r0,[r0,#0x64]` fold for the sentinel's entry-12
 * data, and the epilogue. Every branch and every constant agrees.
 *
 * REMAINING DIFF -- one allocation decision, and everything else follows it:
 *
 *     ROM    str -> STACK SLOT (`str r0,[sp]`, reloaded at all three uses)
 *            a2 * 32 -> sb        &gUnknown_08581160 -> sl
 *            &gUnknown_0202F208 -> r8 (held all function)   i -> ip
 *     here   str -> sl            a2 * 32 -> stack slot
 *            &gUnknown_0202F208 rematerialised after the call, not held
 *
 * The pool word `=gUnknown_08581164` is NOT a separate global and NOT a second
 * table: `&tbl[g].data` is `(g * 8) + (base + 4)`, and which of the two
 * spellings agbcc picks is decided by where the base landed. With the base in a
 * HI register (the ROM, sl) there is no `adds rD,rHI,#4`, so it forces a fresh
 * pool word holding base+4; with the base in a LOW register (this candidate,
 * r6) CSE reaches it with `adds r0,r6,#4`. The pool word and the register
 * assignment are ONE fact -- do not chase the pool word.
 *
 * AXIS RULED OUT, measured this wave, and it points the opposite way to the
 * usual advice: DROPPING the `next` and `dst` binding locals (writing `i++`
 * after the call and the destination expression inline at the call) is much
 * WORSE -- 156 bytes (-16), 11.6%. Without them gcc strength-reduces BOTH
 * walks: `str` becomes a walking pointer (`adds r4,#1`) and the destination
 * becomes a giv (`movs r0,#128; lsls r0,#2; adds r6,r6,r0`). The ROM carries
 * NEITHER -- it recomputes `str + i` and `i * 512` from the index on every
 * outer iteration. This is docs/agbcc-codegen.md's sub_0800CAA0 direction of
 * the induction-variable axis: the CANDIDATE carrying givs the ROM does not.
 * So these two locals are load-bearing and this draft is the better one. */

int sub_08068038(u8 *str, u16 a2)
{
  const struct Unk08581160Entry *new_var;
  int i;
  int next;
  int dst;
  u8 *src;
  i = 0;
  gUnknown_0202F208 = 0;
  new_var = gUnknown_08581160;
  while (str[i] != 0)
  {
    dst = 0x06010000 + (i * 512);
    next = i + 1;
    scan:
    if (new_var[gUnknown_0202F208].chr == 0xff)
    {
      src = gUnknown_08581160[12].data;
    }
    else
      if ((str[i] == new_var[gUnknown_0202F208].chr) || (str[i] == (new_var[gUnknown_0202F208].chr + 0x20)))
    {
      src = gUnknown_08581160[gUnknown_0202F208].data;
    }
    else
    {
      goto advance;
    }

    Decompress(src, (void *) (dst + (a2 * 32)));
    i = next;
    gUnknown_0202F208 = 0;
    continue;
    advance:
    gUnknown_0202F208++;

    goto scan;
  }

  return i;
}
