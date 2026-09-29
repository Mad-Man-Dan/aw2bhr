#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CF48.
 * sub_0802CF48 @ 0x0802CF48, sub_0802CF6C @ 0x0802CF6C, sub_0802CF94 @ 0x0802CF94, sub_0802CFC0 @ 0x0802CFC0, sub_0802CFDC @ 0x0802CFDC
 */

/* sub_0802CFC0's four-call sibling: the same `if (!(a3 & 2))` guard on the same
 * three-argument callback signature, with a longer body. See the note on
 * sub_0802CFC0 for why the third parameter is u8 and the first two are a floor
 * rather than a reading.
 */

void sub_0802CF48(int a1, int a2, u8 a3)
{
    if (!(a3 & 2))
    {
        PushMenu();
        CloseTopMenu();
        sub_08034F10();
        sub_080485AC();
    }
}

/* Two teardown calls, then a guarded hand-off of gPlaySt.unk2e.
 *
 * Both member reads go through `adds r0, r1, #0; adds r0, #0x2e` (and #0x32)
 * rather than an `ldrb` displacement, because 0x2e and 0x32 are both past
 * `ldrb`'s 5-bit offset field. That is addressing, NOT the member-array tell --
 * the `adds` lands on a fresh copy of the base each time, not on the base
 * register itself. sub_0802C184 reads unk32 the same way.
 *
 * unk32 is the same guard sub_0802C184 and sub_08042998 test, so this is the
 * third independent reader of it, and unk2e is the payload it gates.
 */

void sub_0802CF6C(void)
{
    CloseTopMenu();
    sub_08042B9C();

    if (gPlaySt.savingEnabled != 0)
        sub_080344F0(gPlaySt.unk2e);
}

/* A wrapping 0-1-2 counter published to sub_08035020.
 *
 * gUnknown_08090C00 is NOT a global and must not be declared as one: the ROM
 * word at 0x08090C00 is 0x03003FC0, i.e. &gPlaySt, and its immediate
 * neighbours at 0x08090BF8/BFC/C04 hold &gUnknown_030033EC (twice) and
 * &gUnknown_030033E8 -- a `-fforce-addr` address-constant run. The honest
 * spelling is therefore just `gPlaySt.unk2c`, and since wave 18 agbcc
 * parks its own copy of the address in this unit's `.rodata` and the split
 * places it. That reproduces the ROM's double indirection (`ldr r1, =word;
 * ldr r0, [r1]`) with no `*const` pointer declaration and no c_local
 * workaround.
 *
 * The compare is `bhi`/`bls`, an UNSIGNED ordering, which agrees with unk2c's
 * declared u8. The test is written `> 1` with the reset as the THEN arm: the
 * `<= 1` spelling emits `bhi` to the reset instead and puts the increment on
 * the fallthrough, which is the mirror of the ROM. The single `strb` after the
 * merge is agbcc cross-jumping the two stores, not a conditional expression.
 */

void sub_0802CF94(void)
{
    if (gPlaySt.weather > 1)
        gPlaySt.weather = 0;
    else
        gPlaySt.weather++;

    sub_08035020(gPlaySt.weather);
}

/* A three-argument callback that acts only when bit 1 of its third argument is
 * clear. sub_0802CF48, sub_0802CD28, sub_0802CD54 and sub_0802CD78 in this same
 * block share the shape.
 *
 * The third parameter is u8 and this is the clean case of the wave-21 rule:
 * `lsls r2,#0x18; lsrs r2,#0x18` at entry is PROMOTE_MODE's unconditional
 * zero-extension, which an `int` parameter would not pay. The first two
 * parameters are never read -- r0 and r1 are written before use -- so their
 * count is a floor taken from the third one's register index, and `int` is the
 * weakest model for both.
 *
 * CloseTopMenu returns int and the result is discarded, so both calls are bare
 * statements and this is void.
 */

void sub_0802CFC0(int a1, int a2, u8 a3)
{
    if (!(a3 & 2))
    {
        CloseTopMenu();
        sub_0802C280();
    }
}

/* sub_0802C1D0's twin, one block down: the same
 * `sub_08016D04(gPlaySt.gameMode)` result handed to a u16-taking
 * sub_08016Dxx entry, with the fused `lsls #0x18; asrs #8; lsrs #0x10` s8-to-u16
 * conversion between the two `bl`s. LoadSuspendSave's own prologue
 * (`lsls r0,#0x10; lsrs r0,#0x10`) confirms the u16 independently of the call
 * site.
 *
 * The narrowing is the only thing between the second and third calls, so that
 * pair is genuine nesting; CloseTopMenu in front of it is a separate statement
 * whose int result is discarded. `pop {r0}; bx r0`, so void.
 */

void sub_0802CFDC(void)
{
    CloseTopMenu();
    LoadSuspendSave(sub_08016D04(gPlaySt.gameMode));
}
