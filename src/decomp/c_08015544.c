#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015544.
 * sub_08015544 @ 0x08015544, sub_08015550 @ 0x08015550, sub_0801555C @ 0x0801555C
 */

/* A bare forwarder: `push {lr}; bl <callee>; pop {r0}; bx r0`. The `pop {r0}`
 * fixes it as void, and the callee reads no argument register, so there is no
 * parameter to pass through either.
 */
void ClearAllSpriteScripts2(void)
{
    ClearAllSpriteScripts();
}
asm(".global sub_08015544\n.thumb_set sub_08015544, ClearAllSpriteScripts2\n");

/* A bare forwarder: `push {lr}; bl <callee>; pop {r0}; bx r0`. The `pop {r0}`
 * fixes it as void, and the callee reads no argument register, so there is no
 * parameter to pass through either.
 */
void DrawSimpleSpriteScripts2(void)
{
    DrawSimpleSpriteScripts();
}
asm(".global sub_08015550\n.thumb_set sub_08015550, DrawSimpleSpriteScripts2\n");

/* A bare forwarder: `push {lr}; bl <callee>; pop {r0}; bx r0`. The `pop {r0}`
 * fixes it as void, and the callee reads no argument register, so there is no
 * parameter to pass through either.
 */
void TickSimpleSpriteScripts2(void)
{
    TickSimpleSpriteScripts();
}
asm(".global sub_0801555C\n.thumb_set sub_0801555C, TickSimpleSpriteScripts2\n");
