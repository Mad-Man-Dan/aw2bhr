#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807F57C.
 * sub_0807F57C @ 0x0807F57C
 */

/* MATCHED, wave 88 (W88-A). 156 bytes, byte-for-byte, relocs match, first
 * attempt -- and with NOT ONE CHARACTER of this function changed. Parked since
 * wave 4 and classified in wave 73 as residual kind 5, "cross-TU prototype
 * contract, no spelling of this caller reaches it, ORCHESTRATOR DECISION".
 * The classification was right; the conclusion that it was unreachable was not.
 *
 * Walks gUnknown_086166F0, a run of byte ids split into groups by an 0xFF
 * sentinel with one label byte after each sentinel and a second 0xFF ending
 * the run. Each group's unlocked members (IsCoUnlocked) are appended to the
 * gUnknown_030058E0 display list; a non-empty group also records its label in
 * gUnknown_03005958 and its count in gUnknown_03005948 at the
 * gUnknown_03005944 cursor.
 *
 * WHAT CLOSED IT -- the bounded contract change, not a spelling:
 *   1. include/unknown-functions.h: `u8 IsCoUnlocked(u32)` -> `int` (that
 *      symbol only; IsMapCategoryUnlocked and the rest of the bit-reader family stay u8)
 *   2. src/decomp/c_0803CA9C.c: the definition retyped to `int`, BYTE-UNCHANGED
 *      -- it returns `(1 << (id & 7)) & *p`, whose nonzero_bits are provably
 *      <= 0xff, so no narrowing is emitted at either width. That is the body
 *      evidence, and it permits int.
 *   3. the ten callers that DO narrow now say `(u8)IsCoUnlocked(...)`: nine
 *      sites in c_0803C354.c and one in c_08043CA0.c.
 *
 * ALL THIRTEEN affected functions verify byte-for-byte by exit code
 * (ShopAvail_CoNotUnlocked/C48C/C4B4/C4DC/C504/C52C/C598/C5C0/C5E8, BuildUnlockedCoList,
 * IsMapCategoryUnlocked, IsCoUnlocked, and this one); proto_check is clean. Details in
 * work/BuildWarRoomCoSelectGroupList/W88-notes.md.
 *
 * THE RULE: `(u8)f(x)` and an implicit u8 return emit the SAME narrowing, so a
 * wide declaration plus explicit casts is byte-neutral in both directions and
 * a single shared header CAN express a per-file prototype divergence. The
 * wave-59/73 claim that it cannot was about the implicit form only. Do not park
 * a residual as kind 5 before measuring the cast form.
 */
void BuildWarRoomCoSelectGroupList(void)
{
    int i;
    int k;
    int n;

    i = 0;
    k = 0;
    gUnknown_03005944 = 0;
    SetCoSelectGroupSwitchAll();

    while (gUnknown_086166F0[i] != 0xff)
    {
        n = 0;

        while (gUnknown_086166F0[i] != 0xff)
        {
            if (IsCoUnlocked(gUnknown_086166F0[i]))
            {
                n++;
                gUnknown_030058E0[k] = gUnknown_086166F0[i];
                k++;
            }

            i++;
        }

        i++;

        if (n != 0)
        {
            gUnknown_03005958[gUnknown_03005944] = gUnknown_086166F0[i];
            gUnknown_03005948[gUnknown_03005944] = n;
            gUnknown_03005944++;
        }

        i++;
    }
}
asm(".global sub_0807F57C\n.thumb_set sub_0807F57C, BuildWarRoomCoSelectGroupList\n");
