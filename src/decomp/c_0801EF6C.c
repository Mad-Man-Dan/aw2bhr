#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EF6C.
 * sub_0801EF6C @ 0x0801EF6C
 */

/* Family F069's whole-range member: sub_0801EF90 and sub_0801EFA8 beside it
 * blank objects 0x10..0x7F and 0..0xF and reset one counter each; this one
 * covers all 0x80 objects and resets BOTH. That pairing is what the note on
 * sub_0801EF90 predicted from `varies` and it holds here. */
void sub_0801EF6C(void)
{
    HideOamObjects(0, 0x80);
    gUnknown_03002B54 = 0x10;
    gUnknown_03001FE4 = 0;
}
