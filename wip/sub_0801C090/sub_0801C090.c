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
  u16 *dst;
  u32 count;
  long long nextCount;
  u32 remaining;
  int attr0Hi;
  u16 attr0;
  int new_var;
  u16 attr1;
  u16 sourceAttr0;
  count = *((u16 *) template);
  new_var = ~0xff;
  template = ((u16 *) template) + 1;
  dst = gUnknown_03002F2C;
  remaining = count;
  while (remaining != 0)
  {
    if (x & 0x1000)
    {
      u16 negWidth;
      s32 neg;
      s32 sum;
      u16 hi;
      int sa1;
      sa1 = ((struct SpriteTemplateEntry *) template)->attr1;
      remaining = (((((u32) sa1) >> 14) * 4) + (((u32) ((sourceAttr0 = ((struct SpriteTemplateEntry *) template)->attr0) >> 14)) * 16)) + ((u32) gUnknown_0848B56C);
      negWidth = -(*((s16 *) remaining));
      remaining = 0x1ff & sa1;
      if (sa1 & 0x100)
      {
        remaining |= 0xffffff00;
        remaining = (u16) remaining;
      }
      neg = -(remaining << 16);
      attr0Hi = (y | sourceAttr0) & (~0xff);
      attr0 = attr0Hi | ((sourceAttr0 + y) & 0xff);
      hi = (x | sa1) & (~0x1ff);
      sum = (neg >> 16) + x;
      sum += (s16) negWidth;
      remaining = sum;
      attr1 = hi | (remaining & 0x1ff);
    }
    else
    {
      remaining = ((y | ((struct SpriteTemplateEntry *) template)->attr0) & new_var) | (((((struct SpriteTemplateEntry *) template)->attr0 + y) + gUnknown_03002B20) & 0xff);
      attr0 = remaining;
      attr1 = ((x | ((struct SpriteTemplateEntry *) template)->attr1) & (~0x1ff)) | (((((struct SpriteTemplateEntry *) template)->attr1 + x) + gUnknown_030030D0) & 0x1ff);
    }
    *(dst++) = attr0;
    *(dst++) = (attr1 & 0xcfff) | ((x ^ ((struct SpriteTemplateEntry *) template)->attr1) & 0x3000);
    *dst = ((struct SpriteTemplateEntry *) template)->attr2 + tileOffset;
    dst += 2;
    gUnknown_03002F2C = ((u8 *) gUnknown_03002F2C) + 8;
    template = ((u16 *) template) + 3;
    nextCount = (count * 0x10000) + 0xffff0000;
    remaining = nextCount;
    count = remaining >> 16;
  }

}
