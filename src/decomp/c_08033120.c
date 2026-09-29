#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08033120.
 * sub_08033120 @ 0x08033120, sub_08033150 @ 0x08033150, sub_08033174 @ 0x08033174
 */

/* Six teardown calls in ROM order, then the two BG scroll shadows cleared.
 * Both globals are `volatile u16` but the stores are bare scalar assignments,
 * which is byte-identical with or without the qualifier -- the volatile on
 * these two was settled elsewhere (see include/unknown-globals.h). */
void sub_08033120(void)
{
    sub_08013C00();
    sub_08013C54();
    sub_08013CA8();
    sub_08013AEC();
    sub_08013AFC();
    sub_08013B0C();

    gUnknown_03002F18 = 0;
    gUnknown_03002B34 = 0;
}

/* Seven bare `bl`s, every result discarded. Note the order: sub_08032D60 runs
 * BEFORE sub_08032468, which is not the address order the callee list is
 * printed in. */
void sub_08033150(void)
{
    sub_08031430();
    sub_08011B18();
    sub_08031CE4();
    sub_08031E6C();
    sub_08032D60();
    sub_08032468();
    UnlockMainMenu();
}

/* 0xFFD0 is a POSITIVE literal and not -48: SetBgScrollShadow's second parameter is
 * `u16`, and the ROM materialises the value with a pool `ldr`. A -48 would have
 * been `movs r1,#0x30; rsbs r1,r1,#0`, two instructions and no pool word.
 *
 * The pool `ldr` landing before both `movs` is argument setup grouped by
 * operand class, not argument order. */
void sub_08033174(void)
{
    SetBgScrollShadow(0, 0xFFD0, 0);
    SetBgScrollShadow(3, 0, 0);
}
