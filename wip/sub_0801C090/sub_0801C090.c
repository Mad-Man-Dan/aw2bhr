#include "global.h"

/* A sprite template has a count followed by three halfword attributes per
 * OAM entry. The output cursor advances by four halfwords per entry. */
struct SpriteTemplateEntry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
};

/* Copies a counted sprite template to the OAM shadow. When the caller requests
 * horizontal mirroring, the X position is adjusted by the sprite's width. */
void sub_0801C090(s32 x, s32 y, void *template, s32 tileOffset)
{
  u16 *src;
  u16 *dst;
  u32 count;
  u32 remaining;
  u16 attr0;
  u16 attr1;
  u16 sourceAttr0;
  src = template;
  count = *src;
  src++;
  dst = gUnknown_03002F2C;
  remaining = count;
  while (remaining != 0)
  {
    if (x & 0x1000)
    {
      u16 negWidth;
      attr1 = ((struct SpriteTemplateEntry *) src)->attr1;
      remaining = ((((u32) (attr1 >> 14)) * 4) + (((u32) ((sourceAttr0 = ((struct SpriteTemplateEntry *) src)->attr0) >> 14)) * 16)) + ((u32) gUnknown_0848B56C);
      negWidth = -(*((s16 *) remaining));
      remaining = 0x1ff & attr1;
      if (attr1 & 0x100)
      {
        remaining |= 0xffffff00 & 0xffff;
      }
      attr0 = ((y | sourceAttr0) & (~0xff)) | ((sourceAttr0 + y) & 0xff);
      remaining = ((s32) (-(remaining << 16))) >> 16;
      attr1 = ((x | attr1) & (~0x1ff)) | (((remaining + x) + ((s16) negWidth)) & 0x1ff);
    }
    else
    {
      remaining = ((y | ((struct SpriteTemplateEntry *) src)->attr0) & (~0xff)) | (((((struct SpriteTemplateEntry *) src)->attr0 + y) + gUnknown_03002B20) & 0xff);
      attr0 = remaining;
      attr1 = ((x | ((struct SpriteTemplateEntry *) src)->attr1) & (~0x1ff)) | (((((struct SpriteTemplateEntry *) src)->attr1 + x) + gUnknown_030030D0) & 0x1ff);
    }
    *(dst++) = attr0;
    *(dst++) = (attr1 & 0xcfff) | ((x ^ ((struct SpriteTemplateEntry *) src)->attr1) & 0x3000);
    *dst = ((struct SpriteTemplateEntry *) src)->attr2 + tileOffset;
    dst += 2;
    gUnknown_03002F2C = ((u8 *) gUnknown_03002F2C) + 8;
    src += 3;
    remaining = (count * 0x10000) + 0xffff0000;
    count = remaining >> 16;
  }

}
