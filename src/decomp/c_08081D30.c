#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08081D30.
 * sub_08081D30 @ 0x08081D30
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "hardware.h"
#include "proc.h"
/* The menu-input half of the 0x080819A0-0x08084974 block, and the caller of the
 * dispatcher promoted in src/decomp/c_080824D4.c. It runs only while the proc
 * is idle (`unk4e == 0 && unk6a == 0` and no ProcScr_FadeLoadMap proc alive) and
 * then splits on the s16 at +0x64: zero is the outer carousel (Left/Right step
 * gUnknown_0861696C's six-phase permutation at +0x52, A/B commit or cancel),
 * non-zero is the sub-menu on +0x66. Both arms end in the same two
 * "toggle a word and reset the timer" clauses on IsCampaignCompletionFlagSet(0x20)/(0x28),
 * and the function closes by running sub_080824D4 and sub_08084700.
 *
 * Promotion must carry rodata: ["0x081D93AC"], the -fforce-addr copy of
 * &gUnknown_0861696C -- baserom.gba has [0x081D93AC] = 0x0861696C. Note this
 * function reaches the SAME array both ways: `ldr rN,=gUnknown_0861696C` at
 * some sites and the double load `ldr r5,=0x081D93AC ; ldr r4,[r5]` at others.
 * Writing the global's name normally reproduces both; there is nothing to spell
 * differently, and 0x081D93AC is a pool slot, not a symbol to declare.
 *
 * WAVE 56, W56-R -- the one thing here that is not transcription, and it cost
 * every attempt but the first. Two `int` locals, both zeroed at entry (the
 * saved +0x4c timer and a flag), both spilled to the frame at sp+0 and sp+4.
 * The statement `p->unk4e = b` compiles to
 *     mov r3, sp ; ldrh r3, [r3, #4] ; strh r3, [r2]
 * and the ROM loads sp+4 -- the FLAG's own slot -- while every honest spelling
 * loaded sp+0. WHICH VARIABLE YOU NAME IN THAT STATEMENT MAKES NO DIFFERENCE:
 * both pseudos are equal to 0, so cse merges them into one quantity and
 * canon_reg rewrites the operand to that quantity's canonical register before
 * reload picks a slot. Writing `= a` and `= b` produce byte-identical output;
 * a sweep of all eight declaration-order x init-order x operand permutations
 * moved the two stack slots around but never moved this load.
 *
 * cse's canonical register is the one that LIVES LONGEST (make_regs_eqv keeps
 * the reg with the later REGNO_LAST_UID at the head of the quantity), and in
 * the tail `if (b != 0) p->unk4c = 0x22 - a;` the flag's last mention is the
 * compare, two insns BEFORE the value's. So the value wins and the load comes
 * from its slot. Resetting the flag after it is consumed --
 *     if (b != 0) { p->unk4c = 0x22 - a; b = 0; }
 * -- puts the flag's last mention last. reg_scan runs before cse, so cse sees
 * that store and flips the canonical; flow then deletes it as dead and it
 * costs nothing. Byte-exact, and the only change from the 99.9% draft.
 * A trailing `b = 0;` as the function's last statement matches identically, so
 * the placement inside the `if` is a readability choice, not evidence.
 *
 * The struct: +0x4c/+0x4e/+0x64/+0x66/+0x68/+0x6a are all `s16` (every read is
 * the THUMB register-offset `ldrsh`, which has no immediate form, so the
 * `movs rN, #0 ; ldrsh` pairs are forced and the signedness is a hard readout);
 * +0x52 is `u16` (plain `ldrh`, and `gUnknown_03005934 = p->unk52` stores it
 * zero-extended); +0x58 and +0x60 are `int`. This agrees with c_080824D4.c and
 * c_080819A0.c, which model the same proc through smaller windows.
 *
 * `p->unk6a = 0xFFFF` and `p->unk68 = 0xFFFF` are written as the unsigned
 * literal, not as -1: the ROM materialises them with `ldr rN, =0x0000FFFF` from
 * the pool, and -1 would fold to a two-instruction `movs/negs` sequence. */

