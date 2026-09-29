#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B3C8.
 * sub_0803B3C8 @ 0x0803B3C8, sub_0803B3D4 @ 0x0803B3D4, sub_0803B3E0 @ 0x0803B3E0, sub_0803B3EC @ 0x0803B3EC, sub_0803B3F8 @ 0x0803B3F8, sub_0803B404 @ 0x0803B404, sub_0803B408 @ 0x0803B408
 */

/* `SetSoundMixerChannelCount(8)`: the incoming r0 is overwritten by `movs r0, #8` before the
 * call, so this takes no argument. `pop {r0}; bx r0` -> void. */

void SetSoundMixerChannelCount8(void)
{
    SetSoundMixerChannelCount(8);
}
asm(".global sub_0803B3C8\n.thumb_set sub_0803B3C8, SetSoundMixerChannelCount8\n");

/* m4aSoundMode(n << 8) -- the SOUND_MODE_MAXCHN field, so "use n mixer
 * channels". `int` and not a narrow type: the argument reaches the `bl` through
 * a bare `lsls r0, r0, #8` with no PROMOTE_MODE narrowing, which a u8/u16
 * parameter surviving a call would have carried. `pop {r0}; bx r0` -> void. */

void SetSoundMixerChannelCount(int n)
{
    SoundMode_rev01(n << 8);
}
asm(".global sub_0803B3D4\n.thumb_set sub_0803B3D4, SetSoundMixerChannelCount\n");

/* Forwarder to m4aSoundVSyncOn. r0 is never touched and the callee takes no
 * argument, so void(void); `pop {r0}; bx r0` settles the return. */

void EnableSoundVSync(void)
{
    SoundVSyncOn_rev01();
}
asm(".global sub_0803B3E0\n.thumb_set sub_0803B3E0, EnableSoundVSync\n");

/* Forwarder to m4aSoundVSyncOff. void(void) -- see EnableSoundVSync. */

void DisableSoundVSync(void)
{
    SoundVSyncOff_rev01();
}
asm(".global sub_0803B3EC\n.thumb_set sub_0803B3EC, DisableSoundVSync\n");

/* Forwarder to m4aSoundVSync. void(void) -- see EnableSoundVSync. */

void RunSoundVSync(void)
{
    sub_0806FD98();
}
asm(".global sub_0803B3F8\n.thumb_set sub_0803B3F8, RunSoundVSync\n");

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` in front of RunSoundMain. It sits inside the 0x0803B3C8 m4a
 * forwarder run, so it is almost certainly the wrapper for a sound entry point
 * this build compiled away rather than dead code. Nothing about the signature
 * is recoverable -- a leaf ending in a bare `bx lr` that touches no register
 * has no return type and no argument count; void(void) is the weakest model. */

void SoundMainLoopNoOp(void)
{
}
asm(".global sub_0803B404\n.thumb_set sub_0803B404, SoundMainLoopNoOp\n");

/* Forwarder to m4aSoundMain, which is itself a forwarder to sub_0806F744
 * (m4aSoundMain). void(void) -- see EnableSoundVSync. */

void RunSoundMain(void)
{
    m4aSoundMain();
}
asm(".global sub_0803B408\n.thumb_set sub_0803B408, RunSoundMain\n");
