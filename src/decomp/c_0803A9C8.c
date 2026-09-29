#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A9C8.
 * sub_0803A9C8 @ 0x0803A9C8
 */

/* The parameter is a DECLARED `u8` even though the prologue is copy-then-narrow
 * -- the c_080154C4.c case. An `int` parameter with the cast at the call moves
 * the `lsls; lsrs` pair AFTER the pool `ldr`, which is two bytes of difference
 * in the same four instructions.
 *
 * The reload of gUnknown_0849D89C between the `strb` and the `str` is aliasing:
 * the byte store could have hit the pointer word itself. */
void StartDeploymentUnitInfo(u8 a)
{
    InitUnit(&gUnknown_03004100, a);
    sub_080152EC(gUnknown_0849E280, 0);
    gUnknown_0849D89C->unk00 = 0x80;
    gUnknown_0849D89C->unk04 = &gUnknown_03004100;
    gUnknown_0849D89C->unk08 = gUnknown_030033EC;
}
asm(".global sub_0803A9C8\n.thumb_set sub_0803A9C8, StartDeploymentUnitInfo\n");
