#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807046C.
 * sub_0807046C @ 0x0807046C
 */

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * m4aSoundMain. The callee is SoundMain (data/fe_matches.json, `full`),
 * which lives in the m4a_asm.s half of the driver and will never match --
 * this forwarder is the ordinary C beside it, and its own caller
 * RunSoundMain is already promoted in src/decomp/c_0803B3C8.c.
 */
void m4aSoundMain(void)
{
    sub_0806F744();
}
asm(".global sub_0807046C\n.thumb_set sub_0807046C, m4aSoundMain\n");
