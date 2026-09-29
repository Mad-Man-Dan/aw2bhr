#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035DF4.
 * sub_08035DF4 @ 0x08035DF4, sub_08035E00 @ 0x08035E00, sub_08035E24 @ 0x08035E24, sub_08035E6C @ 0x08035E6C
 */

void FadeOutMusicPlayer(void *a)
{
    m4aMPlayFadeOut(a, 1);
}
asm(".global sub_08035DF4\n.thumb_set sub_08035DF4, FadeOutMusicPlayer\n");

void SoundPitchDrop_End(struct Unk03001470 *p)
{
    FadeOutMusicPlayer(gUnknown_03005BA0);
    FadeOutMusicPlayer(gUnknown_030059E0);
    p->unk1e = 0;
}
asm(".global sub_08035E00\n.thumb_set sub_08035E00, SoundPitchDrop_End\n");

/* The first MPlayPitchControl call reuses the value just stored to unk1e; the second
 * reloads it, because the intervening call may have written to it. Both are
 * what the plain `-p->unk1e` spelling gives. */
void SoundPitchDrop_Loop(struct Unk03001470 *p)
{
    if (p->unk1e > 0x27f)
        p->unk08 = 0;

    p->unk1e += 0x14;
    MPlayPitchControl(gUnknown_03005BA0, 1, -p->unk1e);
    MPlayPitchControl(gUnknown_030059E0, 1, -p->unk1e);
}
asm(".global sub_08035E24\n.thumb_set sub_08035E24, SoundPitchDrop_Loop\n");

void SoundPitchDrop_Init(void)
{
    MPlayPitchControl(gUnknown_03005BA0, 1, 0);
    MPlayPitchControl(gUnknown_030059E0, 1, 0);
}
asm(".global sub_08035E6C\n.thumb_set sub_08035E6C, SoundPitchDrop_Init\n");
