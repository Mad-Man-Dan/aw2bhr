#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B79C.
 * sub_0803B79C @ 0x0803B79C, sub_0803B7B4 @ 0x0803B7B4
 */

#include "proc.h"

int IsMusicPauseFadeActive(void)
{
    return Proc_Find(gUnknown_0849E7B8) != 0;
}
asm(".global sub_0803B79C\n.thumb_set sub_0803B79C, IsMusicPauseFadeActive\n");

/* gUnknown_030005CA is reset to 0xFFFF by sub_0803B5F4, so this starts the
 * 0849E7D8 proc only while that slot is still unset. `pop {r0}` says void, and
 * the plain `movs r1, #3` in front of the `bl` is PROC_TREE_3 rather than a
 * forwarded parent.
 */
void StartMusicResumeFade(void)
{
    if (gUnknown_030005CA == 0xFFFF)
        Proc_Start(gUnknown_0849E7D8, PROC_TREE_3);
}
asm(".global sub_0803B7B4\n.thumb_set sub_0803B7B4, StartMusicResumeFade\n");
