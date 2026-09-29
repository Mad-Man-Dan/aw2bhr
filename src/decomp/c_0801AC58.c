#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801AC58.
 * sub_0801AC58 @ 0x0801AC58
 */

/* Commit one save slot's staging buffer back into the live tables.
 *
 * The 0x10-byte stack array preserves bit 4 of every gUnknown_0200CC38.unk20
 * slot across the IsSaveSlotInvalid call, which is free to clobber them, and puts it
 * back afterwards (`& 0xef` clears bit 4, `| tmp[i]` restores it).
 *
 * gUnknown_02002000 is the 0x1000-byte flash staging buffer that
 * src/decomp/c_0801B018.c validates; the header view here is a cast over the
 * same u8 array for the same reason given there -- +0x0e is a u16 destination
 * offset, +0x50 a u16 length and +0x52 the payload. gUnknown_0200CC58 and
 * gUnknown_0200CC48 in asm/ are gUnknown_0200CC38.unk20 and .unk10 with the
 * member offset folded into the address constant by LICM, exactly as the
 * struct's note in include/unknown-globals.h records; likewise
 * gUnknown_02002FEF is BUF->unkfef.
 *
 * gUnknown_0200CC2C is re-read on every iteration of the copy loop because the
 * `strb` through it may alias it -- no volatile is needed for that.
 *
 * ONE local `j` for the IsSaveSlotInvalid result AND every inner loop counter, and
 * that is measured. Split into a separate `r`, j's live range is short enough
 * that it outranks the -fforce-addr base pointers and takes r2, pushing the
 * bases to r3; the ROM has it the other way round in all three inner loops and
 * in the IsSaveSlotInvalid result, which only happens when they are one pseudo whose
 * live range spans the function. */

struct Unk1AC58Buf
{
    /* 0x000 */ u8 filler_000[0x0c];
    /* 0x00c */ u8 unk00c;
    /* 0x00d */ u8 filler_00d[0x01];
    /* 0x00e */ u16 unk00e;
    /* 0x010 */ u8 filler_010[0x40];
    /* 0x050 */ u16 unk050;
    /* 0x052 */ u8 unk052[0xf9d];
    /* 0xfef */ u8 unkfef[0x10];
};
#define BUF ((struct Unk1AC58Buf *)gUnknown_02002000)

int ReadSaveSlot(u8 id, u8 *a2)
{
    u8 tmp[0x10];
    int i;
    int j;

    for (i = 0; i < 0x10; i++)
        tmp[i] = gUnknown_0200CC38.unk20[i] & 0x10;

    j = IsSaveSlotInvalid(id);

    for (i = 0; i < 0x10; i++)
        gUnknown_0200CC38.unk20[i] = (gUnknown_0200CC38.unk20[i] & 0xef) | tmp[i];

    if (j != 0)
        return 1;

    for (i = 0; i < 0x10; i++)
    {
        if (id == gUnknown_0200CC38.unk00[i])
        {
            if (ReadAndValidateSaveSector(i) != 0)
                return 1;

            for (j = 0; j < BUF->unk050; j++)
                ((u8 *)gUnknown_0200CC2C + BUF->unk00e)[j] = BUF->unk052[j];

            if ((BUF->unk00c & 0xf0) == 0 && id == 0)
            {
                for (j = 0; j < 0x10; j++)
                    gUnknown_0200CC38.unk10[j] = BUF->unkfef[j];
            }
        }
    }

    if (id == 0)
    {
        for (j = 0; j < 0x10; j++)
            gUnknown_0200CC38.unk00[j] = gUnknown_0200CC38.unk10[j];
    }

    return 0;
}
asm(".global sub_0801AC58\n.thumb_set sub_0801AC58, ReadSaveSlot\n");
