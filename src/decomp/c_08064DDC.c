#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064DDC.
 * sub_08064DDC @ 0x08064DDC, sub_08064E1C @ 0x08064E1C
 */

/* The third parameter is REASSIGNED rather than copied into a new local: the
 * ROM's `movs r2, #4` writes the same register the index arrived in and the
 * `adds r0, r2, #0; adds r0, #0x4a` afterwards reads it back, so one variable
 * serves as both the table key and the sprite-id base. */
void DrawArmySlotControllerIcon(int a, int b, int c)
{
    if (gUnknown_08580934->unk09[c] == 2)
        c = 4;

    DrawOamObject(c + 0x4A, (a + 8) & 0x1FF, b & 0xFF, 0, 0);
}
asm(".global sub_08064DDC\n.thumb_set sub_08064DDC, DrawArmySlotControllerIcon\n");

/* DrawArmySlotControllerIcon's sibling on the +0x0d table: no conditional remap, a different
 * id base, and the two coordinates biased by -0x1e and +0x10 before their
 * masks. `subs r3, #0x1e` writes the parameter's own register, which is what
 * an expression on the parameter compiles to when nothing else reads it. */
void DrawArmySlotColorIcon(int a, int b, int c)
{
    DrawOamObject(gUnknown_08580934->unk0d[c] + 0x3D, (a - 0x1E) & 0x1FF,
                 (b + 0x10) & 0xFF, 0, 0);
}
asm(".global sub_08064E1C\n.thumb_set sub_08064E1C, DrawArmySlotColorIcon\n");
