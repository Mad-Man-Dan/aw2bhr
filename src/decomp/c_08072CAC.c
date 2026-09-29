#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08072CAC.
 * sub_08072CAC @ 0x08072CAC, sub_08072CE4 @ 0x08072CE4, sub_08072E70 @ 0x08072E70, sub_08072F04 @ 0x08072F04, sub_08072F70 @ 0x08072F70
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
struct UnkCACProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ u8 unk46[14];
    /* 54 */ STRUCT_PAD(0x54, 0x58);
    /* 58 */ int unk58;
    /* 5c */ STRUCT_PAD(0x5c, 0x64);
    /* 64 */ int unk64;
};
#include "hardware.h"
struct UnkCE4Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ STRUCT_PAD(0x46, 0x54);
    /* 54 */ u8 unk54;
    /* 55 */ STRUCT_PAD(0x55, 0x58);
    /* 58 */ int unk58;
    /* 5c */ u16 unk5c;
    /* 5e */ u8 unk5e;
    /* 5f */ u8 unk5f;
    /* 60 */ u16 unk60;
    /* 62 */ STRUCT_PAD(0x62, 0x64);
    /* 64 */ int unk64;
};
struct UnkE70Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ u8 unk46[14];
    /* 54 */ u8 unk54;
    /* 55 */ STRUCT_PAD(0x55, 0x58);
    /* 58 */ int unk58;
    /* 5c */ u16 unk5c;
    /* 5e */ u8 unk5e;
    /* 5f */ STRUCT_PAD(0x5f, 0x64);
    /* 64 */ int unk64;
};
struct UnkF04Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ STRUCT_PAD(0x46, 0x54);
    /* 54 */ u8 unk54;
    /* 55 */ STRUCT_PAD(0x55, 0x58);
    /* 58 */ int unk58;
    /* 5c */ u16 unk5c;
    /* 5e */ u8 unk5e;
    /* 5f */ STRUCT_PAD(0x5f, 0x64);
    /* 64 */ int unk64;
    /* 68 */ u8 unk68;
};
struct UnkF70Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2a);
    /* 2a */ s16 unk2a[14];
    /* 46 */ STRUCT_PAD(0x46, 0x54);
    /* 54 */ u8 unk54;
    /* 55 */ STRUCT_PAD(0x55, 0x58);
    /* 58 */ int unk58;
    /* 5c */ u16 unk5c;
    /* 5e */ u8 unk5e;
    /* 5f */ u8 unk5f;
    /* 60 */ STRUCT_PAD(0x60, 0x64);
    /* 64 */ int unk64;
    /* 68 */ u8 unk68;
};

/* The "snap to target" shortcut of the 0x08072CE4/0x08072E70 bar-animation
 * procs: when the timer at +0x64 is still negative, copy the u8 targets at
 * +0x46 into the s16 working values at +0x2a and jump the script to label 0.
 * +0x58 is reloaded on every iteration because the `strh` through +0x2a may
 * alias it as far as the compiler can tell. */
void HeaderBanner_SnapIfTimerNegative(struct UnkCACProc *proc)
{
    int i;

    if (proc->unk64 < 0)
    {
        for (i = 0; i < proc->unk58; i++)
            proc->unk2a[i] = proc->unk46[i];

        Proc_Goto(proc, 0);
    }
}

