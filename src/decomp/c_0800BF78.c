#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0800BF78.
 * MakeReefSafe @ 0x0800BF78
 */

/* Commits a tile edit at (x, y): stash the cell's current tile in
 * gActiveMap->cursorTerrain, repaint it, then re-run sub_0800C124 and then
 * sub_0800C22C over the cell and its four cardinal neighbours, and repaint
 * once more.  Returns 0 if IsSeaAt rejects the cell, 1 otherwise.  The
 * `bl IsSeaAt` is reached with r0/r1 untouched from entry -- a
 * pass-through, not a nullary call.  The tile fetch is the promoted
 * `c_08001158.c` idiom unchanged (rows at +0x417A, tiles at +0xA22).
 *
 * `pp`, `q` and `s` are the -fforce-addr workaround and none of them is the
 * original source: `gUnknown_08499590` spelled by name is byte-for-byte exact
 * here on the FIRST try, and only its pool word relocates against this unit's
 * own .rodata, which the .text-only split cannot place.  Naming the ROM's word
 * instead means reproducing by hand the CSE the original got for free, and the
 * three locals are a direct readout of where the ROM caches and where it does
 * not:
 *
 *  - `q` is the middle level bound at the top (sb).  It serves the tile fetch
 *    and the FIRST half's `if (y > 0)` inner read, which is the only read in
 *    the function that the original satisfied from the top's forced address.
 *  - `s` is the middle level rebound after each of the two `if (x > 0)` merges
 *    (r7 both times), and each binding then serves three reads across
 *    intervening calls.  `**pp` cannot do this: a const global reached through
 *    a local pointer is not RTX_UNCHANGING_P, so the outer load does not
 *    survive a `bl` -- the wave-13 finding that says to bind the middle level
 *    explicitly.
 *  - `**pp` is right, and a local is wrong, at exactly one site: the SECOND
 *    half's `if (y > 0)` inner read, where the ROM re-derives both levels.
 *
 *  - `b` is bound before `q` because the ROM loads gActiveMap's pool
 *    word first.  That is the LHS-expands-first rule, but it only reaches the
 *    top of the block if the destination is a local: with the RHS split into
 *    statements (which the c_08001158 idiom requires) the store's own
 *    `ldr rN, =sym` would otherwise be emitted last.
 *  - `pp = &gUnknown_0808D86C` sits after sub_0800EC20, not at the top, because
 *    that is where the ROM emits `mov r8, r4`; the pool word itself is loaded
 *    once, at the top, and r4 carries it across the three intervening calls. */
int MakeReefSafe(int x, int y)
{
    u8 **const *pp;
    u8 **q;
    u8 **s;
    struct ActiveMap *b;
    u8 *p;
    u8 *rows;
    u8 *tiles;
    int t;
    int off;
    int n;

    if (IsSeaAt(x, y) == 0)
        return 0;

    if (GetPropertyKindAt(x, y))
        RemovePropertyAt(x, y);

    b = gActiveMap;
    q = gUnknown_0808D86C;
    p = *q;
    t = y * 2;
    rows = p + 0x417A;
    off = (*(u16 *)(rows + t) + x) * 2;
    tiles = p + 0xA22;
    b->cursorTerrain = *(u16 *)(tiles + off);

    SetTerrainAt(x, y, 0x13);
    MakeTileSimple(x, y, 0x168);
    sub_0800EC20(x, y);

    pp = &gUnknown_0808D86C;

    if (y > 0)
    {
        n = y - 1;
        if (x > 0)
            sub_0800C124(x - 1, n);
        sub_0800C124(x, n);
        if (x < *(u16 *)*q - 1)
            sub_0800C124(x + 1, n);
    }
    if (x > 0)
        sub_0800C124(x - 1, y);

    s = *pp;

    if (x < *(u16 *)*s - 1)
        sub_0800C124(x + 1, y);
    if (y < *(u16 *)(*s + 2) - 1)
    {
        n = y + 1;
        if (x > 0)
            sub_0800C124(x - 1, n);
        sub_0800C124(x, n);
        if (x < *(u16 *)*s - 1)
            sub_0800C124(x + 1, n);
    }

    if (y > 0)
    {
        n = y - 1;
        if (x > 0)
            sub_0800C22C(x - 1, n);
        sub_0800C22C(x, n);
        if (x < *(u16 *)**pp - 1)
            sub_0800C22C(x + 1, n);
    }
    if (x > 0)
        sub_0800C22C(x - 1, y);

    s = *pp;

    if (x < *(u16 *)*s - 1)
        sub_0800C22C(x + 1, y);
    if (y < *(u16 *)(*s + 2) - 1)
    {
        n = y + 1;
        if (x > 0)
            sub_0800C22C(x - 1, n);
        sub_0800C22C(x, n);
        if (x < *(u16 *)*s - 1)
            sub_0800C22C(x + 1, n);
    }

    SetTerrainAt(x, y, 0x13);
    MakeTileSimple(x, y, 0x168);
    return 1;
}

asm(".global sub_0800BF78\n.thumb_set sub_0800BF78, MakeReefSafe\n");
