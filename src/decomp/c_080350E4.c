#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080350E4.
 * sub_080350E4 @ 0x080350E4
 */

/* gUnknown_03004490 is declared `u8 []` and shared, so its real shape is
 * spelled here as a cast rather than by reshaping that declaration: a scalar
 * triple at +0 and a per-team byte array at +3. The two pool words the ROM
 * carries (0x03004493 for the loop store, 0x03004490 for the trailing one)
 * are exactly the two address constants this spelling produces, and the
 * `.rodata` reloc naming gUnknown_03004490+3 where the ROM names
 * gUnknown_03004493 is the resolve-to-the-same-address false mismatch.
 *
 * The discarded `ldrh` of gUnknown_030033EC is a VOLATILE read. That global is
 * declared plain `u16` and a dozen matched functions read it, so the qualifier
 * goes at the use, not on the declaration. See docs/agbcc-codegen.md.
 *
 * THE STORE IS WRITTEN TWICE ON PURPOSE. One store compiles to the same 64
 * bytes but leaves the constant 0 rematerialised inside the loop and only two
 * callee-saved registers; the ROM hoists the 0 into r5 and takes a third. The
 * second (idempotent) store is what raises the address's reference count
 * enough for that allocation, and it is CSE'd away so it costs nothing. It is
 * a stand-in for whatever the original actually wrote there -- the shape is
 * right, the second statement is not claimed to be the original text. */
struct Unk03004490
{
    /* 0x00 */ u8 filler_00[3];
    /* 0x03 */ u8 unk03[5];
};

void ResetWeatherRoundCounters(void)
{
    u8 i;

    for (i = 1; i <= 4; i++)
    {
        if (IsPlayerAliveAndActive(i))
            *(vu16 *)&gUnknown_030033EC;

        ((struct Unk03004490 *)gUnknown_03004490)->unk03[i] = 0;
        ((struct Unk03004490 *)gUnknown_03004490)->unk03[i] = 0;
    }

    gUnknown_03004490[0] = 0x1e;
}
asm(".global sub_080350E4\n.thumb_set sub_080350E4, ResetWeatherRoundCounters\n");
