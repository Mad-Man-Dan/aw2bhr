#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08031E7C.
 * sub_08031E7C @ 0x08031E7C, sub_08031E9C @ 0x08031E9C, sub_08031ED0 @ 0x08031ED0, sub_08031EE4 @ 0x08031EE4, sub_08031F10 @ 0x08031F10, sub_08031F28 @ 0x08031F28, sub_08031F5C @ 0x08031F5C, sub_08031F88 @ 0x08031F88, sub_08031FB8 @ 0x08031FB8
 */

struct Unk31E7CProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58; /* a frame counter: sub_08031E9C decrements it every
                           * fourth frame and Proc_Break's when it reaches 0 */
};

#include "proc.h"
struct Unk31E9CProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

/* +0x1e is BELOW PROC_HEADER's 0x29 bytes -- it lands inside proc_next -- so
 * whatever this callback is handed, it is not being read as a `struct Proc`.
 * The record is modelled flat here for that reason; nothing in the trio
 * sub_08031ED0 / sub_08031EE4 / sub_08031F10 touches a proc member or calls a
 * Proc_ function, so there is no evidence pulling the other way. */
struct Unk31ED0Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e; /* signed: sub_08031EE4 tests the decremented value
                           * with `lsls #0x10; cmp #0; bge`, i.e. bit 15 as a
                           * sign */
};

struct Unk31EE4Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

struct Unk31F10Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

struct Unk31F88Proc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

/* `movs r1, #1; rsbs r1, r1, #0` here is the CONSTANT -1 handed to
 * ColFadeFromWhiteRow as its second argument, NOT the negate-and-subtract bitfield
 * mask -- the value goes straight out with no `ands` anywhere. */

void sub_08031E7C(struct Unk31E7CProc *proc)
{
    proc->unk58 = 0x20;

    ColorFadeInit();
    ColFadeFromWhiteRow(0x11, -1);
    sub_080718F0();
    EnablePaletteSync();
}

/* sub_08031E7C's repeat half: the counter it arms with 0x20 ticks down once
 * every fourth frame, `gGameClock & 3` being the frame phase. */

void sub_08031E9C(struct Unk31E9CProc *proc)
{
    if ((gGameClock & 3) == 0)
    {
        sub_080718F0();
        EnablePaletteSync();

        proc->unk58--;
    }

    if (proc->unk58 == 0)
        Proc_Break(proc);
}

void sub_08031ED0(struct Unk31ED0Proc *proc)
{
    proc->unk1e = 0x20;

    ColFadeToWhite(1);
    EnablePaletteSync();
}

/* The test reads the DECREMENTED value out of the register rather than
 * reloading it, so the decrement and the compare are one expression:
 * `if (--proc->unk1e < 0)`. Splitting them into two statements costs an
 * `ldrsh`. */

void sub_08031EE4(struct Unk31EE4Proc *proc)
{
    sub_080718F0();
    EnablePaletteSync();

    if (--proc->unk1e < 0)
        ClearSlotScriptCallback(gUnknown_03001FBC);
}

/* sub_08031ED0 with the other direction: the same 0x20 arm, and ColFadeFromWhite
 * instead of ColFadeToWhite with -1 instead of 1. `movs r0,#1; rsbs r0,r0,#0` is
 * the constant, not a mask. */

void sub_08031F10(struct Unk31F10Proc *proc)
{
    proc->unk1e = 0x20;

    ColFadeFromWhite(-1);
    EnablePaletteSync();
}

void sub_08031F28(void)
{
    PlayMusicOrSfx2(0x6c);

    if (gUnknown_0849B018->unk00 == 3)
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 12, 2);
}

void sub_08031F5C(void)
{
    if (gUnknown_0849B018->unk00 == 3)
        gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 13, 0);
}

/* The `lsls #0x18; lsrs #0x18` on `unk58 + 5` is sub_0801A7D8's declared u8
 * first parameter narrowing an int expression, not a cast in the source. */

void sub_08031F88(struct Unk31F88Proc *proc)
{
    if (gUnknown_0849B018->unk00 == 3)
        sub_0801A7D8(proc->unk58 + 5, gUnknown_02000000, 0x724);
}

/* The bare form of the LinkScreenSetMessage round trip this block repeats eight times:
 * the pointer global is reloaded after the call because the call may have
 * changed it, which is what makes r4 worth saving. */

void sub_08031FB8(void)
{
    gUnknown_0849B060->unk00 = LinkScreenSetMessage(gUnknown_0849B060->unk00, 5, 2);
}
