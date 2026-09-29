#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806A2F8.
 * sub_0806A2F8 @ 0x0806A2F8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

struct Unk806A2F8
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ u32 unk2c;
};

/* The same ticket-taking opening as sub_0806A054, without the screen setup:
 * post-increment the counter, pass the OLD value, stash the result. */
void IntroT3_0806A2F9(struct Unk806A2F8 *proc)
{
    proc->unk2c = GetIntroSceneDuration(gUnknown_0202F204++);
    ResetWindowShadows();
}

asm(".global sub_0806A2F8\n.thumb_set sub_0806A2F8, IntroT3_0806A2F9\n");
