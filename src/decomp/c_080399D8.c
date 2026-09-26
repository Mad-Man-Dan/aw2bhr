#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080399D8.
 * sub_080399D8 @ 0x080399D8
 */

struct UnkP399D8
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u32 unk30;
};

void sub_080399D8(struct UnkP399D8 *a)
{
    gUnknown_03001FF8 += a->unk2c;
    gUnknown_03001418 += a->unk30;
}
