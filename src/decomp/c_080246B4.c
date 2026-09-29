#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080246B4.
 * sub_080246B4 @ 0x080246B4
 */

/* MATCHED -- wave 35, W35-G. 108/108 bytes.
 *
 * PROMOTION NOTE: the reloc at +0x28 is the expected -fforce-addr pool word for
 * gUnknown_08090A2C (whose ROM word holds 0x03004008). This function's
 * data/promoted.json entry needs   "rodata": ["0x08090A2C"]   and then
 * tools/split_rodata.py + tools/gen_lds.py before building.
 *
 * THE LEVER: the ROM's switch tree is
 *   cmp #1 / beq case1 / cmp #1 / ble default / cmp #2 / beq case2 / b default
 * The second test is gcc's LOW BOUND test. W35-C worked out the gate correctly
 * from emit_case_nodes -- a node with a right child and no left child only
 * takes the general path (and so only emits the bound test) when node->right
 * has children or is a range -- but then searched for a RANGE and parked.
 *
 * What it needs is a THIRD case node BELOW the root. With two cases the node
 * list is two long, balance_case_nodes does not split it (`if (i > 2)`), and
 * the root is node 1 with a single-valued leaf as its right child -> shortcut
 * path, no bound test. Adding `case 0: break;` makes i == 3, balance splits "at
 * the middle one", node 1 becomes the root with node 0 on the left and node 2
 * on the right, and the general path emits the ROM's shape exactly.
 *
 * `case 4:` instead of `case 0:` gives root 2 and the WRONG shape -- the extra
 * node must be below the lowest real case, not above the highest. Probed on a
 * standalone three-way switch before being tried here.
 *
 * This is the decision-tree form of the lever already recorded in
 * src/decomp/c_08043DAC.c ("`case 0:` sharing the `default:` label is what puts
 * the bound test into the tree") and in the W35-A jump-table chapter of
 * docs/agbcc-codegen.md. Grep the doc for `case` before deriving a switch.
 *
 * Settled by W35-C and unchanged here: gUnknown_08090A2C is a force-addr word
 * over the s32 gGameClock; the shift is LOGICAL (`(u32)g >> 2`; a plain
 * `>> 2` emits `asrs`); `Div(g, 20) % 4` is a signed remainder and its
 * round-toward-zero correction is byte-exact. */

void sub_080246B4(void)
{
    int mode;

    mode = gUnknown_08499CBC[((u32)gGameClock >> 2) & 0xf];

    switch (mode)
    {
    case 0:
        break;

    case 1:
        sub_0803F880(1, 8);
        break;

    case 2:
        sub_0803F880(0, 8);
        break;
    }

    sub_0803F8E0(0x48, Div(gGameClock, 20) % 4);
    AnimateVolcanoTiles(0x48, gGameClock);
}
