#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804E304.
 * PartRideFigure_Loop2 @ 0x0804E304
 */

/* MATCHED. Byte-for-byte the same function as PartRideFigure_Loop -- identical
 * instruction stream and identical pool words. One C body, two
 * addresses; read that one for the derivation. */
void PartRideFigure_Loop2(void)
{
    RidePartOnFigure(gUnknown_03001470[gUnknown_03001FBC].unk30,
        gUnknown_03001470[gUnknown_03001FBC].unk34, gUnknown_03001FBC);
}

asm(".global sub_0804E304\n.thumb_set sub_0804E304, PartRideFigure_Loop2\n");
