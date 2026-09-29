#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804189C.
 * sub_0804189C @ 0x0804189C
 */

#include "proc.h"
/* The tail StartAttackOnInventionAt falls into when FindLivingInventionTargetAt finds no entry: it decays
 * the cell plane's own counter at (x, y) instead of an entry's unk04, then
 * starts the same 0x0849FE78 proc with unk4c NULL and the two coordinates
 * stored.
 *
 * Both plane addresses go through a struct laid over gUnknown_08499590 -- the
 * ROM computes `(map + 0x417A) + y * 2`, and only a COMPONENT_REF preserves
 * that association; the `u16 *` cast reassociates it to `(map + y * 2) +
 * 0x417A`. Same spelling that closed StartPipeSeamHit and DestroyLaserOrMinicannon.
 *
 * gUnknown_08499590 and gUnknown_020288B4 are both named HONESTLY here even
 * though the ROM reaches them through agbcc's own -fforce-addr words at
 * 0x0809133C and 0x08091338 (whose ROM contents are 0x08499590 and 0x020288B4).
 * The build places this unit's .rodata pool word for each, which is the wave-18
 * rule. StartPipeSeamHit names gUnknown_020288B4 with a clean pool word instead,
 * which is what proves it a real global rather than a pool artefact.
 *
 * The cell value goes through an `int` local, so the subtraction keeps the
 * CSEd `ldrsh` -- unlike the sibling StartAttackOnInventionAt, whose `ent->unk04 -= ...`
 * on a u8 lvalue makes shorten_binary_op narrow the same load to `ldrb`.
 * The address is expanded TWICE, once for the read and once for the store,
 * because the source names the cell expression twice with the diamond between
 * them; agbcc reloads both pointer levels rather than CSEing across the merge.
 *
 * `lim` must be bound AFTER the cell read: the ROM emits `ldrb r3,[r0]` before
 * `ldr r0,=gUnknown_030013D0`, and the binding is needed at all because
 * gUnknown_030013D0 is declared `u8 []`, so an inline `*(s16 *)(g + 0x14)`
 * folds the offset into the relocation and loses the clean pool word. */
struct Unk4189CProc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ int unk2c;
    /* 30 */ int unk30;
    /* 34 */ u8 filler_34[0x18];
    /* 4c */ void *unk4c;
    /* 50 */ u8 filler_50[0x04];
    /* 54 */ void *unk54;
    /* 58 */ u8 filler_58[0x0c];
    /* 64 */ s16 unk64;
};

void StartAttackOnPipeSeamAt(int a1, int a2, int a3)
{
    u8 c = a3;
    struct Unk4189CProc *proc;
    u8 *st;
    u8 *lim;
    int v;

    st = gUnknown_030044B0;
    *(u32 *)(st + 8) = gUnknown_03001FD4;

    CalcBattleDamageVsStructure(gUnknown_03003F38);

    v = gUnknown_020288B4[
        gMap->rowOffset[a2] + a1];

    lim = (u8 *)gBattleAttacker;

    if (v < *(s16 *)(lim + 0x14))
        v = 0;
    else
        v -= *(s16 *)(lim + 0x14);

    gUnknown_020288B4[
        gMap->rowOffset[a2] + a1] = v;

    ResetDisplayEffects();

    proc = Proc_Start(gUnknown_0849FE78, PROC_TREE_3);
    proc->unk2c = a1;
    proc->unk30 = a2;
    proc->unk54 = gUnknown_030040D8;
    proc->unk4c = NULL;
    proc->unk64 = c;
}
asm(".global sub_0804189C\n.thumb_set sub_0804189C, StartAttackOnPipeSeamAt\n");
