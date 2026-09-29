#include "global.h"

void sub_08039588(u8 *str)
{
    u8 *buf;
    int dst;
    u16 i;
    u16 j;
    u16 k;

    buf = sub_08014E44(0x3000);

    j = 0;

    for (i = 0; str[i] != 0; i++)
    {
        if ((u8)(str[i] - 0x18) > 8)
        {
            dst = j * 0x100 + 0x6140;

            for (k = 0; gUnknown_08090F30[k] != 0; k++)
            {
                if (str[i] == gUnknown_08090F30[k])
                {
                    sub_08011C68(buf + k * 0x100, (void *)(0x06010000 + dst), 0x100);
                    break;
                }
            }

            j++;
        }
    }

    sub_08014ED4(buf);
}
