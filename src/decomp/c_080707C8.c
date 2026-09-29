#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080707C8.
 * sub_080707C8 @ 0x080707C8
 */

void MusicPlayerJumpTableCopy(void)
{
    asm("svc #0x2a");
}
asm(".global sub_080707C8\n.thumb_set sub_080707C8, MusicPlayerJumpTableCopy\n");
