#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08021DA0.
 * LoadRiverAnimFrame @ 0x08021DA0
 */

#include "hardware.h"

void LoadRiverAnimFrame(int a1)
{
    u8 n = a1;

    CpuCopyAuto(gUnknown_080C9FC4 + n * 0xc00,
                 (void *)(0x06004000 + (gUnknown_0300251C.bits.chr_block << 14)),
                 0xc00);
}

asm(".global sub_08021DA0\n.thumb_set sub_08021DA0, LoadRiverAnimFrame\n");
