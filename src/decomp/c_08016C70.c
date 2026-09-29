#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016C70.
 * sub_08016C70 @ 0x08016C70, sub_08016C9C @ 0x08016C9C, sub_08016CD8 @ 0x08016CD8, sub_08016CEC @ 0x08016CEC, sub_08016D04 @ 0x08016D04
 */

void DeleteSuspendSave(u8 a)
{
    if (a != 0) {
        MarkProfileSaved();
        EraseSaveSlot(a);
        if (gPlaySt.gameMode == 1)
            BackupBattleMapPoints();
    }
}
asm(".global sub_08016C70\n.thumb_set sub_08016C70, DeleteSuspendSave\n");

/* Returns the address of one of three byte flags in gUnknown_0200C420.
 *
 * The pool words the assembly calls gUnknown_0808E540/_0808E544/_0808E548 are
 * NOT objects: dereferenced in baserom.gba they hold 0x0200C429, 0x0200C42A and
 * 0x0200C42B, i.e. &gUnknown_0200C420.unk09/.unk0a/.unk0b. They are agbcc's own
 * -fforce-addr address constants, so the honest spelling reproduces both loads
 * and split_rodata carves the duplicate out. Nothing else in the tree
 * references those three ROM addresses and aw2bhr.lds binds no symbol there.
 *
 * A `switch` (repeated `cmp r1,#3` with `beq` then `bgt`) with NO default: on
 * any other value r0 is never written, so the incoming argument falls straight
 * out through the shared `bx lr`. */
u8 *GetSuspendFlagPtr(s8 a)
{
    switch (a) {
    case 2:
        return &gUnknown_0200C420.unk09;
    case 3:
        return &gUnknown_0200C420.unk0a;
    case 4:
        return &gUnknown_0200C420.unk0b;
    }
}
asm(".global sub_08016C9C\n.thumb_set sub_08016C9C, GetSuspendFlagPtr\n");

/* The `ldrb` plus `lsls #0x18; asrs #0x18` is the RETURN being narrowed to s8,
 * not the load: a `s8 *` return from GetSuspendFlagPtr would have given a single
 * `ldrsb`. */
s8 GetSuspendFlag(s8 a)
{
    return *GetSuspendFlagPtr(a);
}
asm(".global sub_08016CD8\n.thumb_set sub_08016CD8, GetSuspendFlag\n");

/* GetSuspendFlag's setter. The second parameter's copy-then-narrow
 * (`adds r4,r1,#0; lsls #0x18; lsrs #0x18`) is NOT the int-with-a-cast shape
 * here -- it is a declared-narrow parameter whose pseudo has to live across the
 * `bl`, the case src/decomp/c_080154C4.c measured. */
void SetSuspendFlag(s8 a, u8 b)
{
    *GetSuspendFlagPtr(a) = b;
}
asm(".global sub_08016CEC\n.thumb_set sub_08016CEC, SetSuspendFlag\n");

/* The gPlaySt.gameMode mode -> id map every caller uses
 * (src/decomp/c_0802C1D0.c, c_0802CF48.c, c_08038548.c, c_08045770.c). A
 * `switch`: the repeated `cmp r0,#2` with `beq` then `bgt` is gcc's decision
 * tree. `s8` return and `u8` parameter were already settled by those callers;
 * see include/unknown-functions.h. */
s8 GetSuspendIdForGameMode(u8 a)
{
    switch (a) {
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
        return 4;
    }
    return -1;
}
asm(".global sub_08016D04\n.thumb_set sub_08016D04, GetSuspendIdForGameMode\n");
