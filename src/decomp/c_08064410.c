#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064410.
 * sub_08064410 @ 0x08064410, sub_08064474 @ 0x08064474
 */

#include "hardware.h"

/* Copies the whole 20-byte key state into one of three slots of
 * gUnknown_03002040 -- slot 1 on L (0x200), slot 2 on R (0x100), slot 0 when
 * neither is held. 20 bytes is exactly sizeof(struct KeySt), which is what
 * identifies the slots; the three ldm/stm pairs are one struct assignment.
 *
 * The destination pointer is bound INSIDE each arm rather than once at the top.
 * That is the whole difference: a function-scope binding gets the ROM's
 * one-pool-word shape (`ldr r1, =gUnknown_03002040` then `adds r1, #0x14`) but
 * keeps the base live across all three arms and pays two `adds rN, r3, #0`
 * copies, where the ROM reloads the pool word in each arm. A block-scope
 * binding dies at the end of its arm, so each arm re-materialises it. Spelling
 * the destination as a cast subscript or a byte offset instead --
 * `((struct KeySt *)&gUnknown_03002040)[1]` or
 * `*(struct KeySt *)((u8 *)&gUnknown_03002040 + 0x14)` -- folds base and offset
 * into a symbol+addend relocation and emits THREE pool words with no adds
 * (both measured, wave 34). */
void CopyKeysToPlayerSlotUnused(void)
{
    if (gpKeySt->held & L_BUTTON)
    {
        struct KeySt *dst;

        dst = (struct KeySt *)&gUnknown_03002040;
        dst[1] = *gpKeySt;
    }

    if (gpKeySt->held & R_BUTTON)
    {
        struct KeySt *dst;

        dst = (struct KeySt *)&gUnknown_03002040;
        dst[2] = *gpKeySt;
    }

    if ((gpKeySt->held & (R_BUTTON | L_BUTTON)) == 0)
    {
        struct KeySt *dst;

        dst = (struct KeySt *)&gUnknown_03002040;
        dst[0] = *gpKeySt;
    }
}
asm(".global sub_08064410\n.thumb_set sub_08064410, CopyKeysToPlayerSlotUnused\n");

void DrawUpArrow(int a1, int a2)
{
    if (gUnknown_030005FC == gGameClock - 1)
    {
        a1 = (gUnknown_03000600 + a1) / 2;
        a2 = (gUnknown_03000602 + a2) / 2;
    }

    DrawOamObject(0x43, a1 & 0x1ff, a2 & 0xff, 0, 0);

    gUnknown_030005FC = gGameClock;
    gUnknown_03000600 = a1;
    gUnknown_03000602 = a2 + 1;
}
asm(".global sub_08064474\n.thumb_set sub_08064474, DrawUpArrow\n");
