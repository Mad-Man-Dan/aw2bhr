#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802A880.
 * sub_0802A880 @ 0x0802A880, GetTerrainNamePalette @ 0x0802A8AC
 */

const u8 * GetTerrainPictureGraphic(int a, int b)
{
    if (a == 8)
        return gUnknown_08104464[((b + 0x1e) * 8) & 0x3ff];

    return gUnknown_08104464[((a - 1) * 8) & 0x3ff];
}
asm(".global sub_0802A880\n.thumb_set sub_0802A880, GetTerrainPictureGraphic\n");

/* The `b += 8` has to sit INSIDE the subscript. Written as
 * `gUnknown_08106864[b + 8]` the constant folds into the relocation
 * (`.word gUnknown_08106864+0x100`) and the runtime `adds r0, #8` disappears.
 * Written as a separate `b += 8` statement the add survives but is emitted
 * BEFORE the base `ldr` instead of after it. Only the embedded assignment
 * gives both -- see docs/agbcc-codegen.md.
 */
const u8 * GetTerrainNamePalette(int a, int b)
{
    if (b != 0)
        return gUnknown_08106864[b += 8];

    return gUnknown_08106864[gUnknown_0849A2C8[a].palette];
}

asm(".global sub_0802A8AC\n.thumb_set sub_0802A8AC, GetTerrainNamePalette\n");
