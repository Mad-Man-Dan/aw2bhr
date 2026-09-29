#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08069B6C.
 * sub_08069B6C @ 0x08069B6C
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* sub_080688A8 with one extra call. Diffed against it rather than assumed: the
 * arming line is identical, and the trailing SetDefaultColorEffects is the only
 * difference. Its result is discarded and the epilogue is pop {r0}; bx r0, so
 * this is void and not a tail forward. */
struct Unk69B6CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2C */ int unk2c;
};

void IntroT3_08069B6D(struct Unk69B6CProc *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    SetDefaultColorEffects();
}

asm(".global sub_08069B6C\n.thumb_set sub_08069B6C, IntroT3_08069B6D\n");