struct Unk8081D30
{
    /* 00 */ STRUCT_PAD(0x00, 0x4c);
    /* 4c */ s16 unk4c;
    /* 4e */ s16 unk4e;
    /* 50 */ STRUCT_PAD(0x50, 0x52);
    /* 52 */ u16 unk52;
    /* 54 */ STRUCT_PAD(0x54, 0x58);
    /* 58 */ int unk58;
    /* 5c */ STRUCT_PAD(0x5c, 0x60);
    /* 60 */ int unk60;
    /* 64 */ s16 unk64;
    /* 66 */ s16 unk66;
    /* 68 */ s16 unk68;
    /* 6a */ s16 unk6a;
};
/* sub_080824D4 and MainMenuCarousel_ShowOverwriteWarning are already promoted -- signatures taken from
 * src/decomp/c_080824D4.c and src/decomp/c_08084974.c rather than derived.
 * sub_08084700 is still `asm` and has no include/ entry; c_0808177C.c and
 * c_080819A0.c each declare it against their own tag for the same object, and
 * this file follows that convention. Its argument is the proc pointer, passed
 * straight through in r0, and its result is never read. */
void sub_080824D4(struct Unk8081D30 *);
void sub_08084700(struct Unk8081D30 *);
void MainMenuCarousel_ShowOverwriteWarning(void);

