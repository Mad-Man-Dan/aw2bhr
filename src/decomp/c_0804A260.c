#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A260.
 * sub_0804A260 @ 0x0804A260
 */

#include "hardware.h"
/*
 * MATCHES. Wave 53 (W53-A), 1004 bytes, two try_match attempts.
 *
 * NEEDS ITS .rodata POOL WORDS PLACED. promoted.json entry:
 *   "rodata": ["0x0812A274", "0x0812A278", "0x0812A27C"]
 * Those three words are agbcc's own `-fforce-addr` address constants for
 * gUnknown_030044E0, gBG1TilemapBuffer and gUnknown_02028E48 -- the same
 * mechanism the note on gUnknown_0813204C records for 0x0812A290/94/98/9C.
 * asm/ prints them as though objects lived at 0x0812A274/78/7C; they do not.
 * Dumped from baserom.gba they hold 0x030044E0, 0x0849957C and 0x02028E48.
 *
 * Two things cost the first attempt, both about how a member is SPELLED rather
 * than about the shape, and both worth carrying forward:
 *
 * 1. `*(s16 *)&gUnknown_030044E0->unk1e = 0` -- the spelling the NameEntry_DrawSprites
 *    draft also uses -- makes agbcc RELOAD the global pointer before the next
 *    member store, because a bare pointer-cast MEM is not MEM_IN_STRUCT_P and
 *    so aliases the pointer. The ROM does all six halfword stores (0x1e..0x28)
 *    off one `ldr r0,[r6]` and materialises both zero constants up front. Cast
 *    through a STRUCT instead and the reload goes away. That is what
 *    struct Unk030044E0View below is for.
 *
 * 2. The byte array at +0x41 is NOT reachable as `unk2c[0x15 + i]`. That folds
 *    to `(base + 0x2c) + (0x15 + i)` -- CSE shares the unk2c base and adds the
 *    biased index -- whereas the ROM computes `(base + 0x41) + i`, off a base
 *    register independent of unk2c. So +0x41 is a member in its own right and
 *    the shared struct's unk2c[0x2c] is too long. I have NOT reshaped it (four
 *    promoted files read unk2c and one, c_0804A64C.c, reads unk1e as a `ldrb`,
 *    which is why unk1e cannot simply become s16 either). The view struct gives
 *    both without touching anything shared. If someone retypes
 *    struct Unk030044E0 later, the evidence is: unk1e is a HALFWORD, unk2c runs
 *    0x2c..0x40 (0x15 bytes), and 0x41..0x57 is a separate NUL-terminated byte
 *    buffer -- this function fills it from *(u8 **)&unk58, terminates it, hands
 *    it to TrimTrailingFullWidthSpaces, and then copies it back into unk2c stopping at the
 *    first zero.
 *
 * The `?:` picking gBG2TilemapBuffer / gBG1TilemapBuffer is an if/else here for
 * the same reason the one in UnitList_Init is: the front end folds `x == 0 ? a :
 * b` into `x ? b : a` and inverts the branch, and the statement form is not
 * folded.
 */

/* A second view of the object gUnknown_030044E0 points at, for the two members
 * the shared struct in include/unknown-globals.h cannot spell: a HALFWORD at
 * +0x1e (declared there as `u8 unk1e` plus filler_1f, on the strength of
 * NameEntry_TypeChar's `ldrb`) and the byte array at +0x41 that the shared model
 * swallows inside unk2c[0x2c]. Cast through a struct rather than through a bare
 * `*(s16 *)&...`: the bare cast is not MEM_IN_STRUCT_P, so agbcc reloads the
 * global pointer after it and the ROM does not. */
struct Unk030044E0View
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
    /* 0x20 */ u8 filler_20[0x41 - 0x20];
    /* 0x41 */ u8 unk41[0x17];
};

