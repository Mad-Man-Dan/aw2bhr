#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803A42C.
 * sub_0803A42C @ 0x0803A42C
 */

/* Family F035 (data/families.json): `push {lr}; bl A; movs r0,#0; bl B; bl C;
 * pop {r0}; bx r0` -- 20 bytes, five members, and `varies` lists only the three
 * `bl` targets, so the `movs r0, #0` is byte-identical in every member.
 *
 * THREE STATEMENTS, not a nest. Sequential `bl`s carry no nesting signal at all
 * (docs/agbcc-codegen.md: r0 already holds the result, so the nest is the
 * spelling that needs no instruction), and the evidence is entirely callee-side:
 *   - callee A writes r0 before reading it, so it is nullary;
 *   - callee B does take an argument, but it is given an explicit `movs r0,#0`,
 *     which is what makes A's result dead;
 *   - callee C writes r0 before reading it, so a nest is not expressible.
 * `pop {r0}` is the void epilogue, so nothing is returned either.
 */


/* BG_EnableSyncBG2 and SetMapLayerPrioritiesDefault are both nullary (each writes r0 before any
 * read); sub_08013AD4 takes the u8 its promoted definition in
 * src/decomp/c_08013AD4.c declares. The same three-call sequence appears
 * open-coded at 0x0803A3B0 and 0x0801A414 in asm/.
 */

void UnitInfoPanel_Close(void)
{
    BG_EnableSyncBG2();
    sub_08013AD4(0);
    SetMapLayerPrioritiesDefault();
}
asm(".global sub_0803A42C\n.thumb_set sub_0803A42C, UnitInfoPanel_Close\n");
