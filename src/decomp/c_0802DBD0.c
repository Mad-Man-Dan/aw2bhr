#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802DBD0.
 * sub_0802DBD0 @ 0x0802DBD0, sub_0802DBE4 @ 0x0802DBE4
 */

/* Three statements. RestoreMapCursorPosition and DecrementMapLock take nothing, so the
 * sub_0802776C(1) result cannot be flowing into either of them. */

void sub_0802DBD0(void)
{
    sub_0802776C(1);
    RestoreMapCursorPosition();
    DecrementMapLock();
}

/* The same one-line forwarder as the sub_0802D40C group: 0xC9E is >255 so
 * agbcc has no `movs #imm8` for it and the pool word is forced by the VALUE
 * alone -- no symbol and no type is involved. `pop {r0}`, so void. */

void sub_0802DBE4(void)
{
    sub_08019818(0xC9E, 0, 0);
}
