#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08065EF4.
 * sub_08065EF4 @ 0x08065EF4
 */

/* `i` is a declared local but `t.unk00[i]` is NOT -- spelling the element as a
 * local computes its address before obj->unk28 is loaded, while the ROM loads
 * unk28 first and only then scales the index. The four uses CSE to the one
 * address the ROM keeps in r4.
 *
 * gUnknown_0816E120 is copied WHOLE (a struct assignment, three ldm/stm pairs).
 * See its note in unknown-globals.h for why it is named as a global rather than
 * written as an auto aggregate's initialiser -- both give these exact 116
 * bytes, only this one relocates against the address the ROM does. */
void MatchSetupDrawSelectionArrows(void)
{
    struct Unk0816E120 t = gUnknown_0816E120;
    struct Unk08580934_Obj *obj = gUnknown_08580934->unk34[gUnknown_08580934->unk32 / 2];
    int i = gUnknown_08580934->unk32 & 1;

    DrawUpArrow(obj->unk28 + t.unk00[i].unk08, t.unk00[i].unk0a + 0x34);
    DrawDownArrow(obj->unk28 + t.unk00[i].unk0c, t.unk00[i].unk0e + 0x34);
}
asm(".global sub_08065EF4\n.thumb_set sub_08065EF4, MatchSetupDrawSelectionArrows\n");
