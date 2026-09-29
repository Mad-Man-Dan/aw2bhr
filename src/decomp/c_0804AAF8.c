#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AAF8.
 * sub_0804AAF8 @ 0x0804AAF8
 */

/* gUnknown_0812A28C is NOT a global: it is a `-fforce-addr` address-constant
 * word holding &gUnknown_030044E0, so the global is named directly here.
 *
 * unk66 is loaded twice in the prologue, `ldrb` and `ldrsb`. That pair is what
 * proves the field is u8 and the sign test is an explicit (s8) cast -- an s8
 * field would have served the `& 1` from the one `ldrsb`.
 *
 * The case-5 mask must be bound to a local: `unk66 &= 1;` followed by
 * `if (unk66 != 0)` re-`ldr`s the pointer global and re-`ldrb`s the field,
 * because the store through the pointer kills its own MEM. */
void NameEntry_Loop(void)
{
    u8 v;

    if ((s8)gUnknown_030044E0->unk66 < 0)
    {
        switch (gUnknown_030044E0->unk67)
        {
        case 0:
            ResetTextTileCache(0);
        case 1:
        case 2:
        case 3:
        case 4:
            PutTextScriptImmediate(6, (s16)(gUnknown_030044E0->unk67 * 2 + 6),
                         gBG0TilemapBuffer,
                         gUnknown_084C3B3C[gUnknown_030044E0->unk67
                                           + (gUnknown_030044E0->unk66 & 1) * 5],
                         0x8000, 0);
            break;
        case 5:
            v = gUnknown_030044E0->unk66 & 1;
            gUnknown_030044E0->unk66 = v;
            if (v != 0)
                Decompress(gUnknown_0813593C, (void *)0x060103E0);
            else
                Decompress(gUnknown_081358A0, (void *)0x060103E0);
            ResetTextTileCache(0xb0);
            break;
        }
        gUnknown_030044E0->unk67++;
        BG_EnableSyncBG0();
    }
    else
    {
        if (!sub_08019260())
            NameEntry_HandleInput();
    }
}
asm(".global sub_0804AAF8\n.thumb_set sub_0804AAF8, NameEntry_Loop\n");
