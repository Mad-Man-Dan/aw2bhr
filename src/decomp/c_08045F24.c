#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045F24.
 * sub_08045F24 @ 0x08045F24, sub_08045F40 @ 0x08045F40
 */

#include "proc.h"

void MapEventFx_Sfx1C7WithFlash(void)
{
    PlayMusicOrSfx2(0x1c7);
    StartWhiteFlash(0x14, 0x64, 0x3c, 0);
}
asm(".global sub_08045F24\n.thumb_set sub_08045F24, MapEventFx_Sfx1C7WithFlash\n");

void sub_08045F40(void)
{
    PlayMusicOrSfx2(0x1e1);
    Proc_Start(gUnknown_084B7628, PROC_TREE_3);
}
