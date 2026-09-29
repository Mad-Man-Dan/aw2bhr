#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08058BB4.
 * sub_08058BB4 @ 0x08058BB4, sub_08058C54 @ 0x08058C54, sub_08058CE8 @ 0x08058CE8, sub_08058DEC @ 0x08058DEC, sub_08058E88 @ 0x08058E88, sub_08058F30 @ 0x08058F30, sub_08058F90 @ 0x08058F90
 */

/* WAVE 35: CANONICAL `struct Map`, extended by W36-D with the two plane starts
 * this block reads (+0x1432 and +0x3262). Both are carved out of existing
 * fillers -- unitUnk[0x1928] splits at 0x1432 and move[0x1928] splits at
 * 0x3262 -- so the layout is unchanged and the edit is byte-neutral: only a
 * field's START OFFSET enters the address arithmetic, never its length. */
/* struct UnitType's unk14 target, described as a struct so the `(p + 0x1a)
 * + t` association survives -- `p[0x1a + t]` on a bare `u8 *` folds 0x1a into
 * ldrb's displacement instead. Same reason struct Map's planes are members. */
struct MoveTbl
{
    /* 0x00 */ u8 filler_00[0x1a];
    /* 0x1a */ u8 unk1a[0x20];
};

/* Probes the four cells orthogonally adjacent to unit `id`, letting
 * AiScoreFiringCell keep the running best, and returns that best or -1 if it
 * rejected all four.
 *
 * BINDING THE SENTINEL IS LOAD-BEARING, and it is the whole difference between
 * this and a 52.5% draft. Writing the literal 0x270F at both the seed store and
 * the final test leaves it a plain constant: agbcc loads it into r0, parks it in
 * r8 for the calls and stores from r0, so the ROM's `mov r0, r8` before the
 * `strh` has nothing to come from. Giving the constant its OWN statement makes
 * it a pseudo whose live range starts before the store, so the store must read
 * it back out of the callee-saved register it was allocated -- which is exactly
 * the redundant-looking `ldr r0, =0x270F; mov r8, r0; mov r0, r8; strh` the ROM
 * carries. Same lever as the "binding locals are punctuation" rule in
 * docs/agbcc-codegen.md, read in the opposite direction: here the original had
 * one MORE local than the obvious spelling, not fewer.
 *
 * The out-pointer is `u16 *` and not `s16 *`: the final read is a plain `ldrh`.
 *
 * MATCHED. */
int AiPickFiringCellBesideUnit(u16 id, u16 * out)
{
    struct Unit * u;
    int best;
    int mask;
    int sentinel;

    u = &gUnits[id];
    best = 0;
    mask = gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d;
    sentinel = 0x270F;
    out[0] = sentinel;

    AiScoreFiringCell(u->x - 1, u->y, mask, &best, out);
    AiScoreFiringCell(u->x + 1, u->y, mask, &best, out);
    AiScoreFiringCell(u->x, u->y - 1, mask, &best, out);
    AiScoreFiringCell(u->x, u->y + 1, mask, &best, out);

    if (out[0] == sentinel)
        return -1;

    return best;
}
asm(".global sub_08058BB4\n.thumb_set sub_08058BB4, AiPickFiringCellBesideUnit\n");

/* AiPickFiringCellBesideUnit's twin, taking the cell coordinates directly instead of looking
 * them up from a unit id. Both parameters are `int` -- the prologue is a bare
 * `mov r8, r0` / `adds r4, r1, #0` with no PROMOTE_MODE shift pair, unlike
 * AiPickFiringCellBesideUnit's `u16` id. The fourth call's `adds r4, #1` clobbers y because
 * y is dead after it; that falls out of writing `y + 1` last.
 *
 * See c_08058BB4.c for why the 0x270F sentinel needs its own binding statement.
 *
 * MATCHED. */
int AiPickFiringCellBeside(int x, int y, u16 * out)
{
    int best;
    int mask;
    int sentinel;

    best = 0;
    mask = gUnknown_085D5ABC[gUnknown_030040D8->unk00].unk1d;
    sentinel = 0x270F;
    out[0] = sentinel;

    AiScoreFiringCell(x - 1, y, mask, &best, out);
    AiScoreFiringCell(x + 1, y, mask, &best, out);
    AiScoreFiringCell(x, y - 1, mask, &best, out);
    AiScoreFiringCell(x, y + 1, mask, &best, out);

    if (out[0] == sentinel)
        return -1;

    return best;
}
asm(".global sub_08058C54\n.thumb_set sub_08058C54, AiPickFiringCellBeside\n");

