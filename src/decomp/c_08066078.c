#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066078.
 * sub_08066078 @ 0x08066078
 */

/* `ldrb` + `lsls #0x18; asrs #0x18` on unk32 is a SIGNED byte field, not a u8
 * with a cast -- Thumb has no immediate-offset `ldrsb`. It selects into the
 * unk44[] pointer array; the `ldrsh` pair that follows uses a REGISTER offset
 * at a constant displacement, so unk28/unk2a are plain s16 fields of the
 * selected object and not a second index. */
void MatchSetupDrawTeamArrows(void)
{
    struct Unk08580934_Obj *o;

    o = gUnknown_08580934->unk44[gUnknown_08580934->unk32];
    DrawUpArrow(o->unk28 + 9, o->unk2a + 8);
    DrawDownArrow(o->unk28 + 9, o->unk2a + 0x28);
}
asm(".global sub_08066078\n.thumb_set sub_08066078, MatchSetupDrawTeamArrows\n");
