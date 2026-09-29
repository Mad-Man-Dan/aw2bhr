#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804026C.
 * sub_0804026C @ 0x0804026C, sub_08040290 @ 0x08040290
 */

#include "proc.h"

/* Asks GetInventionTargetCell for the entry's tile position into a stack pair, then
 * starts the 0x0849FADC proc through StartBlackCannonExplosion with that position and its
 * own parent argument. GetInventionTargetCell returns bool8 and the result is dropped --
 * no narrowing follows the `bl`. The twin StartDeathRayDestroyedEffect differs only in calling
 * StartDeathRayExplosion. */
void StartBlackCannonDestroyedEffect(struct Unk02028360 *ent, ProcPtr parent)
{
    struct Unk02028360Pos pos;

    GetInventionTargetCell(ent, &pos);
    StartBlackCannonExplosion(pos.unk00, pos.unk02, parent);
}
asm(".global sub_0804026C\n.thumb_set sub_0804026C, StartBlackCannonDestroyedEffect\n");

/* The twin of StartBlackCannonDestroyedEffect, differing only in which of the two 0x0849FADC
 * starters it calls (StartDeathRayExplosion rather than StartBlackCannonExplosion). */
void StartDeathRayDestroyedEffect(struct Unk02028360 *ent, ProcPtr parent)
{
    struct Unk02028360Pos pos;

    GetInventionTargetCell(ent, &pos);
    StartDeathRayExplosion(pos.unk00, pos.unk02, parent);
}
asm(".global sub_08040290\n.thumb_set sub_08040290, StartDeathRayDestroyedEffect\n");
