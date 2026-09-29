#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044924.
 * sub_08044924 @ 0x08044924, sub_08044940 @ 0x08044940
 */

/* The `(void *)` casts are the price of AddVBlankHook/RemoveVBlankHook taking the
 * list entry as `void *` -- C89 needs one to hand it a function. */
void UnregisterMapAnimationCallbacks(void)
{
    RemoveVBlankHook((void *)sub_080246B4);
    RemoveVBlankHook((void *)AnimatePowerActiveCoPalettes);
}
asm(".global sub_08044924\n.thumb_set sub_08044924, UnregisterMapAnimationCallbacks\n");

void RegisterMapAnimationCallbacks(void)
{
    AddVBlankHook((void *)sub_080246B4);
    AddVBlankHook((void *)AnimatePowerActiveCoPalettes);
}
asm(".global sub_08044940\n.thumb_set sub_08044940, RegisterMapAnimationCallbacks\n");
