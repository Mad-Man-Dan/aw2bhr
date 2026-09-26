#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C260.
 * sub_0802C260 @ 0x0802C260, sub_0802C270 @ 0x0802C270, sub_0802C280 @ 0x0802C280, sub_0802C290 @ 0x0802C290
 */

/* Registers a function with the 16-slot gUnknown_03002FA0 list. sub_08011AAC
 * takes its entry as `void *` (that is how src/decomp/c_08011AAC.c defines it),
 * so a function address has to be cast -- exactly the note carried on the
 * sibling pair sub_08011B34/sub_08011B5C. The pool word is the ADDRESS of
 * sub_080184C8, not a call to it.
 */

void sub_0802C260(void)
{
    sub_08011AAC((void *)sub_080184C8);
}

/* Registers a function with the 16-slot gUnknown_03002FA0 list. sub_08011AAC
 * takes its entry as `void *` (that is how src/decomp/c_08011AAC.c defines it),
 * so a function address has to be cast -- exactly the note carried on the
 * sibling pair sub_08011B34/sub_08011B5C. The pool word is the ADDRESS of
 * sub_080184A4, not a call to it.
 */

void sub_0802C270(void)
{
    sub_08011AAC((void *)sub_080184A4);
}

/* Installs one gUnknown_0200C528 list script. sub_080193B0 returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void sub_0802C280(void)
{
    sub_080193B0(gUnknown_0849A8F0);
}

/* The removal half of the pair: sub_0802C280 installs gUnknown_0849A8F0 through
 * sub_080193B0 and this drops it through sub_0801930C. sub_0801930C returns -1
 * unconditionally and `pop {r0}; bx r0` discards it, so this is void.
 */

void sub_0802C290(void)
{
    sub_0801930C(gUnknown_0849A8F0);
}
