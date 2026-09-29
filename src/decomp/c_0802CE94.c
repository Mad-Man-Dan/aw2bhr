#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802CE94.
 * sub_0802CE94 @ 0x0802CE94
 */

/* Family F045: `push {lr}; lsls r1,#0x18; lsrs r1,#0x18; ldr r0,=g;
 * str r1,[r0]; bl CloseTopMenu; bl <second>; pop {r0}; bx r0`.
 *
 * The `lsls`+`lsrs` pair is a VALUE-KEPT narrowing (not a truth test) sitting
 * before any use, i.e. PROMOTE_MODE re-narrowing a declared `u8` parameter --
 * and it is on r1, the SECOND argument register. So the signature has two
 * parameters and the first one is dead in the body. The store is `str`, a
 * word, so the global is wider than the parameter feeding it; see the
 * gUnknown_030040F0/gUnknown_030044A0 comment in unknown-globals.h.
 *
 * THE FIRST PARAMETER IS NOT PROVED. A dead leading parameter is invisible,
 * and `void *` is chosen only because these functions are callback slots
 * (+0x14 and +0x18) of the 0x20-byte records at gUnknown_0849AC60 /
 * gUnknown_0849ABC0. The sibling slots do not settle it either: OptionsMenu_DeleteUsability
 * (+0x04) and OptionsMenu_YieldUsability both ignore r0 as well.
 *
 * The two `bl`s are two statements: the second callee never reads r0 before
 * writing it, so a nest is not expressible in C. */
void OptionsMenu_Yield(void *a, u8 b)
{
    gUnknown_030044A0 = b;
    CloseTopMenu();
    StartYieldConfirmScript();
}
asm(".global sub_0802CE94\n.thumb_set sub_0802CE94, OptionsMenu_Yield\n");
