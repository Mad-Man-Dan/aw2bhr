#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800553C.
 * sub_0800553C @ 0x0800553C, sub_08005548 @ 0x08005548, sub_08005554 @ 0x08005554
 */

/* Family F058 (data/families.json): `push {lr}; movs r0,#K; bl DesignRoomRefreshSlotFlag;
 * pop {r1}; bx r1` -- 12 bytes, three members, and `varies` has exactly one
 * entry: the immediate. One decision covers all three.
 *
 * `pop {r1}` is the VALUE-RETURNING epilogue (docs/agbcc-codegen.md), so this
 * is not void and the `return` is real. That in turn pins DesignRoomRefreshSlotFlag's own
 * return at `int`: a narrow return would re-narrow here with `lsl; lsr` after
 * the `bl`, because agbcc re-narrows on the callee's declaration.
 */

/* Family F058 (data/families.json): `push {lr}; movs r0,#K; bl DesignRoomRefreshSlotFlag;
 * pop {r1}; bx r1` -- 12 bytes, three members, and `varies` has exactly one
 * entry: the immediate. One decision covers all three.
 *
 * `pop {r1}` is the VALUE-RETURNING epilogue (docs/agbcc-codegen.md), so this
 * is not void and the `return` is real. That in turn pins DesignRoomRefreshSlotFlag's own
 * return at `int`: a narrow return would re-narrow here with `lsl; lsr` after
 * the `bl`, because agbcc re-narrows on the callee's declaration.
 */

/* Family F058 (data/families.json): `push {lr}; movs r0,#K; bl DesignRoomRefreshSlotFlag;
 * pop {r1}; bx r1` -- 12 bytes, three members, and `varies` has exactly one
 * entry: the immediate. One decision covers all three.
 *
 * `pop {r1}` is the VALUE-RETURNING epilogue (docs/agbcc-codegen.md), so this
 * is not void and the `return` is real. That in turn pins DesignRoomRefreshSlotFlag's own
 * return at `int`: a narrow return would re-narrow here with `lsl; lsr` after
 * the `bl`, because agbcc re-narrows on the callee's declaration.
 */


/* DesignRoomRefreshSlotFlag switches on its argument over exactly 0, 1 and 2 (a three-way
 * `cmp`/`beq` ladder, twice -- once per branch of an earlier test), so these
 * three wrappers are its complete call set and the constant is the whole
 * difference between them.
 */

int sub_0800553C(void)
{
    return DesignRoomRefreshSlotFlag(0);
}

/* DesignRoomRefreshSlotFlag switches on its argument over exactly 0, 1 and 2 (a three-way
 * `cmp`/`beq` ladder, twice -- once per branch of an earlier test), so these
 * three wrappers are its complete call set and the constant is the whole
 * difference between them.
 */

int sub_08005548(void)
{
    return DesignRoomRefreshSlotFlag(1);
}

/* DesignRoomRefreshSlotFlag switches on its argument over exactly 0, 1 and 2 (a three-way
 * `cmp`/`beq` ladder, twice -- once per branch of an earlier test), so these
 * three wrappers are its complete call set and the constant is the whole
 * difference between them.
 */

int sub_08005554(void)
{
    return DesignRoomRefreshSlotFlag(2);
}
