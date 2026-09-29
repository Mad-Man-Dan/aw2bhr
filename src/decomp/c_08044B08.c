#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044B08.
 * StartUnitSparkleEffect @ 0x08044B08
 */

void StartUnitSparkleEffect(u8 a1, u8 a2, u8 a3)
{
    StartSparkleAtTile(a1, a2, 0x51CA, a3);
}

asm(".global sub_08044B08\n.thumb_set sub_08044B08, StartUnitSparkleEffect\n");
