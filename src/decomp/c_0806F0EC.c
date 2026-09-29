#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806F0EC.
 * sub_0806F0EC @ 0x0806F0EC
 */

#include "proc.h"
#include "hardware.h"
/* The input loop of the map-select screen: one arm per key, all mutually
 * exclusive, tested against a snapshot of two gpKeySt masks taken before any
 * call. The snapshots are plain locals -- neither survives a `bl` in any arm,
 * which is why they sit in r2/r3 rather than the callee-saved pair.
 *
 * The A and B arms both end in Proc_Break, and agbcc cross-jumps that one call;
 * it is written twice in the source.
 *
 * The Start arm's `proc->unk2c = p->unk58 = <table halfword>` is one chained
 * assignment -- that is why +0x58 of the blocking proc is stored BEFORE the
 * caller's own +0x2c. */
struct Unk6F0ECSubProc
{
    /* 0x00 */ u8 filler_00[0x48];
    /* 0x48 */ u8 unk48;
};
struct Unk6F0ECBlockProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};
struct Unk6F0ECProc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u8 filler_38[0x04];
    /* 0x3c */ struct Unk6F0ECSubProc *unk3c;
    /* 0x40 */ struct Unk6F0ECSubProc *unk40;
    /* 0x44 */ struct Unk6F0ECSubProc *unk44;
};

void SoundRoomMusicPage_Input(struct Unk6F0ECProc *proc)
{
    struct Unk6F0ECBlockProc *p;
    u16 held;
    u16 keys;

    held = gpKeySt->pressed;
    keys = gpKeySt->repeated;

    if (held & 8)
    {
        PlayMusicOrSfx2(0x66);
        proc->unk44->unk48 = 1;
        FadeOutMusicDefault();
        Proc_Goto(proc, 3);
        Proc_Break(proc);
    }
    else if (held & 4)
    {
        PlayMusicOrSfx2(0x66);
        gUnknown_0202F2CC = proc->unk2c;
        gUnknown_0202F2D0 = proc->unk30;
        gUnknown_0202F2D4 = proc->unk34;
        gUnknown_0202F2D6 = proc->unk36;
        Proc_Break(proc);
    }
    else if ((held & 1) && gUnknown_08582764[proc->unk30].unk02 != proc->unk2c)
    {
        PlayMusicOrSfx2(0x71);
        proc->unk3c->unk48 = 1;
        SoundRoomCycleBgPalette(proc);

        if (proc->unk2c != 0)
        {
            p = Proc_StartBlocking(gUnknown_08582E54, proc);
            proc->unk2c = p->unk58 = gUnknown_08582764[proc->unk30].unk02;
        }
        else
        {
            Proc_Start(gUnknown_08582C5C, proc);
            proc->unk2c = gUnknown_08582764[proc->unk30].unk02;
            PlayMusic((s16)proc->unk2c);
        }
    }
    else if ((held & 2) && proc->unk2c != 0)
    {
        PlayMusicOrSfx2(0x66);
        proc->unk40->unk48 = 1;
        p = Proc_StartBlocking(gUnknown_08582E54, proc);
        p->unk58 = -1;
        proc->unk2c = 0;
        Proc_Start(gUnknown_08582C3C, proc);
    }
    else if (keys & 0x20)
    {
        proc->unk30--;

        if (proc->unk30 == 0)
            proc->unk30 = 0x2e;

        FlickSoundRoomArrow(1);
        SoundRoom_RefreshCoPortrait((struct Unk0806F0A0Proc *)proc);
        StartSoundRoomTitleScroll(1, proc->unk30, proc);
    }
    else if (keys & 0x10)
    {
        proc->unk30++;

        if (proc->unk30 == 0x2f)
            proc->unk30 = 1;

        FlickSoundRoomArrow(0);
        SoundRoom_RefreshCoPortrait((struct Unk0806F0A0Proc *)proc);
        StartSoundRoomTitleScroll(-1, proc->unk30, proc);
    }
}
asm(".global sub_0806F0EC\n.thumb_set sub_0806F0EC, SoundRoomMusicPage_Input\n");
