#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802F03C.
 * sub_0802F03C @ 0x0802F03C
 */

/*
 * SioResetBuffers -- reset the link-cable communication state.
 *
 * Clears the counters and flags in the link record (gUnknown_0849B018), sets
 * the per-player tables in gUnknown_0849B01C to 0xFFFF, zeroes the 4- and
 * 128-entry tables, clears the 48 packet buffers (0x88 bytes each), and empties
 * the send ring (512 entries) and the four players' receive rings (1024
 * entries each) along with their cursors.
 *
 * Why the C looks odd: the loop counters i and j swap roles between loops (the
 * packet-buffer loops and the receive-ring loop count with j on the outside),
 * because the compiler gives each counter a register by how it is used, and
 * only this mix of roles gives the original registers. `new_var`/`new_var2`
 * and the empty do/while keep the buffer pointer and the descending counter
 * as separate values.
 */
void SioResetBuffers(void)
{
    struct Unk08090CD8Entry *new_var;
    struct Unk08090CD8Entry *e;
    int i, j;
    int new_var2;

    gUnknown_0300333C = 0;
    gUnknown_0849B018->unk20 = 0;
    gUnknown_0849B018->unk22 = 0;
    gUnknown_0849B018->unk1aac = 0;
    gUnknown_0849B018->unk1aad = 0;
    gUnknown_0849B018->unk1aae = 0;
    gUnknown_0849B018->unk1aaf = 0;
    gUnknown_0849B018->unk1a = 0;
    gUnknown_0849B018->unk1b = 0;
    gUnknown_0849B018->unk1f = 0;

    for (i = 0; i < 4; i++)
    {
        gUnknown_0849B018->unk0a[i] = 0;
        gUnknown_0849B018->unk16[i] = 0;
        gUnknown_0849B018->unk0e[i] = 0;
        gUnknown_0849B018->unk24[i] = 0;
        for (j = 0; j < 64; j++)
            gUnknown_0849B01C->unk08[j][i] |= 0xffff;
        gUnknown_0849B01C->unk08[64][i] |= 0xffff;
        gUnknown_0849B01C->unk208[i] = gUnknown_0849B01C->unk208[i];
        gUnknown_0300449C[i] = 0;
    }

    for (i = 0; i < 128; i++)
    {
        gUnknown_03004400[i] = 0;
        gUnknown_0849B018->unk2c[i] = 0;
    }

    for (j = 0; j < 32; j++)
    {
        e = &gUnknown_0849B018->unk12c[j];
        e->unk00 = 0;
        for (i = 127; i >= 0; i--)
        {
            new_var2 = i;
            do { } while (0);
            e->unk06[new_var2] = 0;
        }
    }

    for (j = 0; j < 16; j++)
    {
        e = &gUnknown_0849B018->unk12c[32 + j];
        new_var = e;
        e->unk00 = 0;
        for (i = 0; i < 128; i++)
            new_var->unk06[i] = 0;
    }

    gUnknown_030040CC = gUnknown_0300410C = 0;

    for (i = 0; i < 0x200; i++)
        gUnknown_02025818[i] = 0;

    for (j = 0; j < 4; j++)
    {
        gUnknown_03003128[j] = gUnknown_03003F48[j] = 0;
        for (i = 0; i <= 0x3ff; i++)
            gUnknown_02025C18[i][j] = 0;
    }
}
asm(".global sub_0802F03C\n.thumb_set sub_0802F03C, SioResetBuffers\n");
