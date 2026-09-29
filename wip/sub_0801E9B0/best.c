#include "global.h"

int sub_0801E9B0(s16 a1, s16 a2, s16 a3, void *a4, long long a5, s16 a6)
{
  u16 *p;
  u16 *dst;
  u16 *t;
  s16 n;
  s16 i;
  u16 t0;
  u16 t1;
  u16 t2;
  int sx;
  int sy;
  int flag;
  int q0;
  int q1;
  int q2;
  int v0;
  int v1;
  u16 h;
  int x;
  int y;
  int prio;
  int dx;
  int dy;
  int ex;
  int ey;
  sx = (sy = 0x100);
  flag = 0;
  p = (u16 *) a4;
  n = *p;
  p++;
  if (((a1 + n) + 1) > (0x80 + 1))
  {
    return 1;
  }
  gUnknown_0300054E = a6;
  if (a6 != (-1))
  {
 do { sx = gUnknown_0200E438[gUnknown_03001470[a6].unk26].unk3c; sy = gUnknown_0200E438[gUnknown_03001470[a6].unk26].unk3e; } while (0);
  }
  t = (u16 *) (&a5);
  t0 = t[0];
  t1 = t[1];
  t2 = t[2];
  dst = &gUnknown_03002520[a1 * 4];
  for (i = 0; i <= (n - 1); i++)
  {
    q0 = p[0];
    q1 = p[1];
    q2 = p[2];
    v0 = ((q0 & 0xFF00) + (t0 & 0xFF00)) | (((q0 & 0xFF) + (t0 & 0xFF)) & 0xFF);
    gUnknown_03000548.unk00 = v0;
    v1 = (((q1 & 0xCE00) + (t1 & 0xCE00)) | (((q1 & 0x1FF) + (t1 & 0x1FF)) & 0x1FF)) | ((q1 ^ t1) & 0x3000);
    gUnknown_03000548.unk02 = v1;
    gUnknown_03000548.unk04 = q2 + t2;
    prio = v1 & 0x3000;
    if (t0 & 0x100)
    {
      prio = 0;
      flag = 1;
    }
    y = v0 & 0x7F;
    if (v0 & 0x80)
    {
      y |= -0x80;
    }
    if ((flag == 0) && (t1 & 0x2000))
    {
      y = -(y + gUnknown_0848B6F8[(v0 & 0xC000) >> 14][(v1 & 0xC000) >> 13]);
    }
 dummy_label_439172: ;
    h = gUnknown_03000548.unk02;
    x = *((u8 *) (&gUnknown_03000548.unk02));
    if (h & 0x100)
    {
      x |= -0x100;
    }
    if ((flag == 0) && (t1 & 0x1000))
    {
      x = -(x + gUnknown_0848B6F6[((gUnknown_03000548.unk00 & 0xC000) >> 5) >> 9][(h & 0xC000) >> 13]);
    }
    if (flag != 0)
    {
      if (t0 & 0x200)
      {
        const s16 (*widths)[8];
        widths = gUnknown_0848B6F6;
        dx = widths[(gUnknown_03000548.unk00 & 0xC000) >> 14][(h & 0xC000) >> 13];
        x = x + dx;
        dy = gUnknown_0848B6F8[(gUnknown_03000548.unk00 & 0xC000) >> 14][(h & 0xC000) >> 13];
        y = y + dy;
        x = (x * ((s16) sx)) / 256;
        y = (y * ((s16) sy)) / 256;
        x = (x + a2) - dx;
        y = (y + a3) - dy;
      }
      else
      {
        ex = gUnknown_0848B6F6[(gUnknown_03000548.unk00 & 0xC000) >> 14][(h & 0xC000) >> 13] / 2;
        x = x + ex;
        ey = gUnknown_0848B6F8[(gUnknown_03000548.unk00 & 0xC000) >> 14][(h & 0xC000) >> 13] / 2;
        y = y + ey;
        x = (x * ((s16) sx)) / 256;
        y = (y * ((s16) sy)) / 256;
        x = (x + a2) - ex;
        y = (y + a3) - ey;
      }
    }
    else
    {
      y = y + a3;
      x = x + a2;
    }
    gUnknown_03000548.unk00 = (gUnknown_03000548.unk00 & 0xFF00) | (y & 0xFF);
    gUnknown_03000548.unk02 = ((gUnknown_03000548.unk02 & 0xFE00) | (x & 0x1FF)) | prio;
    sub_080169A4(a6, &gUnknown_03000548);
    *(dst++) = gUnknown_03000548.unk00;
    *(dst++) = gUnknown_03000548.unk02;
    *dst = gUnknown_03000548.unk04;
    p += 3;
    dst += 2;
  }

  return 0;
}





