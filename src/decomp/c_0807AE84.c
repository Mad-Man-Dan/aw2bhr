#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807AE84.
 * sub_0807AE84 @ 0x0807AE84
 */

/* One of family F000's 16-byte forwarders, over the sub_0803B4DC sound-id
 * call. The id is 0x1DB, which does not fit `movs #imm8` and so arrives from the
 * literal pool -- the same pool `ldr` a symbol would produce, per
 * docs/agbcc-codegen.md, so the word says nothing beyond "wider than 255".
 * sub_0803B4DC takes an `int` (see include/unknown-functions.h); an `s16`
 * parameter would put `lsls #16; asrs #16` in this wrapper's prologue.
 * `pop {r0}; bx r0`, so void.
 */

void sub_0807AE84(void)
{
    sub_0803B4DC(0x1DB);
}
