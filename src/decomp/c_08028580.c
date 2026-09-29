#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028580.
 * sub_08028580 @ 0x08028580
 */

/* MATCHED byte-for-byte (592/592).
 *
 * PROMOTION NOTE -- this match needs its .rodata pool words PLACED. Add to this
 * function's data/promoted.json entry:
 *     "rodata": ["0x08090B50", "0x08090B54"]
 * then re-run tools/split_rodata.py and tools/gen_lds.py before building. Those
 * two words are agbcc's own -fforce-addr address constants for
 * &gPlayers and &gUnknown_08499590; writing the globals' names
 * honestly reproduces the ROM's three-level `ldr rN,=<word>; ldr rN,[rN];
 * ldr rD,[rN]` exactly, which is why neither needs a declaration. Both symbols
 * ALSO appear as ordinary inline pool words elsewhere in this same function --
 * one honest name, two spellings, same as src/decomp/c_080240B4.c.
 *
 * THE LAST 10 BYTES, and the reusable rule behind them. The function sat at
 * 98.3% with size exact and a single divergent statement,
 * `gUnits[v].unk05_3 = 0;`: same six instructions, but the ROM
 * computes `v * 12` BEFORE dereferencing the pointer global, and the plain
 * subscript dereferences it immediately.
 *
 *     original                          plain subscript (WRONG)
 *     ldr  r1, [pc, #112]  &g           ldr  r0, [pc, #112]
 *     lsls r0, r4, #1                   ldr  r0, [r0, #0]      <- eager
 *     adds r0, r0, r4                   lsls r1, r4, #1
 *     lsls r0, r0, #2                   adds r1, r1, r4
 *     ldr  r1, [r1, #0]    *g           lsls r1, r1, #2
 *     adds r1, r1, r0                   adds r1, r1, r0
 *
 * BINDING THE ELEMENT ADDRESS TO ITS OWN LOCAL IS THE FIX, and it is measured,
 * not guessed -- compile_probe, one body and two spellings:
 *   `g[v].m = 0;`            -> ldr .L ; ldr [.] ; lsl ; add ; lsl ; add
 *   `u = &g[v]; u->m = 0;`   -> ldr .L ; lsl ; add ; lsl ; ldr [.] ; add
 * The second is the ROM, down to r0 holding the index and r1 the pointer.
 * `&g[v]` is an ADDRESS expression, so the global stays a MEM through operand
 * expansion and is forced into a register only at the add, after the index is
 * already computed. See docs/agbcc-codegen.md, wave 39 W39-E.
 *
 * REFUTED on the way, worth not repeating: `(v + gUnits)->unk05_3`,
 * to put the index syntactically first, is byte-for-byte IDENTICAL to the plain
 * subscript -- C defines them as the same tree and fold canonicalises them
 * together. And decomp-permuter found nothing better than the 98.3% starting
 * point in 300 s / ~17,000 iterations / 4 threads; it is useless on a residual
 * this shaped, exactly as the brief says.
 *
 * TWO TYPE FINDINGS THAT WERE EACH WORTH REAL BYTES:
 *
 * 1. gUnknown_084995F4 IS NOT `const`, worth 4 bytes and half the loop's
 *    register allocation. The table is in .rodata and every site is a read, so
 *    `const u16` is the obvious declaration; it is wrong. This function stores
 *    into the map plane THROUGH gUnknown_08499590 and then passes
 *    gUnknown_084995F4[team] to SetPropertyTileForOwner. With `const`, agbcc proves the
 *    store cannot alias the table and reuses the register it loaded for the
 *    store; the ROM reloads it (`ldrb r2,[r5]` after the `strb`). With `const`
 *    the function came out 588 bytes AND mis-allocated the loop; dropping it
 *    fixed size and every register slot at once.
 *
 * 2. gProperty MUST be a struct, not `u8 [][4]`. The ROM forms the
 *    element address once and takes the three fields off it as `[r,#0]/#1/#2`
 *    displacements, which only a COMPONENT_REF preserves; the flat array spells
 *    each field as its own address expression and agbcc answers with separate
 *    `+1` and `+2` force-addr pool words plus enough pressure to spill a value
 *    the ROM keeps in a register.
 *
 * Readout notes: `gPlayers[(v >> 6) + 1].unk2a` is the ROM's
 * `adds r1, #0x66` -- 0x3c + 0x2a, the 1-based army slot, NOT a member at +0x66
 * (struct PlayerStruct is only 0x3c long, and its unk2a comment already records
 * AreUnitsOnSameTeam/IsTerrainOwnedByUnitsTeam reaching it the same way). `i` is u16 from
 * `adds #1; lsls #0x10; lsrs #0x10` and the unsigned `bhi`; the `i <= 0x5b` half
 * of the loop condition folds away at the top guard because i is 0 there. r5
 * (&gUnknown_084995F4[team]) and sl (&gProperty) are LICM hoists, not
 * source, and so is the `str r2,[sp,#4]` holding &p->unk68 across the loop.
 */

