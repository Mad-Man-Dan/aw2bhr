#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074038.
 * sub_08074038 @ 0x08074038
 */

void sub_08074038(u16 a1, u16 a2, u16 a3, u16 a4, u16 a5, u8 a6, u16 a7)
{
    u16 i;
    u16 n;
    int j;
    int k;

    if (a6 != 0)
        FillTilemapRect(gBG0TilemapBuffer, a4 - 5, a5, 6, 2, 0);

    switch (a3)
    {
    case 0:
        if (a6 != 0)
            DrawTallNumberRightAligned((s16)a4, (s16)a5, gBG0TilemapBuffer, 0, a7, a3);

        break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        i = 0;

        while (i < a3)
        {
            j = i + 1;

            PutOamHi(((a4 - (a3 - i)) * 8 - gUnknown_03001FF8) & 0x1FF,
                a5 * 8, gUnknown_0848B690, a1 | (a2 << 12));

            i = j;
        }

        break;

    default:
        n = 10;
        i = 0;

        if (a3 >= n)
        {
            do
            {
                n *= 10;
                i++;
            }
            while (i <= 9 && a3 >= n);
        }

        k = i + 3;

        PutOamHi(((a4 - k) * 8 - gUnknown_03001FF8) & 0x1FF,
            a5 * 8, gUnknown_0848B690, a1 | (a2 << 12));

        k = i + 1;

        PutOamHi(((a4 - k) * 8 - gUnknown_03001FF8) & 0x1FF,
            a5 * 8 + 6, gUnknown_0848B688, (a1 + 4) | (a2 << 12));

        if (a6 != 0)
            DrawTallNumberRightAligned((s16)a4, (s16)a5, gBG0TilemapBuffer, a3, a7, 0);

        break;
    }
}
