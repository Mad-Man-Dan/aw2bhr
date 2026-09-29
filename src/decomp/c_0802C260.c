#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C260.
 * sub_0802C260 @ 0x0802C260, sub_0802C270 @ 0x0802C270, sub_0802C280 @ 0x0802C280, sub_0802C290 @ 0x0802C290
 */

/* Registers a function with the 16-slot gUnknown_03002FA0 list. QueueVBlankCallback
 * takes its entry as `void *` (that is how src/decomp/c_08011AAC.c defines it),
 * so a function address has to be cast -- exactly the note carried on the
 * sibling pair AddVBlankHook/RemoveVBlankHook. The pool word is the ADDRESS of
 * DisableCoScreenHBlankAndSoundVSync, not a call to it.
 */

void sub_0802C260(void)
{
    QueueVBlankCallback((void *)DisableCoScreenHBlankAndSoundVSync);
}

/* Registers a function with the 16-slot gUnknown_03002FA0 list. QueueVBlankCallback
 * takes its entry as `void *` (that is how src/decomp/c_08011AAC.c defines it),
 * so a function address has to be cast -- exactly the note carried on the
 * sibling pair AddVBlankHook/RemoveVBlankHook. The pool word is the ADDRESS of
 * EnableCoScreenHBlankAndSoundVSync, not a call to it.
 */

void sub_0802C270(void)
{
    QueueVBlankCallback((void *)EnableCoScreenHBlankAndSoundVSync);
}

/* Installs one gUnknown_0200C528 list script. StartEventScript returns the slot it
 * allocated, and `pop {r0}; bx r0` here discards it -- so this is void and the
 * call is a bare statement. The script is ROM data reached only as an address,
 * hence `const u8 []` and a clean pool word.
 */

void StartSaveConfirmScript(void)
{
    StartEventScript(gUnknown_0849A8F0);
}
asm(".global sub_0802C280\n.thumb_set sub_0802C280, StartSaveConfirmScript\n");

/* The removal half of the pair: StartSaveConfirmScript installs gUnknown_0849A8F0 through
 * StartEventScript and this drops it through EndEventScript. EndEventScript returns -1
 * unconditionally and `pop {r0}; bx r0` discards it, so this is void.
 */

void EndSaveConfirmScript(void)
{
    EndEventScript(gUnknown_0849A8F0);
}
asm(".global sub_0802C290\n.thumb_set sub_0802C290, EndSaveConfirmScript\n");
