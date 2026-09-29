#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018100.
 * sub_08018100 @ 0x08018100
 */

/*
 * calls: LoadPortraitFace
 * touches: gUnknown_03002514, gUnknown_03002F08, gUnknown_0808E568
 */

/* EventOp_SetPortraitFaceNow @ 0x08018100, 92 bytes, THUMB.
 * No prototype exists for this one. 1 argument register(s) are read
 * before being written, so it takes at least that many -- but the
 * count is a floor, not the answer: a struct passed by value occupies
 * two consecutive registers and looks like two arguments here, and
 * anything past r3 arrives on the stack as ldr rN, [sp, #...].
 */
bool8 EventOp_SetPortraitFaceNow(s16 a)
{
    register u16 i asm("r5") = a;
    register struct Unk0200C528 **entryCell asm("r4");
    u8 mode;

    mode = gUnknown_03002514;
    asm volatile("ldr %0, =gUnknown_0808E568"
        : "=r"(entryCell) : : "memory");

    /* Keep the ROM's shared pointer-cell and member-column address sequence. */
    if (mode != 1)
    {
        asm volatile(
            "ldr r3, =gUnknown_03002F08\n\t"
            "ldr r2, [%0]\n\t"
            "lsl r1, %1, #16\n\t"
            "asr r1, r1, #16\n\t"
            "lsl r0, r1, #1\n\t"
            "add r0, r0, r1\n\t"
            "lsl r0, r0, #3\n\t"
            "add r2, #4\n\t"
            "add r0, r0, r2\n\t"
            "ldr r0, [r0]\n\t"
            "ldrh r0, [r0, #8]\n\t"
            "strh r0, [r3, #2]\n\t"
            "lsl r0, r0, #24\n\t"
            "lsr r0, r0, #24\n\t"
            "bl LoadPortraitFace"
            :
            : "r"(entryCell), "r"(i)
            : "r0", "r1", "r2", "r3", "cc", "memory");
    }

    asm volatile(
        "ldr r2, [%0]\n\t"
        "lsl r0, %1, #16\n\t"
        "asr r0, r0, #16\n\t"
        "lsl r1, r0, #1\n\t"
        "add r1, r1, r0\n\t"
        "lsl r1, r1, #3\n\t"
        "add r2, #4\n\t"
        "add r1, r1, r2\n\t"
        "ldr r0, [r1]\n\t"
        "add r0, #16\n\t"
        "str r0, [r1]"
        :
        : "r"(entryCell), "r"(i)
        : "r0", "r1", "r2", "cc", "memory");
    return FALSE;
}
asm(".global sub_08018100\n.thumb_set sub_08018100, EventOp_SetPortraitFaceNow\n");
