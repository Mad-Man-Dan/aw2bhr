#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078AF0.
 * sub_08078AF0 @ 0x08078AF0
 */

/* Family F072, third member -- see sub_080215B8. All four callees were already
 * declared `void f(void)`. */

void SyncAllBgTilemaps(void)
{
    sub_08013AEC();
    sub_08013AFC();
    sub_08013B0C();
    sub_08013B1C();
}
asm(".global sub_08078AF0\n.thumb_set sub_08078AF0, SyncAllBgTilemaps\n");
