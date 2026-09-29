#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805B778.
 * sub_0805B778 @ 0x0805B778
 */

/* Wave 51, W51-E.  Closes wave 48's park (38.5% / +4 bytes) with no change to
 * the shape it had already settled: the residual was entirely the loop FORM.
 * As a `do { } while (...)` LICM hoists three invariants into the preheader --
 * `add rN,sp,#4`, the pooled 0x270F, and gUnknown_03003340's base -- which
 * costs one extra callee-saved register (r9) and, because the hoisted address
 * is re-emitted as an ordinary inline-pool reloc, DELETES gUnknown_03003340's
 * -fforce-addr .rodata word (0x0816D9CC).  Written as a goto loop there is no
 * NOTE_INSN_LOOP_BEG, LICM never runs, and all three come back at once.
 * This is wave 49's `LinkReceiveCommand` / `LinkReceiveBlock` finding applied to the
 * measurement the -fforce-addr chapter itself was still citing as open. */

void AiMoveTowardLandingCell(void)
{
    union Unk802C57CBuf v;
    void *p;

    gUnknown_030013EC(gUnknown_030040D8->unk02, gUnknown_030040D8->unk03,
                      gUnknown_030040D8->unk00, 0x78, 0);
    AiListLandingCells();
loop:
    p = gUnknown_03003F20;
    v.pos.unk00 = 0x270F;
    AiPopLastNearestCandidate(p, (u16 *)&v);
    if (v.pos.unk00 == 0x270F)
        AiFallbackMove();
    if ((s8)gUnknown_03003340[v.pos.unk02][v.pos.unk00]
        <= GetUnitMovementBudget((struct Unit *)gUnknown_030040D8))
        goto loop;
    AiAdvanceToward(&v);
}
asm(".global sub_0805B778\n.thumb_set sub_0805B778, AiMoveTowardLandingCell\n");
