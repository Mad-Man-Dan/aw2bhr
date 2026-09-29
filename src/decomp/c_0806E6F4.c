#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E6F4.
 * sub_0806E6F4 @ 0x0806E6F4
 */

#include "proc.h"
/* Reads a coordinate pair out of GetSoundScopeLevels into two adjacent halfword slots
 * of its own frame -- `sub sp, #4` reserves exactly the two -- and biases both
 * by 0x100 into the two sprites the owner proc holds.
 *
 * ONE binding local for both sprite pointers, not two. Written as two separate
 * locals (or inline as `owner->unk4c->unk3c`), the second pointer picks up r1
 * instead of re-using r2 and the function misses by that register alone, size
 * exact. This is the wave-17 "binding locals are punctuation" rule read in the
 * SAVING direction: one pseudo whose live range covers both statements settles
 * on one hard register, where two pseudos are free to land anywhere. */
struct Unk6E6F4Sprite
{
    /* 00 */ u8 filler_00[0x3c];
    /* 3c */ u16 unk3c;
};
struct Unk6E6F4Owner
{
    /* 00 */ u8 filler_00[0x48];
    /* 48 */ struct Unk6E6F4Sprite *unk48;
    /* 4c */ struct Unk6E6F4Sprite *unk4c;
};
struct Unk6E6F4Proc
{
    /* 00 */ u8 filler_00[0x54];
    /* 54 */ struct Unk6E6F4Owner *unk54;
};

void SoundRoomLevelPulse_Loop(struct Unk6E6F4Proc *proc)
{
    struct Unk6E6F4Owner *owner = proc->unk54;
    struct Unk6E6F4Sprite *sprite;
    u16 x;
    u16 y;

    GetSoundScopeLevels(&x, &y);
    sprite = owner->unk48;
    sprite->unk3c = 0x100 + x;
    sprite = owner->unk4c;
    sprite->unk3c = 0x100 + y;
}
asm(".global sub_0806E6F4\n.thumb_set sub_0806E6F4, SoundRoomLevelPulse_Loop\n");
