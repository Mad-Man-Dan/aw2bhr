#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08044C44.
 * CoPowerOlafWinterFury @ 0x08044C44, CoPowerDrakeTsunami @ 0x08044C80, CoPowerDrakeTyphoon @ 0x08044CBC, CoPowerHawkeBlackWave @ 0x08044CF8, CoPowerHawkeBlackStorm @ 0x08044D34
 */

#include "proc.h"

/* One of five wrappers over the nine-argument StartCoPowerDamageHealScript (four arguments in
 * registers, five on the stack). They differ only in the blob, the palette and
 * four small integers; the wrapper's own parameter is the LAST argument and is
 * StartCoPowerDamageHealScript's `parent` -- see the prototype's comment in
 * include/unknown-functions.h for how the nine were read off the callee.
 *
 * The `str` ordering into the stack slots and the sharing of the constant
 * registers are agbcc's: writing the call as a plain nine-argument call
 * reproduces the ROM's `movs`/`subs` chain exactly, including CoPowerDrakeTsunami's
 * `movs r4,#0; subs r4,#1` for -1 and CoPowerHawkeBlackWave's `subs r4,#2` off a live 1.
 * `pop {r4, r5, r6}; pop {r0}` -- void.
 */
void CoPowerOlafWinterFury(ProcPtr parent)
{
    StartCoPowerDamageHealScript(gUnknown_084A0994, gUnknown_08112F00, gUnknown_08113BC0,
                 gUnknown_030033EC, 2, 0, 1, 0, parent);
}
asm(".global sub_08044C44\n.thumb_set sub_08044C44, CoPowerOlafWinterFury\n");

/* One of five wrappers over the nine-argument StartCoPowerDamageHealScript (four arguments in
 * registers, five on the stack). They differ only in the blob, the palette and
 * four small integers; the wrapper's own parameter is the LAST argument and is
 * StartCoPowerDamageHealScript's `parent` -- see the prototype's comment in
 * include/unknown-functions.h for how the nine were read off the callee.
 *
 * The `str` ordering into the stack slots and the sharing of the constant
 * registers are agbcc's: writing the call as a plain nine-argument call
 * reproduces the ROM's `movs`/`subs` chain exactly, including CoPowerDrakeTsunami's
 * `movs r4,#0; subs r4,#1` for -1 and CoPowerHawkeBlackWave's `subs r4,#2` off a live 1.
 * `pop {r4, r5, r6}; pop {r0}` -- void.
 */
void CoPowerDrakeTsunami(ProcPtr parent)
{
    StartCoPowerDamageHealScript(gUnknown_084A0994, gUnknown_0811315C, gUnknown_08113BA0,
                 gUnknown_030033EC, 1, 0, -1, 1, parent);
}
asm(".global sub_08044C80\n.thumb_set sub_08044C80, CoPowerDrakeTsunami\n");

/* One of five wrappers over the nine-argument StartCoPowerDamageHealScript (four arguments in
 * registers, five on the stack). They differ only in the blob, the palette and
 * four small integers; the wrapper's own parameter is the LAST argument and is
 * StartCoPowerDamageHealScript's `parent` -- see the prototype's comment in
 * include/unknown-functions.h for how the nine were read off the callee.
 *
 * The `str` ordering into the stack slots and the sharing of the constant
 * registers are agbcc's: writing the call as a plain nine-argument call
 * reproduces the ROM's `movs`/`subs` chain exactly, including CoPowerDrakeTsunami's
 * `movs r4,#0; subs r4,#1` for -1 and CoPowerHawkeBlackWave's `subs r4,#2` off a live 1.
 * `pop {r4, r5, r6}; pop {r0}` -- void.
 */
void CoPowerDrakeTyphoon(ProcPtr parent)
{
    StartCoPowerDamageHealScript(gUnknown_084A0994, gUnknown_0811315C, gUnknown_08113BC0,
                 gUnknown_030033EC, 2, 0, 2, 1, parent);
}
asm(".global sub_08044CBC\n.thumb_set sub_08044CBC, CoPowerDrakeTyphoon\n");

/* One of five wrappers over the nine-argument StartCoPowerDamageHealScript (four arguments in
 * registers, five on the stack). They differ only in the blob, the palette and
 * four small integers; the wrapper's own parameter is the LAST argument and is
 * StartCoPowerDamageHealScript's `parent` -- see the prototype's comment in
 * include/unknown-functions.h for how the nine were read off the callee.
 *
 * The `str` ordering into the stack slots and the sharing of the constant
 * registers are agbcc's: writing the call as a plain nine-argument call
 * reproduces the ROM's `movs`/`subs` chain exactly, including CoPowerDrakeTsunami's
 * `movs r4,#0; subs r4,#1` for -1 and CoPowerHawkeBlackWave's `subs r4,#2` off a live 1.
 * `pop {r4, r5, r6}; pop {r0}` -- void.
 */
void CoPowerHawkeBlackWave(ProcPtr parent)
{
    StartCoPowerDamageHealScript(gUnknown_084A0994, gUnknown_081133D0, gUnknown_08113BA0,
                 gUnknown_030033EC, 1, 1, -1, 0, parent);
}
asm(".global sub_08044CF8\n.thumb_set sub_08044CF8, CoPowerHawkeBlackWave\n");

/* One of five wrappers over the nine-argument StartCoPowerDamageHealScript (four arguments in
 * registers, five on the stack). They differ only in the blob, the palette and
 * four small integers; the wrapper's own parameter is the LAST argument and is
 * StartCoPowerDamageHealScript's `parent` -- see the prototype's comment in
 * include/unknown-functions.h for how the nine were read off the callee.
 *
 * The `str` ordering into the stack slots and the sharing of the constant
 * registers are agbcc's: writing the call as a plain nine-argument call
 * reproduces the ROM's `movs`/`subs` chain exactly, including CoPowerDrakeTsunami's
 * `movs r4,#0; subs r4,#1` for -1 and CoPowerHawkeBlackWave's `subs r4,#2` off a live 1.
 * `pop {r4, r5, r6}; pop {r0}` -- void.
 */
void CoPowerHawkeBlackStorm(ProcPtr parent)
{
    StartCoPowerDamageHealScript(gUnknown_084A0994, gUnknown_081133D0, gUnknown_08113BC0,
                 gUnknown_030033EC, 2, 2, -1, 0, parent);
}
asm(".global sub_08044D34\n.thumb_set sub_08044D34, CoPowerHawkeBlackStorm\n");
