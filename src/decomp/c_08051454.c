#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08051454.
 * sub_08051454 @ 0x08051454
 */

#include "hardware.h"
struct Unk85D6A48Row /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u8 filler_0a[0x0a];
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u8 filler_16[0x02];
};

void MissileHitEffect_Init(void)
{
  unsigned int new_var;
  struct OamData oam;
  s16 dx;
  s16 dy;
  u16 x;
  u16 y;
  int mem_x;
  int mem_y;
  struct Unk85D6A48Row *tbl;
  CopySlotSpriteAttrs(gUnknown_03001FBC, (struct UnkVec *) (&oam));
  gUnknown_020298E0[gUnknown_0300453C].unk80[gUnknown_020298E0[gUnknown_0300453C].unk16 - 1] = 0;
  gUnknown_020298E0[gUnknown_0300453C].unk8a = gUnknown_08553660[1] / gUnknown_08553664[1];
  gUnknown_03001470[gUnknown_03001FBC].unk28 = 0;
  gUnknown_03001470[gUnknown_03001FBC].unk30 = gUnknown_0300453C;
  gUnknown_03001470[gUnknown_03001FBC].unk34 = gUnknown_020298E0[gUnknown_0300453C].unk16 - 1;
  oam.priority = 3;
  oam.hFlip = gUnknown_0300453C;
  oam.paletteNum = 8;
  oam.tileNum = (new_var = gUnknown_02029710[gUnknown_0300453C ^ 1].unk00);
  if ((gUnknown_03004580[gUnknown_0300453C ^ 1][1] == 0xE) || (gUnknown_03004580[gUnknown_0300453C ^ 1][1] == 0xF))
  {
    PlayMusicOrSfx(gUnknown_085643A8[0][gUnknown_020298E0[gUnknown_0300453C].unk8c & 1]);
  }
  if (((tbl = (struct Unk85D6A48Row *)gUnknown_085D6A48,
        tbl[gUnknown_03004580[gUnknown_0300453C ^ 1][1]].unk02) == 2) && ((gUnknown_03004580[gUnknown_0300453C ^ 1][1] == 0x17) || (gUnknown_03004580[gUnknown_0300453C ^ 1][1] == 0x11)))
  {
    SetDefaultColorEffects();
    gUnknown_030030E0.bits.effect = 1;
    gUnknown_030030E0.bits.target2_enable_obj = 1;
    gUnknown_030030E0.bits.target2_enable_bg2 = 1;
    gUnknown_030030E0.bits.target2_enable_bg3 = 1;
    gUnknown_03002020 = 0xF;
    gUnknown_03002B28 = 0xA;
    oam.objMode = 1;
  }
  SetSlotSpriteAttrs(gUnknown_03001FBC, *((struct UnkVec *) (&oam)));
  asm("" : : "r"(gUnknown_08553C18));
  asm("" : : "r"(gUnknown_085D6A48));
  dx = gUnknown_08553C18[((struct Unk85D6A48Row *)gUnknown_085D6A48)[((struct Unk85D6A48Row *)gUnknown_085D6A48)[gUnknown_03004580[gUnknown_0300453C ^ 1][1]].unk00].unk14].unk16;
  dy = gUnknown_08553C18[((struct Unk85D6A48Row *)gUnknown_085D6A48)[((struct Unk85D6A48Row *)gUnknown_085D6A48)[gUnknown_03004580[gUnknown_0300453C ^ 1][1]].unk00].unk14].unk1c;
  if (gUnknown_0300453C == 0)
  {
    dx = -dx;
  }
  if (((struct Unk85D6A48Row *) gUnknown_085D6A48)[gUnknown_03004580[gUnknown_0300453C][1]].unk04 == 0)
  {
    x = (mem_x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x) + dx;
    y = (gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y + gUnknown_08553BFC[gUnknown_020298E0[gUnknown_0300453C].unk18].unk04) + dy;
    gUnknown_020298E0[gUnknown_0300453C].unk18++;
    if (gUnknown_020298E0[gUnknown_0300453C].unk18 == 3)
    {
      gUnknown_020298E0[gUnknown_0300453C].unk18 = 0;
    }
  }
  else
  {
    x = (mem_x = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].x) + dx;
    y = (mem_y = gUnknown_02029A10[gUnknown_0300453C].entries[gUnknown_0300451C].y) + dy + gUnknown_08553668[gUnknown_020298E0[gUnknown_0300453C].unk16 - 1];
    gUnknown_020298E0[gUnknown_0300453C].unk18++;
  }
  gUnknown_020298E0[gUnknown_0300453C].unk44[gUnknown_020298E0[gUnknown_0300453C].unk16 - 1] = x;
  gUnknown_020298E0[gUnknown_0300453C].unk4e[gUnknown_020298E0[gUnknown_0300453C].unk16 - 1] = y;
  SetEffectScreenPosition(gUnknown_0300453C, gUnknown_03001FBC, x, y);
}
asm(".global sub_08051454\n.thumb_set sub_08051454, MissileHitEffect_Init\n");
