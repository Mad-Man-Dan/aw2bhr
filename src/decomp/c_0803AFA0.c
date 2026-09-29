#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803AFA0.
 * sub_0803AFA0 @ 0x0803AFA0
 */

#include "hardware.h"
/* MATCHED wave 59 (W59-F), from W50-B's wave-50 draft at 77.9%.
 * PROMOTION NEEDS `"rodata": ["0x080910E4"]` in data/promoted.json, then
 * tools/split_rodata.py and tools/gen_lds.py -- trymatch's only remaining
 * note is that the ROM spells this unit's own gpKeySt force-addr word as the
 * data.s symbol gUnknown_080910E4. Same address, disassembly artefact.
 *
 * THE RESIDUAL W50-B COULD NOT REACH was the loop's `ldr r2,[r7]` -- the
 * candidate was ONE LEVEL TOO SHALLOW. 0x080910E0 is NOT a -fforce-addr pool
 * word holding &gUnknown_0809106C, as unknown-globals.h claimed for nine
 * waves; it is a REAL POINTER VARIABLE whose value is 0x0809106C. Four
 * spellings, measured:
 *   `gUnknown_0809106C[i]` (array)                    2 levels, too few
 *   `gUnknown_080910E0[i]` on a bare pointer global   4 levels, too many:
 *        -fforce-addr fires on the bare-symbol MEM and adds a .rodata word
 *   `tbl = gUnknown_0809106C; tbl[i]`                 2 levels, LICM hoists
 *   `tbl = &gUnknown_080910E0; (*tbl)[i]`             3 levels -- THE ROM
 * Binding the ADDRESS of the pointer variable to a local is what suppresses
 * force-addr: the address reaches the MEM through a register from the TEXT
 * pool, so no .rodata word is created, and the load itself is not hoisted
 * because the loop's `bl PutAsciiStringSprites` may clobber a non-const global.
 *
 * The last 4 bytes were WHERE the bind lands, and it is a three-way readout
 * of the preheader boundary:
 *   before the loop, as a statement -> `ldr r7` emits BEFORE `movs r4,#0`
 *   inside the loop body            -> LICM hoists it correctly, but the
 *                                      gpKeySt force-addr word is then
 *                                      created first and the pool order flips
 *   in the for-init, after `i = 0`  -> exact
 * So it is source, not an LICM hoist: it sits after the counter's init
 * because the comma operator puts it there.
 *
 * Everything else is byte-exact: register allocation (r5=p, r7/r6 the two pool
 * words, r4=i, NO r8 push), the switch tree, all six gpKeySt guards, and both
 * `(u16)(x * 8 + 8)` arguments. That last one must be written `x * 8 + 8` and
 * NOT `(x + 1) * 8`: shorten_binary_op folds the former into
 * `lsls #19; movs #0x80; lsls #12; adds; lsrs #16` exactly as the ROM has it,
 * while the latter shares the `x + 1` with the loop increment and drops the
 * u16 truncation entirely. Fixing that alone also freed r8 and removed the
 * extra push/pop pair. */

struct Unk0803AFA0
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ int unk08;
    /* 0x0c */ u8 filler_0c[0x14];
    /* 0x20 */ s16 unk20;
};

void DebugBackupUtility_Loop(struct Unk0803AFA0 *p)
{
    u8 i;
    u8 v;
    const char ***tbl;

    DebugScreenNoOp();
    PutAsciiStringSprites(0, 0, gUnknown_080910D4);
    for (i = 0, tbl = &gUnknown_080910E0; i <= 2; i++) {
        if (p->unk20 == i)
            v = GetClockPhase();
        else
            v = 0;
        PutAsciiStringSprites(v, i * 8 + 8, (*tbl)[i]);
    }
    PutAsciiStringSprites(0, p->unk20 * 8 + 8, gUnknown_08091064);
    switch (p->unk20) {
    case 0:
        if (gpKeySt->pressed & 1) {
            sub_0803AF84();
            return;
        }
        break;
    case 1:
        if ((gpKeySt->held & (R_BUTTON | L_BUTTON)) == 0x300 && (gpKeySt->pressed & 4)) {
            sub_0803AF78();
            return;
        }
        break;
    case 2:
        if ((gpKeySt->held & (R_BUTTON | L_BUTTON)) == 0x300 && (gpKeySt->pressed & 4)) {
            sub_0803AF90();
            ClearWorkRamAndSoftReset();
            return;
        }
        break;
    }
    if (gpKeySt->pressed & 2) {
        p->unk08 = 0;
        return;
    }
    if (gpKeySt->repeated & DPAD_UP) {
        if (p->unk20 > 0)
            p->unk20--;
    }
    if (gpKeySt->repeated & DPAD_DOWN) {
        if (p->unk20 <= 1)
            p->unk20++;
    }
}
asm(".global sub_0803AFA0\n.thumb_set sub_0803AFA0, DebugBackupUtility_Loop\n");
