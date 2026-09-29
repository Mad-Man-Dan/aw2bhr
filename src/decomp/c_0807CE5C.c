#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807CE5C.
 * sub_0807CE5C @ 0x0807CE5C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"
/* CoSelect_IDLE_0807CE5D @ 0x0807CE5C, 2468 bytes.
 *
 * The per-frame update of the unit-select / group-cursor proc that
 * IsCoSelectionUnique, sub_0807D860 and sub_0807D918 (all promoted) service.  The
 * parameter is the same object those three model; this is the widest window on
 * it so far, so struct Unk7D918's filler is widened here rather than
 * contradicted.
 */

struct Unk807CE5C
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ int unk40;
    /* 0x44 */ u8 filler_44[0x04];
    /* 0x48 */ u16 unk48;
    /* 0x4a */ u8 filler_4a[0x02];
    /* 0x4c */ s16 unk4c;
    /* 0x4e */ s16 unk4e;
    /* 0x50 */ u8 filler_50[0x02];
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 filler_54[0x04];
    /* 0x58 */ int unk58;
    /* 0x5c */ int unk5c;
    /* 0x60 */ int unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6a */ u16 unk6a;
};
/* Promoted but undeclared; signatures taken from src/decomp/, not derived. */
void SetArmyCoIdsFromList(u8 *);
int IsEndCoSelectRunning(void);
void sub_0807D860(struct Unk807CE5C *);
void sub_0807D918(struct Unk807CE5C *);
/* CoSelect_DrawSprites is W56-B's; since wave 56 it is declared in
 * include/unknown-functions.h as `void CoSelect_DrawSprites(struct Unk7DA98 *)`, so
 * this unit casts its own proc pointer to that incomplete tag at the call site
 * rather than redeclaring it. */
/* Not assigned to anyone this wave; signature read off the sole call site,
 * `adds r0, r7, #0; bl CoSelect_AssignRemainingAndFinish` with the result never read. */
void CoSelect_AssignRemainingAndFinish(struct Unk807CE5C *);

