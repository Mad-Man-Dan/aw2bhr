#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D978.
 * sub_0803D978 @ 0x0803D978
 */

#include "proc.h"

void sub_0803D978(void)
{
    if (Proc_Find(ProcScr_BattleMaps) == NULL)
        StartMusicResumeFade();
}
