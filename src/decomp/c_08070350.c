#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08070350.
 * sub_08070350 @ 0x08070350
 */

/* MidiKey2freq. The `<< 24` is bound to its OWN local ahead of the clamp, not
 * applied at the call: the ROM's entry is a bare `lsls r7,r2,#0x18` with no
 * `lsrs` back, and the clamp arm rebuilds 0xff000000 as `movs #255; lsls #24`
 * rather than assigning 255 to the key-adjust. Shifting at the call site
 * instead costs 12 bytes (measured, 88.0%). */
u32 MidiKey2fr(struct WaveData *wav, u8 key, u8 fineAdjust)
{
    u32 val1;
    u32 val2;
    u32 fine;

    fine = fineAdjust << 24;

    if (key > 178)
    {
        key = 178;
        fine = 255 << 24;
    }

    val1 = gUnknown_081B9DF4[key];
    val1 = gUnknown_081B9EA8[val1 & 0xf] >> (val1 >> 4);

    val2 = gUnknown_081B9DF4[key + 1];
    val2 = gUnknown_081B9EA8[val2 & 0xf] >> (val2 >> 4);

    return sub_0806F734(wav->freq, val1 + sub_0806F734(val2 - val1, fine));
}
asm(".global sub_08070350\n.thumb_set sub_08070350, MidiKey2fr\n");
