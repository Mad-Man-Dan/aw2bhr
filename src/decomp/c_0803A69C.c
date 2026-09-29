#define sub_0803EED4 sub_0803EED4_u8
#include "global.h"
#undef sub_0803EED4

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A69C.
 * sub_0803A69C @ 0x0803A69C
 */

#include "hardware.h"
u16 sub_0803EED4(int, int);

/* WAVE 62: improved from 79.5% to 97.8% (596/596 total bytes) by binding
 * unk02, unk03 and the map pointer in source order, then binding `col = x +
 * dx` after the pointer load. The clear loop now has the ROM's instruction
 * order; its residual is only a low-register swap. The separate final-call
 * u8-versus-u16 narrowing remains the cross-TU prototype conflict documented
 * in NOTES.md. Code is still two bytes short and recovered as padding.
 *
 * PARKED at 79.5%, SIZE-EXACT (596/596). Wave 50, W50-B.
 * See work/UnitClassInfo_Loop/NOTES.md for the exact remaining diff.
 *
 * THE STRUCTURAL FINDING, worth more than the score: both stepper loops are
 * `body; while (cond) { body; }`, NOT `do { body } while (cond);`. The ROM
 * emits each body TWICE -- a peeled copy, then the loop copy at _0803A700 /
 * _0803A78E. The do/while spelling emits it once and came out 96 bytes SHORT
 * (11.1%); duplicating it landed the size exactly and took the function to
 * 79.5% in one attempt.
 *
 * gUnknown_0849D89C->unk0a is read `movs r1,#0xa; ldrsb r1,[r2,r1]` -- SIGNED
 * -- while include/unknown-globals.h declares it `u8`. The member is NOT
 * retyped here (a dozen promoted files share the struct); the cast below is
 * byte-identical to a declared s8, because sign_extend of a QImode mem folds
 * to ldrsb either way. The evidence is recorded on the member itself. */

void UnitClassInfo_Loop(void)
{
  u8 x;
  u8 y;
  u8 dy;
  u8 dx;
  int col;
  int new_var;
  u16 *map;
  StepMapCursorAndDraw(2);
  if (gpKeySt->repeated & (DPAD_LEFT | DPAD_UP))
  {
    PlayMusicOrSfx2(0x67);
    gUnknown_0849D89C->unk09--;
    if (gUnknown_0849D89C->unk09 < 0)
    {
      gUnknown_0849D89C->unk09 = 0xf;
    }
    while (gUnknown_0849E398[gUnknown_081BA068[gUnknown_0849D89C->unk04->type] - 1][gUnknown_0849D89C->unk09] == 0)
    {
      gUnknown_0849D89C->unk09--;
      if (gUnknown_0849D89C->unk09 < 0)
      {
        gUnknown_0849D89C->unk09 = 0xf;
      }
    }

  }
  if (gpKeySt->repeated & (DPAD_RIGHT | DPAD_DOWN))
  {
    PlayMusicOrSfx2(0x67);
    gUnknown_0849D89C->unk09++;
    if (gUnknown_0849D89C->unk09 > 0xf)
    {
      gUnknown_0849D89C->unk09 = 0;
    }
    while (gUnknown_0849E398[gUnknown_081BA068[gUnknown_0849D89C->unk04->type] - 1][gUnknown_0849D89C->unk09] == 0)
    {
      gUnknown_0849D89C->unk09++;
      if (gUnknown_0849D89C->unk09 > 0xf)
      {
        gUnknown_0849D89C->unk09 = 0;
      }
    }

  }
  if (((s8) gUnknown_0849D89C->unk0a) != gUnknown_0849D89C->unk09)
  {
    gUnknown_0849D89C->unk0a = gUnknown_0849D89C->unk09;
    sub_08014878();
    for (y = 0; y <= 7; y++)
    {
      for (x = 0; x <= 0xd; x++)
      {
        new_var = 32;
        dx = gUnknown_0849D89C->unk02;
        dy = gUnknown_0849D89C->unk03;
        map = gBG0TilemapBuffer;
        col = x + dx;
        map[(col + ((y + dy) * new_var)) + 0x21] = 0;
      }

    }

    sub_08013AD4(0);
    sub_080146D4(gUnknown_0849D89C->unk02 + 1, gUnknown_0849D89C->unk03 + 1, gBG0TilemapBuffer, gUnknown_0849E398[gUnknown_081BA068[gUnknown_0849D89C->unk04->type] - 1][gUnknown_0849D89C->unk09], 0x8000, 0xf8);
  }
  else
    if (gpKeySt->pressed & 2)
  {
    PlayMusicOrSfx2(0x66);
    sub_08014878();
    EndUnitInfoPanelScripts();
    sub_0801537C(gUnknown_0849E2C0);
    ClearSlotScriptCallback(gUnknown_03001FBC);
  }
  else
    if (gpKeySt->pressed & R_BUTTON)
  {
    sub_08014878();
    EndUnitInfoPanelScripts();
    sub_0801537C(gUnknown_0849E2C0);
    ClearSlotScriptCallback(gUnknown_03001FBC);
    ShowTerrainInfoWindow(sub_0803EED4(gUnknown_030033E4.unk00, gUnknown_030033E4.unk02));
  }
}
asm(".global sub_0803A69C\n.thumb_set sub_0803A69C, UnitClassInfo_Loop\n");
