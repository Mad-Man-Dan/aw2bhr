#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039A58.
 * sub_08039A58 @ 0x08039A58
 */

void CoPowerPanelNoOp(int arg0, int arg1)
{
}
asm(".global sub_08039A58\n.thumb_set sub_08039A58, CoPowerPanelNoOp\n");
