#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802ED40.
 * SioHandleIrq_Serial @ 0x0802ED40
 */

/* SioHandleIrq_Serial @ 0x0802ED40 -- the serial (multiplayer) interrupt body.
 *
 * PROMOTION NEEDS A "rodata" ENTRY.  Three `-fforce-addr` address constants
 * live in this function's own .rodata and the ROM homes them at
 *   0x08090C68 -> &gUnknown_0849B018
 *   0x08090C6C -> &gUnknown_0300333C
 *   0x08090C70 -> &gUnknown_0849B01C
 * so data/promoted.json needs
 *   "rodata": ["0x08090C68", "0x08090C6C", "0x08090C70"]
 * and tools/split_rodata.py must carve them out of the blob.  trymatch reports
 * `relocs: name different symbols that resolve to the same address`, which is
 * the expected verdict for the honest spelling -- the globals are named
 * directly and agbcc parks the addresses itself.
 *
 * Wave 56, W56-I.
 */

#include "global.h"
#include "hardware.h"
/* A volatile view of struct Unk0849B01C's unk08 row array.  The member itself
 * cannot carry the qualifier -- see the note in include/unknown-globals.h:
 * LinkRestartKeySync's `unk08[0][i] |= 0xffff` goes +4 bytes if it does -- but the
 * ROM's plain row store here carries the dead `ldrh` of the destination that
 * only a volatile AGGREGATE lvalue produces.  A `(volatile u16 *)` cast on the
 * row pointer is NOT equivalent: it folds the 0x8 into the store displacement
 * and emits no dead load. */
struct Unk0849B01CRows
{
    /* 0x000 */ u8 filler_00[8];
    /* 0x008 */ volatile u16 unk08[64][4];
};

void SioHandleIrq_Serial(void)
{
    u16 buf[4];
    u16 v;
    u16 count;
    int i;

    count = 0;
    gUnknown_02023894 = 1;
    gUnknown_0849B018->unk1a = 0;
    gUnknown_0300333C = 1;
    REG_TM3CNT_H = 0;
    gUnknown_0849B018->unk02 = gUnknown_0300056C = REG_SIOCNT;
    if (gUnknown_0849B018->unk04 != 6)
        gUnknown_0849B018->unk06 = (gUnknown_0849B018->unk02 >> 4) & 3;
    *(u64 *)buf = *(u64 *)(REG_BASE + REG_OFFSET_SIOMULTI0);
    REG_SIOCNT = gUnknown_03000560 | 0x6000;
    REG_SIOMLT_SEND = 0x7FFF;
    if (gUnknown_03000570 != 0)
    {
        gUnknown_03000570 = 2;
        gUnknown_0849B018->unk08 = 1 << gUnknown_0849B018->unk06;
        for (i = 0; i < 4; i++)
        {
            if (buf[i] != 0xFFFF)
            {
                if (gUnknown_0849B018->unk0a[i] == 0)
                    gUnknown_0849B018->unk0a[i] = 1;
                gUnknown_0849B018->unk08 |= 1 << i;
            }
            if (SioIsPlayerLinked(i) == 1)
            {
                if (gUnknown_0849B018->unk0e[i] == 0xFFFF)
                    gUnknown_0849B018->unk16[i]++;
                else
                    gUnknown_0849B018->unk16[i] = 0;
            }
            gUnknown_0849B018->unk0e[i] = buf[i];
            gUnknown_02025C18[gUnknown_03003F48[i]][i] = buf[i];
            gUnknown_03003F48[i]++;
            gUnknown_03003F48[i] &= 0x3FF;
        }
        if (gUnknown_0849B018->unk04 > 4)
        {
            switch (gUnknown_0849B018->unk01)
            {
            case 1:
                if (gUnknown_0300410C != gUnknown_030040CC)
                {
                    v = gUnknown_02025818[gUnknown_030040CC];
                    gUnknown_030040CC++;
                    gUnknown_030040CC &= 0x1FF;
                    SioSend16(&v, 1);
                }
                if ((gUnknown_03000564 & 0xC000) == 0xC000
                 && gUnknown_0300055C == 0
                 && gUnknown_03003F6C->unk0a != 0)
                    SioStartTimer3(gUnknown_03003F6C->unk0a);
                break;
            case 2:
                if (gUnknown_0849B018->unk06 != 0)
                {
                    SioSend16((u16 *)&gUnknown_0849B01C->unk06, 1);
                    gUnknown_0849B01C->unk06 = 0x5FFF;
                }
                for (i = 0; i < 4; i++)
                    ((struct Unk0849B01CRows *)gUnknown_0849B01C)->unk08[gUnknown_0849B01C->unk04][i] = buf[i];
                gUnknown_0849B01C->unk04++;
                gUnknown_0849B01C->unk04 &= 0x3F;
                break;
            case 3:
                if (gUnknown_0849B018->unk06 != 0)
                {
                    SioSend16((u16 *)&gUnknown_0849B01C->unk06, 1);
                    gUnknown_0849B01C->unk06 = 0x5FFF;
                }
                for (i = 0; i < 4; i++)
                {
                    if (SioIsPlayerLinked(i) && buf[i] != 0x9ABC)
                        count++;
                }
                if (count == 0)
                    gUnknown_0849B01C->unk212 = 1;
                break;
            }
        }
    }
    gUnknown_0300333C = 0;
}

asm(".global sub_0802ED40\n.thumb_set sub_0802ED40, SioHandleIrq_Serial\n");
