#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808B184.
 * sub_0808B184 @ 0x0808B184
 */

/* The flash byte-program sequence: the two-cycle unlock at 0x0E005555 /
 * 0x0E002AAA, the 0xA0 program command, the byte itself, then the chip-specific
 * wait routine gUnknown_03005C70 that IdentifyFlash installed.
 *
 * `*src` really is loaded TWICE -- once for the store and once for the wait
 * call -- and that needs no `volatile`: the intervening `*dst = *src` may alias
 * `src`, so agbcc cannot reuse the first load.
 *
 * `bl _call_via_r3` is an ordinary indirect call with THREE arguments (the
 * register index counts them), and the `lsls #0x10; lsrs #0x10` after it is the
 * re-narrowing of that pointer's declared u16 return. */
u16 ProgramByte_MX(u8 *src, u8 *dst)
{
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;

    *dst = *src;

    return gUnknown_03005C70(1, dst, *src);
}
asm(".global sub_0808B184\n.thumb_set sub_0808B184, ProgramByte_MX\n");