/* The staggered "pop in" draw: entry i starts moving on frame i*4 and rides
 * the 21-entry scale ramp gUnknown_081CBFC8; once the ramp reads 0x100 the
 * entry is drawn as a plain sprite, otherwise affine with OBJ-affine slot
 * unk5f + (i & 7). The angle is a constant 0, so SIN_Q12(0) is gSinLut[0] and
 * COS_Q12(0) is gSinLut[0x40] and this is a pure scale; Div is called four
 * times with two identical argument pairs because agbcc does not CSE calls.
 *
 * `t` is REUSED for the second arm's `i * 8` (decomp-permuter found this;
 * every hand version had `i * 8` written out in both arms). With the same
 * expression in both arms, loop.c makes `i * 8` a general induction variable
 * -- a third one alongside `i` and `i * 2` -- and it lands in a fifth frame
 * slot, costing `sub sp, #0x14` where the ROM has `#0x10` plus an
 * `ldr/adds/str` triple in the loop increment. Reusing an already-live local
 * for one of the two occurrences leaves the giv with a single use and loop.c
 * declines it, which is what the ROM has: `mov r4, sb; lsls r3, r4, #3`
 * recomputed in each arm. This is the wave-17 "binding locals are punctuation"
 * rule read backwards -- FEWER distinct pseudos, not more. */
void HeaderBanner_PopInLoop(struct UnkCE4Proc *proc)
{
    int i;
    int t;
    s32 scale;

    for (i = 0; i < proc->unk58; i++)
    {
        if (proc->unk64 >= i * 4)
        {
            t = proc->unk64 - i * 4;

            if (t > 0x14)
                scale = 0x100;
            else
                scale = gUnknown_081CBFC8[t];

            if (scale == 0x100)
            {
                PutSprite(1, proc->unk2a[i], proc->unk54, gUnknown_081CC01C,
                    i * 8 + proc->unk5c + (proc->unk5e << 12));
            }
            else
            {
                SetObjAffine(proc->unk5f + (i & 7),
                    Div(COS_Q12(0) * 16, scale),
                    Div(-SIN_Q12(0) * 16, scale),
                    Div(SIN_Q12(0) * 16, scale),
                    Div(COS_Q12(0) * 16, scale));

                t = i * 8;

                PutSpriteExt(1,
                    ((proc->unk2a[i] - 8) & 0x1FF) | ((proc->unk5f + (i & 7)) << 9),
                    ((proc->unk54 - 0x10) & 0xFF) | 0x300,
                    gUnknown_081CC01C,
                    t + proc->unk5c + (proc->unk5e << 12));
            }
        }
    }

    proc->unk64++;

    if (proc->unk64 > proc->unk60)
    {
        proc->unk64 = 0;
        Proc_Break(proc);
    }
}

/* The eight-frame slide of the bar: every entry's x eases from its current
 * s16 at +0x2a to its u8 target at +0x46 over unk64 = 0..8, then on the frame
 * after the last one the targets are committed into +0x2a and the proc
 * breaks. Same sprite layout as HeaderBanner_HoldLoop. */
void HeaderBanner_SlideToTargetLoop(struct UnkE70Proc *proc)
{
    int i;

    for (i = 0; i < proc->unk58; i++)
    {
        PutSprite(1,
            Interpolate(0, proc->unk2a[i], proc->unk46[i], proc->unk64, 8),
            proc->unk54,
            gUnknown_081CC01C,
            i * 8 + proc->unk5c + (proc->unk5e << 12));
    }

    proc->unk64++;

    if (proc->unk64 > 8)
    {
        for (i = 0; i < proc->unk58; i++)
            proc->unk2a[i] = proc->unk46[i];

        Proc_Break(proc);
    }
}

/* The static (non-animating) draw of the same bar the 0x08072E70 proc
 * animates: one sprite per entry, skipping entries whose s16 value at +0x2a
 * has fallen to -0x10 or below. +0x5c is the base OAM attr2 word, +0x5e the
 * palette/priority nibble at bit 12, and each entry is 8 pixels further along.
 * The `unk68` flag makes the proc break on the frame it is set. */
void HeaderBanner_HoldLoop(struct UnkF04Proc *proc)
{
    int i;

    if (proc->unk68 != 0)
    {
        proc->unk64 = 0;
        Proc_Break(proc);
    }

    for (i = 0; i < proc->unk58; i++)
    {
        if (proc->unk2a[i] > -0x10)
            PutSprite(1, proc->unk2a[i], proc->unk54, gUnknown_081CC01C,
                i * 8 + proc->unk5c + (proc->unk5e << 12));
    }
}

