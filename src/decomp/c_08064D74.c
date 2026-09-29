#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064D74.
 * sub_08064D74 @ 0x08064D74
 */

/* Twin of TeamBadgeEnter_Loop -- see that file for why gUnknown_08580934's ADDRESS is
 * bound to a local at the top rather than named at its single deep use.
 * The sum is `obj->unk38 + gUnknown_085809F0[obj->unk26]` in that order: with
 * the table first, -fforce-addr computes the element address before loading
 * unk38, which is the reverse of the ROM. */
void TeamBadgeEnterAtBase_Loop(struct Unk08580934_Obj *obj)
{
    struct Unk08580934 **stp;

    stp = &gUnknown_08580934;

    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = obj->unk38 + gUnknown_085809F0[obj->unk26];
        TeamBadge_Draw(obj);

        if (obj->unk26 != 0)
        {
            obj->unk26--;
        }
        else
        {
            (*stp)->unk2d--;
            LinkRestartKeySync();
            ClearSlotScriptCallback(gUnknown_03001FBC);
        }
    }
}
asm(".global sub_08064D74\n.thumb_set sub_08064D74, TeamBadgeEnterAtBase_Loop\n");
