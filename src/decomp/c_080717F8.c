#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080717F8.
 * sub_080717F8 @ 0x080717F8, sub_08071840 @ 0x08071840, sub_08071854 @ 0x08071854, sub_08071868 @ 0x08071868, sub_0807187C @ 0x0807187C, sub_08071890 @ 0x08071890, sub_080718A4 @ 0x080718A4, sub_080718B0 @ 0x080718B0, sub_080718BC @ 0x080718BC, sub_080718D0 @ 0x080718D0
 */

#define READ_XCMD_BYTE(var, n)       \
{                                    \
    u32 byte = track->cmdPtr[(n)]; \
    byte <<= n * 8;                  \
    (var) &= ~(0xFF << (n * 8));     \
    (var) |= byte;                   \
}

void sub_080717F8(void *mplayInfo, struct MusicPlayerTrack *track)
{
    u32 wav;

    READ_XCMD_BYTE(wav, 0)
    READ_XCMD_BYTE(wav, 1)
    READ_XCMD_BYTE(wav, 2)
    READ_XCMD_BYTE(wav, 3)

    track->tone.wav = wav;
    track->cmdPtr += 4;
}

void sub_08071840(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.type = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_08071854(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.attack = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_08071868(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.decay = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_0807187C(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.sustain = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_08071890(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.release = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_080718A4(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->echoVolume = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_080718B0(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->echoLength = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_080718BC(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.length = *track->cmdPtr;
    track->cmdPtr++;
}

void sub_080718D0(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.pan_sweep = *track->cmdPtr;
    track->cmdPtr++;
}
