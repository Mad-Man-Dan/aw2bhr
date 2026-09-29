#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B7D8.
 * sub_0803B7D8 @ 0x0803B7D8, sub_0803B804 @ 0x0803B804
 */

/* "Suspend the BGM": stash the currently-requested song id in gUnknown_030005C8
 * so ReplayPendingMusic / ReplayPendingMusicIfEnabled can restart it later, blank the requested slot
 * to the 0xFFFF sentinel, then fade the mixer out and pause it over 2 frames
 * (m4aMPlayFadeOutPause, m4aMPlayFadeOutPause). Its resume counterpart is MusicResumeFade_Start
 * next door, which copies the pair back the other way and calls
 * m4aMPlayFadeInContinue (m4aMPlayFadeInContinue) instead.
 * The `ldrh`/`strh` pair between the two globals is a plain copy between two
 * DIFFERENT addresses, not the self-store that would be a volatile tell.
 * `pop {r0}; bx r0` -> void.
 *
 * THE CAST IS LOAD-BEARING AND IT MARKS AN UNRESOLVED TYPE TENSION. Two
 * internally consistent models meet at this call. gUnknown_03005AE0 is
 * declared `u8 []` in unknown-globals.h -- deliberately, as the weakest model
 * that reproduces the bare `ldr rN, =gUnknown_03005AE0` with no extra
 * instruction, because no member of it is reachable from C yet. But
 * m4aMPlayFadeOutPause and m4aMPlayFadeInContinue were promoted in an earlier wave with the m4a
 * family's real `struct MusicPlayerInfo *`, and a promoted definition wins over
 * a weakest-model guess. The cast is byte-neutral (a pointer cast emits
 * nothing) and satisfies both.
 *
 * The clean end state is to retype gUnknown_03005AE0 to `struct
 * MusicPlayerInfo` and pass `&gUnknown_03005AE0` at all four C call sites --
 * here, c_0803B35C.c:50 (MPlayVolumeControl) and c_0803B578.c:106 (m4aMPlayFadeOut) --
 * dropping the casts and the `void *` declarations together. That is a
 * self-contained change worth one wave's attention; it was not done inline here
 * because it touches a shared global plus three declarations and this wave had
 * already spent a build cycle on the same collision. */

void MusicPauseFade_Start(void)
{
    gUnknown_030005C8 = gUnknown_030005CA;
    gUnknown_030005CA = 0xFFFF;

    m4aMPlayFadeOutPause((struct MusicPlayerInfo *)gUnknown_03005AE0, 2);
}
asm(".global sub_0803B7D8\n.thumb_set sub_0803B7D8, MusicPauseFade_Start\n");

/* The resume half of MusicPauseFade_Start: copy the stashed song id back into the
 * requested slot and fade the mixer in over 2 frames (m4aMPlayFadeInContinue,
 * m4aMPlayFadeInContinue). Unlike its counterpart it does NOT blank the slot it
 * read from, which is the only structural difference between the two.
 * `pop {r0}; bx r0` -> void. */

void MusicResumeFade_Start(void)
{
    gUnknown_030005CA = gUnknown_030005C8;

    m4aMPlayFadeInContinue((struct MusicPlayerInfo *)gUnknown_03005AE0, 2);
}
asm(".global sub_0803B804\n.thumb_set sub_0803B804, MusicResumeFade_Start\n");
