#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803F43C.
 * sub_0803F43C @ 0x0803F43C
 */

#include "proc.h"
struct UnkF43CProc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x30);
    /* 30 */ int unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x50);
    /* 50 */ struct Unk0801C210 *unk50;
    /* 54 */ int unk54;
    /* 58 */ int unk58;
};

void VolcanoRock_Fall(struct UnkF43CProc *proc)
{
    AP_Update(proc->unk50,
                 (proc->unk54 - gMap->scrollX) & 0x1FF,
                 (proc->unk58 - gMap->scrollY) & 0xFF);
    proc->unk58 += 0xA;
    if (proc->unk30 * 16 + 0x10 < proc->unk58)
    {
        struct Unk0801C210 *sprite;

        AP_Delete(proc->unk50);
        proc->unk58 = proc->unk30 * 16 + 8;
        sprite = AP_Create(gUnknown_081171EC, 1, 1);
        proc->unk50 = sprite;
        sprite->unk22 = 0x51CA;
        AP_SwitchAnimation(proc->unk50, 1);
        PlayMusicOrSfx2(0x1D4);
        StartScreenShake(0, 8, 0);
        Proc_Break(proc);
    }
}
asm(".global sub_0803F43C\n.thumb_set sub_0803F43C, VolcanoRock_Fall\n");
