#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AAC0.
 * sub_0803AAC0 @ 0x0803AAC0
 */

/* The fourth parameter is real but unread here -- sub_0803AB3C passes an s8 in
 * r3 (0, 0xd or -1) and this body never touches it. An unreferenced trailing
 * parameter costs no instructions, so adding it left this file byte-identical.
 * Wave 43, W43-I. */
void UnitInfoPanel_DrawIconPair(u8 a1, u8 a2, u8 a3, s8 a4)
{
    int x;
    int t;

    x = (a2 >> 1) * 2;
    t = a3 + 0xac;
    DrawOamObject(x + t,
                 gUnknown_0849D89C->unk00 + gUnknown_0849E2F8[a1 * 2] + 1,
                 gUnknown_0849E2F8[a1 * 2 + 1], 0, 0);
    DrawOamObject(a2 + 0xb2,
                 gUnknown_0849D89C->unk00 + gUnknown_0849E2F8[a1 * 2] + 1,
                 gUnknown_0849E2F8[a1 * 2 + 1], 0, 0);
}
asm(".global sub_0803AAC0\n.thumb_set sub_0803AAC0, UnitInfoPanel_DrawIconPair\n");
