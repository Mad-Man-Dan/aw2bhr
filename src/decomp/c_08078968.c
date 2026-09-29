#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078968.
 * sub_08078968 @ 0x08078968, sub_08078988 @ 0x08078988
 */

#include "proc.h"

/* `movs r0, #0xd0; lsls r0, r0, #2` is a plain 0x340 -- minimal shift for that
 * value, and the `movs` and the `lsls` write the same register, so it is not
 * wave 23's named constant local. */

void WorldMap_StartConfirmExit(ProcPtr parent)
{
    InitTextTileCache(0x340);
    Proc_StartBlocking(ProcScr_WM_ConfirmExit, parent);
}
asm(".global sub_08078968\n.thumb_set sub_08078968, WorldMap_StartConfirmExit\n");

/* A second bare `bx lr` do-nothing callback, same reading as CampaignMapNoOp. */

void sub_08078988(void)
{
}
