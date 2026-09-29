#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804009C.
 * sub_0804009C @ 0x0804009C
 */

#include "proc.h"
/* A four-way switch on the proc's unk54 mode, each arm setting a sound/timer
 * pair. The two NEGATIVE-ONE and NEGATIVE-TWO arms `return`; the -3 arm
 * `break`s and FALLS INTO the shared tail, which is also where every
 * unmatched value lands -- that is why the ROM's -3 body is immediately
 * followed by the tail with no branch, while the other two arms end in
 * `b` to the epilogue. There is no `default:` label: control simply leaves
 * the switch.
 *
 * unk14 is a POINTER, not the `int` its bare `ldr` suggests: it is handed to
 * StartScreenShake's third and StartWhiteFlash's fourth parameters, both declared
 * pointers, and -Werror rejects the integer spelling. */
struct Unk4009CProc
{
    /* 00 */ u8 filler_00[0x14];
    /* 14 */ void *unk14;
    /* 18 */ u8 filler_18[0x3c];
    /* 54 */ int unk54;
};

void ExplosionEffect_StartSoundAndScreenFx(struct Unk4009CProc *proc)
{
    switch (proc->unk54)
    {
    case -1:
        PlayMusicOrSfx2(0x1D5);
        StartScreenShake(2, 0x5A, proc->unk14);
        StartWhiteFlash(0x5A, 0xF, 0x1E, proc->unk14);
        return;
    case -2:
        PlayMusicOrSfx2(0x1D5);
        StartScreenShake(2, 0xA0, proc->unk14);
        StartWhiteFlash(0xA0, 0x1E, 0x32, proc->unk14);
        return;
    case -3:
        PlayMusicOrSfx2(0x1E2);
        StartScreenShake(1, 0x14, proc->unk14);
        StartWhiteFlash(4, 0, 2, proc->unk14);
        break;
    }

    PlayMusicOrSfx2(0x10);
    StartScreenShake(1, 0x14, proc->unk14);

    if (gPlaySt.gameMode != 5)
        StartWhiteFlash(4, 0, 2, proc->unk14);
}
asm(".global sub_0804009C\n.thumb_set sub_0804009C, ExplosionEffect_StartSoundAndScreenFx\n");
