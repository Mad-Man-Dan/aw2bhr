#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015E80.
 * sub_08015E80 @ 0x08015E80
 */

/* Family F067: one command of the gUnknown_03001470 script interpreter.
 * `.unk04` is the slot's instruction cursor; the command is 8 bytes wide and
 * its first word is the operand, so the body is "run the operand, step over
 * the command, report that the slot is still alive" (`movs r0,#1` under the
 * `pop {r1}` value epilogue). Only the callee varies across the family's three
 * members -- `varies` lists index 11 and nothing else, so the operand's own
 * spelling is fixed by the exemplar and is not a per-member decision.
 *
 * NAMING THE MEMBER TWICE IS LOAD-BEARING, and it is the opposite of the
 * `p[X + C]` workaround in docs/agbcc-codegen.md. Binding `&g[i].unk04` to a
 * pointer local -- the shape that rule recommends -- makes agbcc fold the +4
 * into the RELOCATION and emit `.word gUnknown_03001470+0x4`, which is not the
 * ROM's clean pool word. Writing `g[i].unk04` in both statements is what
 * produces `ldr r1,=gUnknown_03001470; adds r1,#4; adds r4,r4,r1`: the address
 * is CSEd across the two statements as a value, so the constant has to be
 * added at run time. (Binding `&g[i]` instead is the third outcome, and gives
 * the `ldr r0,[r4,#0x4]` displacement form.) All four spellings probed. */

bool8 SlotOp_StartEventScript(u8 i)
{
    StartEventScript(*(const u8 **)gUnknown_03001470[i].unk04);
    gUnknown_03001470[i].unk04 = (const u8 *)gUnknown_03001470[i].unk04 + 8;
    return 1;
}
asm(".global sub_08015E80\n.thumb_set sub_08015E80, SlotOp_StartEventScript\n");
