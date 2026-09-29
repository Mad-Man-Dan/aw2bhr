#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804AE10.
 * sub_0804AE10 @ 0x0804AE10
 */

/* Installs one gUnknown_0200C528 list script. StartEventScript returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void NameEntry_StartPromptMessage(void)
{
    StartEventScript(gUnknown_084C38BC);
}
asm(".global sub_0804AE10\n.thumb_set sub_0804AE10, NameEntry_StartPromptMessage\n");
