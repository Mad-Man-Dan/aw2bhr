#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08077EDC.
 * sub_08077EDC @ 0x08077EDC
 */

#include "proc.h"

/* Wave 35 (W35-B). Runs the current gUnknown_08615194 entry's payload, either
 * unconditionally or behind the entry's own predicate. Both words are loaded
 * BEFORE the null test on the first, which is what binds each to its own local.
 * unk2c is a `u8 (*)(void)`: the ROM calls it through `bl _call_via_r1` with no
 * argument register set up -- r1 is simply where the pointer already sat, so the
 * register index is NOT the arity readout here -- and truth-tests the result
 * after `lsls #0x18`, which is what makes the return `u8`.
 *
 * The predicate arm is the FALLTHROUGH and the null arm is the branch target,
 * so the `if` has to be spelled `f != NULL` with the plain call in the `else`.
 * Spelled the other way round the two arms swap and the second StartBlockingEventScript
 * lands on the wrong side of the literal pool, which is -4 bytes of alignment. */
void WorldMap_StartPreBattleDialogue(ProcPtr proc)
{
    void *p = gUnknown_08615194[gUnknown_0202FDFC.unk0c].preBattleDialogue;
    u8 (*f)(void) = gUnknown_08615194[gUnknown_0202FDFC.unk0c].unk2c;

    if (p != NULL)
    {
        if (f != NULL)
        {
            if (f())
                StartBlockingEventScript(p, proc);
        }
        else
        {
            StartBlockingEventScript(p, proc);
        }
    }
}
asm(".global sub_08077EDC\n.thumb_set sub_08077EDC, WorldMap_StartPreBattleDialogue\n");
