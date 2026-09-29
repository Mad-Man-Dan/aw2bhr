#include "global.h"

int sub_0801ECE8(s16 a1, int a2, int a3, int a4, long long a5, volatile int a6)
{
  unsigned int new_var;
  long long v;
  int t;
  t = a6;
  gUnknown_0200ED20[gUnknown_03002510].unk00 = a2;
  gUnknown_0200ED20[gUnknown_03002510].unk02 = a3;
  gUnknown_0200ED20[gUnknown_03002510].unk04 = a4;
  gUnknown_0200ED20[gUnknown_03002510].unk08 = 0;
  new_var = 0x1000;
 do { v = a5 & (~0x2000); v &= ~new_var; gUnknown_0200ED20[gUnknown_03002510].unk0c = v; gUnknown_0200ED20[gUnknown_03002510].unk0a = t; } while (0);
  if (sub_0801A718(&gUnknown_0200ED20[gUnknown_03002510], a1) == (-1))
  {
    return 1;
  }
  gUnknown_03002510++;
  return 0;
}

/* WAVE 86 (W86-E) -- MIRROR/TWIN AXIS. HYPOTHESIS REFUTED, MECHANISM NAMED.
 * See work/sub_0801ECE8/W86-notes.md. 0 try_match spent.
 *
 * Pair class against sub_0801E8D8 (src/decomp/c_0801E8D8.c, vocabulary
 * Jaccard 1.00): TRUE SHAPE TWIN -- its whole body is this skeleton with four
 * field writes instead of six. But it has no long long member and no mask, so
 * it contains NO instance of this residual and can transplant nothing for it.
 *
 * Its one untried construct is the constant written FIRST in a member-store
 * expression (unk04 = 0x80000000 | a4). Probed here as
 * v = ~0x2000 & a5; v = ~0x1000 & v; because the ROM destroys the constant
 * register (ands r1, r5) where this draft destroys the value (and r4, r4, r1).
 * REFUTED: output unchanged, still five pool words against the ROM four, and
 * the -0x1 word still spent on the compare.
 *
 * THE MECHANISM, from agbcc own emitted comment:
 *     ldr r1, .L5+0x8   @ created by thumb_load_double_from_address
 *     ldr r2, .L5+0xc   @ created by thumb_load_double_from_address
 * The 0xFFFFFFFF is emitted by thumb_load_double_from_address, agbcc DImode
 * constant loader, which pulls BOTH words of the CONST_DOUBLE out of the pool
 * as a register pair BEFORE any AND rtx exists. It is not created by the AND
 * expand and not by CSE; CSE only re-spends the pseudo it is handed. So NO
 * source-level respelling of a DImode AND against a negative int constant can
 * avoid it -- one CONST_DOUBLE rtx, an unconditional loader. Folding (w58),
 * parameter self-assignment (w58), interleaving (w81-D), statement split
 * (w81-D) and operand order (w86-E) all behave identically, as predicted.
 * Stop respelling the mask: treat this as kind 3 with a CLOSED mechanism. */