void MainMenuCarouselWheel_InputLoop(struct Unk8081D30 *p)
{
    int a;
    int b;

    a = 0;
    b = 0;

    if (p->unk4e != 0 && p->unk64 == 0 && p->unk68 == 0 && p->unk4c > 0x1B
        && (gpKeySt->repeated & (DPAD_UP | DPAD_DOWN)))
    {
        p->unk4e = b;
        a = p->unk4c;
        b = 1;
    }

    if (p->unk4e == 0 && p->unk6a == 0 && Proc_Find(ProcScr_FadeLoadMap) == 0)
    {
        if (p->unk64 == 0)
        {
            if (gpKeySt->repeated & DPAD_UP)
            {
                if (p->unk52 == 5)
                    p->unk52 = 0;
                else
                    p->unk52 = p->unk52 + 1;

                p->unk4c = 0;
                p->unk4e = p->unk4e - 1;
                PlayMusicOrSfx2(0x67);
                gUnknown_03005920 = 0;
            }
            else if (gpKeySt->repeated & DPAD_DOWN)
            {
                if (p->unk52 == 0)
                    p->unk52 = 5;
                else
                    p->unk52 = p->unk52 - 1;

                p->unk4c = 0;
                p->unk4e = p->unk4e + 1;
                PlayMusicOrSfx2(0x67);
                gUnknown_03005920 = 0;
            }
            else if (gpKeySt->pressed & (A_BUTTON | DPAD_RIGHT))
            {
                if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 2
                    || gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 4)
                {
                    if (gpKeySt->pressed & 1)
                    {
                        p->unk6a = 1;

                        if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 2)
                            gUnknown_030033FC = 6;
                        else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 4)
                            gUnknown_030033FC = 9;

                        gUnknown_03005934 = p->unk52;
                        PlayMusicOrSfx2(0x71);
                        Proc_Start(gUnknown_08616A68, p);
                        LockMainMenu();
                    }
                }
                else
                {
                    p->unk64 = 1;
                    p->unk66 = 6;
                    p->unk68 = 1;
                    p->unk4c = 0;
                    p->unk4e = p->unk4e + 1;

                    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0 && GetCampaignSaveFlag() == 0)
                        p->unk66 = 7;

                    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1 && GetVersusSaveFlag() == 0)
                        p->unk66 = 7;

                    if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5 && GetWarRoomSaveFlag() == 0)
                        p->unk66 = 7;

                    if (gpKeySt->pressed & 1)
                        PlayMusicOrSfx2(0x71);
                    else if (gpKeySt->pressed & DPAD_RIGHT)
                        PlayMusicOrSfx2(0x67);
                }
            }
            else if (gpKeySt->pressed & 2)
            {
                p->unk6a = 0xFFFF;
                gUnknown_030033FC = 0xD;
                gUnknown_03005934 = p->unk52;
                gUnknown_03005924 = p->unk66;
                Proc_Start(gUnknown_08616A68, p);
                PlayMusicOrSfx2(0x66);
                UnlockMainMenu();
            }
        }
        else if (p->unk64 > 0)
        {
            if (gpKeySt->pressed & DPAD_UP)
            {
                if (p->unk66 == 6)
                    p->unk66 = 0xB;
                else
                    p->unk66 = p->unk66 - 1;

                if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0 && GetCampaignSaveFlag() == 0)
                    p->unk66 = 7;
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1 && GetVersusSaveFlag() == 0)
                    p->unk66 = 7;
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5 && GetWarRoomSaveFlag() == 0)
                    p->unk66 = 7;
                else
                {
                    PlayMusicOrSfx2(0x67);
                    gUnknown_03005920 = 0;
                }
            }
            else if (gpKeySt->pressed & DPAD_DOWN)
            {
                if (p->unk66 == 0xB)
                    p->unk66 = 6;
                else
                    p->unk66 = p->unk66 + 1;

                if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0 && GetCampaignSaveFlag() == 0)
                    p->unk66 = 7;
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1 && GetVersusSaveFlag() == 0)
                    p->unk66 = 7;
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5 && GetWarRoomSaveFlag() == 0)
                    p->unk66 = 7;
                else
                {
                    PlayMusicOrSfx2(0x67);
                    gUnknown_03005920 = 0;
                }
            }
            else if (gpKeySt->pressed & 1)
            {
                p->unk6a = 1;

                if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
                {
                    if (DivRem(p->unk66, 2) == 0)
                    {
                        gUnknown_030033FC = 0;
                    }
                    else if (DivRem(p->unk66, 2) == 1)
                    {
                        gUnknown_030033FC = 1;

                        if (GetCampaignSaveFlag() != 0)
                            MainMenuCarousel_ShowOverwriteWarning();
                    }
                }
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 1)
                {
                    if (DivRem(p->unk66, 2) == 0)
                    {
                        gUnknown_030033FC = 2;
                    }
                    else if (DivRem(p->unk66, 2) == 1)
                    {
                        gUnknown_030033FC = 3;

                        if (GetVersusSaveFlag() != 0)
                            MainMenuCarousel_ShowOverwriteWarning();
                    }
                }
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 5)
                {
                    if (DivRem(p->unk66, 2) == 0)
                    {
                        gUnknown_030033FC = 4;
                    }
                    else if (DivRem(p->unk66, 2) == 1)
                    {
                        gUnknown_030033FC = 5;

                        if (GetWarRoomSaveFlag() != 0)
                            MainMenuCarousel_ShowOverwriteWarning();
                    }
                }
                else if (gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 3)
                {
                    if (DivRem(p->unk66, 2) == 0)
                        gUnknown_030033FC = 8;
                    else if (DivRem(p->unk66, 2) == 1)
                        gUnknown_030033FC = 7;
                }

                gUnknown_03005934 = p->unk52;
                gUnknown_03005924 = p->unk66;
                PlayMusicOrSfx2(0x71);
                Proc_Start(gUnknown_08616A68, p);
                LockMainMenu();
            }
            else if (gpKeySt->pressed & (B_BUTTON | DPAD_LEFT))
            {
                p->unk68 = 0xFFFF;
                p->unk4c = 0;
                p->unk4e = p->unk4e + 1;

                if (gpKeySt->pressed & 2)
                    PlayMusicOrSfx2(0x66);
                else if (gpKeySt->pressed & DPAD_LEFT)
                    PlayMusicOrSfx2(0x67);
            }
        }

        if ((gpKeySt->pressed & 4) && IsCampaignCompletionFlagSet(0x20) != 0
            && gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 0)
        {
            gUnknown_03005968 = ~gUnknown_03005968 & 1;
            p->unk60 = 1;
            p->unk4c = 0;
        }

        if ((gpKeySt->pressed & 4) && IsCampaignCompletionFlagSet(0x28) != 0
            && gUnknown_0861696C[DivRem(p->unk52 + 2, 6)] == 2)
        {
            gUnknown_03005920 = ~gUnknown_03005920 & 1;

            if (gUnknown_03005920 != 0)
                LoadMainMenuCentreTileGraphic(6);
            else
                LoadMainMenuCentreTileGraphic(2);

            p->unk4c = 0;
        }
    }

    if (b != 0)
    {
        p->unk4c = 0x22 - a;
        b = 0;
    }

    sub_080824D4(p);
    sub_08084700(p);

    if (p->unk60 != 0 && p->unk4c == 4)
        p->unk60 = 0;

    p->unk4c = p->unk4c + 1;

    if (p->unk4e == 0)
        p->unk58 = p->unk58 + 1;
    else if (p->unk4c == 8)
        p->unk58 = 0;
}

asm(".global sub_08081D30\n.thumb_set sub_08081D30, MainMenuCarouselWheel_InputLoop\n");
