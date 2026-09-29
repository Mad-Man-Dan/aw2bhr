#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F34C.
 * DrawOamObject @ 0x0801F34C
 */

/* Finds the gUnknown_0200F920 entry for tile id a1 and draws it as a sprite.
 * The OBJ blob is picked out of gUnknown_0848BAE4 by the entry's tile
 * dimensions; the fifth PutSpriteExt word is the base tile id offset by the
 * entry's own tile and by the slot's palette bank, stored biased by 0x10
 * because OBJ palettes start at bank 16.
 *
 * THE FIFTH ARGUMENT IS ACCUMULATED INTO THE `a4` PARAMETER, NOT INTO A FRESH
 * LOCAL. agbcc evaluates this call's arguments LEFT TO RIGHT, so spelling the
 * OAM word inline as the fifth argument computes it AFTER the gUnknown_0848BAE4
 * lookup, where the ROM computes it before -- it has to be its own statement.
 * Given that, `int word = a4 + ...` and `a4 = a4 + ...` differ: the fresh local
 * is a new pseudo, which pushes a1 into r5 and a4 into r6 and costs a second hi
 * register (`push {r6,r7}` / `mov r9,r2`, +4 bytes). Assigning back over the
 * dead parameter keeps a1 in r6, a2 in r7, a3 in r8 and the OAM word in r5,
 * which is the ROM's allocation exactly. */
void DrawOamObject(int a1, int a2, int a3, int a4, int a5)
{
    struct Unk0200F920 *e;
    int i;

    e = &gUnknown_0200F920[GetTilePoolForGraphic(a1)];
    for (i = 0; i < e->unk05; i++) {
        if (e->unk08[i].unk02 == a1) {
            a4 = a4 + e->unk08[i].unk00 + ((e->unk04 - 0x10) << 12);
            PutSpriteExt(a5, a2, a3,
                         gUnknown_0848BAE4[gUnknown_0848B780[a1].unk01 * 17
                                           + gUnknown_0848B780[a1].unk00],
                         a4);
            return;
        }
    }
}

asm(".global sub_0801F34C\n.thumb_set sub_0801F34C, DrawOamObject\n");
