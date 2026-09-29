#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803B640.
 * sub_0803B640 @ 0x0803B640, sub_0803B660 @ 0x0803B660, sub_0803B680 @ 0x0803B680, sub_0803B684 @ 0x0803B684, sub_0803B688 @ 0x0803B688
 */

/* The drain half of PlayMusicIfEnabled: if a song id was PARKED in gUnknown_030005C8
 * while sound was suppressed (i.e. the slot is not the 0xFFFF sentinel), start
 * it now. The `ldrh` for the sentinel compare and the `ldrsh` for the argument
 * read the SAME u16 global two different ways, which is not a contradiction --
 * the second is the explicit (s16) conversion for PlayMusic's `int`
 * parameter folded into the load, and it is the tell the gUnknown_030005C8
 * comment in include/unknown-globals.h already records. Without the cast agbcc
 * re-uses the halfword already in r1 (`adds r0, r1, #0`) and the `ldrsh`
 * disappears, so the cast is in the original source.
 * `pop {r0}; bx r0` -> void. */

void ReplayPendingMusic(void)
{
    if (gUnknown_030005C8 != 0xFFFF)
        PlayMusic((s16)gUnknown_030005C8);
}
asm(".global sub_0803B640\n.thumb_set sub_0803B640, ReplayPendingMusic\n");

/* Byte-for-byte the same function as ReplayPendingMusic (src/decomp/c_0803B640.c)
 * with PlayMusicIfEnabled in place of PlayMusic -- the two are the "restart the
 * parked song" pair, one going through the suppression check and one straight
 * to the player. Everything in the c_0803B640 note applies verbatim: the
 * `ldrh` for the 0xFFFF sentinel compare and the `ldrsh` for the argument read
 * the same u16 global two ways, and the second is the explicit (s16)
 * conversion for PlayMusicIfEnabled's `int` parameter folded into the load. Drop the
 * cast and agbcc re-uses the halfword already sitting in r1.
 * `pop {r0}; bx r0` -> void. */

void ReplayPendingMusicIfEnabled(void)
{
    if (gUnknown_030005C8 != 0xFFFF)
        PlayMusicIfEnabled((s16)gUnknown_030005C8);
}
asm(".global sub_0803B660\n.thumb_set sub_0803B660, ReplayPendingMusicIfEnabled\n");

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` behind it. Nothing about the signature is recoverable;
 * void(void) is the weakest model. See SoundMainLoopNoOp in
 * src/decomp/c_0803B3C8.c. */

void sub_0803B680(void)
{
}

/* A do-nothing stub: the whole body is `bx lr`, padded to 4 bytes by the
 * `.align 2, 0` behind it. It is the second of an adjacent PAIR with
 * sub_0803B680, which is the shape a matched/unmatched or install/remove pair
 * of hooks takes when both sides are empty. Nothing about the signature is
 * recoverable; void(void) is the weakest model. */

void sub_0803B684(void)
{
}

/* Two statements, both into the m4a forwarder run: select sound-mode preset 3
 * out of gUnknown_080910FC (SetSoundModeByIndex) and then re-apply the mixer channel
 * count (SetSoundMixerChannelCount8, which is `SetSoundMixerChannelCount(8)`). The `movs r0, #3` is
 * consumed by the first call and the second takes no argument, so the two `bl`s
 * are sequential statements and not a nesting -- SetSoundMixerChannelCount8 is declared
 * void(void) and writes r0 before reading it.
 * `pop {r0}; bx r0` -> void. */

void InitSoundMode(void)
{
    SetSoundModeByIndex(3);
    SetSoundMixerChannelCount8();
}
asm(".global sub_0803B688\n.thumb_set sub_0803B688, InitSoundMode\n");
