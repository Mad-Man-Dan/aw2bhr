#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064C34.
 * sub_08064C34 @ 0x08064C34
 */

/* The ROM hoists `ldr r5, =gUnknown_08580934` ahead of the first test and keeps
 * the ADDRESS in r5 for the whole body, even though its only use is two nested
 * else-branches deep. The pool ORDER says so independently: gUnknown_08580934's
 * word is emitted before gUnknown_085809D8's, the reverse of the order the code
 * uses them, so its address constant was created FIRST in RTL. Binding that
 * address to a local at the top of the function is what creates it there. */
void TeamBadgeEnter_Loop(struct Unk08580934_Obj *obj)
{
    struct Unk08580934 **stp;

    stp = &gUnknown_08580934;

    if (obj->unk24 != 0)
    {
        obj->unk24--;
    }
    else
    {
        obj->unk2a = gUnknown_085809D8[obj->unk26];
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
asm(".global sub_08064C34\n.thumb_set sub_08064C34, TeamBadgeEnter_Loop\n");
