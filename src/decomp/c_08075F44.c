#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075F44.
 * sub_08075F44 @ 0x08075F44
 */

/* MATCHED. NEEDS ITS POOL WORD PLACED: add "rodata": ["0x081CC584"] to this
 * function's data/promoted.json entry, then re-run tools/split_rodata.py and
 * tools/gen_lds.py before building. trymatch reports
 * `relocs: name different symbols that resolve to the same address` -- that
 * word is agbcc's own -fforce-addr copy of &gUnknown_0202FDFC, not a global.
 *
 * The one non-obvious spelling, and it was the whole difference from 98.3%:
 * the row stride is written as `<< 5`, NOT `* 32`. fold reorders a commutative
 * PLUS so that a MULT_EXPR operand comes first, so `x + y * 32` emits
 * `adds rD, rMUL, rX` while `x + (y << 5)` emits `adds rD, rX, rSHIFT`, which
 * is what the ROM has. Evaluation order is the same either way.
 *
 * gUnknown_0202FDFC is spelled honestly and referenced FOUR times, which is
 * what makes agbcc take the force-addr indirection (`ldr r2, =.LC0` once, then
 * `ldr r2, [r2]` again at the second group). The `(a + b) / 8` term is written
 * out TWICE rather than bound to a local: the ROM recomputes it after the
 * branch, which a single local cannot produce.
 *
 * The parameter stays `void *` (the header declares it that way and
 * c_0807614C.c passes its own proc type); the two +0x3c accesses are cast at
 * the use rather than bound to a typed local, because the local costs an extra
 * prologue copy in about half of these. */
#include "global.h"
#include "proc.h"
struct Unk8075F44
{
    /* 0x00 */ u8 filler_00[0x3c];
    /* 0x3c */ int unk3c;
};

void WorldMapNationPanel_Refresh(void *proc)
{
    u16 *base;
    int v;
    int t;
    u32 zero;

    if ((gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00) / 8 > 0x1F)
        base = (u16 *)gUnknown_08614280 + 0x3E0;
    else
        base = (u16 *)gUnknown_08614280;

    v = base[(gUnknown_0202FDFC.unk04 + gUnknown_0202FDFC.unk00) / 8
             + ((gUnknown_0202FDFC.unk06 + gUnknown_0202FDFC.unk02) / 8 << 5)] >> 12;

    if (v == ((struct Unk8075F44 *)proc)->unk3c)
        return;

    ((struct Unk8075F44 *)proc)->unk3c = v;

    zero = 0;
    CpuFastSet(&zero, gUnknown_08551A00 + 0x140, 0x01000040);

    t = gUnknown_081CC4F8[v];
    ApplyWindowFramePalette(t, 0);
    sub_080718F8(gUnknown_08551A04 + 0x140, gUnknown_081D22C4, 0x360);
    EnablePaletteSync();
    InitTextTileCache(0x340);
    PutTextScriptImmediate(3, 0xB, gUnknown_08551A00,
                 gTextTable[gUnknown_081CC578[t]], 0, 0);
    PutTilePoolGraphicTilemap(gUnknown_081CC538[v], gUnknown_08551A00 + 0x161);
}
asm(".global sub_08075F44\n.thumb_set sub_08075F44, WorldMapNationPanel_Refresh\n");
