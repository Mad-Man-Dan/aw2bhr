#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080447EC.
 * sub_080447EC @ 0x080447EC
 */

#include "proc.h"

void CoPowerSonja(ProcPtr parent)
{
    PlayMusicOrSfx2(0xc4);
    StartCoPowerWhiteFlash(parent);
}
asm(".global sub_080447EC\n.thumb_set sub_080447EC, CoPowerSonja\n");
