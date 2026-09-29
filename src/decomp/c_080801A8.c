#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080801A8.
 * sub_080801A8 @ 0x080801A8
 */

#include "hardware.h"

/* SuperCoPowerScene_UploadBgTilemaps @ 0x080801A8, 172 bytes, THUMB. Matched.
 *
 * Blits one EWRAM tile buffer into the screen blocks of the two BG shadows
 * gUnknown_03002B6C and gUnknown_030030B4: four 0x400-byte pages of
 * gUnknown_02010050 into each, then one 0x200-byte page of gUnknown_0200FC50
 * into each at the block base.
 *
 * `lsls #0x13; lsrs #0x1b; lsls #0xb` is BgCnt.tm_block (bits 8..12) turned
 * into its VRAM byte address -- each screen block is 0x800 bytes.
 *
 * `subs r5, #1; cmp r5, #0; bge` at the bottom is check_dbra_loop's rewrite of
 * the ASCENDING `for (i = 0; i < 4; i++)`; the address accumulator counts UP
 * by 0x400, which is what fixes the direction.
 */
void SuperCoPowerScene_UploadBgTilemaps(void)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        CpuFastSet(gUnknown_02010050,
            (void *)((gUnknown_03002B6C.bits.tm_block << 11) + 0x06000000 + i * 0x400), 0x100);
        CpuFastSet(gUnknown_02010050,
            (void *)((gUnknown_030030B4.bits.tm_block << 11) + 0x06000000 + i * 0x400), 0x100);
    }

    CpuFastSet(gUnknown_0200FC50,
        (void *)((gUnknown_03002B6C.bits.tm_block << 11) + 0x06000000), 0x80);
    CpuFastSet(gUnknown_0200FC50 + 0x200,
        (void *)((gUnknown_030030B4.bits.tm_block << 11) + 0x06000200), 0x80);
}
asm(".global sub_080801A8\n.thumb_set sub_080801A8, SuperCoPowerScene_UploadBgTilemaps\n");
