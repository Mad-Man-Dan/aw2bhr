#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808AC7C.
 * sub_0808AC7C @ 0x0808AC7C
 */

/* MATCHED in wave 79 (W79-D) WITH NOT ONE CHARACTER OF C CHANGED. The wave-46
 * source below was always correct; what was wrong was the configuration.
 *
 * NEEDS THE FLASH BLOCK'S OVERRIDE -- -O2 AND -fforce-addr removed, -O1 added --
 * now recorded in data/compiler-overrides.json, the same entry its thirteen
 * neighbours IdentifyFlash..sub_0808B540 carry.
 *
 * This function was parked for thirty-three waves behind "TOOLCHAIN AXIS RULED
 * OUT (wave 46, W46-I) ... do not re-run it". That sweep held the C constant
 * across five configurations -- default, -fprologue-bugfix off, -fforce-addr
 * off, old_agbcc, old_agbcc + no force-addr -- and EVERY ONE OF THEM HOLDS -O2.
 * The optimisation level was never varied. Wave 60 separately established that
 * the flash library is -O1 and matched nine siblings on the flag alone, but its
 * probe kept -fforce-addr on and recorded this function as "gets LONGER under
 * -O1", which read as a second refutation. Removing both flags matches.
 *
 * old_agbcc really is byte-identical to the default here: the compiler BINARY
 * is not the axis, the OPTIMISATION LEVEL is.
 */
#include "global.h"
#include "hardware.h"

/* PARKED at 47.0%, SIZE-EXACT (168 bytes), instruction list right apart from
 * two things. The 0x03000F68-0x03000F7C module note in unknown-globals.h
 * describes the semantics; none of that is in doubt.
 *
 * Residual 1 -- an EXTRA `adds r0,r4,#2; mov ip,r0` hoisted to the timer-stop
 * store, and `mov r0,ip` where the ROM recomputes `adds r0,r4,#2` at the point
 * of use. `gUnknown_03000F78[1] = 0` and the `p + 1` inside `*p++ = ...` are
 * the SAME expression, so agbcc CSEs the first into a pseudo and keeps it live
 * across eight statements; by then no lo register is free, so it lands in ip.
 * The ROM has no such pseudo -- one live value fewer, which is the wave-17
 * "a value the ROM recomputes in a scratch" diagnostic read backwards.
 *
 * Residual 2 -- the ROM copies the shifted bit into r1 (`adds r1,r0,#0`) and
 * loads REG_IE into r0, so `orrs r0,r1` ties the result to REG_IE's register.
 * Both `REG_IE |= x` and `REG_IE = REG_IE | x` tie it to the shift's register.
 *
 * THE POST-INCREMENT PAIR IS LOAD-BEARING AND MUST NOT BE REWRITTEN. Three
 * spellings have now been measured against it and all three are worse, for two
 * different reasons:
 *
 *  - `*p = x; p++; *p = y; p--;` (wave 33): 35.7%, 4 bytes SHORT. RELOADS
 *    gUnknown_03000F78 from memory between the two stores; the ROM reads the
 *    pointer variable once and writes it twice.
 *  - a `vu16 *p` local plus `gUnknown_03000F78 = p + 1;` (wave 34): 33.9%,
 *    4 bytes LONG.
 *  - a `vu16 **pp` local plus `*pp = p + 1;` (wave 34): 38.7%, 8 bytes SHORT.
 *
 * The last two fail the same way and it is worth stating precisely, because the
 * second was chosen specifically to defeat the first: with the pointer bound to
 * a LOCAL, agbcc proves the intervening `p[1] = e[1]` cannot alias the pointer
 * variable and DELETES the `gUnknown_03000F78 = p + 1` store as dead -- killing
 * the ROM's `adds r0,r4,#2; str` outright. Routing the store through `*pp` does
 * not restore it: agbcc still sees a store to the same object. Only the
 * post-increment on the global itself keeps both writes, because there the
 * write-back is part of the same expression as the aliasing store. That also
 * matches the `*g++ = 0; *g-- = 0;` idiom already recorded for StopFlashTimer on
 * this same global. These are dead ends, not near misses -- do not re-run them.
 *
 * MEASURED AND WORSE for residual 2 (wave 34, 33.9%): binding the shifted bit
 * to its own `int bit` local before `REG_IE |= bit`. It does not produce the
 * ROM's `adds r1,r0,#0`; it reorders REG_IE's own pool load and disturbs the
 * sl/sb/r8 assignment across the whole body. Residual 2 is not reachable from
 * that statement's spelling.
 *
 * WAVE 44 (W44-G) TESTED THE FIX THIS NOTE ITSELF PROPOSED, AND IT IS WRONG.
 * The note below used to end by predicting that residual 1 "wants a spelling of
 * the timer-stop store that reaches +2 as a load DISPLACEMENT without creating
 * `p + 1` as a reusable pseudo -- most likely a struct type over the timer
 * register pair". That was tried in the least invasive form available, a
 * struct-typed VIEW cast in this file only (leaving the global's `vu16 *` type
 * alone, so the `*p++` / `*p--` pair still works):
 *
 *     struct Unk3000F78Timer { vu16 count; vu16 control; };
 *     ((struct Unk3000F78Timer *)gUnknown_03000F78)->control = 0;
 *
 * Result: 32.1%, +4 bytes, WORSE. A COMPONENT_REF does NOT reach +2 as a bare
 * displacement -- `adds r0,r4,#2; mov ip,r0` is emitted exactly as before, so
 * residual 1 is untouched -- and the volatile struct member assignment ALSO
 * emits a spurious `ldrh r0,[r4,#2]` read before the store, costing the 4
 * bytes. So the ARRAY_REF-vs-COMPONENT_REF distinction is not the lever, and
 * retyping the global (which would break the `*p++` pair and touch this
 * global's other readers) would not have helped either. Do not re-run it.
 *
 * WHAT IS LEFT, restated without the refuted prediction: residual 1 is agbcc
 * CSEing `p + 1` across eight statements when the ROM does not. Nothing tried
 * in four waves changes WHERE that pseudo is created; what has never been tried
 * is changing whether the two occurrences are the same expression at all --
 * i.e. a timer-stop store that does not go through gUnknown_03000F78. Whether
 * the ROM even had one is unknown; the data_refs list offers no other pointer.
 *
 * NOTE the three-local address/value/read lever that closed CoPowerScreenBlend_FadeInLoop,
 * CoPowerScreenBlend_FadeOutLoop and sub_08085F94 in wave 34 does NOT apply here: this function's
 * `mov r8, r0` already shows the address bound to a local, and the residual is
 * about a CSE of an offset expression rather than about which pseudo wins a
 * callee-saved register. */
void StartFlashTimer(u8 slot)
{
    u16 *e;

    e = gUnknown_03000F68 + slot * 3;

    gUnknown_03000F7C = REG_IME;
    REG_IME = 0;

    gUnknown_03000F78[1] = 0;
    REG_IE |= 8 << gUnknown_03000F70;
    gUnknown_03000F74 = 0;

    gUnknown_03000F72 = *e++;
    *gUnknown_03000F78++ = e[0];
    *gUnknown_03000F78-- = e[1];

    REG_IF = 8 << gUnknown_03000F70;
    REG_IME = 1;
}
asm(".global sub_0808AC7C\n.thumb_set sub_0808AC7C, StartFlashTimer\n");
