#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AD6C.
 * sub_0808AD6C @ 0x0808AD6C
 */

/* SetReadFlash1 -- the agb_flash idiom, and the C below is unchanged from the
 * parked draft.
 *
 * MATCHES -- but only when compiled WITHOUT `-fforce-addr`. It does NOT match
 * under the default build flags, so `try_match` and a plain
 * `python tools/trymatch.py SetReadFlash1` both still report 85.7% and fail.
 *
 * Verified byte-for-byte (bytes and relocations) with:
 *
 *   CFLAGS = the Makefile's CFLAGS with `-fforce-addr` removed
 *
 * Either compiler binary works -- agbcc and old_agbcc both match once the flag
 * is gone, and both miss by the same 8 bytes with it. So this one is purely a
 * flag, where the m4a pair (MPlayTempoControl, MP_clear_modM) is purely a binary.
 *
 * The flag is exactly the axis the parked note said was missing. The ROM loads
 * the `ReadFlash1` pool word TWICE -- once as the eor's destination, which
 * the eor clobbers, and once again for the subtraction:
 *
 *     ldr r3,=ReadFlash1 ; movs r0,#1 ; eors r3,r0
 *     ldr r0,=SetReadFlash1 ; ldr r1,=ReadFlash1 ; subs r0,r0,r1
 *
 * With `-fforce-addr` the symbol's address is forced into a pseudo before the
 * loop's MEM is built, CSE unifies the second reference with it, and the
 * clobber has to be paid for with a register copy:
 *
 *     ldr r3,=ReadFlash1 ; adds r1,r3,#0 ; movs r0,#1 ; eors r3,r0
 *
 * Same 56 bytes, same instruction count, two wrong instructions. Twenty-three
 * source spellings, five probes and a permuter run could not reach the
 * re-load, because no source spelling can: the flag decides it.
 *
 * Note the earlier note's conclusion "agbcc's CSE unifies two source references
 * to one symbol unconditionally" is too strong -- it is `-fforce-addr` that
 * does it, and only when one of the references is also used as a memory
 * address. Two purely-integer references to one symbol emit one pool load with
 * the flag and one pool load without it. See docs/agbcc-codegen.md.
 *
 * Promotion is blocked on a build-system decision, not on the C: the Makefile
 * needs a per-file `CFLAGS` override for whichever src/decomp file this lands
 * in. No promoted function above 0x08063A3C is `-fforce-addr`-sensitive, so an
 * override here costs nothing.
 */
void SetReadFlash1(u16 *dst)
{
    const u16 *src;
    u16 n;

    gUnknown_03000F6C = (u8 (*)(u8 *))((u8 *)dst + 1);

    src = (const u16 *)ReadFlash1;
    src = (const u16 *)((u32)src ^ 1);
    n = (u16 *)SetReadFlash1 - (u16 *)ReadFlash1;

    while (n != 0)
    {
        *dst++ = *src++;
        n--;
    }
}
asm(".global sub_0808AD6C\n.thumb_set sub_0808AD6C, SetReadFlash1\n");
