#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080193B0.
 * sub_080193B0 @ 0x080193B0
 */

/*
 * StartEventScript -- start a script in a free gUnknown_0200C528 slot.
 *
 * Runs the three openers ClearTextSkipFlag, EnableScriptedInput and ClearCoScreenDrawHook, asks
 * FindEventScriptSlot for a free slot, and seeds it: both .unk00 and .unk04 point at
 * `script`, with no callback and a zero counter. Returns the slot, or NULL when
 * FindEventScriptSlot reports -1.
 *
 * Why the C looks odd: the slot index is held in two locals of different
 * signedness off the one call result. `r` is what the `== -1` test reads and
 * `idx` is what every subscript reads, through an explicit `(s16)`. The
 * original keeps both a sign-extended and a zero-extended copy of that result,
 * and one local can only produce one of them, whatever it is cast to.
 */
struct Unk0200C528 *StartEventScript(const u8 *script)
{
    s16 r;
    u16 idx;

    ClearTextSkipFlag();
    EnableScriptedInput();
    ClearCoScreenDrawHook();

    r = FindEventScriptSlot(NULL);
    idx = r;

    if (r == -1)
        return NULL;

    gUnknown_0200C528[(s16)idx].unk00 = (struct Unk0200C528Node *)script;
    gUnknown_0200C528[(s16)idx].unk08 = NULL;
    gUnknown_0200C528[(s16)idx].unk04 = (struct Unk0200C528Node *)script;
    gUnknown_0200C528[(s16)idx].unk0c = 0;

    return &gUnknown_0200C528[(s16)idx];
}
asm(".global sub_080193B0\n.thumb_set sub_080193B0, StartEventScript\n");
