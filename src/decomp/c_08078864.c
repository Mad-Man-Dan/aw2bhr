#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078864.
 * SetupCoSelectHotPursuit @ 0x08078864, SetupCoSelectFinalFront @ 0x08078884, SetupCoSelectLiberation2 @ 0x08078890
 */

/* Six `bl`s with a single `movs r0, #0` between the first and the second, and
 * nothing at all between the middle four. That gap is the whole reading: the
 * four gUnknown_030058E0 builders each take an index in r0 and return the
 * advanced index in r0, so consecutive calls with no argument setup are a
 * NEST, not four statements. Four statements would each have had to reload r0.
 *
 * The absent code is also what fixes the width: a `u8`/`u16` parameter or
 * return anywhere in the chain would have put `lsl #24; lsr #24` between two
 * `bl`s, and there is none, so every link is int-wide.
 *
 * SetupCoSelectLiberation, 0xd4 bytes above, is the short version of the same source --
 * `ClearArmyCount(); SetCoSelectGroupSwitchNone(AddCoSelectGroupOrangeStar(0));`.
 */
void SetupCoSelectHotPursuit(void)
{
    ClearArmyCount();
    AddCoSelectGroupYellowComet(AddCoSelectGroupGreenEarth(AddCoSelectGroupBlueMoon(AddCoSelectGroupOrangeStar(0))));
    SetCoSelectGroupSwitchAll();
}

asm(".global sub_08078864\n.thumb_set sub_08078864, SetupCoSelectHotPursuit\n");

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * The callee reads no argument register before writing it, so there
 * is no parameter to pass through either.
 */
void SetupCoSelectFinalFront(void)
{
    SetupCoSelectHotPursuit();
}

asm(".global sub_08078884\n.thumb_set sub_08078884, SetupCoSelectFinalFront\n");

/* Family F001 forwarder, 12 bytes:
 *     push {lr}
 *     bl   <callee>
 *     pop  {r0}
 *     bx   r0
 * The `pop {r0}` fixes THIS function as void: it overwrites whatever
 * the callee returned, so nothing about the callee is visible from
 * here. Everything below was read off the callee's own body instead.
 *
 * The callee reads no argument register before writing it, so there
 * is no parameter to pass through either.
 */
void SetupCoSelectLiberation2(void)
{
    SetupCoSelectLiberation();
}
asm(".global sub_08078890\n.thumb_set sub_08078890, SetupCoSelectLiberation2\n");
