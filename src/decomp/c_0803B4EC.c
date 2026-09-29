#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B4EC.
 * sub_0803B4EC @ 0x0803B4EC, sub_0803B524 @ 0x0803B524
 */

#include "proc.h"

/* "Play song id, unless sound is suppressed": when gPlaySt.unk0c is
 * set the id goes to PlayMusic and actually starts; otherwise it is only
 * PARKED in gUnknown_030005C8 with the requested slot gUnknown_030005CA blanked
 * to 0xFFFF, which is precisely the state ReplayPendingMusic next door later drains
 * by replaying gUnknown_030005C8 through PlayMusic.
 *
 * The `u16` is a LOCAL and the parameter is `int`. Both readings -- `int a`
 * with `u16 v = a`, and a bare `u16` parameter -- are byte-identical here and
 * were probed as such, so the body cannot settle it; the CALLERS can, and they
 * say `int`: all four (ReplayPendingMusicIfEnabled, PlayArmyCoMusic, PlayCoPowerMusic, PlaySuperCoPowerMusic)
 * are still assembly, but the (s16) cast below is only reachable with a
 * non-`u16` parameter -- with `u16` the cast would fold away and the ROM's
 * `lsls #0x10; asrs #0x10` in front of the `bl` would be a zero-extending
 * `lsrs` instead. The cast is needed because PlayMusic is declared `int`
 * (include/unknown-functions.h), so nothing narrows the argument for us.
 * `pop {r0}; bx r0` -> void. */

void PlayMusicIfEnabled(int a)
{
    u16 v = a;

    if (gPlaySt.bgmOn != 0)
    {
        PlayMusic((s16)v);
    }
    else
    {
        gUnknown_030005C8 = v;
        gUnknown_030005CA = 0xFFFF;
    }
}
asm(".global sub_0803B4EC\n.thumb_set sub_0803B4EC, PlayMusicIfEnabled\n");

/* "Start song id, unless it is already the one playing": compare against the
 * requested slot, and on a change tear down the ProcScr_FadeSound proc, hand
 * the id to ActivateMusicOrSoundId (m4aSongNumStart) and record it in BOTH slots. The
 * 0xFFFF the neighbours write is just a value that never equals a real id.
 *
 * THE DEFINITION IS OLD-STYLE ON PURPOSE AND THE `s16` IS NOT A PROTOTYPE
 * CHANGE. include/unknown-functions.h still declares `void PlayMusic(int);`
 * and every caller still sees `int`; C89 6.7.1 makes an old-style definition
 * compatible with a prototype when the declared parameter type promotes to the
 * prototype's, which `s16` -> `int` does, so this compiles clean under -Werror
 * and the two spellings are ONE declaration, not a disagreement.
 *
 * It is also the only spelling found that reproduces the bytes, and the
 * conflict it resolves is worth recording because both halves are hard:
 *  - CALLEE side: the ROM materialises `(u16)a` into r4 at entry
 *    (`lsls #0x10; lsrs #0x10`) and then derives BOTH later forms from r4 --
 *    `lsls r5,r4,#0x10; asrs r0,r5,#0x10` for the compare and `lsrs r0,r5,#0x10`
 *    for ActivateMusicOrSoundId's `u16` argument, spending a third callee-saved register
 *    on r5. That two-chain shape is PROMOTE_MODE on a NARROW parameter: the
 *    promoted pseudo is the parameter's home, so combine cannot fold the later
 *    casts back onto the raw incoming r0. Probed: `int a` with `u16 v = a`,
 *    `int a` with a bare `(s16)a`, `u16 a`, and an `int a; a = (u16)a;`
 *    reassignment ALL fold it to a single chain and come out 1-2 instructions
 *    short. Only a narrow parameter produces it, and only a SIGNED one -- `u16`
 *    gives the folded form too.
 *  - CALLER side: `s16` in the PROTOTYPE is ruled out, and by an already-matched
 *    function rather than by argument. src/decomp/c_08016104.c passes a u16 out
 *    of a script stream and the ROM loads it `ldrh r0, [r0, #4]`; probed, an
 *    `s16` parameter turns that into `movs r1,#4; ldrsh r0,[r0,r1]` and breaks
 *    a match that already holds. Nothing an `s16` parameter can be handed
 *    produces `ldrh`.
 * The old-style definition is the only shape that gives the callee its narrow
 * signed parameter while leaving every caller converting to `int`. Where the
 * assembly and a prototype disagree the prototype is normally the suspect --
 * here they disagree in opposite directions and BOTH readings were right about
 * their own side. */

void PlayMusic(a)
    s16 a;
{
    if (gUnknown_030005CA != a)
    {
        Proc_EndEach(ProcScr_FadeSound);
        ActivateMusicOrSoundId(a);

        gUnknown_030005C8 = a;
        gUnknown_030005CA = a;
    }
}
asm(".global sub_0803B524\n.thumb_set sub_0803B524, PlayMusic\n");
