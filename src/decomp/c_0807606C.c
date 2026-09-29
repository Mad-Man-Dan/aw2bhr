#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807606C.
 * sub_0807606C @ 0x0807606C
 */

/* The `if`/`else` spelling is load-bearing. `int n = 0x11; if (x < 0) n = 0;`
 * lets agbcc's scheduler hoist the `movs r2, #0x11` ahead of the field load,
 * and a ternary either way inverts which arm is preset -- only the two-armed
 * `if`/`else` puts the constant between the sign-extend and the `cmp`, where
 * the ROM has it.
 *
 * `gBG0TilemapBuffer + (n + 0x200)` with the constant bound to the INDEX, not
 * to the pointer: `p + n + 0x200` parses as `(p + n) + 0x200` and adds 0x400 to
 * the pointer after the scaling, where the ROM folds it into the offset
 * (`lsls #1` then `+ 0x400`) before the pointer is even loaded. */
struct Unk807606C
{
    /* 0x00 */ u8 filler_00[0x3a];
    /* 0x3a */ s8 unk3a;
};

void WorldMapNationPanel_Draw(struct Unk807606C *proc)
{
    int n;

    if (proc->unk3a < 0)
        n = 0;
    else
        n = 0x11;

    sub_08071900(gUnknown_08551A00 + 0x140, gBG0TilemapBuffer + (n + 0x200), 0xd, 4);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
}
asm(".global sub_0807606C\n.thumb_set sub_0807606C, WorldMapNationPanel_Draw\n");
