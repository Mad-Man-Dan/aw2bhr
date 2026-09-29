#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032048.
 * sub_08032048 @ 0x08032048
 */

/* Three things here are structure and not noise:
 *
 * 1. The two byte globals are one CHAINED assignment, not two statements.
 *    Both pool words are materialised before the first store and
 *    gUnknown_03003F1C's comes FIRST, which is the outer assignment's lvalue
 *    address being evaluated ahead of its right-hand side; two separate
 *    statements load gUnknown_030044C4 first and swap the pool. The `ldrb`
 *    between the two stores is old gcc re-reading a VOLATILE lvalue for the
 *    value of the assignment expression -- a plain u8 forwards the 0 and emits
 *    no load at all. That is the whole evidence for the volatile.
 * 2. `(unk02 - 0xb4)` is computed TWICE, once for the modulo and once for the
 *    divide, and the ldrb of gPlaySt.mapID is repeated with it.
 *    Binding it to a local is what would cost bytes.
 * 3. gUnknown_0849B060 is reloaded between the two stores. The lvalue's
 *    address is evaluated before the right-hand side, so the copy kept in r5
 *    for unk08 predates __modsi3 and survives it in a callee-saved register --
 *    but that CSE is killed by the call, which is why unk09's store reloads.
 *
 * agbcc does not strength-reduce `/ 3` or `% 3`; the dividend is an `ldrb`
 * minus 180, i.e. int arithmetic that can go negative, hence the SIGNED
 * __divsi3 / __modsi3 rather than their unsigned twins. */

void LinkDecodeSharedMapId(void)
{
    gUnknown_03003F1C = gUnknown_030044C4 = 0;

    gUnknown_0849B060->unk02 = 3;
    gUnknown_0849B060->unk08 = (gPlaySt.mapID - 0xb4) % 3;
    gUnknown_0849B060->unk09 = (gPlaySt.mapID - 0xb4) / 3;
}
asm(".global sub_08032048\n.thumb_set sub_08032048, LinkDecodeSharedMapId\n");
