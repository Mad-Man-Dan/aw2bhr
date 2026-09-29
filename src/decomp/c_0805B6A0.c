#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B6A0.
 * sub_0805B6A0 @ 0x0805B6A0
 */

/* Ghidra-assisted, matched with the neighboring map-record vocabulary. */
int AiNextInterestListEntry(int *slot, int *cursor, int *outX, int *outY)
{
    int index;
    u8 *record;
    u8 *recordY;
    /* Separate bases keep the two X reads and Y recomputation in ROM order. */
    index = gUnknown_02029ED8[*cursor + *slot * 0x20
                             + gUnknown_030033EC * 0xc00];
    while (index != 0xff)
    {
        record = &gUnknown_02029ED8[index * 8 + *slot * 0x3e0
                                    + gUnknown_030033EC * 0xc00];
        if (*(volatile u8 *)&record[0x60] != 0xfe)
        {
            *outX = *(volatile u8 *)&record[0x60];
            recordY = &gUnknown_02029ED8[index * 8 + *slot * 0x3e0
                                          + gUnknown_030033EC * 0xc00];
            *outY = recordY[0x61];
            return 1;
        }
        ++*cursor;
        if (*cursor > 0x1f)
            return 0;
        index = gUnknown_02029ED8[*cursor + *slot * 0x20
                                 + gUnknown_030033EC * 0xc00];
    }
    return 0;
}
asm(".global sub_0805B6A0\n.thumb_set sub_0805B6A0, AiNextInterestListEntry\n");
