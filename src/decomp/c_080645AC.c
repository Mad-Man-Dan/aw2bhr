#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080645AC.
 * sub_080645AC @ 0x080645AC
 */

/* obj->unk4c is a one-argument callback taking the object: `bl _call_via_r1`
 * with r0 still holding the incoming pointer. `Div(unk28 - 7, 0x20)` is
 * recomputed in each arm rather than bound -- agbcc emits the call three times
 * because a bound local would have to survive two further calls. */
void RuleOption_Draw(struct Unk08580934_Obj *obj)
{
    obj->unk4c(obj);

    DrawOamObject(Div(obj->unk28 - 7, 0x20) + 0xC1, obj->unk28 & 0x1FF,
                 (obj->unk2a - 8) & 0xFF, 0, 0);

    if (obj->unk46 == 0)
        DrawOamObject(Div(obj->unk28 - 7, 0x20) + 0xD1, obj->unk28 & 0x1FF,
                     obj->unk2a & 0xFF, 0, 3);
    else
        DrawOamObject(Div(obj->unk28 - 7, 0x20) + 0xD1, (obj->unk28 - 0x10) & 0x1FF,
                     ((obj->unk2a - 0x10) & 0xFF) | 0x300, 0, 3);
}
asm(".global sub_080645AC\n.thumb_set sub_080645AC, RuleOption_Draw\n");
