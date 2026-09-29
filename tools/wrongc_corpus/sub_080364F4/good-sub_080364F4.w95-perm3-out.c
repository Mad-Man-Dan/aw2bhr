#include "global.h"

/* Reseed gPlaySt's per-slot arrays from the chapter record
 * gUnknown_085C77A0[unk02], then start whichever proc-list script that record
 * names.
 *
 * The switch operand is int, not the u8 member: the dispatch tree's `blt` is
 * SIGNED, which a zero-extended byte's range cannot produce.
 *
 * The +0x44 run is read as FOUR CONSECUTIVE BYTES (`base + unk02*0x5c + i`,
 * i unscaled, `ldrb`), i.e. a third array parallel to unk3c and unk40 -- not
 * `unk44[i].unk00`, which would scale the index by 4. That contradicts the
 * wave-41 `struct Unk085C77A0Slot unk44[5]` declaration, so this file reads it
 * through a cast rather than reshaping a member two other functions already
 * use. Recorded, not fixed.
 *
 * PARKED, wave 71 (W71-D): target 296 bytes, candidate 280 (-16).  A
 * controlled private-slot probe named the ROM words at 0x08090EBC and
 * 0x08090EC0 and thereby reproduced the initial extra indirection, but the
 * additional table/base locals forced a 4-byte frame absent from the ROM.
 * This confirms the deficit is base association/allocation rather than
 * missing statements; the established shared struct layout is unchanged.
 */
void sub_080364F4(void)
{
  unsigned char isEmpty;
  int mode;
  u8 *t;
  u8 *d1;
  u8 *d2;
  u8 *d3;
  u8 i;
  sub_080191B0();
  mode = gPlaySt.gameMode;
  switch (mode)
  {
    case 1:

    case 2:
      t = (u8 *) gUnknown_085C77A0;
      for (i = 0; i < 4; i++)
    {
      mode = i + (gPlaySt.mapID * 0x5c);
      gPlaySt.unk42[i + 1] = (d1 = t + 0x44, d1[(gPlaySt.mapID * 0x5c) + (i & 0xFF)]);
      gPlaySt.armyColor[i + 1] = (d2 = t - -0x40, d2[mode]);
      isEmpty = ((d3 = t + 0x3c, d3[(gPlaySt.mapID * 0x5c) + i])) == 0xff;
      if (isEmpty && ((i + 1) <= 3))
      {
        gPlaySt.co[i + 1] = gUnknown_030058D4[i];
      }
      else
      {
 do { gPlaySt.co[i + 1] = d3[(gPlaySt.mapID * 0x5c) + i]; } while (0);
      }
    }

      break;

    case 0:
      for (i = 0; i < 4; i++)
    {
      gPlaySt.co[i + 1] = gUnknown_085C77A0[gPlaySt.mapID].unk3c[i];
    }

      break;

  }

  if ((gUnknown_085C77A0[(&gPlaySt)->mapID].hardcodedUnits != ((void *) 0)) && (gUnknown_085C77A0[gPlaySt.mapID].category <= 2))
  {
    sub_080193B0(gUnknown_085C77A0[gPlaySt.mapID].hardcodedUnits);
  }
  else
  {
    sub_080364E0();
  }
}





