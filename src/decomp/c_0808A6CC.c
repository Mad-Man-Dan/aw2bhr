#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A6CC.
 * sub_0808A6CC @ 0x0808A6CC
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
#include "hardware.h"

/* Wave 53, W53-D. MATCHED.
 *
 * THIS FUNCTION RETYPED A PROTOTYPE. SetupMenuScreenBgs was declared `void
 * SetupMenuScreenBgs(void)` and promoted that way; it actually takes one argument.
 * The body never reads r0, so at every previously-matched call site the
 * argument was already in the right register and cost zero instructions --
 * the wave-51 arity rule exactly. CampaignIntro_Init is the differently-shaped
 * caller that exposes it: it SPILLS its proc to [sp] because the two
 * CpuFastSet loops use every callee-saved register including r8/sb/sl, and
 * then reloads it into r0 for nothing but the `bl` --
 *   `str r0,[sp]; movs r0,#9; bl SetSoundMixerChannelCount; ldr r0,[sp]; bl SetupMenuScreenBgs`
 * A `(void)` callee emits no such reload. include/unknown-functions.h,
 * src/decomp/c_0807898C.c, src/decomp/c_08080498.c and
 * src/decomp/c_08078E20.c were all updated; SetupMenuScreenBgs (356 B),
 * SuperCoPowerScene_StartNameStage (324 B) and ResultsScreen_Init (76 B) were each re-verified
 * byte-for-byte afterwards. Nothing in C calls ResultsScreen_Init, so gaining its
 * own forwarded ProcPtr parameter cascaded no further.
 *
 * The two loops are tile-index arithmetic, not pointer arithmetic: the
 * `(x & 0x3ff) * 0x20` form is the tile-to-VRAM-address idiom already in
 * c_08039A5C.c, c_08040430.c and c_08087B74.c, and writing it that way is
 * what produces the `lsl; and; lsl; add` group and the four separate givs
 * (8+4i, 0x40+8i, 0x28+4i, 0x44+8i) of the second loop. */
void CampaignIntro_Init(ProcPtr proc)
{
    int i;

    SetSoundMixerChannelCount(9);
    SetupMenuScreenBgs(proc);

    Decompress(gUnknown_0822FEF0,
        (void *)(gUnknown_0300251C.bits.chr_block * 0x4000 + 0x06000000));
    Decompress(gUnknown_0822F9AC, gBG3TilemapBuffer);
    BG_EnableSyncBG3();
    ApplyPaletteExt(gUnknown_0822FE50, 0x20, 0xa0);
    Proc_Start(gUnknown_086170D4, proc);
    Decompress(gUnknown_0823FFBC, gUnknown_0200FC50);

    for (i = 0; i < 8; i++)
        CpuFastSet(gUnknown_0200FC50 + ((i * 0x20) & 0x3ff) * 0x20,
                   (void *)(0x06010000 + ((i * 8) & 0x3ff) * 0x20), 0x40);

    for (i = 0; i < 8; i++)
    {
        CpuFastSet(gUnknown_0200FC50 + ((8 + i * 4) & 0x3ff) * 0x20,
                   (void *)(0x06010000 + ((0x40 + i * 8) & 0x3ff) * 0x20), 0x20);
        CpuFastSet(gUnknown_0200FC50 + ((0x28 + i * 4) & 0x3ff) * 0x20,
                   (void *)(0x06010000 + ((0x44 + i * 8) & 0x3ff) * 0x20), 0x20);
    }

    CpuFastSet(gUnknown_0200FED0, (void *)0x06010B00, 0x20);
    ApplyPaletteExt(gUnknown_08240AD4, 0x200, 0x20);
    Proc_Start(gUnknown_0861707C, proc);
    InitTextTileCache(0);
    ApplyPaletteExt(gUnknown_081320AC, 0x100, 0x20);
    BG_EnableSyncBG0();
}

asm(".global sub_0808A6CC\n.thumb_set sub_0808A6CC, CampaignIntro_Init\n");

extern bool8 IsMusicFadeActive(void);
extern void CampaignIntro_StartPrologueText(void);
extern void CampaignIntro_WaitForTextBoxes(void);
extern void CampaignIntro_WaitForButtonA(void);
extern void CampaignIntro_WaitForSkip(void);

struct ProcCmd CONST_DATA ProcScr_CampaignIntro[] =
{
    PROC_29(1),
    PROC_WHILE(IsMusicFadeActive),
    PROC_1D(30),
    PROC_YIELD,
    PROC_CALL(CampaignIntro_Init),
    PROC_1E(5),
    PROC_YIELD,
    PROC_1B(425),
    PROC_CALL(CampaignIntro_StartPrologueText),
    PROC_REPEAT(CampaignIntro_WaitForTextBoxes),
    PROC_SLEEP(90),
    PROC_REPEAT(CampaignIntro_WaitForButtonA),
    PROC_REPEAT(CampaignIntro_WaitForSkip),
    PROC_END,
};

asm(".global gUnknown_08616FD4\n.set gUnknown_08616FD4, ProcScr_CampaignIntro\n");
