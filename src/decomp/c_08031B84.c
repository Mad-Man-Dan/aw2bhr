#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031B84.
 * sub_08031B84 @ 0x08031B84
 */

#include "hardware.h"

/* Both `unk06 = 0x9ABC` and `unk212 = 0` emit a dead load of the address they
 * are about to store to -- the volatile signature struct Unk0849B01C already
 * carries on every member.
 *
 * The result goes through a LOCAL rather than two `return`s. With `return 1;`
 * as the then-arm gcc's jump pass moves that block past the literal pool and
 * inverts the branch; assigning to a variable and returning once keeps
 * `movs r0,#1` inline where the ROM has it, and lets the `movs r0,#0` merge
 * with the zero the unk212 store already needs. */
int LinkWaitReadyBarrierBeepOnB(void)
{
    int r;

    gUnknown_0849B01C->unk06 = 0x9ABC;

    if (gpKeySt->pressed & 2)
        PlayMusicOrSfx2(0x68);

    if (gUnknown_0849B01C->unk212 == 0)
    {
        r = 1;
    }
    else
    {
        gUnknown_0300410C = gUnknown_030040CC;
        gUnknown_0849B01C->unk212 = 0;
        r = 0;
    }

    return r;
}
asm(".global sub_08031B84\n.thumb_set sub_08031B84, LinkWaitReadyBarrierBeepOnB\n");
