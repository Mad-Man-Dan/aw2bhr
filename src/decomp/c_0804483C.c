#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804483C.
 * sub_0804483C @ 0x0804483C
 */

#include "proc.h"

void CoPowerHachi(ProcPtr parent)
{
    PlayMusicOrSfx2(502);
    StartCoPowerWhiteFlash(parent);
}
asm(".global sub_0804483C\n.thumb_set sub_0804483C, CoPowerHachi\n");
