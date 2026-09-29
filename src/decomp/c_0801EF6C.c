#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EF6C.
 * sub_0801EF6C @ 0x0801EF6C
 */

/* Family F069's whole-range member: ClearHiOamShadow and ClearLoOamShadow beside it
 * blank objects 0x10..0x7F and 0..0xF and reset one counter each; this one
 * covers all 0x80 objects and resets BOTH. That pairing is what the note on
 * ClearHiOamShadow predicted from `varies` and it holds here. */
void ClearOamShadowResetCursors(void)
{
    HideOamObjects(0, 0x80);
    gUnknown_03002B54 = 0x10;
    gUnknown_03001FE4 = 0;
}
asm(".global sub_0801EF6C\n.thumb_set sub_0801EF6C, ClearOamShadowResetCursors\n");
