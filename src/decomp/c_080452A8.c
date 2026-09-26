#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080452A8.
 * sub_080452A8 @ 0x080452A8
 */

struct Unk452A8
{
    /* 0x00 */ u8 filler_00[0x2f];
    /* 0x2f */ u8 unk2f;
};

void sub_080452A8(struct Unk452A8 *p)
{
    if (p->unk2f != 0xff)
        ChangeGameWeather(p->unk2f);

    sub_08039F58();
}
