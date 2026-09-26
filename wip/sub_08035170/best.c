#include "global.h"

u8 sub_08035170(void)
{
  struct PlaySt *new_var;
  int v;
  u8 mode;
  v = gPlaySt.weather;
  switch (v)
  {
    default:
      v = 0;

    case 0:
      new_var = &gPlaySt;
      if ((*new_var).randomWeatherOn == 1)
    {
      if (sub_080129F8(gUnknown_03004490[2]))
      {
        v = 1;
      }
      else
        if (sub_080129F8(gUnknown_03004490[1]))
      {
        v = 2;
      }
    }
      break;

    case 1:

    case 2:
      mode = (*new_var).randomWeatherOn;
      if (((mode != 2) && ((v != gPlaySt.defaultWeather) || (mode != 3))) && sub_08035080())
    {
      v = (*new_var).defaultWeather;
    }
      break;

  }

  return v;
}
