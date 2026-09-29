#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080470F8.
 * sub_080470F8 @ 0x080470F8
 */

/* Enter a map: latch the map id and its companion byte, decide whether the id
 * is one of the "special" ones, restart the two proc scripts, then set the
 * cursor-nudge flag from how close the camera already is to the top of the
 * map.
 *
 * The range test is a SWITCH, not an `if` chain, and that is the whole
 * derivation: agbcc's expand_case emits SIGNED compares against the case
 * bounds because the promoted index type is `int`, while any `&&`/`||`
 * spelling of the same ranges is folded by combine into UNSIGNED tests with
 * the constants shifted by one (`cmp #0xe; bls`) because the index is a u8 and
 * provably non-negative. group_case_nodes merges the consecutive labels into
 * the two ranges 15..16 and 21..31, and emit_case_nodes emits them in exactly
 * the ROM's order -- low bound, high bound, then the second node's HIGH bound
 * before its low. See docs/agbcc-codegen.md, "Switch statements".
 *
 * `*(s16 *)(gUnknown_08499590 + 4)` follows the house idiom for that symbol:
 * the byte arithmetic stays on the `u8 *` and the width comes from the cast.
 * The `bge; adds #0xf; asrs #4` is a signed divide by 16, which is what makes
 * the field s16 rather than u16.
 *
 * The last if/else is written `<= 6` and not `> 6`: which arm FALLS THROUGH is
 * the discriminator, and the ROM falls through to the 0x80 store. */
void ShowTerrainInfoWindow(u16 a1)
{
    gUnknown_02028DD6 = a1 & 0x1f;
    gUnknown_02028DD7 = GetCellCountry(a1);
    gUnknown_02028DD4 = 1;

    switch (gUnknown_02028DD6)
    {
    case 15:
    case 16:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
        gUnknown_02028DD4 = 0;
        break;
    }

    RebuildMapUnitLayers2();
    sub_080152EC(gUnknown_084C2140, 0);
    sub_080152EC(gUnknown_084C2198, 0);

    if (gUnknown_030033E4.unk00 - gMap->scrollX / 16 <= 6)
        gUnknown_02028DD5 = 0x80;
    else
        gUnknown_02028DD5 = 0;
}
asm(".global sub_080470F8\n.thumb_set sub_080470F8, ShowTerrainInfoWindow\n");