/* Scores the cell (x, y) as a move target and keeps the running best.
 *
 * gUnknown_08499590 is read TWICE and the two reads are spelled differently in
 * the ROM: the first goes through a -fforce-addr `.LC` word (three levels,
 * `ldr =word; ldr; ldr`) and the second is a plain `ldr =gUnknown_08499590;
 * ldr`. Writing the symbol honestly at both sites reproduces exactly that --
 * agbcc supplies the force-addr copy for the first and reloads at the `.L9`
 * merge because old-gcc's CSE table is per extended basic block and MEM loads
 * are not GCSE'd, while the `y * 2` subexpression IS (it survives in ip).
 * The `map` local covers only the first, single-predecessor stretch; the last
 * block must name the global again or the reload disappears.
 *
 * MATCHED first draft. */
void AiScoreFiringCell(int x, int y, int mask, int * pBest, u16 * out)
{
    struct Map * map;
    int idx;
    int score;

    score = 0;

    if (x < 0)
        return;
    if (y < 0)
        return;

    map = gMap;

    if (x >= map->width)
        return;
    if (y >= map->height)
        return;

    idx = map->rowOffset[y] + x;

    if (gMap->unit[idx] != gUnknown_03003F38 && gMap->unit[idx] != 0)
        return;

    if (gUnknown_03003340[y][x] > 0x78)
        return;

    if (gUnknown_030040D8->unk00 == 0
     || gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 0x10)
        score = gUnknown_085D583C[map->terrain[idx] & 0x1f].defense * 10;

    if ((gMap->dangerMask[
            gMap->rowOffset[y] + x] & mask) == 0)
        score += 100;

    if (score < *pBest)
        return;

    *pBest = score;
    out[0] = x;
    out[1] = y;
}
asm(".global sub_08058CE8\n.thumb_set sub_08058CE8, AiScoreFiringCell\n");

/* The AiCheckDropNeighbour counterpart of AiPickFiringCellBesideUnit/AiPickFiringCellBeside: gates on the
 * current unit's move-cost entry for the terrain under (x, y) and then offers
 * the four neighbours to AiCheckDropNeighbour, which writes back the first it accepts.
 * Returns 0 on success and -1 both when the gate rejects and when no neighbour
 * was accepted -- the ROM reaches the same `movs r0,#1; rsbs` tail from both.
 *
 * The sentinel binding is the same lever c_08058BB4.c documents, one notch
 * weaker: the constant needs its own statement so it lands directly in the
 * callee-saved r4 (`ldr r4, =0x270F; strh r4, [r5]`), where the literal alone
 * loads into r0 and copies. r4 is a low register here, so unlike AiPickFiringCellBesideUnit
 * no `mov` back is needed at the store. */
int AiPickDropCellBeside(int x, int y, u16 * out)
{
    struct MoveTbl * tbl;
    struct Map * map;
    int sentinel;

    tbl = (struct MoveTbl *)gUnknown_085D5ABC[gUnknown_030040D8->unk00].transportTable;
    map = gMap;

    if (tbl->unk1a[map->terrain[map->rowOffset[y] + x] & 0x1f] == 0)
        return -1;

    sentinel = 0x270F;
    out[0] = 0x270F;

    AiCheckDropNeighbour(x - 1, y, out);
    AiCheckDropNeighbour(x + 1, y, out);
    AiCheckDropNeighbour(x, y - 1, out);
    AiCheckDropNeighbour(x, y + 1, out);

    if (out[0] == sentinel)
        return -1;

    return 0;
}
asm(".global sub_08058DEC\n.thumb_set sub_08058DEC, AiPickDropCellBeside\n");

/* Accepts (x, y) as a destination and writes it to *out. The single
 * `lsls #0x18; asrs #0x18` on the gUnknown_03003340 read serves BOTH compares,
 * so it is an int local carrying one explicit (s8) cast, not an s8 local.
 *
 * MATCHED first draft. */
