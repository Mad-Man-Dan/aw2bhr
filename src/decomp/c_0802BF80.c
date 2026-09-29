#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802BF80.
 * sub_0802BF80 @ 0x0802BF80, sub_0802BF94 @ 0x0802BF94, sub_0802BFA8 @ 0x0802BFA8, sub_0802BFBC @ 0x0802BFBC
 */

/* One statement. The second argument is the literal 0 that every other
 * sub_080152EC wrapper passes (src/decomp/c_08042B70.c is the promoted
 * sibling); `pop {r0}; bx r0` makes this void and discards the returned slot.
 * gUnknown_0849A3C0 is ROM data reached only as an address, so it is
 * declared `const u8 []` in include/unknown-globals.h and the pool word stays
 * clean.
 */

void StartScreenCoverWipeScript(void)
{
    sub_080152EC(gUnknown_0849A3C0, 0);
}
asm(".global sub_0802BF80\n.thumb_set sub_0802BF80, StartScreenCoverWipeScript\n");

/* One statement. The second argument is the literal 0 that every other
 * sub_080152EC wrapper passes (src/decomp/c_08042B70.c is the promoted
 * sibling); `pop {r0}; bx r0` makes this void and discards the returned slot.
 * gUnknown_0849A3F0 is ROM data reached only as an address, so it is
 * declared `const u8 []` in include/unknown-globals.h and the pool word stays
 * clean.
 */

void StartScreenRevealWipeScript(void)
{
    sub_080152EC(gUnknown_0849A3F0, 0);
}
asm(".global sub_0802BF94\n.thumb_set sub_0802BF94, StartScreenRevealWipeScript\n");

/* One statement. The second argument is the literal 0 that every other
 * sub_080152EC wrapper passes (src/decomp/c_08042B70.c is the promoted
 * sibling); `pop {r0}; bx r0` makes this void and discards the returned slot.
 * gUnknown_0849A428 is ROM data reached only as an address, so it is
 * declared `const u8 []` in include/unknown-globals.h and the pool word stays
 * clean.
 */

void StartScreenCoverWipeLocked(void)
{
    sub_080152EC(gUnknown_0849A428, 0);
}
asm(".global sub_0802BFA8\n.thumb_set sub_0802BFA8, StartScreenCoverWipeLocked\n");

/* One statement. The second argument is the literal 0 that every other
 * sub_080152EC wrapper passes (src/decomp/c_08042B70.c is the promoted
 * sibling); `pop {r0}; bx r0` makes this void and discards the returned slot.
 * gUnknown_0849A450 is ROM data reached only as an address, so it is
 * declared `const u8 []` in include/unknown-globals.h and the pool word stays
 * clean.
 */

void StartScreenRevealWipeLocked(void)
{
    sub_080152EC(gUnknown_0849A450, 0);
}
asm(".global sub_0802BFBC\n.thumb_set sub_0802BFBC, StartScreenRevealWipeLocked\n");
