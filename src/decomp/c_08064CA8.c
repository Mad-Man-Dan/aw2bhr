#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064CA8.
 * sub_08064CA8 @ 0x08064CA8
 */

/* A velocity integrator: unk3c accelerates unk3a, unk3a moves unk2a. All three
 * are halfword read-modify-writes, so the `ldrh` on the signed unk2a is the
 * documented "the sign bits cannot survive the strh" narrowing; the SIGNED read
 * only appears where the value is compared, as `lsls #0x10; asrs #0x10`.
 *
 * `movs r0, #0x20; rsbs r0, r0, #0` is the literal -0x20, and the compare is
 * against a register because -32 is not a `cmp` immediate. */
void TeamBadgeExitUp_Loop(struct Unk08580934_Obj *obj)
{
    obj->unk3a += obj->unk3c;
    obj->unk2a += obj->unk3a;

    if (obj->unk2a < -0x20)
        ClearSlotScriptCallback(gUnknown_03001FBC);

    TeamBadge_Draw(obj);
}
asm(".global sub_08064CA8\n.thumb_set sub_08064CA8, TeamBadgeExitUp_Loop\n");
