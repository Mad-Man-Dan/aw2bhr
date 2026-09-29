#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C52C.
 * sub_0806C52C @ 0x0806C52C
 */

#include "hardware.h"
#include "proc.h"
struct Unk0806C52CProc
{
    /* 0x00 */ u8 filler_00[0x38];
    /* 0x38 */ int unk38;
};

/* The same screen-setup shape as CreditsStaff_Init / SoundRoomMusicPage_Init -- one
 * SetDispEnable, four BgCnt priorities, then palettes and graphics -- but with
 * the graphics set chosen by a single `if`. The two arms are the same five calls
 * over ten different symbols.
 *
 * Two readings that are not guesses. `movs r1, #3; rsbs; ands; subs r1, #2;
 * ands; subs r1, #4; ands` is three consecutive 1-bit CLEARS on gDispIo's byte
 * 1, sharing their masks by `subs` from the second on (~0x02, ~0x04, ~0x08) --
 * the bitfield-clear signature -- so this is SetDispEnable(1, 0, 0, 0, 1) and
 * not a mask. And the else arm stores `r5`, the value IsHardCampaignMode returned,
 * rather than materialising a zero: on that path cse's record_jump_equiv knows
 * r5 == 0 from the `cmp r5, #0; beq`, so the plain `proc->unk38 = 0` is what
 * produces it and nothing has to be spelled with the variable.
 */
void CreditsResult_Init(struct Unk0806C52CProc *proc)
{
    int v;

    Proc_EndEach(gUnknown_085819D4);
    ResetWindowShadows();
    SetDefaultColorEffects();
    SetDispEnable(1, 0, 0, 0, 1);
    gUnknown_03002B6C.bits.color_depth = 1;
    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 1;
    gUnknown_030030B4.bits.priority = 2;
    gUnknown_0300251C.bits.priority = 3;

    v = IsHardCampaignMode();

    if (v != 0)
    {
        ApplyPalettes(gUnknown_081A21B4, 0, 16);
        ApplyPalette(gUnknown_081A3D64, 16);
        Decompress(gUnknown_0819C454, (void *)0x06000000);
        Decompress(gUnknown_081A1C50, (void *)0x0600D800);
        Decompress(gUnknown_081A31C4, (void *)0x06010000);
        proc->unk38 = 1;
    }
    else
    {
        ApplyPalettes(gUnknown_0819C254, 0, 16);
        ApplyPalette(gUnknown_081A3D44, 16);
        Decompress(gUnknown_08195318, (void *)0x06000000);
        Decompress(gUnknown_0819BCF0, (void *)0x0600D800);
        Decompress(gUnknown_081A2A04, (void *)0x06010000);
        proc->unk38 = 0;
    }

    SetBgScrollShadow(0, 0, 0);
    SetSoundMixerChannelCount8();
}
asm(".global sub_0806C52C\n.thumb_set sub_0806C52C, CreditsResult_Init\n");