void CoSelect_IDLE_0807CE5D(struct Unk807CE5C *p)
{
    int i;

    if (!(u8)IsEndCoSelectRunning() && p->unk4e == 0 && p->unk60 == 0 && p->unk66 == 0)
    {
        if (p->unk40 == 0 && (gpKeySt->repeated & DPAD_LEFT))
        {
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014200, 0x16);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014680, 0x17);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014B00, 0x18);

            p->unk4c = 0;

            if (p->unk52 == 0)
                p->unk52 = gUnknown_03005948[p->unk58] - 1;
            else
                p->unk52 = p->unk52 - 1;

            p->unk4e = -1;

            LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013D80, 0x15);
            PlayMusicOrSfx2(0x67);
        }
        else if (p->unk40 == 0 && (gpKeySt->repeated & DPAD_RIGHT))
        {
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014200, 0x16);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 1, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014680, 0x17);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06014B00, 0x18);

            p->unk4c = 0;

            if (p->unk52 == gUnknown_03005948[p->unk58] - 1)
                p->unk52 = 0;
            else
                p->unk52 = p->unk52 + 1;

            p->unk4e = 1;

            LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
            LoadCoFace(gUnknown_030058E0[DivRem(p->unk52 + 2, gUnknown_03005948[p->unk58]) + p->unk5c], (void *)0x06013D80, 0x15);
            PlayMusicOrSfx2(0x67);
        }
        else if (p->unk40 == 0 && (gpKeySt->repeated & DPAD_UP) && (int)gUnknown_03005944 > 1 && gUnknown_030059C0[p->unk64] != 0)
        {
            gUnknown_03005938[0] = p->unk52;
            gUnknown_03005938[1] = p->unk58;
            gUnknown_03005938[2] = p->unk5c;
            gUnknown_03005950[p->unk58] = p->unk52;

            if (p->unk58 == 0)
                p->unk58 = gUnknown_03005944;
            p->unk58--;

            while (gUnknown_03005910[p->unk58] != 0)
            {
                if (p->unk58 == 0)
                    p->unk58 = gUnknown_03005944;
                p->unk58--;
            }

            p->unk5c = 0;
            p->unk52 = gUnknown_03005950[p->unk58];
            p->unk4c = 0;

            for (i = 0; i < p->unk58; i++)
                p->unk5c += gUnknown_03005948[i];

            p->unk60 = -1;

            LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
            ApplyWindowFramePalette(gUnknown_03005958[p->unk58], 8);
            PlayMusicOrSfx2(0x67);
        }
        else if (p->unk40 == 0 && (gpKeySt->repeated & DPAD_DOWN) && (int)gUnknown_03005944 > 1 && gUnknown_030059C0[p->unk64] != 0)
        {
            gUnknown_03005938[0] = p->unk52;
            gUnknown_03005938[1] = p->unk58;
            gUnknown_03005938[2] = p->unk5c;
            gUnknown_03005950[p->unk58] = p->unk52;

            p->unk58++;
            if (p->unk58 == gUnknown_03005944)
                p->unk58 = 0;

            while (gUnknown_03005910[p->unk58] != 0)
            {
                p->unk58++;
                if (p->unk58 == gUnknown_03005944)
                    p->unk58 = 0;
            }

            p->unk5c = 0;
            p->unk52 = gUnknown_03005950[p->unk58];
            p->unk4c = 0;

            for (i = 0; i < p->unk58; i++)
                p->unk5c += gUnknown_03005948[i];

            p->unk60 = 1;

            ApplyWindowFramePalette(gUnknown_03005958[p->unk58], 8);
            LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
            PlayMusicOrSfx2(0x67);
        }
        else if (gpKeySt->pressed & 1)
        {
            if (p->unk40 > 0)
            {
                p->unk4c = 0;
                p->unk6a = 0;
                SetArmyCoIdsFromList(gUnknown_030058D4);
                LockMainMenu();
                Proc_Start(gUnknown_08616690, p);
                SetHeaderBannerTransitionFrames(0x10);
                Proc_Break(p);
                PlayMusicOrSfx2(0x71);
            }

            if ((u8)IsCoSelectionUnique((struct Unk807D800 *)p))
            {
                gUnknown_030058D4[p->unk64] = gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c];
                p->unk64++;
                p->unk66++;
                p->unk4c = 0;
                gUnknown_0300599C[p->unk64 - 1] = p->unk58;

                if (p->unk64 == sub_0803BD14() && p->unk40 == 0)
                {
                    p->unk40 = 1;
                }
                else
                {
                    if (gUnknown_030059C0[p->unk64] != 0)
                        gUnknown_03005910[p->unk58] = 1;

                    gUnknown_03005938[0] = p->unk52;
                    gUnknown_03005938[1] = p->unk58;
                    gUnknown_03005938[2] = p->unk5c;
                    gUnknown_03005950[p->unk58] = p->unk52;

                    p->unk58++;
                    if (p->unk58 == gUnknown_03005944)
                        p->unk58 = 0;

                    while (gUnknown_03005910[p->unk58] != 0)
                    {
                        p->unk58++;
                        if (p->unk58 == gUnknown_03005944)
                            p->unk58 = 0;
                    }

                    p->unk5c = 0;
                    p->unk52 = gUnknown_03005950[p->unk58];
                    p->unk4c = 0;

                    for (i = 0; i < p->unk58; i++)
                        p->unk5c += gUnknown_03005948[i];

                    p->unk60 = 1;

                    LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
                    ApplyWindowFramePalette(gUnknown_03005958[p->unk58], 8);
                }

                PlayMusicOrSfx2(0x71);
            }
        }
        else if (gpKeySt->pressed & 2)
        {
            if (Proc_Find(gUnknown_086166A8) == 0)
                PlayMusicOrSfx2(0x66);

            if (p->unk64 != 0)
            {
                p->unk64--;
                p->unk66--;
                p->unk4c = 0;

                if (p->unk40 > 0)
                {
                    p->unk40 = 0;
                }
                else if (gUnknown_030059C0[p->unk64] != 0)
                {
                    gUnknown_03005910[gUnknown_0300599C[p->unk64]] = 0;
                    LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
                }
                else
                {
                    gUnknown_03005938[0] = p->unk52;
                    gUnknown_03005938[1] = p->unk58;
                    gUnknown_03005938[2] = p->unk5c;
                    gUnknown_03005950[p->unk58] = p->unk52;

                    p->unk58 = p->unk64;
                    p->unk5c = 0;
                    p->unk52 = gUnknown_03005950[p->unk58];
                    p->unk4c = 0;

                    for (i = 0; i < p->unk58; i++)
                        p->unk5c += gUnknown_03005948[i];

                    p->unk60 = -1;

                    LoadCoNameGraphic(gUnknown_030058E0[DivRem(p->unk52, gUnknown_03005948[p->unk58]) + p->unk5c], p->unk64 * 12 + 960);
                    ApplyWindowFramePalette(gUnknown_03005958[p->unk58], 8);
                }
            }
            else
            {
                p->unk40--;
                Proc_Start(gUnknown_086166A8, p);
                UnlockMainMenu();
            }
        }
        else if (gpKeySt->pressed & R_BUTTON)
        {
            CoSelect_AssignRemainingAndFinish(p);
        }
    }

    if (p->unk4e != 0 || p->unk60 != 0)
        p->unk68 = 0;
    else if (p->unk68 <= 3)
        p->unk68++;
    else
        p->unk48++;

    if (p->unk4e != 0)
    {
        if (p->unk4c <= 7)
        {
            p->unk34 = Interpolate(1, 0, 0x78, p->unk4c, 8);
            p->unk38 = p->unk4e * Interpolate(1, 0, 0xc, p->unk4c, 8);
            p->unk2c = p->unk4e * Interpolate(1, 0, 0xf, p->unk4c, 8);
        }
        else
        {
            p->unk34 = Interpolate(4, 0x78, 0, p->unk4c - 8, 8);
            p->unk38 = p->unk4e * Interpolate(4, 0xc, 0x18, p->unk4c - 8, 8);
            p->unk2c = p->unk4e * Interpolate(4, 0xf, 0x1d, p->unk4c - 8, 8);
        }

        sub_0807D860(p);

        if (p->unk4c == 0x10)
            p->unk4e = 0;
    }

    if (p->unk60 != 0)
    {
        if (p->unk4c <= 0xb)
        {
            p->unk34 = Interpolate(1, 0, 0x78, p->unk4c, 0xc);
            p->unk30 = p->unk60 * Interpolate(1, 0, 0x80, p->unk4c, 0xc);
        }
        else
        {
            p->unk34 = Interpolate(4, 0x78, 0, p->unk4c - 0xc, 0xc);
            p->unk30 = p->unk60 * Interpolate(4, 0x78, 0xff, p->unk4c - 0xc, 0xc);
        }

        sub_0807D918(p);

        if (p->unk4c == 0x18)
            p->unk60 = 0;
    }

    p->unk4c++;
    CoSelect_DrawSprites((struct Unk7DA98 *)p);
    p->unk3c--;
}

asm(".global sub_0807CE5C\n.thumb_set sub_0807CE5C, CoSelect_IDLE_0807CE5D\n");
