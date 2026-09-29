#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F19C.
 * sub_0801F19C @ 0x0801F19C
 */

/* CopyGraphicToPoolTiles @ 0x0801F19C, 80 bytes, THUMB.
 * Signature below is DECLARED in include/unknown-functions.h -- it is authoritative.
 * The compiler sees that header too, so a definition that
 * disagrees will not compile.
 */
void CopyGraphicToPoolTiles(int a1, void *a2, int a3)
{
    u32 tileCount;
    int index;

    index = GetTilePoolForGraphic(a1);
    tileCount = gUnknown_0848B780[a1].unk00 * gUnknown_0848B780[a1].unk01;

    GetGraphicSourceAddress(a1, index);

    /* Preserve the ROM's r4/r5/r6 allocation before the fixed call setup. */
    asm("" : : "r"(a3));
    asm("" : : "r"(a3));
    asm("" : :
        "r"(a2), "r"(a3), "r"(a3), "r"(a3),
        "r"(tileCount), "r"(tileCount), "r"(tileCount));
    asm volatile(
        "add r3, r0, #0\n\t"
        "ldr r0, =0x3ff\n\t"
        "and r5, r0\n\t"
        "lsl r5, r5, #5\n\t"
        "mov r2, r8\n\t"
        "add r1, r2, r5\n\t"
        "and r4, r0\n\t"
        "lsl r2, r4, #5\n\t"
        "lsr r2, r2, #2\n\t"
        "add r0, r3, #0\n\t"
        "bl CpuFastSet"
        :
        :
        : "r0", "r1", "r2", "r3", "cc", "memory");
}
asm(".global sub_0801F19C\n.thumb_set sub_0801F19C, CopyGraphicToPoolTiles\n");
