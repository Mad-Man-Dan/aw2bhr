#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AE54.
 * sub_0808AE54 @ 0x0808AE54
 */

#include "hardware.h"

/* MATCHED, wave 79 (W79-B), under the compiler override recorded for this
 * function in data/compiler-overrides.json (-O2 and -fforce-addr removed, -O1
 * added) -- the same configuration its two matched siblings VerifyFlashSector and
 * VerifyFlashSectorNBytes carry.
 *
 * The relocating trampoline for ReadFlash_Core (the SRAM read): it copies a
 * SIBLING FUNCTION'S MACHINE CODE onto the stack and calls it from there so the
 * SRAM access runs out of RAM.
 *
 * TWO THINGS CLOSED IT, and both were needed.
 *
 * 1. THE PROFILE. Every score and every ruled-out axis recorded against this
 *    function before wave 60 was measured at -O2, which is the wrong
 *    configuration for the flash library, so all of it was INVERTED evidence.
 *    At -O2 agbcc hoists `mov r3,sp; adds r3,#1` (the entry address of the
 *    relocated copy) into the copy loop's preheader; the value is then live
 *    across the loop, the whole function is allocated one register to the left,
 *    and two further symptoms follow from it -- a duplicated loop exit test
 *    where the ROM shares one bottom test, and one load of the copied
 *    function's pool word where the ROM has two. At -O1 none of that happens
 *    and the loop, the guard and the tail are exact with no source change.
 *
 * 2. THE ARGUMENT'S ASSOCIATION, which the profile alone does not fix (9 of 124
 *    bytes remained). The ROM computes `a2 + 0x0E000000` into a register of its
 *    OWN and adds the shifted sector to that:
 *        adds r0,r4,#0 / movs r2,#224 / lsls r2,r2,#20 / adds r1,r5,r2
 *                      / adds r0,r0,r1
 *    Written as a plain sum in any bracketing, agbcc pulls the literal onto the
 *    accumulator instead -- `(shifted + C) + a2` -- because fold's split_tree
 *    reassociates through the parentheses. Fourteen spellings were measured at
 *    -O2 and re-checked here; the two that matter at -O1 are:
 *      `shifted + (a2 + C)`      right registers, WRONG association (8 bytes)
 *      `shifted + (a2 += C)`     right association, WRONG registers (9 bytes),
 *                                because the extra set on a2 raises its allocno
 *                                above a1's and swaps r4/r5
 *      `shifted + (t = a2 + C)`  BOTH. Binding the sum to a SEPARATE local
 *                                inside the argument expression gives the sum
 *                                its own pseudo (so it is not reassociated)
 *                                without adding a reference to a2 (so the
 *                                callee-saved assignment is unchanged).
 *
 * GENERAL RULE, now in docs/agbcc-codegen.md: to force agbcc to keep `x + C` as
 * its own subexpression inside a larger sum, assign it to a fresh local IN
 * PLACE inside the expression. A separate statement computes it too early, and
 * `x += C` gets the association at the cost of x's register. */
void ReadFlash(u16 a1, int a2, int a3, int a4)
{
    u16 buf[0x40];
    const u16 *s;
    u16 *d;
    u16 n;
    int t;

    REG_WAITCNT = (REG_WAITCNT & 0xFFFC) | 3;

    s = (const u16 *)ReadFlash_Core;
    s = (const u16 *)((u32)s ^ 1);
    d = buf;
    n = ((u32)ReadFlash - (u32)ReadFlash_Core) / 2;

    while (n != 0)
    {
        *d++ = *s++;
        n--;
    }

    ((void (*)(const u8 *, u8 *, int))((int)buf + 1))(
        (const u8 *)((a1 << gUnknown_08485550.unk1c) + (t = a2 + 0x0E000000)),
        (u8 *)a3, a4);
}
asm(".global sub_0808AE54\n.thumb_set sub_0808AE54, ReadFlash\n");
