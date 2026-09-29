#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063A3C.
 * sub_08063A3C @ 0x08063A3C
 */

struct Unk03001470 *GetCurrentSlotScript(void)
{
    return &gUnknown_03001470[gUnknown_03001FBC];
}
asm(".global sub_08063A3C\n.thumb_set sub_08063A3C, GetCurrentSlotScript\n");
