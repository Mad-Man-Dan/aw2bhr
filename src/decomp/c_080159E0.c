#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080159E0.
 * sub_080159E0 @ 0x080159E0
 */

/* Runs one gUnknown_03001470 slot's pending-wake path: if flag bit 0 is set,
 * clear it, point gUnknown_03001FBC at the slot, call the slot's +0x08
 * callback if it has one, and step the script.
 *
 * The mask is the literal 0xfffe and not `~1`: `~1` is -2, which agbcc
 * materialises as `movs r0,#2; rsbs r0,r0,#0` rather than the pool word the
 * ROM has.
 *
 * ->unk08 is reached as `(base + 8) + index*0x60` rather than off the `&g[a]`
 * register that is already live -- the offset-hoisted-onto-the-base shape the
 * flat `g[i].m` spelling produces, which the struct's own note in
 * unknown-globals.h records for the same members. Binding `&g[a]` to a local
 * pointer folds the 8 into the load displacement instead.
 */
void RunSlotScriptFrame(u8 a)
{
    if (gUnknown_03001470[a].unk12 & 1)
    {
        gUnknown_03001FBC = a;
        gUnknown_03001470[a].unk12 &= 0xfffe;

        if (gUnknown_03001470[a].unk08 != 0)
            ((void (*)(struct Unk03001470 *))gUnknown_03001470[a].unk08)(&gUnknown_03001470[a]);

        StepSlotScript(a);
    }
}
asm(".global sub_080159E0\n.thumb_set sub_080159E0, RunSlotScriptFrame\n");
