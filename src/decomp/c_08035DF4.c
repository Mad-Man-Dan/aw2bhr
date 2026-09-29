#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08035DF4.
 * sub_08035DF4 @ 0x08035DF4, sub_08035E00 @ 0x08035E00, sub_08035E24 @ 0x08035E24, sub_08035E6C @ 0x08035E6C
 */

void sub_08035DF4(void *a)
{
    m4aMPlayFadeOut(a, 1);
}

void sub_08035E00(struct Unk03001470 *p)
{
    sub_08035DF4(gUnknown_03005BA0);
    sub_08035DF4(gUnknown_030059E0);
    p->unk1e = 0;
}

/* The first MPlayPitchControl call reuses the value just stored to unk1e; the second
 * reloads it, because the intervening call may have written to it. Both are
 * what the plain `-p->unk1e` spelling gives. */
void sub_08035E24(struct Unk03001470 *p)
{
    if (p->unk1e > 0x27f)
        p->unk08 = 0;

    p->unk1e += 0x14;
    MPlayPitchControl(gUnknown_03005BA0, 1, -p->unk1e);
    MPlayPitchControl(gUnknown_030059E0, 1, -p->unk1e);
}

void sub_08035E6C(void)
{
    MPlayPitchControl(gUnknown_03005BA0, 1, 0);
    MPlayPitchControl(gUnknown_030059E0, 1, 0);
}
