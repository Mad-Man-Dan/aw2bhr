#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08086BF8.
 * sub_08086BF8 @ 0x08086BF8, sub_08086CE0 @ 0x08086CE0
 */

/* MATCHES.  Wave 46, W46-L.
 *
 * PROMOTION MUST CARRY THE POOL WORD.  try_match reports
 *   relocs: name different symbols that resolve to the same address
 *     +0x098  original gUnknown_081D943C  candidate .rodata
 * which is a match, not a difference: gUnknown_081D943C is not an object, it is
 * agbcc's own -fforce-addr address constant for the `u16 *` gBG0TilemapBuffer
 * (the word at 0x081D943C in baserom.gba is 0x08499578).  This function's
 * data/promoted.json entry must carry
 *   "rodata": ["0x081D943C"]
 * and tools/split_rodata.py and tools/gen_lds.py must be re-run before building.
 * Do NOT declare gUnknown_081D943C as a global.
 *
 * The two levers that closed this (and its twin DrawMapListRowsFromThird) are written up in
 * docs/agbcc-codegen.md, "The ROM's y-offset is an EXPLICIT SOURCE BIV": the
 * `+2` counter is a strength-reduction giv and needs `int k = i * 2;` declared
 * in the ARM'S OWN BLOCK (function scope does not reduce), and the sum needs a
 * third local `z` so expand_binop does not swap the ADDS operands.
 */

void DrawMapListFirstTwoRows(u32 a1, int a2, int a3)
{
    int i;
    int flag;
    int y;
    int z;
    u8 t;

    ClearBg0Tilemap();

    PutTextTableEntryImmediate(1, 5, gBG0TilemapBuffer, gUnknown_08499CE4[gUnknown_0300596C], 0, 0);

    if (gUnknown_02027F74.unk37 < a2)
        a2 = gUnknown_02027F74.unk37 + 1;

    if (a2 > 2)
        a2 = 2;

    for (i = 0; i < a2; i++)
    {
        t = HasMapBeenPlayed(gUnknown_02027F74.unk04[a1 + i]);
        flag = 1;
        if (t)
            flag = 0;

        if (gUnknown_02027F74.unk04[a1 + i] <= 0xb3
            && (u8)IsCampaignMapUnlocked(gUnknown_02027F74.unk04[a1 + i]) == 0)
        {
            int k = i * 2;
            y = a3 + 9;
            z = k + y;
            PutTextScriptImmediate(1, (s16)z, gBG0TilemapBuffer, gUnknown_084C3F50, 0, flag);
        }
        else
        {
            u8 *p = GetMapName(gUnknown_02027F74.unk04[a1 + i]);
            int k = i * 2;
            y = a3 + 9;
            z = k + y;
            PutTextScriptImmediate(1, (s16)z, gBG0TilemapBuffer, p, 0, flag);
        }
    }
}
asm(".global sub_08086BF8\n.thumb_set sub_08086BF8, DrawMapListFirstTwoRows\n");

/* MATCHES.  Wave 46, W46-L.  Twin of DrawMapListFirstTwoRows; relocs match, nothing to
 * place at promotion.
 *
 * The asymmetry between the two arms is REAL and load-bearing, not an artefact:
 * the then-arm writes `y = a3 + 9;` and the else-arm writes `y = a3; y = y + 9;`.
 * Both spellings emit identical instructions, but the extra statement boundary
 * in one arm alone changes pseudo creation order and is what puts a2 in r4 and
 * the &gUnknown_02027F74 pseudo in r5 rather than the other way round.  It was
 * found by decomp-permuter (11 iterations) after every hand lever failed; see
 * docs/agbcc-codegen.md, "BOTH FUNCTIONS CLOSED (wave 46, W46-L)".  Do not
 * "tidy" the two arms into the same shape -- it un-matches the function.
 */

void DrawMapListRowsFromThird(u32 a1, int a2, int a3)
{
  int i;
  int flag;
  int y;
  int z;
  u8 t;
  if (gUnknown_02027F74.unk37 < a2)
  {
    a2 = gUnknown_02027F74.unk37 + 1;
  }
  BG_EnableSyncBG0();
  if (a2 > 2)
  {
    for (i = 2; i < a2; i++)
    {
      t = HasMapBeenPlayed(gUnknown_02027F74.unk04[a1 + i]);
      flag = 1;
      if (t)
      {
        flag = 0;
      }
      if ((gUnknown_02027F74.unk04[a1 + i] <= 0xb3) && (((u8) IsCampaignMapUnlocked(gUnknown_02027F74.unk04[a1 + i])) == 0))
      {
        int k = i * 2;
        y = a3 + 9;
        z = k + y;
        PutTextScriptImmediate(1, (s16) z, gBG0TilemapBuffer, gUnknown_084C3F50, 0, flag);
      }
      else
      {
        u8 *p = GetMapName(gUnknown_02027F74.unk04[a1 + i]);
        int k = i * 2;
        y = a3;
        y = y + 9;
        z = k + y;
        PutTextScriptImmediate(1, (s16) z, gBG0TilemapBuffer, p, 0, flag);
      }
    }

  }
}
asm(".global sub_08086CE0\n.thumb_set sub_08086CE0, DrawMapListRowsFromThird\n");
