#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B3C8.
 * sub_0803B3C8 @ 0x0803B3C8, sub_0803B3D4 @ 0x0803B3D4, sub_0803B3E0 @ 0x0803B3E0, sub_0803B3EC @ 0x0803B3EC, sub_0803B3F8 @ 0x0803B3F8, sub_0803B404 @ 0x0803B404, sub_0803B408 @ 0x0803B408
 */

/* `sub_0803B3D4(8)`: the incoming r0 is overwritten by `movs r0, #8` before the
 * call, so this takes no argument. `pop {r0}; bx r0` -> void. */

void sub_0803B3C8(void)
{
    sub_0803B3D4(8);
}

/* m4aSoundMode(n << 8) -- the SOUND_MODE_MAXCHN field, so "use n mixer
 * channels". `int` and not a narrow type: the argument reaches the `bl` through
 * a bare `lsls r0, r0, #8` with no PROMOTE_MODE narrowing, which a u8/u16
 * parameter surviving a call would have carried. `pop {r0}; bx r0` -> void. */

void sub_0803B3D4(int n)
{
    SoundMode_rev01(n << 8);
}

/* Forwarder to m4aSoundVSyncOn. r0 is never touched and the callee takes no
 * argument, so void(void); `pop {r0}; bx r0` settles the return. */

void sub_0803B3E0(void)
{
    SoundVSyncOn_rev01();
}

/* Forwarder to m4aSoundVSyncOff. void(void) -- see sub_0803B3E0. */

void sub_0803B3EC(void)
{
    SoundVSyncOff_rev01();
}

/* Forwarder to m4aSoundVSync. void(void) -- see sub_0803B3E0. */

void sub_0803B3F8(void)
{
    sub_0806FD98();
}

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` in front of sub_0803B408. It sits inside the 0x0803B3C8 m4a
 * forwarder run, so it is almost certainly the wrapper for a sound entry point
 * this build compiled away rather than dead code. Nothing about the signature
 * is recoverable -- a leaf ending in a bare `bx lr` that touches no register
 * has no return type and no argument count; void(void) is the weakest model. */

void sub_0803B404(void)
{
}

/* Forwarder to m4aSoundMain, which is itself a forwarder to sub_0806F744
 * (m4aSoundMain). void(void) -- see sub_0803B3E0. */

void sub_0803B408(void)
{
    m4aSoundMain();
}
