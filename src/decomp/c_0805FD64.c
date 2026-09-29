#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805FD64.
 * sub_0805FD64 @ 0x0805FD64
 */

/* Probe: is gUnknown_030045D4 VOLATILE? That is the only mechanism left that
 * stops combine folding `sign_extend (mem:HI)` into `ldrsh` -- combine will not
 * touch a volatile MEM, so the read stays an `ldrh` and the narrowing stays a
 * `lsls #0x10; asrs #0x10` pair, which is exactly the ROM's shape.
 */

void AiExecuteActionStep(void)
{
    gUnknown_03004774 = 0;

    switch ((s16)*(volatile u16 *)&gUnknown_030045D4)
    {
    case 0:
        sub_0805FE0C();
        break;
    case 1:
        sub_0805FF64();
        break;
    case 2:
        sub_0805FFA0();
        break;
    case 3:
        sub_08060424();
        break;
    case 4:
        sub_0806044C();
        break;
    case 5:
        sub_08060474();
        break;
    case 6:
        sub_080604A4();
        break;
    case 7:
        sub_08060324();
        break;
    case 8:
        sub_08060384();
        break;
    case 9:
        sub_080603D4();
        break;
    case 10:
        sub_0806050C();
        break;
    case 11:
        sub_08060554();
        break;
    }
}
asm(".global sub_0805FD64\n.thumb_set sub_0805FD64, AiExecuteActionStep\n");
