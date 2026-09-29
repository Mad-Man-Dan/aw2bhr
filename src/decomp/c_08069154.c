#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069154.
 * sub_08069154 @ 0x08069154
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

#include "proc.h"
/* Takes a ticket and hands it to the same GetIntroSceneDuration slot sub_08068AC4 and
 * sub_08069864 use (+0x2c), loads one blob, then starts a ten-argument
 * StartIntroBgAffineTween under itself. The `adds r0, #0x40` in the stack-argument setup
 * is 0xc0 being reused to build 0x100 -- one CSEd constant chain, not two
 * different values. */
struct Unk69154Proc
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
};

void IntroT3_08069155(struct Unk69154Proc *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    Decompress(gUnknown_08183A00, gBG2TilemapBuffer);
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    StartIntroBgAffineTween(1, -1, 0x170, 0x88, -0x3800, 0, 0xc0, 0x100, 0xc, proc);
}

asm(".global sub_08069154\n.thumb_set sub_08069154, IntroT3_08069155\n");
