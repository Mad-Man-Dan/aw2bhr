#include "global.h"
#include "hardware.h"

/* Builds a background screen from compressed data.
 *
 * It clears the tilemap buffer, decompresses the tile graphics straight into
 * VRAM and the tilemap into the buffer, adds a tile and palette offset to
 * every entry in the buffer, then copies the finished map to the background's
 * screen block.
 *
 * Why the C looks odd: the buffer entries are written through a volatile
 * pointer, so the buffer pointer is re-read from memory on every pass of the
 * loop rather than held in a register.
 */
void sub_0808A3DC(void)
{
    u16 fill;
    int i;

    fill = 0;
    CpuSet(&fill, gUnknown_0849957C, 0x01000400);

    Decompress(gUnknown_0823E7A0,
               (void *)(0x06000400 + gUnknown_03001FE8.bits.chr_block * 0x4000));
    Decompress(gUnknown_0823E684, gUnknown_0849957C + 0x200);

    for (i = 0; i <= 0x3FF; i++)
        ((vu16 *)gUnknown_0849957C)[i] += 0x1020;

    sub_0802D5CC(3, 1);

    CpuFastSet(gUnknown_0849957C,
               (void *)(0x06000800 + gUnknown_03001FE8.bits.tm_block * 0x800),
               0x200);
}
