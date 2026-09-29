#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08010EBC.
 * sub_08010EBC @ 0x08010EBC, sub_08010EC0 @ 0x08010EC0
 */

void DummyIntFunc(void)
{
}
asm(".global sub_08010EBC\n.thumb_set sub_08010EBC, DummyIntFunc\n");

void sub_08010EC0(void)
{
}
