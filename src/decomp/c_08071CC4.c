#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071CC4.
 * sub_08071CC4 @ 0x08071CC4, sub_08071CDC @ 0x08071CDC
 */

/*
 * MATCHES under the default compiler, first attempt. Wave 38 (W38-A extension).
 *
 * Applies the wave-27 palette writer SetBlackPal to all 32 palette slots.
 * `cmp r4,#0x1f; ble` is on the ALREADY-INCREMENTED counter, so it is the
 * ordinary `i < 32`, not `i <= 31` -- and the missing entry guard is agbcc
 * turning a for-loop with a known-nonzero trip count into a do/while. Neither
 * is a source feature to reproduce.
 *
 * SetAllWhitePals is the same function over SetWhitePal (the other palette).
 */
void SetAllBlackPals(void)
{
    int i;

    for (i = 0; i < 32; i++)
        SetBlackPal(i);
}
asm(".global sub_08071CC4\n.thumb_set sub_08071CC4, SetAllBlackPals\n");

/*
 * MATCHES under the default compiler, first attempt. Wave 38 (W38-A extension).
 * The twin of SetAllBlackPals -- byte-identical apart from the callee, which is
 * SetWhitePal (the second palette) rather than SetBlackPal. See that file.
 */
void SetAllWhitePals(void)
{
    int i;

    for (i = 0; i < 32; i++)
        SetWhitePal(i);
}
asm(".global sub_08071CDC\n.thumb_set sub_08071CDC, SetAllWhitePals\n");