struct Unk28580
{
    /* 0x0000 */ u8 filler_0000[0x64];
    /* 0x0064 */ s16 unk64;
    /* 0x0066 */ s16 unk66;
    /* 0x0068 */ u16 unk68;
};

void ArmyDefeat_Init(struct Unk28580 *p)
{
    u16 i;
    int team;
    u8 v;
    struct Unit *unit;

    StartCoSpeechScript(gUnknown_08499FA0[gPlayers[p->unk64].teamColor - 1], 0, 0);

    if (p->unk66 == 2)
    {
        struct Map *map;
        int idx;

        map = gMap;
        idx = map->rowOffset[gPlayers[p->unk64].hqY & 0x7f]
            + (gPlayers[p->unk64].hqX & 0x7f);
        team = map->terrain[idx] >> 5;
        map->terrain[idx] = 8 | gUnknown_084995F4[p->unk64];
        SetPropertyTileForOwner(gPlayers[p->unk64].hqX & 0x7f,
                     gPlayers[p->unk64].hqY & 0x7f,
                     gUnknown_084995F4[p->unk64]);
        RecountArmyProperties();
    }
    else
    {
        team = 0;
    }

    for (i = 0; i <= 0x5b && gProperty[i].flags != 0xff; i++)
    {
        if (MAP_OBJ_ARMY_MASK(gProperty[i].flags) == gUnknown_084995F4[p->unk64])
        {
            v = gMap->unit[
                    gMap->rowOffset[
                        gProperty[i].y] + gProperty[i].x];

            if (v != 0
             && gPlayers[(v >> 6) + 1].team == gPlayers[team].team)
            {
                unit = &gUnits[v];
                unit->unk05_3 = 0;
            }

            if (MAP_OBJ_TERRAIN(gProperty[i].flags) == 8)
            {
                gMap->terrain[
                    gMap->rowOffset[
                        gProperty[i].y] + gProperty[i].x]
                            = 6 | gUnknown_084995F4[team];
                sub_0802419C(gProperty[i].x, gProperty[i].y, 0);
                SetPropertyTileForOwner(gProperty[i].x, gProperty[i].y,
                             gUnknown_084995F4[team]);
            }
            else
            {
                gMap->terrain[
                    gMap->rowOffset[
                        gProperty[i].y] + gProperty[i].x]
                            = MAP_OBJ_TERRAIN(gProperty[i].flags)
                            | gUnknown_084995F4[team];
                SetPropertyTileForOwner(gProperty[i].x, gProperty[i].y,
                             gUnknown_084995F4[team]);
            }
        }
    }

    RebuildMapUnitLayers2();
    RecountArmyProperties();
    RecountArmyIncome();
    p->unk68 = 1;
}
asm(".global sub_08028580\n.thumb_set sub_08028580, ArmyDefeat_Init\n");
