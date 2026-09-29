#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080670A0.
 * sub_080670A0 @ 0x080670A0, sub_080670BC @ 0x080670BC
 */

/* Family F054 -- four wrappers, two subsystems, no exemplar before this wave.
 * The shape is "set a 0/1 mode flag, then install the slot script":
 * `gFlag = K; sub_080152EC(gScript, 2);`.
 *
 * Two things worth reading off it. The store puts the ADDRESS in r0 and the
 * value in r1, which is the reverse of families F008/F017 -- that is not a
 * source feature, it is just r0 being the register the following call needs, so
 * the allocator parks the address there and the store borrows r1. And the
 * `sub_080152EC` result is discarded (`pop {r0}`), which is what makes the
 * wrapper void even though the callee returns a slot pointer.
 *
 * The pairing is already documented on gUnknown_08580DD8 in
 * include/unknown-globals.h: this is the installer, IsMatchSetupScreenRunning is the
 * matching liveness predicate. */

void StartMatchSetupScreen(void)
{
    gUnknown_0202F200 = 0;
    sub_080152EC(gUnknown_08580DD8, 2);
}
asm(".global sub_080670A0\n.thumb_set sub_080670A0, StartMatchSetupScreen\n");

/* Family F054, the mode-1 half of the StartMatchSetupScreen pair. See the note there. */

void sub_080670BC(void)
{
    gUnknown_0202F200 = 1;
    sub_080152EC(gUnknown_08580DD8, 2);
}
