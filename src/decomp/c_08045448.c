#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08045448.
 * sub_08045448 @ 0x08045448, sub_08045460 @ 0x08045460, sub_08045478 @ 0x08045478
 */

#include "proc.h"

struct Unk45448Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};
#include "proc.h"

struct Unk45460Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x54);
    /* 54 */ int unk54;
};
struct Unk45478
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
};

void CoPowerSenseiCopterCommand(ProcPtr parent)
{
    struct Unk45448Proc *proc = Proc_StartBlocking(gUnknown_084A09CC, parent);

    proc->unk54 = 1;
}
asm(".global sub_08045448\n.thumb_set sub_08045448, CoPowerSenseiCopterCommand\n");

void CoPowerSenseiAirborneAssault(ProcPtr parent)
{
    struct Unk45460Proc *proc = Proc_StartBlocking(gUnknown_084A09CC, parent);

    proc->unk54 = 2;
}
asm(".global sub_08045460\n.thumb_set sub_08045460, CoPowerSenseiAirborneAssault\n");

void CoPowerCreateUnits_Init(struct Unk45478 *p)
{
    p->unk2c = 0;
    p->unk30 = 0;
    StartCoPowerAnimation(gUnknown_030033EC);
}
asm(".global sub_08045478\n.thumb_set sub_08045478, CoPowerCreateUnits_Init\n");
