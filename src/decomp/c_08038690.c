#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08038690.
 * sub_08038690 @ 0x08038690
 */

/* Family F013, the two-argument form of the fade wrappers -- see StartMidLockingFadeToBlack
 * in src/decomp/c_08071F88.c. `adds r1, r0, #0` ahead of `movs r0, #0x60` puts
 * the incoming parameter in the SECOND argument slot and the literal in the
 * first; the pool-free constant and the untouched-then-copied r0 are the whole
 * shape. `pop {r0}` makes it void, and so is SetCampaignCompletionFlag.
 *
 * The argument is `int`: it survives a `bl` with a bare `adds r1, r0, #0` and
 * no PROMOTE_MODE narrowing, which a u8 or u16 parameter would have carried. */

void SetHardCampaignFlag(int a)
{
    SetCampaignCompletionFlag(0x60, a);
}
asm(".global sub_08038690\n.thumb_set sub_08038690, SetHardCampaignFlag\n");
