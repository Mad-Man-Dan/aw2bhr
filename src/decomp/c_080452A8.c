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

void CoPowerDamageHeal_ApplyWeather(struct Unk452A8 *p)
{
    if (p->unk2f != 0xff)
        ChangeGameWeather(p->unk2f);

    ApplyCoPowerStatus();
}
asm(".global sub_080452A8\n.thumb_set sub_080452A8, CoPowerDamageHeal_ApplyWeather\n");