/* The "fly in" variant of the bar draw: over unk68 frames each entry's x
 * interpolates from its +0x2a value to -0x14 while a second Interpolate ramps
 * a scale from 0x100 down to 0x10, and the sprite is drawn affine with
 * OBJ-affine slot unk5f + i. The angle is a constant 0, so SIN_Q12(0) is
 * gSinLut[0] and COS_Q12(0) is gSinLut[0x40] -- both read once and CSEd
 * across the four Div calls. Only pa and pc divide by the animated scale;
 * pb and pd divide by the fixed 0x100, which is what makes the sprite
 * squash horizontally only.
 *
 * The reset value is spelled 0xFFEC and not -0x14: agbcc materialises -0x14
 * as `movs #0x14; rsbs` (it does exactly that for Interpolate's third
 * argument above), and only the already-truncated 0xFFEC needs the pool word
 * the ROM has. */
void HeaderBanner_FlyOutLoop(struct UnkF70Proc *proc)
{
    int i;
    s32 v;
    s32 scale;

    for (i = 0; i < proc->unk58; i++)
    {
        v = Interpolate(0, proc->unk2a[i], -0x14, proc->unk64, proc->unk68);
        scale = Interpolate(0, 0x100, 0x10, proc->unk64, proc->unk68);

        SetObjAffine(proc->unk5f + i,
            Div(COS_Q12(0) * 16, scale),
            Div(-SIN_Q12(0) * 16, 0x100),
            Div(SIN_Q12(0) * 16, scale),
            Div(COS_Q12(0) * 16, 0x100));

        PutSpriteExt(1,
            (v & 0x1FF) | ((proc->unk5f + i) << 9),
            proc->unk54 | 0x100,
            gUnknown_081CC01C,
            i * 8 + proc->unk5c + (proc->unk5e << 12));
    }

    proc->unk64++;

    if (proc->unk64 == proc->unk68)
    {
        for (i = 0; i < proc->unk58; i++)
            proc->unk2a[i] = 0xFFEC;

        proc->unk68 = 0;
        Proc_Break(proc);
    }
}

asm(".global sub_08072CAC\n.thumb_set sub_08072CAC, HeaderBanner_SnapIfTimerNegative\n"
    ".global sub_08072CE4\n.thumb_set sub_08072CE4, HeaderBanner_PopInLoop\n"
    ".global sub_08072E70\n.thumb_set sub_08072E70, HeaderBanner_SlideToTargetLoop\n"
    ".global sub_08072F04\n.thumb_set sub_08072F04, HeaderBanner_HoldLoop\n"
    ".global sub_08072F70\n.thumb_set sub_08072F70, HeaderBanner_FlyOutLoop\n");

extern void HeaderBanner_FlyInLoop(void);

struct ProcCmd CONST_DATA ProcScr_MainMenuPutSelectModeSprite[] =
{
    PROC_YIELD,
    PROC_END_DUPS,
    PROC_CALL(HeaderBanner_SnapIfTimerNegative),
    PROC_REPEAT(HeaderBanner_PopInLoop),
    PROC_REPEAT(HeaderBanner_SlideToTargetLoop),
PROC_LABEL(0),
    PROC_REPEAT(HeaderBanner_HoldLoop),
    PROC_REPEAT(HeaderBanner_FlyOutLoop),
    PROC_REPEAT(HeaderBanner_HoldLoop),
    PROC_REPEAT(HeaderBanner_FlyInLoop),
    PROC_GOTO(0),
    PROC_END,
};

asm(".global ProcScr_MainMenuC3\n.set ProcScr_MainMenuC3, ProcScr_MainMenuPutSelectModeSprite\n"
    ".global gUnknown_086140D4\n.set gUnknown_086140D4, ProcScr_MainMenuPutSelectModeSprite\n");
