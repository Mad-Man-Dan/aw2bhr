#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080705D8.
 * sub_080705D8 @ 0x080705D8
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
 * m4aMPlayContinue. One pointer passed through to MPlayContinue
 * (MPlayContinue, already promoted in src/decomp/c_080703B8.c); the
 * neighbouring m4aMPlayAllContinue is m4aMPlayAllContinue, which loops the same
 * callee over gUnknown_08242308. Ordinary C inside the m4a span, like every
 * other m4a*() entry point.
 */
void m4aMPlayContinue(struct MusicPlayerInfo * mplayInfo)
{
    MPlayContinue(mplayInfo);
}
asm(".global sub_080705D8\n.thumb_set sub_080705D8, m4aMPlayContinue\n");
