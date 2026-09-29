#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C6CC.
 * sub_0802C6CC @ 0x0802C6CC, sub_0802C6FC @ 0x0802C6FC, sub_0802C72C @ 0x0802C72C, sub_0802C75C @ 0x0802C75C
 */

/* Family F046: four copies of one predicate whose whole difference is the
 * constant gPlaySt.unk09 is compared against (0, 1, 2, 3) -- the
 * single entry in data/families.json's `varies` for this family.
 *
 * The explicit `if (...) return TRUE;` chain and not `return a || b || c;`:
 * both 0 and 1 are materialised and the literal pool sits BETWEEN the two
 * return blocks, which per docs/agbcc-codegen.md is the four-byte-longer
 * if/return form. Probed against the one-`||` spelling in the same file --
 * that one inverts the last branch and puts the pool after the `return 1`
 * block, i.e. it is the short fallthrough shape.
 *
 * The two call-site narrowings are the callees' declared bool8 returns:
 * `lsls #0x18; lsrs #0x18; cmp #1` is the value kept for a compare against a
 * non-zero constant, `lsls #0x18; cmp #0` alone is a truth test.
 *
 * Nothing in the ROM calls any of the four, so the return type is a free
 * choice -- the body returns literal 0/1, so `int` is byte-identical and
 * bool8 is not proved. */

bool8 OptionsMenu_NoVisualUsability(void)
{
    if (IsLinkGame() == TRUE)
        return TRUE;

    if (IsMapCategoryZero())
        return TRUE;

    if (gPlaySt.animOpts != 0)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802C6CC\n.thumb_set sub_0802C6CC, OptionsMenu_NoVisualUsability\n");

/* Family F046: four copies of one predicate whose whole difference is the
 * constant gPlaySt.unk09 is compared against (0, 1, 2, 3) -- the
 * single entry in data/families.json's `varies` for this family.
 *
 * The explicit `if (...) return TRUE;` chain and not `return a || b || c;`:
 * both 0 and 1 are materialised and the literal pool sits BETWEEN the two
 * return blocks, which per docs/agbcc-codegen.md is the four-byte-longer
 * if/return form. Probed against the one-`||` spelling in the same file --
 * that one inverts the last branch and puts the pool after the `return 1`
 * block, i.e. it is the short fallthrough shape.
 *
 * The two call-site narrowings are the callees' declared bool8 returns:
 * `lsls #0x18; lsrs #0x18; cmp #1` is the value kept for a compare against a
 * non-zero constant, `lsls #0x18; cmp #0` alone is a truth test.
 *
 * Nothing in the ROM calls any of the four, so the return type is a free
 * choice -- the body returns literal 0/1, so `int` is byte-identical and
 * bool8 is not proved. */

bool8 OptionsMenu_VisualAUsability(void)
{
    if (IsLinkGame() == TRUE)
        return TRUE;

    if (IsMapCategoryZero())
        return TRUE;

    if (gPlaySt.animOpts != 1)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802C6FC\n.thumb_set sub_0802C6FC, OptionsMenu_VisualAUsability\n");

/* Family F046: four copies of one predicate whose whole difference is the
 * constant gPlaySt.unk09 is compared against (0, 1, 2, 3) -- the
 * single entry in data/families.json's `varies` for this family.
 *
 * The explicit `if (...) return TRUE;` chain and not `return a || b || c;`:
 * both 0 and 1 are materialised and the literal pool sits BETWEEN the two
 * return blocks, which per docs/agbcc-codegen.md is the four-byte-longer
 * if/return form. Probed against the one-`||` spelling in the same file --
 * that one inverts the last branch and puts the pool after the `return 1`
 * block, i.e. it is the short fallthrough shape.
 *
 * The two call-site narrowings are the callees' declared bool8 returns:
 * `lsls #0x18; lsrs #0x18; cmp #1` is the value kept for a compare against a
 * non-zero constant, `lsls #0x18; cmp #0` alone is a truth test.
 *
 * Nothing in the ROM calls any of the four, so the return type is a free
 * choice -- the body returns literal 0/1, so `int` is byte-identical and
 * bool8 is not proved. */

bool8 OptionsMenu_VisualBUsability(void)
{
    if (IsLinkGame() == TRUE)
        return TRUE;

    if (IsMapCategoryZero())
        return TRUE;

    if (gPlaySt.animOpts != 2)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802C72C\n.thumb_set sub_0802C72C, OptionsMenu_VisualBUsability\n");

/* Family F046: four copies of one predicate whose whole difference is the
 * constant gPlaySt.unk09 is compared against (0, 1, 2, 3) -- the
 * single entry in data/families.json's `varies` for this family.
 *
 * The explicit `if (...) return TRUE;` chain and not `return a || b || c;`:
 * both 0 and 1 are materialised and the literal pool sits BETWEEN the two
 * return blocks, which per docs/agbcc-codegen.md is the four-byte-longer
 * if/return form. Probed against the one-`||` spelling in the same file --
 * that one inverts the last branch and puts the pool after the `return 1`
 * block, i.e. it is the short fallthrough shape.
 *
 * The two call-site narrowings are the callees' declared bool8 returns:
 * `lsls #0x18; lsrs #0x18; cmp #1` is the value kept for a compare against a
 * non-zero constant, `lsls #0x18; cmp #0` alone is a truth test.
 *
 * Nothing in the ROM calls any of the four, so the return type is a free
 * choice -- the body returns literal 0/1, so `int` is byte-identical and
 * bool8 is not proved. */

bool8 OptionsMenu_VisualCUsability(void)
{
    if (IsLinkGame() == TRUE)
        return TRUE;

    if (IsMapCategoryZero())
        return TRUE;

    if (gPlaySt.animOpts != 3)
        return TRUE;

    return FALSE;
}
asm(".global sub_0802C75C\n.thumb_set sub_0802C75C, OptionsMenu_VisualCUsability\n");
