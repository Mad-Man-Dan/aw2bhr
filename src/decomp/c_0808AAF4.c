#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AAF4.
 * sub_0808AAF4 @ 0x0808AAF4
 */

/* Reads the save flash chip's ID.
 *
 * It sends the chip's enter-ID-mode command sequence, waits, reads the device
 * and maker codes through a tiny read routine that SetReadFlash1 copies onto
 * the stack, sends the exit sequence, waits again, and returns
 * (device << 8) | maker.
 */

#define FLASH_BASE 0x0E000000
#define FLASH_WRITE(a, d) (*(vu8 *)(FLASH_BASE + (a)) = (d))
#define DELAY()                  \
do {                             \
    vu16 i;                      \
    for (i = 20000; i != 0; i--) \
        ;                        \
} while (0)


u16 ReadFlashId(void)
{
    u16 flashId;
    u16 readFlash1Buffer[0x20];
    u8 (*readFlash1)(u8 *);

    SetReadFlash1(readFlash1Buffer);
    readFlash1 = (u8 (*)(u8 *))((s32)readFlash1Buffer + 1);

    FLASH_WRITE(0x5555, 0xAA);
    FLASH_WRITE(0x2AAA, 0x55);
    FLASH_WRITE(0x5555, 0x90);
    DELAY();

    flashId = readFlash1((u8 *)(FLASH_BASE + 1)) << 8;
    flashId |= readFlash1((u8 *)FLASH_BASE);

    FLASH_WRITE(0x5555, 0xAA);
    FLASH_WRITE(0x2AAA, 0x55);
    FLASH_WRITE(0x5555, 0xF0);
    DELAY();

    return flashId;
}
asm(".global sub_0808AAF4\n.thumb_set sub_0808AAF4, ReadFlashId\n");
