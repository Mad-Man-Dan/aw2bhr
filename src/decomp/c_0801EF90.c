#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801EF90.
 * sub_0801EF90 @ 0x0801EF90, sub_0801EFA8 @ 0x0801EFA8
 */

/* Family F069, second member: the same OAM blank-and-reset over objects
 * 0x10..0x7F. Note that `varies` gives indices 1 and 5 the same three values
 * (#0, #0x10, #0) across the family -- the object the range starts at is also
 * the value the counter is reset to, so the pair is one quantity written
 * twice, not two coincidences. */

void ClearHiOamShadow(void)
{
    HideOamObjects(0x10, 0x70);
    gUnknown_03002B54 = 0x10;
}
asm(".global sub_0801EF90\n.thumb_set sub_0801EF90, ClearHiOamShadow\n");

/* Family F069, third member: objects 0..0xF, and a different counter.
 * gUnknown_03001FE4 is newly declared `u16` in unknown-globals.h -- it has
 * only four accesses in the ROM and all are 16-bit, the decisive one being the
 * `ldrh; adds #1; strh` bump inside sub_0801EEnn's sprite-emit loop, which
 * fixes the width at 16 rather than at "at least 16". */

void ClearLoOamShadow(void)
{
    HideOamObjects(0, 0x10);
    gUnknown_03001FE4 = 0;
}
asm(".global sub_0801EFA8\n.thumb_set sub_0801EFA8, ClearLoOamShadow\n");
