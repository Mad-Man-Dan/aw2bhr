#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C130.
 * sub_0802C130 @ 0x0802C130, sub_0802C144 @ 0x0802C144
 */

/* One statement. The second argument is the literal 0 that every other
 * sub_080152EC wrapper passes (src/decomp/c_08042B70.c is the promoted
 * sibling); `pop {r0}; bx r0` makes this void and discards the returned slot.
 * gUnknown_0849A4A0 is ROM data reached only as an address, so it is
 * declared `const u8 []` in include/unknown-globals.h and the pool word stays
 * clean.
 */

void StartQuitToMainMenuScript(void)
{
    sub_080152EC(gUnknown_0849A4A0, 0);
}
asm(".global sub_0802C130\n.thumb_set sub_0802C130, StartQuitToMainMenuScript\n");

/* Installs one gUnknown_0200C528 list script. StartEventScript returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void StartQuitToMainMenuConfirmScript(void)
{
    StartEventScript(gUnknown_0849A520);
}
asm(".global sub_0802C144\n.thumb_set sub_0802C144, StartQuitToMainMenuConfirmScript\n");
