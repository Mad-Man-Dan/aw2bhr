#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808844C.
 * sub_0808844C @ 0x0808844C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* Matched wave 54 (W54-B), 2192 bytes, three attempts.

 * PROMOTION NEEDS A rodata ENTRY -- trymatch reports `relocs: name different
 * symbols that resolve to the same address` and that is the false mismatch, not
 * a real one. data/promoted.json must carry:
 *   "rodata": ["0x081D9458", "0x081D945C", "0x081D9460", "0x081D9464",
 *              "0x081D9468"]
 * then tools/split_rodata.py and tools/gen_lds.py before building. Those five
 * words are this function's own -fforce-addr pool holding &gpKeySt,
 * &gUnknown_030058E0, &gUnknown_03005948, &gUnknown_03005958 and
 * &gUnknown_03005964 -- see the note by the W51-I entry in
 * include/unknown-globals.h.

 * Two things that were not obvious from the assembly:
 *  - the ApplyPaletteExt argument CANNOT be written as pointer arithmetic. Four
 *    spellings were measured; all put the base's pool `ldr` one slot above the
 *    ROM's `lsls`, because C canonicalises `int + ptr` to `ptr + int` and
 *    force_operand then expands the SYMBOL_REF first. PAL_AT below is the only
 *    form that matches. New chapter in docs/agbcc-codegen.md.
 *  - `gUnknown_03005944 > 1` is a SIGNED compare in the ROM while the global is
 *    declared u32; the `(int)` casts are deliberate and are explained in the
 *    header rather than by retyping a global other matched functions read. */
#include "global.h"
#include "hardware.h"
#include "proc.h"
struct Unk0808844C
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ int unk38;
    /* 0x3c */ int unk3c;
    /* 0x40 */ u8 filler_40[0x08];
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
    /* 0x64 */ u16 unk64;
    /* 0x66 */ u8 filler_66[0x02];
    /* 0x68 */ s16 unk68;
};
struct Unk0808844CChild
{
    /* 0x00 */ u8 filler_00[0x64];
    /* 0x64 */ u16 unk64;
};
#define FRAME(t) gUnknown_030058E0[DivRem((t), gUnknown_03005948[proc->unk58]) + proc->unk5c]
/* The palette base has to be the RIGHT operand of an INTEGER add: written as
 * `&gUnknown_08234AF0[i]` (or any pointer-arithmetic spelling) C canonicalises
 * the pointer to the left, force_operand expands it first, and the pool `ldr`
 * lands one slot above the ROM's `lsls`. See the W54-B chapter in
 * docs/agbcc-codegen.md. */
#define PAL_AT(base, i) ((u16 *)((i) * 2 + (int)(base)))

