#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A8C0.
 * sub_0808A8C0 @ 0x0808A8C0
 */

#include "hardware.h"
/* Sibling of CoSelect_SetupBlend and CoSelectConfirm_BeginFadeToWhite: the same blend/BG shadow setup run,
 * ending in a `proc->unk4c = 0` rather than the window writes.
 *
 * The raw-view spelling here MUST be the `*(u16 *)&` cast, not `.raw`, and this
 * function is a fresh confirmation of the wave-21 rule: `gUnknown_03001FFC = 0`
 * puts a CSE-able zero in flight just before the first masked insert, and with
 * `.raw` agbcc parks it in a callee-saved register -- probed, and it comes back
 * as `push {r4, r5, r6, lr}` plus a stray `mov r5, #0` where the ROM has
 * `push {r4, r5, lr}`. The cast is byte-exact. */
struct Unk0808A8C0
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void CampaignIntroFrame_Init(struct Unk0808A8C0 *proc)
{
    gUnknown_030030E0.bits.effect = 1;

    gUnknown_03002020 = 0x10;
    gUnknown_03002B28 = 4;
    gUnknown_03001FFC = 0;

    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xFFE0) | 0x08;
    gUnknown_030030E0.bits.target1_enable_bd = 0;
    *(u16 *)&gUnknown_030030E0 = (*(u16 *)&gUnknown_030030E0 & 0xE0FF) | 0x1000;
    gUnknown_030030E0.bits.target2_enable_bd = 0;

    gUnknown_03002B6C.bits.priority = 0;
    gUnknown_03001FE8.bits.priority = 2;
    gUnknown_030030B4.bits.priority = 3;
    gUnknown_0300251C.bits.priority = 1;

    proc->unk4c = 0;
}
asm(".global sub_0808A8C0\n.thumb_set sub_0808A8C0, CampaignIntroFrame_Init\n");
