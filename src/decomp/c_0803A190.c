#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A190.
 * sub_0803A190 @ 0x0803A190
 */

/* MATCHED in Wave 74: 300/300 bytes under the configured profile.
 * The recovered allocation needs the byte-sized new_var2 binding below.  The
 * final signed-shift residual is fixed by narrowing only the two call
 * arguments; DrawTallNumberRightAligned keeps its authoritative int first parameter.  Do
 * not tidy this spelling.  See NOTES.md for the measured progression. */
struct Unk0803A190Tbl
{
    void *filler_00[5];
    void *unk14[1];
};

void UnitInfoPanel_LoadPictureDrawMoveAndVision(int a1, struct Unit *a2)
{
  u8 v;
  u8 new_var2;
  int n;
  void *new_var;
  int k;
  s16 x;
  s16 y;
  v = a1;
  new_var2 = gPlayers[gUnknown_0849D89C->unk08].teamColor;
  n = new_var2;
  n = n - 1;
  k = GetPlayerCoCountry(gUnknown_0849D89C->unk08) - 1;
  new_var = ((struct Unk0803A190Tbl *) gUnknown_0849DC18)->unk14[(a2->type * 15) + k];
  Decompress(new_var, gUnknown_0200FD50);
  QueueVBlankCallback((void *) UnitInfoPanel_CopyPictureToVram);
  ApplyPaletteExt(gUnknown_08555D30[a2->type - 1][n], 0x260, 0x40);
  for (y = 0; y < 20; y++)
  {
    for (x = 0; x < 15; x++)
    {
      gBG0TilemapBuffer[((y * 32) + (v / 8)) + x] = 0;
    }
  }

  DrawTallNumberRightAligned((u8)((v + 0x60) / 8), 1, gBG0TilemapBuffer, GetUnitMovementWithCoBonus(gUnknown_0849D89C->unk08, a2->type), 0x8000, 0);
  DrawTallNumberRightAligned((u8)((v + 0x60) / 8), 3, gBG0TilemapBuffer, GetUnitVisionWithCoBonus(gUnknown_0849D89C->unk08, a2->type), 0x8000, 0);
}
asm(".global sub_0803A190\n.thumb_set sub_0803A190, UnitInfoPanel_LoadPictureDrawMoveAndVision\n");
