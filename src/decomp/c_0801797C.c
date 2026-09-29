#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801797C.
 * sub_0801797C @ 0x0801797C, sub_08017988 @ 0x08017988
 */

void ResumeEventScripts(void)
{
    gUnknown_03002B38 = 0;
}
asm(".global sub_0801797C\n.thumb_set sub_0801797C, ResumeEventScripts\n");

s16 AreEventScriptsPaused(void)
{
    return gUnknown_03002B38;
}
asm(".global sub_08017988\n.thumb_set sub_08017988, AreEventScriptsPaused\n");