void AiCheckDropNeighbour(int x, int y, u16 * out)
{
    struct Map * map;
    struct MoveTbl * tbl;
    int idx;
    int t;

    if (x < 0)
        return;
    if (y < 0)
        return;

    map = gMap;

    if (x >= map->width)
        return;
    if (y >= map->height)
        return;

    idx = map->rowOffset[y] + x;

    if (gMap->unit[idx] != gUnknown_03003F38 && gMap->unit[idx] != 0)
        return;

    t = (s8)gUnknown_03003340[y][x];

    if (t < 0)
        return;
    if (t == 0x79)
        return;

    tbl = (struct MoveTbl *)gUnknown_085D5ABC[gUnknown_030040D8->unk00].transportTable;

    if (tbl->unk1a[map->terrain[idx] & 0x1f] == 0)
        return;

    out[0] = x;
    out[1] = y;
}
asm(".global sub_08058E88\n.thumb_set sub_08058E88, AiCheckDropNeighbour\n");

/* Writes a per-unit value to *out, defaulting to 0x78 and clamping back to
 * 0x78 on overflow.
 *
 * The final `if (*out > 0x78)` reads the REGISTER, not memory: agbcc forwards
 * the just-stored value through CSE and only has to re-apply the QImode
 * truncation, which is the `lsls #0x18; lsrs #0x18` pair the ROM carries. A
 * separate local would have needed its own truncation at the store as well.
 * gUnknown_030040D8->unk00 is read once and CSEd into GetUnitMovementWithCoBonus's second
 * argument, which is why r2 survives the stride multiply.
 *
 * MATCHED first draft. */
void AiGetReachBudget(u8 * out)
{
    *out = 0x78;

    if ((gUnknown_030046B8 & 2) == 0)
        return;

    if (gUnknown_085D5ABC[gUnknown_030040D8->unk00].deployLocation != 7)
        return;

    *out = GetUnitMovementWithCoBonus(gUnknown_030033EC, gUnknown_030040D8->unk00)
         * gUnknown_085766E0->unk0e;

    if (*out > 0x78)
        *out = 0x78;
}
asm(".global sub_08058F30\n.thumb_set sub_08058F30, AiGetReachBudget\n");

/* Runs AiConsiderEnemyHq once per set bit of the current army's four-bit mask at
 * gPlayers[gUnknown_030033EC].unk2c, passing the 1-based army index
 * and a shared s16 sentinel. Returns -1 if nothing wrote the sentinel.
 *
 * The sentinel really is `s16` and not `u16`: the final read is
 * `movs r1,#0; ldrsh r0,[r0,r1]`, the register-offset form agbcc uses for a
 * signed halfword on the stack. Its callers' out-pointers, by contrast, are
 * read with a plain `ldrh` -- see the note on AiScoreFiringCell in
 * include/unknown-functions.h.
 *
 * The final test is written `!= sentinel -> 0` and not `== sentinel -> -1`.
 * Both are the same predicate, but the arm written FIRST is the fall-through
 * and the other is laid out after the literal pool, so the spelling decides
 * which of `movs r0,#0` and `movs r0,#1; rsbs` follows the `bne`. That is 16
 * bytes and the only difference between this and the obvious ordering.
 *
 * The mask expression is written out four times rather than bound to a local:
 * a non-const global's MEM is killed by every call, so the ROM re-derives the
 * whole `unk2c` address each time and only the two pool words survive in
 * callee-saved registers.
 *
 * MATCHED. */
int AiFindNearestEnemyHq(void * arg)
{
    s16 best;

    best = 0x7FFF;

    if (gPlayers[gUnknown_030033EC].unk2c & 1)
        AiConsiderEnemyHq(1, &best, arg);

    if (gPlayers[gUnknown_030033EC].unk2c & 2)
        AiConsiderEnemyHq(2, &best, arg);

    if (gPlayers[gUnknown_030033EC].unk2c & 4)
        AiConsiderEnemyHq(3, &best, arg);

    if (gPlayers[gUnknown_030033EC].unk2c & 8)
        AiConsiderEnemyHq(4, &best, arg);

    if (best != 0x7FFF)
        return 0;

    return -1;
}
asm(".global sub_08058F90\n.thumb_set sub_08058F90, AiFindNearestEnemyHq\n");
