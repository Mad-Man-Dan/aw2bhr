#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08061E98.
 * sub_08061E98 @ 0x08061E98, AiScanBuildableFacilities @ 0x08061F34
 */

/* Runs one handler over every unit record of the current army window.
 *
 * `fns` is a LOCAL array: agbcc parks the three function pointers in this
 * unit's own .rodata and copies them to the stack with one `ldm`/`stm` pair,
 * which is where the ROM's 12-byte blob at 0x0816DB10 comes from -- it reads
 * 0x08061DCD / 0x08061E55 / 0x08061E81 out of baserom.gba, THUMB bit set. The
 * element type is left UNPROTOTYPED because the three handlers are declared in
 * include/unknown-functions.h with three different incomplete tags: each
 * promoted definition reads the unit record through its own bitfield view, and
 * in C89 `void (*)()` is compatible with all of them, so this needs no cast and
 * no promoted file has to be retyped.
 *
 * The dispatch is `bl _call_via_r1` -- ONE argument, per the register-index
 * rule -- and the `lsls #0x1d; lsrs #0x1b` on the selector is the three-bit
 * bitfield read already scaled by 4 for the word index, not a mask plus a
 * shift.
 *
 * The unit record is reached through a file-local bitfield view cast onto the
 * shared symbol: struct Unit declares +0x09 as a plain byte, and only a
 * real bitfield produces the ROM's SImode `movs #8; rsbs; ands` clear. Its
 * filler is padded to the shared struct's 12-byte stride, which is what the
 * loop's `adds r5, #0xc` giv confirms.
 *
 * gUnknown_085766E0 is subscripted with a 12-byte stride here as well, which
 * its own declared extent cannot carry -- see the CONFLICT note beside it in
 * include/unknown-globals.h -- so the `&g[type].unk04` address is formed as
 * byte arithmetic instead. The parenthesisation matters: the ROM builds the
 * whole offset before adding the base.
 *
 * gUnknown_03003F2C is named honestly and referenced from two basic blocks, so
 * -fforce-addr gives it a private .rodata word and the three-level chain --
 * that word is the ROM's gUnknown_0816DB1C, which holds 0x03003F2C. */
struct Unk61E98Unit
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[8];
    /* 0x09 */ u8 unk09_0 : 3;
               u8 unk09_3 : 5;
    /* 0x0a */ u8 filler_0a[2];
};
/* Sweeps every cell of the gUnknown_08499590 map and accumulates two byte
 * masks: gUnknown_030045C0 over the cells IsCellOwnedByArmyTeam rejects, and
 * gUnknown_030046B8 over every cell. `v` is the five-entry bit-mask table
 * copied off its ROM template with the repo's standard
 * `sub_0808B6E8(buf, <rom blob>, N)` stack-copy idiom.
 *
 * A C array INITIALISER (`u8 v[5] = {0,0,1,2,4};`) is byte-for-byte identical
 * here -- agbcc parks the same five bytes in this unit's .rodata and calls a
 * memcpy with the same three arguments -- but it emits `bl memcpy`, and the
 * ROM's call relocates against the game's own sub_0808B6E8. That is the only
 * thing that distinguishes the two spellings, and it is what says the original
 * wrote the copy out by hand against a named ROM template.
 *
 * The map MUST be reached through a struct laid over the header rather than by
 * byte arithmetic on the `u8 *` symbol -- the ROM computes `(map + 0x417A) +
 * y * 2` and `(map + 0x1432) + idx`, and only a COMPONENT_REF keeps that
 * association (the gUnknown_08499590 note in include/unknown-globals.h).
 *
 * The two clears are ONE chained assignment: the ROM loads both pool addresses
 * before materialising the 0, which is the operand-class grouping of a single
 * statement. Written as two statements agbcc interleaves the ldr/strb pairs. */
void AiUpdateUnitModes(void)
{
    void (*fns[3])() = {sub_08061DCC, AiUpdateModeResupply, AiUpdateModeRepair};
    struct Unk61E98Unit *p;
    int i;

    for (i = gUnknown_03003F2C; i < gUnknown_03003F2C + 0x40; i++)
    {
        p = &((struct Unk61E98Unit *)gUnits)[i];
        if (p->unk00 != 0)
        {
            gUnknown_03004784 = (u8 *)gUnknown_085766E0 + (p->unk00 * 12 + 4);
            if (p->unk09_0 > 2)
                p->unk09_0 = 0;
            fns[p->unk09_0](p);
        }
    }
}
asm(".global sub_08061E98\n.thumb_set sub_08061E98, AiUpdateUnitModes\n");

void AiScanBuildableFacilities(void)
{
    u8 v[5];
    int x;
    int y;

    sub_0808B6E8(v, gUnknown_0816DB20, 5);

    gUnknown_030046B8 = gUnknown_030045C0 = 0;

    for (y = 0; y < gMap->height; y++)
    {
        for (x = 0; x < gMap->width; x++)
        {
            if (IsCellOwnedByArmyTeam(gUnknown_030033EC, x, y) == 0)
                gUnknown_030045C0 |= v[gUnknown_085767F2[gMap->terrain[gMap->rowOffset[y] + x] & 0x1f] >> 1];

            gUnknown_030046B8 |= v[gUnknown_085767F2[gMap->terrain[gMap->rowOffset[y] + x] & 0x1f] >> 1];
        }
    }
}

asm(".global sub_08061F34\n.thumb_set sub_08061F34, AiScanBuildableFacilities\n");
