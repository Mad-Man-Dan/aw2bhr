#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806C154.
 * sub_0806C154 @ 0x0806C154
 */

#include "proc.h"
#include "hardware.h"
/* The tick half of the same blend family: it drives the two BLDALPHA
 * coefficient shadows off a counter at +0x58 and draws one sprite, then hands
 * over to SetDefaultColorEffects (the wholesale blend clear) when the counter runs out.
 * `asrs` on the counter is what makes it signed. */

struct Unk806C154
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
    /* 0x30 */ u8 filler_30[0x24];
    /* 0x54 */ u32 unk54;
    /* 0x58 */ int unk58;
};

void CreditsResultSprite_FadeInLoop(struct Unk806C154 *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = proc->unk58 >> 1;
    gUnknown_03002B28 = 0x10 - (proc->unk58 >> 1);
    gUnknown_03001FFC = gUnknown_03001FFC;

    PutSpriteExt(1, proc->unk2c & 0x1FF, 0x400, (u16 *)proc->unk54, 0);

    if (proc->unk58 > 0x1F)
    {
        SetDefaultColorEffects();
        Proc_Break(proc);
    }
    else
    {
        proc->unk58++;
    }
}
asm(".global sub_0806C154\n.thumb_set sub_0806C154, CreditsResultSprite_FadeInLoop\n");
