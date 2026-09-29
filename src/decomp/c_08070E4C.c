#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070E4C.
 * sub_08070E4C @ 0x08070E4C
 */

/* MidiKey2CgbFr. The packed table byte reaches an `int` local, which is why
 * both `>> 4` extractions are `asrs` on a value a bare u8 read would have let
 * agbcc shift unsigned. In the low-key arm fineAdjust is cleared BEFORE key --
 * the ROM materialises a separate zero for ip. */
int MidiKey2CgbFr(u8 chan, u8 key, u8 fineAdjust)
{
    int val1;
    int val2;

    if (chan == 4)
    {
        if (key <= 20)
        {
            key = 0;
        }
        else
        {
            key -= 21;

            if (key > 59)
                key = 59;
        }

        return gUnknown_081B9F8C[key];
    }

    if (key <= 35)
    {
        fineAdjust = 0;
        key = 0;
    }
    else
    {
        key -= 36;

        if (key > 130)
        {
            key = 130;
            fineAdjust = 255;
        }
    }

    val1 = gUnknown_081B9EF0[key];
    val1 = gUnknown_081B9F74[val1 & 0xf] >> (val1 >> 4);

    val2 = gUnknown_081B9EF0[key + 1];
    val2 = gUnknown_081B9F74[val2 & 0xf] >> (val2 >> 4);

    return val1 + ((val2 - val1) * fineAdjust >> 8) + 0x800;
}
asm(".global sub_08070E4C\n.thumb_set sub_08070E4C, MidiKey2CgbFr\n");
