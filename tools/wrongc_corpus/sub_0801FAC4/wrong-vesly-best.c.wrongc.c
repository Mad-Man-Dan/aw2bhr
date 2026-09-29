#include "global.h"

void sub_0801FAC4(u16 a1, u16 a2, u16 a3, u16 a4, u8 a5)
{
  int k;
  int x;
  int y;
  int s;
  int e;
  int w;
  k = 1;
  switch (a3)
  {
    case 1:
      for (y = a2; (y > (a2 - a4)) && (y >= 0); k++, y--)
    {
      s = a1 + 1;
      s -= k;
      w = (k * 2) - 1;
      if (s < 0)
      {
        w += s;
        s = 0;
      }
      e = s + w;
      if (e > (*((u16 *) gUnknown_08499590)))
      {
        e = *((u16 *) gUnknown_08499590);
      }
      for (x = s; x < e; x++)
      {
        gUnknown_03003340[y][x] = a5;
      }

    }

      break;

    case 0:
      for (y = a2; (y < (a2 + a4)) && (y < (*((u16 *) (gUnknown_08499590 + 2)))); k++, y++)
    {
      s = a1 + 1;
      s -= k;
      w = (k * 2) - 1;
      if (s < 0)
      {
        w += s;
        s = 0;
      }
      e = s + w;
      if (e > (*((u16 *) gUnknown_08499590)))
      {
        e = *((u16 *) gUnknown_08499590);
      }
      for (x = s; x < e; x++)
      {
        gUnknown_03003340[y][x] = a5;
      }

    }

      break;

    case 2:
      for (x = a1; (x > (a1 - a4)) && (x >= 0); k++, x--)
    {
      s = a2 + 1;
      s -= k;
      w = (k * 2) - 1;
      if (s < 0)
      {
        w += s;
        s = 0;
      }
      ;
      if ((s + w) > (*((u16 *) (gUnknown_08499590 + 2))))
      {
        e = *((u16 *) (gUnknown_08499590 + 2));
      }
      for (y = s; y < e; y++)
      {
        gUnknown_03003340[y][x] = a5;
      }

    }

      break;

    case 3:
      for (x = a1; (x < (a1 + a4)) && (x < (*((u16 *) gUnknown_08499590))); k++, x++)
    {
      s = a2 + 1;
      s -= k;
      w = (k * 2) - 1;
      if (s < 0)
      {
        w += s;
        s = 0;
      }
      e = s + w;
      if (e > (*((u16 *) (gUnknown_08499590 + 2))))
      {
        e = *((u16 *) (gUnknown_08499590 + 2));
      }
      for (y = s; y < e; y++)
      {
        e = x;
        gUnknown_03003340[y][e] = a5;
      }

    }

      break;

  }

}
