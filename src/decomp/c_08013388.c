#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08013388.
 * sub_08013388 @ 0x08013388, sub_0801339C @ 0x0801339C
 */

struct Unk0801339C
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ u32 unk54;
    /* 0x58 */ u32 unk58;
};

void ScreenShake_OnEnd(void)
{
    gUnknown_030030D0 = 0;
    gUnknown_03002B20 = 0;
}
asm(".global sub_08013388\n.thumb_set sub_08013388, ScreenShake_OnEnd\n");

void ScreenShake_Init(struct Unk0801339C *s)
{
    gUnknown_030030D0 = 0;
    gUnknown_03002B20 = 0;
    s->unk54 = 0;
    s->unk58 = 0;
}
asm(".global sub_0801339C\n.thumb_set sub_0801339C, ScreenShake_Init\n");
