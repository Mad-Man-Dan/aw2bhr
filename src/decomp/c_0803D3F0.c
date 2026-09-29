#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0803D3F0.
 * sub_0803D3F0 @ 0x0803D3F0, sub_0803D48C @ 0x0803D48C, sub_0803D4A8 @ 0x0803D4A8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

/* The local `struct Map` cast is load-bearing, not decoration: the ROM
 * computes every plane address as `(map + K) + idx`, and only a COMPONENT_REF
 * preserves that association. Written as pointer arithmetic
 * (`*(u16 *)(gUnknown_08499590 + 0x417A + y * 2)`) fold's `associate` path
 * pulls the constant outward and emits `(map + y * 2) + 0x417A` instead --
 * measured on this function, where it also cost the two registers the ROM
 * spends holding 0x417A and 0x1432 across the inner loop. Same finding as the
 * wave-34 note in include/unknown-globals.h, reached independently here.
 *
 * The member set is written to match CopyMapRecordToGMap's copy of this declaration
 * exactly, so the two survive being promoted into one translation unit. */
/* The 0x02000000 staging record. Declared locally and cast onto the `u8 []`
 * global for the same reason src/decomp's `struct Map` files do it: the ROM
 * computes the copy loop's source as `(rec + 0x4C4) + i`, and only a
 * COMPONENT_REF preserves that association. Spelled as pointer arithmetic
 * (`p[0x4C4 + i]`, `(p + 0x4C4)[i]`, both measured) fold's `associate` path
 * pulls the constant outward and emits `(i + p) + 0x4C4` -- same size, four
 * wrong bytes, and it was the whole residual of a 95.1% candidate. */
struct Rec
{
    /* 0x0000 */ u8 width;
    /* 0x0001 */ u8 height;
    /* 0x0002 */ u16 tile[(0x04B2 - 0x0002) / 2];
    /* 0x04B2 */ u8 name[0x11];
    /* 0x04C3 */ u8 unk4C3;
    /* 0x04C4 */ u8 unk4C4[5];
    /* 0x04C9 */ u8 unk4C9;
    /* 0x04CA */ u8 unk4CA;
    /* 0x04CB */ u8 cell[1];
};

void RebuildTerrainFromTiles(void)
{
    int x, y;

    for (y = 0; y < gMap->height; y++) {
        for (x = 0; x < gMap->width; x++) {
            int idx = gMap->rowOffset[y] + x;

            gMap->terrain[idx] =
                gUnknown_0849959C[gMap->tile[idx]];
        }
    }
    RecountArmyProperties();
    RebuildMapUnitLayers2();
}
asm(".global sub_0803D3F0\n.thumb_set sub_0803D3F0, RebuildTerrainFromTiles\n");

/* Drives LoadDesignRoomSlotEntry over slots 0..11. The counter is `u8`: it is
 * re-narrowed (`adds r0,r4,#1; lsls #0x18; lsrs #0x18`) on every increment,
 * which an `int` would not do, and `cmp r4,#0xb; bls` is the unsigned test
 * `i < 12` on that width. The result of each call is discarded, so nothing
 * here settles LoadDesignRoomSlotEntry's return type. */
void RefreshDesignRoomSlotDirectory(void)
{
    u8 i;

    for (i = 0; i < 12; i++)
        LoadDesignRoomSlotEntry(i);
}

/* `p` must be bound BEFORE the guard, not at first use: the ROM loads
 * gUnknown_02000000 in the entry block, ahead of `cmp r5,#3`, which only
 * happens when the pseudo's live range crosses the `bl IsSaveSlotInvalid` and so
 * earns a callee-saved register. Naming the global at each use instead puts
 * the `ldr` inside the success block and reorders the literal pool.
 *
 * The guard is spelled as the FAILING condition with `||`, not as the
 * succeeding one with `&&`: `a > 3 || f(b) != 0` puts the 0xff store
 * immediately after the two branches (`bhi` and `beq` both jumping over it),
 * which is the ROM's block order. The `&&` spelling emits the success path
 * first and the failure path last. */
int LoadDesignRoomSlotEntry(u8 a)
{
    struct Rec *p = (struct Rec *)gUnknown_02000000;
    u8 b;
    u8 i;

    if (a > 3 || IsSaveSlotInvalid(b = a + 5) != 0) {
        gUnknown_020280C0[a].unk13 = 0xff;
        return 0;
    }
    ReadSaveSlot(b, (u8 *)p);
    CopyString(gUnknown_020280C0[a].unk02, p->name);
    gUnknown_020280C0[a].unk13 = p->unk4C3;
    gUnknown_020280C0[a].filler_14[5] = p->unk4C9;
    gUnknown_020280C0[a].filler_14[6] = p->unk4CA;
    for (i = 0; i < 5; i++)
        gUnknown_020280C0[a].filler_14[i] = p->unk4C4[i];
    return 1;
}
asm(".global sub_0803D4A8\n.thumb_set sub_0803D4A8, LoadDesignRoomSlotEntry\n");

asm(".global sub_0803D48C\n.thumb_set sub_0803D48C, RefreshDesignRoomSlotDirectory\n");
