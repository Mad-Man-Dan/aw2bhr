#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080707CC.
 * sub_080707CC @ 0x080707CC, sub_080707E0 @ 0x080707E0
 */

/* Wave 47 (W47-B): both parameters retyped from `int` to `void *`. MPlayOpen_rev01
 * (MPlayOpen) hands Clear64byte_rev the MusicPlayerInfo it is about to fill in --
 * MP2K's Clear64byte -- which is the first call site either forwarder has had.
 * Byte-neutral: re-verified as still matching.
 */
void ClearChain_rev(void *a1)
{
    gUnknown_030057C8(a1);
}
asm(".global sub_080707CC\n.thumb_set sub_080707CC, ClearChain_rev\n");

void Clear64byte_rev(void *a1)
{
    gUnknown_030057CC(a1);
}
asm(".global sub_080707E0\n.thumb_set sub_080707E0, Clear64byte_rev\n");
