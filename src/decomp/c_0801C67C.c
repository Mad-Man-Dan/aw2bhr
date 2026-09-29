#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801C67C.
 * sub_0801C67C @ 0x0801C67C
 */

/* Runs one AP_ExecFrame step with +0x18 zeroed and +0x1a forced to 0x100,
 * restoring +0x1a afterwards.
 *
 * THE ZERO IS A SEPARATE STATEMENT, and that is the whole difficulty. The ROM
 * order is `movs r0,#0` / `ldrh r5,[r4,#0x1a]` / `strh r0,[r4,#0x18]` -- the
 * constant materialised, THEN the load, THEN the store. Neither natural
 * two-statement spelling produces it: `saved = ...; a1->unk18 = 0;` emits
 * ldrh/movs/strh and `a1->unk18 = 0; saved = ...;` emits movs/strh/ldrh, both
 * measured with compile_probe. agbcc runs no post-reload scheduler here, so
 * the insn order IS the source order, and the only source that puts the
 * constant's own insn ahead of the load is one where the stored value is a
 * variable assigned on its own line. */
void AP_ExecDummyFrame(struct Unk0801C210 *a1)
{
    u16 saved;
    u16 zero;

    zero = 0;
    saved = a1->unk1a;
    a1->unk18 = zero;
    a1->unk1a = 0x100;
    AP_ExecFrame(a1);
    a1->unk1a = saved;
}
asm(".global sub_0801C67C\n.thumb_set sub_0801C67C, AP_ExecDummyFrame\n");
