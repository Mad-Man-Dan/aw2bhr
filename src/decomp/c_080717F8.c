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

void ply_xwave(void *mplayInfo, struct MusicPlayerTrack *track)
{
    u32 wav;

    READ_XCMD_BYTE(wav, 0)
    READ_XCMD_BYTE(wav, 1)
    READ_XCMD_BYTE(wav, 2)
    READ_XCMD_BYTE(wav, 3)

    track->tone.wav = wav;
    track->cmdPtr += 4;
}
asm(".global sub_080717F8\n.thumb_set sub_080717F8, ply_xwave\n");

void ply_xtype(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.type = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_08071840\n.thumb_set sub_08071840, ply_xtype\n");

void ply_xatta(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.attack = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_08071854\n.thumb_set sub_08071854, ply_xatta\n");

void ply_xdeca(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.decay = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_08071868\n.thumb_set sub_08071868, ply_xdeca\n");

void ply_xsust(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.sustain = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_0807187C\n.thumb_set sub_0807187C, ply_xsust\n");

void ply_xrele(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.release = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_08071890\n.thumb_set sub_08071890, ply_xrele\n");

void ply_xiecv(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->echoVolume = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_080718A4\n.thumb_set sub_080718A4, ply_xiecv\n");

void ply_xiecl(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->echoLength = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_080718B0\n.thumb_set sub_080718B0, ply_xiecl\n");

void ply_xleng(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.length = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_080718BC\n.thumb_set sub_080718BC, ply_xleng\n");

void ply_xswee(void *mplayInfo, struct MusicPlayerTrack *track)
{
    track->tone.pan_sweep = *track->cmdPtr;
    track->cmdPtr++;
}
asm(".global sub_080718D0\n.thumb_set sub_080718D0, ply_xswee\n");
