#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0808A978.
 * sub_0808A978 @ 0x0808A978
 */

#include "hardware.h"
#include "proc.h"
/* MATCHED (252 bytes). Promotion must carry "rodata": ["0x081D9480"].
 *
 * The angle is a u16 counter at +0x4C stepped once per frame; SIN_Q12 /
 * COS_Q12 mask it with 0xFF, which on the low byte of that halfword folds to a
 * single `ldrb`. The four Div results are re-narrowed at the call because
 * SetObjAffine takes s16, not because the source casts.
 *
 * The loop's OAM blob is reached through agbcc's own -fforce-addr word at
 * 0x081D9480 (ROM contents 0x0848B6BE); naming gUnknown_0848B6BE directly is
 * what reproduces the in-loop re-load. gUnknown_0848B6A0 is used once and gets
 * a plain pool word instead, which is the single-use / multi-use split the
 * -fforce-addr chapter describes. */

struct Unk8A978Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x4c);
    /* 4C */ u16 unk4C;
};

void CampaignIntroFrame_Loop(struct Unk8A978Proc *proc)
{
    int i;

    SetObjAffine(0,
        Div(COS_Q12(proc->unk4C) * 16, 0x100),
        Div(-SIN_Q12(proc->unk4C) * 16, 0x100),
        Div(SIN_Q12(proc->unk4C) * 16, 0x100),
        Div(COS_Q12(proc->unk4C) * 16, 0x100));

    PutSpriteExt(2, 0, 0x100, gUnknown_0848B6A0, 0x800);
    PutSprite(0, 0x18, 8, gUnknown_08615C62, 0x40);

    for (i = 0; i < 8; i++)
        PutSpriteExt(1, i * 32, 0x18, gUnknown_0848B6BE, 0x58);

    proc->unk4C++;
}
asm(".global sub_0808A978\n.thumb_set sub_0808A978, CampaignIntroFrame_Loop\n");
