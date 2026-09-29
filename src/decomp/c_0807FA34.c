#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807FA34.
 * sub_0807FA34 @ 0x0807FA34
 */

#include "hardware.h"
#include "proc.h"
struct Unk0807FA34
{
    /* 0x00 */ u8 filler_00[0x4c];
    /* 0x4c */ u16 unk4c;
};

void CoPowerScene_Init(struct Unk0807FA34 *proc)
{
    LoadCoFullBodyAndPalette(gUnknown_03005970, 0x1CA, 4);
    ApplyPalettes(gUnknown_08239DE4, 0x13, 1);
    proc->unk4c = 0;
    Proc_Start(gUnknown_086167EC, proc);
    Proc_Start(gUnknown_086167BC, proc);
    CoPowerScene_PlayMusic();
}
asm(".global sub_0807FA34\n.thumb_set sub_0807FA34, CoPowerScene_Init\n");