void CoDesignEditor_Loop(struct Unk0808844C *proc)
{
    int i;

    if (Proc_Find(gUnknown_08616EDC) == NULL)
    {
        if (gUnknown_03005908 == 0)
        {
            if (proc->unk4e == 0 && proc->unk60 == 0)
            {
                if (gpKeySt->repeated & DPAD_LEFT)
                {
                    LoadCoFace(FRAME(proc->unk52), (void *)0x06014200, 0x16);
                    LoadCoFace(FRAME(proc->unk52 + 1), (void *)0x06014680, 0x17);
                    LoadCoFace(FRAME(proc->unk52 + 2), (void *)0x06014B00, 0x18);
                    proc->unk4c = 0;
                    proc->unk52 = (proc->unk52 != 0 ? proc->unk52 : gUnknown_03005948[proc->unk58]) - 1;
                    proc->unk4e = -1;
                    LoadCoFace(FRAME(proc->unk52), (void *)0x06013D80, 0x15);
                    LoadCoNameGraphic(FRAME(proc->unk52), 0x2CC);
                    PlayMusicOrSfx2(0x67);
                }
                else if (gpKeySt->repeated & DPAD_RIGHT)
                {
                    LoadCoFace(FRAME(proc->unk52), (void *)0x06014200, 0x16);
                    LoadCoFace(FRAME(proc->unk52 + 1), (void *)0x06014680, 0x17);
                    LoadCoFace(FRAME(proc->unk52 + 2), (void *)0x06014B00, 0x18);
                    proc->unk4c = 0;
                    if (proc->unk52 == gUnknown_03005948[proc->unk58] - 1)
                        proc->unk52 = 0;
                    else
                        proc->unk52 = proc->unk52 + 1;
                    proc->unk4e = 1;
                    LoadCoFace(FRAME(proc->unk52 + 2), (void *)0x06013D80, 0x15);
                    LoadCoNameGraphic(FRAME(proc->unk52), 0x2CC);
                    PlayMusicOrSfx2(0x67);
                }
                else if ((gpKeySt->repeated & DPAD_UP) && (int)gUnknown_03005944 > 1)
                {
                    gUnknown_03005978[proc->unk58] = proc->unk52;
                    if (proc->unk58 == 0)
                        proc->unk58 = gUnknown_03005944;
                    proc->unk58--;
                    proc->unk5c = 0;
                    proc->unk52 = gUnknown_03005978[proc->unk58];
                    proc->unk4c = 0;
                    for (i = 0; i < proc->unk58; i++)
                        proc->unk5c += gUnknown_03005948[i];
                    proc->unk60 = -1;
                    ApplyWindowFramePalette(gUnknown_03005958[proc->unk58], 8);
                    LoadCoNameGraphic(FRAME(proc->unk52), 0x2CC);
                    PlayMusicOrSfx2(0x67);
                }
                else if ((gpKeySt->repeated & DPAD_DOWN) && (int)gUnknown_03005944 > 1)
                {
                    gUnknown_03005978[proc->unk58] = proc->unk52;
                    proc->unk58++;
                    if (proc->unk58 == gUnknown_03005944)
                        proc->unk58 = 0;
                    proc->unk5c = 0;
                    proc->unk52 = gUnknown_03005978[proc->unk58];
                    proc->unk4c = 0;
                    for (i = 0; i < proc->unk58; i++)
                        proc->unk5c += gUnknown_03005948[i];
                    proc->unk60 = 1;
                    ApplyWindowFramePalette(gUnknown_03005958[proc->unk58], 8);
                    LoadCoNameGraphic(FRAME(proc->unk52), 0x2CC);
                    PlayMusicOrSfx2(0x67);
                }
                else if (gpKeySt->pressed & 1)
                {
                    gUnknown_03005908 = 3;
                    ApplyWindowFramePalette(gUnknown_03005958[proc->unk58], 1);
                    proc->unk4c = 0;
                    gUnknown_03005964 = GetLoadedCoPalette(FRAME(proc->unk52));
                    PlayMusicOrSfx2(0x71);
                }
                else if (gpKeySt->pressed & 2)
                {
                    gUnknown_03005908 = -1;
                    ((struct Unk0808844CChild *)Proc_Start(gUnknown_08616E64, PROC_TREE_3))->unk64 = proc->unk64;
                    PlayMusicOrSfx2(0x66);
                }
            }

            if (proc->unk4e != 0 || proc->unk60 != 0)
                proc->unk68 = 0;
            else if (proc->unk68 <= 3)
                proc->unk68++;
            else
                proc->unk48++;

            if (proc->unk4e != 0)
            {
                if (proc->unk4c <= 7)
                {
                    proc->unk34 = Interpolate(1, 0, 0x78, proc->unk4c, 8);
                    proc->unk38 = proc->unk4e * Interpolate(1, 0, 0xC, proc->unk4c, 8);
                    proc->unk2c = proc->unk4e * Interpolate(1, 0, 0xF, proc->unk4c, 8);
                }
                else
                {
                    proc->unk34 = Interpolate(4, 0x78, 0, proc->unk4c - 8, 8);
                    proc->unk38 = proc->unk4e * Interpolate(4, 0xC, 0x18, proc->unk4c - 8, 8);
                    proc->unk2c = proc->unk4e * Interpolate(4, 0xF, 0x1D, proc->unk4c - 8, 8);
                }

                CoDesignEditor_LoadCoGraphicsMidSlide((struct Unk08088CDC *)proc);

                if (proc->unk4c == 0x10)
                    proc->unk4e = 0;
            }

            if (proc->unk60 != 0)
            {
                if (proc->unk4c <= 0xB)
                {
                    proc->unk34 = Interpolate(1, 0, 0x78, proc->unk4c, 0xC);
                    proc->unk30 = proc->unk60 * Interpolate(1, 0, 0x80, proc->unk4c, 0xC);
                }
                else
                {
                    proc->unk34 = Interpolate(4, 0x78, 0, proc->unk4c - 0xC, 0xC);
                    proc->unk30 = proc->unk60 * Interpolate(4, 0x78, 0xFF, proc->unk4c - 0xC, 0xC);
                }

                CoDesignEditor_LoadCoGraphicsMidGroupSlide((struct Unk08088DA4 *)proc);

                if (proc->unk4c == 0x18)
                    proc->unk60 = 0;
            }
        }
        else if (gUnknown_03005908 == 1)
        {
            if (gpKeySt->repeated & DPAD_LEFT)
            {
                if (gUnknown_03005964 == 0)
                    gUnknown_03005964 = 7;
                else
                    gUnknown_03005964 = gUnknown_03005964 - 1;

                LoadCoPaletteVariant(FRAME(proc->unk52), 0x11, gUnknown_03005964);
                PlayMusicOrSfx2(0x64);
            }
            else if (gpKeySt->repeated & DPAD_RIGHT)
            {
                if (gUnknown_03005964 == 7)
                    gUnknown_03005964 = 0;
                else
                    gUnknown_03005964 = gUnknown_03005964 + 1;

                LoadCoPaletteVariant(FRAME(proc->unk52), 0x11, gUnknown_03005964);
                PlayMusicOrSfx2(0x64);
            }
            else if (gpKeySt->pressed & DPAD_UP)
            {
                if (gUnknown_03005964 > 3)
                {
                    gUnknown_03005964 = gUnknown_03005964 - 4;
                    LoadCoPaletteVariant(FRAME(proc->unk52), 0x11, gUnknown_03005964);
                    PlayMusicOrSfx2(0x64);
                }
            }
            else if (gpKeySt->pressed & DPAD_DOWN)
            {
                if (gUnknown_03005964 <= 3)
                {
                    gUnknown_03005964 = gUnknown_03005964 + 4;
                    LoadCoPaletteVariant(FRAME(proc->unk52), 0x11, gUnknown_03005964);
                    PlayMusicOrSfx2(0x64);
                }
            }
            else if (gpKeySt->pressed & 1)
            {
                gUnknown_03005908 = 5;
                proc->unk4c = 0;
                PlayMusicOrSfx2(0x71);
            }
            else if (gpKeySt->pressed & 2)
            {
                gUnknown_03005908 = 4;
                LoadCoPaletteVariant(FRAME(proc->unk52), 0x11, GetLoadedCoPalette(FRAME(proc->unk52)));
                proc->unk4c = 0;
                PlayMusicOrSfx2(0x66);
            }
        }
        else if (gUnknown_03005908 == 2)
        {
            if (gpKeySt->pressed & 1)
            {
                proc->unk64 = 1;
                PlayMusicOrSfx2(0x71);
                SetLoadedCoPalette(FRAME(proc->unk52), gUnknown_03005964);
                LoadCoFace(FRAME(proc->unk52), (void *)0x06013000, 0x12);
                Proc_Start(gUnknown_08616EDC, proc);
            }
            else if (gpKeySt->pressed & 2)
            {
                gUnknown_03005908 = 6;
                proc->unk4c = 0;
                PlayMusicOrSfx2(0x66);
            }
        }
    }

    proc->unk4c++;

    if (gUnknown_03005908 == -1 || gUnknown_03005908 == 0)
        CoDesignEditor_DrawBrowse(proc);
    else if (gUnknown_03005908 == 1)
        CoDesignEditor_DrawPicker(proc);
    else if (gUnknown_03005908 == 2 || gUnknown_03005908 == -2)
        CoDesignEditor_DrawConfirm((struct Unk080895E4Proc *)proc);
    else if (gUnknown_03005908 == 3)
        CoDesignEditor_DrawEnterPicker((struct Unk080897C8 *)proc);
    else if (gUnknown_03005908 == 4)
        CoDesignEditor_DrawLeavePicker((struct Unk08089A04 *)proc);
    else if (gUnknown_03005908 == 5)
        CoDesignEditor_DrawPickerToConfirm((struct Unk08089C14 *)proc);
    else if (gUnknown_03005908 == 6)
        CoDesignEditor_DrawConfirmToPicker((struct Unk08089F90 *)proc);

    if (gUnknown_03005908 != -1 && gUnknown_03005908 != 0)
        CoDesignEditor_DrawCoBody((struct Unk8A2F4Proc *)proc);

    proc->unk3c--;

    if (gUnknown_03005908 != -1)
    {
        ApplyPaletteExt(PAL_AT(gUnknown_08234AF0, DivRem(Div((u16)proc->unk3c, 4), 0x10)), 0x218, 2);
        ApplyPaletteExt(PAL_AT(gUnknown_08239F84, DivRem(Div((u16)proc->unk3c, 4), 0x10)), 0x398, 2);
    }
}

asm(".global sub_0808844C\n.thumb_set sub_0808844C, CoDesignEditor_Loop\n");