void NameEntry_Init(void)
{
    u16 * p;
    u8 * src;
    s16 i;

    ResetTextTileCache(0);
    sub_080366C4(DefaultMainLoopCallback);
    sub_080366D0(DefaultVBlankCallback);
    SetupBackgrounds(gUnknown_084C3D1C);
    ((struct Unk030044E0View *)gUnknown_030044E0)->unk1e = 0;
    gUnknown_030044E0->unk20 = 0;
    gUnknown_030044E0->unk22 = 0;
    gUnknown_030044E0->unk24 = 0;
    gUnknown_030044E0->unk26 = 0;
    gUnknown_030044E0->unk28 = 0;
    gUnknown_030044E0->unk63 = 0;
    gUnknown_030044E0->unk61 = 14 - (gUnknown_030044E0->unk5e >> 1);
    gUnknown_030044E0->unk62 = 0;
    gUnknown_030044E0->unk2a = 0;
    gUnknown_030044E0->unk64 = 0;
    gUnknown_030044E0->unk66 = 0;
    LoadBg1WindowFrame(0);
    ApplyWindowFramePalette(0, 8);
    LoadCursorSpriteGraphics();
    ApplyPaletteExt(gUnknown_0809165C, 0x140, 0x20);
    gUnknown_03002B6C.bits.size = 1;
    gDispIo.disp_ct.bg1_enable = 0;
    FillTilemapRect(gBG1TilemapBuffer, 0, 0, 0x1E, 0x14, 0x360);

    if (gUnknown_02028E48 != 0 || gUnknown_030044E0->unk5c != 0)
    {
        gUnknown_03001FF8 = 0x1D8;
        gUnknown_030030A0 = 0x1D8;
        gUnknown_030044E0->unk2a = 0xF0;
    }
    else
    {
        gUnknown_03001FF8 = 0x138;
        gUnknown_030030A0 = 0x138;
        gUnknown_030044E0->unk2a = 0xA0;
    }

    ClearBg0Tilemap();
    CpuCopyAuto(gBG0TilemapBuffer, (void *)0x06007800, 0x800);
    ClearBg2Tilemap();
    ClearBg3TilemapBuffer();
    PutTextScriptImmediate(6, 6, gBG0TilemapBuffer, gUnknown_084C3B3C[0], 0x8000, 0);
    PutTextScriptImmediate(6, 8, gBG0TilemapBuffer, gUnknown_084C3B3C[1], 0x8000, 0);
    PutTextScriptImmediate(6, 0xA, gBG0TilemapBuffer, gUnknown_084C3B3C[2], 0x8000, 0);
    PutTextScriptImmediate(6, 0xC, gBG0TilemapBuffer, gUnknown_084C3B3C[3], 0x8000, 0);
    PutTextScriptImmediate(6, 0xE, gBG0TilemapBuffer, gUnknown_084C3B3C[4], 0x8000, 0);

    src = (u8 *)gUnknown_030044E0->unk58;

    for (i = 0; i < gUnknown_030044E0->unk5f; i++)
        ((struct Unk030044E0View *)gUnknown_030044E0)->unk41[i] = src[i];

    ((struct Unk030044E0View *)gUnknown_030044E0)->unk41[i] = 0;
    TrimTrailingFullWidthSpaces(((struct Unk030044E0View *)gUnknown_030044E0)->unk41);
    NameEntry_ClearBuffer();

    for (i = 0; i < gUnknown_030044E0->unk5f
              && ((struct Unk030044E0View *)gUnknown_030044E0)->unk41[i] != 0; i++)
        gUnknown_030044E0->unk2c[i] = ((struct Unk030044E0View *)gUnknown_030044E0)->unk41[i];

    gUnknown_030044E0->unk5d = i;
    RedrawNameEntryText(0);
    LoadBg3Backdrop(0, 0, 0);
    sub_080152C0((s32)gUnknown_0849D55C, 0);
    Decompress(gUnknown_084C3B2C[gUnknown_02028E40], (void *)0x06010000);
    Decompress(gUnknown_081358A0, (void *)0x060103E0);
    ApplyPaletteExt(gUnknown_0813204C, 0x2E0, 0x20);
    ApplyPaletteExt(gUnknown_0813204C, 0x280, 0x20);
    ApplyPaletteExt(gUnknown_0812B21C, 0x200, 0x20);
    ApplyPaletteExt(gUnknown_0812B21C, 0x220, 0x20);
    ApplyPaletteExt(gUnknown_0812B21C, 0x240, 0x20);
    ApplyPaletteExt(gUnknown_0812B21C, 0x260, 0x20);
    ApplyPaletteExt(gUnknown_0812B21C, 0x2A0, 0x20);

    if (gUnknown_02028E48 != 0 || gUnknown_030044E0->unk5c != 0)
        p = gBG2TilemapBuffer;
    else
        p = gBG1TilemapBuffer;

    TmApplyTsaClipped(p, gUnknown_030044E0->unk61 - 1, 1, gUnknown_0812AD2C, 0x8360);

    for (i = 0; i < gUnknown_030044E0->unk5e; i++)
        TmApplyTsaClipped(p, gUnknown_030044E0->unk61 + i, 1, gUnknown_0812AD38, 0x8360);

    TmApplyTsaClipped(p, gUnknown_030044E0->unk61 + i, 1, gUnknown_0812AD44, 0x8360);
    TmApplyTsaClipped(p, 5, 5, gUnknown_0812AD50, 0x8360);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
    BG_EnableSyncBG3();
    LoadCoFullBodyAndPalette(0, 0x27, 6);
    sub_080152C0((s32)gUnknown_084C3D8C, 0);
    PlayMusic(0x190);
}
asm(".global sub_0804A260\n.thumb_set sub_0804A260, NameEntry_Init\n");
