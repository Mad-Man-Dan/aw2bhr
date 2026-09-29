#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072B2C.
 * sub_08072B2C @ 0x08072B2C
 */

/* Maps a 0..0xef screen coordinate onto -0x60..0x5f, clamping at both ends.
 * `lsls #1; adds; lsls #6` is agbcc's expansion of `* 0xc0` -- (2a + a) << 6 --
 * and not a hand-rolled shift chain; the multiply spelling is what produces it.
 * Div is the BIOS-style signed divide, already declared s32(s32, s32). */
int Screen2Pan(int a)
{
    if (a < 0)
        return -0x60;

    if (a > 0xef)
        return 0x5f;

    return Div(a * 0xc0, 0xf0) - 0x60;
}
asm(".global sub_08072B2C\n.thumb_set sub_08072B2C, Screen2Pan\n");
