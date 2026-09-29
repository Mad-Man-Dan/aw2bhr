#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801BAB8.
 * sub_0801BAB8 @ 0x0801BAB8
 */

void DummyIRQRoutine(void)
{
}
asm(".global sub_0801BAB8\n.thumb_set sub_0801BAB8, DummyIRQRoutine\n");
