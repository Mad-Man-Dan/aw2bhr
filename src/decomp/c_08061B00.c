#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061B00.
 * sub_08061B00 @ 0x08061B00
 */

/* Two ROM tables of nullary function pointers stepped by one shared cursor.
 * The post-increment is what puts `adds r0,#1; str r0,[r2]` between the address
 * computation and the `ldr r0,[r1]` of the slot; `bl _call_via_r0` is agbcc's
 * ordinary indirect-call trampoline and its register index is the arity. */
void AiStartNextPass(void)
{
    if (gPlaySt.fog == 0)
        gUnknown_085766E8[gUnknown_03004770++]();
    else
        gUnknown_08576738[gUnknown_03004770++]();
}
asm(".global sub_08061B00\n.thumb_set sub_08061B00, AiStartNextPass\n");
