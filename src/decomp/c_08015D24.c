#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015D24.
 * sub_08015D24 @ 0x08015D24
 */

/* MATCHED -- `relocs: name different symbols that resolve to the same address`.
 * PROMOTION MUST CARRY THE POOL WORD:  "rodata": ["0x0808E534"].
 *
 * A gUnknown_0848A160 opcode handler: if any slot's unk00 already equals the
 * word the cursor points at, stop (return FALSE); otherwise step the cursor by
 * one 8-byte record and keep going.
 *
 * gUnknown_0808E534 is NOT a global. It is agbcc's own -fforce-addr .rodata
 * address-constant slot holding &gUnknown_03001470 -- the word at 0x0808E534 in
 * baserom.gba is 0x03001470, and 0x0808E538 holds the same address again for
 * the next function, which is the `.LC` block signature. Naming the array
 * directly is what produces it, exactly as c_0804E8F0.c records for
 * gUnknown_08136090; the `ldr r6,=...; ldr r5,[r6]` pair is the pool word's
 * address followed by its contents, and the second `ldr r0,[r6]` after the loop
 * is force-addr re-reading it rather than a second global. Noted at the
 * gUnknown_03001470 extern in include/unknown-globals.h.
 *
 * THE COMPARISON VALUE IS INSIDE THE LOOP, not hoisted by hand. The ROM's
 * preheader is `movs r3,#0` (the source `for` init) and only THEN the address
 * load and the `*cursor` read -- i.e. the read is an LICM hoist sitting AFTER a
 * source statement, which is the giveaway. Writing `v = *cursor;` as its own
 * statement ahead of the loop puts it before the `movs r3,#0`, loses the CSE of
 * `a * 2` across the loop (the ROM keeps it in r2 and reuses it in the
 * epilogue) and drops from `push {r4,r5,r6,lr}` to `push {r4,r5,lr}`.
 */
bool8 SlotOp_WaitForScriptEnd(u8 a)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (gUnknown_03001470[i].unk00
            == *(const u32 *)gUnknown_03001470[a].unk04)
            return FALSE;
    }

    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return TRUE;
}
asm(".global sub_08015D24\n.thumb_set sub_08015D24, SlotOp_WaitForScriptEnd\n");
