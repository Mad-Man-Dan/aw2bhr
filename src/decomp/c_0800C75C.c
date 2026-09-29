#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800C75C.
 * sub_0800C75C @ 0x0800C75C, sub_0800C7A4 @ 0x0800C7A4, GetPropertyKindForTerrain @ 0x0800C7E8, GetPropertyKindAt @ 0x0800C840
 */

/* The SET half of the pair ClearArmyHq clears: the same binary-search switch
 * mapping 0x28/0x48/0x68/0x88 to a slot index 0..3, then the caller's two
 * values into gActiveMap's unk17 and unk1b planes at that index.
 *
 * The extra `push {r4, lr}` over ClearArmyHq is the second argument being
 * carried across the decision tree; the third needs no saving because nothing
 * on the way clobbers r2.  Both stores are bare `strb`s, so the parameter
 * widths are a floor only and `int` is the weakest that fits. */
void SetArmyHq(int a, int b, int c)
{
    int i;

    switch (a)
    {
    case 0x28:
        i = 0;
        break;
    case 0x48:
        i = 1;
        break;
    case 0x68:
        i = 2;
        break;
    case 0x88:
        i = 3;
        break;
    default:
        return;
    }

    gActiveMap->hqX[i] = b;
    gActiveMap->hqY[i] = c;
}
asm(".global sub_0800C75C\n.thumb_set sub_0800C75C, SetArmyHq\n");

/* The CLEAR half of the SetArmyHq pair: same switch, same two planes, but
 * both slots take a "none" constant instead of arguments.
 *
 * A `switch`, not an if/else chain -- the repeated `cmp r0,#0x48` with `beq`
 * then `bgt` is gcc's binary-search decision tree, exactly as in
 * c_08016A2C.c.  The four arms only assign the index and merge, so the index
 * is one variable and not four duplicated bodies.  The default falls straight
 * to the function's bare `bx lr`, which is a `return` in the default arm and
 * not a guard ahead of the switch: a guard would have to name the four ids
 * twice.
 *
 * The two constants are written differently by the ROM -- `movs r1,#0xff` for
 * unk17 and `movs r1,#1; rsbs r1,r1,#0` for unk1b -- so the original source
 * really does spell one 0xFF and the other -1.  That says nothing about the
 * declared signedness of either member; both stores are byte-identical under
 * u8 or s8. */
void ClearArmyHq(int a)
{
    int i;

    switch (a)
    {
    case 0x28:
        i = 0;
        break;
    case 0x48:
        i = 1;
        break;
    case 0x68:
        i = 2;
        break;
    case 0x88:
        i = 3;
        break;
    default:
        return;
    }

    gActiveMap->hqX[i] = 0xFF;
    gActiveMap->hqY[i] = -1;
}
asm(".global sub_0800C7A4\n.thumb_set sub_0800C7A4, ClearArmyHq\n");

/* A classification of the low five bits of a terrain byte into 0, 1 or 2.
 * agbcc emits a jump table because the twelve labels 6..17 are dense, and the
 * six "1" cases share one body, so the table holds the same target six times
 * -- the same construction as GetIncomeForTerrainKind.
 *
 * The `movs r2, #0` sits AHEAD of the mask and the switch, and the three arms
 * converge on `adds r0, r2, #0`, so the result is ONE shared variable the
 * default arm falls through to -- not a `return` per arm, which would have
 * written r0 directly at each one.
 *
 * The lone `case 8` is written BEFORE the six-way group even though 8 sits
 * inside its value range, and the arm order is readable straight off the
 * bodies: gcc lays case bodies out in SOURCE order, so the arm that needs an
 * explicit `b` to the tail is the earlier one and the arm that falls through
 * is the later one.  The ROM has `movs r2,#2; b` first and `movs r2,#1`
 * falling through; writing the group first is 9 bytes different with the jump
 * table pointing the other way.
 *
 * The mask is this function's own: its caller GetPropertyKindAt masks too, but the
 * `movs r1,#0x1f; ands r1,r0` here is inside the callee, so the parameter is
 * unconstrained and `int`.
 *
 * A leaf: no `push`, and the epilogue is a bare `bx lr`. */
int GetPropertyKindForTerrain(int a)
{
    int r;

    r = 0;

    switch (a & 0x1F)
    {
    case 8:
        r = 2;
        break;
    case 6:
    case 10:
    case 11:
    case 14:
    case 16:
    case 17:
        r = 1;
        break;
    }

    return r;
}

/* sub_080016D0's shape over the BYTE plane instead of the halfword one: the
 * same `rowOffset[y] + x` addressing off gUnknown_08499590, the terrain byte
 * at +0x1432, and the low five bits of that byte handed to GetPropertyKindForTerrain.
 *
 * It RETURNS what GetPropertyKindForTerrain returns, and the epilogue is the only thing
 * that says so: `pop {r4}; pop {r1}; bx r1` keeps r0 intact, where a void
 * function would have popped the return address into r0 itself.
 *
 * The three binding locals are what keep the row table and the terrain plane
 * as separate address computations off one `p`; see c_0800164C.c for the same
 * arithmetic written the same way. */
int GetPropertyKindAt(int x, int y)
{
    int off;

    off = gMap->rowOffset[y] + x;
    return GetPropertyKindForTerrain(gMap->terrain[off] & 0x1F);
}

asm(".global sub_0800C7E8\n.thumb_set sub_0800C7E8, GetPropertyKindForTerrain\n"
    ".global sub_0800C840\n.thumb_set sub_0800C840, GetPropertyKindAt\n");
