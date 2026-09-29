#ifndef UNKNOWN_GLOBALS_H
#define UNKNOWN_GLOBALS_H

#include "global.h"

/* Types and declarations for the globals that do not have real names yet;
 * the counterpart to unknown-functions.h. Every promoted file gets this
 * header through global.h, so each global has one declaration and one type.
 *
 * Rules for editing:
 *   * A struct here is a superset. Adding a field is fine; moving one is not.
 *     Offsets, member types and the total size all matter: agbcc picks the
 *     load and store instructions from the member type and the index stride
 *     from sizeof, so a change to any of them silently breaks a match
 *     somewhere else. The size in the comment on each struct is the checked
 *     value.
 *   * `filler_XX` is unexplored space, `unkXX` is a field something reads.
 *     Narrowing a filler into a field is the normal way this grows.
 *   * The display-register shadows (gUnknown_03002B6C, gUnknown_030030A4,
 *     gUnknown_030030E0 and neighbours) are not here; they live in
 *     hardware.h, next to the register types they mirror.
 */

/* ---------------------------------------------------------------- types -- */

struct UnkVec
{
    u32 unk00;
    u32 unk04;
};

/* One sprite's OAM attributes (attr0, attr1, attr2) in the GBA hardware layout.
 * `struct UnkVec` is the same eight bytes seen as two plain words. hFlip,
 * paletteNum, priority and tileNum are confirmed by the code that uses them
 * (sub_0804D928, sub_0804E3B4); the other field names are the standard GBA
 * ones. */
struct OamData
{
    /* 0x00 */ u32 y : 8;
               /* attr0 bits 8 and 9: rotation/scaling enable, and double-size
                * (when bit 8 is set) or OBJ disable (when it is clear). Keep
                * them two 1-bit fields: sub_08016944 and sub_08016974 change
                * bit 9 alone, and a single 2-bit field compiles differently. */
               u32 affineEnable : 1;
               u32 doubleSize : 1;
               u32 objMode : 2;
               u32 mosaic : 1;
               u32 bpp : 1;
               u32 shape : 2;
    /* 0x02 */ u32 x : 9;
               u32 matrixNum : 3;
               u32 hFlip : 1;
               u32 vFlip : 1;
               u32 size : 2;
    /* 0x04 */ u16 tileNum : 10;
               u16 priority : 2;
               u16 paletteNum : 4;
    /* 0x06 */ u16 affineParam;
};

/* A 0x10-byte node, walked in arrays by sub_08017A58 and its neighbours. unk0c
 * is either the value to store (sub_08017B64, sub_08017B8C, sub_08017BB0) or
 * the address to read that value from (sub_08017BD4, sub_08017BFC,
 * sub_08017C24). unk04 is the store's target address in some of these functions
 * and the next node in sub_08018BAC. The functions cast both fields at each
 * use. */
struct Unk0200C528Node /* 0x10 */
{
    /* 0x00 */ u8 filler_00[4];
    /* 0x04 */ struct Unk0200C528Node *unk04;
    /* 0x08 */ u16 unk08; /* read by sub_08018C54, sub_08018D90 */
    /* 0x0a */ u16 unk0a; /* read by sub_08018CBC, sub_08018D90 */
    /* 0x0c */ u32 unk0c;
};

struct Unk0200C528 /* 0x18 */
{
    /* 0x00 */ struct Unk0200C528Node *unk00;
    /* 0x04 */ struct Unk0200C528Node *unk04;
    /* 0x08 */ struct Unk0200C528Node *unk08; /* sub_08018B40 stores a node's unk04 here */
    /* 0x0c */ u16 unk0c;
    /* 0x0e */ s16 unk0e; /* counter that the sub_080180A8, sub_080180CC and
                           * sub_0801820C callbacks step by 1 or 4 */
    /* 0x10 */ s8 unk10; /* written by sub_0803D8C0 and sub_0803D92C;
                          * sub_0803D8F8 tests it for 6 */
    /* 0x11 */ u8 unk11; /* copied from the node's unk08 by
                          * sub_08018A64/sub_08018ADC; read by the callbacks
                          * they install */
    /* 0x12 */ s16 unk12; /* copied from the node's unk0a by sub_08018A64;
                           * sub_08018A28 tests it for < 0 */
    /* 0x14 */ u32 unk14; /* sub_08019818 stores its u16 parameter here, the
                           * only access; the real type is unknown */
};

struct SpriteEntry /* 0x10 */
{
    /* 0x00 */ struct SpriteEntry *next;
    /* 0x04 */ u16 oam1;
    /* 0x06 */ u16 oam0;
    /* 0x08 */ u16 oam2;
    /* 0x0c */ u16 *object;
};

/* Draw-gate flags in bits 12 and 13 of Unk0200E438.unk30. */
struct SpriteScriptFlags
{
    u32 unk00_0 : 12;
    s32 hidden : 1;
    s32 flicker : 1;
};

/* unk0c/unk10 is an (x, y) step and unk14/unk18 the position it moves, both 8.8
 * fixed point: sub_0801D98C sets the step, sub_0801D96C the position, and
 * sub_0801DDBE adds the step to the position. unk30 is returned by value by
 * sub_08015608 and sub_08015638. unk1e and unk28 are only ever read unsigned,
 * which does not prove they are u8 rather than s8. */
struct Unk0200E438 /* 0x4c */
{
    /* 0x00 */ u16 unk00; /* sub_0801DC50 zeroes unk00 and unk02 */
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u32 unk08;
    /* 0x0c */ s32 unk0c;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 filler_1f[0x01];
    /* 0x20 */ u32 unk20; /* set by sub_08015778 */
    /* 0x24 */ u16 unk24; /* sub_0801DC50 zeroes unk24 and unk26 */
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 filler_29[0x03];
    /* 0x2c */ u32 unk2c; /* returned by sub_0801DA44 */
    /* 0x30 */ struct UnkVec unk30;
    /* 0x38 */ s16 unk38; /* compared with -1, then passed to sub_08015328 */
    /* 0x3a */ s16 unk3a; /* sub_0801DC50 sets it to -1 */
    /* 0x3c */ u16 unk3c;
    /* 0x3e */ u16 unk3e;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 filler_42[0x02];
    /* 0x44 */ u32 unk44;
    /* 0x48 */ u32 *unk48;
};

/* struct Unk0200C420 (defined after the record type below) is the 0xe0-byte
 * object at gUnknown_0200C420. Its first two words are a save/restore pair
 * shared with the 8-byte gUnknown_0200C500; the rest is unrelated. unk20 is
 * indexed by `a % 24` (sub_08017860). */
/* A 4-byte record: three fields of 8, 12 and 12 bits packed in one word. Used
 * by gUnknown_0200C078, gUnknown_0200C2D0 and gUnknown_0200C420.unk38. Must
 * stay one bitfield word, not a u8 and two u16s: sub_08016A54 and sub_08038368
 * set and clear the fields with word masks that only this layout produces. */
struct Unk0200C078Rec /* 0x04 */
{
    u32 unk00_00 : 8;
    u32 unk00_08 : 12;
    u32 unk00_14 : 12;
};

struct Unk0200C420 /* 0xe0 */
{
    /* 0x00 */ u32 unk00; /* an amount that sub_08017704 subtracts from only
                           * when enough is left (it returns 0 otherwise) */
    /* 0x04 */ u32 unk04; /* sub_0803BB90 is the predicate `unk04 != 0` */
    /* 0x08 */ u8 unk08; /* sub_08016A14 bumps it while bit 0 is clear */
    /* 0x09 */ u8 unk09; /* sub_08016C9C returns the address of unk09, unk0a or
                          * unk0b (its cases 2 to 4).
                          * gUnknown_0808E540/_0808E544/_0808E548 are
                          * compiler-made pool words holding these members'
                          * addresses, not objects; C code names the members,
                          * never the words. */
    /* 0x0a */ u8 unk0a;
    /* 0x0b */ u8 unk0b;
    /* 0x0c */ u8 unk0c; /* when nonzero, sub_08085F94 makes its second
                          * sub_08087974 call */
    /* 0x0d */ u8 unk0d; /* sub_0803BBA8 stores the second word of a
                          * gUnknown_0849EACC entry here, cut to a byte; nothing
                          * reads it */
    /* 0x0e */ u8 unk0e; /* sub_08034780 copies it into gPlaySt.unk09; nothing
                          * else uses it */
    /* 0x0f */ u8 unk0f; /* counter that sub_08049B28 increments and holds at
                          * 0xFF, so it must stay unsigned */
    /* 0x10 */ u16 unk10; /* a counter sub_0803C52C passes to sub_08037DA4,
                           * which maps it to 2..5 at the thresholds 0xc7, 0xf9
                           * and 0x117 */
    /* 0x12 */ u16 unk12; /* zeroed with unk10 by sub_08016A54; nothing reads it */
    /* 0x14 */ u8 unk14; /* sub_08034780 sets gPlaySt.unk0c to `unk14 == 0` */
    /* 0x15 */ u8 filler_15[0xb];
    /* 0x20 */ u8 unk20[0x18];
    /* 0x2a records filling the rest of the struct. In each, unk00_08 is nonzero
     * for a live entry, unk00_00 is the id sub_08038368 searches for, and
     * unk00_14 is the quantity sub_08038434 sums and averages. Indices 0..0x28
     * are walked, and sub_08038368 clears the entry after the last one as an
     * end marker, so 0x29 is used too. */
    /* 0x38 */ struct Unk0200C078Rec unk38[0x2a];
};

/* The state gActiveMap points at: cursor, edit mode, panels and counters used
 * by the map design screens. Must stay a struct, not a u16 array:
 * sub_08003934's `|= 8` on flags only compiles to the original bytes through a
 * struct member. */
struct ActiveMap /* 0xb0 */
{
    /* 0x00 */ u16 flags; /* bit 3 set by sub_08003934, cleared by sub_08003948;
                           * bit 14 set by sub_08004BC0 */
    /* 0x02 */ u16 state;
    /* 0x04 */ u16 mode;
    /* 0x06 */ s8 stateChanged; /* set to 1 by sub_0800056C, tested by
                                 * sub_08000694 */
    /* 0x07 */ s8 editMode; /* 0 = terrain editing (a ring of 10 items), nonzero
                             * = unit editing (a ring of 8) */
    /* 0x08 */ s16 cursorX; /* cursor position in map cells (y in cursorY);
                             * sub_080085E0 passes both to many tile functions */
    /* 0x0a */ s16 cursorY;
    /* 0x0c */ s32 stateTimer; /* countdown that sub_08000694 sets to 2 or 10
                                * and decrements */
    /* 0x10 */ u8 designSlot; /* 0, 1 or 2: sub_08004EDC, sub_08004F1C and
                               * sub_08004F5C each test one flags bit (0x200,
                               * 0x400, 0x800) and store the matching value
                               * before starting the same script */
    /* 0x11 */ s8 inputDelay; /* delay counter in sub_08000D9C, reloaded with
                               * 0x14 when it reaches zero */
    /* 0x12 */ u8 propertyCount; /* number of live gProperty records.
                                  * sub_0800C574 appends at this index (0x5B is
                                  * the last valid one), sub_0800C608 decrements
                                  * it down to 0, and sub_0800C874 recounts the
                                  * list. Readers cast it to s8; keep the member
                                  * u8. */
    /* 0x13 */ u8 army1UnitCount; /* unit counts for armies 1-4, filled by
                                   * sub_080088F0 from sub_08025308(1..4) */
    /* 0x14 */ u8 army2UnitCount;
    /* 0x15 */ u8 army3UnitCount;
    /* 0x16 */ u8 army4UnitCount;
    /* 0x17 */ u8 hqX[4]; /* x of each army's HQ, set by sub_0800C75C and
                           * cleared to 0xFF by sub_0800C7A4. Readers cast to
                           * s8, so 0xFF reads as -1, "none". Must stay u8:
                           * sub_0800C8D8's `|= 0xFF` on hqX and hqY only
                           * compiles to the original bytes for an unsigned
                           * member. */
    /* 0x1b */ u8 hqY[4]; /* y of each army's HQ, same index as hqX.
                           * sub_0800C7A4 writes -1 here to clear it; u8 for the
                           * same reason as hqX. */
    /* 0x1f */ u8 filler_1f[0x01];
    /* 0x20 */ u16 cursorTerrain; /* map tile under the cursor, stored by
                                   * sub_080085E0 on entry */
    /* 0x22 */ u8 filler_22[0x02];
    /* 0x24 */ u16 cursorUnit; /* unit id at the cursor (sub_08008928); its low
                                * six bits select an entry of gUnknown_085D5ABC */
    /* 0x26 */ u16 savedUnit; /* sub_08007354 saves gDesignRing[i].itemId here
                               * in unit editing, or in savedTerrain in terrain
                               * editing */
    /* 0x28 */ u16 selectionIndex; /* index into gUnknown_0200B224 (sub_08001CE8) */
    /* 0x2a */ u16 selectedTerrain; /* sub_080085E0 switches on its low five bits
                                     * (cases 1-19) and passes the whole value to
                                     * sub_0800C454 */
    /* 0x2c */ u16 savedTerrain; /* terrain-editing counterpart of savedUnit */
    /* 0x2e */ s8 propertyArmy;  /* copied to savedPropertyArmy by sub_08007354.
                                  * Must have the same type as unitArmy:
                                  * sub_08005F4C reads one or the other through
                                  * a single pointer chosen by editMode. */
    /* 0x2f */ s8 unitArmy; /* set by sub_080078D4; sub_08008928 passes it to
                             * sub_08025308 */
    /* 0x30 */ u8 savedPropertyArmy;  /* copies of propertyArmy and unitArmy made by
                                       * sub_08007354 */
    /* 0x31 */ u8 savedUnitArmy;
    /* 0x32 */ s8 selectionAnimKind; /* a 0..4 selector sub_08005F4C stores in its
                                      * state 0x28 and passes to sub_080077EC /
                                      * sub_080078D4 */
    /* 0x33 */ s8 selectionAnimFrame; /* frame index into the script
                                       * gUnknown_084886DC; sub_08005F4C clears it,
                                       * then steps it once per state */
    /* 0x34 */ s8 spriteFrame;  /* read by sub_08001DAC and passed to
                                 * sub_0800272C/sub_08002844; no writer found,
                                 * so the width is not confirmed */
    /* 0x35 */ u8 filler_35[0x01];
    /* 0x36 */ s8 terrainListIndex;  /* a list index and its saved copy for each
                                      * edit mode: sub_08007354 copies
                                      * terrainListIndex to savedTerrainListIndex
                                      * and unitListIndex to savedUnitListIndex */
    /* 0x37 */ s8 savedTerrainListIndex;
    /* 0x38 */ s8 unitListIndex;
    /* 0x39 */ s8 savedUnitListIndex;
    /* 0x3a */ s16 ringIndex; /* position in gDesignRing; sub_08007328 stores
                               * (ringIndex + n - 1) % n into previousRingIndex,
                               * with n chosen by editMode */
    /* 0x3c */ u16 previousRingIndex;
    /* 0x3e */ s16 panelSide; /* 0 places the panels on the left (sprite x 2),
                               * nonzero on the right (x 0xCE) */
    /* 0x40 */ u8 filler_40[0x02];
    /* 0x42 */ s16 introScreenY; /* screen y that moves between -10 and -2
                                  * (sub_08003890 reads it, sub_08003910 sets it
                                  * to -10) */
    /* 0x44 */ u8 filler_44[0x02];
    /* 0x46 */ u16 menuCursorX; /* an x/y pair the screen setups write together:
                                 * (0x15,0x10) by sub_08004B7C/sub_08004C5C,
                                 * (0x57,0x10) by sub_080049E8, (0x15,0x18) by
                                 * sub_08004C10. Nothing reads them yet. */
    /* 0x48 */ u16 menuCursorY;
    /* 0x4a */ u16 tilePanelState; /* state of the second state machine in
                                    * sub_08001DAC: 0, 0xA, 0x14, 0x1E, 0x64, 0x6E
                                    * or 0x78 */
    /* 0x4c */ u16 tilePanelYState;
    /* 0x4e */ s16 tilePanelX; /* screen x that sub_08001DAC eases toward a
                                * target by `v += (K - v) >> 3`; values
                                * -0x3C0..0x1180 */
    /* 0x50 */ s16 tilePanelY; /* screen y moved by tilePanelYSpeed and clamped
                                * to 0x6A..0xB8 */
    /* 0x52 */ u8 filler_52[0x02];
    /* 0x54 */ s16 tilePanelYSpeed; /* step added to tilePanelY, capped at 8 */
    /* 0x56 */ s16 selectionAffineScale; /* OBJ affine scale for sub_08005F4C: starts at
                                          * 0x100 and steps down by 0x40 to no less than
                                          * 0x10, or takes frames from
                                          * gUnknown_084886DC. Used as a divisor. */
    /* 0x58 */ s8 cursorIdleFrames;  /* counts frames while the cursor is held
                                      * (sub_0800081C); past 0x31 it resets to 0 and
                                      * sub_08004D10 runs */
    /* 0x59 */ s8 cursorIdleTimer;  /* set to 0xc while the cursor is held and
                                     * counted down otherwise; when it passes zero,
                                     * both idle bytes reset */
    /* 0x5a */ u8 sidePanelState; /* state byte for sub_08002510: 0, 0xA, 0x14,
                                   * 0x1E, 0x32, 0x3C, 0x46, 0x50, the same
                                   * sequence as armyPanelState, for a single
                                   * panel */
    /* 0x5b */ u8 sidePanelFrame; /* frame counter that sub_08002510 steps modulo
                                   * 0x40 */
    /* 0x5c */ s16 sidePanelX; /* screen x of the sidePanelState panel, eased
                                * like armyPanelX; values -0x280..0x11A0 */
    /* 0x5e */ u8 filler_5e[0x02];
    /* 0x60 */ s16 countPanelX; /* screen x of the countPanelState panel, eased
                                 * like sidePanelX; sub_08000E48 resets it to
                                 * 0xFC00. Declared signed to match sidePanelX;
                                 * no read confirms the sign. */
    /* 0x62 */ u8 filler_62[0x02];
    /* 0x64 */ u8 countPanelState; /* state byte for sub_08002298, stepped through
                                    * the same sequence as sidePanelState */
    /* 0x65 */ u8 cursorMoved; /* set to 1 by sub_08000BF8 when the terrain or
                                * unit under the cursor changes; sub_08001DAC
                                * then runs the cursorMove animation and clears
                                * it */
    /* 0x66 */ u8 cursorMoveState; /* state of that animation: 0, 8, 9 or 0xA */
    /* 0x67 */ s8 cursorMoveTimer; /* frame countdown for that animation
                                    * (sub_08001DAC). Must stay s8: its `--` only
                                    * compiles to the original bytes through a
                                    * signed byte. */
    /* 0x68 */ s16 cursorMoveScale; /* scale of that animation, stepped by 0x20 up
                                     * to 0x100 and used with gSinLut and Div */
    /* 0x6a */ u8 soundId;  /* sound id sub_080085E0 leaves behind: 0x2d, 0x4b
                             * or 0x87..0x8a, depending on which case ran */
    /* 0x6b */ s8 spriteId;   /* sprite id, -1 for none; sub_08005F1C and
                               * sub_08007B74 pass it to sub_08015328 and then
                               * set it to -1 */
    /* 0x6c */ u8 overlayState;  /* cleared with overlayX/overlayY by
                                  * sub_08005D50 and sub_08007A0C */
    /* 0x6d */ u8 overlayTimer;  /* dwell counter for the overlayState machine;
                                  * sub_08007A30 sets it to 0x1e and counts it
                                  * down. It is used as a signed byte:
                                  * sub_08005D74 and sub_08005E30 decrement it
                                  * through an `(s8 *)` cast. Retyping it to s8
                                  * means checking sub_08007A30 again. */
    /* 0x6e */ s16 overlayX; /* an x/y pair: (0x100,0x800) by sub_08005D50,
                              * (0x780,0x970) by sub_08007A0C; sub_08007A30
                              * eases them toward a target */
    /* 0x70 */ s16 overlayY;
    /* 0x72 */ u8 armyPanelState[4]; /* one state byte per army panel (lanes 0-3,
                                      * sub_080030BC), stepped 0, 0xA, 0x14, 0x1E,
                                      * 0x32, 0x3C, 0x46, 0x50 */
    /* 0x76 */ u8 armyPanelAffineState[4]; /* a second per-lane state byte (sub_080032EC),
                                            * stepped 0, 0xA, 0x14, 0x1E */
    /* 0x7a */ s16 armyPanelTimer[4]; /* per-lane frame countdown (sub_080030BC),
                                       * seeded from the ROM table gUnknown_0808D754 */
    /* 0x82 */ s16 armyPanelX[4]; /* per-lane screen x, eased toward a target by
                                   * `v += (K - v) >> 3`; values -0x280..0x1180 */
    /* 0x8a */ s16 armyPanelScale[4]; /* per-lane sprite scale that sub_080032EC
                                       * divides gSinLut by, clamped at 0x100 */
    /* 0x92 */ s16 armyPanelAngle[4]; /* per-lane angle, counted down by 0x20 and
                                       * wrapping to 0x148; its low byte indexes
                                       * gSinLut */
    /* 0x9a */ u8 filler_9a[0x02];
    /* 0x9c */ u8 designName[0x13]; /* NUL-terminated design-room name
                                     * (sub_080048D4 copies it, sub_0800487C
                                     * compares it) */
};

/* One entry of gDesignRing, the item ring that ringIndex steps through.
 * gDesignRing is the array itself, not a pointer to one; indices 0..9 are known
 * to be used. flags is a word of bit flags (1, 8, 0x10 .. 0x100). x and y are
 * fixed point with 8 fraction bits: sub_08002C38 and sub_08002AB0 use x >> 8
 * and y >> 8. */
struct DesignRingEntry /* 0x1c */
{
    /* 0x00 */ s32 flags;
    /* 0x04 */ u16 itemId;
    /* 0x06 */ s16 spriteSlot;
    /* 0x08 */ s16 targetX; /* where x is heading: sub_08005F4C moves x toward
                             * `targetX << 8` each frame */
    /* 0x0a */ u8 filler_0a[0x02];
    /* 0x0c */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 xVelocity; /* added to x each frame; sub_08005F4C keeps it
                               * between -0x480 and 0x480 */
    /* 0x18 */ u8 filler_18[0x04];
};

extern struct DesignRingEntry gDesignRing[11];

/* ROM table of halfword pairs, 5 pairs per row, that sub_080077EC and
 * sub_080078E4 copy into gUnknown_0200B224's 4-byte entries. Must stay a flat
 * u16 array: the two readers index it in different ways (`row * 10 + col * 2`
 * and `(k * 5 + col) * 2`), and only a flat array compiles to both. */
extern const u16 gUnknown_084887AC[];

/* Four contiguous ROM tables for sub_08005F4C's OBJ-affine animation.
 * gUnknown_084886DC is the frame script (signed halfwords, -1 ends it); the
 * other three hold one 10-entry row per editMode value. Keep those three
 * two-dimensional (a flat `row * 10 + col` index compiles differently) and keep
 * gUnknown_084886F8 volatile (only a volatile read makes state 0x35 reload
 * editMode, as the original does). */
extern const s16 gUnknown_084886DC[];
extern const volatile s32 gUnknown_084886F8[][10];
extern const u8 gUnknown_08488748[][10];
extern const s32 gUnknown_0848875C[][10];

/* Two halfword scripts that sub_080078E4 walks, one for each branch on its
 * first argument, each ended by 0xFF; lengths unknown. gUnknown_08488810 holds
 * pairs: in the first halfword, bits 5-7 choose whether the pair is copied as
 * is or replaced from gUnknown_084887AC, and bits 0-4 select the case.
 * gUnknown_08488856 is a flat list of single halfwords. */
extern const u16 gUnknown_08488810[];
extern const u16 gUnknown_08488856[];

/* Two ROM blocks, one per axis, that sub_08007A30 passes to sub_0801BD00
 * together with overlayX / overlayY. Their contents and lengths are unknown; no
 * C code reads them. */
extern u8 gUnknown_08488880[];
extern u8 gUnknown_08488888[];

/* Row 1 of each of the four 2 x 16-byte tables declared below
 * (gUnknown_084888A0, C0, E0 and gUnknown_08488900). sub_0800CAA0 searches
 * the four rows side by side, one per player slot, against
 * gPlaySt.unk33[1..4]. Every byte is a slot number 1..4 doubled (2, 4, 6 or
 * 8). gUnknown_08488900 is declared here because the code reaches the fourth
 * row as gUnknown_08488900[i + 0x10]; do not add a gUnknown_08488910. */
extern const u8 gUnknown_084888B0[];
extern const u8 gUnknown_084888D0[];
extern const u8 gUnknown_084888F0[];
extern const u8 gUnknown_08488900[];

/* Four 2 x 16-byte tables, 0x20 bytes apart; gUnknown_08488900 above is the
 * fourth. sub_0800CB30 reads each as base[row * 0x10 + k], so row 1 is the
 * same bytes as gUnknown_084888B0, D0 and F0 above. */
extern const u8 gUnknown_084888A0[];
extern const u8 gUnknown_084888C0[];
extern const u8 gUnknown_084888E0[];
/* Five 4-entry u16 tables, 8 bytes apart, chosen by the low five bits of a
 * terrain cell and indexed by `(b - 2) >> 1`, where b is a byte from the
 * tables above. gUnknown_08488948 is a u8 table indexed by `(b >> 1) - 1`.
 * sub_0800CB30 reads them. */
extern const u16 gUnknown_08488920[];
extern const u16 gUnknown_08488928[];
extern const u16 gUnknown_08488930[];
extern const u16 gUnknown_08488938[];
extern const u16 gUnknown_08488940[];
extern const u8 gUnknown_08488948[];

/* Direction deltas for sub_0800F77C, indexed by direction: gUnknown_0848895C
 * is dx = {-1, 1, 0, 0} and gUnknown_08488964 is dy = {0, 0, -1, 1}. Must
 * stay non-const: sub_0800F77C reads an element again after a function call,
 * and with const the compiler would reuse the earlier value instead of
 * loading it again. */
extern s16 gUnknown_0848895C[];
extern s16 gUnknown_08488964[];
/* A second copy of the same dx/dy tables at other ROM addresses, used by
 * sub_08010664; do not merge the two pairs. Non-const for the same reason as
 * the pair above. 0x0808D89C is a compiler-made pool word holding the
 * address of gUnknown_08488974; C code names the global, never the word. */
extern s16 gUnknown_0848896C[];
extern s16 gUnknown_08488974[];

/* One entry of gUnknown_0200B224, read by sub_08001CE8, sub_08001D24 and
 * sub_08006288. unk00 is signed. */
struct Unk0200B224 /* 0x04 */
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ u16 unk02; /* sub_080077EC fills entries 9..13 with halfword
                           * pairs from gUnknown_084887AC (unk00, unk02).
                           * Nothing reads it yet, so the sign is unknown. */
};

/* One of the 48 entries of gUnknown_0200B3B4. sub_08011C18 clears every
 * member of every entry and is the only reader so far. Keep unk08 and unk0a
 * volatile: before each store to them the original makes an extra load whose
 * result is unused, and only a volatile byte or halfword member gets that. */
struct Unk0200B3B4 /* 0x0c */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ volatile u16 unk08;
    /* 0x0a */ volatile u8 unk0a;
    /* 0x0b */ u8 filler_0b[0x01];
};

struct Unk0200F720 /* 0x10 */
{
    /* 0x00 */ u16 unk00; /* unk00..unk04: sub_0801E22C writes all three,
                           * sub_0801E248 the first two and sub_0801E264 only
                           * unk04. Nothing reads them, so the sign is
                           * unknown. */
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u16 unk0c; /* 0x0c/0x0e set together by sub_0801E294 */
    /* 0x0e */ u16 unk0e;
};

struct Unk0200F920Entry
{
    u16 unk00;
    u16 unk02;
};

/* One element of gUnknown_0200F920. sub_0801F150 fills its header: unk00,
 * unk04 (its fourth argument), unk05 = 0 and unk08[0].unk00. No matched
 * function touches bytes 6 and 7. */
struct Unk0200F920 /* 0x88 */
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 filler_06[0x02];
    /* 0x08 */ struct Unk0200F920Entry unk08[32];
};

/* gUnknown_02027F74. unk36 and unk37 are a cursor and its inclusive upper
 * bound, reset together to (0, 0xff) by sub_080374F0 and sub_08037508 and to
 * (0, 0x6a) by sub_0803753C and sub_08037570. */
struct Unk02027F74 /* >= 0x38 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 unk04[0x32]; /* A byte list that sub_08087104 reads
                                * (through a byte pointer to the struct plus
                                * 4, not by this member name). The 0x32
                                * length is only the room before unk36, not a
                                * known bound. */
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
};

/* gUnknown_02027FB0: 16 slots of 8 bytes, walked by sub_0803C750. unk00 is a
 * callback that sub_0803C784 copies from gUnknown_0849EDB0[].unk08. unk04 is
 * a small unsigned use count; sub_0803C750 skips a slot whose count is above
 * 1. */
struct Unk02027FB0 /* 0x08 */
{
    /* 0x00 */ int (*unk00)(int);
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 filler_05[0x03];
};

struct Unk02028030 /* 0x48 */
{
    /* 0x00 */ u8 unk00[8];
    /* 0x08 */ u8 unk08[8];
    /* 0x10 */ u8 unk10[2];
    /* 0x12 */ u8 unk12[0x18];
    /* 0x2a */ u8 unk2a[3];
    /* 0x2d */ u8 unk2d[3];
    /* 0x30 */ u8 unk30[0x18];
};

struct Unk020280C0 /* 0x1c */
{
    /* 0x00 */ u8 filler_00[0x02];
    /* 0x02 */ u8 unk02[0x11]; /* a NUL-terminated name; sub_0803CCB8 copies
                                * it out with sub_0803CC84. The 0x11 length
                                * is only the room before unk13. */
    /* 0x13 */ u8 unk13; /* 0xff = empty slot. sub_0803CCB8 does not copy the
                          * name of an empty slot, sub_0803CCEC returns the
                          * fallback string gUnknown_0849F320 for one, and
                          * sub_0803CD14 returns this byte. */
    /* 0x14 */ u8 filler_14[0x08];
};

struct Unk02029A10 /* 0x24 */
{
    /* 0x00 */ u8 unk00; /* sub_08051F4C sets it to 1 when unk01 is 1. */
    /* 0x01 */ u8 unk01; /* a three-way state: sub_08051F4C branches on == 1,
                          * then on == 0, and does nothing for anything else. */
    /* 0x02 */ u8 unk02; /* zeroed by sub_08057164 together with unk00,
                          * unk01, unk04 and unk06. */
    /* 0x03 */ u8 filler_03[0x01];
    /* 0x04 */ u16 unk04; /* unk04/unk06: start position. sub_0804D290 and
                           * sub_0804DCA8 fill them from a ROM record and
                           * copy them straight into x/y. */
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 x;  /* Keep u16 although sub_0804FA2C loads it signed:
                        * passing a u16 to an s16 parameter compiles to that
                        * same signed load. */
    /* 0x0a */ u16 y;
    /* 0x0c */ u16 xSub;
    /* 0x0e */ u16 ySub;
    /* 0x10 */ u16 xStep;
    /* 0x12 */ u16 yStep;
    /* 0x14 */ u16 frame;
    /* 0x16 */ u16 frameCount;
    /* 0x18 */ s16 unk18; /* a proc id, -1 = none. sub_08053860 and
                           * sub_08053BB8 pass it to sub_08053614, which
                           * checks for -1. */
    /* 0x1a */ u16 unk1a; /* 0 = entry idle: sub_080513FC and sub_08051920
                           * test it before setting bit 6 of
                           * gUnknown_02029664. Matched code never writes it;
                           * sign unknown. */
    /* 0x1c */ u16 unk1c; /* a flag: sub_08052818 sets it to 1, and
                           * sub_08052650 and sub_08052AF4 call sub_08052E04
                           * when it is 1. */
    /* 0x1e */ u16 unk1e; /* zeroed by sub_0804D290 and sub_0804DCA8. */
    /* 0x20 */ u16 unk20; /* an accumulator whose low 4 bits are a phase:
                           * sub_0804CA98 adds gUnknown_02029B94[side][slot]
                           * to it, acts when (unk20 & 0xf) == 1, and stores
                           * 0xff to stop the sequence. Sign unknown. */
    /* 0x22 */ u16 unk22; /* step counter: sub_0804CA98 increments it,
                           * compares it with 3, and uses it as the index of
                           * the entry in this group whose x/y it writes. */
};

/* One entry of gUnknown_02029690: the animation driver for one
 * gUnknown_02029A10 group. sub_0804B330 steps unk0e and reads two rows of
 * unk04's table; sub_0804B2A8 checks unk08 and unk00; sub_0804BA64 writes
 * unk00 and unk0a. unk04 points at a u16 table with x values in one row and
 * y values 0x28 entries later. All members are u16 by width; the sign is
 * unknown. */
struct Unk02029690 /* 0x10 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 filler_02[0x02];
    /* 0x04 */ u16 *unk04;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u16 unk0c; /* zeroed with the other members by sub_0804B8BC's
                           * reset loop. */
    /* 0x0e */ u16 unk0e;
};

/* A request to set up one gUnknown_02029A10 entry: sub_0804BCB8 builds it on
 * its stack and sub_08056E28 applies it. unk00 is the group and unk02 the
 * entry; unk04..unk0c are copied into that entry's xSub, xStep, ySub, yStep
 * and frameCount. */
struct Unk56E28
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u16 unk0c;
};

struct Unk02029A10Group /* 0xb4 */
{
    /* 0x00 */ struct Unk02029A10 entries[5];
};

/* gUnknown_02029BA8: an array of 0x20-byte records of word members; the
 * length is unknown. unk04..unk14 are passed to sub_080156E8, which uses
 * them as small table indices, not addresses. Do not narrow them: every
 * caller loads the whole word, and a u16 member would load only a halfword.
 * unk18 is a two-entry array (sub_0804EAEC and sub_08050424 index it). */
struct Unk02029BA8 /* 0x20 */
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
    /* 0x08 */ void *unk08;
    /* 0x0c */ void *unk0c;
    /* 0x10 */ void *unk10;
    /* 0x14 */ void *unk14;
    /* 0x18 */ void *unk18[2];
};

/* This note is about struct Unk0202FDFC, defined after struct Unk0202FDEC
 * below. gUnknown_0202FDFC is one record, not three globals: the addresses
 * 0x0202FE0E and 0x0202FE38 are its members unk12 and unk3c. unk10 is
 * cleared by sub_08074AAC before it starts its proc. unk12 has 42 entries,
 * scanned by sub_08076858 and cleared by sub_080745C0. unk3c is signed, -1 =
 * unset (sub_080745C0 resets it to -1; sub_08074670 tests for it). The size
 * past 0x3e is unknown. */
/* gUnknown_0202FDEC, filled by sub_08038240. At most 0x10 bytes, because
 * gUnknown_0202FDFC follows it. unk00 and unk04 are two 4-entry byte lists
 * that split armies 1..4 by sub_080266DC's test; unk08 and unk09 are their
 * counts. unk0a holds gPlayers[..].unk38, sometimes doubled; unk0c holds the
 * low half of gUnknown_0200C420.unk00. Signs unknown; bytes 0x0e and 0x0f
 * are unused. */
struct Unk0202FDEC /* >= 0x0e */
{
    /* 0x00 */ u8 unk00[4];
    /* 0x04 */ u8 unk04[4];
    /* 0x08 */ u8 unk08;
    /* 0x09 */ u8 unk09;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u16 unk0c;
};

struct Unk0202FDFC /* >= 0x3e */
{
    /* 0x00 */ s16 unk00;      /* unk00/unk02: the camera origin (scroll
                                * offset). sub_080748A0 subtracts it from an
                                * object's position to get its screen
                                * position. */
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;      /* unk04/unk06: a second x/y offset.
                                * sub_0807610C adds it to unk00/unk02 to get
                                * one position, and sub_08075EC4 reads it
                                * signed. */
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s16 unk08;      /* zeroed by sub_08076A68. Nothing reads it,
                                * so the sign is unknown. */
    /* 0x0a */ u8 filler_0a[0x02];
    /* 0x0c */ s32 unk0c;      /* base index into gUnknown_0861500C's 8-byte
                                * records; sub_0807831C adds a proc's counter
                                * to it. Sign unknown. */
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;       /* a two-valued mode flag read by
                                * sub_08076B7C, sub_08076BF0 and
                                * sub_08076C1C. */
    /* 0x12 */ u8 unk12[0x2a]; /* 0x0202FE0E in the disassembly is this
                                * array, not a separate global. sub_08076B20
                                * clears bit 0 of an entry. */
    /* 0x3c */ s16 unk3c;
};

/* 0x081CC590, 0x081CC594 and 0x081CC59C are not globals: they are
 * compiler-made pool words holding the address of gUnknown_0202FDFC, and
 * 0x081CC598 holds &gpKeySt. C code names gUnknown_0202FDFC, never the
 * words. sub_08076CAC and sub_08076D68, which use them, clamp that struct's
 * camera X pair (unk04 against unk00) and Y pair (unk06 against unk02). */

/* gSmoothScroll: the smooth-scroll state sub_08076E20 advances each frame.
 * The target is set from gUnknown_08614588 (doubled), and the current
 * position moves half the remaining distance toward it. frameCounter must
 * stay unsigned: sub_08076E20's modulo tests on it use the unsigned division
 * helper. */
struct SmoothScrollState /* 0x0c */
{
    /* 0x00 */ s16 targetX;
    /* 0x02 */ s16 targetY;
    /* 0x04 */ s16 currentX;
    /* 0x06 */ s16 currentY;
    /* 0x08 */ u32 frameCounter;
};

extern struct SmoothScrollState gSmoothScroll;

/* s16 (x, y) pairs picked by bits 4..7 of sub_08076E20's argument. Must stay
 * a two-dimensional array: a struct of two s16 or a flat s16 array makes the
 * compiler compute the second element's address differently. */
extern s16 gUnknown_08614588[][2];

/* Signed bytes indexed by sub_080761C8's step counter (+0x40, 0..3); used as
 * a halfword index and as sub_08071900's third argument. */
extern s8 gUnknown_08614458[];

/* Passed by sub_080772B8 to sub_080733C8 and used nowhere else, so its
 * element type and length are unknown; `u8 []` is a placeholder. */
extern u8 gUnknown_086145C8[];

/* Four 5-entry byte tables, indexed by the step counter (+0x44, 0..4) of the
 * animation procs at 0x08077xxx. gUnknown_086145D8 pairs with DD
 * (sub_08077690; sub_08077DF0 uses D8 alone) and E2 with E7 (sub_08077870;
 * sub_08077954 uses E2 alone). D8 and E2 are sub_08071900's third argument
 * and a halfword index into the two frame buffers; DD and E7 are a scroll
 * position, stored in gUnknown_0300064C and passed to sub_08077620
 * subtracted from 0xA8. Keep the two types: the original loads D8 and E2 as
 * signed bytes, but DD and E7 as unsigned bytes that it then sign-extends,
 * which is a u8 table read through an (s8) cast. */
extern s8 gUnknown_086145D8[];
extern u8 gUnknown_086145DD[];
extern s8 gUnknown_086145E2[];
extern u8 gUnknown_086145E7[];

/* The proc script sub_08075E68 starts as a blocking child of its caller's
 * proc; it then sets the new proc's +0x58, +0x2c, +0x30 and +0x54. */
extern const struct ProcCmd gUnknown_08614410[];

/* Pending OAM transfer descriptor. sub_0801BB88 splits the OAM shadow at an
 * object index, filling one descriptor for each side of the split.
 */
struct OamTransfer /* 0x0c */
{
    /* 0x00 */ void *src;
    /* 0x04 */ void *dst;
    /* 0x08 */ u16 oamOffset;
    /* 0x0a */ u16 objectCount;
};

/* gUnknown_03000288: 16 entries of 0x2c bytes. sub_0801C6E8 searches unk00
 * for a key and returns the entry; sub_0801C1F8 clears every unk00. */
struct Unk03000288 /* 0x2c */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u8 filler_04[0x28];
};

/* gUnknown_03001470[]: 0x60-byte slots, handed out by sub_080152EC and
 * sub_08014740. unk04 is a cursor into a script of 8-byte commands: a word
 * at +0 (a value, or a jump target) and an unsigned halfword at +4. Command
 * handlers copy the word into unk08 or unk0c, or the halfword into unk10,
 * and advance unk04 by 8; the jump handlers (sub_08015E04, sub_08015E2C)
 * load the word into unk04 instead. unk04 is `const void *`, cast at each
 * use, because the +0 word is a number for some commands and an address for
 * others.
 *
 * The members below are only one view of the slot. Other code uses the same
 * bytes differently: sub_080048D4 writes a 19-byte name across 0x1e..0x30,
 * sub_08019A60 has its own struct Unk8019A60, and the path mover described
 * at the end of the struct uses 0x3c..0x5f as floats. Do not change a member
 * to fit a new function; give that function its own view struct, as
 * c_080048D4.c does. */
struct Unk03001470 /* 0x60 */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ const void *unk04;
    /* 0x08 */ u32 unk08;
    /* 0x0c */ u32 unk0c;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12; /* flag bits: sub_0801527C stores 4, sub_08015224
                           * stores 0 and sub_08015438 sets bit 1. Sign
                           * unknown. */
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 filler_15[0x03];
    /* 0x18 */ int unk18; /* sub_08037610 stores a value that sub_08037638
                           * computes as base + ((n & 0x3ff) << 5), so
                           * probably an address. */
    /* 0x1c */ u16 unk1c; /* sub_08066580 stores its slot index here (the
                           * index it also uses for
                           * gUnknown_08580934->unk74[] and unk70[]). Nothing
                           * reads it yet. */
    /* 0x1e */ s16 unk1e; /* frame counter: sub_0803B118 increments it each
                           * call and gives up at 90. Must stay s16: that
                           * function compares the new value as signed. */
    /* 0x20 */ s16 unk20; /* stored by sub_08035144 and sub_08042B84;
                           * sub_0802966C reads it as a signed index into
                           * gUnknown_0849A06C. */
    /* 0x22 */ s16 unk22; /* sub_0802966C reads it as a signed index into
                           * gUnknown_030040D8->unk07[]. */
    /* 0x24 */ s16 unk24; /* sub_0802966C stores 1 here when sub_0802E7C8
                           * returns 1; sub_080277BC reads it as a signed
                           * index into gUnknown_08090AA8. */
    /* 0x26 */ u16 unk26;
    /* 0x28 */ u32 unk28; /* unk28..unk34: four words. sub_0804D928 and
                           * sub_0804E3B4 zero unk28/unk2c and set
                           * unk30/unk34 from the u16 globals
                           * gUnknown_0300453C and gUnknown_0300451C. */
    /* 0x2c */ int unk2c; /* tick counter: sub_08050958 increments it and
                           * compares it with gUnknown_02029710[side].unk16.
                           * Must stay signed: that compare is signed in the
                           * original, and an unsigned member makes it
                           * unsigned. */
    /* 0x30 */ u32 unk30;
    /* 0x34 */ u32 unk34;
    /* 0x38 */ u16 unk38; /* probably s16 in the original: sub_0802A6B0 and
                           * the path mover (end of this struct) read it
                           * signed, and their C casts it to s16. Left u16
                           * because other matched files use it and the
                           * change has not been checked against them. */
    /* 0x3a */ u8 unk3a; /* sub_0808A6A0 sets it to 6 on the slot
                          * sub_08014740 returns. Nothing reads it yet. */
    /* 0x3b */ u8 filler_3b[0x01];
    /* 0x3c */ void (*unk3c)(void); /* a callback: sub_08018758 stores
                                     * sub_080185A0 here. That is the only
                                     * access, so the type comes from that
                                     * one store. */
    /* 0x40 */ u8 filler_40[0x02];
    /* 0x42 */ u8 unk42; /* sub_0801A614 copies unk42, unk48, unk4a and unk4c
                          * together into one gUnknown_03002F50 stack entry.
                          * Signs unknown. */
    /* 0x43 */ u8 filler_43[0x01];
    /* 0x44 */ u16 unk44; /* sub_080656E0 and sub_0806D820 set it to 0xe on
                           * the slot sub_080152EC returns. */
    /* 0x46 */ u8 filler_46[0x02];
    /* 0x48 */ u16 unk48; /* sub_0802D33C compares it with 0xf (on the slot
                           * sub_080637AC returns) and picks 1 or 0x10. Sign
                           * unknown. */
    /* 0x4a */ u16 unk4a; /* see unk42 */
    /* 0x4c */ u8 unk4c;
    /* 0x4d */ u8 filler_4d[0x13];
    /* Bytes 0x3c..0x5f are also used by the float path mover (sub_080161B4,
     * sub_080162A4, sub_08016370, sub_0801642C and sub_080164E0) as:
     *     0x3c float x       0x40 float y
     *     0x4c float dx      0x50 float dy
     *     0x54 float ddx     0x58 float ddy
     *     0x5c int   frames_remaining
     * That view overlaps unk3c..unk4c above; the original probably had a
     * union. It is not modelled here, so the member names above stay valid;
     * those functions use their own struct Unk1470Path (c_080161B4.c). unk38
     * is the mover's tick. */
};

/* Defined here, out of address order, because struct Unk030020A8 below
 * points to it. A list node: the ROM word gUnknown_0808E5C8 points at an
 * array of them, and sub_0801A6C0 clears unk00 of entries 0..0x80. unk04 is
 * the next link: sub_0801A700 pops the head of the list at
 * gUnknown_030020A8.unk04 and returns its unk00. unk08 is the signed sort
 * key sub_0801A718 inserts by. Bytes 0x0a and 0x0b are never accessed. */
struct Unk0808E5C8 /* 0x0c */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ struct Unk0808E5C8 *unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0a */ u8 filler_0a[0x02];
};

/* gUnknown_030020A8, exactly 8 bytes (gUnknown_030020B0 follows).
 * sub_0801A6C0 initialises it; unk04 is the head of the Unk0808E5C8 list
 * that sub_0801A700 pops. */
struct Unk030020A8 /* 0x08 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 filler_02[0x02];
    /* 0x04 */ struct Unk0808E5C8 *unk04;
};

struct Unk03002040 /* 0x58 */
{
    /* 0x00 */ u8 filler_00[0x50];
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u16 unk54;
    /* 0x56 */ u16 unk56;
};

struct Unk03002B80 /* 0x35a */
{
    /* 0x000 */ u8 unk00;
    /* 0x001 */ u8 filler_01[0x357];
    /* 0x358 */ u16 unk358;
};

/* gUnknown_03002F50[]: a stack of 8-byte entries, with gUnknown_03002F24 as
 * its cursor. sub_0801A604 resets it, sub_0801A614 pushes an entry (four
 * signed bytes and a word) and sub_0801A664 pops one. */
struct Unk03002F50 /* 0x08 */
{
    /* 0x00 */ s8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ s8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ u32 unk04;
};

/* The three one-byte locks at 0x030030F0 are declared in include/lock.h as
 * `struct GameLock gGameLock`. Accessors: sub_08034F48..sub_08034F60
 * (unitSelection), sub_08034F6C..sub_08034FA4 (map) and
 * sub_0803BD54..sub_0803BD6C (mainMenu). */

/* gUnknown_03002F08, exactly 8 bytes: sub_080171B4 and sub_08017540 copy it
 * whole to and from a caller struct at +0xB98. unk00 is set to 0, 8 or 0xf
 * (sub_08074384, sub_080743B8, sub_08078468); unk02 is read by sub_08017FA8
 * and sub_080180DC. */
struct Unk03002F08 /* 0x08 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x04];
};

struct Unk03003130 /* 0x12 */
{
    /* 0x00 */ u8 unk00; /* a mode: sub_080276D0 sets 0 and sub_080276F0 sets
                          * 1, each then setting unk04 from
                          * gUnknown_08090A98[mode]. */
    /* 0x01 */ u8 filler_01[0x03];
    /* 0x04 */ int unk04; /* gUnknown_08090A98[unk00], a signed halfword
                           * widened to a word. */
    /* 0x08 */ u8 unk08; /* sub_0802BB98 clears it on the path where it does
                          * not set unk00 to 1. */
    /* 0x09 */ u8 filler_09[0x03];
    /* 0x0c */ int unk0c; /* sub_0802BB98 sets it to 0x94 or 3, on the two
                           * sides of that same test. */
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
};

/* A 2 x u16 pair used for several independent cursor/position globals.
 * gUnknown_03003F24 and gUnknown_03003100 are also compared as a whole word,
 * so those two are declared through the union below.
 */
struct Unk802C57C
{
    u16 unk00;
    u16 unk02;
};

/* The signed view of the same pair, reached through union Unk802C57CBuf.
 * sub_0802CBA0, sub_0802CBC8 and sub_0802CC04 read gUnknown_03003100's
 * halves as signed halfwords and need this view; other code reads them
 * unsigned through `pos`. Do not make struct Unk802C57C signed instead: many
 * promoted files read it unsigned. */
struct Unk802C57CS
{
    s16 unk00;
    s16 unk02;
};

union Unk802C57CBuf
{
    struct Unk802C57C pos;
    struct Unk802C57CS spos;
    u32 raw;
};

/* A four-byte link packet: type, slot, halfword value. sub_080320CC fills
 * gUnknown_0202575C with 0xa8, gUnknown_0849B018->unk06 and 0, and passes it
 * to sub_0802F588; sub_080309AC sends type 0xAE the same way. */
struct Unk0202575C /* 0x04 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
};

/* One of the 12 records in gUnknown_02025564.unk20[], 0x1c bytes each.
 * sub_0802F28C resets them; nothing reads them yet, so the field sizes are
 * only what that reset writes. Not volatile, unlike the parent's unk00,
 * unk02 and unk05: the reset does not read these bytes before writing them. */
struct Unk02025584 /* 0x1c */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02[0x11];
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14[0x05];
    /* 0x19 */ u8 unk19;
    /* 0x1a */ u8 unk1a;
    /* 0x1b */ u8 filler_1b[0x01];
};

/* gUnknown_02025564 is one object. The linker symbol list also names
 * gUnknown_02025566 and gUnknown_02025569, but those are only the addresses
 * of unk02 and unk05; declaring them as separate globals breaks
 * sub_0802F28C, which reaches unk20 from the same base address as unk00.
 * unk00, unk02 and unk05 must stay volatile: the original reads each byte
 * before writing it, and the compiler emits that read only for a volatile
 * member. */
struct Unk02025564
{
    /* 0x00 */ volatile u8 unk00[0x02];
    /* 0x02 */ volatile u8 unk02[0x03];
    /* 0x05 */ volatile u8 unk05[0x18];
    /* 0x1d */ u8 filler_1d[0x03];
    /* 0x20 */ struct Unk02025584 unk20[0x0c];
};

/* Play settings, gPlaySt at 0x03003FC0. Field names come from SRR_AW2's
 * `struct playSt`. Checked against this game's code: gameMode, mapID,
 * dispMiniPanel, coPowersEnabled, coAbilities, propertyFunds, weather,
 * armyColor, aiControlled and co. Not yet checked, so treat as guesses:
 * event20, campaignRelated, animOpts, bgmOn, fog, randomWeatherOn,
 * defaultWeather, turnLimit, captureLimit and savingEnabled. The per-slot
 * arrays are indexed by army slot 1..4. SRR's mapID is a u16; only its low
 * byte is declared here. */
struct PlaySt /* 0x48 */
{
    /* 0x00 */ u8 unk00; /* set to 1 by sub_0803BDBC; purpose unknown */
    /* 0x01 */ u8 gameMode; /* 1, 2 or 3: sub_0803BADC, sub_0803BA00 and
                             * sub_0803B8C4 each set their mode just before
                             * starting its process */
    /* 0x02 */ u8 mapID; /* set by sub_0803BCD0; sub_0803BD14 uses it to index
                          * gUnknown_085C77A0[] */
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 event20; /* bit mask: sub_08028944, sub_08028990 and
                            * sub_080289FC test bits 0, 1, 2 and 4 when
                            * deciding a win or loss; sub_08018C54 writes it */
    /* 0x05 */ u8 dispMiniPanel; /* 0/1 flag: sub_08018800 clears it and
                                  * sub_080187C8 sets it; no reader is known */
    /* 0x06 */ u8 campaignRelated;
    /* 0x07 */ u8 coPowersEnabled; /* nonzero allows CO Powers; sub_08019888
                                    * and sub_08019894 set and clear it */
    /* 0x08 */ u8 coAbilities; /* gates the CO bonus tables:
                                * sub_08042E2C..sub_0804301C return a fixed
                                * fallback when it is zero */
    /* 0x09 */ u8 animOpts; /* 0..3; sub_0802C6CC, sub_0802C6FC, sub_0802C72C
                             * and sub_0802C75C each test for one value. SRR
                             * gives 0 = off, 1-3 = modes A-C. */
    /* 0x0a */ u8 filler_0a[0x02];
    /* 0x0c */ u8 bgmOn; /* sub_0802C78C compares it with 1 and sub_0802C7A0
                          * with 0 */
    /* 0x0d */ u8 fog; /* sub_08042998 runs one block only when this and
                        * savingEnabled are both 0 */
    /* 0x0e */ u8 filler_0e[0x02];
    /* unk10[1..4] is one word per army slot (element 0 is never used),
     * written in a loop by sub_0803BE60. unk24 is a separate word.
     * sub_08034780 clears all five. Sign unknown: nothing reads them. */
    /* 0x10 */ u32 unk10[0x05];
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u32 propertyFunds; /* sub_0803BD78 sets it to 1000; sign
                                   * unknown */
    /* 0x2c */ u8 weather; /* 1 = snow: sub_08035CF4 returns whether it
                            * equals 1 */
    /* 0x2d */ u8 randomWeatherOn; /* set to 0/1/2 by sub_08035558/sub_08035538/sub_08035548 */
    /* 0x2e */ u8 unk2e; /* sub_0802CF6C passes it to sub_080344F0 when
                          * savingEnabled is set */
    /* 0x2f */ u8 defaultWeather;
    /* 0x30 */ u8 turnLimit;
    /* 0x31 */ u8 captureLimit;
    /* 0x32 */ u8 savingEnabled;
    /* armyColor, aiControlled, co and unk42 are four parallel 5-byte arrays
     * indexed by army slot 1..4; element 0 is never used. sub_0803C1D4 fills
     * all four in one loop from the gUnknown_08580934 tables. */
    /* 0x33 */ u8 armyColor[0x05];
    /* 0x38 */ u8 aiControlled[0x05];
    /* 0x3d */ u8 co[0x05]; /* the CO in each army slot */
    /* 0x42 */ u8 unk42[0x05];
    /* 0x47 */ u8 unk47; /* flag: sub_08029CB8 stores either 0 or its u8
                          * argument depending on it */
};

struct Unk030040D8
{
    /* 0x00 */ u8 unk00; /* small id: sub_0802CC90 compares it with 0x18, and
                          * sub_080421D0 and sub_0804223C use it to pick a
                          * 0x5c-byte record of gUnknown_085D5ABC */
    /* 0x01 */ u8 unk01;  /* bit 0 tested by sub_0802E4B4. This struct is
                           * struct Unit under another name: sub_0802E4B4
                           * points gUnknown_030040D8 into gUnknown_08499594.
                           * They are not merged because c_080424BC.c's
                           * `->unk05 &= 7` would have to be rewritten for
                           * struct Unit's bitfields. */
    /* 0x02 */ u8 unk02; /* unk02 and unk03 are the map position:
                          * sub_080425B8 copies gUnknown_03003100's two
                          * halves into them */
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 unk04 : 7; /* must stay a 7-bit bitfield: sub_08042650 both
                              * tests it and uses its value, and only a
                              * bitfield makes the compiler read it the two
                              * different ways the original does. Unsigned. */
    /* 0x04 */ u8 unk04_7 : 1;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06 : 7;  /* a 7-bit unsigned bitfield for the same
                               * reason as unk04: sub_0805D648 uses its value */
               u8 unk06_7 : 1; /* bit 7 of the byte; purpose unknown */
    /* 0x07 */ u8 unk07[0x05]; /* byte 2 (offset 0x09) is really a bitfield
                                * container: sub_0805BFDC writes its bits
                                * 3..5 through a file-local bitfield view.
                                * Kept as an array because promoted code
                                * reads unk07[0], [1] and [4]; if you split
                                * it, re-verify c_0805B980.c and
                                * c_0805BC7C.c. */
};

/* Entries of gUnknown_02028360[] and gUnknown_020283E0[], 8 bytes each. The
 * size must stay 8: sub_0803F5D4 turns an entry pointer back into an index.
 * unk00 and unk01 are the entry's tile x and y; sub_0803DF54 skips entries
 * whose unk04 is 0. sub_0803E01C fills a new entry. unk02_6 is compared with
 * 1 and 5, and sub_0803DE68 clears it. unk02 must stay bitfields: the
 * original extracts them with shifts, and masking a plain u16 compiles
 * differently. */
struct Unk02028360 /* 0x08 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02_0 : 3;
               u16 unk02_3 : 3;
               u16 unk02_6 : 4;
               /* sub_0803EB40 copies unk02_a into unk06. */
               u16 unk02_a : 4;
               u16 unk02_e : 2;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 filler_05[0x01];
    /* 0x06 */ s8 unk06; /* a counter that sub_0803EAD0 decrements; sign
                          * unknown (c_0803E088.c's own view of this record
                          * declares it u8) */
    /* 0x07 */ s8 unk07; /* must stay s8: sub_0803ED60 passes it to
                          * sub_0803E594, sub_0803E6C4 and sub_0803E808, and
                          * one of those calls needs a sign-extending load
                          * that a cast on a u8 cannot produce */
};

/* Entries of gUnknown_0849F688 and gUnknown_0849F698, two ROM tables
 * declared below. sub_0803E7C0 and sub_0803E7E4 pick gUnknown_0849F698 when
 * their first argument is 4 and gUnknown_0849F688 otherwise, index it with
 * their second argument, and return unk00 or unk04. `void *` is unconfirmed:
 * the words may be integers. */
struct Unk0849F688 /* 0x08 */
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
};
/* gUnknown_08090AA8, declared below: ten u16 sprite-scale steps (0x10, 0x20,
 * 0x38, 0x58, 0x80, 0x110, 0x107, 0x103, 0x100, 0) that sub_080277BC steps
 * through by the frame counter gUnknown_03001470[gUnknown_03001FBC].unk24.
 * Must stay u16: one read is compared with 0x100, and with s16 the compiler
 * would load it sign-extended, which the original does not. */
/* Compared by sub_080253B0 with the top three bits (the owning army, `&
 * 0xe0`) of the terrain byte at gUnknown_08499590 + 0x1432. Purpose
 * otherwise unknown. */
extern u16 gUnknown_03004084;
/* A 0/1 flag; sub_08029FE4 passes `1 - value` to sub_08029AF8. Sign unknown. */
extern u8 gUnknown_03004007;
extern const u16 gUnknown_08090AA8[];
/* gUnknown_08499DE8 and gUnknown_08499DDC, declared below: the graphics pair
 * sub_08027A50 passes as sub_08015438's first and third arguments. No C code
 * reads their contents; they are non-const because sub_08015438 takes plain
 * `void *`. */
/* One ROM byte per unit type (indexed by struct Unit.unk00): a terrain code
 * in 0..0x1f, which sub_080253B0 compares with the low five bits of a
 * terrain byte. */
extern const u8 gUnknown_084995DA[];
/* 16-entry ROM byte table indexed by `(gGameClock >> 2) & 0xf`; sub_080246B4
 * switches on the value (only 1 and 2 are handled). A real array, not a
 * compiler-made pool word like some symbols near it. */
extern const u8 gUnknown_08499CBC[];
extern u8 gUnknown_08499DDC[];
extern u8 gUnknown_08499DE8[];
extern const struct Unk0849F688 gUnknown_0849F688[];
extern const struct Unk0849F688 gUnknown_0849F698[];
/* The ROM tables declared below, used by the struct Unk02028360 drawing
 * code.
 *
 * gUnknown_0849F990..gUnknown_0849F9E0: eleven 8-byte blobs; sub_0803F990
 * passes the one for a terrain code to sub_0803F908's third parameter.
 * gUnknown_0849FAB0 and gUnknown_0849FAC4: tables of pointers to such blobs,
 * indexed by the owning army (terrain byte >> 5); sub_0803F990 also uses
 * gUnknown_0849FAB0[0] directly as a fallback.
 *
 * gUnknown_0849FA08..gUnknown_0849FA9A: seven blobs of different lengths;
 * sub_0803FC28 picks one from the record's unk02_6, unk04, unk02_e and
 * IsHardCampaignMode() and passes it to sub_0803F908. They are never indexed, so
 * `const u8 []` is a placeholder.
 *
 * gUnknown_0849F6B8: 4-byte entries selected by the same 3-or-4 argument
 * sub_0803E7C0 switches on; only the first halfword is read, so `[][2]` is a
 * placeholder.
 *
 * gUnknown_0849F728: two pointers to 0xFFFF-terminated {x, y} lists, chosen
 * by `gUnknown_03004080 & 1`; sub_0803ED60 walks one with sub_0803E764. */
extern const u8 gUnknown_0849F990[];
extern const u8 gUnknown_0849F998[];
extern const u8 gUnknown_0849F9A0[];
extern const u8 gUnknown_0849F9A8[];
extern const u8 gUnknown_0849F9B0[];
extern const u8 gUnknown_0849F9B8[];
extern const u8 gUnknown_0849F9C0[];
extern const u8 gUnknown_0849F9C8[];
extern const u8 gUnknown_0849F9D0[];
extern const u8 gUnknown_0849F9D8[];
extern const u8 gUnknown_0849F9E0[];
extern const u8 *const gUnknown_0849FAB0[];
extern const u8 *const gUnknown_0849FAC4[];
extern const u8 gUnknown_0849FA08[];
extern const u8 gUnknown_0849FA22[];
extern const u8 gUnknown_0849FA3C[];
extern const u8 gUnknown_0849FA56[];
extern const u8 gUnknown_0849FA5E[];
extern const u8 gUnknown_0849FA78[];
extern const u8 gUnknown_0849FA9A[];
extern const u16 gUnknown_0849F6B8[][2];
extern struct Unk02028360Pos *const gUnknown_0849F728[];
/* A tile position {x, y}: the out-parameter sub_0803DFE0 fills and
 * sub_0803DF54 reads, and the element of the lists gUnknown_0849F728 points
 * to. Must stay a struct, not `u16 [2]`: with an array the compiler
 * recomputes the address of the second half. */
struct Unk02028360Pos
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
};

/* 0x03003338 points at ROM (set from gUnknown_0849FE74[0] by sub_0803486C).
 * Stride 8, four u16 written together by sub_080413B4.
 */
struct Unk03003338 /* 0x08 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
};

/* The record gUnknown_030044E0 points to; sub_0804B0B4 allocates 0x6c bytes
 * for it. sub_0804B0CC and sub_0804B10C fill unk58..unk60 the same way,
 * except that one sets unk5c to 0 and the other to 1; unk5f and unk60 are
 * unk5e times 2 and times 8. The length of unk2c is a placeholder. */
struct Unk030044E0 /* >= 0x6c */
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e; /* sub_0804A64C passes `unk20 * 15 + unk1e` to
                          * sub_0804A18C */
    /* 0x1f */ u8 filler_1f[0x01];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22; /* two animation clocks, unk22/unk24 and
                           * unk26/unk28: a frame counter and a delay.
                           * sub_0804ABDC counts the delay down to 0, then
                           * advances the counter (back to 0 above 0x78) and
                           * uses `(counter / 4) & 0xf` as the frame into
                           * gUnknown_0813204C, gUnknown_08131D8C and
                           * gUnknown_08131DEC. */
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2a */ s16 unk2a; /* a screen Y: sub_0804AFCC lowers it by 4 while it
                           * is above 0xa0. Must stay s16: the original
                           * compares it with a sign-extending load. */
    /* 0x2c */ u8 unk2c[0x2c];
    /* 0x58 */ int unk58;
    /* 0x5c */ u8 unk5c;
    /* 0x5d */ u8 unk5d;
    /* 0x5e */ u8 unk5e;
    /* 0x5f */ u8 unk5f;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61; /* tile/palette index that sub_0804A1E4 passes to
                          * sub_08012BC8 and sub_080149C0; sign unknown */
    /* 0x62 */ u8 unk62; /* counter: sub_0804AE20 increments it when
                          * sub_08019260 returns 0 */
    /* 0x63 */ u8 unk63; /* sub_0804A6D8 sets it to 1 or 2 depending on
                          * whether any live entry is left; nothing reads it */
    /* 0x64 */ u8 unk64; /* cleared by sub_0804A260; nothing reads it */
    /* 0x65 */ u8 unk65; /* scratch: sub_0804A1E4 stores sub_08014CEC's
                          * result here and, when it is nonzero, subtracts
                          * its own u8 argument from it */
    /* 0x66 */ u8 unk66; /* flag: sub_0804A18C picks one of four ROM byte
                          * tables from unk5c and unk66. Bit 7 is a separate
                          * flag that sub_0804AAF8 tests as `(s8)unk66 < 0`;
                          * keep the field u8 and the cast, since an s8 field
                          * changes that function's loads. */
    /* 0x67 */ u8 unk67; /* step counter 0..5: sub_0804AAF8 switches on it,
                          * increments it, and uses it as the inner index of
                          * gUnknown_084C3B3C[][5] */
};

/* gUnknown_03004504: eight 1-bit flags and a halfword at +2. sub_080546BC
 * sets and clears the flags one at a time. Keep the flags as bitfields: the
 * original clears them the way only a bitfield store compiles. The
 * container's width is unknown; u8 puts unk02 at +2, where the code stores
 * it. */
struct Unk03004504 /* >= 0x04 */
{
    /* 0x00 */ u8 bit0 : 1;
    /* 0x00 */ u8 bit1 : 1;
    /* 0x00 */ u8 bit2 : 1;
    /* 0x00 */ u8 bit3 : 1;
    /* 0x00 */ u8 bit4 : 1;
    /* 0x00 */ u8 bit5 : 1;
    /* 0x00 */ u8 bit6 : 1;
    /* 0x00 */ u8 bit7 : 1;
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u16 unk02;
};

struct Unk08090CD8Entry /* 0x88 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 unk06[128]; /* payload; unk04 holds its length.
                               * sub_08030930 copies 0x80 bytes and
                               * sub_080308B4 copies 20. */
    /* 0x86 */ u8 filler_86[0x88 - 0x86];
};

struct Unk08090CD8Buf
{
    /* 0x0000 */ u8 filler_00[6];
    /* 0x0006 */ u8 unk06;
    /* 0x0007 */ u8 filler_07[0x20 - 0x07];
    /* 0x0020 */ volatile u16 unk20;
    /* 0x0022 */ u8 filler_22[0x12c - 0x22];
    /* 0x012c */ struct Unk08090CD8Entry unk12c[48];
    /* 0x1aac */ u8 filler_1aac[1];
    /* 0x1aad */ volatile u8 unk1aad;
};

struct Unk08090CD8
{
    /* 0x00 */ struct Unk08090CD8Buf *unk00;
};


/* A list-panel record passed by pointer to sub_0804769C, sub_080482D8 and
 * sub_080484CC, which are in different files, so it is declared here.
 * c_08047B98.c declares its own narrower struct Unk08047B98 for the same
 * record; add new fields here, not there. Only the named members are known. */
struct Unk0804769C /* >= 0x2a */
{
    /* 0x00 */ u8 filler_00[0x1f];
    /* 0x1f */ u8 unk1f;   /* the cursor; sub_080484CC starts its downward
                            * scan from it */
    /* 0x20 */ u8 unk20;   /* the scroll position: sub_080482D8 shows
                            * gUnknown_02028DD8[unk20 + 0..5] and subtracts
                            * unk20 from unk1f to get a pixel offset */
    /* 0x21 */ u8 unk21;   /* how many gUnknown_02028DD8 entries are live;
                            * sub_0804769C's walk stops there (unsigned) */
    /* 0x22 */ u8 filler_22[6];
    /* 0x28 */ u16 unk28;  /* sub_080484CC stores the chosen
                            * gUnknown_02028DD8 byte here; sign unknown,
                            * nothing reads it */
};


/* The terrain info-box art table, indexed by terrain kind. Both names come
 * from the '44TerrainBoxEditor' Nightmare module (a community ROM-editor
 * definition). The module prints both fields at offset 0, which is a typo in
 * the module; the two call sites settle it -- sub_08017E00 reaches
 * gUnknown_08106A64 through +0 and sub_08017E1C reaches the palette table
 * gUnknown_08106864 through +2. */
struct Unk0849A2C8
{
    /* 0x00 */ u16 nameGraphic;
    /* 0x02 */ u16 palette;
};

struct Unk0849A354
{
    u16 unk00;
    u16 unk02;
};

/* Entries of the array gUnknown_084995A0 points to: at least 92
 * (sub_08061CDC clears 0..91), parallel to gProperty[]. sub_0800C574 stores
 * the same tile id, x and y into both at one index, and sub_0800C608 removes
 * an entry from both. unk00 = 0 marks an empty slot and 0xFF ends the list.
 * unk03[] is an array of three counters: sub_08058144 indexes it with a
 * variable. */
struct PropertyListEntry /* 0x08 */
{
    /* 0x00 */ u8 unk00; /* tile id / slot tag; 0 is empty and 0xFF terminates
                          * the list, matching Property's flags byte */
    /* 0x01 */ u8 unk01; /* map column, the same value as Property's x */
    /* 0x02 */ u8 unk02; /* map row, the same value as Property's y */
    /* 0x03 */ u8 unk03[0x03];
    /* 0x06 */ u8 filler_06[0x02];
};

struct Unk0849B018 /* >= 0x1ab5 */
{
    /* 0x00 */ volatile u8 unk00; /* The link-cable state record
                                   * gUnknown_0849B018 points to; struct
                                   * Unk08090CD8Buf is an older, narrower
                                   * view of the same record. Keep the
                                   * members volatile: the original reads
                                   * each one before writing it, which only a
                                   * volatile member produces. unk00 is a
                                   * mode: sub_08031F28, sub_08031F5C and
                                   * sub_08031F88 act only when it is 3. */
    /* 0x01 */ volatile u8 unk01; /* sub_080303B0 returns whether it equals 2 */
    /* 0x02 */ volatile u16 unk02; /* status bits that sub_0802F408 reads and
                                    * clears; bit 3 is the "link stalled"
                                    * flag it checks along with REG_SIOCNT
                                    * bit 3 */
    /* 0x04 */ volatile u16 unk04;
    /* 0x06 */ volatile s8 unk06; /* this player's slot number: sub_08030930
                                   * stamps it into each new packet's unk01,
                                   * and sub_080321F0 indexes unk0a with it.
                                   * Must stay volatile: without it the
                                   * compiler reads it in sub_080321F0 with a
                                   * different, shorter instruction sequence. */
    /* 0x07 */ volatile u8 unk07;
    /* 0x08 */ volatile u8 unk08; /* a bit mask like unk09: sub_0802F480
                                   * tests `(unk08 >> index) & 1` as
                                   * sub_0802F460 does for unk09 */
    /* 0x09 */ volatile u8 unk09;
    /* 0x0a */ volatile u8 unk0a[0x04]; /* one byte per slot, indexed by
                                         * unk06; sub_080321A8 compares an
                                         * element with 2 */
    /* unk0e: one halfword per slot, cleared by sub_0802F03C in the same loop
     * as unk0a, unk16 and unk24. */
    /* 0x0e */ volatile u16 unk0e[4];
    /* unk16: one byte per slot; sub_08030C54 passes each to
     * `sub_0802BD54(0xa0, (i + 10) * 8, unk16[i])` and then unk1b the same
     * way. */
    /* 0x16 */ volatile u8 unk16[0x04];
    /* 0x1a */ volatile u8 unk1a;
    /* 0x1b */ volatile u8 unk1b; /* retry counter: sub_08030234 and
                                   * sub_080303C8 increment it when a packet
                                   * disagrees and clear it when a frame is
                                   * accepted */
    /* 0x1c */ volatile u8 unk1c;
    /* 0x1d */ volatile u8 unk1d;
    /* 0x1e */ volatile u8 unk1e;
    /* 0x1f */ volatile u8 unk1f; /* stall counter: sub_0802F408 increments
                                   * it while the link is idle, clears it
                                   * when bit 3 of unk02 or of REG_SIOCNT is
                                   * set, and reports the link healthy while
                                   * it is below 0xb */
    /* unk20 is this player's packet sequence number: sub_08030930 stamps it
     * into each new packet's unk02, then increments it. sub_08033638 clears
     * unk20, unk22 and unk24[]. */
    /* 0x20 */ volatile u16 unk20;
    /* 0x22 */ volatile u16 unk22;
    /* 0x24 */ volatile u16 unk24[4]; /* the sequence number expected next
                                       * from each slot, indexed by a
                                       * packet's unk01. sub_080309AC accepts
                                       * a packet whose unk02 matches it and
                                       * then increments it; otherwise it
                                       * sends an 0xAE packet carrying this
                                       * value through gUnknown_0202575C. */
    /* unk2c: 128 halfwords, cleared by sub_0802F03C together with
     * gUnknown_03004400[], a parallel 128-entry table. */
    /* 0x2c */ volatile u16 unk2c[128];
    /* 0x12c */ struct Unk08090CD8Entry unk12c[48]; /* two packet rings; see
                                                     * the cursors below */
    /* 0x1aac */ volatile u8 unk1aac; /* read cursor of slots 0..31:
                                       * sub_080307E0 returns the packet
                                       * there if its unk00 is 0xAF */
    /* 0x1aad */ volatile u8 unk1aad; /* write cursor of slots 0..31:
                                       * sub_08030930 builds a packet there,
                                       * then steps the cursor modulo 32 */
    /* 0x1aae */ volatile u8 unk1aae; /* read cursor of slots 32..47:
                                       * sub_080309AC and sub_08030B00
                                       * consume the packet there, then step
                                       * the cursor modulo 16 */
    /* 0x1aaf */ volatile u8 unk1aaf; /* write cursor of slots 32..47:
                                       * sub_08030838 copies a packet there,
                                       * then steps the cursor modulo 16 */
    /* 0x1ab0 */ volatile u16 unk1ab0;
    /* 0x1ab2 */ volatile u8 unk1ab2;
    /* 0x1ab3 */ volatile u8 unk1ab3;
    /* 0x1ab4 */ volatile s8 unk1ab4;
};

/* 0x0849B01C -- ROM pointer to a RAM record (a different record from
 * gUnknown_0849B018's). Every member except unk08 must stay volatile:
 * sub_0802F23C and sub_08031B30 read each of these addresses again before
 * storing to it, and only a volatile member compiles that way.
 */
struct Unk0849B01C /* >= 0x214 */
{
    /* 0x000 */ volatile u16 unk00;
    /* 0x002 */ volatile u16 unk02;
    /* 0x004 */ volatile u8 unk04;
    /* 0x005 */ volatile u8 unk05;
    /* 0x006 */ volatile u16 unk06;
    /* A ring of 64 rows of four halfwords: sub_0802ED40 writes row unk04, and
     * sub_080301E8 copies row unk05 into unk208[] and wraps unk05 at 64. Must
     * stay a two-dimensional array: written as a flat index the copy compiles
     * differently. Must stay non-volatile, unlike the other members:
     * sub_08030178 only matches with it plain, so sub_0802ED40 writes through
     * its own volatile view of the rows. */
    /* 0x008 */ u16 unk08[64][4];
    /* 0x208 */ volatile u16 unk208[4];
    /* 0x210 */ volatile u16 unk210;
    /* 0x212 */ volatile u16 unk212;
};

/* 0x0849B060 -- ROM pointer to a RAM record. unk00 is round-tripped through
 * sub_080315E8 by sub_080319A8 and sub_08031BF0; unk0a is set from a byte by
 * sub_08031BE0 and cleared by sub_08031CD4.
 */
struct Unk0849B060 /* >= 0x11 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ s16 unk04; /* signed; sub_08032420 multiplies it by 40 and passes
                           * it to sub_08032340 as a coordinate */
    /* 0x06 */ s16 unk06; /* the second coordinate, used the same way by
                           * sub_08032420 */
    /* 0x08 */ u8 unk08; /* sub_08032048 stores (gPlaySt.mapID - 0xb4) % 3 here
                          * and the quotient by 3 into unk09 */
    /* 0x09 */ u8 unk09; /* a player slot: sub_080326FC compares it with
                          * gUnknown_0849B018->unk06 */
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u8 unk0c; /* a small count; sub_08032A00 runs one block only when
                          * it is above 1 */
    /* 0x0d */ u8 unk0d; /* script selector: sub_08034208 uses it to index
                          * gUnknown_0849BC44[] */
    /* 0x0e */ u16 unk0e; /* a y position: the four gUnknown_0849B650 steppers
                           * (sub_08032734, sub_08032788, sub_080327FC,
                           * sub_08032850) change it and sub_08032A00 reads it.
                           * Readers store unk0e - 0x60 into gUnknown_03002B34;
                           * nothing reads it as signed, although the steppers
                           * can store negative values. */
    /* 0x10 */ u8 unk10; /* direction, 0 to 2: the four steppers test it for 1
                          * and 2 to choose the sign of the
                          * gUnknown_0849B650[unk0a] step */
};

/* The compressed unit-sprite table. sprite[] and spriteFormat are named after
 * the community 'Compressed Unit Sprite' Nightmare module, whose 24 records of
 * 0x24 bytes start one record after this symbol (entry 0 is a dummy, as in the
 * unit and terrain tables). The module does not name +0x18 onward. sub_08035B3C
 * also reads a row as nine words chosen at run time, through a `u8 **` cast of
 * the row's address.
 */
struct Unk0849CD88 /* 0x24 */
{
    /* 0x00 */ void *sprite[5]; /* one compressed sprite per army, in the
                                 * module's order: OS, BM, GE, YC, BH */
    /* 0x14 */ u32 spriteFormat;
    /* 0x18 */ s16 unk18; /* sub_08035B00 returns it, as it is or doubled */
    /* 0x1a */ u16 unk1a; /* sub_08035E90 and sub_08035FA8 test it for zero and
                           * pass it to sub_0803B48C as an s16. Must stay u16:
                           * the zero tests load it unsigned. */
    /* 0x1c */ u8 filler_1c[0x02];
    /* 0x1e */ u16 unk1e; /* sub_08035F68 compares it with the special value
                           * 0x8000; signedness unknown */
    /* 0x20 */ void *unk20; /* a RAM pointer (the ROM holds 0x030xxxxx
                             * addresses); sub_08036024 copies it into a proc's
                             * +0x18 slot */
};

/* An eight-byte ROM row indexed by a palette or theme id: sub_08035020 passes
 * unk00 to ApplyPaletteExt as the source, and sub_08035064 passes unk04 to
 * sub_0803B4DC. Nothing reads +0x06.
 */
struct Unk0849BD20 /* 0x08 */
{
    /* 0x00 */ u16 *unk00;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
};

/* 0x0849D5F8 -- ROM pointer to a RAM record; not const, because the code
 * reloads it at every use. unk45 is a signed slot index into the parallel s8
 * tables unk20[], unk2c[] and unk38[]. Some readers sign-extend these bytes
 * with two shifts instead of a signed load; that comes from the compiler
 * running short of registers, not from a u8 type, so do not retype them on
 * seeing it.
 */
struct Unk0849D5F8 /* >= 0x46 */
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 unk1f;
    /* 0x20 */ s8 unk20[0x0c];
                          /* unk20[], unk2c[] and unk38[] must stay three
                           * arrays, all indexed by unk45: sub_08038B84 adds the
                           * index to base + 0x2c and base + 0x38, which one
                           * long array does not compile to. Their lengths are
                           * what fits before the next member. */
    /* 0x2c */ s8 unk2c[0x0c];
    /* 0x38 */ s8 unk38[0x0d];
    /* 0x45 */ s8 unk45;
};

/* 0x0849ECDC -- ROM pointer to a two-byte counter in RAM; not const, because
 * sub_0803BF10 reloads it after each store. unk00 is the limit: sub_0803BDF8,
 * sub_0803BE10 and sub_0803BE28 set it to 2, 3 or 4, and sub_0803B930 to an
 * argument. unk01 is the cursor: sub_0803BEF8 resets it, sub_0803BF10 moves it
 * (storing gUnknown_020288B0 into gPlaySt.co[unk01 + 1]), sub_0803BFA4 loads
 * gUnknown_020288B0 with unk01 + 1, and sub_0803BF70 sets gUnknown_03002F1C
 * while unk01 differs from unk00.
 */
struct Unk0849ECDC /* >= 0x02 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
};

struct Unk08580934_Sub
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ u32 unk08;
};

/* One object in Unk08580934's unk34[], unk44[] and unk54[] tables (unk54[]
 * holding this type is assumed, not confirmed). The spawners sub_080646BC to
 * sub_0806517C fill it and increment gUnknown_08580934->unk2d; the tick
 * handlers sub_080646D4, sub_08064FC8 and sub_08065118 update it every frame.
 *   unk1c  the object's slot index in unk44[] (sub_08064BC8)
 *   unk24  set to unk1c * 2 by sub_0806502C's caller; the tick handlers test
 *     and decrement it
 *   unk26  set to 0x0b, 0x0c, 0x0e or 0x10 by the spawners; the tick handlers
 *     count it down and use it as a table index
 *   unk28, unk2a  a position pair; sub_08064E5C passes both on
 * The other halfword fields are only ever stored, so their widths may be too
 * narrow and their signedness is unknown.
 */
struct Unk08580934_Obj /* >= 0x50 */
{
    /* 0x00 */ u8 filler_00[0x1c];
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u8 filler_1e[0x06];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2a */ s16 unk2a;
    /* 0x2c */ int unk2c; /* unk2c, unk30 and unk34 are whole words:
                           * sub_0806D208 adds a table entry to unk2c to make
                           * unk28, and every frame unk34 is added to unk30 and
                           * unk30 to unk28 (a fixed-point velocity and
                           * position). Signedness unknown. */
    /* 0x30 */ int unk30;
    /* 0x34 */ int unk34;
    /* 0x38 */ u16 unk38;
    /* 0x3a */ u16 unk3a;
    /* 0x3c */ u16 unk3c;
    /* 0x3e */ u16 unk3e; /* unk3e and unk40 are graphics bases: sub_0806D268
                           * sets them to 0x6000 plus an entry of the
                           * seven-entry tables gUnknown_0816E0D0 and
                           * gUnknown_0816E0DE */
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42; /* sub_0806D53C sets it to 0x42b0, or to
                           * 0x42a0 + i * 4, depending on
                           * gUnknown_08580934->unk09[i]; the same function
                           * sets unk40 to 0x8290 + i * 4 */
    /* 0x44 */ u16 unk44; /* sub_08064E5C ORs it with 0xc00 to make
                           * sub_08043FD8's third argument */
    /* 0x46 */ u8 unk46; /* marks the current entry: sub_0806DDF4 clears it on
                          * all seven unk54[] objects and sets it on the one
                          * unk33 selects */
    /* 0x47 */ u8 unk47; /* mode flag: sub_0806DC50 tests it for 1, and
                          * otherwise bounds unk48 by unk4b - 1 */
    /* 0x48 */ u8 unk48; /* a cursor over unk4b entries: sub_08064738 picks one
                          * of two sprite ids by whether it is zero, and
                          * sub_08064774 uses it to index a four-entry table */
    /* 0x49 */ u8 unk49; /* sub_08064E5C copies unk48 into it */
    /* 0x4a */ u8 filler_4a[0x01];
    /* 0x4b */ u8 unk4b; /* the number of entries unk48 moves over: sub_0806DC50
                          * and sub_0806DD34 compare unk48 with unk4b - 1 */
    /* 0x4c */ void (*unk4c)(struct Unk08580934_Obj *);
                         /* callback taking the object itself; sub_080645AC
                          * calls it */
};

/* The record gUnknown_08580934 points to. unk2d is a plain u8 counter that each
 * spawner increments; it is not volatile. unk44[] is indexed by an object's own
 * unk1c.
 * unk44[] and unk54[] must stay two arrays: sub_0806DDF4 walks unk54[], and the
 * same addresses written as unk44[i + 4] compile to a different loop. unk44[]'s
 * length 4 is what fits before unk54[].
 * gUnknown_08580934 does not need const, even though sub_08064BC8 keeps its
 * loaded value across a store.
 */
struct Unk08580934
{
    /* 0x00 */ u8 unk00;  /* unk00-unk07 are a header that sub_08065818 copies,
                           * one field per byte, into unk84-unk8a. unk04 is
                           * divided by 500 there. */
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u8 unk08; /* mode selector: sub_0806675C tests it for 2, and in
                          * that case also stores it into unk26 */
    /* 0x09 */ u8 unk09[4]; /* unk09[], unk0d[], unk11[] and unk20[] are four
                             * parallel per-slot byte tables; sub_0803C1D4
                             * copies all four into gPlaySt's per-army arrays.
                             * sub_08064DDC compares unk09[] with 2,
                             * sub_08064E1C adds 0x3d to unk0d[] to make a
                             * sprite id, and sub_08064BF4 indexes unk11[] by an
                             * object's unk1c and adds 0xbd. */
    /* 0x0d */ u8 unk0d[4];
    /* 0x11 */ u8 unk11[4];
    /* 0x15 */ u8 unk15;  /* sub_08065818 subtracts it from unk07 + 1 to make
                           * unk88. unk16 is a countdown that sub_08065818
                           * decrements. */
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;  /* the number of entries a unk1c[] cursor cycles
                           * through: sub_08065DAC steps a cursor modulo unk17 */
    /* 0x18 */ u8 *unk18; /* byte table indexed by a unk1c[] cursor;
                           * sub_08065DAC passes the element to sub_08043E3C */
    /* 0x1c */ u8 unk1c[0x04]; /* one cursor per slot into unk18; the length 4
                                * is what fits before unk20 */
    /* 0x20 */ u8 unk20[0x04]; /* the fourth per-slot table (see unk09);
                                * sub_0803C1D4 reads entries 0 to 3 */
    /* 0x24 */ u8 unk24;  /* mode flag: sub_08066B6C picks sub_0806630C or
                           * sub_08066B40 by whether it is zero */
    /* 0x25 */ s8 unk25;  /* sub_0806574C sets it to sub_0802F4F4()'s result
                           * when unk24 is set, and to -1 otherwise */
    /* 0x26 */ s8 unk26;  /* sub_0806630C and sub_08066B40 act on the values 0
                           * and 1 and ignore others */
    /* 0x27 */ u8 filler_27[0x03];
    /* 0x2a */ s16 unk2a; /* animation counter for the menu the unk33 cursor
                           * drives: sub_08066BF4 and sub_0806DCB8 clear it when
                           * the cursor moves, as they play sound 0x64 */
    /* 0x2c */ u8 unk2c;  /* a 0/1 mode flag: sub_0806574C copies
                           * gUnknown_0202F200 into it and sub_0806D850 copies
                           * gUnknown_0202F2C8; sub_0803BFBC tests it for 1 */
    /* 0x2d */ u8 unk2d;
    /* 0x2e */ u16 unk2e; /* id of the graphics now loaded: sub_0806DF58 uploads
                           * new tiles only when its chosen id differs, then
                           * stores the id here */
    /* 0x30 */ u8 unk30; /* byte flag: sub_08065990 clears it, calls
                          * sub_08065238, then sets it to 1; sub_0806D944 clears
                          * it at its end */
    /* 0x31 */ u8 unk31; /* sub-mode flag used when unk08 is 2: sub_08066220
                          * compares it with 1 to choose which key bit to fake
                          * into gpKeySt->held */
    /* 0x32 */ s8 unk32; /* index into unk44[] (sub_08066078); sub_08065EF4
                          * indexes unk34[] with unk32 / 2 */
    /* 0x33 */ s8 unk33; /* index of the current unk54[] entry, 0 to 6 */
    /* 0x34 */ struct Unk08580934_Obj *unk34[4];
                             /* a third object table beside unk44[] and unk54[];
                              * sub_08065EF4 indexes it with unk32 / 2. The
                              * length 4 is what fits before unk44[]. */
    /* 0x44 */ struct Unk08580934_Obj *unk44[4];
    /* 0x54 */ struct Unk08580934_Obj *unk54[7];
    /* unk70[] is a per-slot mark, indexed by an object's unk1c like unk44[]. It
     * is written as 0xff, 1 or 0: sub_08066470 sets 0xff while the slot is
     * drawing and 0 on its last frame. The values are signed (-1, 1, 0), but
     * the member is declared u8 and its readers (sub_08065E5C, sub_080665D4,
     * sub_0806666C) cast it to s8 at the use; whether retyping it to s8 would
     * change any promoted function has not been checked. */
    /* 0x70 */ u8 unk70[4];  
    /* 0x74 */ struct Unk03001470 *unk74[4];
                             /* Filled by sub_08066580 with sub_080152EC's
                              * return value, indexed like unk70[]. sub_08066580
                              * writes the pointed-to struct's unk28 as two
                              * halfwords, spelled `((u16 *)&p->unk28)[0]` and
                              * `[1]`, because unk28 is a u32 everywhere else. */
    /* 0x84 */ u8 unk84;  /* sub_08065818 fills unk84-unk8a from the
                           * header at +0x00:
                           *   unk84 = (unk00 == 0)
                           *   unk85 = unk06
                           *   unk86 = unk04 / 500 - 2
                           *   unk87 = unk03 ? unk03 - 4 : 0
                           *   unk88 = unk07 ? unk07 + 1 - unk15 : 0
                           *   unk89 = (unk01 == 0)
                           *   unk8a = unk02 */
    /* 0x85 */ u8 unk85;
    /* 0x86 */ u8 unk86;
    /* 0x87 */ u8 unk87;
    /* 0x88 */ u8 unk88;
    /* 0x89 */ u8 unk89;
    /* 0x8a */ u8 unk8a;
};

/* 0x0816E1B8 is a compiler-made pool word holding &gUnknown_08580934, not a
 * real global: the original C named gUnknown_08580934 directly. Declaring the
 * word as a const pointer to this one-member wrapper and reading ->unk00 gives
 * the same bytes (see "The .rodata address-constant reroute" in
 * docs/agbcc-codegen.md).
 * Must stay const: sub_0806DDF4 moves the load out of its loop, which the
 * compiler only does for a const global. That function must also take the
 * address into a local (`pp = &gUnknown_0816E1B8`), or the pool word changes.
 */
struct Unk0816E1B8
{
    /* 0x00 */ struct Unk08580934 *unk00;
};

/* gUnknown_085C77A0 is the array of map headers (struct Unk085C77A0, further
 * down), not a pointer to it. sub_080206B0 searches entries 0 to 0xbf for a
 * matching mapData[0], and sub_080587FC tests bit 0 of unk28.
 */
/* A gfx/palette pointer pair; sub_08043A80 fetches unk00 and sub_08043A90
 * unk04, and sub_08044AB8 feeds both to sub_08039A5C (Decompress +
 * ApplyPaletteExt).
 */
struct Unk084A06F0 /* 0x08 */
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
};

/* 0x084C1430 -- ROM pointer to a RAM record, reloaded at every use (so not
 * const). unk28 is a 5-element word array summed into unk24 by sub_08046218.
 * unk58/unk59 are a counter and its delay, stepped by sub_080466A4.
 */
struct Unk084C1430 /* >= 0x5f */
{
    /* 0x00 */ u8 filler_00[0x24];
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u32 unk28[5];
    /* 0x3c */ u32 unk3c[5]; /* per-army percentages: sub_08046030 stores
                              * unk28[k] * 100 / unk24 in unk3c[k] */
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 filler_51[0x02];
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 filler_55[0x03];
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 unk59;
    /* 0x5a */ u8 filler_5a[0x04];
    /* 0x5e */ u8 unk5e;
};

/* 0x084C3240 -- ROM pointer to a RAM record. unk2c is a frame counter:
 * sub_08049E38 clears it and sub_08049E50 ends the sequence once it passes
 * 0xdc.
 */
struct Unk084C3240 /* >= 0x2f */
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ s16 unk20; /* signed; sub_08049BEC sets it to 0x40 and
                           * sub_08049C38 reads it */
    /* 0x22 */ u8 filler_22[0x08];
    /* 0x2a */ u8 unk2a; /* cleared by sub_08049BEC */
    /* 0x2b */ u8 filler_2b[0x01];
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 unk2e;
};

/* The six event-script pointers of one map, reached through
 * gUnknown_085C77A0[map].dialogueHeader. Each slot is one trigger class, run by
 * its own function through sub_08074484: unk00 by sub_08074460, unk04 by
 * sub_080742FC, unk08 by sub_08074320, unk0c by sub_080743E8, unk10 by
 * sub_08074410 and unk14 by sub_0807443C.
 */
struct Unk08074584 /* 0x18 */
{
    /* 0x00 */ u8 *unk00;
    /* 0x04 */ u8 *unk04;
    /* 0x08 */ u8 *unk08;
    /* 0x0c */ u8 *unk0c;
    /* 0x10 */ u8 *unk10;
    /* 0x14 */ u8 *unk14;
};

/* One army's preset flags in a map header (Unk085C77A0's unk44 and unk48[]).
 * Must stay a struct, not a row of a u8 [5][4] array: sub_08026CD0 reaches both
 * bytes from one pointer that steps by 4, and a flat array compiles to a
 * different loop.
 */
struct Unk085C77A0Slot /* 0x04 */
{
    /* 0x00 */ u8 unk00; /* 0xff means "no preset"; otherwise OR'd into the
                          * army's gPlayers unk2d */
    /* 0x01 */ u8 unk01; /* OR'd into the same army's unk2e */
    /* 0x02 */ u8 filler_02[0x02]; /* never read */
};

/* The map headers, indexed by gPlaySt.mapID; entry 0 is a dummy. Field names
 * come from the community 'Advance Wars 2 Map Header Editor' Nightmare module.
 * Its records start 0x60 bytes into this table, four bytes into entry 1, so a
 * field at module offset X is at X + 4 here (its 'Name Index' at +0x10 is
 * nameIndex at +0x14), and our index is the module's plus one. The module's
 * 'Number of Players' (unk18 here) is not used as a name: the game reads that
 * byte as an id.
 */
struct Unk085C77A0 /* 0x5c */
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ const struct Unk08074584 *dialogueHeader; /* the map's event-script
                                                          * pointers; sub_08074584 returns
                                                          * this */
    /* 0x08 */ const u8 *unk08; /* script blob: sub_0802C7B4 passes it to
                                 * sub_080193B0 */
    /* 0x0c */ const u8 *hardcodedUnits; /* a second script blob: when category
                                          * is 2 or less sub_080364F4 passes it
                                          * to sub_080193B0; otherwise
                                          * sub_080364E0 runs the default at
                                          * gUnknown_0849D10C */
    /* 0x10 */ u8 *tileGraphic4x4; /* default Decompress source for sub_0803FD80 */
    /* 0x14 */ u16 nameIndex; /* row of the map's name in gTextTable[]
                               * (sub_08024944) */
    /* 0x16 */ u8 unk16; /* sub_08078E48 passes it to sub_0807A99C */
    /* 0x17 */ u8 fogOfWar; /* non-zero turns fog on: sub_080346FC sets
                             * gPlaySt.fog from it */
    /* 0x18 */ u8 unk18; /* an id sub_0802490C returns for maps outside
                          * 0xb4-0xbf (those come from sub_0803CD14); purpose
                          * unknown */
    /* 0x19 */ u8 filler_19[0x01];
    /* 0x1a */ u16 category; /* tested against 0 by sub_0802C660 */
    /* 0x1c */ u16 unk1c; /* unk1c and unk1e are passed together, in that order,
                           * to five map-decoration spawners by sub_0803E3D8 */
    /* 0x1e */ u16 unk1e;
    /* 0x20 */ u16 speedRankTurnLimitNc; /* turn limits for the speed rank, one
                                          * per mode: sub_080263A4 uses the Hc
                                          * value when IsHardCampaignMode() is true
                                          * and compares the limit with
                                          * gUnknown_03004080 */
    /* 0x22 */ u16 speedRankTurnLimitHc;
    /* 0x24 */ u16 timer; /* sub_08043630 returns it when non-zero, and
                           * gPlaySt.turnLimit otherwise */
    /* 0x26 */ u8 filler_26[0x01];
    /* 0x27 */ u8 unk27; /* when non-zero, sub_08061788 uses it as the row index
                          * into gUnknown_0857690C[][19] in place of
                          * gUnknown_030046B8 */
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 filler_29[0x03];
    /* 0x2c */ void *mapData[2]; /* two compressed map blobs: sub_080247A4 takes
                                  * mapData[IsHardCampaignMode()], falls back to
                                  * mapData[0] when that is NULL, and passes it
                                  * to LZ77UnCompWram */
    /* 0x34 */ void *unk34[2]; /* two pointers chosen like mapData:
                                * sub_080196C0 passes unk34[IsHardCampaignMode() != 0]
                                * to sub_080196F4, which reads through it. Typed
                                * void * until that object is known. */
    /* 0x3c */ u8 unk3c[4]; /* 0xff means empty; sub_0803BD14 counts the leading
                             * entries that are not 0xff */
    /* 0x40 */ u8 unk40[4]; /* paired with unk3c: sub_08026AC0 returns
                             * unk40[slot - 1] when unk3c[slot - 1] is not 0xff */
    /* 0x44 */ struct Unk085C77A0Slot unk44;
    /* 0x48 */ struct Unk085C77A0Slot unk48[4];
                            /* unk44 and unk48[] are the preset flags for army
                             * slots 0 to 4 (struct Unk085C77A0Slot). They must
                             * stay split this way: sub_08026CD0 walks all five
                             * as unk48[-1] to unk48[3], and only a member that
                             * starts at 0x48 compiles to the same loop. */
    /* 0x58 */ u8 unk58; /* sub_08024984 returns it when it is non-zero */
    /* 0x59 */ u8 filler_59[0x03];
};

/* The campaign map headers, one 0x30-byte record per campaign map, indexed
 * by gUnknown_0202FDFC.unk0c or by `gPlaySt.mapID - 0x8a`. Field names come
 * from the "Advance Wars 2 Campaign Header Editor" Nightmare module, a
 * community ROM-editor definition. That module calls +0x10 a two-byte "Map
 * Description", but the code reads a whole word there, so the name is not
 * used. IsHardCampaignMode() picks between each normal/hard pair. */
struct Unk08615194 /* 0x30 */
{
    /* 0x00 */ u16 mapID; /* Index into the map header table
                           * gUnknown_085C77A0. sub_08077F30 copies it into
                           * the u8 gPlaySt.mapID, so only the low byte is
                           * kept. */
    /* 0x02 */ u8 specialProperty;
    /* 0x03 */ u8 difficultyStars; /* Difficulty stars for normal mode;
                          * hardModeStars is the hard-mode value.
                          * sub_08076F34 passes the one IsHardCampaignMode()
                          * selects to sub_08075298. */
    /* 0x04 */ u8 hardModeStars;
    /* 0x05 */ u8 filler_05[0x01];
    /* 0x06 */ s16 flagX; /* Map position (x, y) of the flag; sub_080749FC
                           * passes both to sub_08074C84. */
    /* 0x08 */ s16 flagY;
    /* 0x0a */ u8 filler_0a[0x02];
    /* 0x0c */ const void *mapSectionToColor; /* Nullable pointer to a blob:
                             * u8 x, y, width, height, then at +0x04 a
                             * pointer to width*height palette-bank ids.
                             * sub_08075904 and sub_080759A0 stamp the ids
                             * into gUnknown_08614280's tilemap; they
                             * declare the blob's type locally. */
    /* 0x10 */ void *unk10; /* A word that sub_08077304 copies unchanged
                             * into its proc at +0x30. Nothing dereferences
                             * it, so its type is unknown. */
    /* 0x14 */ void *preBattleDialogue; /* Optional script: when not NULL,
                             * sub_08077EDC passes it to sub_08078540, gated
                             * by unk2c. */
    /* 0x18 */ void *unk18; /* unk18/unk1c: two optional scripts.
                             * sub_08076C1C passes unk18 to sub_08078540
                             * when gUnknown_0202FDFC.unk11 is non-zero,
                             * else unk1c; NULL means none. Pointee type
                             * unknown. */
    /* 0x1c */ void *unk1c;
    /* 0x20 */ void *coSelect; /* When not NULL, sub_08077304 decompresses
                             * the gUnknown_081D1F74 graphics and keeps this
                             * pointer in its proc at +0x38. When NULL it
                             * calls sub_08043E3C with
                             * gUnknown_085C77A0[mapID].unk3c[0] instead.
                             * Pointee type unknown. */
    /* 0x24 */ void *factoryScriptNc; /* Factory unit schedule for normal
                             * mode; factoryScriptHc is the hard-mode one.
                             * sub_08077F30 stores the one
                             * IsHardCampaignMode() selects in
                             * gFactoryUnitSchedule. */
    /* 0x28 */ void *factoryScriptHc;
    /* 0x2c */ u8 (*unk2c)(void); /* Optional predicate: when set,
                             * sub_08077EDC calls it and runs
                             * preBattleDialogue only if it returns
                             * non-zero. Returns u8 because only the low
                             * byte of the result is tested. */
};

/* ------------------------------------------------------- m4a / MP2K -- */

/* The Nintendo MusicPlayer2000 (MP2K) sound driver, 0x0806F734-0x080718E4. Its
 * ARM half and the ply_* command handlers are hand-written (see
 * data/asm-resident.json); the THUMB entry points are ordinary compiler output
 * and correspond to m4a.c in the public GBA decomps. The structs below follow
 * the public MP2K layouts, and every offset this ROM's code reaches, including
 * the hand-written half, agrees with them.
 *
 * MusicPlayerTrack's modulation block is modM, mod, modT at 0x16-0x18, as in
 * MP2K. A copy of the public layout that leaves out `mod` makes the later
 * members look shifted by one byte; they are not.
 *
 * A `filler` member's trailing comment lists the public names it covers.
 * Narrow one out when a function needs it; never move anything. */
#define MPLAY_ID_NUMBER 0x68736D53 /* 'Smsh' */
#define MUSICPLAYER_STATUS_PAUSE 0x80000000
#define FADE_VOL_SHIFT 2
#define TEMPORARY_FADE 0x0001
#define FADE_IN 0x0002 /* sub_08070640 stores a bare 2 into fadeOV */

/* Bits of MusicPlayerTrack::flags, with the public MP2K names. VOLCHG and
 * PITCHG ask for the track's volume or pitch to be recomputed. */
#define MPT_FLG_VOLCHG 0x03
#define MPT_FLG_PITCHG 0x0C
/* Set while the track is in use. The MPlay*Control functions skip tracks
 * without it. */
#define MPT_FLG_EXIST 0x80
/* Set on every track MPlayStart (sub_08070BAC) starts; it tests the bit on
 * track 0 to see whether the current song is still playing. That function
 * writes it as a literal 0x40. */
#define MPT_FLG_START 0x40

struct ToneData /* 0x0c */
{
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 key;
    /* 0x02 */ u8 length;
    /* 0x03 */ u8 pan_sweep;
    /* 0x04 */ u32 wav;
    /* 0x08 */ u8 attack;
    /* 0x09 */ u8 decay;
    /* 0x0a */ u8 sustain;
    /* 0x0b */ u8 release;
};

/* MP2K's SoundChannel, one mixer channel; MusicPlayerTrack::chan points at
 * one. rp is signed: the hand-written ChnVolSet (sub_08070090) reads it
 * sign-extended and uses it as both 0x80 + rp and 0x7f - rp. */
struct SoundChannel /* 0x40 */
{
    /* 0x00 */ u8 status;          /* SoundClear (sub_08070A28) zeroes it in
                                    * all twelve channels. */
    /* 0x01 */ u8 type;            /* The low three bits hold the CGB
                                    * channel number (1-4), zero otherwise.
                                    * TrackStop (sub_0807004C) turns the CGB
                                    * channel off when they are non-zero. */
    /* 0x02 */ u8 rightVolume;
    /* 0x03 */ u8 leftVolume;
    /* 0x04 */ u8 filler_04[0x0d]; /* attack decay sustain release ky
                                    * envelopeVolume envelopeVolumeRight
                                    * envelopeVolumeLeft echoVolume echoLength
                                    * d1 d2 gt */
    /* 0x11 */ u8 mk;              /* The key this channel is playing.
                                    * ply_endtie (sub_080702C0) compares it
                                    * with MusicPlayerTrack::key to find the
                                    * channel to stop. */
    /* 0x12 */ u8 ve;
    /* 0x13 */ u8 filler_13[0x01]; /* pr */
    /* 0x14 */ s8 rp;
    /* 0x15 */ u8 filler_15[0x17]; /* d3[3] ct fw freq wav cp */
    /* 0x2c */ void *track;        /* Set to NULL by TrackStop
                                    * (sub_0807004C) when it detaches the
                                    * channel. Never read, so the pointee
                                    * type is unknown. */
    /* 0x30 */ u8 filler_30[0x04]; /* pp */
    /* 0x34 */ struct SoundChannel *np;
                                   /* Next channel in the chain, or NULL. */
    /* 0x38 */ u8 filler_38[0x08]; /* d4 xpi xpc */
};

/* MP2K's CgbChannel, one per GB sound channel 1-4. m4aSoundInit
 * (sub_080703F4) hands the array gUnknown_030057D0 to MPlayExtender
 * (sub_080706B0), and SoundInfo::cgbChans points at it. Most members use
 * MP2K's names.
 *
 * rightVolume and leftVolume must stay volatile: CgbModVol (sub_08070F44,
 * parked) reads them again at each use, and a plain u8 lets the compiler
 * reuse the first read. No promoted code reads them. */
struct CgbChannel /* 0x40 */
{
    /* 0x00 */ u8 sf;
    /* 0x01 */ u8 ty;              /* The CGB channel number, 1-4.
                                    * MPlayExtender (sub_080706B0) sets it,
                                    * with the matching panMask. */
    /* 0x02 */ volatile u8 rightVolume;
    /* 0x03 */ volatile u8 leftVolume;
    /* Used by CgbSound (sub_08070FAC), with MP2K's names:
     *   at/de/unk06/re  attack, decay, sustain and release rates. unk06 is
     *                   MP2K's `su`, named as in sub_08070F44's draft.
     *   ev/ec           the current envelope value and its phase counter.
     *   envelopeGoal    MP2K's `eg`; unk19 is `sg`, the sustain goal.
     *   echoVolume/echoLength  the echo level and a signed countdown.
     *   n4              shadow of the NRx4 register; bit 7 is the restart bit.
     *   mo              what changed this frame: 1 = envelope and panning,
     *                   2 = frequency.
     *   le/sw           NRx1's length and channel 1's sweep.
     *   fr              the 11-bit frequency, a whole word. Code that reads
     *                   only its high byte does so through `(u8 *)&fr + 1`.
     *   wp/cp           the wave-RAM pattern and the copy currently loaded. For
     *                   channels 1 and 2, wp holds the duty value instead. */
    /* 0x04 */ u8 at;
    /* 0x05 */ u8 de;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 re;
    /* 0x08 */ u8 filler_08[0x01];
    /* 0x09 */ u8 ev;
    /* 0x0a */ u8 envelopeGoal;
    /* 0x0b */ u8 ec;
    /* 0x0c */ u8 echoVolume;
    /* 0x0d */ u8 echoLength;
    /* 0x0e */ u8 filler_0e[0x0b];
    /* 0x19 */ u8 unk19;
    /* 0x1a */ u8 n4;
    /* 0x1b */ u8 pan;
    /* 0x1c */ u8 panMask;
    /* 0x1d */ u8 mo;
    /* 0x1e */ u8 le;
    /* 0x1f */ u8 sw;
    /* 0x20 */ u32 fr;
    /* 0x24 */ u32 *wp;
    /* 0x28 */ u32 *cp;
    /* 0x2c */ u8 filler_2c[0x14];
};

/* MP2K's WaveData. Only freq is used here: MidiKey2freq (sub_08070350) reads
 * it as the sample's base frequency. The other members follow the public
 * layout and are unconfirmed. */
struct WaveData /* >= 0x10 */
{
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 status;
    /* 0x04 */ u32 freq;
    /* 0x08 */ u32 loopStart;
    /* 0x0c */ u32 size;
};

/* The MP2K pitch tables.
 *   gUnknown_081B9DF4  gScaleTable: one byte per MIDI key, low nibble an
 *                      index into gFreqTable, high nibble a right shift.
 *   gUnknown_081B9EA8  gFreqTable.
 *   gUnknown_081B9EF0, gUnknown_081B9F74  the same pair for the CGB channels,
 *                      used by sub_08070E4C; its value table is signed.
 *   gUnknown_081B9F8C  the noise-channel table sub_08070E4C returns directly
 *                      for channel 4. */
extern const u8 gUnknown_081B9DF4[];
extern const u32 gUnknown_081B9EA8[];
extern const u8 gUnknown_081B9EF0[];
extern const s16 gUnknown_081B9F74[];
extern const u8 gUnknown_081B9F8C[];
/* MP2K's gCgb3Vol, 16 entries. CgbSound (sub_08070FAC) maps the wave
 * channel's 4-bit envelope value through it to the NR32 output level, since
 * that channel has no hardware envelope. */
extern const u8 gUnknown_081B9FC8[];
/* MP2K's gPcmSamplesPerVBlankTable. SampleFreqSet (sub_080708EC) reads entry
 * `freq - 1`; freq is 4 bits, so there are at most 15 entries. The exact
 * length is unknown. */
extern const u16 gUnknown_081B9ED8[];
/* Three blobs sub_0806BF40 passes to sub_080718F8 as its `u8 *` second
 * argument (with 0xe0 as the third), one each for entry types 3, 4 and 5.
 * Their contents are not read in C. Not const, to match that parameter.
 * Declared separately: they are 0x74 and 0x84 bytes apart, so they are not
 * one array. */
extern u8 gUnknown_081B9BC8[];
extern u8 gUnknown_081B9C3C[];
extern u8 gUnknown_081B9CC0[];

struct MusicPlayerTrack /* 0x50 */
{
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 wait;
    /* 0x02 */ u8 patternLevel;
    /* 0x03 */ u8 repN;
    /* 0x04 */ u8 filler_04[0x01]; /* gateTime */
    /* 0x05 */ u8 key;             /* The note being played. ply_endtie
                                    * (sub_080702C0) stores it and compares
                                    * it with SoundChannel::mk. */
    /* 0x06 */ u8 filler_06[0x02]; /* velocity runningStatus */
    /* 0x08 */ u8 keyM;            /* keyM/pitM: the key and fine-pitch
                                    * halves of the pitch TrkVolPitSet
                                    * (sub_08070D98) computes from keyShift,
                                    * keyShiftX, tune, pitX, bend and
                                    * bendRange. */
    /* 0x09 */ u8 pitM;
    /* 0x0a */ s8 keyShift;
    /* 0x0b */ s8 keyShiftX;
    /* 0x0c */ s8 tune;
    /* 0x0d */ u8 pitX;
    /* 0x0e */ s8 bend;
    /* 0x0f */ u8 bendRange;
    /* 0x10 */ u8 volMR;           /* volMR/volML: the right and left volume
                                    * TrkVolPitSet (sub_08070D98) computes.
                                    * ChnVolSet (sub_08070090) scales each
                                    * channel's volume by them. */
    /* 0x11 */ u8 volML;
    /* 0x12 */ u8 vol;             /* vol and volX multiply into the track
                                    * volume; pan and panX are signed. modM
                                    * is really signed too (TrkVolPitSet
                                    * reads it sign-extended) but is
                                    * declared u8 and cast at each use. */
    /* 0x13 */ u8 volX;
    /* 0x14 */ s8 pan;
    /* 0x15 */ s8 panX;
    /* 0x16 */ u8 modM;
    /* 0x17 */ u8 mod;
    /* 0x18 */ u8 modT;
    /* 0x19 */ u8 lfoSpeed;
    /* 0x1a */ u8 lfoSpeedC;
    /* 0x1b */ u8 lfoDelay;
    /* 0x1c */ u8 lfoDelayC;
    /* 0x1d */ u8 priority;
    /* 0x1e */ u8 echoVolume;
    /* 0x1f */ u8 echoLength;
    /* 0x20 */ void *chan;
    /* 0x24 */ struct ToneData tone;
    /* 0x30 */ u8 filler_30[0x10];
    /* 0x40 */ u8 *cmdPtr;
    /* 0x44 */ u8 *patternStack[3];
};

/* MP2K's SoundInfo, 0xfb0 bytes, reached only through gUnknown_03007FF0
 * (MP2K's SOUND_INFO_PTR, at the usual 0x03007FF0). `ident` holds the 'Smsh'
 * ID; SoundVSyncOff (sub_08070A7C) adds 10 to it while the vsync DMA is off,
 * and SoundVSyncOn (sub_08070AF8) subtracts the 10 again.
 *
 * pcmDmaCounter is volatile because sub_08070AF8 has an unused read of the
 * byte just before it stores 0 there, which agbcc emits only for a volatile
 * member. sub_0806FD98 (parked) decrements it with a single read, which only
 * a plain u8 gives. Which is right is unresolved; changing the qualifier
 * means re-checking sub_08070AF8. */
struct MusicPlayerInfo;
struct SoundInfo
{
    /* 0x00 */ vu32 ident;
    /* 0x04 */ vu8 pcmDmaCounter;
    /* 0x05 */ u8 reverb;  /* Reverb level, set by m4aSoundMode
                            * (sub_08070990). */
    /* 0x06 */ u8 unk06;   /* unk06/unk07: MP2K's maxChans and masterVolume.
                            * SoundInit (sub_080707F4) sets them to 8 and
                            * 15. */
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u8 freq;             /* Sample-rate index from the sound
                                     * mode. SampleFreqSet (sub_080708EC)
                                     * looks up gUnknown_081B9ED8[freq - 1]. */
    /* 0x09 */ u8 filler_09[0x01];  /* mode */
    /* 0x0a */ u8 c15;              /* Frame counter: CgbSound
                                     * (sub_08070FAC) counts it down and
                                     * restarts it at 14 after 0. MP2K's
                                     * name. */
    /* 0x0b */ u8 pcmDmaPeriod;     /* The value pcmDmaCounter is reloaded
                                     * from when it reaches zero
                                     * (sub_0806FD98). */
    /* 0x0c */ u8 maxLines;         /* Set only by MPlayExtender
                                     * (sub_080706B0), which stores the
                                     * link-time symbol gMaxLines (value 0)
                                     * here. */
    /* 0x0d */ u8 filler_0d[0x03];  /* gap[3] */
    /* 0x10 */ s32 pcmSamplesPerVBlank;
                                    /* pcmSamplesPerVBlank, pcmFreq and
                                     * divFreq are set by SampleFreqSet
                                     * (sub_080708EC). They must stay
                                     * signed: the code divides them with
                                     * the signed division helper. */
    /* 0x14 */ s32 pcmFreq;
    /* 0x18 */ s32 divFreq;
    /* 0x1c */ struct CgbChannel *cgbChans;
                                    /* The four CGB channels. SoundClear
                                     * (sub_08070A28) checks it for NULL
                                     * first. */
    /* 0x20 */ void (*func)(struct MusicPlayerInfo *);
                                    /* func/intp: the per-frame player hook
                                     * and its argument. MPlayOpen
                                     * (sub_08070B34) moves any existing
                                     * pair into the new player's func/intp,
                                     * then installs MPlayMain
                                     * (sub_0806FDE4) with that player. */
    /* 0x24 */ struct MusicPlayerInfo *intp;
    /* 0x28 */ void (*unk28)(void); /* unk28..unk3c are MP2K's hook block.
                                     * SoundInit (sub_080707F4) sets unk28,
                                     * unk2c, unk30 and unk3c to
                                     * sub_080718E4, unk38 to sub_080700C0,
                                     * and unk34 to &gUnknown_03005740. */
    /* 0x2c */ void (*unk2c)(u8);   /* The CGB oscillator-off hook:
                                     * SoundClear (sub_08070A28) calls it
                                     * with each CGB channel number.
                                     * SoundInit stores it through a cast. */
    /* 0x30 */ void (*unk30)(void);
    /* 0x34 */ void *unk34;
    /* 0x38 */ void (*unk38)(void);
    /* 0x3c */ void (*unk3c)(void);
    /* 0x40 */ u8 filler_40[0x10];
    /* 0x50 */ struct SoundChannel chans[12];
                                    /* The twelve PCM channels; the array
                                     * ends exactly where pcmBuffer starts. */
    /* 0x350 */ u8 pcmBuffer[2][0x630];
                                    /* The two DMA source buffers:
                                     * sub_080707F4 points DMA1 at +0x350
                                     * and DMA2 at +0x980. */
};

/* The m4a song and player tables. gUnknown_0824238C is the song table: each
 * struct Song holds the song header and, at +4, the player the song plays
 * on. gUnknown_08242308 is the player table, 11 entries (gNumMusicPlayers). */
struct Song /* 0x08 */
{
    /* 0x00 */ void *header;
    /* 0x04 */ u16 ms;
    /* 0x06 */ u16 me;
};
struct MusicPlayer /* 0x0c */
{
    /* 0x00 */ struct MusicPlayerInfo *info;
    /* 0x04 */ struct MusicPlayerTrack *track;
    /* 0x08 */ u8 trackCount;
    /* 0x09 */ u8 filler_09[0x01];
    /* 0x0a */ u16 unk_0a;
};

/* MP2K's song header: what Song::header points at and MPlayStart
 * (sub_08070BAC) reads. part[] runs for trackCount entries. */
struct SongHeader
{
    /* 0x00 */ u8 trackCount;
    /* 0x01 */ u8 blockCount;
    /* 0x02 */ u8 priority;
    /* 0x03 */ u8 reverb;
    /* 0x04 */ struct ToneData *tone;
    /* 0x08 */ u8 *part[1];
};
extern const struct MusicPlayer gUnknown_08242308[];
extern const struct Song gUnknown_0824238C[];
/* The number of m4a players, 11 (MP2K's NUM_MUSIC_PLAYERS). An absolute
 * symbol defined in aw2bhr.lds, not a variable: the code uses its address as
 * the value, written `(u16)(u32)&gNumMusicPlayers`. A plain constant 11 does
 * not give the same code, because the original loads the value from a pool
 * word. */
extern const u8 gNumMusicPlayers;
/* MP2K's MAX_LINES, another absolute symbol from aw2bhr.lds, value 0.
 * MPlayExtender (sub_080706B0) stores its address into SoundInfo::maxLines. */
extern const u8 gMaxLines;
/* Two IWRAM function pointers, each called through its own forwarder that
 * passes its one argument straight through: sub_080707CC calls
 * gUnknown_030057C8 and sub_080707E0 calls gUnknown_030057CC.
 * gUnknown_030057CC is MP2K's Clear64byte: MPlayOpen (sub_08070B34) calls it
 * on the 0x40-byte MusicPlayerInfo it is about to fill. gUnknown_030057C8's
 * argument type is unconfirmed; it copies its twin's. */
extern void (*gUnknown_030057C8)(void *);
extern void (*gUnknown_030057CC)(void *);
struct MusicPlayerInfo /* 0x40 */
{
    /* 0x00 */ void *songHeader;
    /* 0x04 */ u32 status;
    /* 0x08 */ u8 trackCount;      /* Number of entries in tracks[]. The
                                    * MPlay*Control functions loop over that
                                    * many. */
    /* 0x09 */ u8 priority;        /* sub_080700C0 adds it to the track's own
                                    * priority at +0x1d and clamps to 0xff */
    /* 0x0a */ u8 filler_0a[0x01]; /* cmd */
    /* 0x0b */ u8 unk_0b;          /* Copied from MusicPlayer::unk_0a by
                                    * m4aSoundInit (sub_080703F4).
                                    * MPlayStart (sub_08070BAC) always
                                    * accepts a new song when it is zero. */
    /* 0x0c */ u32 clock;          /* zeroed by MPlayStart */
    /* 0x10 */ u8 filler_10[0x08]; /* gap[8] */
    /* 0x18 */ u8 *memAccArea;     /* m4aSoundInit (sub_080703F4) points
                                    * every player at gUnknown_03005BE0. */
    /* 0x1c */ u16 tempoD;
    /* 0x1e */ u16 tempoU;
    /* 0x20 */ u16 tempoI;
    /* 0x22 */ u16 tempoC;         /* Zeroed by MPlayStart together with
                                    * tempoD, tempoU and tempoI. */
    /* 0x24 */ u16 fadeOI;
    /* 0x26 */ u16 fadeOC;
    /* 0x28 */ u16 fadeOV;
    /* 0x2a */ u8 filler_2a[0x02];
    /* 0x2c */ struct MusicPlayerTrack *tracks;
    /* 0x30 */ struct ToneData *tone;
    /* 0x34 */ u32 ident;
    /* 0x38 */ void (*func)(struct MusicPlayerInfo *);
                                   /* func/intp: where MPlayOpen
                                    * (sub_08070B34) saves the SoundInfo
                                    * func/intp pair it replaces, so the
                                    * types must match SoundInfo's. */
    /* 0x3c */ struct MusicPlayerInfo *intp;
};

/* -------------------------------------------------------------- EWRAM -- */

extern struct ActiveMap *gActiveMap;
/* The ActiveMap that gActiveMap points at: sub_08000E48 sets gActiveMap to it
 * and clears all 0xB0 bytes. */
extern struct ActiveMap gUnknown_0200B000;
extern struct Unk0200B224 gUnknown_0200B224[];
/* HBlank scanline buffer, one halfword per scanline (160 entries).
 * sub_08011228 fills it; sub_08011298 and sub_0801137C pass it to
 * sub_080111C8 as the source. */
extern u16 gUnknown_0200B274[];
/* Queue of deferred copy requests (see gUnknown_030044D0), exactly 48
 * entries: sub_08011C18 clears all 48, and gUnknown_0200B5F4 starts right
 * after the last one. */
extern struct Unk0200B3B4 gUnknown_0200B3B4[];
/* When non-zero, sub_08011E54 does the copy at once through sub_08011C68
 * instead of adding it to the gUnknown_0200B3B4 queue. sub_08036944 sets it
 * and clears it again. */
extern u16 gUnknown_030044D0;
/* Signed offsets that the ARM routine sub_08000234 adds to palette
 * components. sub_080136C4 clears all 32. */
extern s8 gUnknown_0200B5F4[0x20];
/* Unpacked RGB copy of palette colours, directly after gUnknown_0200B5F4: three
 * bytes per colour, the 5-bit red, green and blue of gPal[c * 16 + i], at
 * (c * 16 + i) * 3. sub_08075A54 fills it. sub_080139E0 reads it through an
 * (s8) cast so that an overflowed sum reads as negative and clamps to 0.
 * Left unsized: nothing bounds c. */
extern u8 gUnknown_0200B614[];
/* The record sub_080147B4 builds and sub_08014074 reads; gUnknown_0200C020 is
 * the one instance. sub_080145E8 copies the same fields, and sub_080149C0 and
 * sub_08014A5C write a gUnknown_03001470 slot through this type too (struct
 * Unk03001470 declares other widths at these offsets). +0x20 is a gTextTable
 * entry, +0x3c a callback (sub_080147B4 stores sub_08013AEC); +0x24 is only
 * ever stored 0, so its type is a guess. The object is 0x58 bytes in RAM; the
 * struct stops at 0x41 because nothing indexes it. c_08013D4C.c (struct
 * Unk8013D4C) and c_0801B964.c (struct Unk1B998) still use file-local partial
 * views of the same record. */
struct Unk08014074
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ u8 *unk20;
    /* 0x24 */ u32 unk24;
    /* 0x28 */ u16 *unk28;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u16 unk34;
    /* 0x36 */ u16 unk36;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3a */ s8 unk3a;
    /* 0x3b */ u8 filler_3b[0x01];
    /* 0x3c */ void (*unk3c)(void);
    /* 0x40 */ u8 unk40;
};
/* The one instance of the above: sub_08014614, sub_08014668 and sub_080146D4
 * fill it with sub_080147B4 and then pass it to sub_08014074. */
extern struct Unk08014074 gUnknown_0200C020;
/* Two tables of struct Unk0200C078Rec, adjacent in RAM and always handled
 * together: sub_08016A54 clears both, sub_08016B2C saves both and sub_08016BC0
 * restores them. gUnknown_0200C078 is 30 rows of 5, gUnknown_0200C2D0 42 rows
 * of 2. Each row is a struct because the save and restore code indexes a row
 * and then walks its words. */
struct Unk0200C078 /* 0x14 */
{
    /* 0x00 */ struct Unk0200C078Rec unk00[5];
};
struct Unk0200C2D0 /* 0x08 */
{
    /* 0x00 */ struct Unk0200C078Rec unk00[2];
};
extern struct Unk0200C078 gUnknown_0200C078[30];
extern struct Unk0200C2D0 gUnknown_0200C2D0[42];
/* The 0x5CC-byte save block sub_08016B2C fills and sub_08016BC0 reads back
 * (both return 0x5CC); sub_08016EA4 and sub_08016ED8 read it through
 * gUnknown_02000000. The original copy loops for unk2a0 copy five words per
 * 8-byte row, so each row copy runs 12 bytes into the next; the last row's
 * overrun lands in unk3f0, which is overwritten straight after. This bug is
 * reproduced, not fixed. */
struct Unk08016B2C /* 0x5cc */
{
    /* 0x000 */ struct Unk02028030 unk000;
    /* 0x048 */ struct Unk0200C078 unk048[30];
    /* 0x2a0 */ struct Unk0200C2D0 unk2a0[42];
    /* 0x3f0 */ struct Unk0200C420 unk3f0;
    /* 0x4d0 */ u8 unk4d0[0xfc]; /* the first 0xfc bytes of gUnknown_0202FDFC */
};
/* 0x0808E53C and 0x0808E54C are compiler-made pool words holding this
 * global's address; C code names the global, never the words. */
extern struct Unk0200C420 gUnknown_0200C420;
extern u32 gUnknown_0200C500[2];
extern struct Unk0200C528 gUnknown_0200C528[];
/* The sprite-entry pool the bump allocator hands out; gUnknown_03002B24 is
 * reset to it by sub_0801BE78. 0x0200D510 - 0x0200CD10 == 0x800, i.e. exactly
 * 128 entries before the layer-head array. */
extern struct SpriteEntry gUnknown_0200CD10[];
extern struct SpriteEntry gUnknown_0200D510[];
extern struct Unk0200E438 gUnknown_0200E438[];
extern struct Unk0200F720 gUnknown_0200F720[];
extern struct Unk0200F920 gUnknown_0200F920[];
/* The sprite-request ring, 0x14 bytes per entry: sub_0801E338, sub_0801E4B0
 * and sub_0801E8D8 fill the entry at index gUnknown_03002510 and hand it to
 * sub_0801A718. unk00, unk02 and unk08 are really signed (sub_0801DFE8
 * sign-extends them into int parameters) but are declared u16, so
 * src/decomp/c_0801DF94.c casts them to s16 there. If you retype them, drop
 * those casts. */
struct Unk0200ED20 /* 0x14 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ long long unk0c; /* sub_0801E338 copies its long long argument
                                 * here; sub_0801ECE8 masks the two halves. */
};
extern struct Unk0200ED20 gUnknown_0200ED20[];
extern int gUnknown_03002510;
/* sub_0801DC50's high-water index: it stores a1 + 1 here when a1 reaches it. */
extern int gUnknown_03003034;
/* 0x0808F09C, 0x0808F0A0 and 0x0808F0A4 are compiler-made pool words holding
 * the addresses of gUnknown_0200E438 and gUnknown_03003034 (twice); C code
 * names the globals, never the words. */
/* A staging buffer: a ROM blob is decompressed into it, then sub_0803A174
 * copies 0x800 bytes of it to VRAM 0x06015D00 with sub_08011C68. Its real
 * length is not known. */
extern u8 gUnknown_0200FD50[];
extern struct Unk02027F74 gUnknown_02027F74;
/* A list of byte ids, filled from index 0 by sub_08037448, sub_08037508,
 * sub_0803753C and sub_08037570, and read through the gUnknown_02027F74.unk36
 * and .unk37 cursors. It overlaps gUnknown_02027F74 (it starts at +4), so the
 * cursors are entries 0x32 and 0x33; the 0x6b-entry fills overwrite them and
 * then reset them. Must stay a separate extern: the original code names both
 * addresses. Signedness unconfirmed. */
extern u8 gUnknown_02027F78[];
extern struct Unk02027FB0 gUnknown_02027FB0[16];
/* Four addresses the disassembly names as globals are members of this object:
 * gUnknown_02028038, gUnknown_02028040, gUnknown_02028042 and gUnknown_0202805A
 * are .unk08, .unk10, .unk12 and .unk2a. Write them as members; do not declare
 * them. */
extern struct Unk02028030 gUnknown_02028030;
/* Shadow copy of gUnknown_02028030, directly after it: sub_0803BCA0 and
 * sub_0803BCB8 copy 0x48 bytes between the two, one in each direction. Nothing
 * else uses it, so its field layout is taken from gUnknown_02028030. */
extern struct Unk02028030 gUnknown_02028078;
extern struct Unk020280C0 gUnknown_020280C0[];
/* Four bytes per gUnknown_020280C0 entry that sub_0803BFBC reads as
 * gUnknown_020280D4[k * 28 + 1 + i], with k = gPlaySt.unk02 - 0xb4 (0..0xb).
 * It is the same memory as bytes 0x15..0x18 of gUnknown_020280C0[k]: this
 * address is gUnknown_020280C0 + 0x14 and that struct is 0x1c bytes. */
extern u8 gUnknown_020280D4[];
extern struct Unk02025564 gUnknown_02025564;
/* Four rows of 13 bytes, cleared row by row by sub_0802F28C. Must stay
 * volatile: the original clear loop reads each byte before it writes it. */
extern volatile u8 gUnknown_020257E4[][13];
extern struct Unk02028360 gUnknown_02028360[];
/* A second array of struct Unk02028360, 0x80 bytes (16 entries) after
 * gUnknown_02028360; sub_0803DE68 clears the same bits (mask 0x3C0) of unk02 in
 * both. The length of neither array is known. */
extern struct Unk02028360 gUnknown_020283E0[];
/* The original copy of the four army rosters, 64 struct Unit records each:
 * sub_08026040 copies all 0xC00 bytes into a scratch buffer and hands each
 * quarter to sub_080260D0. */
extern struct Unit gUnknown_02022684[];
/* 0xff-terminated byte list scanned by sub_0803BFBC; sub_08043C98 hands out
 * its address. 0x08090A5C is a compiler-made pool word holding this array's
 * address; C code names the array, never the word. */
extern u8 gUnknown_020288A0[];
/* Scratch byte; sub_0803BFA4 loads it with gUnknown_0849ECDC->unk01 + 1 and
 * sub_08043D5C zeroes it. sub_0803BF10 copies it into gPlaySt.
 */
extern u8 gUnknown_020288B0;
/* Slot indices into gUnknown_08499594: sub_0804769C reads entry i as
 * gUnknown_08499594[gUnknown_03003F2C + gUnknown_02028DD8[i]] for
 * i < p->unk21. The length comes from the caller's count. */
extern u8 gUnknown_02028DD8[];
extern u8 gUnknown_02028E3C;
extern u8 gUnknown_02028E40;
extern u8 gUnknown_02028E41[];
/* A flag byte; sub_0804AF88 tests it against 0 together with
 * gUnknown_030044E0->unk5c. */
extern u8 gUnknown_02028E48;
/* Two flag bytes 8 apart, one per side: sub_080553C8 clears [5] or [0xd] when
 * row 0 or row 1 of gUnknown_03004580 has [5] != [6]. Only these two offsets
 * are used, so the record they belong to is not declared. Signedness
 * unconfirmed. */
extern u8 gUnknown_02028E4C[];
/* Two-halfword rows indexed by gUnknown_0300453C. [i][0] is a small state flag
 * (sub_0804B3E0 compares it with 1, sub_08051F4C sets it to 1); [i][1] indexes
 * gUnknown_085644D4 and gUnknown_085644C8 in sub_0804F0D8 and sub_080501DC.
 * Signedness and row count unknown. */
extern u16 gUnknown_02028E5C[][2];
/* 0x08136030 and 0x08136034 are compiler-made pool words holding this array's
 * address, one per function; C code names the array, never the words. */
extern struct Unk02029690 gUnknown_02029690[];
/* The ROM words from 0x08136130 to 0x0813615C are also compiler-made pool
 * words, not objects. They hold the addresses of gUnknown_02029C04,
 * gUnknown_03004580 (four times), gUnknown_020296B0, gUnknown_03001470,
 * gUnknown_02029A10, gUnknown_03004548, gUnknown_03004538, gUnknown_02029BE8
 * and gUnknown_0300450C. C code names those globals, never the words. */
extern struct Unk02029A10Group gUnknown_02029A10[];
/* One byte per gUnknown_02029A10 slot, five per side; sub_08051BEC reads
 * [a][j] and skips the slot when it is 1. The number of sides is unknown. */
extern u8 gUnknown_02029C14[][5];
extern struct Unk02029BA8 gUnknown_02029BA8[];
/* 0x02029BC4 is gUnknown_02029BA8[0].unk18[1], not a separate global;
 * sub_080566C8 writes both unk18 entries. */
/* Double-buffer pointers into gUnknown_0202F8DC: sub_08073A00 points
 * gUnknown_0202FDDC at its first half and gUnknown_0202FDE0 at its second, and
 * sub_08073AE8 swaps them. sub_08073B00 feeds gUnknown_0202FDE0 to
 * REG_DMA0SAD. */
extern void *gUnknown_0202FDDC;
extern void *gUnknown_0202FDE0;
/* Source pointer for the HBlank DMA that copies one gUnknown_0202F8DC entry per
 * scanline into REG_WIN1H: sub_080735EC and sub_080737EC point it at
 * gUnknown_0202F8DC, and sub_08073930 re-arms it from gUnknown_0202FDDC.
 * 0x081CC024, 0x081CC028 and 0x081CC02C are compiler-made pool words holding
 * its address; C code names the global, never the words. */
extern void *gUnknown_0202FDE4;
/* The per-scanline window table the HBlank DMA reads: 0x280 halfwords (it ends
 * where gUnknown_0202FDDC starts), used as two halves of 0x140, two halfwords
 * for each of 0xA0 scanlines. sub_08073A00 clears both halves and publishes
 * them through gUnknown_0202FDDC, gUnknown_0202FDE0 and gUnknown_0202FDE4.
 * 0x081CC030 is a compiler-made pool word holding this array's address; C code
 * names the array, never the word. */
extern u16 gUnknown_0202F8DC[];
/* gUnknown_0202FDE8 and gUnknown_0202FDEA (declared below, after
 * gUnknown_02029F3C): the x, y halfword pair sub_08073F90 returns through its
 * two out-parameters. Must stay two scalars, not one struct: the original code
 * loads each address from its own pool word. */
/* One 0xC00-byte record per value of gUnknown_030033EC, laid out as
 *     0x000  u8 head[3][0x20];
 *     0x060  struct { u8 x, y; u8 pad[2]; u8 *cell; } rec[3][0x7c];
 * The first index is a small player or army index. head[] bytes are
 * 0xff-terminated indices into rec[]; rec[].x and .y are a map cell, and
 * x == 0xfe means move on to the next head byte. Must stay a flat u8 []: each
 * function casts onto it at the use, and the cast's spelling fixes the order in
 * which the compiler adds the index terms (see docs/agbcc-codegen.md). */
extern u8 gUnknown_02029ED8[];
/* gUnknown_02029ED8 + 0x64: the pointer field at +4 of each rec[] entry above,
 * which the original code names as its own symbol. sub_080620FC stores a
 * pointer into the map's +0x193A plane there. Must stay u8 []: the code's
 * subscript is a byte offset that a wider element type would rescale. */
extern u8 gUnknown_02029F3C[];
extern u16 gUnknown_0202FDE8;
extern u16 gUnknown_0202FDEA;
/* 0x08090F00 is a compiler-made pool word holding this global's address; C
 * code names the global, never the word. */
extern struct Unk0202FDEC gUnknown_0202FDEC;
extern struct Unk0202FDFC gUnknown_0202FDFC;
/* 0x081CC4D8 and 0x081CC4DC are compiler-made pool words, both holding the
 * address of gUnknown_0202FDFC (sub_08074BDC and sub_08074C1C each have one);
 * C code names the global, never the words. */

/* -------------------------------------------------------------- IWRAM -- */

/* 16 entries: sub_08011B18 clears indices 0..15 and sub_08011B34 stops
 * scanning at gUnknown_03000040, the next symbol in the linker script. */
/* 0x0808E518 is a compiler-made pool word holding this array's address, which
 * sub_08011B5C uses; C code names the array, never the word. The elements are
 * not declared volatile: sub_08011B98, which needs a second load of a slot,
 * reads it through a local volatile pointer. */
extern void *gUnknown_03000000[];
extern u16 gUnknown_03000040;
/* A u16 pair set together by sub_0801224C. Must stay two scalars, not one
 * struct: the original code loads each address from its own pool word. */
extern u16 gUnknown_03000044;
extern u16 gUnknown_03000046;
/* sub_0801BB88 fills both in one go, splitting the OAM shadow at object `a`:
 * the head gets (shadow, OAM, 0, a), while the tail gets
 * (shadow + a*8, OAM + a*8, a*8, 0x80-a). */
extern struct OamTransfer gOamTransferTail;
extern struct OamTransfer gOamTransferHead;
extern struct Unk03000288 gUnknown_03000288[];
/* The serial/link block, gUnknown_03000560 to gUnknown_03000574 below: plain
 * 32-bit words except gUnknown_0300056C. The low two bits of gUnknown_03000564
 * are a 2-bit state that sub_0802ED00 compares against 2. gUnknown_03000564
 * must stay volatile: sub_0802EB28 does three read-modify-writes on it in a row
 * and the original reloads it each time. */
/* A one-byte mode flag: sub_08022A08 clears it, and the four redraw wrappers at
 * 0x08023DCC-0x08023EA4 run a third pass when it is 1. */
extern u8 gUnknown_03000559;
extern u32 gUnknown_03000560;
extern volatile u32 gUnknown_03000564;
extern u32 gUnknown_03000568;
/* The last REG_SIOCNT value, latched at the top of the serial interrupt by
 * sub_0802ED40, which then copies it to gUnknown_0849B018->unk02. A halfword,
 * unlike the words around it. Must stay volatile: the original reads it back
 * straight after storing it. */
extern volatile u16 gUnknown_0300056C;
extern u32 gUnknown_03000570;
extern u32 gUnknown_03000574;
/* The link receive state, one entry per link slot (0..3), read by sub_0802F9BC:
 * gUnknown_03003128 is each slot's read cursor into the ring gUnknown_02025C18
 * and gUnknown_03003F48 its write cursor; a slot is empty when they are equal.
 * The ring is 0x400 rows of four interleaved u16 channels (the cursors wrap at
 * 0x3ff). Must stay [][4], not flat: flat indexing moves a shift. All three
 * must stay volatile: the original code reads each cell before writing it. */
extern volatile u16 gUnknown_03003128[];
extern volatile u16 gUnknown_03003F48[];
extern volatile u16 gUnknown_02025C18[][4];
/* The link send ring, 0x200 halfwords: sub_0802F03C clears exactly that many,
 * and c_0802ED40.c reads it at the read cursor gUnknown_030040CC. Must stay
 * volatile: the original clear loop reads each entry before writing it.
 * Signedness unconfirmed. */
extern volatile u16 gUnknown_02025818[];
/* A ROM table of six pointers to the link rings and their cursors. These are
 * real data, not compiler-made pool words:
 *   0x08090C80 -> &gUnknown_0300410C   send-ring write cursor
 *   0x08090C84 ->  gUnknown_02025818   send ring
 *   0x08090C88 -> &gUnknown_030040CC   send-ring read cursor
 *   0x08090C8C ->  gUnknown_03003128   receive read cursors
 *   0x08090C90 ->  gUnknown_03003F48   receive write cursors
 *   0x08090C94 ->  gUnknown_02025C18   receive ring
 * The pointee types and volatile qualifiers follow those objects. */
extern volatile u16 *const gUnknown_08090C80;
extern volatile u16 *const gUnknown_08090C84;
extern u16 *const gUnknown_08090C88;
extern volatile u16 *const gUnknown_08090C8C;
extern volatile u16 *const gUnknown_08090C90;
extern volatile u16 (*const gUnknown_08090C94)[4];
/* A word sub_0802F03C zeroes. Not volatile, unlike the other cells that
 * function clears. Signedness unknown. */
extern u32 gUnknown_0300333C;
/* A cursor into gUnknown_08090C44, zeroed by sub_0802EA5C. Must stay volatile:
 * sub_0802EC64 reloads it after incrementing it, to index the table. */
extern volatile u32 gUnknown_03000578;
/* A current/requested id pair; 0xFFFF means none (sub_0803B524, sub_0803B640
 * and sub_0803B660 compare against it). sub_0803B37C resets both to 0xFFFF;
 * sub_0803B5F4 resets only gUnknown_030005CA. */
extern u16 gUnknown_030005C8;
extern u16 gUnknown_030005CA;
/* sub_0803B350 sets it and sub_0803B37C resets it to 0x100. sub_0803B414
 * passes it as the volume argument of sub_08071420 (MPlayVolumeControl). */
extern u16 gUnknown_030005CC;
/* sub_0803B35C stores its argument here and passes it as the volume argument
 * of sub_08071420 (MPlayVolumeControl). */
extern u16 gUnknown_030005CE;
/* Flags, written 0 and 1 by sub_08057B2E and sub_08057BCC and read by
 * sub_08057CA4. */
extern s16 gUnknown_030005E8[];
/* Two 16.16 fixed-point pairs filled by sub_08057AE8: gUnknown_030005D8[i] is
 * gUnknown_02029B78[i] << 16 (the current value) and gUnknown_030005E0[i] is
 * (d << 16) / (d / 5 + 20), the per-frame step, where
 * d = gUnknown_02029B78[i] - gUnknown_02029B7C[i]. 0x081361A0 and 0x081361AC
 * are compiler-made pool words holding their addresses; C code names the
 * globals, never the words. */
extern int gUnknown_030005D8[];
extern int gUnknown_030005E0[];
extern u8 gUnknown_03000650[];
/* A pointer to code copied into RAM: sub_0808AD6C copies the 4-byte body of
 * sub_0808AD68 (return the byte at p) into a caller's buffer and stores
 * buffer + 1 (the THUMB bit) here. sub_0808ADA4 calls it. */
extern u8 (*gUnknown_03000F6C)(u8 *);
/* 0x03000F68 to 0x03000F7C are the state of a driver for one hardware timer,
 * chosen out of the four when it is registered: sub_0808AC44 registers it
 * (it rejects ids above 3 and hands back sub_0808AC20 as the tick),
 * sub_0808AC7C arms it, sub_0808AC20 ticks and sub_0808AD24 disarms it. */
/* A table of 6-byte records indexed by slot id, read by sub_0808AC7C when it
 * arms the timer: +0x00 the frame countdown (copied to gUnknown_03000F72),
 * +0x02 the TMnCNT_L reload value, +0x04 the TMnCNT_H control value. */
extern u16 *gUnknown_03000F68;
/* The hardware timer index, 0-3. It selects the timer registers and is the
 * shift count for the timer's IRQ bit (sub_0808AD24). Must stay volatile:
 * sub_0808AC44 reads it back straight after storing it. */
extern volatile u8 gUnknown_03000F70;
/* The frame countdown, loaded by sub_0808AC7C and ticked down by sub_0808AC20.
 * Must stay volatile: sub_0808AC20 reads it twice, to test it and to decrement
 * it. */
extern volatile u16 gUnknown_03000F72;
/* The expiry flag: sub_0808AC7C clears it when the timer is armed and
 * sub_0808AC20 sets it to 1 when gUnknown_03000F72 reaches zero. Only stored,
 * so its width is not confirmed. */
extern u8 gUnknown_03000F74;
/* &REG_TMnCNT_L of the selected timer (0x04000100 + index * 4, built by
 * sub_0808AC44): [0] is TMnCNT_L and [1] TMnCNT_H. sub_0808AC7C and
 * sub_0808AD24 step this global itself (`*g++ = ...; *g-- = ...`) rather than a
 * local. */
extern vu16 *gUnknown_03000F78;
/* The saved REG_IME value: sub_0808AC7C saves it before masking interrupts and
 * sub_0808AD24 restores it. */
extern u16 gUnknown_03000F7C;
/* gUnknown_030013EC, declared below gUnknown_0816D948: a callback slot that
 * sub_0801F4A4 sets to sub_0801F4B4 and many functions call through. Declared
 * without a parameter list, so its calls are not type-checked. */
/* 0x0816D948 is a compiler-made pool word holding the address of
 * gUnknown_030013B0 (gBattleDefender), one of a run of pool words at
 * 0x0816D938-0x0816D9BC. sub_08058A2C names gBattleDefender instead; nothing
 * in src/ uses this extern. */
extern u8 *const gUnknown_0816D948;
extern void (*gUnknown_030013EC)();
/* A BG scroll shadow that sub_08012420 writes to REG_BG2VOFS. Must stay
 * volatile: sub_0807A0C4 reads it back straight after storing it. */
extern volatile u16 gUnknown_03001400;
/* A flag set to 1 and 0 by c_08017E74.c. Must stay s16: sub_08019470's zero
 * test loads it sign-extended, which a cast on a u16 does not reproduce. */
extern s16 gUnknown_03001404;
/* A name buffer: its address goes to sub_0803CCB8, which copies a
 * NUL-terminated name (at most 0x11 bytes) into it from
 * gUnknown_020280C0[i].unk02. Its length is unknown. */
extern u8 gDesignRoomName[];
/* A BG scroll shadow that sub_08012420 writes to a BGxOFS register, like its
 * neighbour gUnknown_03001FF8. Must stay volatile: sub_08019C40's chained
 * assignment `gUnknown_03001FF8 = gUnknown_03001418 = 0` reads it back straight
 * after storing it. */
extern volatile u16 gUnknown_03001418;
/* Must stay volatile: sub_08017EEC reads it twice (into gUnknown_030030A8, and
 * for gUnknown_03001FF4 = it + 0x6f) and the original loads it both times. */
extern volatile u16 gUnknown_03001420;
/* At least 0x20 slots: sub_0801DA94 clears [0..0x1f] and sub_0801DAB0 scans
 * them. sub_0801DAE8 takes an s16 index and skips -1, so -1 means "no slot". */
extern s16 gUnknown_03001430[];
/* 0x0808E534 and 0x0808E538 are compiler-made pool words holding this array's
 * address, one per function; C code names the array, never the words. */
extern struct Unk03001470 gUnknown_03001470[];
/* 0x0813606C and 0x08136070 are compiler-made pool words holding this global's
 * address (one each for sub_0804D0FC and sub_0804D4E8), and 0x08136074 one
 * holding gUnknown_03001470's; C code names the globals, never the words. */
extern s16 gUnknown_03001FBC;
/* gUnknown_03001FDC must NOT be declared in this header. src/proc.c defines it
 * with an IWRAM_DATA section attribute and includes this header; an extern seen
 * first makes agbcc drop the attribute, the linker then moves the symbol, and
 * three pool words in the ROM change. Per-function checks do not catch this;
 * only the full ROM checksum does. The same holds for any global defined in
 * src/proc.c or src/title-screen.c. */
/* The HBlank scanline cursor sub_08011228 walks; sub_08011298 seeds it with 0
 * and sub_0801137C with 0x140. 0x0808DF94 is a compiler-made pool word holding
 * its address; C code names the global, never the word. */
extern volatile s16 gUnknown_03001408;
/* The companion scroll accumulator: sub_080114A0 advances it by the proc's
 * step and clamps it to 0x140. */
extern volatile s16 gUnknown_03002F3C;
/* A counter that sub_080501DC increments on one branch. Signedness
 * unknown. */
extern u16 gUnknown_03004544;
/* An s16 counter used only at [2]: sub_08063BBC decrements it and sub_08063BE0
 * tests it against 0. Declared as an array because no other offset is used; its
 * length is unknown. */
extern s16 gUnknown_0202F0E8[];
extern u32 gUnknown_03001FD4;
extern u32 gUnknown_03001FE0;
/* A ROM table of six 12-byte rows: {graphics blob, palette, base tile/palette
 * index}. sub_0801F178 hands unk04 to ApplyPaletteExt; the six unk08 values are
 * what sub_0801F400 returns per row and what sub_0801F3D4 compares against. */
struct Unk0848B738 /* 0x0c */
{
    /* 0x00 */ const void *unk00;
    /* 0x04 */ u16 *unk04;
    /* 0x08 */ int unk08;
};
extern const struct Unk0848B738 gUnknown_0848B738[];
/* A ROM table indexed by a1 in sub_0801F19C and sub_0801F234, 4 bytes per
 * entry; unk00 and unk01 are a width and height, multiplied to give a tile
 * count. */
struct Unk0848B780
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 filler_02[0x02];
};
extern const struct Unk0848B780 gUnknown_0848B780[];
/* The original table was probably not const: sub_0801F2AC casts the const
 * away at its two reads, because with const the compiler hoists those reads out
 * of its loops and the original does not. sub_0801F444 matches either way. If
 * you drop the const, remove those casts and re-check sub_0801F2AC,
 * sub_0801F34C and sub_0801F444. */
/* A ROM table of OAM object lists indexed by a sprite's size in tiles:
 * sub_0801F34C reads entry [height * 17 + width] (from gUnknown_0848B780) and
 * passes it to PutSpriteExt. Row width 17; the number of rows is unknown. */
extern u16 *const gUnknown_0848BAE4[];
/* Two parallel ROM tables of pointers indexed by the effect id sub_08022DD4
 * computes: gUnknown_0848ABF4[id] goes to sub_0801BD00 and gUnknown_0848AE98[id]
 * to sub_0801C01C. */
extern void *const gUnknown_0848ABF4[];
extern void *const gUnknown_0848AE98[];
/* A word that sub_08036C4C and sub_08036C80 both set to 0xE28. Nothing reads
 * it; purpose unknown. */
extern u32 gUnknown_030032CC;
/* A six-byte per-army tally used only by sub_08021810: it clears [0..5],
 * counts map cells into [5] and into [1..4] per army, and folds the largest
 * into [0]. 0x0809096C is a compiler-made pool word holding its address.
 * Nothing in asm/ names this address, so aw2bhr.lds defines the symbol by
 * hand. */
extern u8 gUnknown_030032D0[];
/* A sprite/animation descriptor blob: sub_080272C4 and sub_08027428 pass its
 * address to sub_0801C210. Only the address is used. */
extern u8 gUnknown_08112614[];
/* An OAM object counter: sub_0801DF94, sub_0801EF6C and sub_0801EFA8 zero it,
 * and sub_0801EE80 counts it up in its sprite-emit loop. Must stay volatile:
 * sub_0801EE80 reads it twice with no store between, and the original loads
 * it both times. */
extern volatile u16 gUnknown_03001FE4;
/* A callback: sub_080198F0 calls it when it is not null and tests the result.
 * sub_080198C4 and sub_08017F0C clear it; sub_080171B4 and sub_08017540 save
 * and restore it with gUnknown_03002F20. */
extern bool8 (*gUnknown_03001FF0)(void);
extern u16 gUnknown_03001FF4;
/* The BG0 x scroll shadow. Must stay volatile: sub_0801258C packs it with the
 * BG0 y shadow as x | (y << 16), and only a volatile read gives the original's
 * operand order. */
extern volatile u16 gUnknown_03001FF8;
/* gUnknown_03002B6C must NOT be declared here: include/hardware.h declares it
 * as union BgCntBuf, and a second declaration with another type breaks every
 * compile. Code that needs its bitfields includes hardware.h. */
/* The wipe-transition tile data that sub_0802BE28 and sub_0802BE80 pass to
 * Decompress. Not const, like the other Decompress sources. */
extern u8 gUnknown_0810BDC0[];
extern u16 gUnknown_03002000;
extern u16 gUnknown_0300200C;
extern struct Unk03002040 gUnknown_03002040;
/* The frame (gGameClock) of sub_08064474's last call. With the position it
 * drew at (gUnknown_03000600 and gUnknown_03000602), it lets a call on the
 * next frame average the old and new positions. 0x0816E0B0 and 0x0816E0B4 are
 * compiler-made pool words holding the addresses of gUnknown_03000600 and
 * gUnknown_03000602; C code names the globals, never the words. */
extern s32 gUnknown_030005FC;
extern struct Unk030020A8 gUnknown_030020A8;
/* gUnknown_030020B4 is the REG_DISPSTAT shadow -- declared in hardware.h */
extern u8 gUnknown_030020B8;
extern u8 gUnknown_030024E4;
/* 32 slot-in-use flags. Must stay volatile: the original code reads each
 * element around writing it (sub_0801DF94, sub_0801E13C, sub_0801E17C). */
extern volatile u8 gUnknown_030024F0[];
/* A small mode enum, 0/1/2. sub_08013D40 and sub_08014BB4 clear it,
 * sub_08014468 sets it to 1, and sub_080145BC resets 2 to 0. */
extern u8 gUnknown_03002514;
/* gUnknown_0300251C, the REG_BG3CNT shadow, is declared in hardware.h with the
 * other BG-control shadows as union BgCntBuf. Code that writes it whole spells
 * it *(u16 *)&gUnknown_0300251C. */
extern u16 gUnknown_03002520[];
/* An OAM shadow: sub_0801EFD8 copies 0x380 bytes of it to OAM entries 4..127
 * (0x07000080) through sub_08011C90. Its real length is not known. */
extern u8 gUnknown_030025A0[];
/* The x half of a scroll origin (the y half is gUnknown_030030D0) that
 * sub_08012420 subtracts from gUnknown_03001418 / gUnknown_03001FF8 when it
 * writes BGxHOFS; sub_08013324, sub_08013388 and sub_0801339C clear both. Must
 * stay volatile: sub_08012420 reloads it for each of its eight subtractions. */
extern volatile u16 gUnknown_03002B20;
extern struct SpriteEntry *gUnknown_03002B24;
/* A 0/1 toggle: sub_080129B4 flips it (1 - x) and writes it into REG_BLDCNT's
 * effect field. Must stay volatile: that function reads it back after storing
 * it. */
extern volatile u16 gUnknown_03002B2C;
extern u8 gUnknown_03002B30;
/* The BG1 x scroll shadow, twin of gUnknown_03001FF8. Must stay volatile for
 * the same reason: sub_0801258C packs it with gUnknown_03002F18. */
extern volatile u16 gUnknown_03002B34;
/* Set to 1 by sub_08017970 and to 0 by sub_0801797C; sub_08017988 returns
 * it. */
extern s16 gUnknown_03002B38;
extern u8 gUnknown_03002B40;
extern u8 gUnknown_03002B44;
extern u8 gUnknown_03002B4C;
/* The OAM-shadow write cursor the sprite-emit path advances: sub_0801DF94 sets
 * it to 0x10, sub_0801E0F0 clears it and sub_0801E0BC returns it. Must stay
 * volatile: in sub_0801DFE8 only a volatile read gives the original's separate
 * sign extension and load order. */
extern volatile u16 gUnknown_03002B54;
/* A 0/1 phase flag. Six users (sub_08011050, sub_080110E8, sub_08011290 and
 * others) test it and then store the other value: switch state, or bail out if
 * already there. */
extern u16 gUnknown_03002B5C;
extern u8 gUnknown_03002B68;
extern struct Unk03002B80 gUnknown_03002B80;
/* Compared against 0 and 1. sub_080199C4 clears it, sub_080199D0 sets it from
 * a u8 argument, and sub_08019674 copies a u16 into it. */
extern u16 gUnknown_03002EE4;
/* A display shadow that c_080128D0.c copies into REG_WIN0V's high byte. Must
 * stay volatile: sub_08005F4C steps it and reads it back straight after storing
 * it. */
extern volatile u8 gUnknown_03002EFC;
extern u16 gUnknown_03002F00;
extern u16 sModifiedBGs;
/* The BG1 y scroll shadow. Must stay volatile: sub_0804BA4C ends with the
 * self-assignment g = g, which the compiler deletes for a plain object. */
extern volatile u16 gUnknown_03002F18;
extern struct Unk03002F08 gUnknown_03002F08;
/* A halfword selector; sub_080184A4 uses it as an s16 index into
 * gUnknown_0848A370. Must stay volatile: only a volatile u16 read with an (s16)
 * cast gives the original's load-then-shift code. */
extern volatile u16 gUnknown_03002F90;
/* A one-shot "the next command is a jump" flag for the gUnknown_03001470
 * script interpreter. sub_08013028, sub_080160DC and sub_0803BF70 set it;
 * sub_08016094 then clears it and loads the script cursor from the word the
 * current command points at, instead of stepping 8 bytes on. */
extern u16 gUnknown_03002F1C;
/* A callback like gUnknown_03001FF0: sub_080183C0 calls it when it is not null
 * and ignores the result. Set by sub_080198A0 and by sub_08018B18 (from a list
 * node), cleared by sub_080198AC. */
extern void (*gUnknown_03002F20)(void);
/* The stack cursor for gUnknown_03002F50 below. */
extern struct Unk03002F50 *gUnknown_03002F24;
/* Read as s16 by sub_08011BC4. Must stay a volatile u16: an s16 or a plain u16
 * gives a different sign-extending load. */
extern volatile u16 gUnknown_03002F30;
extern struct Unk03002F50 gUnknown_03002F50[];
extern void *gUnknown_03002FA0[];
/* 0x40 bytes = 16 entries. The IRQ handler table: crt0.s indexes it by the
 * interrupt's word offset and `bx`es to the entry; sub_0801BAE0 fills entries
 * 0..14 with sub_0801BAB8. sub_0801BB00(index, handler) is the setter. */
extern void *gUnknown_03002FE0[];
/* sub_0801BABC, the IRQ setup, fills gUnknown_03002FE0[0..14] with the
 * do-nothing handler sub_0801BAB8, copies crt0.s's ARM IrqMain (0x40 words)
 * into gUnknown_03000068, and stores the copy's address in gUnknown_0200BFFC.
 * gUnknown_0200BFFC overlapping gUnknown_0200BC14 is expected. */
extern u8 gUnknown_03000068[];
extern void *gUnknown_0200BFFC;
/* Indexed by a u16 argument in sub_08010EE8. */
extern void *gUnknown_03003050[];
extern u16 gUnknown_030030A0;
/* Must stay volatile: sub_08017880 reads it four times with no call between,
 * and the original loads it every time. */
extern volatile u16 gUnknown_030030A8;
extern u16 gUnknown_030030C4;
/* The y half of the scroll origin pair whose x half is gUnknown_03002B20 --
 * volatile for the reason recorded there. */
extern volatile u16 gUnknown_030030D0;
extern volatile u16 gUnknown_030030E8;
extern union Unk802C57CBuf gUnknown_03003100;
/* At least 3 bytes: sub_08035568 clears [0..2] through a variable index.
 * Signedness unknown. */
extern u8 gUnknown_03003124[];
extern struct Unk03003130 gUnknown_03003130;
extern u16 gUnknown_030032C0;
extern struct Unk802C57C gUnknown_030032C4;
extern u16 gUnknown_030032D8;
/* sub_0802C480 zeroes it on every call, after its guarded coordinate
 * update. */
extern u16 gUnknown_03003334;
/* 0x08091310 and 0x08091314 are compiler-made pool words holding this
 * global's address (one each for sub_080415E4 and sub_080416A4); C code names
 * the global, never the words. */
extern struct Unk03003338 *gUnknown_03003338;
/* A row-pointer table for the gUnknown_08499590 map: sub_0801F838 fills
 * gUnknown_03003340[y][x] for each row. The cells are really signed (negative
 * means none: sub_08020020 and sub_08059050 test < 0 and sub_0802E2D0 stores
 * -1), but the rows are declared u8 *, so those functions cast a row to (s8 *)
 * at the use. Retyping it changes every reader; check them all. */
extern u8 *gUnknown_03003340[];
/* The movement-range flood fill (sub_0801F6F0, driven by sub_0801F4B4).
 * gUnknown_084999C8 is a ROM word holding 0x03003400, the fill's state block
 * (struct Unk84999C8): +0x00 a 0x20-byte cost table (sub_0801F888 fills it
 * from terrain movement costs), s16 bounds at +0x20/+0x22, a 4-entry mask
 * table at +0x24 indexed by the unit plane's top two bits and ANDed with +0x2a,
 * and the map's width/height at +0x28/+0x29 (sub_0801F92C stores them).
 * gUnknown_03003F64 is the write cursor (a pointer variable, not an array) into
 * a queue of 4-byte records, and gUnknown_030040E0 the queue length, capped at
 * 0x15C. 0x08090928, 0x0809092C, 0x08090930, 0x08090934 and 0x08090938 are
 * compiler-made pool words holding the addresses of gUnknown_0300409C,
 * gUnknown_084999C8, gUnknown_03003340 and gUnknown_08499590 (twice); C code
 * names the globals, never the words. */
/* One flood-fill queue record: x, y, a tag (unk02) and a cost (unk03).
 * sub_0801F6F0 writes records through gUnknown_03003F64; sub_0801F4B4 reads
 * them through gUnknown_0300409C, switches on the tag (1..5) and clears it. */
struct Unk300409C
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
};

struct Unk84999C8
{
    /* 0x00 */ u8 unk00[0x20];
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24[4];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2a */ u8 unk2a;
};

extern struct Unk300409C *gUnknown_0300409C;
extern struct Unk84999C8 *gUnknown_084999C8;
extern u8 *gUnknown_03003F64;
/* The flood fill's double-buffer bit: at the top of each pass sub_0801F4B4
 * tests it and swaps which of the two queues at gUnknown_084999C8 + 0x2c and
 * + 0x5A4 gUnknown_03003F64 writes and gUnknown_0300409C reads. 0x08090920 is a
 * compiler-made pool word holding its address; C code names the global, never
 * the word. */
extern u8 gUnknown_03003FBC;
extern int gUnknown_030040E0;
extern struct Unk802C57C gUnknown_030033E0;
extern struct Unk802C57C gUnknown_030033E4;
/* The current army number (gUnknown_03003F2C is (it - 1) * 0x40).
 * sub_08017658 and sub_0807A934 read only its low byte. Must stay non-volatile:
 * many matched functions read it. sub_080350E4 needs one volatile read and
 * spells it *(vu16 *)&gUnknown_030033EC at the use. */
extern u16 gUnknown_030033EC;
/* A bit set of at least 4 bytes: sub_0803CB8C clears [0..3] and sub_0803CB74
 * tests bit id & 7 of byte id >> 3. */
extern u8 gUnknown_030033F4[];
/* A small state id held in a word: sub_08080FB8 tests it against 6 and
 * advances it to 0xc. */
extern u32 gUnknown_030033FC;
/* A second pointer to the scratch buffer gUnknown_03003338 points at:
 * sub_0803486C stores gUnknown_0849FE74[0] in both. sub_0804151C and
 * sub_08041758 walk it as 4-byte records {u8 x; u8 y; s16 v;}, ended by
 * v == 0xFFFF, through a local struct; sub_080415E4 and sub_080416A4 walk the
 * same buffer through gUnknown_03003338 as 8-byte records. The buffer holds
 * either layout; do not reshape struct Unk03003338 for the 4-byte one. */
extern struct Unk03003338 *gUnknown_03003F20;
extern union Unk802C57CBuf gUnknown_03003F24;
/* The first gUnits slot of the current army, (gUnknown_030033EC - 1) * 0x40;
 * the army scanners loop over the 0x40 slots from here. 0x0816D994, 0x0816D998,
 * 0x0812A0F4 and 0x0812A0F8 are compiler-made pool words holding its address;
 * C code names the global, never the words. */
extern u16 gUnknown_03003F2C;
/* A 5-byte copy sub_0803D2F8 fills from its record argument's +0x4C4..+0x4C8;
 * sub_0803D4A8 copies the same five bytes into struct Unk020280C0's +0x14
 * bytes. At least 5 bytes; the real length is not known. */
extern u8 gUnknown_03003FF3[];
/* At least 7 signed bytes: sub_0803BBD4 clears [0..6]. The getters at
 * 0x0803BC7C, 0x0803BC88 and 0x0803BC94 return them as u8; sub_0803BB44,
 * sub_0803BB5C and sub_0803BB74 return 1 when element 1, 2 or 3 is zero and 2
 * otherwise. */
extern s8 gUnknown_03003F30[];
/* sub_08022DD4 stores its (x, y, effect id) here and reads x and the id back.
 * Only these three members are known; the object's size is not. */
struct Unk03003F58
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
};
extern struct Unk03003F58 gUnknown_03003F58;
extern int gUnknown_03003F40;
/* The current map's base tile layout: LoadMapData (sub_080247A4) allocates it
 * with sub_08014E44 and decompresses the map into it. Byte width and height,
 * then width * height u16 tiles. sub_08016F38 diffs gMap->tile against it to
 * save only the changed cells. The tile count is not fixed. */
struct MapLayout
{
    /* 0x00 */ u8 width;
    /* 0x01 */ u8 height;
    /* 0x02 */ u16 tile[1];
};
extern struct MapLayout *gUnknown_03003F68;
extern struct Unk0202575C gUnknown_0202575C;
/* Both must stay volatile: sub_08032048's chained assignment
 * `gUnknown_03003F1C = gUnknown_030044C4 = 0` reads gUnknown_030044C4 back
 * after storing it, and sub_080319AC's compare of gUnknown_03003F1C with
 * gUnknown_0849B018->unk09 only matches with a volatile read. */
extern volatile u8 gUnknown_030044C4;
extern volatile u8 gUnknown_03003F1C;
/* The send-ring write cursor (see gUnknown_08090C80). sub_08031B30 and
 * sub_08032104 set it to gUnknown_030040CC. Must stay volatile: in
 * sub_0802F03C's `gUnknown_030040CC = gUnknown_0300410C = 0` the original reads
 * it back after storing it. */
extern volatile u16 gUnknown_0300410C;
/* The send-ring read cursor: sub_0802ED40 reads gUnknown_02025818 at it while
 * it differs from gUnknown_0300410C, then advances it (`++`, then `&= 0x1ff`).
 * Must stay volatile: the original reloads it between those two statements. */
extern volatile u16 gUnknown_030040CC;
/* Must stay volatile: sub_08030768's chained assignment
 * `gUnknown_03003F1C = gUnknown_030044C4 = gUnknown_030040AC = 0` reads it back
 * after storing it. */
extern volatile u8 gUnknown_030040AC;
extern struct PlaySt gPlaySt;
/* gUnknown_03003F60, declared below gUnknown_03003F3C: the sub-state of the
 * screen sub_08034350 runs. It dispatches on it; sub_08034394 and sub_080343D8
 * set it to 4 and 0. */
/* A word sub_0802150C sets to 1 in its reset run. Signedness unknown. */
extern u32 gUnknown_03003F3C;
extern u16 gUnknown_03003F60;
/* At least 5 bytes, indexed 1..4 by player slot (index 0 is unused, as in
 * gUnknown_0810E6E0); sub_08026340 counts the non-zero entries. */
extern u8 gUnknown_03003FF8[];
/* gUnknown_03001FE8, the REG_BG1CNT shadow, must NOT be declared here:
 * include/hardware.h declares it as union BgCntBuf, and a second declaration
 * with another type breaks every file that includes both. Include hardware.h
 * and use its .bits / .raw members. */
extern u16 gUnknown_03004080;
/* Two halfword selectors sub_080210C8 picks between by kind (cases 1 and 3 the
 * first, case 2 the second); each indexes a run of 0x508-byte records at
 * gUnknown_08499590 + 0x1E42. Signedness unknown. */
extern u16 gUnknown_03004070;
extern u16 gUnknown_03004088;
extern struct Unk802C57C gUnknown_03004090;
extern struct Unk802C57C gUnknown_030040A4;
/* A word; sub_0804138C clears it and sub_08041398 reads it. Callers keep only
 * the low halfword (sub_08029234 stores it as one). */
extern u32 gUnknown_030040A8;
/* A callback slot installed by sub_0803662C; sub_080366F4 calls it when it is
 * not null. */
extern void (*gUnknown_030040D0)(void);
/* Written once, by sub_0803C52C, with sub_08037DA4's result (2..5); nothing
 * reads it. int to match sub_08037DA4's return type. */
extern int gUnknown_030040D4;
extern struct Unk030040D8 *gUnknown_030040D8;
/* A one-shot argument: sub_08042998 passes it to sub_08025B80 and then clears
 * it. */
extern u8 gUnknown_03004074;
/* A mode flag; sub_0805C974 returns whether it is 1. Signedness unknown. */
extern u8 gUnknown_030040DC;
/* A flag: sub_08035760 sets it to 1, sub_08035828 and sub_0803647C clear it,
 * and sub_080345C8 reads it. */
extern s16 gUnknown_030040E4;
/* A counter: sub_08034FB0 increments it and sub_08034FC0 decrements it when it
 * is not 0. */
extern s16 gUnknown_030040E8;
extern void (*gUnknown_030040EC)(void);
/* One byte per player slot (5). sub_0803D6B8 fills it with 0..4; sub_0803D6D0
 * refills it from gPlayers[i].unk1a and sub_0803D6FC from a caller's buffer at
 * +0x4C4. */
extern u8 gUnknown_030040F8[];
/* Two halfword offset tables that sub_08037A78 adds to a destination pointer:
 * gUnknown_03004010[x] by map column and gUnknown_030032E0[y] by map row.
 * Signedness unknown. */
extern u16 gUnknown_030032E0[];
extern u16 gUnknown_03004010[];
/* The frame counter; code also tests single bits of it. Really unsigned
 * (sub_08022A6C shifts it logically, sub_0806F064 takes an unsigned modulo),
 * but it must stay s32: src/title-screen.c, upstream's source, declares it
 * that way. sub_0806F064 casts it to u32 instead. */
extern s32 gGameClock;
/* A frame mask ANDed with gGameClock in sub_080369BC; sub_08036B28 and
 * sub_08036B34 clear it. */
extern u16 gUnknown_030043F4;
extern struct Unk802C57C gUnknown_030044A4;
/* The handle of the active proc of the 0x0849FB04 family: sub_0803FF48 sets it
 * from its third argument and stores it at +0x54 of the proc it starts;
 * sub_0803FF2C ends every such proc whose +0x54 differs. Never
 * dereferenced. */
extern int gUnknown_030044D4;
/* A parking slot for gUnknown_030032D8: sub_08028CD8 moves gUnknown_030032D8
 * here and writes 0x10 in its place; sub_08034ED0 moves it back and zeroes
 * this one. sub_080346BC compares it with 0xe. */
extern u16 gUnknown_030044DC;
extern struct Unk030044E0 *gUnknown_030044E0;
extern struct Unk03004504 gUnknown_03004504;
/* gUnknown_0300450C, gUnknown_0300451C and gUnknown_0300453C are read by
 * sub_0804D928, sub_0804E3B4 and about 40 functions between 0x0804B180 and
 * 0x08053614. gUnknown_0300453C selects a row of gUnknown_08551D0C, sets the
 * sprite's horizontal flip (XOR 1), and XORed with gUnknown_0300450C indexes
 * gUnknown_085523A4. gUnknown_0300451C is copied to gUnknown_03001470[i].unk34
 * and never examined. Must stay non-volatile: the original keeps one loaded
 * value across several stores. */
extern u16 gUnknown_0300450C;
/* A frame counter for the 0x08053xxx cutscene player: sub_0805316C increments
 * it and ends its proc at 0x12C; sub_08053FBC, sub_0805414C, sub_08054278 and
 * sub_08054488 compare it with script timestamps. Signedness unknown. */
extern u16 gUnknown_03004508;
/* The u16 script ids sub_08053F0C indexes with gUnknown_0300450C to pick an
 * entry of gUnknown_08553734. */
extern u16 gUnknown_030045A0[];
/* A ROM table of cutscene step handlers: sub_08053F0C calls entry
 * gUnknown_030045A0[gUnknown_0300450C], or entry [2] when the flags at
 * 0x03004504 say so. */
extern void (*const gUnknown_08553734[])(void);
/* A flag byte for the 0x08053820 stage machine: bits 0-1 are a small state
 * (compared with 1 under a & 3 mask), and bits 1, 4 and 7 are set as the stage
 * progresses. */
extern u8 gUnknown_02029664;
extern u16 gUnknown_0300451C;
extern u16 gUnknown_0300453C;
/* A continuation callback: sub_0807166C, a bytecode opcode handler, calls it
 * with its own two pointer arguments when its predicate holds. No writer is
 * known, so the parameter types are a guess. */
extern void (*gUnknown_03005744)(void *, void *);
/* A per-side byte pair: sub_08057AE8 multiplies [0] and [1] by 0x20 to pick a
 * gUnknown_0816D498 palette bank. */
extern u8 gUnknown_03004500[];
extern u16 gUnknown_03004518;
extern u16 gUnknown_03004538;
/* One 16-byte row per gUnknown_0300453C, reached through two overlapping
 * symbols: gUnknown_03004582 is the same memory 2 bytes in. Both must stay
 * declared: sub_0804D290 and sub_0804DCA8 load both addresses.
 *   gUnknown_03004582[i][0]  index into gUnknown_085D6A48, and the outer index
 *                            of gUnknown_08552FB8
 *   gUnknown_03004580[i][2]  the middle index of gUnknown_08552FB8
 *   gUnknown_03004580[i][3]  the second argument of sub_08057D44
 *   gUnknown_03004580[i][5]  the row index into the table sub_08057D44 returns
 * The row length 8 is the stride; the row count is unknown. Must stay a 2-D
 * array, not an array of structs: the column reads compile differently (see
 * "column-offset fold" in docs/agbcc-codegen.md). */
extern u16 gUnknown_03004580[][8];
/* 0x03004588 is gUnknown_03004580[i][4], not another object: an offset of 8
 * or more past a base gets its own pool word, which the disassembly shows as a
 * new symbol. Check the offset before declaring one. */
extern u16 gUnknown_03004582[][8];
/* A u16 lookup table read by sub_0804B830 with an unchecked u16 index; its
 * length is unknown. */
extern u16 gUnknown_030045A8[];
/* Really volatile: sub_0805FD64 switches on it and needs the read
 * `(s16)*(volatile u16 *)&gUnknown_030045D4` to match. It is not declared
 * volatile because eight files store to it (c_08034394.c, c_080600F0.c,
 * c_080601F0.c, c_08060424.c, c_080604A4.c, c_080604F4.c, c_0806050C.c,
 * c_08060554.c). If you add the qualifier, re-check those and drop the cast in
 * c_0805FD64.c. */
extern u16 gUnknown_030045D4;
/* Used by sub_08060A7C, sub_0806050C and sub_08060930.
 * gUnknown_030045D8: a word compared (signed) with sub_08057FA8's result.
 * gUnknown_030045E0: two proc pointers, each passed to sub_08035828 when it is
 * not null.
 * gUnknown_030046B8: a mask of the facility kinds on this map, built by
 * AiScanBuildableFacilities (sub_08061F34): bit 0 an airport, bit 1 a port.
 * SRR_AW2's Definitions.s calls it aiUnitType, which is wrong. */
extern int gUnknown_030045D8;
struct Unk35828Proc;
extern struct Unk35828Proc *gUnknown_030045E0[];
extern u8 gUnknown_030046B8;
/* sub_0805A95C's only output: it stores
 * gUnknown_0857680F[gUnknown_030040D8->unk00], or 0 when that byte is 4 and the
 * current unit's three-bit +0x09 field is clear. Signedness unknown. */
extern u8 gUnknown_030046AC;
/* A zero-terminated list of small ids: sub_0805CDF0 and sub_0805CE20 seed it
 * with {0x40, 0} and point gUnknown_030046B0 at it; sub_0805D344 walks it and
 * sorts it together with the keys in gUnknown_030046E0. It, gUnknown_030046E0
 * and gUnknown_030046B0 must stay volatile: the original sort reads each
 * element before writing it. c_0805CDF0.c's locals are volatile to keep the
 * qualifier. */
extern volatile u8 gUnknown_030045F0[];
/* sub_0805D344's sort keys, one unsigned byte per gUnknown_030045F0 entry,
 * filled with sub_08042D1C's results. Volatile for the reason given on
 * gUnknown_030045F0. */
extern volatile u8 gUnknown_030046E0[];
/* The 20-byte command block sub_080308B4 copies: sub_080344B4 and
 * sub_08034534 fill bytes +0, +1, +6, +7 and +0x12 and pass it as
 * sub_080308B4's source. sub_0803446C also fills +2..+5 and hands
 * gUnknown_030044B0 + 0xc to sub_08034400 as a 6-byte destination. */
extern u8 gUnknown_030044B0[];
/* Read cursor into gUnknown_030045F0 -- sub_0805D438 dereferences it with
 * `ldrb`, compares against the 0x40 sentinel and bumps it by 1. */
extern volatile u8 *gUnknown_030046B0;
/* A word four bytes past gUnknown_030046B0 and unrelated to it: sub_08077F30
 * sets it to gUnknown_08615194[i].unk24 or .unk28, chosen by
 * IsHardCampaignMode(). sub_080607E8 reads three unit ids per day slot from
 * it. */
extern void *gFactoryUnitSchedule;
/* 64 bytes, indexed & 0x3f. Must stay volatile: the original code reads each
 * element around writing it (sub_0805AC88, and the increment at 0x0805AC28). */
extern volatile u8 gUnknown_03004730[];
/* gUnknown_03004778, declared below gUnknown_03004770: a per-mode step
 * callback that sub_0805D438 calls; sub_0805CDF0 and sub_0805CE20 set it to
 * sub_0805DB64 and sub_0805DB70. */
/* A cursor into the two 0x08576xxx function-pointer tables: sub_08061B00 calls
 * tbl[g] and increments it, sub_08061908 resets it to 0, and sub_0805C1C4 and
 * sub_0805C208 compare it (unsigned) with 1. */
extern u32 gUnknown_03004770;
extern void (*gUnknown_03004778)(void);
/* gUnknown_03004780, declared below gUnknown_030045DC: sub_0805D338 sets it to
 * 5, sub_0805FFA0 to 2, and sub_08062DF0 copies gUnknown_030045DC into it.
 * sub_080343D8 reads it as s16. sub_0806171C loads it unsigned and then
 * sign-extends it, which no reading of this s16 gives; c_0806171C.c reads it
 * through a volatile pointer to reproduce that. */
/* sub_08062DF0 copies it into gUnknown_03004780; nothing else uses it.
 * Signedness unknown. */
extern u16 gUnknown_030045DC;
extern s16 gUnknown_03004780;
/* A word flag: sub_0802F3D8 stores into it, and the 0x0805Cxxx list builders
 * pass it to sub_0805D344, which tests it against 0. */
extern u32 gUnknown_0300477C;
/* The record payload array the gUnknown_03005944 cursor family writes into:
 * sub_08078608, sub_08078658, sub_080786A4 and sub_080786F0 write three bytes
 * per record, and sub_08044C10 and the 0x0807Cxxx readers read them back.
 * Indexed by a caller-supplied cursor, not by gUnknown_03005944. */
extern u8 gUnknown_030058E0[];
/* A word that sub_08080A70 adds to a gUnknown_030059A0 entry to form
 * PutSprite's y argument. Signedness unknown. Not const: the original reloads
 * it on every pass of the loop. */
extern u32 gUnknown_030058D0;
/* The compacted copy of the first sub_0803BD14() entries of gUnknown_030058E0
 * that sub_0807F434 builds, handing each byte to sub_08043B14; sub_08043BA4
 * reads it back. Its length is not fixed. */
extern u8 gUnknown_030058D4[];
/* Per-column sprite y offsets, indexed by a loop counter up to proc->unk58 in
 * sub_0807FC70 (which adds 8) and sub_08080A70 (which adds
 * gUnknown_030058D0). */
extern u8 gUnknown_030059A0[];
/* A word counter. sub_08087298 returns gUnknown_030058F4 * 2 - K as a u16,
 * with K chosen by gPlaySt.unk01. */
extern u32 gUnknown_030058F4;
/* A word flag guarding an optional sub-proc: sub_08080F3C and sub_08080F90 set
 * and clear it; sub_08081290 and the code at 0x080814E0 test it. */
extern u32 gUnknown_030058FC;
/* Zeroed by sub_08085AF4's reset with gUnknown_0300596C/80/90 and
 * gUnknown_03005930; read by sub_08085F94 and sub_08085C10. It pairs with
 * gUnknown_03005930 as gUnknown_03005990[gUnknown_0300596C] pairs with
 * gUnknown_03005980: sub_08085F94 picks one pair or the other on
 * gUnknown_081D940C->unk01 == 2 and adds it into a proc field at +0x58. */
extern u8 gUnknown_03005900;
/* A word: sub_08080FB8 tests it against 0 and sub_08081E54 stores to it. */
extern u32 gUnknown_03005920;
/* 0x03002EE0 is gpKeySt (include/hardware.h, not pulled in by global.h), not
 * an unnamed global: aw2bhr.lds already names it, so a gUnknown_03002EE0
 * declaration links to nothing. The address appears only inside pool words
 * (0x0816E1B0, 0x0816E1B4, 0x0816E1C8, 0x081D93B8, 0x0808D7CC), which makes it
 * easy to miss. Before declaring a gUnknown_<addr> reached only through a pool
 * word, grep aw2bhr.lds for that address. */
/* Small state id: sub_08080F3C and sub_08080F90 both set it to 6, and
 * sub_0808135C copies it into a u16 proc field (+0x66). Must stay 32-bit: the
 * ROM reads it with a word load. */
extern u32 gUnknown_03005924;
/* Word cell: sub_0807BA90 stores sub_0807B7BC's result here, and sub_0807BA90
 * and sub_0807BE24 copy it into a u16 proc field (+0x4c). Must stay 32-bit:
 * the ROM reads it with a word load. */
extern u32 gUnknown_0300592C;
/* Halfword zeroed by sub_08085AF4; sub_08085F94 adds it to gUnknown_03005900.
 * Must stay unsigned: the ROM zero-extends it. */
extern u16 gUnknown_03005930;
/* The only output of sub_08080F54, which looks its parameter up in the
 * six-byte permutation gUnknown_0861696C and stores (i + 4) % 6 here, or 4
 * when it is not found. Holds 0..5; the signedness is unconfirmed. */
extern u32 gUnknown_03005934;
/* A saved (entry, group, offset) triple: each time sub_0807CE5C moves the edit
 * cursor it stores its proc's +0x52, +0x58 and +0x5c fields here as u16, and
 * sub_0807DA98 reads them back. The length past [2] is unknown. */
extern u16 gUnknown_03005938[];
/* Number of records in gUnknown_03005948 / gUnknown_03005958, and so the index
 * of the next one: sub_08078608 and its siblings write a record and then
 * increment it. Declared u32, but sub_0808844C and sub_08088ECC compare it as
 * signed and need an (int) cast to match; if a negative value is ever found
 * here, retype it to int and drop those casts. */
extern u32 gUnknown_03005944;
/* Two parallel byte arrays with one entry per record, indexed by
 * gUnknown_03005944. sub_08078608 and its siblings write them; sub_0807DA98
 * and others read them. */
extern u8 gUnknown_03005948[];
extern u8 gUnknown_03005958[];
/* Two five-byte arrays, each cleared by its own loop: gUnknown_03005978 by
 * sub_08088044, gUnknown_03005910 by sub_0807C588. */
extern u8 gUnknown_03005978[];
extern u8 gUnknown_03005910[];
/* A 0/1 flag held in a word: sub_08081D30 toggles it, sub_08080F90 clears it,
 * and sub_080846F4 returns it. */
extern u32 gUnknown_03005968;
/* The ROM words 0x081D9364, 0x081D93D0-0x081D944C and 0x081D9458-0x081D9474
 * are compiler-made pool words holding the addresses of globals (gSinLut,
 * gpKeySt, gUnknown_03005948, gUnknown_03005964 and others), one per function
 * that uses them. Do not declare them; C code names the globals. The exception
 * in that range is 0x081D9424, which is real data: the u16 table
 * gUnknown_081D9424, declared further down. There is no gUnknown_0808F100:
 * that address is gSinLut. */
/* A 0..7 layout selector: 0..3 and 4..7 (used as the value minus 4) choose
 * between two halves of a layout. Must stay non-const: sub_080895E4 re-reads
 * it after each of several calls, which the compiler skips for a const
 * global. */
extern u8 gUnknown_03005964;
/* Three word cells sub_0807C034 writes on its way to a sprite coordinate and
 * no promoted function reads back: gUnknown_030058F8 gets the proc's unk66,
 * gUnknown_0300590C gets Div(proc->unk66, 10), and gUnknown_03005960 gets a
 * difference of two bytes minus 8. Must stay word-sized (the ROM stores whole
 * words); the signedness is unconfirmed. */
extern int gUnknown_030058F8;
extern int gUnknown_0300590C;
extern int gUnknown_03005960;
/* Index into gUnknown_03005990[], set to 2 by sub_08085AF4. Must stay s16:
 * sub_08085F94 and sub_080860DC sign-extend it when they load it. */
extern s16 gUnknown_0300596C;
/* Halfword zeroed by sub_08085AF4; sub_08085F94 adds it to
 * gUnknown_03005990[gUnknown_0300596C] (gUnknown_03005930 plays the same part
 * on the other branch). Must stay unsigned: the ROM zero-extends it. */
extern u16 gUnknown_03005980;
/* Nine bytes, cleared together by sub_08085AF4 and indexed by
 * gUnknown_0300596C in sub_08085F94 and sub_080860DC. */
extern u8 gUnknown_03005990[];
/* Per-entry subscripts into gUnknown_03005958: sub_0807CAFC reads entry i and
 * uses the byte to index gUnknown_03005958. Length unknown. */
extern u8 gUnknown_0300599C[];
/* Five words, cleared/filled as a unit by sub_08078740 and sub_08078758. */
extern u32 gUnknown_030059C0[];
/* The sound driver's objects, set up by sub_080703F4 (m4aSoundInit):
 *   gUnknown_03000FB0  0x400-byte IWRAM buffer that receives a copy of
 *                      sub_0806F7C8's code (element type unknown).
 *   gUnknown_03004790  the SoundInfo, passed to sub_080707F4 (SoundInit).
 *   gUnknown_030057D0  the CGB channel records (four 0x40-byte records; the
 *                      count is unconfirmed).
 *   gUnknown_03005AE0  the music players; element 0, the BGM player, is the
 *                      first argument of every m4a MPlay* call.
 *   gUnknown_03005BE0  the players' shared memAccArea. Must stay a scalar
 *                      used as &gUnknown_03005BE0, not an array: with the
 *                      array spelling sub_080703F4 no longer matches. */
extern u32 gUnknown_03000FB0[];
extern struct SoundInfo gUnknown_03004790;
extern struct CgbChannel gUnknown_030057D0[];
extern struct MusicPlayerInfo gUnknown_03005AE0[];
extern u8 gUnknown_03005BE0;
/* SOUND_INFO_PTR: pointer to the SoundInfo, at the fixed address the GBA sound
 * driver convention uses. sub_0806F744 (SoundMain) checks the ident it points
 * at against 0x68736D53. */
extern struct SoundInfo *gUnknown_03007FF0;

/* ---------------------------------------------------------------- ROM -- */

/* A ROM word holding the address of a RAM array of 0x0c-byte records (struct
 * Unk0808E5C8). */
extern struct Unk0808E5C8 *gUnknown_0808E5C8;
/* The list's head sentinel, the record just before the gUnknown_0200C624
 * array. sub_0801A718 starts its sorted insert here and reads its link back
 * into gUnknown_030020A8.unk04. The ROM word 0x0808E5D0 is a compiler-made
 * pool word holding this address, and 0x0808E5CC one holding
 * &gUnknown_030020A8; neither needs a declaration. */
extern struct Unk0808E5C8 gUnknown_0200C618;
extern struct Unk0808E5C8 gUnknown_0200C624[];
/* 0x0808E580-0x0808E58C are compiler-made pool words, not objects: 0x0808E580
 * and 0x0808E588 hold &gUnknown_0200C528, 0x0808E584 and 0x0808E58C hold
 * &gUnknown_03002514 (one pair each for sub_080185D0 and sub_08018694). Do not
 * declare them; C code names the two globals. */
/* A variable-length byte-code script: sub_0801B8A8 runs sub_0801B7C0 on each
 * command and steps sub_0808B6B0(p) + 1 bytes until it reaches a byte 1.
 * sub_0801B750 starts it. */
extern const u8 gUnknown_0808EF64[];
/* Used by address only: sub_080051EC compares a byte against this symbol's
 * address (see that function). Nothing reads its contents. */
extern const u8 gUnknown_0808D7B4[];
/* 0x0808EF84, 0x0808EF88, 0x0808EF8C and 0x0808F048 are compiler-made pool
 * words holding 0x03000054, &gUnknown_03000058, &gUnknown_0300005C and
 * &gUnknown_0808EF90. Do not declare them, not even as pointers (that adds a
 * load); sub_0801B8D0 and sub_0801BA4C name the globals. */
/* 0x03000054 cannot be declared: aw2bhr.lds places it inside
 * gUnknown_03000050, so a gUnknown_03000054 symbol does not link. Write
 * (&gUnknown_03000050)[1], as sub_0801B8D0 does. It is one of the three words
 * at 0x03000054-0x0300005C that sub_0801B8D0 zeroes together; it then stores
 * sub_0801B738's result there once per character and never reads it. */
/* Sine table in degrees: one s16 per degree for 0..90, plus one entry of
 * padding (92 in all). sub_0801BA4C folds any angle into 0..90 and negates the
 * result for the lower half-turn; sub_0801BAA8 is the cosine built on it. */
extern const s16 gUnknown_0808EF90[];
/* 0x40 bytes of uncompressed 4bpp tile data (two tiles) that sub_080059E4
 * queues for VRAM 0x060158C0 with sub_08011E54. Non-const because sub_08011E54
 * takes a plain void *. */
extern u8 gUnknown_0808DF4C[];
/* Zero-terminated table of timeouts in timer cycles, walked by the
 * gUnknown_03000578 cursor. sub_0802F8FC passes the selected entry to
 * sub_0802ECEC, which arms timer 3 with it; an entry of 0 means no timeout,
 * and sub_0802EC64 wraps the cursor back to 0 when it reaches one. */
extern int gUnknown_08090C44[];
extern struct Unk08090CD8 *const gUnknown_08090CD8;
/* gUnknown_0812A10C and these two structs describe a misreading: 0x0812A10C is
 * not an object but a compiler-made pool word holding &gUnknown_084C1430, and
 * the apparent second pointer level is that word. No promoted file uses them;
 * sub_080466DC names gUnknown_084C1430 (gUnknown_084C1430->unk50) instead. */
struct Unk0812A10CInner
{
    /* 0x00 */ u8 filler_00[0x50];
    /* 0x50 */ u8 unk50;
};
struct Unk0812A10C
{
    /* 0x00 */ struct Unk0812A10CInner *unk00;
};
extern struct Unk0812A10C *gUnknown_0812A10C;
/* The ROM words 0x080909E8-0x08090A64 are compiler-made pool words, one per
 * function, not objects. They hold the addresses of gUnknown_030033E0,
 * gUnknown_030033E4, gUnknown_08499590, gUnknown_08499594, gUnknown_08499C7C,
 * gUnknown_08499B0C, gPlayers, gPlaySt and a few other globals. Do not
 * declare them, not even as pointers (that adds a load): name the global and
 * the build places the word. Open case: sub_08026290 (in work/) reaches
 * gPlaySt both through 0x08090A60 and directly, which naming the global does
 * not reproduce. */
extern const s16 gUnknown_08090EAC[];
/* Twelve m4a sound-mode words; sub_0803B3B0 passes the one it selects to
 * sub_08070990 (m4aSoundMode). */
extern const u32 gUnknown_080910FC[];
/* 0x280 bytes: 20 uncompressed 4bpp tiles. sub_08037258 returns its
 * address. */
extern u8 gUnknown_080913BC[];
/* 0x800 bytes of uncompressed data that sub_080116E8 copies to VRAM 0x06017800
 * with sub_08011C68. */
extern u8 gUnknown_080A1424[];
/* 0x100 bytes of tile data that sub_0807420C copies to VRAM 0x06001F00 with
 * sub_08011C68. */
extern u8 gUnknown_0812B29C[];
/* 1024 bytes, one per map tile id (a gMap->tile[] value); the community
 * Nightmare module '16TerrainEditor' calls the byte 'Type'. sub_080215D0
 * copies all of it into the buffer gUnknown_0849959C points at. */
extern u8 gUnknown_080C1BC4[];
/* A ROM word holding &gUnknown_0200D510, the sprite-list layer heads. Unlike
 * the pool words around it, this is real data: sub_0801BE78 loads it
 * directly. */
extern struct SpriteEntry *gUnknown_0808F090;
/* The ROM words 0x0808F08C, 0x0808F094 and 0x0808F09C-0x0808F0B4 are
 * compiler-made pool words, not objects: 0x0808F08C holds 0x03002620, the end
 * of the 32-entry OAM buffer gUnknown_03002520 (sub_0801BC3C); 0x0808F0B4
 * holds &gUnknown_0200F720 (sub_0801E2A4); the others hold 0x0200E390,
 * 0x0200E438 and 0x03003034. Do not declare them; name the global or the
 * address expression, e.g. `(void *)(gUnknown_03002520 + 128)`. */
/* 32 u16 entries, entry i = i << 9: the OAM attr1 affine-parameter number
 * (bits 9-13), already shifted into place. sub_0801E3E8 ORs one into attr1
 * after masking it with 0xC1FF. */
extern u16 gUnknown_0808F0B8[];
/* 0x1a4-byte compressed blob; sub_0802D5B8 gets it from sub_08037250 and
 * decompresses it. Non-const because Decompress takes a plain u8 *. */
extern u8 gUnknown_080D3FE4[];
/* sub_0803FD80's overlay data: gUnknown_080D22C4, gUnknown_080D24E0,
 * gUnknown_080D2AE8 and gUnknown_080D3268 are Decompress sources,
 * gUnknown_080D3FC4 a palette for ApplyPaletteExt. Non-const: both functions
 * take plain pointers. Sizes unknown. */
extern u8 gUnknown_080D22C4[];
extern u8 gUnknown_080D24E0[];
extern u8 gUnknown_080D2AE8[];
extern u8 gUnknown_080D3268[];
extern u16 gUnknown_080D3FC4[];
/* sub_0803F5E4's tile data: sub_08011E54 copies 0xb80 bytes from
 * gUnknown_080CFFC4 or gUnknown_080D0B44; gUnknown_080D16C4 and
 * gUnknown_080D1BC4 are tile sets (32-byte tiles, 10-bit tile index) passed to
 * CpuFastSet. Non-const because sub_08011E54 takes a plain pointer. */
extern u8 gUnknown_080CFFC4[];
extern u8 gUnknown_080D0B44[];
extern u8 gUnknown_080D16C4[];
extern u8 gUnknown_080D1BC4[];
extern const struct ProcCmd gUnknown_086140D4[];
/* Sprite/animation table that sub_08043418 passes to sub_0801C7DC, which reads
 * u16 offsets from it and passes the entry it reaches to PutSpriteExt. */
extern const u16 gUnknown_08101EC0[];
/* sub_0803F140's palette for ApplyPaletteExt. Non-const because that function
 * takes a plain u16 *. */
extern u16 gUnknown_08109564[];
/* sub_0803FFA0's four overlay groups, chosen by proc->unk54. Each group is a
 * Decompress source (u8), a palette for ApplyPaletteExt (u16) and a sprite
 * descriptor for sub_0801C210 (u8). Non-const: all three functions take plain
 * pointers. Sizes unknown. */
extern u8 gUnknown_0810EB00[];
extern u16 gUnknown_0810F364[];
extern u8 gUnknown_0810F384[];
extern u8 gUnknown_0810F410[];
extern u16 gUnknown_0810FAE8[];
extern u8 gUnknown_0810FB08[];
extern u8 gUnknown_0810FB94[];
extern u16 gUnknown_0810FF4C[];
extern u8 gUnknown_0810FF6C[];
extern u8 gUnknown_0810FFE0[];
extern u16 gUnknown_08110CBC[];
extern u8 gUnknown_08110CDC[];
/* gUnknown_0810A3E8 and gUnknown_0810AFC8 (declared after the next pair): the
 * two blobs sub_0803F128 returns, chosen by a mode id, for use with
 * sub_0801C70C. Only their addresses are used, so the u16 element type is
 * unconfirmed. */
/* The two Decompress sources sub_0803F110 returns, chosen by the same mode id
 * as sub_0803F128. */
extern u8 gUnknown_081095A4[];
extern u8 gUnknown_0810A6F0[];
extern const u16 gUnknown_0810A3E8[];
extern const u16 gUnknown_0810AFC8[];
extern u8 gUnknown_0810BE60[];
/* gUnknown_0810BB9C is a sprite descriptor sub_08039EA8 passes to
 * sub_0801C210. gUnknown_0810E6E0 and gUnknown_0810E720 are two 0x20-byte
 * palettes that sub_08024330 alternates between every 40 frames, copied to
 * palette RAM + 0x1c0 with sub_08011C68. */
extern u8 gUnknown_0810BB9C[];
extern u8 gUnknown_0810E6E0[];
extern u8 gUnknown_0810E720[];
extern u8 gUnknown_0810E820[];
/* Four banks of 32-byte tiles. sub_0802A880 and GetTerrainNamePalette
 * (sub_0802A8AC) read gUnknown_08104464 and gUnknown_08106864. Keep the [][32]
 * shape: indexing by tile number is what reproduces the ROM's address
 * arithmetic. */
extern const u8 gUnknown_08104464[][32];
extern const u8 gUnknown_08106864[][32];
extern const u8 gUnknown_08106A64[][32];
extern const u8 gUnknown_08108264[][32];
/* gUnknown_08112F00, gUnknown_0811315C and gUnknown_081133D0 (LZ77 blobs of
 * 0x25c, 0x274 and 0x7d0 bytes) and the 16-colour palettes gUnknown_08113BA0
 * and gUnknown_08113BC0, all declared below: the five sub_08044D70 wrappers at
 * 0x08044C44-0x08044D34 each pass one blob and one palette to the proc they
 * start, which keeps them at +0x4c/+0x50. Non-const: Decompress and
 * ApplyPaletteExt take plain pointers. */
/* Two u16, {0x0001, 0xffff}: sub_0804BA64 copies them to its stack and indexes
 * the copy by side. Real data, unlike the pool words 0x08136030-0x08136038
 * before it. */
extern const u16 gUnknown_0813603C[2];
extern u8 gUnknown_08112F00[];
extern u8 gUnknown_0811315C[];
/* Decompress source shared by sub_08045358 and sub_080453CC. */
extern u8 gUnknown_08112704[];
/* The 0x0803F overlay data, in groups: Decompress sources (u8), palettes for
 * ApplyPaletteExt (u16) and sprite descriptors for sub_0801C70C (const u8).
 * Sizes unknown. */
extern u8 gUnknown_08115A78[];
extern const u8 gUnknown_081161CC[];
extern u16 gUnknown_081169B0[];
extern u8 gUnknown_08117380[];
extern const u8 gUnknown_081183EC[];
extern u16 gUnknown_081190D8[];
/* Tile set that sub_0803FE50 copies from in 0x80-byte units with sub_08011E54:
 * 32-byte tiles addressed by a 10-bit tile index. */
extern u8 gUnknown_081245F8[];
extern u8 gUnknown_081133D0[];
extern u16 gUnknown_08113BA0[];
extern u16 gUnknown_08113BC0[];
/* Compressed blobs (0x2f8 and 0x37c bytes), used only as Decompress
 * sources. */
extern u8 gUnknown_08126244[];
extern u8 gUnknown_0812653C[];
extern u8 gUnknown_081268F8[]; /* handed out by sub_0801F49C */
/* gUnknown_084873BC-gUnknown_084879D4 (declared below gUnknown_0848591C) are
 * the entries of two tables, not separate blobs: five 0x48-byte entries from
 * 0x084873BC and five 0xA0-byte entries from 0x08487754. The wrappers at
 * 0x08004AA0-0x08004B6C pass them to sub_080152EC and sub_080193B0. They are
 * `const u8 []` because only their addresses are used; a struct type would
 * need those entry sizes. */
/* s16 table indexed by a 9-bit neighbour mask (bits 8..0 = NW, N, NE, W,
 * unused, E, SW, S, SE; at most 0x200 entries). sub_0800A95C returns the
 * entry, and its caller sub_0800A588 treats it as signed. */
extern const s16 gUnknown_08486BC4[];
/* The same kind of table, read by sub_0800A884. */
extern const s16 gUnknown_084867C4[];
/* Translation table between two map object-id spaces: 2 rows of 49 s16.
 * GetTileWithShadow (sub_08001704) and GetTileWithShadow2 (sub_08001A04) map
 * an id in row 0 to row 1 when the cell left of (x, y) has one of 27 terrain
 * codes, and row 1 to row 0 otherwise; -1 when the id is not found. The
 * symbol's size (0xC4 bytes) fixes the [2][49] shape. */
extern const s16 gUnknown_0848591C[2][49];
extern const u8 gUnknown_084873BC[];
extern const u8 gUnknown_08487404[];
extern const u8 gUnknown_0848744C[];
extern const u8 gUnknown_08487494[];
extern const u8 gUnknown_084874DC[];
extern const u8 gUnknown_08487754[];
extern const u8 gUnknown_084877F4[];
extern const u8 gUnknown_08487894[];
extern const u8 gUnknown_08487934[];
extern const u8 gUnknown_084879D4[];
/* More blobs from the same screen-setup tables, used by address only:
 * gUnknown_084872B4 (sub_080049E8, to sub_080152EC), gUnknown_084872FC
 * (sub_08004B7C, to sub_0801A148), gUnknown_08487AC4 / gUnknown_08487B64
 * (sub_08004BD8 picks one by gActiveMap->flags & 0x1000, to sub_080193B0),
 * gUnknown_08487C04 (sub_08004C34, to sub_0801A148) and gUnknown_08487D44
 * (declared below; sub_08004D10, to sub_080193B0). */
extern const u8 gUnknown_084872B4[];
extern const u8 gUnknown_084872FC[];
extern const u8 gUnknown_08487AC4[];
extern const u8 gUnknown_08487B64[];
extern const u8 gUnknown_08487C04[];

/* sub_08004CA0 passes this to sub_0801A104(blob, 2, 2, 0). */
extern const u8 gUnknown_08487C84[];

/* Eight bytes, 01 03 05 07 03 01 07 05: two rows of four frame counts.
 * sub_080030BC copies them to its stack and reads buf[lane] or buf[lane + 4],
 * choosing the row by gActiveMap->panelSide. */
extern const u8 gUnknown_0808D754[];
extern const u8 gUnknown_08487D44[];
/* A gUnknown_03001470 script blob, not a proc script: sub_08004EDC,
 * sub_08004F1C and sub_08004F5C start it with sub_080152EC(blob, 0). */
extern const u8 gUnknown_08487E14[];
/* A gUnknown_03001470 script blob that sub_08004958 starts with
 * sub_080152EC(blob, 0). It holds 8-byte records of {THUMB function pointer,
 * u32 flags}. */
extern const u8 gUnknown_0848721C[];
/* Two proc scripts, each started by two functions that store a u16 at +0x64
 * of the new proc: sub_08011550 and sub_080115E0 start gUnknown_0848923C,
 * sub_0801156C and sub_080115F8 start gUnknown_0848925C. sub_08011588 /
 * sub_080115B4 (on tree 3) and sub_08011610 / sub_0801163C (with
 * Proc_StartBlocking) do the same and also set
 * gUnknown_030030E0.bits.effect. */
extern const struct ProcCmd gUnknown_0848923C[];
extern const struct ProcCmd gUnknown_0848925C[];
/* A proc script: sub_080111C8 starts it and sub_08011218 ends it with
 * Proc_EndEach. Four ProcCmds. */
extern const struct ProcCmd gUnknown_0848927C[];
/* Two more proc scripts: gUnknown_0848929C is started by sub_08011668 (tree 3)
 * and sub_08013098 (Proc_StartBlocking), gUnknown_084892C4 by sub_08011684 and
 * sub_080130B0. Every starter stores a u16 at +0x64 of the new proc. */
extern const struct ProcCmd gUnknown_0848929C[];
extern const struct ProcCmd gUnknown_084892C4[];
/* The proc script sub_08013338 starts (blocking under its parent argument, or
 * on tree 3 when that is NULL) and sub_08013378 ends with Proc_EndEach. 44
 * ProcCmds. */
extern const struct ProcCmd gUnknown_084893AC[];
/* gUnknown_0848A140 (declared below; also named ProcScr_DialogueOnEnd): a proc
 * script that sub_08014BC0 and sub_08014C74 start with Proc_Start. */
/* Two gUnknown_03001470 script blobs, 0x20 and 0xbb8 bytes: 8-byte records of
 * {THUMB function pointer, u16, u16}, not ProcCmds. sub_08014668 starts the
 * first and its identical twin sub_080146D4 the second. */
extern const u8 gUnknown_08489548[];
extern const u8 gUnknown_08489568[];
/* Two more gUnknown_03001470 script blobs: sub_08014BE8 passes each address to
 * sub_08015BD0 to test whether a slot is running it. */
extern const u8 gUnknown_0848A120[];
extern const u8 gUnknown_0848A130[];
extern const struct ProcCmd gUnknown_0848A140[];
/* A proc script: sub_08045F80 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_0848A150[];
/* A gUnknown_0200C528 list script: sub_08019818 passes it to sub_080193B0,
 * which stores it in a slot, and sub_08019850 to sub_08019290, which returns
 * -1 when no slot holds it. */
extern const u8 gUnknown_0848A3EC[];
/* Table of handler pointers: sub_080184A4 indexes it by (s16)gUnknown_03002F90
 * and passes the entry to sub_08012A54. */
extern void *gUnknown_0848A370[];
/* Another gUnknown_0200C528 list script; sub_08018E7C passes its address to
 * sub_0801930C. */
extern const u8 gUnknown_0848A378[];
/* Opcode table of the gUnknown_03001470 scripts: sub_08015A30 calls entry [n],
 * n being the u16 at +6 of the slot's current command, with the slot index,
 * and repeats while it returns non-zero. The u8 return type is unconfirmed;
 * the length is unknown. */
extern u8 (*const gUnknown_0848A160[])(u8);
/* A gUnknown_03001470 script (0x20 bytes): sub_08019F90 starts it with
 * sub_080152EC(blob, 0) and sub_0801A168 stops it with sub_0801537C. Four
 * other functions pass its address to slot lookups (sub_08015BD0,
 * sub_080637AC). */
extern const u8 gUnknown_0848A42C[];
/* A proc script that sub_0801BFFC looks up with Proc_Find when it is given a
 * NULL proc; it then stores two words at +0x2c/+0x30 of the result. */
extern const struct ProcCmd gUnknown_0848B418[];
/* The OAM size tables, indexed by shape (attr0 >> 14) and size (attr1 >> 14).
 * gUnknown_0848B56C: {width, height} in pixels, u16 each, 4 shapes x 4 sizes
 * (the prohibited shape 3 is all 8x8); sub_0801C090 reads the width.
 * gUnknown_0848B5C4: {width, height} in tiles, u8 each, 3 shapes x 4 sizes;
 * sub_0801C53C reads both. Must stay flat arrays, not [][2]: the ROM
 * recomputes the index for the second value instead of using an offset. */
extern const u16 gUnknown_0848B56C[];
extern const u8 gUnknown_0848B5C4[];
/* A proc script; sub_0801C7B4 returns whether one is running. */
extern const struct ProcCmd gUnknown_0848B5AC[];
extern u16 *gUnknown_08499578;
/* AW2 stores pointers in ROM; these aliases name the four EWRAM tilemaps. */
extern u16 *gBG0TilemapBuffer;
extern u16 *gBG1TilemapBuffer;
extern u16 *gBG2TilemapBuffer;
extern u16 *gBG3TilemapBuffer;
/* gUnknown_084C3F50: 0x20 bytes that sub_08086BF8 and sub_08086CE0 pass to
 * sub_080149C0's u8 * fourth parameter. (0x081D943C, used by sub_08086BF8, is
 * a compiler-made pool word holding &gUnknown_08499578, not an object.) */
extern u8 gUnknown_084C3F50[];
/* gUnknown_084C3F40 and gUnknown_084C3F4C: more data for sub_080149C0's fourth
 * parameter, from sub_08046030
 * (0x0812A108 is a pool word holding &gUnknown_084C3F4C). gUnknown_08499CE4:
 * 12 u16 entries indexed by gUnknown_0300596C, passed to sub_08014A5C's fourth
 * parameter by sub_08086BF8 and sub_08086CE0. */
extern u8 gUnknown_084C3F40[];
extern u8 gUnknown_084C3F4C[];
extern u16 gUnknown_08499CE4[];
/* 0xFF-terminated list of ids; sub_0802D2EC calls sub_0801F234 on each. */
extern u8 gUnknown_0849AAA8[];
/* -1-terminated list of menu-entry ids that sub_0802D67C filters; each indexes
 * gUnknown_085D5ABC. Must stay s8: the terminator test sign-extends the byte.
 * (0x08090C08 is a pool word holding its address, not an object.) */
extern s8 gUnknown_081BA054[];

/* The option-list overlay: a view of a gUnknown_03001470[] slot, used by
 * sub_08019A60, sub_08019C40, sub_08019E68 and sub_08019F90. struct
 * Unk03001470 declares other types at the same offsets and is left as it is.
 * Unk8019A60Item is one 0x20-byte entry of the table unk20 points at; the
 * table ends with unk00 == 0xff. */
struct Unk8019A60Item /* 0x20 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x03];
    /* 0x04 */ int (*unk04)(void);
    /* 0x08 */ u8 filler_08[0x08];
    /* 0x10 */ void (*unk10)(u8, u8, u8);
    /* 0x14 */ void (*unk14)(u8, u8, u8);
    /* 0x18 */ void (*unk18)(u8, u8, u8);
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u8 filler_1e[0x02];
};

/* unk20 is the entry table. unk24 has a byte per entry and unk31 a byte per
 * selectable entry; unk40 and unk41 count them. unk0c is the proc callback
 * (sub_08019D78 or sub_08019DA8) and unk44 a second gUnknown_03001470 slot,
 * both set by sub_08019F90. unk48 and unk4a are signed (sub_08019E68 passes
 * them to s16 parameters). The array lengths are unconfirmed: sub_08019F90
 * writes 0 at +0x3e, inside unk31, and +0x3e/+0x3f may really be separate
 * fields. */
struct Unk8019A60
{
    /* 0x00 */ u8 filler_00[0x0c];
    /* 0x0c */ void (*unk0c)(ProcPtr);
    /* 0x10 */ u8 filler_10[0x10];
    /* 0x20 */ struct Unk8019A60Item *unk20;
    /* 0x24 */ u8 unk24[0x0d];
    /* 0x31 */ u8 unk31[0x0f];
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ struct Unk03001470 *unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4a */ s16 unk4a;
    /* 0x4c */ u8 unk4c;
};
/* 0x0812A178 is not an object: it is a compiler-made pool word holding
 * &gUnknown_02028E41, the signature bytes sub_0804A124 checks. Do not declare
 * gUnknown_0812A178; name gUnknown_02028E41. */
/* Table of 19-byte strings: sub_08032A00 passes entry gUnknown_0849B060->unk04
 * to sub_080149C0's text parameter. */
extern u8 gUnknown_02027C2C[];
/* Two LZ77 blobs sub_08075314 decompresses: the first to VRAM 0x06000800, the
 * second into the buffer gUnknown_0849957C points at. Non-const because
 * Decompress takes a plain u8 *. */
extern u8 gUnknown_081D1398[];
extern u8 gUnknown_081D13E0[];
/* Colour table: sub_08075340 sets gPal[0x59] =
 * gUnknown_081D1624[(a & 0x1F) >> 1], so it has at least 16 entries. */
extern const u16 gUnknown_081D1624[];
/* 0/1 mode flag for the gUnknown_08580DD8 slot script, the twin of
 * gUnknown_0202F2C8: sub_080670A0 sets 0 and sub_080670BC sets 1, each just
 * before sub_080152EC(gUnknown_08580DD8, 2). One byte, separate from
 * gUnknown_0202F204; signedness unknown. */
extern u8 gUnknown_0202F200;
/* Counter: sub_0806A054 increments it and passes the old value to
 * sub_080674F4. */
extern u8 gUnknown_0202F204;
/* sub_08068038's glyph-lookup cursor into gUnknown_08581160[]. It is a global,
 * not a local: the function resets it and re-reads it from memory on each pass
 * of its inner loop. */
extern s32 gUnknown_0202F208;
/* Blob that sub_0806A054 passes to sub_080670F8; only its address is used. */
extern const u8 gUnknown_085814A8[];
extern u16 *gUnknown_08499580;
/* Tilemap buffer pointer, like gUnknown_08499578 and gUnknown_08499580.
 * sub_08077B74 clears the buffer with CpuFastSet and then reads u16 entries at
 * proc->unk4a * 32 + t. */
extern u16 *gUnknown_0849957C;
/* Not real objects: compiler-made pool words holding &gUnknown_08499580 and
 * &gUnknown_085D5ABC. They are declared as pointers to their targets for
 * sub_08022618's draft, which reads through them (`pp = &gUnknown_080909A8`);
 * other code should name the targets. */
extern u16 **const gUnknown_080909A8;
extern const struct UnitType *const gUnknown_080909AC;
/* Two more such pool words, from the 0x08091350-0x0809138C run:
 * gUnknown_08091364 holds &gUnknown_030040D8 and gUnknown_08091368 holds
 * &gUnknown_085D5ABC. gUnknown_08091364 is typed struct Unit ** because
 * sub_08042998 reads the same fields through it as through
 * gUnknown_08499594[i]. */
extern struct Unit **const gUnknown_08091364;
extern const struct UnitType *const gUnknown_08091368;
/* Tilemap buffer pointer, the fourth with gUnknown_08499578/7C/80. Must stay
 * non-const: sub_080616F0 re-reads it on every pass of its clear loop. */
extern u16 *gUnknown_08499584;
/* Screen-setup data for sub_08068AC4 and sub_080688E4: gUnknown_0817DA18 is a
 * palette (loaded as BG palette 1 and OBJ palette 0), gUnknown_0817C408,
 * gUnknown_0817DA38 and gUnknown_0817E208 are Decompress sources,
 * gUnknown_0817D874 goes to sub_080718F8 and gUnknown_085813D4 to
 * sub_08012C58. Non-const because Decompress takes a plain u8 *. */
extern u16 gUnknown_0817DA18[];
extern u8 gUnknown_0817C408[];
extern u8 gUnknown_0817D874[];
extern u8 gUnknown_0817DA38[];
extern u8 gUnknown_0817E208[];
extern u8 gUnknown_085813D4[];
/* Palette that sub_08067D04 loads with ApplyPaletteExt(gUnknown_0817D9F8,
 * 0x220, 0x20). Non-const because ApplyPaletteExt takes a plain u16 *. */
extern u16 gUnknown_0817D9F8[];
/* Small tables that sub_080035C8, sub_08003640 and sub_08004724 copy to the
 * stack with sub_0808B6E8: gUnknown_0808D774 = u16 {7, 1, 3, 4},
 * gUnknown_0808D77C = u8 {7, 1, 3, 4}, gUnknown_0808D7A0 = u16
 * {0x2A, 1, 0x87, 0x20}. The ROM words between them (0x0808D780, 0x0808D7A8)
 * are pool words, not tables. */
extern const u16 gUnknown_0808D774[];
extern const u8 gUnknown_0808D77C[];
extern const u16 gUnknown_0808D7A0[];
extern u8 **const gUnknown_0808D7F8;
extern u8 **const gUnknown_0808D7FC;
/* gUnknown_0808D7F8 (sub_080081E0), gUnknown_0808D7FC (sub_080083E0) and the
 * five below are not real objects: they are compiler-made pool words holding
 * &gUnknown_08499590, one per function (the comments name it). All of
 * 0x0808D6DC-0x0808D8A8 is such words, for the functions sub_08000694 to
 * sub_08010DD4 in address order. Most code names gUnknown_08499590 and lets
 * the build place the word; sub_0800A3D4, sub_0800AA30, sub_0800ABD0 and
 * MakeReefSafe (sub_0800BF78) name the word itself. The const is on the outer
 * pointer only: the ROM keeps the word's value across calls but reloads
 * gUnknown_08499590. */
extern u8 **const gUnknown_0808D81C;   /* sub_0800977C */
extern u8 **const gUnknown_0808D83C;   /* sub_0800A3D4 */
extern u8 **const gUnknown_0808D854;   /* sub_0800AA30 */
extern u8 **const gUnknown_0808D858;   /* sub_0800ABD0 */
extern u8 **const gUnknown_0808D86C;   /* sub_0800BF78 */
/* Pointer to the map: width, height, scroll, camera and the per-cell planes
 * indexed by rowOffset[y] + x. Its layout is struct Map (include/map.h), and
 * gMap is a linker alias for this same word with that type; new code should
 * use gMap. This declaration must stay `u8 *`: over a hundred promoted files
 * do byte arithmetic on it, which a struct type would rescale. To reproduce
 * the ROM's address arithmetic, read a plane as a struct member
 * (gMap->tile[i]) rather than as gUnknown_08499590 + constant + i; the
 * compiler moves the constant in the second form. Many ROM words hold this
 * pointer's address (for example 0x0808D6DC-0x0808D8A8,
 * 0x08090978-0x08090A50, 0x0816D93C, 0x0816DB30): they are compiler-made pool
 * words, so do not declare them; name gUnknown_08499590 or gMap and the build
 * places the word. */
extern u8 *gUnknown_08499590;

/* The same map as struct Map in include/map.h, with only three members named.
 * No promoted file uses this struct any more; use struct Map through gMap. */
struct Unk08499590
{
    /* 0x0000 */ u8 filler_0000[0x0A22];
    /* 0x0a22 */ u16 unk0a22[0xC94];
    /* 0x234a */ u8 unk234a[0x1E30];
    /* 0x417a */ u16 unk417a[0x40];
};
/* 0xFFFF-terminated list of map tile ids, with a second table packed behind it
 * from index 0xa. sub_080240B4 and sub_0802419C scan it for the tile under the
 * cursor, count the entries passed, and read the entry at n + (a3 >> 5);
 * sub_0802419C adds 0xa to read the second table. The ROM words 0x08090A24 and
 * 0x08090A28 both hold its address: they are pool words, not two lists. */
extern u16 gUnknown_08499B0C[];
/* u16 data that sub_08024720 passes to sub_0801368C's u16 * first parameter,
 * whole or as &gUnknown_0809139C[idx]. */
extern u16 gUnknown_0809139C[];
/* sub_080240B4 stores here, as a full word, the map tile id it is about to
 * look up in gUnknown_08499B0C. No promoted function reads it; signedness
 * unknown. */
extern u32 gUnknown_030033F8;
/* Not a real object: a compiler-made pool word holding &gUnknown_08499590. C
 * code names gUnknown_08499590; no promoted file uses this declaration
 * (only old drafts in work/ do). */
extern u8 **gUnknown_080912FC;
/* Three pointers to Shift-JIS tile runs inside the same ROM blob (0x300 bytes
 * apart); sub_08013D7C picks one by the character's code range. Each run holds
 * u16 tile ids, which are ORed with BG attribute bits and written to the
 * tilemap. Length past [2] unknown. */
extern u16 *gUnknown_0808F380[];
/* Shift-JIS fallback table that sub_08013D7C scans, up to gUnknown_0809091C,
 * for characters above 0x8397: 6-byte records of
 * {u16 key, u16 top tile, u16 bottom tile}, the key being the two character
 * bytes read as a little-endian halfword. Must stay a flat u16 array: agbcc
 * pads a struct of three u16 to 8 bytes, so a struct pointer would step 8
 * bytes instead of 6. */
extern u16 gUnknown_0808FC8C[];
/* The end of gUnknown_0808FC8C, one past the last record: the scan's bound,
 * loaded as its own pool word rather than computed from the table's extent. */
extern u16 gUnknown_0809091C[];
/* Table of void (*)(void) functions. sub_0805B3F4 calls one entry, chosen by
 * gUnknown_08499594[..].unk0b when that is non-zero and otherwise by a byte
 * from its own stack table. Length unknown. */
extern void (*gUnknown_08576890[])(void);
/* Direction table of signed (dx, dy) pairs, indexed by the D-pad bits of the
 * key state: sub_0800105C adds [i][0] to the cursor x and [i][1] to the cursor
 * y. Must stay non-const: sub_08023518 only matches when the table is re-read
 * after each intervening store, which the compiler skips for a const table. */
extern s16 gUnknown_08499C7C[][2];
/* The map cursor (tile x, y) that the direction table above moves;
 * gUnknown_030032C4 is its pixel-space partner, moved by four times the same
 * step. */
extern struct Unk802C57C gUnknown_030033E4;
/* Two-state frame toggle: sub_08039ACC increments it, wraps it from 2 back to
 * 0, and uses it as the last index into gUnknown_084A0090's animation table;
 * sub_08039A5C zeroes it. int, because the wrap test is a signed compare. */
extern int gUnknown_030043F8;
/* sub_08039588's glyph table: a NUL-terminated string of character codes. The
 * index of a matching character is also its glyph's slot in the 0x3000-byte
 * decompression buffer. Signedness unknown. */
extern u8 gUnknown_08090F30[];
/* Unit sprite art installed by sub_0804103C and sub_08041128.
 * gUnknown_081213F4 is a run of 16-colour palettes, one per
 * gPlayers[army].unk1a. The other symbols are 0x400-byte tile frames that
 * sub_0804103C chooses between and copies with sub_08011E54; gUnknown_081218BC
 * is itself a run of such frames, indexed by sub_08024984(sel) - 1. Non-const:
 * sub_08011E54 and ApplyPaletteExt take plain pointers. */
extern u16 gUnknown_081213F4[];
extern u8 gUnknown_081218BC[];
extern u8 gUnknown_08122CBC[];
extern u8 gUnknown_081230BC[];
extern u8 gUnknown_081234BC[];
extern u8 gUnknown_081238BC[];
extern u8 gUnknown_08123CBC[];
/* Table of pointers to Decompress sources; sub_08041128 indexes it by
 * sub_08042DE0's result. */
extern u8 *gUnknown_0849FD6C[];
/* A byte per map cell, indexed like the map's unit plane (rowOffset[y] + x).
 * sub_0804189C subtracts *(s16 *)(gUnknown_030013D0 + 0x14) from a cell,
 * stopping at zero. The pool word at 0x08091338 holds its address; name this
 * symbol, not the word. */
extern u8 gUnknown_020288B4[];

/* Eight u16, {0, 3, 1, 1, 1, 1, 1, 0}, indexed by a unit's unk1b in
 * sub_08020984. Real data, unlike the pool words on either side
 * (0x08090940, 0x08090954-0x0809095C), which must not be declared. */
extern const u16 gUnknown_08090944[];
/* Box-corner sprite data that sub_08022AF8 passes to sub_0801BD00's void *
 * third parameter: top-left, top-right, bottom-left, bottom-right, 8 bytes
 * apart. */
extern u8 gUnknown_08499B6C[];
extern u8 gUnknown_08499B74[];
extern u8 gUnknown_08499B7C[];
extern u8 gUnknown_08499B84[];
/* Pointer to a 0x400-byte RAM buffer; sub_080215D0 copies gUnknown_080C1BC4
 * into it. */
extern u8 *gUnknown_0849959C;
extern struct PropertyListEntry *gUnknown_084995A0;
/* A byte per unit type, indexed by struct Unit's unk00: sub_0804209C rejects a
 * unit whose entry is 0 before any other test. */
extern const u8 gUnknown_084995A8[];
/* A byte per unit type: HasSupplyAbility (sub_08042084) indexes it with a
 * byte read through its pointer argument and returns whether the entry is
 * non-zero. */
extern const u8 gUnknown_084995C1[];
/* A u16 per army (1-based): sub_08028580 compares it with the owner field
 * (flags & 0xe0) of map entries. Must stay non-const: sub_08028580 re-reads it
 * after a store through the map pointer, which the compiler skips for a const
 * table. */
extern u16 gUnknown_084995F4[];
/* The first unit id of each army, indexed by the 1-based army number:
 * {0, 0, 0x40, 0x80, 0xC0}. gUnknown_08499594 gives each army a 64-entry
 * group, of which entries base + 1 to base + 0x32 hold units. The values are
 * unsigned but the declaration is s16, for sub_080257C0, which passes an entry
 * to an s16 parameter; readers that need the unsigned value cast to u16. Must
 * stay non-const: sub_080212AC re-reads it inside a loop that calls a
 * function, which the compiler skips for a const table. */
extern s16 gUnknown_084995FE[];
/* Two proc scripts started on tree 3 by sub_08027278 (gUnknown_08499CFC) and
 * sub_0802723C (gUnknown_08499D2C), identical apart from the script and the
 * value stored at +0x54 (0 and 2). sub_0802759C reports whether a
 * gUnknown_08499CFC proc is running. */
extern const struct ProcCmd gUnknown_08499CFC[];
extern const struct ProcCmd gUnknown_08499D2C[];
extern void *gUnknown_08499E38[];
/* gUnknown_08499FA0: a u16 per army, indexed by gPlayers[army].unk1a - 1;
 * sub_08028580 passes the value to sub_08019818. Non-const like
 * gUnknown_084995F4. gUnknown_08499FAC: a proc script sub_08028848 starts on
 * tree 3, storing two u16 arguments at +0x64 and +0x66. */
extern u16 gUnknown_08499FA0[];
extern const struct ProcCmd gUnknown_08499FAC[];
/* OAM sprite data that sub_08028E24 passes to PutSprite's u16 * parameter.
 * Non-const because that parameter is. */
extern u16 gUnknown_08499FE4[];
/* A proc script; sub_08028ED0 starts it with Proc_StartBlocking. */
extern const struct ProcCmd gUnknown_08499FEC[];
/* A gUnknown_03001470 script blob that sub_08042B70 and sub_08042B84 start
 * with sub_080152EC. Only its address is used, so it is `const u8 []`. */
extern const u8 gUnknown_0849A0F0[];
/* More gUnknown_03001470 script blobs. Each is started by one function (with
 * sub_080152C0 or sub_080152EC) and stopped by another (with sub_0801537C,
 * which finds the slot running it): 0849A108 sub_0802A514 / sub_0802A528,
 * 0849D41C sub_08037610 / sub_08037628, 0849D55C sub_08037F58 / sub_08037F70,
 * 0849E6D4 sub_0803B240 / sub_0803B254, 084C2198 sub_080470F8 / sub_080470E8,
 * 0849E670 sub_0803B16C / sub_0803B15C, 0849E700 sub_0803B2BC / sub_0803B33C.
 * Their sizes differ, so they are separate blobs, not one table. */
extern const u8 gUnknown_0849A108[];
extern const struct Unk0849A2C8 gUnknown_0849A2C8[];
extern const struct Unk0849A354 gUnknown_0849A354[];
/* A gUnknown_03001470 script blob that StartSupplyAnimation (sub_08029CB8)
 * starts with sub_080152EC(blob, 0). */
extern const u8 gUnknown_0849A0A8[];
/* u16 table indexed by sub_0802B6C8's 0..2 result, in sub_0802B768 and
 * sub_0802B7E8. */
extern const u16 gUnknown_0849A2A0[];
/* Three s16 per entry: sub_0802AA14 reads [i * 3 + 1]. A separate table that
 * starts right after gUnknown_0849A2A0's three entries. */
extern const s16 gUnknown_0849A2A6[];
/* Two rows of three u16, indexed [sel][n] by sub_0802AA78 with n the 0..2
 * result of sub_0802B6C8; the value is added to an x coordinate. A separate
 * symbol from gUnknown_0849A2A6: sub_0802AA78 reaches it through its own pool
 * word. */
extern const u16 gUnknown_0849A2B2[][3];
/* u16 values for sub_0802AA78: [0] is a threshold compared with
 * gUnknown_03003130.unk0c, and [1] and [3] are the two y origins it chooses
 * between. */
extern const u16 gUnknown_0849A2BE[];
/* Byte offsets: sub_0802AA14 adds bytes +2 and +3 to an x and a y. */
extern const u8 gUnknown_0849A284[];
/* Sprite data that sub_0802AA78 passes to sub_0801C7DC's const u16 * first
 * parameter. */
extern const u16 gUnknown_081243C4[];
/* Object template passed to sub_0801BD00's void * third parameter. */
extern u16 gUnknown_0849A248[];
/* A proc script; sub_0802BFD0 starts it on tree 3. */
extern const struct ProcCmd gUnknown_0849A480[];
/* Graphics data used by address only: gUnknown_0810E9E0 is copied with
 * sub_08011E54 (sub_0802B8C4). In sub_0802BFD0, gUnknown_081248F8 is a
 * Decompress source, gUnknown_08125190 a palette for ApplyPaletteExt and
 * gUnknown_08124FB8 a sprite blob for sub_0801C70C; the calls cast them to the
 * parameter types. */
extern const u8 gUnknown_0810E9E0[];
extern const u8 gUnknown_081248F8[];
extern const u8 gUnknown_08124FB8[];
extern const u8 gUnknown_08125190[];
/* 0x08090BB8 and 0x08090BBC are compiler-made pool words holding
 * &gUnknown_03003130, used by sub_0802B768 and sub_0802B7E8. Do not declare
 * them; name gUnknown_03003130. */
/* More gUnknown_03001470 script blobs, each started with sub_080152EC(blob, 0)
 * by a one-line wrapper: sub_0802BF80, sub_0802BF94, sub_0802BFA8,
 * sub_0802BFBC and sub_0802C130. Only their addresses are used. */
extern const u8 gUnknown_0849A3C0[];
extern const u8 gUnknown_0849A3F0[];
extern const u8 gUnknown_0849A428[];
extern const u8 gUnknown_0849A450[];
extern const u8 gUnknown_0849A4A0[];
/* Three gUnknown_0200C528 list scripts, each passed to sub_080193B0 by a
 * one-line wrapper: gUnknown_0849A520 by sub_0802C144, gUnknown_0849A5E0 by
 * sub_0802C1B0, gUnknown_0849A6B0 by sub_0802C1C0. */
extern const u8 gUnknown_0849A520[];
extern const u8 gUnknown_0849A5E0[];
extern const u8 gUnknown_0849A6B0[];
/* A gUnknown_0200C528 list script: sub_0802C280 passes it to sub_080193B0
 * (which stores it in a slot's unk00 and unk04), sub_0802C290 to sub_0801930C,
 * and sub_0802C2A0 tests whether its argument is this address. */
extern const u8 gUnknown_0849A8F0[];
/* A gUnknown_03001470 script blob; sub_0802C2B4 starts it with
 * sub_080152EC. */
extern const u8 gUnknown_0849A990[];
/* gUnknown_0849AA68 is a proc script sub_0802CD28 starts on tree 3. The blobs
 * after it are passed by address only: gUnknown_0849AAC0 and gUnknown_0849AE28
 * to sub_0801A104 (by sub_0802D458 and sub_0802D558), gUnknown_0849AC60 and
 * gUnknown_0849ABC0 to sub_08019F2C (by sub_0802D4B0 and sub_0802D504). Each
 * pair of callers is identical apart from the blob. */
extern const struct ProcCmd gUnknown_0849AA68[];
extern const u8 gUnknown_0849AAC0[];
extern const u8 gUnknown_0849ABC0[];
extern const u8 gUnknown_0849AC60[];
/* A gUnknown_03001470 script blob that sub_0802CD78 starts with
 * sub_080152EC(blob, 0). */
extern const u8 gUnknown_0849ADD0[];
extern const u8 gUnknown_0849AE28[];
/* Pointer to a RAM record (struct Unk0849B018). The ROM words
 * 0x08090CA8-0x08090CF0 are compiler-made pool words holding
 * &gUnknown_0849B018 or &gUnknown_0849B01C, one per function: do not declare
 * them, name these pointers. struct Unk08090CD8 and gUnknown_08090CD8
 * (declared earlier in this file) describe this same record through one of
 * those words; sub_080308B4 still uses them, but new code should not. */
extern struct Unk0849B018 *gUnknown_0849B018;
/* 13-byte packet template that sub_08030D84 copies into gUnknown_020256DA
 * before sending it with sub_0802F588. */
extern const u8 gUnknown_0849B038[];
/* Four u16 per entry, indexed [a1 * 4 + k] by sub_08030F60: [0] and [1] go to
 * gUnknown_03002B40 and gUnknown_03002EFC, [2] + [0] and [3] + [1] to
 * gUnknown_03002B4C and gUnknown_03002B44. Must stay non-const: sub_08030F60
 * re-reads entries after storing to gUnknown_03002B40, which the compiler
 * skips for a const table. */
extern u16 gUnknown_0849B188[];
extern struct Unk0849B01C *gUnknown_0849B01C;
extern struct Unk0849B060 *gUnknown_0849B060;
/* Proc scripts used only through Proc_Start, Proc_StartBlocking or Proc_Find,
 * with the function that starts each: 0849B3CC sub_080342DC, 0849B8B8
 * sub_08034308, 0849BB50 sub_080338C0 (Proc_Find), 0849BC98 sub_0803433C,
 * 0849D56C sub_08037F80, 0849D77C sub_08039634, 0849D7FC sub_08039930,
 * 0849D82C sub_08039BB4, 0849E728 sub_0803B55C, 0849E778 sub_0803B6E8,
 * 0849E7A0 sub_0803B774, 0849E7B8 sub_0803B788, 0849EAAC sub_0803B9EC,
 * 0849EBBC sub_0803BADC, 0849EC1C sub_0803BA00, 0849ECE0 sub_0803B8C4,
 * 0849F5D0 sub_0803D960, 0849F888 sub_0803F2B8, 0849F918 sub_0803F3E4. */
extern const struct ProcCmd gUnknown_0849B3CC[];
/* Started the same way: 0849B284 by sub_08031418, 0849B2A4 by sub_08031E44,
 * 0849B62C by sub_08032454 (and ended by sub_08032468), 0849B6B0 by
 * sub_08032D4C, and 0849BB28 by sub_080337D8, which creates an object that is
 * not a Proc (see that function). */
extern const struct ProcCmd gUnknown_0849B284[];
/* gUnknown_0849B294 and gUnknown_0849B304 are only ever ended, by Proc_EndEach
 * in sub_08031CE4 and sub_080320F4; nothing starts them. */
extern const struct ProcCmd gUnknown_0849B294[];
extern const struct ProcCmd gUnknown_0849B2A4[];
extern const struct ProcCmd gUnknown_0849B304[];
extern const struct ProcCmd gUnknown_0849B62C[];
extern const struct ProcCmd gUnknown_0849B6B0[];
/* Two proc scripts that sub_080321A8 waits on: it breaks its own proc only
 * when neither is running. */
extern const struct ProcCmd gUnknown_0849B7D8[];
extern const struct ProcCmd gUnknown_0849B868[];
extern const struct ProcCmd gUnknown_0849B8B8[];
extern const struct ProcCmd gUnknown_0849BB28[];
extern const struct ProcCmd gUnknown_0849BB50[];
/* A proc script reached only through Proc_GotoScript: sub_08033230 switches
 * its own proc to it when proc->unk64 is 0 and gpKeySt->held has bit 0x2
 * set. */
extern const struct ProcCmd gUnknown_0849BA68[];
/* sub_0803343C starts gUnknown_08614284 on tree 3; sub_08033404 breaks its own
 * proc only when neither of these two scripts is running. */
extern const struct ProcCmd gUnknown_08614284[];
extern const struct ProcCmd gUnknown_0861429C[];
extern const struct ProcCmd gUnknown_0849BC98[];
extern const struct Unk0849CD88 gUnknown_0849CD88[];
/* A proc script; sub_0803710C returns whether one is running. */
extern const struct ProcCmd gUnknown_0849D3BC[];
/* Two more gUnknown_03001470 script blobs; their start and stop functions are
 * listed at gUnknown_0849A108. */
extern const u8 gUnknown_0849D41C[];
extern const u8 gUnknown_0849D55C[];
extern const struct ProcCmd gUnknown_0849D56C[];
/* sub_08039188's marker-tile table, 5 u16 per row, indexed
 * [sub_080390CC(i)][sub_08039064(i)]. Each value is a tile number, ORed with
 * 0x3000 (palette and priority bits) into a PutSprite OAM word. Row count
 * unknown. */
extern u16 gUnknown_0849D5C4[][5];
extern struct Unk0849D5F8 *gUnknown_0849D5F8;
/* Six u16 ROM tables used to draw map tiles. gUnknown_0849D3DC has 32 entries,
 * one per terrain kind (5-bit terrain code), giving the first tile of that
 * terrain's run (sub_0801759C, sub_080378A8); the community Nightmare module
 * 'Advance Wars 2 44TerrainEditor' calls it 'Graphic'. In sub_080378A8,
 * gUnknown_0849D474 is both a tile table (indexed
 * [gUnknown_0849D3DC[cell & 0x1f] + (cell >> 5)]) and the x-offset table,
 * gUnknown_0849D4F4 is the y-offset table, and gUnknown_0849D434 is a blank
 * tile quad used outside the map. In sub_08037A78, gUnknown_0849D534 is a
 * four-halfword AND mask for a tile quad and gUnknown_08582E74 is indexed by
 * gUnknown_030040F8[(terrain >> 6) + 1] + 0x12. */
extern const u16 gUnknown_0849D3DC[];
extern const u16 gUnknown_0849D434[];
extern const u16 gUnknown_0849D474[];
extern const u16 gUnknown_0849D4F4[];
extern const u16 gUnknown_0849D534[];
extern const u16 gUnknown_08582E74[];
/* sub_08039850's scripted-line table: one pointer per map, indexed by
 * gPlaySt.unk02 - 0x8a; NULL means the map has none. Each points at 8-byte
 * records, ending with unk01 == -1. */
struct Unk0849D62C
{
    /* 0x00 */ u8 unk00; /* army slot, matched against proc unk54; 0 = any */
    /* 0x01 */ s8 unk01; /* terrain id to match; -1 ends the list */
    /* 0x02 */ u8 unk02; /* compared with sub_08044374's result */
    /* 0x03 */ u8 filler_03[0x01];
    /* 0x04 */ u16 unk04; /* passed to sub_080397F4 */
    /* 0x06 */ u8 filler_06[0x02];
};
extern const struct Unk0849D62C *const gUnknown_0849D62C[];
/* A proc script sub_08039674 starts; sub_080396F4 and sub_08039750 call
 * Proc_BreakEach on it rather than ending it. 11 ProcCmds. */
extern const struct ProcCmd gUnknown_0849D6D4[];
/* Sprite data that sub_0803941C passes as the first and third arguments of
 * sub_08015438, the slots gUnknown_0849E700 and gUnknown_0849E6F8 fill in
 * sub_0803B2BC. Whether the original was const is unknown. */
extern u8 gUnknown_0849D730[];
extern u8 gUnknown_0849D73C[];
/* gUnknown_0849D76C: sub_08039544 starts it with
 * sub_080152EC(gUnknown_0849D76C, 0) and copies a string into the new slot at
 * +0x26. That makes it a gUnknown_03001470 script blob, although it is
 * declared as ProcCmds (only its address is used, so the type does not change
 * the code). */
extern const struct ProcCmd gUnknown_0849D76C[];
extern const struct ProcCmd gUnknown_0849D77C[];
extern const struct ProcCmd gUnknown_0849D7FC[];
/* sub_08039DBC's sprite data, sub_0801BD00's third argument. */
extern u8 gUnknown_0849D81C[];
/* sub_08039F80's sprite data, sub_0801BD00's third argument. */
extern u8 gUnknown_0849D8A0[];
extern const struct ProcCmd gUnknown_0849D82C[];
/* The two proc scripts sub_08039B88 checks: it returns whether either is
 * running. */
extern const struct ProcCmd gUnknown_0849D84C[];
extern const struct ProcCmd gUnknown_0849D874[];
/* A gUnknown_03001470 script blob: sub_0803ACB8 starts it with
 * sub_080152EC(blob, 0), and sub_0803ACD0 returns whether a slot is running
 * it. */
extern const u8 gUnknown_0849E600[];
/* A real proc script (its first command starts a blocking child proc,
 * 0x08616DFC), unlike the gUnknown_03001470 blobs next to it. sub_0803B9D4
 * starts it on tree 3 after sub_08044BB0. */
extern const struct ProcCmd gUnknown_0849EA94[];
/* Three more gUnknown_03001470 script blobs (start and stop functions listed
 * at gUnknown_0849A108). They sit between real proc scripts, so address order
 * does not tell which kind a blob is. */
extern const u8 gUnknown_0849E670[];
extern const u8 gUnknown_0849E6D4[];
extern const u8 gUnknown_0849E700[];
/* ---- the 0x0803B block ---- */
/* u16 bob offsets for a ten-frame cycle, read at (gGameClock / 3) % 10 by
 * sub_0803B1F0; the ROM starts 0, 1, 2, 3, 3, 3, 2, 1. */
extern const u16 gUnknown_080910E8[];
/* gUnknown_0849E6A4 and gUnknown_0849E6B8 are sprite data that sub_0803B1F0
 * draws with sub_0801BD00; gUnknown_0849E6F8 is passed to sub_08015438 with
 * gUnknown_0849E700. Non-const: both functions take plain void *. */
extern u8 gUnknown_0849E6A4[];
extern u8 gUnknown_0849E6B8[];
extern u8 gUnknown_0849E6F8[];
/* A gUnknown_03001470 script blob: PlayMusicOrSfx (sub_0803B48C) starts it
 * with sub_080152EC once sub_08015BD0 reports that no slot is running it. */
extern const u8 gUnknown_0849E710[];
/* A proc script: sub_0803BA88 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_0849EB7C[];

/* ---- the 0x08067 block ---- */
/* sub_08067638's four screen blobs: three Decompress sources (to VRAM
 * 0x06000000, 0x0600D800 and 0x0600E000) and one palette for ApplyPaletteExt.
 * Non-const: both functions take plain pointers. */
extern u8 gUnknown_0817B970[];
extern u8 gUnknown_0817BE90[];
extern u8 gUnknown_0817C138[];
extern u16 gUnknown_0817C3E8[];
/* sub_0806A6F0's two sources: sub_08011E54 copies 0x800 bytes of
 * gUnknown_0817B150 to VRAM 0x06013940, and gUnknown_0817B950 is a 0x20-byte
 * palette for ApplyPaletteExt (it ends where gUnknown_0817B970 starts).
 * Non-const: both functions take plain pointers. */
extern u8 gUnknown_0817B150[];
extern u16 gUnknown_0817B950[];
/* sub_0806A218's two Decompress sources: one into VRAM 0x06000000, the other
 * into the buffer gUnknown_08499578 points at. */
extern u8 gUnknown_081866F8[];
extern u8 gUnknown_08186D4C[];
/* A proc script; sub_0806A218 starts it with its own proc as the parent. */
extern const struct ProcCmd gUnknown_085814E8[];
/* sub_08067C94's OAM blob, PutSprite's `u16 *` fourth argument. */
extern u16 gUnknown_085810A8[];

/* ---- the 0x08044 block ---- */
/* sub_0804402C's sprite object list (PutSpriteExt's `u16 *`) and
 * sub_08044D70's blocking proc script. The word at 0x08091388 beside them is
 * not a global: it is a compiler-made pool word holding &gPlayers, so C names
 * gPlayers and never that address. */
extern u16 gUnknown_084A07DA[];
extern const struct ProcCmd gUnknown_084A08EC[];
/* A script blob for gUnknown_03001470, not a proc script: sub_0803B83C hands
 * it to sub_0801537C, which takes `const void *`. 0x18 bytes. Must stay
 * `const`: a plain `void *` would discard the const, which the build treats as
 * an error. */
extern const u8 gUnknown_0849B048[];
/* Two proc scripts, both handed to Proc_Start on tree 3: gUnknown_0849E7F8 by
 * sub_0803B83C and gUnknown_0849EC8C by sub_0803B858. 0x20 and 0x50 bytes,
 * i.e. four and ten ProcCmd entries. */
extern const struct ProcCmd gUnknown_0849E7F8[];
extern const struct ProcCmd gUnknown_0849EC8C[];
extern const struct ProcCmd gUnknown_0849E728[];
/* A proc script, only ever handed to Proc_Find: sub_0803B628 is the whole of
 * `return Proc_Find(gUnknown_0849E750) != 0;`. */
extern const struct ProcCmd gUnknown_0849E750[];
extern const struct ProcCmd gUnknown_0849E778[];
extern const struct ProcCmd gUnknown_0849E7A0[];
extern const struct ProcCmd gUnknown_0849E7B8[];
/* A proc script: sub_0803B7B4 hands it to Proc_Start on tree 3, but only when
 * gUnknown_030005CA still holds its 0xFFFF "unset" value. */
extern const struct ProcCmd gUnknown_0849E7D8[];
/* The table at 0x084A0090 is struct Unk084A0090 below, one 0x44-byte row per
 * CO. Callers that pack a variant and a CO into one number split it with
 * `/ 24` and `% 24`; the row index is the remainder. */
/* One CO power level's entry, 0x14 bytes. struct Unk084A0090 below holds two
 * of them, subscripted `gPlayers[].unk1f - 1`, so that field counts from 1.
 * Two is a ceiling: 0x28 bytes are set aside there. */
struct Unk084A0090Entry /* 0x14 */
{
    /* 0x00 */ u8 unitAnimation;
    /* 0x01 */ u8 unitAnimationPalette;
    /* 0x02 */ u8 filler_02[0x02];
    /* 0x04 */ bool8 (*animationCondition)(void *); /* called with a unit record;
                              * sub_080445A8 checks it against NULL */
    /* 0x08 */ void (*onEachUnit)(void *); /* sub_08044610 calls it with the
                              * same unit record and discards the result */
    /* 0x0c */ void (*onActivate)(void *); /* called with a literal 0 */
    /* 0x10 */ s16 sound[2]; /* The two sound ids for this power level,
                              * chosen by the gUnknown_030043F8 toggle, which
                              * only ever holds 0 or 1. */
};

/* The per-CO presentation table -- portraits, palettes and the two CO-power
 * hooks -- indexed by gPlayers[].co, beside the stats in gUnknown_085D3DD0.
 * Field names come from the community 'CO Other Editor' Nightmare module, a
 * ROM-editor definition neither this tree nor SRR_AW2 authored: its CO Face /
 * Face Happy / Face Sad pointers are face[3], and its COP and SCOP blocks are
 * power[2], one entry per power level. */
struct Unk084A0090 /* 0x44 */
{
    /* 0x00 */ u8 **fullBody;
    /* 0x04 */ u8 *nameGraphic;
    /* 0x08 */ u16 *palette;
    /* 0x0c */ u8 *face[3]; /* Decompress sources, one per portrait
                              * variant; sub_08043E3C picks with `a / 24`
                              * while `a % 24` picks the CO. */
    /* 0x18 */ void *miniPortrait;
    /* 0x1c */ struct Unk084A0090Entry power[2];
};
extern const struct Unk084A0090 gUnknown_084A0090[];
/* A 16-colour palette in ROM, handed to ApplyPaletteExt's `u16 *` first
 * parameter by sub_08043B44 with no arithmetic. Left unqualified, like every
 * other palette that reaches that function. */
extern u16 gUnknown_080F6164[];
/* A sprite object list in ROM, handed to PutSpriteExt's `u16 *` fourth
 * parameter by sub_08043B60 with no arithmetic. gUnknown_084A0790 and
 * gUnknown_084A07DA next to it are the rest of the same run. */
extern u16 gUnknown_084A0730[];
extern const struct ProcCmd gUnknown_0849EAAC[];
extern const struct ProcCmd gUnknown_0849EBBC[];
extern const struct ProcCmd gUnknown_0849EC1C[];
/* A ROM byte table sub_0803B930 reads as `gUnknown_0849EA78[i - 1]` for
 * i = 1..gUnknown_0849ECDC->unk00; the values land in gPlaySt.unk3d[1..4].
 * At least four elements are live; the real extent is unknown. */
extern u8 gUnknown_0849EA78[];
extern struct Unk0849ECDC *gUnknown_0849ECDC;
extern const struct ProcCmd gUnknown_0849ECE0[];
/* The fallback name string sub_0803CCEC hands out in place of
 * &gUnknown_020280C0[i].unk02 when that slot's unk13 is 0xff (empty). Same
 * type as the member it stands in for, so one return type covers both. */
extern u8 gUnknown_0849F320[];
/* A proc script: sub_0803D73C hands it to Proc_Start on tree 3, writing a u16
 * at +0x64 and a word at +0x4c of the new proc, and sub_0803D770 is the
 * matching `return Proc_Find(script) != 0;` existence test. */
extern const struct ProcCmd gUnknown_0849F330[];
/* The script sub_0803D88C sends its own proc to with Proc_GotoScript when
 * gPlaySt.unk01 is 1 and sub_0803861C returns 0. */
extern const struct ProcCmd gUnknown_0849F388[];
/* Not a proc script: sub_0803D8C0 and sub_0803D92C hand it to sub_080193B0,
 * whose parameter is `const u8 *`, then poke +0x10 of the struct Unk0200C528
 * it returns. */
extern const u8 gUnknown_0849F3A8[];
extern const struct ProcCmd gUnknown_0849F5D0[];
extern const struct ProcCmd gUnknown_0849F888[];
extern const struct ProcCmd gUnknown_0849F918[];
extern const struct ProcCmd gUnknown_0849FB44[];
/* Three proc scripts, each only handed to Proc_StartBlocking with the
 * starter's own last argument as parent: gUnknown_0849FB8C by sub_0804046C,
 * gUnknown_0849FBBC by sub_08040554 and gUnknown_0849FBEC by sub_0804074C.
 * None of the three starters reads the script itself. */
extern const struct ProcCmd gUnknown_0849FB8C[];
extern const struct ProcCmd gUnknown_0849FBBC[];
extern const struct ProcCmd gUnknown_0849FBEC[];
/* Three more proc scripts from the same 0x0849Fxxx run. sub_080411FC starts
 * gUnknown_0849FD14 and copies its own +0x3c/+0x3e halfword pair into the new
 * proc; that script's PROC_ONEND callback is sub_08040AFC. gUnknown_0849FE54
 * is started blocking beside it, and gUnknown_0849FE78 is the unit-move proc
 * both sub_08041820 and sub_0804189C start on tree 3. */
extern const struct ProcCmd gUnknown_0849FD14[];
extern const struct ProcCmd gUnknown_0849FE54[];
extern const struct ProcCmd gUnknown_0849FE78[];
/* A proc script: sub_08042C10 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_0849FC0C[];
/* Not a proc script: sub_0804087C hands it to sub_080193B0, whose parameter is
 * `const u8 *`, and discards the struct Unk0200C528 that comes back. */
extern const u8 gUnknown_0849FC64[];
/* sub_08042C24 hands it to Proc_Start or Proc_StartBlocking depending on
 * whether its parent argument is a real ProcPtr or a small tree number. */
extern const struct ProcCmd gUnknown_0849FCA4[];
/* A proc script: sub_08041180 hands it to Proc_Find and writes +0x40 (u16) of
 * the proc it returns. */
extern const struct ProcCmd gUnknown_0849FD44[];
/* A proc script: sub_080411A0 hands it to Proc_StartBlocking with its own
 * parameter as parent and ignores the proc that comes back. */
extern const struct ProcCmd gUnknown_0849FE0C[];
/* A proc script: sub_080411D0 hands it to Proc_StartBlocking under its own
 * parameter as parent, then widens two of that parameter's bytes (+0x48/+0x49)
 * into halfwords at +0x4c/+0x4e of the new proc. */
extern const struct ProcCmd gUnknown_0849FE34[];
/* Two proc scripts, each only ever handed to Proc_Find: sub_0802C550 is
 * `Proc_Find(gUnknown_0849FEF8) || Proc_Find(gUnknown_0849FFB0)`. */
extern const struct ProcCmd gUnknown_0849FEF8[];
extern const struct ProcCmd gUnknown_0849FFB0[];
/* Named proc scripts reached from this file. Declared non-const to agree with
 * the existing declaration in src/title-screen.c. */
extern struct ProcCmd ProcScr_MainMenu[];
extern const struct Unk084A06F0 gUnknown_084A06F0[];
extern const struct ProcCmd gUnknown_084A07E8[];
/* A proc script: sub_080443D8 hands it to Proc_StartBlocking. */
extern const struct ProcCmd gUnknown_084A0818[];
/* A proc script started blocking by sub_08044A88 and sub_08044AA0, which
 * then write 0x50 and 0x28 to a halfword at +0x64 of the new proc. */
extern const struct ProcCmd gUnknown_084A0858[];
/* A 24-entry menu layout table, 0x1c bytes, terminated by 0xff. sub_08044BB0
 * is the only reader: an entry with the high bit set is a group header whose
 * low 7 bits are a group index 0..4, matching the five bytes of
 * gUnknown_03005948 the same function clears on entry; the rest are item ids
 * for sub_0803CAD4. Must stay non-const, or the compiler keeps an entry in a
 * register across the sub_0803CAD4 call instead of reloading it. */
extern u8 gUnknown_084A08D0[];
/* A proc script: sub_08045F40 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_084B7628[];
/* A proc script started blocking by sub_08045448 and sub_08045460, which then
 * write 1 and 2 respectively to a word at +0x54 of the new proc. */
extern const struct ProcCmd gUnknown_084A09CC[];
/* A proc script started on tree 3 by sub_080452C0, which then stashes two
 * words at +0x3c/+0x40 and a byte at +0x2c of the new proc. */
extern const struct ProcCmd gUnknown_084A096C[];
/* The script sub_08044D70 forwards to Proc_Start. All five of its wrappers at
 * 0x08044C44-0x08044D34 pass this one script and differ only in the sprite
 * blob, the palette and four small integers. */
extern const struct ProcCmd gUnknown_084A0994[];
/* A proc script: sub_08045790 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_084A0A3C[];
extern struct Unk084C1430 *gUnknown_084C1430;
/* ROM blobs handed to sub_080152EC, the same slot as gUnknown_0849A0F0.
 * 084C3D6C is the only one started on tree 1 rather than 0. */
extern const u8 gUnknown_084C1824[];
/* Another gUnknown_03001470 script blob: sub_080470F8 hands it to
 * sub_080152EC and sub_080470E8 hands the same address to sub_0801537C. */
extern const u8 gUnknown_084C2198[];
/* A script blob sub_080470F8 starts with `sub_080152EC(g, 0)`, typed like
 * gUnknown_084C2198 above because sub_080152EC takes `const void *` and
 * nothing dereferences it. Its first ROM word is 0x08034F7D, the address of
 * sub_08034F7C with the THUMB bit set. */
extern const u8 gUnknown_084C2140[];
extern const u8 gUnknown_084C21C8[];
/* A proc script: sub_08049BD8 hands it to Proc_Start on tree 3. */
extern const struct ProcCmd gUnknown_084C3138[];
extern struct Unk084C3240 *gUnknown_084C3240;
extern const u8 gUnknown_084C325C[];
/* A proc script started blocking by sub_08049F08, which forwards its own
 * second argument as the parent. */
extern const struct ProcCmd gUnknown_084C327C[];
extern const u8 gUnknown_084C3824[];
/* Two OBJ sprite blobs sub_08049F24 hands to sub_0801BD00 as its `void *`
 * third parameter: gUnknown_084C37E4 once per pass of a four-pass loop and
 * gUnknown_084C3800 once after it. Non-const `u8 []` because the callee's
 * parameter is a plain `void *`; a `const` spelling would need a cast at every
 * call, and nothing here constrains the element type further. */
extern u8 gUnknown_084C37E4[];
extern u8 gUnknown_084C3800[];
/* The text renderer's per-character pixel-width table. sub_08014CEC and
 * sub_08014D38 index it by a raw character byte and add the result to a running
 * width; sub_08014D20 divides that width by 8 to get tiles. Unsized: neither
 * caller bounds the character. */
extern const u8 gUnknown_084C36E4[];
extern const u8 gUnknown_084C3D6C[];
/* The second blob sub_0804B088 hands to sub_0801537C, right after
 * gUnknown_084C3D6C above and typed the same way -- sub_0801537C takes
 * `const void *`. */
/* A proc script reached only as an address: sub_0804AE50 hands it to
 * sub_080152C0. The declared `void sub_080152C0(s32, u8)` is why the call site
 * needs the `(s32)` cast. */
extern const u8 gUnknown_084C3D7C[];
extern const u8 gUnknown_084C3D8C[];
/* Another gUnknown_03001470 script blob: sub_0804B0CC/sub_0804B10C hand it to
 * sub_080152EC(script, 0) and sub_0804B160 is the matching
 * `sub_08015BD0(script) != -1` liveness predicate. */
extern const u8 gUnknown_084C3D9C[];
/* A gUnknown_0200C528 list script. Its only reference in the ROM is
 * sub_0804AE10 handing its address to sub_080193B0(const u8 *) and discarding
 * the result. */
extern const u8 gUnknown_084C38BC[];
/* Two ROM pointer pairs indexed by gUnknown_0300453C (0 or 1). Each element
 * points at a u16 that sub_0804D290 / sub_0804DCA8 subtract from an entry's x
 * and y before calling sub_080155C0, i.e. the scroll origin for that side.
 * Two entries each; the word after gUnknown_084C3F78's pair is 0x00400010,
 * which is not an address. Callers must read both pointers into `u16 *` locals
 * before the call, or the two loads come out in the wrong order. */
extern u16 *gUnknown_084C3F70[];
extern u16 *gUnknown_084C3F78[];
/* Two more 0x400-entry u16 tilemap buffers. sub_08054C04 CpuFastSets 0x200
 * words out of gUnknown_08551A04 to 0x06002800, which fixes the extent; the
 * writers sub_0805701C/sub_08057110 store TILEREF-shaped halfwords. */
extern u16 *gUnknown_08551A00;
extern u16 *gUnknown_08551A04;
/* A ROM table of three-halfword rows indexed by gUnknown_0300453C; the stride
 * is exactly 6 bytes. Column 0 is read as a halfword into OamData.paletteNum
 * and is the only column with a reader so far. Must not become a struct: agbcc
 * rounds a three-u16 struct up to 8 bytes, which changes the index
 * arithmetic. */
extern u16 gUnknown_08551D0C[][3];
/* Maps a unit's state byte to a tile source: indexed by
 * gUnknown_02029A10[side].entries[slot].unk00, and the u16 it yields then
 * selects one of the two words in gUnknown_02029BA8[side].unk18[]. Read by
 * sub_0804EA54 and sub_0804EAEC. Extent unknown. */
extern u16 gUnknown_08551D1C[];
/* Two ROM u16 tables indexed by the same 10-bit tile delta
 * `(dst->unk04 - oam.tileNum) & 0x3FF` that sub_0804E8F0 / sub_0804FE10
 * compute. gUnknown_08552A40 is the selector -- 0xFFFF means no entry, and the
 * two live values seen so far, 0 and 0x28, each pick one of two branches -- and
 * gUnknown_08552700 is the payload passed to sub_0804EA54 / sub_0804EAEC.
 * Extents unknown. */
extern u16 gUnknown_08552700[];
extern u16 gUnknown_08552A40[];
/* Six-halfword ROM rows: the row is gUnknown_03001470[i].unk30 (the side) and
 * the column is sub_0804BDD8's u16 result. sub_0804E7A8 and sub_0804FCA4 ADD
 * the value to gUnknown_02029A10[side].entries[slot].x, so it is a pixel step.
 * Whether it is meant to be signed is unknown: the sum truncates back to 16
 * bits on the store either way. */
extern u16 gUnknown_08553B28[][6];
/* Six-halfword ROM rows indexed [side][t]: a palette index table.
 * sub_0804C828 and sub_0804CD84 read one element straight into struct OamData's
 * 4-bit paletteNum. The table itself is unsigned; only the column index is
 * signed. */
extern u16 gUnknown_08553B40[][6];
/* A ROM table of 28-byte rows, indexed by gUnknown_02029808[i].unk30[j] and
 * read by sub_08051DE0, sub_080524C0, sub_0805297C and sub_08055058.
 *   unk00  a byte length: sub_08055058 gates the row on it and passes
 *          `unk00 >> 2` to CpuFastSet as the word count.
 *   unk02  an x offset per gUnknown_0300453C side, and unk06 the matching y --
 *          both are added to gUnknown_02029A10[..].entries[..].x / .y.
 *   unk08  the CpuFastSet source in sub_08055058, so a pointer.
 * Must stay a struct rather than `u16 [][14]`: the two-dimensional spelling
 * folds the member offset into the symbol's own address instead of into the
 * load's displacement. Everything past unk08 is unread. */
struct Unk08552D80 /* 0x1c */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02[2];
    /* 0x06 */ u16 unk06;
    /* 0x08 */ const void *unk08;
    /* 0x0c */ void *unk0c[2]; /* Two per-side word columns, unk0c and
                                * unk14, both indexed by gUnknown_0300453C.
                                * sub_08055D4C copies them into
                                * gUnknown_02029808[i].unk44[] and .unk58[],
                                * which are already `void *[5]`. The [2] is
                                * the side, which is all the 8 bytes allow. */
    /* 0x14 */ void *unk14[2];
};
extern struct Unk08552D80 gUnknown_08552D80[];
/* NOT globals in the source: these are two slots of agbcc's own `-fforce-addr`
 * pool of address constants in `.rodata`, each holding &gUnknown_03001470 for
 * one function. C code names gUnknown_03001470; these symbols exist only
 * because declaring them is the one spelling that reproduces the ROM's extra
 * `ldr` with a resolvable relocation. The two functions that use them have
 * identical source; what tells them apart is a compiler artefact, not the C.
 *
 * The wrapper struct is load-bearing: the ROM adds the member offset to the
 * base and only then the index, which needs `g->unk00[i].m` on a pointer to a
 * struct whose member is the array. A plain `struct Unk03001470 *const` with
 * `g[i].m` folds the offset into the load instead and cannot reach the ROM. */
struct Unk08136090 /* not a real object: see above */
{
    /* 0x00 */ struct Unk03001470 unk00[30];
};
extern struct Unk08136090 *const gUnknown_08136090;
extern struct Unk08136090 *const gUnknown_081360C8;
/* A ROM u16 table of small values (0..3), indexed by
 * `gUnknown_0300453C ^ gUnknown_0300450C` and masked with 3 into
 * OamData.priority, so an OBJ-priority lookup. The first eight entries are
 * 3,2,2,3,3,2,0,0; what follows looks like signed pixel offsets, so only those
 * eight belong to the priority use. */
extern u16 gUnknown_085523A4[];
/* The same priority table two halfwords earlier, reached through its own linker
 * symbol: sub_0804DCA8 indexes 0x0855239C as
 * `[gUnknown_0300453C * 2 + gUnknown_0300450C]`, where its twin sub_0804D290
 * uses gUnknown_085523A4 (== 0x0855239C + 8) with an XOR index. The same eight
 * halfwords read two different ways. Must stay flat, not `[][2]`: the ROM
 * computes `(a * 2 + b) * 2` as one chain, and the two-dimensional spelling
 * scales each index separately. */
extern u16 gUnknown_0855239C[];
/* A ROM dispatch table of THUMB function pointers, indexed by
 * gUnknown_085D6A48[gUnknown_03004580[i][1]][2]. sub_08050B70 is its only
 * reader and calls the entry with two u16 arguments: every target it reaches
 * (sub_08050D44, sub_08050E08, sub_08051BEC, sub_08052718, sub_08051F4C,
 * sub_08052E04, sub_08052BBC, sub_080523E8) narrows r0 to 16 bits on entry and
 * most narrow r1 too. Left unsized on purpose: slots 2 and 3 hold 0x08553648
 * and 0x08553674, which are even and point back into this same data region, so
 * they are not code. Either those slots are never reached or the array is
 * heterogeneous; sub_08050B70 cannot tell. */
extern void (*const gUnknown_085535B0[])(u16, u16);
/* A ROM table of three-word rows, three rows per outer index; sub_0804D290 and
 * sub_0804DCA8 read it as `[w][q][2]`. Column 2 holds an odd THUMB function
 * pointer (0x08050365 -> sub_08050364, 0x0804E8F1 -> sub_0804E8F0) handed to
 * sub_08015928 as a continuation; column 1 is another one and column 0 is a
 * data address, so the row is not uniformly typed and u32 is the honest element
 * type. */
extern u32 gUnknown_08552FB8[][3][3];
/* A ROM u16 pair indexed by a 0/1 flag and its complement: sub_0805741C uses
 * gUnknown_085538AE[a ^ 1] for the BG3 control shadow and [a] for
 * gUnknown_0300251C. The address is halfword- but not word-aligned, so this is
 * a plain u16 array and not an aggregate. */
extern const u16 gUnknown_085538AE[];
/* Two adjacent ROM u16 pairs, each indexed by gUnknown_0300453C: sub_08051BEC
 * uses gUnknown_08553B10 and sub_08051F4C uses gUnknown_08553B14, and each
 * binds the element to a local and hands it to sub_080157A4, which takes s16.
 * The tables stay unsigned; the sign extension belongs to that parameter, not
 * to the element. The two symbols are 4 bytes apart and are probably one table
 * of pairs, kept separate because each function names its own symbol. */
extern u16 gUnknown_08553B10[];
/* A blob whose address sub_080523E8 passes as sub_08015410's `void *` first
 * parameter, with no dereference, exactly where its neighbours pass an element
 * of gUnknown_085535B8 -- so it is the pointed-to data, not a table of
 * pointers. */
extern u8 gUnknown_085536EC[];
extern u16 gUnknown_08553B14[];
extern u32 *gUnknown_08555450[];
extern struct Unk08580934 *gUnknown_08580934;
/* Two ROM halfword tables of per-frame Y offsets, indexed by the object's own
 * unk26 countdown. sub_08064C34 stores gUnknown_085809D8[unk26] into unk2a;
 * sub_08064D74 adds unk38 to gUnknown_085809F0[unk26] first. Extents unknown:
 * unk26 is bounded only by the countdown's start value, which neither function
 * sets. */
extern const u16 gUnknown_085809D8[];
extern const u16 gUnknown_085809F0[];
/* Three bases into one run of ROM halfwords, declared as three arrays because
 * that is what reproduces the ROM: sub_080646D4, sub_08064FC8 and sub_08065118
 * are the same code apart from which symbol each loads, and the addresses are
 * contiguous (0x08580948 + 160*2 == 0x08580A88, and + 27*2 == 0x08580ABE).
 * Each is read as `t[obj->unk26]`, the vertical offset of a falling or arcing
 * sprite, and the values are signed: 0x08580948 runs 0, -4, -6, -7, -7, -5,
 * -1, 3, 10, 18 ... and 0x08580ABE has -3, -13, -24. The element is loaded
 * unsigned because the sum is stored straight back into a halfword, which
 * truncates it anyway. */
extern s16 gUnknown_08580948[];
extern s16 gUnknown_08580A88[];
extern s16 gUnknown_08580ABE[];
/* A gUnknown_03001470 script blob, not a proc script: sub_080670BC hands it to
 * sub_080152EC(script, 2) and sub_080670D8 is the matching
 * `sub_08015BD0(script) != -1` liveness test. */
extern const u8 gUnknown_08580DD8[];
/* Four more ROM blobs of the same 8-byte {THUMB function pointer, u32 flags}
 * record shape, but with a different consumer from the gUnknown_03001470 lists:
 * sub_08063A30 stores the pointer at +0x04 of the object in r0, then zeroes
 * +0x08 and +0x10, and the four wrappers sub_08065F68, sub_08065F78,
 * sub_08066200 and sub_08066210 are nothing but that store. Their address does
 * not make them gUnknown_03001470 blobs: none of them has the sub_080152EC /
 * sub_0801537C install-and-remove pair that would show it. */
/* A proc script reached only as an address: sub_0806502C is
 * `sub_080152EC(gUnknown_08580A38, 3)`, and sub_080152EC's first parameter is
 * `const void *`. */
extern const u8 gUnknown_08580A38[];
extern const u8 gUnknown_08580A68[];
extern const u8 gUnknown_08580B18[];
extern const u8 gUnknown_08580C00[];
extern const u8 gUnknown_08580C20[];
/* Two ROM blobs handed to sub_0801BD00's `void *` third parameter by
 * sub_080655B0, picked on whether gUnknown_08580934->unk30 is zero. 14 bytes
 * apart and both opening with the word 0x40000002, so they are the same shape.
 * Nothing indexes or dereferences them, so the element type is
 * unconstrained. */
/* A halfword colour table. sub_08066EBC reads
 * `gUnknown_0817AF18[0x20 + ((gGameClock >> 1) & 0xf)]`; the sixteen RGB15
 * values from index 0x20 ramp 0x0000, 0x0C61, 0x18C2, 0x2D64, 0x3DE7, 0x4E69,
 * 0x672C, 0x7FEF and back down again, a symmetric fade. Must stay non-const:
 * sub_08066EBC re-reads the element after storing into gPal instead of reusing
 * the loaded value, which agbcc only does while the two u16 objects might
 * alias. */
extern u16 gUnknown_0817AF18[];
/* Three halfword graphics-id tables sub_08066F20 selects between and indexes.
 * The value is compared against and then stored into gUnknown_08580934->unk2e,
 * the graphics id currently loaded. gUnknown_08580D6C holds six entries
 * (0x09D1..0x09D6) indexed by the s8 unk33, whose seventh value, 6, is the case
 * that diverts to gUnknown_08580D88 instead; gUnknown_08580D88 holds four
 * (0x09D7..0x09DA) indexed by an unk54[] entry's u8 unk48; gUnknown_08580D78
 * alternates 0x09DC / 0x09DD over its eight slots. */
/* Three more proc scripts in the 0x0858xxxx table: sub_0806675C hands each one
 * to sub_0806377C, whose parameter is `const void *`, with no arithmetic in
 * front. gUnknown_08580AF0 also goes to sub_08063A00 as the script to match
 * against a Unk03001470 slot's unk00, which is the same use. */
extern const u8 gUnknown_08580A08[];
extern const u8 gUnknown_08580D90[];
extern const u8 gUnknown_08580AF0[];
extern const u8 gUnknown_08580B90[];
extern const u8 gUnknown_08580BC8[];
/* Two more proc-script blobs in the same table: sub_08066A20 hands
 * gUnknown_08580D3C and sub_08066874 hands gUnknown_08580D54 to sub_080152EC,
 * whose first parameter is `const void *`, with no arithmetic in front. Each is
 * 0x18 bytes. */
extern const u8 gUnknown_08580D3C[];
extern const u8 gUnknown_08580D54[];
extern const u16 gUnknown_08580D6C[];
extern const u16 gUnknown_08580D78[];
extern const u16 gUnknown_08580D88[];
extern u8 gUnknown_08580C40[];
extern u8 gUnknown_08580C4E[];
/* A halfword ease table indexed by an s16 countdown: 0, 2, 6, 0xc, 0x14, 0x1e,
 * 0x2a ..., second differences constant at 2, so a quadratic ramp. Signed --
 * sub_080655B0 sign-extends the element, feeds it to sub_0801BD00's s32 first
 * parameter and also uses it in a `0xc0 - x` subtraction. */
extern s16 gUnknown_08580C5C[];
/* Proc scripts in the 0x0858xxxx table, each the sole script argument of one
 * Proc_Start or Proc_Find call. `const` because that is what proc.h's
 * prototypes take; they are ROM data and nothing writes them.
 *   08580E94  Proc_Start, sub_080673B0 (fills +0x2c/+0x38/+0x3c)
 *   08580EAC  Proc_Start, sub_080673D0 (fills +0x2c/+0x38/+0x3c)
 *   08580EC4  Proc_Start, sub_080673F0 (fills +0x2c/+0x38/+0x3c)
 *             -- three consecutive 0x18-byte scripts with three identical
 *             starters, one per variant
 *   08580FE4  Proc_Find,  sub_0806780C (+0x5c)
 *   08580FF4  Proc_Find,  sub_080678BC (+0x60) and sub_080678D4 (+0x3c)
 *   08581014  Proc_Find,  sub_08067A24 (+0x50, u8)
 *   08581068  Proc_Find,  sub_08067C7C (+0x38)
 *   085810E4  Proc_Start, sub_08067DD4 (no fields written)
 *   08581420  Proc_Find,  sub_0806978C/sub_080697A4 (+0x36, u8 = 0/1)
 *   08582AF4  Proc_Start, sub_0806E210 (+0x58) and Proc_Find, sub_0806E228,
 *             both writing +0x58 = arg + 1
 *
 * 08580FE4 is reached two ways: sub_0806780C finds it and writes +0x5c = 1,
 * while sub_080677BC starts it under its own fourth argument and writes
 * +0x2c/+0x30/+0x34/+0x38/+0x58 as well as the same +0x5c = 1.
 */
extern const struct ProcCmd gUnknown_08580E94[];
extern const struct ProcCmd gUnknown_08580EAC[];
extern const struct ProcCmd gUnknown_08580EC4[];
/* A ROM table of pointers to signed byte arrays, indexed by a plain int.
 * sub_0806775C stashes one whole element in a proc at +0x2c and that element's
 * [0] at +0x30; the read is a signed byte load, so the pointee is s8, not u8. */
extern s8 *gUnknown_08580FC0[];
/* 08580FCC  Proc_Start, sub_0806775C, started under the starter's own second
 *           parameter. */
extern const struct ProcCmd gUnknown_08580FCC[];
extern const struct ProcCmd gUnknown_08580FE4[];
extern const struct ProcCmd gUnknown_08580FF4[];
extern const struct ProcCmd gUnknown_08581014[];
extern const struct ProcCmd gUnknown_08581068[];
extern const struct ProcCmd gUnknown_085810E4[];
/* Three ROM tables of Decompress sources, each indexed by a plain int and
 * dereferenced straight into Decompress's `u8 *` first parameter, which is why
 * neither the elements nor the arrays are const -- Decompress takes non-const.
 *   08581050  sub_08067B90, into raw VRAM 0x06001400
 *   0858105C  sub_08067B90, into *gUnknown_0849957C
 *   085810C8  sub_08067D04, into raw VRAM 0x06010000                       */
extern u8 *gUnknown_08581050[];
extern u8 *gUnknown_0858105C[];
extern u8 *gUnknown_085810C8[];
/* A ROM table of signed bytes: sub_08067D94 walks it with an s16 proc counter
 * running 0..0xc and reads each element as a signed byte. The values feed
 * sub_08072C40's third (u16) parameter as a fade level. */
extern const s8 gUnknown_085810D4[];
/* A ROM halfword table indexed by a proc's u8 field at +0x2a. Both readers hand
 * the element on as an OBJ tile id: sub_08067DF8 to sub_08043BF8's second
 * parameter, sub_08067E88 OR'd with 0x2000 into sub_08043C28's third. */
extern const u16 gUnknown_08581104[];
/* More 0x0858xxxx proc scripts, each the sole script argument of one starter:
 *   08581108  Proc_Find,  sub_0806A4B0 (+0x3d, u8)
 *   08581138  Proc_Start, sub_08068014 (+0x2c/+0x30/+0x34 = args, +0x40 = 0)
 *   08581480  Proc_Start, sub_08069FAC (+0x34/+0x38/+0x3c = args, +0x40 = 0)
 *   08581AC8  Proc_Start(.., PROC_TREE_3), sub_0806C874 (no fields written)
 */
extern const struct ProcCmd gUnknown_08581108[];
/* Two more 0x0858xxxx proc scripts, each the sole script argument of one
 * Proc_Start:
 *   08581210  sub_080686E8 -- writes +0x29, +0x30 (halfword), +0x32, +0x2a and
 *             +0x4d, then clears +0x4f.
 *   08581264  sub_08068810 -- the same fields minus +0x4d, plus +0x38, +0x39
 *             and +0x4e. */
extern const struct ProcCmd gUnknown_08581210[];
extern const struct ProcCmd gUnknown_08581264[];
/* A ROM blob handed straight to sub_0801BD00's `void *` third parameter by
 * sub_0806A7B4, with no arithmetic on the symbol: the address itself is the
 * argument. Not const, because that parameter is a plain `void *`. */
extern u8 gUnknown_085815C0[];
/* Two Decompress `u8 *` sources in sub_08068A00's cutscene switch, each handed
 * to Decompress by name with no arithmetic:
 *   0817DE24 -> 0x06009400 (a BG map/tile blob, uploaded in the same arm that
 *              turns BG0/1/2 and OBJ on and BG3 off)
 *   0818E364 -> 0x06010000 (OBJ tile VRAM)
 * Non-const, because Decompress takes a plain `u8 *`. */
extern u8 gUnknown_0817DE24[];
extern u8 gUnknown_0818E364[];
/* sub_0806A8E4's camera path: signed byte (dx, dy) pairs walked by the proc's
 * own +0x5a step counter, stride 2. dx is negated and shifted left 12 into a
 * Q12 offset; dy is compared against 0x5a as the end-of-path sentinel.
 * Must stay flat, not `[][2]`: the ROM adds the column offset to the index,
 * where the two-dimensional spelling folds it into the array's base address. */
extern const s8 gUnknown_08581608[];
/* 0858168C  Proc_Find, sub_0806AAC4 -- the bare existence predicate
 *           `return Proc_Find(script) != 0;`, no proc field touched. */
extern const struct ProcCmd gUnknown_0858168C[];
/* 08581500  the same existence predicate again, sub_0806A474. */
extern const struct ProcCmd gUnknown_08581500[];
extern const struct ProcCmd gUnknown_08581138[];
/* The (x, y) offset pairs sub_08068770 indexes with `proc->unk4f % 10 / 2` --
 * a five-phase animation cycle over a ten-frame counter. Signed, and this is
 * its only reference in the ROM.
 * Must stay flat `s16 []` indexed `[k * 2]` and `[k * 2 + 1]`, not `s16 [][2]`:
 * the two-dimensional spelling lets the compiler reach the second element from
 * the first, where the ROM recomputes it from the index.
 *
 * gUnknown_0858125C is the sprite template that goes with it, handed straight
 * to PutSprite. Non-const `u16 []` only because that is what PutSprite's
 * fourth parameter is declared as; nothing writes the object. */
extern const s16 gUnknown_08581248[];
/* sub_08068038's glyph table: 8-byte records of a character code and the
 * compressed tile blob for it, scanned linearly until the code matches --
 * exactly, or with 0x20 added, which is the case-folding half of the test --
 * and terminated by a record whose code is 0xFF, at which point record 12 is
 * used as the fallback glyph.
 *
 * One table, not two arrays, although the splitter also invented
 * gUnknown_08581164 for the second field. With `-fforce-addr`, `tbl[j].data`
 * has the address `symbol + j * 8 + 4` and the constant folds into the
 * relocation, so the second field gets a pool word of its own. The fallback arm
 * reaches this same table at a folded displacement of 0x64 == 12 * 8 + 4, which
 * is what fixes the record size at 8 rather than 4.
 *
 * `data` is `u8 *` to agree with Decompress's first parameter. Nothing reads
 * +0x01..+0x03, so their purpose is unknown. */
struct Unk08581160Entry
{
    /* 0x00 */ u8 chr;
    /* 0x01 */ u8 filler_01[0x03];
    /* 0x04 */ u8 *data;
};

extern const struct Unk08581160Entry gUnknown_08581160[];
extern u16 gUnknown_0858125C[];
/* The 8-byte OAM blob the 0x08581210 menu proc draws each of its rows with:
 * sub_080681B8 hands the address to PutSprite's `u16 *` fourth parameter and
 * sub_080682E8 / sub_080684E0 hand the same address to PutSpriteExt's, with no
 * arithmetic and no dereference in any of the three. Non-const `u16 []` only
 * because that is what those shared prototypes declare; nothing writes it. */
extern u16 gUnknown_08581208[];
extern const struct ProcCmd gUnknown_08581420[];
/* Four pairs of signed bytes, read by sub_0806974C alone: one loop of four
 * passes copies [i][0] and [i][1] into two parallel s16 runs at +0x2a and
 * +0x30 of the gUnknown_08581420 proc it has just started. Non-const only to
 * follow the surrounding 0x0858xxxx tables. */
extern s8 gUnknown_08581430[][2];
extern const struct ProcCmd gUnknown_08581480[];
extern const struct ProcCmd gUnknown_08581AC8[];
extern const struct ProcCmd gUnknown_08582AF4[];
/* The proc script sub_0806F41C ends and restarts around its screen rebuild --
 * Proc_EndEach then Proc_Start on the same symbol. */
extern const struct ProcCmd gUnknown_08582CAC[];
/* A POINTER to a table of 0x10-byte screen-content records, indexed by
 * sub_0806F41C's proc `s8` at +0x38. Must stay a pointer, not an array: every
 * reader loads its value before indexing.
 *
 * unk00 is `u32` because it is two things at once: sub_0806F41C compares it
 * unsigned against 0x11 and treats values <= 0x11 as a small id handed to
 * sub_0806AEC4, anything larger as a Decompress source address. */
struct Unk0816E808Entry /* 0x10 */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u8 *unk04;  /* Decompress source, into whatever gUnknown_08499578
                            * points at */
    /* 0x08 */ u16 *unk08; /* ApplyPaletteExt's `u16 *` first parameter */
    /* 0x0c */ u8 unk0c;   /* palette length in 0x20-byte units; times
                            * 0x20 is ApplyPaletteExt's third argument */
    /* 0x0d */ u8 unk0d;   /* == 1 selects the 256-colour bit of the
                            * gUnknown_03002B6C BG shadow */
    /* 0x0e */ u16 unk0e;
};
extern struct Unk0816E808Entry *gUnknown_0816E808;
/* A gUnknown_03001470 script blob, not a proc script: sub_0806E17C hands it to
 * sub_080152EC(script, 2) and sub_0806E198 is the matching
 * `sub_08015BD0(script) != -1` liveness test. */
extern const u8 gUnknown_08581F7C[];
extern const struct Unk085C77A0 gUnknown_085C77A0[];
/* Three ROM blob boundaries, used only as addresses and never dereferenced.
 *
 * sub_08033194 publishes gUnknown_08584CE8 as a base and
 * `gUnknown_085867D8 - gUnknown_08584CE8` as a byte length -- a bare
 * subtraction with no division, which only byte-sized elements produce.
 *
 * sub_08033470 DMAs gUnknown_0858D0D0 to 0x02000000 with a word count of
 * `(gUnknown_085C77A0 - gUnknown_0858D0D0) / 4`; gUnknown_085C77A0 is a struct
 * array above, so that subtraction is written through `(u8 *)` casts. */
extern u8 gUnknown_08584CE8[];
extern u8 gUnknown_085867D8[];
extern u8 gUnknown_0858D0D0[];
/* A real four-entry ROM table of blob boundaries, not part of agbcc's
 * `-fforce-addr` pool of address constants, even though it sits beside genuine
 * pool words. The words are { 0x08584CE8, 0x085867D8, 0x0858D0D0, 0x085C77A0 }:
 * two ascending start/end pairs, each consumed as a base plus a length.
 * sub_08033194 publishes [0] and [1] - [0]; sub_08033470 DMAs [2] with a word
 * count of ([3] - [2]) / 4.
 *
 * What makes it a table and not a pool: two different functions read different
 * elements of the one 16-byte run off a shared base register, and a
 * `-fforce-addr` pool is private to one function. Naming the four globals
 * directly emits a separate pool word per symbol and does not match. */
extern u8 *const gUnknown_08090D5C[];
extern const s16 gUnknown_08580E64[];
/* A ROM byte per unit-type id, indexed by struct Unit's unk00. At an odd
 * address, so a plain `u8 []` and not an aggregate. sub_08058254 and
 * sub_08058318 keep only the units whose entry reads exactly 2, the same class
 * tag role gUnknown_085D5ABC[type].unk1b plays for the 0x0805Cxxx list
 * builders. Extent unknown: only ids > 2 are ever looked up. */
extern u8 gUnknown_0857680F[];
/* Another ROM byte per unit-type id, indexed by struct Unit's `type` exactly
 * like gUnknown_084995A8 beside it. sub_0805A5E0 scores a candidate with
 * `(5 - gUnknown_08576828[p->unk00]) * 16`, so the byte is a small rank in 0..5
 * and the subtraction inverts it: a lower table value means a higher score. */
extern const u8 gUnknown_08576828[];
/* A pair of class tables that define each other. sub_0805A388 accepts a map
 * cell exactly when
 *     gUnknown_0857685A[terrain & 0x1f] == gUnknown_08576841[unit->unk00]
 * so one is a unit-type-to-class map and the other a terrain-to-class map,
 * joined on a shared small class id.
 *   gUnknown_08576841 is indexed by a unit-type id, like gUnknown_08576828 and
 * gUnknown_084995A8 above it. Its bytes are
 *     00 01 01 01 01 01 01 01 01 01 01 01 04 01 01 01 02 02 02 02 02 03 03 03 03
 * then zeros -- 25 meaningful entries with values 0..4, the same 25-code extent
 * the terrain tables use.
 *   gUnknown_0857685A is indexed by the low five bits of struct Map's +0x1432
 * plane, the same subscript gUnknown_085767D5 and gUnknown_085767F2 take. Its
 * bytes are 00 00 00 00 00 00 01 00 01 00 02 03 00 00 01 00 then zeros, values
 * 0..3. It is nearly gUnknown_085767F2 halved, but not identically: index 8
 * reads 1 here against 0 there, so they are two objects.
 *   Both must stay non-const. A const global's loaded value survives a
 * control-flow merge where a non-const one does not, so adding const could let
 * the compiler reuse a load the ROM recomputes. Extents unknown for both: only
 * a masked or small subscript ever reaches them. */
extern u8 gUnknown_08576841[];
extern u8 gUnknown_0857685A[];
/* A ROM byte per terrain code, indexed by the low five bits of struct Map's
 * +0x1432 plane. At an odd address, so a plain `u8 []` and not an aggregate --
 * the same shape as gUnknown_0857680F above. sub_0805C128 uses it as a
 * predicate only: non-zero selects the cells that must additionally match
 * gUnknown_03004084 in their top three bits, so nothing constrains the value
 * range beyond zero and non-zero. Extent unknown -- only codes 0..0x1f can
 * reach it, and 0x0b and 0x0d are rejected before the lookup. */
extern u8 gUnknown_085767D5[];
/* A second ROM byte-per-terrain table taking exactly the same subscript as
 * gUnknown_085767D5 above -- the low five bits of struct Map's +0x1432 plane --
 * read by sub_08059674 immediately after it. The value is not a flag:
 * sub_08059674 compares it for equality against the u8 gUnknown_030046AC as
 * well as against zero, so it is a small id drawn from the same space as that
 * variable. Extent unknown; gUnknown_085767D5 begins 0x1d bytes later, which
 * caps the part this reader can touch at 29 entries. Must stay non-const, like
 * gUnknown_085767D5: const would let the compiler reuse a load the ROM
 * recomputes. */
extern u8 gUnknown_085767B8[];
/* A ROM table of 24-byte rows indexed by gUnknown_03004582[i][0]. Column 0 is
 * read as a halfword and is the first argument of sub_08057D44; sub_0804C098
 * tests column 9 (+0x12) against zero. Rows 0..3 read 0/1/2/3 in column 0, so
 * column 0 is the row's own id. Left non-const: nothing indexes it inside a
 * loop yet, so const would buy nothing.
 *
 * The rows are probably structs rather than `u16 [12]`. sub_0804FA2C reads
 * column 9 with the column offset in the load's displacement, which an array
 * row cannot produce: the compiler pulls the constant out of the address sum
 * and pays two extra instructions, where a struct member applies its offset to
 * the load instead. It is left as `[][12]` because the change is not free at
 * column 0, where the array spelling is what matches, and every promoted reader
 * uses column 0. Retyping is a job for whoever closes sub_0804FA2C; the 24-byte
 * row would be `{ u16 unk00; u8 filler_02[0x10]; u16 unk12; u8 filler_14[4]; }`.
 * See the "column-offset fold" section in docs/agbcc-codegen.md. */
extern u16 gUnknown_085D6A48[][12];
/* == gUnknown_085D6A48 + 0xa, i.e. column 5 of the same 24-byte rows reached
 * through its own linker symbol -- the same device gUnknown_02029924 and
 * gUnknown_08551D2A are for their tables. sub_08054C5C reads
 * `[gUnknown_03004582[i][0]][k]` off a pool word of exactly 0x085D6A52 in the
 * same loop where it reads column 0 of gUnknown_085D6A48 through the bare
 * symbol, so both spellings are live in one function -- which is what fixes
 * this as a separate declared view rather than a `[..][k + 5]` subscript. The
 * row length is the 24-byte stride, not a proved extent. */
extern u16 gUnknown_085D6A52[][12];
/* A 0x40-byte ROM blob sub_08054C5C hands to CpuFastSet as 0x10 words into
 * 0x05000300, i.e. bank 6 of the object palettes -- two 16-colour palettes.
 * Only its address is used. */
extern const u16 gUnknown_08540F7C[];
/* An 8-byte record hanging off the same index pair as gUnknown_085D6A48: the
 * outer subscript is gUnknown_03004580[i][1] with a 40-byte stride and the
 * inner gUnknown_020296B0[i].unk18 with an 8-byte stride, both read off
 * sub_08050E08's address chain. Inside the record sub_08050E08 reads `unk00[i]`
 * -- one halfword per gUnknown_0300453C side -- and `unk04`, and adds the pair
 * to a coordinate as a (width, height) style offset. Must stay a struct: as a
 * flat `u16 [][5][4]` the constant last subscript folds into the base address,
 * where the ROM keeps it in the load's displacement. unk06 has no reader. */
struct Unk085D81E8 /* 0x08 */
{
    /* 0x00 */ u16 unk00[2];
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[2];
};
extern struct Unk085D81E8 gUnknown_085D81E8[][5];
/* A ROM table of 20-byte rows, the row indexed by
 * gUnknown_085D6A48[gUnknown_03004580[i][1]][0], so it hangs off the same chain
 * gUnknown_08552FB8 does. sub_0805198C reads column 0 as a halfword and tests
 * it against 0 as a "this side has one" guard; nothing else in `asm/` reaches
 * any other column, so the `[10]` row length is the measured 20-byte stride and
 * not a proved layout. */
extern u16 gUnknown_085D6EC8[][10];
/* A ROM table of five 8-byte records per row: the row is indexed by
 * gUnknown_03004580[i][1] and the record by gUnknown_0300451C, so it is the
 * per-slot pixel offset that goes with gUnknown_02029A10[i].entries[j]. The row
 * stride is 40 bytes and the record stride 8, as read off sub_08051DE0,
 * sub_080524C0 and sub_0805297C. unk00 is indexed by gUnknown_0300453C (the
 * side) and added to the x sum; unk04 is not side-indexed and is added to the y
 * sum -- that asymmetry is the ROM's. Only those two of the four halfwords have
 * a reader. Must stay a struct, like gUnknown_08552D80: unk04 needs its
 * constant in the load's immediate, which a flat `u16 [][5][4]` folds into the
 * symbol's address instead. */
struct Unk085D7E28 /* 0x08 */
{
    /* 0x00 */ u16 unk00[2];
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
};
extern struct Unk085D7E28 gUnknown_085D7E28[][5];
/* A ROM table of signed halfwords, handed straight to sub_0803B48C, the
 * sound-id call. Three sites index it, in sub_0804E7A8 and sub_0804FCA4, and
 * between them they fix the shape:
 *   byte offset = (gUnknown_020296B0[i].unk1a & 1) * 2
 *               + (gUnknown_03004580[i][2] - 1) * 4
 *               + gUnknown_03004580[i][1] * 24
 * so the row is 24 bytes = 6 halfwords = [6][2], the inner pair selected by the
 * alternating phase bit and the middle index by a 1-based count.
 *
 * Two symbols, one table, as gUnknown_0855239C and gUnknown_085523A4 already
 * are: the base is 0x085D6C88 and gUnknown_085D6C94 is row [3] of it. Only rows
 * 3 and 4 have a reader, and the extents are unknown in both directions.
 * The two views must keep their different types. sub_0804FCA4 needs its two
 * constant offsets added at run time off one shared pool word, which only a
 * struct member array produces; sub_0804E7A8 needs the whole `+0xc` inside the
 * relocation, which only a plain array subscript with no constant index
 * produces. */
struct Unk085D6C88 /* 0x18 */
{
    /* 0x00 */ s16 unk00[2][2]; /* A pair of pairs at the head of the
                                 * row, on the same two indices unk0c uses:
                                 * sub_08053FBC reads
                                 * `unk00[g03004580[i][3] == 2][g02029C04[i] & 1]`
                                 * off `row * 24` and hands it to
                                 * sub_0803B48C as an `s16`. */
    /* 0x08 */ s16 unk08[2]; /* A third entry group on the same row,
                              * between unk00 and unk0c and on the FIRST
                              * of their two indices only: sub_080541F0
                              * reads `unk08[gUnknown_03004580[i][3] == 2]`
                              * off `gUnknown_03004580[i][1] * 24` and
                              * hands it to sub_0803B48C as an `s16`. */
    /* 0x0c */ s16 unk0c[2][2];
    /* 0x14 */ u8 filler_14[0x04];
};
extern struct Unk085D6C88 gUnknown_085D6C88[];
extern s16 gUnknown_085D6C94[][6][2];
/* A ROM table of string pointers indexed by a u32; sub_08039F18 takes the index
 * out of gUnknown_085D3DD0[..].unk38[..].unk00. The pointed-to bytes are a
 * NUL-terminated string: sub_08039F18's result goes straight into sub_08039544,
 * which copies bytes until the first zero. Left non-const because sub_08039544
 * has no prototype yet and takes a plain `u8 *`. */
/* NOT GLOBALS. 0x0816D9E0-0x0816DA43 is agbcc's own `.rodata` pool of address
 * constants: twelve consecutive pairs plus one lone word, one entry per LOOPING
 * list-builder function at 0x0805CA60-0x0805D2A0, in address order. Every pair
 * holds &gUnknown_030046B0 and &gUnknown_030045F0; the lone word at 0x0816DA40
 * holds &gUnknown_030044B0 for sub_0805D5EC. Do not declare any of them as
 * globals -- the source is the plain
 *     gUnknown_030046B0 = gUnknown_030045F0;
 * and `-fforce-addr` synthesised the pair because the address stays live across
 * the builder's loop. The two builders with no loop, sub_0805CDF0 and
 * sub_0805CE20, have no pair, which is what pins the rule. See "The .rodata
 * address-constant reroute" in docs/agbcc-codegen.md. */
/* NOT GLOBALS EITHER. 0x0816DA44-0x0816DACC continues the same `-fforce-addr`
 * pool of address constants past the builder run above, one entry per
 * referencing function in address order, and must not be declared either.
 * Dereferenced against the ROM these words hold &gUnknown_030040D8,
 * &gUnknown_08499590, &gUnknown_085D5ABC and &gUnknown_03003F2C -- addresses of
 * globals that ARE declared, which is exactly why no symbol belongs here.
 *
 * The diagnostic that tells this apart from a genuine local literal, since the
 * two look alike in a diff: compare the offsets on the two relocation lines.
 * The same offset (`44: gUnknown_0816DA98` against `44: .rodata`) is the honest
 * spelling working correctly -- leave it alone. Different offsets (`ec:`
 * against `e8:`) means the pool really gained or lost a word and something
 * upstream is wrong.
 *
 * Related: gUnknown_085D5AD0 is not an object either. It is
 * &gUnknown_085D5ABC[0].unk14, i.e. base + 0x14 on the 0x5c-stride array below.
 * Declaring it separately leaves the generated code byte-identical and costs a
 * false object. Do not add it. */
/* 0x0816E164 is another slot of the same pool and holds the same value,
 * 0x08580934. sub_08066D30 is sub_0806DDF4's twin over it, down to the loop
 * bound and the +0x46 flag, so it takes the same type and the same `const`
 * pointer-to-the-symbol treatment. */
/* Four halfwords (0x00cb, 0x00ce, 0x00cf, 0x00d0 in the ROM) that sub_080649D0
 * copies onto an 8-byte stack buffer with sub_0808B6E8 and then indexes with
 * obj->unk48 to pick a sprite id. The extent is the copy length. */
extern const u16 gUnknown_0816E0C8[4];
extern struct Unk0816E1B8 *const gUnknown_0816E164;
/* 0x20 bytes of ROM that sub_08065EF4 copies onto its stack and then indexes
 * with `unk32 & 1`, reading the s16 at +8/+0xa for one call and +0xc/+0xe for
 * the other. The values are
 *   { 0, 0, 0x30, 0x30, 0x10, -8, 0x10, 0x30 }
 *   { 0x28, 0, 0x10, 0x10, 0x28, -9, 0x28, 0x10 }
 * A local aggregate with that initialiser generates the same code, but its pool
 * word relocates against a fresh `.rodata` label instead of this address, which
 * the match tooling cannot resolve -- naming the object here is the spelling
 * that verifies. Whether the original source had a global or a local cannot be
 * told apart in the ROM. */
struct Unk0816E120Entry
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s16 unk08;
    /* 0x0a */ s16 unk0a;
    /* 0x0c */ s16 unk0c;
    /* 0x0e */ s16 unk0e;
};

struct Unk0816E120
{
    /* 0x00 */ struct Unk0816E120Entry unk00[2];
};

extern const struct Unk0816E120 gUnknown_0816E120;
/* See struct Unk0816E1B8 for why this is const and why it is an extra level of
 * indirection in front of gUnknown_08580934 rather than a global of its own. */
extern struct Unk0816E1B8 *const gUnknown_0816E1B8;
extern u8 *gTextTable[];
/* A ROM u16 table read as `gUnknown_08616F0C[unk1d * 4 + gUnknown_03005940]` by
 * sub_080852A8. Must stay flat, not `[][4]`: the ROM scales the row index and
 * adds the column before one element shift, where a `u16 [][4]` would scale
 * each index separately. Both indices are variables here, which is what makes
 * the two spellings tell apart at all. */
extern u16 gUnknown_08616F0C[];
/* A NULL-terminated table of six nullary predicates, walked by sub_08084804,
 * which calls each one through a register and zero-extends the result to a byte.
 * The seven words at 0x08616B00 hold
 *   sub_080848B5, sub_08084939, sub_08084921, sub_080848FD, sub_08084971,
 *   sub_080848D9, 0
 * (odd, because they are THUMB), and every one of the six is a `bool8 f(void)`
 * that scans a range and returns FALSE on the first failure. sub_08084804's
 * loop runs exactly six passes, so the terminator is never read there; the
 * array is left incomplete rather than [6] on that basis. */
extern u8 (*const gUnknown_08616B00[])(void);
/* Proc scripts. sub_0807639C and sub_0807F8D0 hand 08614460 / 08616740 to
 * Proc_Start(script, parent); sub_08078540 hands 08615AAC to
 * Proc_StartBlocking and stashes a pointer at +0x54 of the new proc. */
extern const struct ProcCmd gUnknown_08614460[];
extern const struct ProcCmd gUnknown_08615AAC[];
/* Proc_StartBlocking'd by sub_0807F82C under the proc it just found through
 * gUnknown_086165C0; sub_0807F618 is its existence predicate. */
/* Two more proc scripts of the 0x0807B000 tree, each the first argument of a
 * `Proc_Start(script, proc)`: 0861604C from sub_0807B760 and 08616570 from
 * sub_0807BA68 and sub_0807BE90, all three gated on a gpKeySt->held bit. */
/* The proc script sub_0803EF44 hands to Proc_StartBlocking under its own third
 * argument, writing +0x2c and +0x30 of the new proc from its first two before
 * passing the same pair, narrowed to s16, to sub_0802909C. */
extern const struct ProcCmd gUnknown_0849F7F0[];
/* sub_0803F0A4's script. */
extern const struct ProcCmd gUnknown_0849F830[];
extern const struct ProcCmd gUnknown_0861604C[];
extern const struct ProcCmd gUnknown_08616570[];
extern const struct ProcCmd gUnknown_08616710[];
/* Two more scripts in the same run: sub_0807CE5C hands gUnknown_08616690 to
 * Proc_Start with itself as parent and immediately Proc_Break()s, and uses
 * gUnknown_086166A8 with both Proc_Find and Proc_Start. Proc_Find's parameter
 * type is what fixes these as scripts rather than data; their first halfwords in
 * the ROM are 2 and 0x11, i.e. opcodes. */
extern const struct ProcCmd gUnknown_08616690[];
extern const struct ProcCmd gUnknown_086166A8[];
extern const struct ProcCmd gUnknown_08616740[];
/* Proc_Start'd by sub_08087884 under a caller-supplied parent, with one word
 * of payload stashed at +0x54 of the new proc. */
/* Two proc scripts sub_08086EB0 drives as an either/or pair: it hands both to
 * Proc_EndEach / Proc_Find and starts exactly one of them under PROC_TREE_3 --
 * 08616D1C when the byte it looks up is <= 0xb3 and sub_0803CA54 accepts it,
 * 08616D6C otherwise. The 08616D1C proc carries an s16 at +0x66, the same field
 * src/decomp/c_08086D98.c reads back. */
extern const struct ProcCmd gUnknown_08616D1C[];
extern const struct ProcCmd gUnknown_08616D6C[];
extern const struct ProcCmd gUnknown_08616D94[];
/* Both from sub_080849C8, the 0x08084 tree's screen setup. 08616BE4 is a proc
 * script: `Proc_Start(script, parent)` with the function's own ProcPtr argument
 * as the parent, reached through a `-fforce-addr` pool word at 0x081D93E4.
 * 08616B1C is a byte table indexed by gPlayers[gUnknown_030033EC].unk1a off the
 * bare symbol, so the symbol address IS the base and the element is one byte;
 * the value goes to sub_0802D5A0's `int` second parameter with no narrowing, so
 * the signedness is unknown and u8 is the weakest fit. Extent unknown: unk1a is
 * a small army key and nothing bounds the table. Not const, like the tables
 * above. */
extern u8 gUnknown_08616B1C[];
/* From sub_08085708: the two twenty-entry id tables its `?:` picks between on
 * sub_080261E8's bool, read at indices 0..0x13. u16 because the read is a bare
 * halfword load and the value goes to sub_080432E0 / sub_08085410 /
 * sub_08043190's `int` parameters with no narrowing, so nothing here proves a
 * sign. The extents are exact and mutually confirming: 0x08616B22 + 20*2 ==
 * 0x08616B4A, and 0x08616B4A + 20*2 == 0x08616B72, which is where
 * gUnknown_08616B74 starts after two bytes of alignment. `const` because both
 * pool words are clean ROM addresses and nothing writes either table. */
extern const u16 gUnknown_08616B22[];
extern const u16 gUnknown_08616B4A[];
extern const struct ProcCmd gUnknown_08616BE4[];
/* Both from sub_08084C14 and both plain `Proc_Start(script, proc)` first
 * arguments. 08616C24 is also the script sub_08084C14 probes with Proc_Find
 * before it will accept any input -- the "a submenu is already open" guard --
 * and it is reached through the `-fforce-addr` pool word at 0x081D93F0;
 * 08616BFC is named by a direct pool word. */
extern const struct ProcCmd gUnknown_08616BFC[];
extern const struct ProcCmd gUnknown_08616C24[];
/* From sub_080860DC: `Proc_Find(gUnknown_08616CCC)` as an existence guard and
 * `Proc_Start(gUnknown_08616CCC, proc)` in two of that function's arms, both by
 * name with no arithmetic. */
extern const struct ProcCmd gUnknown_08616CCC[];
/* A ROM halfword table: every user computes `index * 2 + &g` and reads a
 * halfword (sub_08074D28, sub_08074EEC, sub_08074FE4, sub_08075008 and four
 * more), so the symbol address is the array base and not a pointer. Must stay
 * non-const: sub_08074EEC reads one element inside a five-pass loop and the ROM
 * reloads it every pass, where const lets the compiler hoist the load out of
 * the loop. */
extern u16 gUnknown_0861433C[];
/* The 0x30-byte record array behind struct Unk08615194 above. */
extern const struct Unk08615194 gUnknown_08615194[];
/* Two more scripts of the 0x08614xxx block, each fixed by the starter it is
 * handed to: 08614268 goes to `Proc_Start(script, PROC_TREE_3)` in sub_08074320
 * and 086142CC to `Proc_StartBlocking(script, proc)` in sub_08074A28, which then
 * writes +0x2a/+0x2c/+0x2e/+0x30 and +0x38 of the child -- the same field group
 * its 086142E4 sibling uses. */
extern const struct ProcCmd gUnknown_08614268[];
extern const struct ProcCmd gUnknown_086142CC[];
/* 08614314 -- the scroll proc. sub_08074C84 Proc_Find's it as a busy check and
 * then starts it either way (Proc_StartBlocking under its own first argument if
 * that is non-null, Proc_Start under PROC_TREE_3 otherwise), filling +0x2c..
 * +0x36 of the child with the target and the current gUnknown_0202FDFC origin.
 * sub_08074B60 is its body. */
extern const struct ProcCmd gUnknown_08614314[];
/* An EWRAM byte table sub_08074B60 indexes with its proc's +0x3c countdown and
 * subtracts from the +0x3a accumulator. Signed: the ROM uses a signed byte
 * load, where a `u8 []` would need an explicit cast at the one use. */
extern s8 gUnknown_0202FEF8[];
/* sub_08074F2C hands the address straight to PutSpriteExt's `u16 *` fourth
 * parameter with no dereference, so this is real sprite data at that address --
 * unlike its 0x081CC4D8 / 0x081CC4DC / 0x081CC4E0 neighbours, which are
 * compiler-made pool words holding &gUnknown_0202FDFC. The element type is not
 * proved: nothing reads through it yet. */
extern u16 gUnknown_081CC4E8[];
/* The 0x081CC4F0 neighbour of the blob above, on the same evidence:
 * sub_0807519C, sub_08075248 and sub_080750C0 each hand the address to
 * PutSprite / PutSpriteExt's `u16 *` fourth parameter with no dereference. Not
 * const, because that parameter is not. */
extern u16 gUnknown_081CC4F0[];
/* A four-entry signed byte table of sprite y-jitter: both sub_08075008 and
 * sub_08075248 index it with `(counter >> 3) & 3` and add the result to the
 * +0x2e / +0x36 y coordinate. The add is signed, so the table dips the sprite
 * both ways. */
extern s8 gUnknown_0861436C[];
/* The script behind sub_08075058, the per-entry child spawner of the
 * gUnknown_08614390 menu proc: sub_0807519C calls sub_08075058 once per
 * frame-group and stores the returned proc into the ten-entry pointer table at
 * +0x3c..+0x60 of its own proc, the same table sub_080752D8 walks. The starter
 * writes the +0x2a/+0x2c/+0x2e/+0x30 halfwords and zeroes the +0x34 word, and
 * sub_080750C0 is its body: it reads +0x34..+0x3a and Proc_Breaks. */
extern const struct ProcCmd gUnknown_08614370[];
/* More proc scripts, from the 0806Exxx / 08073xxx / 08074xxx starters:
 *   08582BB4  Proc_StartBlocking, sub_0806E6C8 (+0x5c = arg)
 *   08582BE4  Proc_Start,         sub_0806E728 (+0x54 = the parent it was
 *                                 started under, so the field is a ProcPtr)
 *   08582C24  Proc_Start,         sub_0806E8C8 (+0x58/+0x5c = 0, +0x60 = arg)
 *   086141B4  Proc_Start(.., PROC_TREE_VSYNC), sub_08073900 (+0x5c = arg)
 *   086142E4  Proc_StartBlocking, sub_08074AAC (+0x2c = arg, +0x30 = 0)
 *
 * And the same family again:
 *   08582B2C  Proc_Start,         sub_0806E5CC (+0x34 u16 = arg, +0x2c = 0x140,
 *                                 +0x30 = 0xa0 -- 320x160, so the pair is a
 *                                 screen-sized extent rather than two flags)
 *   08582B74  Proc_StartBlocking, sub_0806E698, started under the starter's own
 *                                 first argument, and +0x5c of the new proc is
 *                                 copied from +0x5c of that parent -- so the
 *                                 parent is a proc of this same shape
 *   08582B94  Proc_StartBlocking, sub_0806E6B4 AND sub_0806E6E0, two identical
 *                                 20-byte starters on one script, neither
 *                                 writing a field
 *   08582D74  Proc_StartBlocking, sub_0806F710 (no fields written). This one
 *                                 sits immediately above the m4a/MP2K span, but
 *                                 it takes its parent in r0 like every other
 *                                 member of this family, so it is ordinary
 *                                 compiler output and not m4a_asm.s.
 *   0861418C  Proc_Start(.., PROC_TREE_VSYNC), sub_080736C4 (no fields written)
 *   086141DC  Proc_Start(.., PROC_TREE_VSYNC), sub_08073C88 (+0x5c = arg)
 */
extern const struct ProcCmd gUnknown_08582B2C[];
extern const struct ProcCmd gUnknown_08582B74[];
extern const struct ProcCmd gUnknown_08582B94[];
extern const struct ProcCmd gUnknown_08582BB4[];
extern const struct ProcCmd gUnknown_08582BE4[];
extern const struct ProcCmd gUnknown_08582C24[];
extern const struct ProcCmd gUnknown_08582D74[];
/* Proc scripts from the 0x080719C0-0x0807298C starter family. The +0x64 group
 * is six starters over four scripts, in two shapes that pair off: a tree-3
 * Proc_Start taking only the payload, and a Proc_StartBlocking that forwards its
 * own second parameter as the parent. Both stash one HALFWORD at +0x64.
 *   08613E08  Proc_Start tree 3,  sub_080719C0 (no fields written)
 *   08613E64  Proc_Start tree 3,  sub_08071EF0 (+0x64) and
 *             Proc_StartBlocking, sub_08071F28 (+0x64), parent forwarded
 *   08613E84  Proc_Start tree 3,  sub_08071F0C (+0x64) and
 *             Proc_StartBlocking, sub_08071F40 (+0x64), parent forwarded
 *   08613EA4  Proc_StartBlocking, sub_08071F58 (+0x64), parent forwarded
 *   08613EC4  Proc_StartBlocking, sub_08071F70 (+0x64), parent forwarded
 *   08613F0C  Proc_StartBlocking, sub_0807249C (+0x58 word), parent is that
 *             starter's FIRST parameter
 *   08613F34  Proc_Start tree 3,  sub_08072970 (+0x2c/+0x34 words)
 *   08613F44  Proc_Start tree 3,  sub_0807298C (+0x2c/+0x30/+0x34 words) */
extern const struct ProcCmd gUnknown_08613E08[];
extern const struct ProcCmd gUnknown_08613E64[];
extern const struct ProcCmd gUnknown_08613E84[];
extern const struct ProcCmd gUnknown_08613EA4[];
extern const struct ProcCmd gUnknown_08613EC4[];
/* 08613EE4  Proc_Find, sub_080723C0 (+0x4c word = 0). Unlike the rest of this
 *           group the find is NULL-checked before the store, so the poker is
 *           `p = Proc_Find(s); if (p) p->unk4c = 0;` rather than the usual
 *           unchecked cast-and-store. */
extern const struct ProcCmd gUnknown_08613EE4[];
extern const struct ProcCmd gUnknown_08613F0C[];
extern const struct ProcCmd gUnknown_08613F34[];
extern const struct ProcCmd gUnknown_08613F44[];
extern const struct ProcCmd gUnknown_0861418C[];
extern const struct ProcCmd gUnknown_086141B4[];
extern const struct ProcCmd gUnknown_086141DC[];
extern const struct ProcCmd gUnknown_086142E4[];
/* Proc scripts from the 0x08074xxx-0x08078xxx starter family:
 *   086142B4  Proc_Start,         sub_08074714 (+0x58 = 0xc00), and
 *             Proc_Find,          sub_0807472C (+0x58 = arg << 10) -- one
 *             script with a starter and a poker, so +0x58 is one u32 field
 *   08614344  Proc_Start,         sub_08074ED0 (+0x54 = arg, +0x58 = 0), and
 *             Proc_Find,          sub_08074EEC -- which walks +0x40 as an array
 *             of FIVE pointers (`ldm r3!, {r1}` five times off proc+0x40) and
 *             pokes a halfword into +0x22 of each. So this proc owns a
 *             5-element pointer table at +0x40 and runs to at least +0x54
 *   086143E0  Proc_Start,         sub_080758BC (+0x2c/+0x30/+0x38 = args,
 *                                 +0x34 = 0 as a HALFWORD, +0x3c/+0x40 = 0)
 *   086144FC  Proc_Start,         sub_08076770 (+0x2c/+0x30/+0x58 = args,
 *                                 +0x5c = 0, +0x64 = 0 halfword), and
 *             Proc_Find,          sub_080767A8 (+0x64 = 1 halfword)
 *   0861485C  Proc_Start(.., PROC_TREE_3), sub_0807813C (no fields written)
 *   08614894  Proc_Start(.., PROC_TREE_3), sub_080780D0 (no fields written)
 *   0861598C  Proc_StartBlocking, sub_08078480 (+0x54 = arg) -- same shape as
 *                                 08615AAC / sub_08078540
 *   08615A8C  Proc_StartBlocking, sub_080784E4 (+0x58/+0x5c/+0x60 = args)
 */
extern const struct ProcCmd gUnknown_086142B4[];
extern const struct ProcCmd gUnknown_08614344[];
extern const struct ProcCmd gUnknown_086143E0[];
extern const struct ProcCmd gUnknown_086144FC[];
extern const struct ProcCmd gUnknown_0861485C[];
extern const struct ProcCmd gUnknown_08614894[];
extern const struct ProcCmd gUnknown_0861598C[];
extern const struct ProcCmd gUnknown_08615A8C[];
/* The scripts named by the second half of the 16-byte proc-forwarder family
 * (family F000 in data/families.json). Each is `const struct ProcCmd []`
 * because its consumers are proc.h prototypes that take exactly that, and
 * `const` because they are ROM data nothing writes. One line per script -- in
 * every case a Proc_EndEach forwarder from that family plus at least one starter
 * or poker that pins the object as a proc script rather than a bare blob:
 *   08580F24  sub_0806A444 -> sub_08067504, which is Proc_BreakEach open-coded
 *             over sProcArray. Sole reference in the ROM, so nothing here
 *             constrains the proc's fields.
 *   085810B8  Proc_Start,   sub_08067D04 (+0x34/+0x38 = args, +0x3c = 0);
 *             Proc_EndEach, sub_08067D4C
 *   08613E54  Proc_Start,   sub_08071B28 (+0x2c = a gUnknown_0202F2DC record
 *             the starter also fills);  Proc_EndEach, sub_08071B88
 *   08613F2C  Proc_Start,   sub_080725A8 (+0x2c word, then halfwords at
 *             +0x30/+0x32/+0x34/+0x36/+0x38/+0x3a); Proc_EndEach, sub_08072598
 *   08614220  Proc_Start,   sub_08073FF4 (+0x2c word, then 8 halfword pairs
 *             written from +0x30 and +0x40);        Proc_EndEach, sub_08074028
 *   08614390  Proc_Start,   sub_08075298 (+0x2c word, +0x34/+0x36/+0x38
 *             halfwords); Proc_Find, sub_080752D8, which pokes +0x38 and then
 *             walks a TEN-entry pointer table at +0x3c..+0x60 poking +0x30 of
 *             each non-NULL entry;                  Proc_EndEach, sub_08075304
 *   086143B8  Proc_Start,   sub_0807548C (+0x2a..+0x34 halfwords, +0x38/+0x3c
 *             words); Proc_Find, sub_0807553C (same fields);
 *             Proc_EndEach, sub_080755E0
 *   08615CA0  Proc_Start,   sub_08078D80 under the caller's own parent, no
 *             fields written;                       Proc_EndEach, sub_08078E04
 *   08616638  Proc_Start,   sub_0807C978 under the caller's own parent, no
 *             fields written;                       Proc_EndEach, sub_0807F8C0
 *   08616DB4  Proc_Find,    sub_08087974 and sub_08087C14 (+0x54 word);
 *             Proc_Start,   sub_08087974;           Proc_EndEach, sub_080879A0
 */
extern const struct ProcCmd gUnknown_08580F24[];
extern const struct ProcCmd gUnknown_085810B8[];
/* The gUnknown_08613E54 proc's per-slot record, one per palette bank:
 * sub_08071B28 allocates &gUnknown_0202F2DC[index], copies 0x10 halfwords of
 * &gPal[index * 16] into its head -- which is what fixes both the 0x20-byte
 * snapshot and the 0x30 stride -- and then fills the five fields behind it.
 * unk20 is the source palette the caller supplied, unk24 the gPal slice to fade
 * towards it, unk28 a step counter started at 0, and unk2a/unk2c a duration and
 * that duration plus one. */
struct Unk0202F2DC /* 0x30 */
{
    /* 0x00 */ u16 unk00[0x10];
    /* 0x20 */ const void *unk20;
    /* 0x24 */ u16 *unk24;
    /* 0x28 */ u16 unk28;
    /* 0x2a */ u16 unk2a;
    /* 0x2c */ u16 unk2c;
    /* 0x2e */ u8 filler_2e[0x02];
};
extern struct Unk0202F2DC gUnknown_0202F2DC[];
/* A small ROM step table indexed by the debug proc's own 0..4 cursor (+0x5c):
 * sub_080719EC adds gUnknown_08613E48[unk5c] to the value at +0x58 on Up and
 * subtracts it on Down, so the entries are the per-digit increments of a decimal
 * editor whose value is clamped to 0x1869f (999999). */
extern const u16 gUnknown_08613E48[];
extern const struct ProcCmd gUnknown_08613E54[];
extern const struct ProcCmd gUnknown_08613F2C[];
/* 08614014  Proc_StartBlocking, sub_080729AC under the starter's own only
 *           parameter, stashing sub_08034F6C()'s result at +0x64 (s16);
 *           sub_080729CC is the matching wait, breaking the frame
 *           sub_08034F6C() comes back equal to that snapshot. */
extern const struct ProcCmd gUnknown_08614014[];
extern const struct ProcCmd gUnknown_08614220[];
extern const struct ProcCmd gUnknown_08614390[];
extern const struct ProcCmd gUnknown_086143B8[];
extern const struct ProcCmd gUnknown_08615CA0[];
extern const struct ProcCmd gUnknown_08616638[];
extern const struct ProcCmd gUnknown_08616DB4[];
/* NOT proc scripts, despite sitting inside the same 0x0858xxxx table as the ones
 * above: these two are gUnknown_03001470-list blobs, the same slot as
 * gUnknown_0849A0F0 and gUnknown_0849A3C0. Each has an install/remove pair --
 * sub_080656E0 is `sub_080152EC(08580C7C, 3)` and sub_08065700 is
 * `sub_0806377C(08580C7C)`; sub_0806D820 / sub_0806D840 are the same two lines
 * over 08581F40. Nothing indexes or dereferences either, and both consumers take
 * them as an opaque `const void *`; widen when sub_080152EC's slot payload is
 * settled. Address proximity to the proc scripts is therefore NOT evidence of
 * what a 0x0858xxxx symbol is -- read the consumer, not the address. */
extern const u8 gUnknown_08580C7C[];
extern const u8 gUnknown_08581F40[];
/* Three more gUnknown_03001470-list blobs of exactly the same kind, one per
 * reader -- sub_08064B68 (0858096C), sub_0806D34C (08581E94), sub_0806D620
 * (08581ECC). Each reader is `if (sub_08015BD0((s32)blob) != -1) { loop;
 * sub_0806377C(blob); }`, i.e. the same two consumers as the pair above.
 *
 * The addresses are not in the readers' own literal pools. Each reader loads a
 * word out of agbcc's own `-fforce-addr` pool of address constants -- reading
 * the blob twice across the loop is the trigger -- and the ROM words 0x0816E0F4,
 * 0x0816E194 and 0x0816E198 hold 0x0858096C, 0x08581E94 and 0x08581ECC. Name
 * the BLOB: declaring the 0x0816Exxx word itself as a `const u8 *` global adds a
 * third load at both use sites, because agbcc then force-addrs that global in
 * turn. This is the same object the gUnknown_0816E1B8 comment describes,
 * reached from the other end. */
extern const u8 gUnknown_0858096C[];
extern const u8 gUnknown_08581E94[];
extern const u8 gUnknown_08581ECC[];
/* The four parallel seven-entry ROM tables sub_0806D268 walks as it builds the
 * seven gUnknown_08580934->unk54[] objects. The extents are the loop's seven
 * passes (i <= 6) and, for each table, the address of the next one:
 *   gUnknown_0816E0D0[7]  u16, 0x0050 0x0058 0x0060 0x0068 0x0070 0x0078 0x0122
 *   gUnknown_0816E0DE[7]  u16, 0x0000 0x0010 0x0020 0x0030 0x0030 0x0040 0x0112
 *   gUnknown_085809A4[7]  u16, 2 4 0x12 0x60 0x64 2 4 -- read as a halfword and
 *                         stored as a byte into Unk08580934_Obj.unk4b, so the
 *                         u16 is the table's own width and the truncation is at
 *                         the use
 *   gUnknown_085809D0[7]  u8, all 1
 * gUnknown_085809B4 is the seven handlers those objects run: 08064739 08064775
 * 080647BD 0806486D 08064919 08064739 080649D1, all odd, so THUMB entry points
 * with the T bit set, and they land in Unk08580934_Obj.unk4c, which
 * sub_080645AC already types as `void (*)(struct Unk08580934_Obj *)`. Elements 0
 * and 5 are the SAME handler, which a table of distinct globals would not be.
 * gUnknown_085809B4 and gUnknown_085809D0 are reached through agbcc's own
 * `-fforce-addr` words at 0x0816E18C and 0x0816E190; naming the tables is what
 * reproduces that. The other three are reached by a plain pool-word load. */
extern const u16 gUnknown_0816E0D0[7];
extern const u16 gUnknown_0816E0DE[7];
extern const u16 gUnknown_085809A4[7];
extern void (*const gUnknown_085809B4[7])(struct Unk08580934_Obj *);
extern const u8 gUnknown_085809D0[7];
/* Two 0xFF-terminated u8 id lists handed to sub_08074AAC by sub_08078440 and
 * sub_08078454. The ROM holds {8, 9, 0xa, 0xff} and {0x10, 0x11, 0x12, 0xff}.
 *
 * They are TWO symbols and not one array, because of sub_08078358, the third
 * caller of sub_08074AAC: it walks a run of 4-byte records based at
 * gUnknown_08615974 (i = 0..3, so 0x08615974/78/7c/80) and passes
 * `&record[i][2]` -- the last two bytes of each record, which are also a
 * 0xFF-terminated list. So the whole 0x08615974-0x0861598B block is a mixture of
 * two unrelated things at 4-byte granularity, and the 0x84/0x88 pair is reached
 * only by two separate wrappers naming two separate addresses. */
extern const u8 gUnknown_08615984[];
extern const u8 gUnknown_08615988[];
/* A gUnknown_0200C528 list script, exactly the gUnknown_0849A8F0 shape: its sole
 * reference in the ROM is sub_08078958 handing its address to
 * sub_080193B0(const u8 *). Nothing indexes or dereferences it. */
extern const u8 gUnknown_08615B4C[];

/* The start/exists proc-script family, scattered across code-0806CFC8.s from
 * 0x08078150 to 0x0808AA88. Each script below gets exactly two wrappers, and
 * where both exist they are ADJACENT at 0x14 spacing -- a 20-byte starter
 *     void f(void)  { Proc_Start(script, PROC_TREE_3); }
 * followed immediately by a 24-byte existence predicate
 *     int  g(void)  { return Proc_Find(script) != 0; }
 * Ten such pairs plus three predicates whose starters sit up at 0x080780D0 /
 * 0x0807813C. All thirteen scripts take PROC_TREE_3 and neither wrapper reads
 * or writes a single proc field, so these say nothing about the proc layout --
 * unlike the 086142B4 family above, they constrain no struct offsets. All 23
 * wrappers are uniform: no per-member variation in tree number, shift kind or
 * masking.
 *
 *   086147FC  Proc_Find only, sub_08078150   (starter not in this file)
 *   08615CB0  sub_0807A970 / sub_0807A984
 *   08615FB4  sub_0807B790 / sub_0807B7A4
 *   086164A0  sub_0807C55C / sub_0807C570
 *   086165C0  sub_0807F524 / sub_0807F538
 *   086166C8  sub_0807F550 / sub_0807F564
 *   08616990  sub_080846C8 / sub_080846DC
 *   08616B74  sub_08085AC8 / sub_08085ADC
 *   08616C54  sub_08087858 / sub_0808786C
 *   08616DFC  sub_0808A638 / sub_0808A64C
 *   08616FD4  sub_0808AA74 / sub_0808AA88
 * and reusing two scripts already declared above:
 *   0861485C  sub_0807813C  / sub_08078168
 *   08614894  sub_080780D0  / sub_08078180
 *
 * The PREDICATE half of the family is wider than the starter half, and the two
 * do not have to travel together. Three more 24-byte predicates of exactly the
 * shape above sit on scripts whose starters are ordinary payload-writing
 * wrappers rather than the 20-byte PROC_TREE_3 form, and they are nowhere near
 * 0x08078150:
 *   086141B4  sub_08073918  (starter sub_08073900, PROC_TREE_VSYNC, +0x5c = arg)
 *   086141DC  sub_08073CA0  (starter sub_08073C88, PROC_TREE_VSYNC, +0x5c = arg)
 *   08616710  sub_0807F618  (Proc_StartBlocking'd by sub_0807F82C)
 * Each is the immediate neighbour of its own starter, at the same 0x14/0x18
 * spacing. So when scoping a batch: a 24-byte leaf that is `bl Proc_Find`
 * followed by cmp/beq/movs is a member of this family whatever its address, and
 * `return Proc_Find(script) != 0;` matches it first try.
 */
extern const struct ProcCmd gUnknown_086147FC[];
extern const struct ProcCmd gUnknown_08615CB0[];
extern const struct ProcCmd gUnknown_08615FB4[];
extern const struct ProcCmd gUnknown_086164A0[];
extern const struct ProcCmd gUnknown_086165C0[];
extern const struct ProcCmd gUnknown_086166C8[];
extern const struct ProcCmd gUnknown_08616990[];
extern const struct ProcCmd gUnknown_08616B74[];
extern const struct ProcCmd gUnknown_08616C54[];
extern const struct ProcCmd gUnknown_08616DFC[];
extern const struct ProcCmd gUnknown_08616FD4[];

/* gUnknown_03001470-list script blobs, all reached ONLY as an address handed to
 * sub_080152EC (`const void *`) or sub_080152C0, exactly the slot
 * gUnknown_0849A0F0 / gUnknown_0849A3C0 / gUnknown_0849A108 occupy. Nothing
 * indexes or dereferences any of them, and both consumers take an opaque
 * pointer, so neither `const` spelling risks a warning. Extents come from
 * data/data*.s, where the split drew the symbol boundaries, so they are upper
 * bounds on each record and not proved sizes:
 *   08485D9C 0x18   08485DB4 0x10   08499E4C 0x98   08499EE4 0x68
 *   0849A128 0x70   0849A1C0 0x30   0849D10C 0x60   0849E610 0x38
 *   0849E648 0x28   0849F628 0x30   084C3128 0x10
 * Which consumer each one has, since the two are NOT interchangeable in the
 * source (sub_080152C0's first parameter is declared s32 and needs a cast):
 *   sub_080152EC  08485D9C 08485DB4 08499E4C 08499EE4 0849E610 0849E648 0849F628
 *   sub_080152C0  0849A128 0849A1C0 0849D10C 084C3128 (and 0849A108, 0849E6D4)
 */
/* One more of the same species, one record earlier: sub_08003910 is
 * `sub_080152EC(gUnknown_08485D8C, 0)` followed by
 * `gActiveMap->introScreenY = 0xFFF6`. */
extern const u8 gUnknown_08485D8C[];
/* sub_08002AB0 and sub_08002C38 both pass it as sub_0801BD00's third argument,
 * which is `void *`; nothing indexes it, so `const u8 []` plus the cast at the
 * call site is all the evidence supports. It lives in data/data.s, not
 * rodata. */
extern const u8 gUnknown_08485B52[];
extern const u8 gUnknown_08485D9C[];
extern const u8 gUnknown_08485DB4[];
extern const u8 gUnknown_08499E4C[];
extern const u8 gUnknown_08499EE4[];
extern const u8 gUnknown_0849A128[];
extern const u8 gUnknown_0849A1C0[];
extern const u8 gUnknown_0849D10C[];
extern const u8 gUnknown_0849E610[];
extern const u8 gUnknown_0849E648[];
extern const u8 gUnknown_0849F628[];
extern const u8 gUnknown_084C3128[];
/* Two more of the same blobs, but reached from the REMOVER side: sub_0803A59C
 * and sub_08049FF4 stop each of these pairs with two back-to-back
 * sub_0801537C(const void *) calls. 0849E240 also appears at sub_08014C94 and
 * three times around sub_0803A94C, and 084C3814 again at sub_0804A09C, so both
 * have starters elsewhere in the ROM. Same `const u8 []` model; 0849E240 and
 * 0849E280 are 0x40 each, 084C3814 is 0x10 and sits immediately below the
 * already-declared gUnknown_084C3824. */
extern const u8 gUnknown_0849E240[];
extern const u8 gUnknown_0849E280[];
/* The script sub_0803A65C starts, the sibling of gUnknown_0849E280 above. */
extern const u8 gUnknown_0849E2C0[];
/* The 0x0803Axxx "flag control" debug screen's tables.
 *
 * gUnknown_0849E2C8 is another gUnknown_03001470 script blob (sub_080152EC).
 *
 * gUnknown_0849E2F8 is FLAT `u16`, not an array of pairs: sub_0803AAC0 reads it
 * at `[i * 2]` and `[i * 2 + 1]`, where a `u16 [][2]` would fold the odd element
 * into a displacement the ROM does not have. The ROM holds {0, 0x49, 0x1a, 0x49,
 * 0x34, 0x49, 0x4e, 0x49, ...} -- an (x, 0x49) coordinate pair per row.
 *
 * gUnknown_0849E318 / gUnknown_0849E358 are 4-byte records read as bytes at +0
 * and +2, subscripted by the SIGNED gUnknown_0849D89C->unk09 in sub_0803A5B8.
 *
 * gUnknown_0849E398 is 0x20-byte rows whose first `u16` is the sprite id
 * sub_0803A4A8 hands to sub_08014740.
 *
 * gUnknown_0849E5F8 holds {"OFF", "ON"} -- the two words live at 0x08090F90 and
 * 0x08090F8C, immediately after this file's pool of address constants (see the
 * gUnknown_08090F74 note). */
extern const u8 gUnknown_0849E2C8[];
/* Must stay non-const: sub_0803AAC0 keeps the two element ADDRESSES in
 * callee-saved registers across its first sub_0801F34C call and reloads both
 * afterwards, where a `const u16 []` lets the compiler carry the values across
 * the call instead. */
extern u16 gUnknown_0849E2F8[];
/* STRUCTS, not `u8 [][4]`: sub_0803A5B8 reads +0 and +2 off ONE scaled address,
 * which is the member-displacement form. A flat `[i][2]` folds the +2 into the
 * address constant instead and costs two extra instructions. */
struct Unk0849E318
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 filler_03[0x01];
};
extern const struct Unk0849E318 gUnknown_0849E318[];
extern const struct Unk0849E318 gUnknown_0849E358[];
/* 24 rows of 16 halfwords: the unit info-screen text ids. Column order is
 * from the 'Info Screen Editor' Nightmare module (a community ROM-editor
 * definition) -- 0 unit, 1 movement, 2 vision, 3 fuel, 4 weapon 1, 5 ammo,
 * 6 range, 7..10 weapon 1 detail 1..4, 11 weapon 2, 12..15 weapon 2 detail
 * 1..4. The row is picked by gUnknown_081BA068[..] - 1 and the column by
 * gUnknown_0849D89C->unk09, so the subscripts stay dynamic here. */
extern const u16 gUnknown_0849E398[][0x10];
extern const char *const gUnknown_0849E5F8[];
/* A table of `void *` blobs sub_0803A07C hands to sub_0801BD00 as its third
 * argument, indexed `[unit->unk00 * 15 + sub_08042DE0(...) - 1]`. */
extern void *gUnknown_0849DC18[];
/* A plain byte table sub_0803A07C subscripts with
 * gUnknown_0849D89C->unk04->unk00 and passes to sub_0801F34C as a sprite id. */
extern const u8 gUnknown_0849E224[];
/* The tileset blob sub_0803A338 / sub_0803A4A8 hand to sub_08071948 with the
 * size 0x8360 as the fifth argument. */
extern const u8 gUnknown_080D4228[];
/* SIGNED: sub_0803A4A8 loads a byte and immediately sign-extends it before
 * subtracting 1, and the ROM holds 0xFF sentinels between the runs
 * (05 02 01 FF 07 03 04 FF ...), so -1 is a real value in the table. */
extern const s8 gUnknown_081BA068[];
/* The descriptor sub_0803CE28 hands to sub_0803CFA4 by ADDRESS, with no
 * dereference, so it is data and not a pool word. */
extern const u8 gUnknown_0809113C[];
/* The two string literals inside the 0x08090F74 pool block, declared as objects
 * rather than written as `"FLAG CONTROL"` / `"FLAG"` in the C. Both spellings
 * emit the identical .rodata, but the literal one costs 2 bytes: agbcc then
 * relocates the pool words against the SECTION with addends 0x00/0x10, where the
 * ROM relocates against a symbol at each string with addend 0. */
extern const char gUnknown_08090F94[];
extern const char gUnknown_08090FA4[];
/* AUDITED POOL WORDS -- DO NOT DECLARE AS OBJECTS. 0x08090F74..0x08090FAB is
 * agbcc's own `.rodata` pool of address constants for the 0x0803Axxx debug
 * screen, and the ROM shows it word by word:
 *
 *   0x08090F74 -> 0x0849D89C   &gUnknown_0849D89C
 *   0x08090F78 -> 0x03002EE0   &gpKeySt
 *   0x08090F7C -> 0x0849D89C   &gUnknown_0849D89C   (a second private copy)
 *   0x08090F80 -> 0x08499578   &gUnknown_08499578
 *   0x08090F84 -> 0x08499590   &gUnknown_08499590
 *   0x08090F88 -> 0x0849D89C   &gUnknown_0849D89C   (a third private copy)
 *   0x08090F8C -> "ON"         0x08090F90 -> "OFF"
 *   0x08090F94 -> "FLAG CONTROL"                    0x08090FA4 -> "FLAG"
 *   0x08090FAC -> 0x03002EE0   &gpKeySt
 *
 * The same address appearing at three scattered words is the tell. A
 * `ldr =sym` followed by a load through it is NOT, because a real ROM pointer
 * variable emits the identical two loads -- gUnknown_0849D89C itself, one level
 * further in, is exactly such a variable. The honest spelling for every one of
 * these is the named object, and the string words are ordinary string
 * literals. */
/* The pool block above starts one word LOWER than first recorded. In the ROM:
 *   0x08090F70 -> 0x0849D89C   &gUnknown_0849D89C   (a FOURTH private copy)
 * and 0x08090F6C -> 0x030043F8, so the block is 0x08090F6C..0x08090FAB.
 * sub_0803A190 reaches the record ONLY through 0x08090F70, so the honest
 * spelling there is `gUnknown_0849D89C->unk08`; do NOT declare
 * gUnknown_08090F70. */

/* The 0x0803Axxx "BACKUP UTL" / "EDIT" debug screens. Every word below was
 * dereferenced in the ROM before being declared.
 *
 * REAL OBJECTS (declared here):
 *   0x08090FB0  9 `const char *` -- sub_0803AD48's row labels. INDEXED by a
 *               computed subscript BEFORE the dereference, which is the inverse
 *               test for a real table. A tenth word would be 0x4c462020
 *               ("  FL"), i.e. string data, so it is exactly 9 -- and 9 is also
 *               how many passes both loops that walk it make.
 *   0x08091038  9 u16 -- per-row maxima (1,1,1,1,0x270f,0x270f,1,1,1)
 *   0x0809104A  9 u16 -- per-row steps  (1,1,1,1,0x64,  0x64,  1,1,1)
 *   0x0809105C  "EDIT"      0x08091064  "//"      0x080910D4  "BACKUP UTL"
 *   0x0809106C  3 `const char *` -> the "  FLASH..." strings at 0x080910B8 /
 *               0x08091098 / 0x08091078 (the table is in reverse address order)
 *
 * POOL WORDS -- NOT declared, they hold another ADDRESS:
 *   0x08091068 -> 0x03002EE0   &gpKeySt              (sub_0803AD48)
 *   0x080910E4 -> 0x03002EE0   &gpKeySt              (sub_0803AFA0)
 * 0x080910E0 is NOT a pool word, whatever it looks like; see its own note below.
 * Declaring a gUnknown_03002EE0 for the &gpKeySt pair would not link --
 * aw2bhr.lds already binds 0x002EE0 as gpKeySt. Write `gpKeySt->held` and let
 * agbcc emit the word into the unit's own .rodata. */
extern const char *const gUnknown_08090FB0[];
extern const u16 gUnknown_08091038[];
extern const u16 gUnknown_0809104A[];
extern const char gUnknown_0809105C[];
extern const char gUnknown_08091064[];
extern const char *const gUnknown_0809106C[];
/* A REAL POINTER VARIABLE, not a compiler-made address constant. Its value is
 * 0x0809106C, so its content alone cannot tell the two apart. What settles it is
 * that sub_0803AFA0 RELOADS it on every pass of a loop containing a call: an
 * address constant is loop-invariant and would be hoisted, where a non-const
 * global pointer must be re-read across a call. Reach it as
 * `tbl = &gUnknown_080910E0; (*tbl)[i]`; a bare `gUnknown_080910E0[i]` emits a
 * second address word and one indirection too many. Must NOT be const-qualified:
 * the reload is the whole point, and const would license the hoist. */
extern const char **gUnknown_080910E0;
extern const char gUnknown_080910D4[];
/* A REAL ROM POINTER WORD, not a compiler-made address constant: it holds
 * 0x0200FC50, and THREE separate functions (sub_0803A65C, sub_0803A9C8,
 * sub_0803AA78) reach this same symbol, where `-fforce-addr` would give each
 * function its own private copy. Every use loads the pointer and then the
 * record, and the reloads fall out of ordinary aliasing -- sub_0803A9C8 re-reads
 * it after its own byte store through the pointer, sub_0803AA78 does not re-read
 * across a plain word store.
 *
 * The record itself: unk00 is a mode byte (sub_0803A9C8 stores 0x80,
 * sub_0803A190 / sub_0803A2BC consume it), unk02/unk03 are the x/y sub_0803A65C
 * hands to sub_0801A444, unk04 is the live struct Unit, unk08 takes the low byte
 * of gUnknown_030033EC, and unk09/unk0a are a pair sub_0803A65C resets to 0 and
 * 0xff. Nothing bounds the record past +0x0a. */
struct Unk0849D89C
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ struct Unit *unk04;
    /* 0x08 */ u8 unk08;
    /* 0x09 */ s8 unk09;  /* SIGNED: sub_0803A5B8 reads it with a
                           * signed byte load, which nothing but a
                           * declared `s8` produces. It agrees with the
                           * note below: the sub_0803A65C reset of
                           * unk09/unk0a is 0 / -1. */
    /* 0x0a */ u8 unk0a;  /* The evidence says this is SIGNED, but it is
                           * left as u8 on purpose: retyping a member a
                           * dozen promoted files share is not worth it.
                           * sub_0803A69C compares it with unk09 and
                           * sign-extends BOTH sides, which a declared u8
                           * cannot produce on the left. c_0803A69C.c
                           * therefore spells its own access
                           * `(s8)gUnknown_0849D89C->unk0a`, which is
                           * byte-identical to a declared s8 and costs
                           * every other reader nothing. Before retyping
                           * the member itself, the only writer to check
                           * is c_0803A65C.c's `unk0a = 0xff`, which is
                           * byte-identical either way. */
};
extern struct Unk0849D89C *gUnknown_0849D89C;
/* Byte scripts, one per value of gUnknown_0849D89C->unk04->unk00.
 * sub_0803AB3C walks the selected script one byte at a time: 0x06 ends a row
 * and 0x07 ends the script. */
extern u8 *gUnknown_084997C8[];
/* The unit sub_0803A9C8 and sub_0803AA78 set up with sub_08025BE0 and then
 * store in gUnknown_0849D89C->unk04. */
extern struct Unit gUnknown_03004100;
extern const u8 gUnknown_084C3814[];
extern const u8 gUnknown_084C383C[];
/* Four parallel byte tables of 0x5b entries each, laid out back to back.
 * sub_0804A18C picks one of the four from gUnknown_030044E0->unk5c and ->unk66
 * and indexes it with a value sub_0804A64C builds as `unk20 * 15 + unk1e`. */
extern const u8 gUnknown_084C3BA6[];
extern const u8 gUnknown_084C3C01[];
extern const u8 gUnknown_084C3C5C[];
/* Graphics sources for sub_080149C0's fourth argument: five of them per value
 * of gUnknown_030044E0->unk66 bit 0. Must stay one-dimensional and be indexed
 * `[unk67 + (unk66 & 1) * 5]`; a `[][5]` declaration does not reproduce the
 * original code. */
extern u8 *const gUnknown_084C3B3C[];
/* One (x, y) offset pair per value of gUnknown_03003F40, stored flat: the pair
 * for g is at [g * 2] and [g * 2 + 1]. sub_080428F0 adds them to
 * gUnknown_03003100. */
extern const s16 gUnknown_0849FFF8[];
/* Two compressed tile sources for the same destination at 0x060103E0, chosen
 * by gUnknown_030044E0->unk66 bit 0. Must stay non-const: Decompress takes a
 * plain `u8 *`. */
extern u8 gUnknown_0813593C[];
extern u8 gUnknown_081358A0[];
extern const u8 gUnknown_084C3CB7[];
/* 0x0812A2A8 is not a global. It is a compiler-made pool word holding the
 * address of the u16 gUnknown_030030A0; C code names that global, never the
 * word. */
/* Proc scripts of 3 or 4 entries each. gUnknown_085815D0 and
 * gUnknown_085815E8 are started by sub_0806A7AC and sub_0806A938 and are ended
 * together by sub_0806AA64. gUnknown_08614134, gUnknown_0861418C and
 * gUnknown_08614200 are each ended by their own wrapper, which then
 * re-registers sub_080735B0. */
extern const struct ProcCmd gUnknown_085815D0[];
extern const struct ProcCmd gUnknown_085815E8[];
extern const struct ProcCmd gUnknown_08614134[];
extern const struct ProcCmd gUnknown_08614200[];

/* Two parallel 11-entry word tables, indexed by sub_0800272C's first argument.
 *   gUnknown_08485C9C  OBJ tile values. sub_0800272C masks each word with
 *                      0x3FF and scales it by 32 to reach OBJ VRAM at
 *                      0x06010000, so a word carries more than the tile
 *                      number.
 *   gUnknown_08485CC8  pointers, each handed straight to sub_0801BD00's
 *                      `void *` third parameter. The element type must stay
 *                      `void *const` and not `const void *`, or that call no
 *                      longer compiles. */
/* Must stay non-const, although the table lives in ROM and is never written:
 * sub_08003640 reads gUnknown_08485C9C[i + 1] on both sides of a call and the
 * original code loads it twice. On a const array the compiler keeps the first
 * value and emits one load. */
extern u32 gUnknown_08485C9C[];
extern void *const gUnknown_08485CC8[];
/* sub_08001D04's lookup table: key/value byte pairs, walked two bytes at a
 * time and terminated by a key of 0xFF. Flat bytes, not an array of records. */
extern const u8 gUnknown_084859E0[];
/* One byte per map cell, walked in order by sub_08004724's second pass. Every
 * element is below 4 and selects one of four halfwords the function has copied
 * to its stack. */
extern const u8 gUnknown_08486FC4[];
/* The same pair of tables one record further along, read by sub_08002844 the
 * same way: nine OBJ tile values and nine pointers. Each pointer goes straight
 * to sub_0801BD00's `void *` third parameter, so the element type must stay
 * `void *const` and not `const void *`. */
extern const u32 gUnknown_08485D20[];
extern void *const gUnknown_08485D44[];
/* One 16-colour OBJ palette, loaded into slot 30 by sub_08002844. Must stay a
 * non-const `u16 []`: ApplyPaletteExt takes a plain `u16 *`. */
extern u16 gUnknown_081268D8[];

/* 0x0808D6DC is not a global. It is one of the compiler-made address words
 * that follow the original code, and it holds the address of gActiveMap; the
 * original source of sub_08000694 simply said `gActiveMap->state`. See the
 * `-fforce-addr` section of docs/agbcc-codegen.md.
 *
 * The declaration below is kept only so that the next reader does not work
 * this out again. Naming the word instead of the global does reproduce how the
 * original code caches the address, but the compiler then loads an address for
 * this symbol in turn and the function comes out longer than the original.
 * Write `gActiveMap` directly; do not add users of this symbol. */
extern struct ActiveMap **const gUnknown_0808D6DC;

/* Four ROM blobs sub_08023360 loads, each typed from the function that
 * consumes it: Decompress, sub_08012C58 or ApplyPaletteExt. None of them may
 * be made const, because none of those three takes a const pointer.
 * gUnknown_0809175C is read at +0 and at +0xa0, which is why it is an array. */
/* Five-byte records, selected by struct Unk03001470's unk20. Must stay a flat
 * byte array indexed `[i * 5 + k]`: agbcc rounds every struct size up to a
 * multiple of 4, so a five-byte record would stride by 8 instead of 5.
 * Bytes +1 and +2 are read as signed, with a cast at the use site; +3 and +4
 * are only ever copied byte for byte. */
extern const u8 gUnknown_0849A06C[];
/* Four records of three halfwords, read by sub_08029D3C as `[i * 3 + k]`. Must
 * stay a flat halfword array and not an array of records: agbcc rounds a
 * three-halfword record up to 8 bytes and would stride by 8 instead of 6.
 * Halfwords +1 and +2 are added to gUnknown_03003100.pos.unk00 and .unk02 and
 * passed to sub_0804209C; +0 is read with a signed cast. Whether the stored
 * values are signed is unknown. */
extern const u16 gUnknown_0849A0D8[];
/* Two OAM sprite blobs for PutSprite / PutSpriteExt's `u16 *` fourth
 * parameter. gUnknown_0849B6D6 is aligned to a halfword but not to a word, so
 * a halfword is the widest element it can have. */
extern u16 gUnknown_0849B6C8[];
extern u16 gUnknown_0849B6D6[];
/* Two more OAM sprite blobs for PutSpriteExt's `u16 *` fourth parameter, used
 * as a pair by sub_08076494 and sub_0807662C. Halfword 0 is the number of
 * objects (3 and 1), followed by that many (y, x, tile) triples. */
extern u16 gUnknown_086144C0[];
extern u16 gUnknown_086144D4[];
/* Two parallel pointer tables; sub_0802B91C picks one and indexes it with its
 * fourth argument minus 1. Each element goes straight to sub_0801BD00's
 * `void *` third parameter, so the element type must stay `void *const`. */
extern void *const gUnknown_0849A218[];
extern void *const gUnknown_0849A22C[];
/* A pair used by sub_08027DD8: gUnknown_08499E18 is the proc script it ends
 * (sub_08027B10 starts it) and gUnknown_08499D90 is a script blob it passes to
 * sub_0801537C. */
extern const struct ProcCmd gUnknown_08499E18[];
extern const u8 gUnknown_08499D90[];
/* Pointers indexed by struct Unk08580934_Obj's unk1c, the same 0..3 slot index
 * that reaches its unk44[] and unk70[]. sub_08066470, the only reader, hands
 * the element to sub_0801BD00's `void *` third parameter, so the element type
 * must stay `void *const` and not `const void *`. */
extern void *const gUnknown_08580CFC[];
/* The OAM sprite blob sub_08027B68 and sub_08027CC8 hand to PutSpriteExt's
 * `u16 *` fourth parameter. Must stay non-const, like that parameter. It ends
 * 8 bytes later where gUnknown_08499E18 begins, so it holds at most 4
 * halfwords. */
extern u16 gUnknown_08499E10[];
/* The blobs sub_080339B0 loads when it sets the screen up. Each is typed from
 * the function that consumes it -- ApplyPaletteExt for the palettes,
 * Decompress and sub_08011C68 for the compressed data, sub_08073304 for the
 * EWRAM buffer -- and none may be made const, because none of those takes a
 * const pointer. gUnknown_0849BC3E is a halfword lookup table indexed by a
 * byte field of the proc; its length is unknown. */
extern u8 gUnknown_02010C50[];
extern u16 gUnknown_0809165C[];
/* A 0x8360-byte blob sub_08046030 hands to sub_08071948, the same way
 * gUnknown_0812A8C8 is used. The symbol is the blob itself, not a pointer to
 * it. Element type unknown: nothing indexes it. */
extern u8 gUnknown_08125530[];
extern u8 gUnknown_0812B49C[];
extern u8 gUnknown_0812B61C[];
extern u8 gUnknown_0812B6FC[];
extern u16 gUnknown_081320AC[];
/* sub_08031018's three blobs.
 *   gUnknown_081D3E48  a palette, copied 0x20 bytes at a time into slots
 *                      0x300, 0x320, 0x340 and 0x360.
 *   gUnknown_081D3810  compressed data, decompressed to 0x060114A0.
 *   gUnknown_0849B0A0  0x20 bytes of palette, loaded into slot 0xe0.
 * Nothing indexes any of the three; only the address is used. None may be made
 * const: the functions that take them take plain pointers. */
extern u16 gUnknown_081D3E48[];
extern u8 gUnknown_081D3810[];
extern u16 gUnknown_0849B0A0[];
extern u16 gUnknown_081D3E88[];
extern u8 gUnknown_081D8A54[];
/* Compressed tile data, decompressed to 0x06015780 by sub_08032D70 -- the
 * same destination sub_080339B0 fills from gUnknown_081D8A54. */
extern u8 gUnknown_081D3C34[];
extern u16 gUnknown_081D92B8[];
extern u8 gUnknown_08239FA4[];
extern u8 gUnknown_0823A3D4[];
extern u16 gUnknown_0849BC3E[];
extern u8 gUnknown_085802F0[];
/* The blob sub_08088044 hands to sub_08073304, filling the same slot
 * gUnknown_085802F0 fills for sub_080339B0. Only the symbol's address is
 * used. */
extern u8 gUnknown_085802C0[];
extern const struct ProcCmd gUnknown_0849BB68[];
extern const struct ProcCmd gUnknown_0849BB80[];
extern const struct ProcCmd gUnknown_0849BC50[];
extern const struct ProcCmd gUnknown_08616EFC[];
/* Two proc scripts. sub_0808844C looks gUnknown_08616EDC up with Proc_Find and
 * later starts that same script; it starts gUnknown_08616E64 in PROC_TREE_3
 * and writes a halfword to +0x64 of the proc it gets back, which describes the
 * child proc's struct and not the script. */
extern const struct ProcCmd gUnknown_08616E64[];
extern const struct ProcCmd gUnknown_08616EDC[];
/* Proc scripts, one per value of gUnknown_0849B060->unk0d. sub_08034208 loads
 * one and passes it to Proc_StartBlocking. */
extern const struct ProcCmd *const gUnknown_0849BC44[];
/* Sixteen y offsets. sub_08032734, sub_08032788, sub_080327FC and
 * sub_08032850 step through them using a proc's unk0a as the index, and turn
 * round at index 0 and index 15. Whether the values were declared signed
 * cannot be told from the code: every read would compile the same either
 * way. */
extern u16 gUnknown_0849B650[];
/* Interleaved x/y pairs for the rotating pair of sprites sub_08032340 and
 * sub_08033800 draw: pair n is at [n * 2] and [n * 2 + 1], with n a counter
 * that wraps at 32, so there are at least 64 elements. Must stay a flat array:
 * with an array of two-member records the compiler folds the y offset into the
 * load instead of computing it. */
extern const s16 gUnknown_0849B108[];
/* A four-entry bob animation: sub_08032A00 indexes it with
 * `(gGameClock >> 3) & 3` and uses the byte once as `0x58 - value` and once as
 * `value + 0xd8`. */
extern const u8 gUnknown_0849B0C0[];
/* Three sprite blobs for PutSpriteExt's `u16 *` fourth argument, selected by
 * the 0..2 cursor sub_08034130 steps; sub_08033FFC and sub_0803405C read them
 * as `gUnknown_0849BC18[proc->unk36]`. */
extern u16 *const gUnknown_0849BC18[];
/* The second row of the same family: three words between gUnknown_0849BC18
 * and gUnknown_0849BC30. sub_08033B3C reads them with a 0..2 loop counter and
 * hands each one to PutSpriteExt's `u16 *` fourth parameter. */
extern u16 *const gUnknown_0849BC24[];
/* A two-level table of sprite blobs: sub_08033EC8 and sub_08033F1C read it as
 * `gUnknown_0849BC30[proc->unk30[i]][i]` and hand the result to PutSpriteExt's
 * `u16 *` fourth parameter. */
extern u16 *const *const gUnknown_0849BC30[];
/* Three halfwords, six bytes in all, running up to gUnknown_0849BC3E.
 * Declared `u8 []` on purpose: sub_08033FFC and sub_0803405C read byte 2 and
 * pass it as part of PutSpriteExt's third argument (the OAM x and flags word),
 * and a wider element type would turn those byte loads into halfword loads of
 * the wrong offset. sub_08033F1C, which wants the halfwords, casts at its own
 * use instead. */
extern const u8 gUnknown_0849BC38[];
/* Text ids: sub_08034A7C indexes this by its second argument and uses the
 * result to index gTextTable[]. */
extern const int gUnknown_08499CCC[];
/* Holds the address of the stride-8 record array gUnknown_03003338 and
 * gUnknown_03003F20 point into; sub_0803486C copies element 0 into both. */
extern struct Unk03003338 *const gUnknown_0849FE74[];
/* 20-byte records indexed by unit id. Only the word at +0x10 is ever read,
 * and always as `unk10 * 10` (sub_08036F68, sub_080249EC and sub_0802AB70 all
 * do the same). The purpose of the other fields is unknown. */
/* The per-terrain data record. The field names come from the community
 * 'Advance Wars 2 Terrain Editor' Nightmare module, which describes 31 records
 * of 0x14 bytes starting one record after this symbol; index 0 is a dummy
 * record, as it is in the unit table at gUnknown_085D5ABC, and the terrain
 * table ends where that unit table begins. `defense` is a star count:
 * sub_08046D9C returns it multiplied by 10, i.e. as a percentage.
 * gUnknown_0849982C and gUnknown_084998A4 hold two more tables of this record
 * type.
 *
 * The module calls +0x08 a three-byte graphics field; the code reads a whole
 * word there, so it is declared as a pointer. */
struct Unk085D583C /* 0x14 */
{
    /* 0x00 */ u8 *picture;  /* sub_08046E48 hands +0x00 straight to
                            * Decompress and +0x04 straight to
                            * ApplyPaletteExt, so each member has that
                            * function's parameter type. */
    /* 0x04 */ u16 *picturePalette;
    /* 0x08 */ u16 *unk08;  /* A whole word sub_08046A84 hands
                             * straight to sub_0801C7DC's `const u16 *` first
                             * parameter. sub_08046A84 reaches this member
                             * through all three tables of this record type
                             * (gUnknown_085D583C, gUnknown_0849982C and
                             * gUnknown_084998A4). */
    /* 0x0c */ u16 nameIndex;   /* sub_08046914 uses it as a subscript
                             * into gTextTable[], the same role struct
                             * CoModeData.unk00 plays. */
    /* 0x0e */ u16 descriptionIndex; /* sub_08046D30 passes it as
                           * sub_08014668's tile argument */
    /* 0x10 */ int defense;
};
extern const struct Unk085D583C gUnknown_085D583C[];
/* An EWRAM buffer whose address is published: sub_08036F68 clears its byte 1
 * and stores the buffer's address into both slots of gUnknown_03004528. Must
 * stay an array rather than a scalar: that is what makes the byte write and the
 * address share one pool word. */
extern u8 gUnknown_02027F68[];
extern u8 *gUnknown_03004528[];
extern u16 gUnknown_03004520;
extern u8 gUnknown_0809175C[];
extern u16 gUnknown_0809163C[];
extern u8 gUnknown_080BD1EC[];
/* Four halfwords per entry -- a 2x2 tile template -- read by sub_08023A4C and
 * sub_08023BAC. Must stay two-dimensional: with a constant second subscript the
 * compiler folds the column offset into the address constant, which is what the
 * original code does and what an array of records would not do. The splitter's
 * gUnknown_080BFBCA is this table's fourth column, not a separate object. */
extern const u16 gUnknown_080BFBC4[][4];
extern u8 gUnknown_0849D16C[];
/* Three more blobs sub_0806B708 uses to set the screen up: gUnknown_0822DE80
 * is a palette loaded into OBJ palette 0, gUnknown_081918A4 is compressed data,
 * and gUnknown_0858193C is a proc script started as a child of the caller's own
 * proc. */
extern u16 gUnknown_0822DE80[];
/* sub_0807BA90's two compressed sources, each passed to Decompress by name, so
 * each takes that function's `u8 *` parameter type and cannot be made const.
 * Lengths unknown. gUnknown_0822BE1C is decompressed to the BG character base
 * ((gUnknown_030030B4.bits.chr_block << 14) + 0x06000000) and gUnknown_0822D888
 * to *gUnknown_08499580. */
extern u8 gUnknown_0822BE1C[];
extern u8 gUnknown_0822D888[];
/* Four alternative 16-colour palettes, one per arm of sub_0807B884's switch on
 * gUnknown_085C77A0[gPlaySt.unk02].unk58; each is loaded at palette word 0.
 * They sit 0x20 bytes apart, which is one palette each. gUnknown_0822DB88
 * serves two arms of the switch, which is why five cases need only four
 * palettes. Must stay non-const: ApplyPaletteExt takes a plain `u16 *`. */
extern u16 gUnknown_0822DB88[];
extern u16 gUnknown_0822DBA8[];
extern u16 gUnknown_0822DBC8[];
extern u16 gUnknown_0822DBE8[];
/* Proc scripts, both started as children of sub_0807B884's own proc:
 * gUnknown_086165B0 before its palette switch and gUnknown_08616508 as the last
 * thing it does. */
extern const struct ProcCmd gUnknown_08616508[];
extern const struct ProcCmd gUnknown_086165B0[];
extern u8 gUnknown_081918A4[];
extern const struct ProcCmd gUnknown_0858193C[];

/* The link-session descriptor sub_0802EA5C is handed as its only argument and
 * stores here. Known members: +0x00 is copied straight into gUnknown_03000564;
 * +0x04 is the halfword sub_0802EB28 complements into REG_SIODATA8; +0x06 is a
 * timeout sub_0802EA5C defaults to 10; the low two bits of +0x08 become
 * gUnknown_03000560; and sub_0802EF10 tests +0x0a before arming timer 3. */
struct Unk03003F6C /* 0x0c */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
};
extern struct Unk03003F6C *gUnknown_03003F6C;
/* The link handshake's result code, and the fifth member of the serial block
 * above. Must stay volatile: sub_0802EB28 stores a value here and then reads
 * the same address back to test it, with nothing in between. Signed, because
 * sub_0802EB28 returns it from the same expression that returns -1 elsewhere.
 * Values seen: 0xF0 (what sub_0802EA5C puts there), 0, and a 0..3 slot
 * index. */
extern volatile int gUnknown_0300055C;
/* A word flag in EWRAM cleared by sub_0802EA5C and by sub_0802EB28's case 0,
 * and tested `!= 0` by case 1 to decide whether the link is up. */
/* Must stay volatile. sub_080303C8 spins on it waiting for the link interrupt
 * (`while (gUnknown_02023894 == 0 && gUnknown_0849B018->unk04 != 2);`) and
 * needs the value re-read on every turn of the loop. Without the qualifier
 * agbcc lifts the load out of the loop and the loop can never end, so this is a
 * correctness matter and not only a code-generation one. The other volatile
 * read in the same condition does not prevent the hoist. */
extern volatile int gUnknown_02023894;
/* 0x08090D88 is not a global. It is a compiler-made address word holding the
 * address of gUnknown_030032D8, with the addresses of gUnknown_030044B0 and
 * gPlaySt beside it. The original source of sub_080345C8 said plain
 * `gUnknown_030032D8`, and every other function that touches that global names
 * it directly.
 *
 * Naming the word instead is a workaround for one thing: the honest spelling
 * gives the same instructions but puts the address word in the calling file's
 * own read-only data, not at 0x08090D88. The promoted file that uses this
 * declaration must keep reading through a local `u16 *const *`; binding the
 * address to a local is what stops the compiler adding an address word of its
 * own on top. */
extern u16 *const gUnknown_08090D88;
/* 0x08090D84 is the same thing for gUnknown_030044B0. sub_080344F0 is the one
 * function that reaches the command block through this word rather than by
 * name. Bind the address to a local, exactly as the note above requires. */
extern u8 *const gUnknown_08090D84;
/* 0x08090D7C is the same again, one word further down the run: it holds the
 * address of gUnknown_0849BC38, and sub_08033EC8 is its only referrer.
 *
 * This one is an exception to the rule below, and the reason is that the honest
 * spelling does not reproduce the original here. Plain
 * `gUnknown_0849BC38[i * 2]` keeps the address in a register for the whole loop
 * and emits no address word at all, two instructions shorter than the original.
 * Its neighbours sub_08033F1C and sub_080346FC have enough live values in their
 * loop bodies that the compiler spills an address word by itself, so there the
 * honest spelling works. Here, bind `&gUnknown_08090D7C` to a local
 * `const u8 *const *` and read `(*tbl)[...]`, as the promoted sub_080345C8
 * does for gUnknown_08090D88. */
extern const u8 *const gUnknown_08090D7C;
/* 0x08090C04 is another word in the same run and is not a global either: it
 * holds the address of gUnknown_030033E8, with the addresses of gPlaySt and
 * gUnknown_030033EC beside it. Its one referrer is sub_0802CFFC.
 *
 * No declaration is needed and none is added. Naming gUnknown_030033E8
 * directly reproduces sub_0802CFFC exactly; the promoted entry only has to
 * claim the word with `"rodata": ["0x08090C04"]`. That is the normal way to
 * handle these words, and the gUnknown_08090D88 declaration above is kept only
 * because a promoted file already uses it. Do not add more of them. */
/* 0x08090C28 and 0x08090C2C are two more words in that run and are not globals
 * either. Both hold the address of gUnknown_03003F68, one copy for sub_0802E010
 * and one for sub_0802E130, which is how one address ends up wearing two
 * invented symbol names. No declaration is needed and none is added: naming
 * gUnknown_03003F68 reproduces both functions, and one function can reach the
 * record through an address word and by name at the same time.
 * Check what the ROM word actually holds before believing any 0x08090Cxx
 * symbol name. */
/* A proc script; sub_080345C8 asks sub_08015BD0 whether an instance is live
 * with the usual `!= -1` predicate. */
extern const struct ProcCmd gUnknown_0849A00C[];
/* The blocking progress-bar proc sub_0803376C starts. Its body is
 * sub_080335CC, which is what fixes the proc record's layout: unk20 is a
 * callback, unk24 a byte cursor, unk2a the total, unk2c the amount done and
 * unk2f the percentage. */
extern const struct ProcCmd gUnknown_0849BB08[];
/* Two proc scripts, always used as a pair: sub_08033030 runs only when neither
 * has a live instance, and sub_080330C0 ends both, in that order. */
extern const struct ProcCmd gUnknown_0849B670[];
extern const struct ProcCmd gUnknown_0849B688[];
/* The EWRAM area sub_08033470 decompresses a routine into and then calls, so
 * its contents are code and the symbol is only ever used as an address.
 * gUnknown_0203BFFC is the word just below it, stamped with the signature
 * 0x485153CD that the routine checks. */
extern u32 gUnknown_0203BFFC;
extern u8 gUnknown_0203C000[];
/* The next two 0x20-byte slots after gUnknown_0849A00C, and proc scripts like
 * it: sub_0802B3AC and sub_0802B4D4 pass gUnknown_0849A02C to Proc_Find, and
 * sub_0802C4B8 and sub_0802C4D4 start them with Proc_StartBlocking. */
extern const struct ProcCmd gUnknown_0849A02C[];
extern const struct ProcCmd gUnknown_0849A04C[];
/* One record of the list a handle's +0x0c cursor walks: a delay and an
 * argument. */
struct Unk0801C210Cmd
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
};

/* The animation handle sub_0801C210 allocates and returns. sub_0801C69C fills
 * in every scalar below, sub_0801C27C draws the current frame and sub_0801C2DC
 * steps the animation on.
 *   +0x00  the animation script the handle is running. sub_0801C640 stores it,
 *          and the other entry points do nothing unless it is non-NULL.
 *   +0x04, +0x08, +0x0c  three cursors sub_0801C640 derives from that script in
 *          one go (+0x08 and +0x0c always start out equal). +0x0c walks a list
 *          of struct Unk0801C210Cmd records; +0x08 is the loop-back anchor the
 *          0xFF command restores +0x0c from.
 *   +0x10, +0x14  the two OAM pointers sub_0801C2DC resolves and sub_0801C27C
 *          hands to PutSpriteExt. A non-NULL +0x14 also means "has affine
 *          data".
 *   +0x18, +0x1a  a frame counter and a speed. sub_0801C67C zeroes +0x18 and
 *          forces +0x1a to 0x100 around one step call, then puts +0x1a back:
 *          run a single frame at a fixed speed. +0x18 is signed because
 *          sub_0801C2DC decrements it and then tests the sign.
 *   +0x1e  the OAM layer word. +0x21 is the priority nibble, shifted into the
 *          OAM x-and-flags word; +0x22 an OAM-shaped `tile | pal << 12`
 *          halfword; +0x24 the decompression buffer; +0x28 the last argument a
 *          script record published.
 *   +0x20  bit 1 chooses how the script is encoded: set means halfword offsets
 *          relative to their own address, whose low bit is a flag and is masked
 *          off; clear means a flat table of pointers. */
struct Unk0801C210
{
    /* 0x00 */ void *unk00;
    /* 0x04 */ u8 *unk04;
    /* 0x08 */ struct Unk0801C210Cmd *unk08;
    /* 0x0c */ struct Unk0801C210Cmd *unk0c;
    /* 0x10 */ u16 *unk10;
    /* 0x14 */ u16 *unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1a */ u16 unk1a;
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u16 unk1e;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ void *unk24;
    /* 0x28 */ u16 unk28;
};
/* 16-colour palettes, one per player, indexed by gPlayers[n].unk1a - 1 in
 * sub_080355CC. Must stay non-const: ApplyPaletteExt takes a plain `u16 *`.
 *
 * Two things about the use site matter, and both are needed. Keep the `[][16]`
 * shape, and work the index out in its own statement (`i = ...unk1a - 1;` then
 * `tbl[i]`); written inline, the -1 is folded into the relocation and the
 * subtraction disappears. If the original loads the table's address before the
 * record that indexes it, bind the table to a local `u16 (*tbl)[16]` as well.
 * See docs/agbcc-codegen.md. */
extern u16 gUnknown_0810EA60[][16];
/* One moving particle, 0x0c bytes. sub_080353E8 steps a table of 32 of them.
 * unk00 and unk02 are 8.8 fixed-point screen positions, unk04 and unk06 the
 * step added to them once per frame, and unk08 picks a row of
 * gUnknown_0849BDA0 in units of eight bytes. All are unsigned halfwords.
 * Nothing in this block touches +0x0a. */
struct Unk02027DE8 /* 0x0c */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u8 filler_0a[0x02];
};
extern struct Unk02027DE8 gUnknown_02027DE8[];
/* Sprite rows for sub_0801BDB4's `u16 *` third argument, four halfwords each;
 * a struct Unk02027DE8's unk08 picks one. The element type comes from that
 * already-promoted parameter, not from anything here. */
extern u16 gUnknown_0849BDA0[];
/* A second table of the same records, 32 of them, sitting immediately below
 * gUnknown_02027DE8. sub_08035224 fills it the way sub_08035354 fills the
 * other, and sub_080352B4 steps and draws it the way sub_080353E8 does. */
extern struct Unk02027DE8 gUnknown_02027C68[];
/* Sixteen groups of three halfwords, read as `[j * 3 + k]` by sub_08035224 and
 * sub_08035354, with j the entry number modulo 16. Must stay flat rather than
 * `[16][3]`: both functions work the address out again for each of the three
 * reads instead of folding k into the load, which is what the flat spelling
 * produces. Unlike the words just below it at 0x08090E30..0x08090E3C, this one
 * is real data and not a compiler-made address word. */
extern u16 gUnknown_08090E40[];
/* Two 0x60-byte blobs sub_08011C68 copies to VRAM: to 0x06012E00 for
 * sub_08035224 and to 0x06012E60 for sub_08035354. Neither is indexed, so
 * nothing here settles the element type. */
extern const u8 gUnknown_0809169C[];
extern const u8 gUnknown_080916FC[];
/* The gUnknown_02027C68 counterpart of gUnknown_0849BDA0: the sprite rows
 * sub_080352B4 passes to sub_0801BDB4. This caller passes the base unindexed
 * and hands the record's unk08 over separately as the fourth argument. */
extern u16 gUnknown_0849BD98[];
/* The proc script sub_08035850 starts on PROC_TREE_5. sub_08035760,
 * sub_08035828 and sub_080358AC drive its proc: +0x35 and +0x36 are state
 * bytes, +0x42 and +0x44 a signed position pair in sixteenths of a pixel
 * (sub_08035850 multiplies by 16 going in, sub_08035760 divides by 16 coming
 * out), and +0x4c a sixteen-byte copy buffer. */
extern const struct ProcCmd gUnknown_0849BDB8[];
/* The proc script sub_080355CC starts on PROC_TREE_5. */
extern const struct ProcCmd gUnknown_0849BE38[];
/* Three neighbours of gUnknown_0849BE38 in the same block. gUnknown_0849BD20 is
 * the palette row table above. gUnknown_0849BD38 and gUnknown_0849BDE8 are
 * gUnknown_03001470 list scripts, used only as addresses: sub_08035144 and
 * sub_080351F0 start the first with sub_080152EC, and sub_08035F68 stops the
 * second with sub_0801537C. Nothing dereferences either. */
extern const struct Unk0849BD20 gUnknown_0849BD20[];
extern const u8 gUnknown_0849BD38[];
extern const u8 gUnknown_0849BDE8[];
/* Three tile blobs sub_08011C68 copies from, addressed as `(tile & 0x3FF) * 0x20`
 * bytes into the blob, which is all that fixes the element type.
 * gUnknown_08090EC4 is 0x20 zero bytes -- a blank tile to erase with. It sits
 * between two compiler-made address words but is real data: sub_0803678C passes
 * its address and never reads through it. */
extern const u8 gUnknown_081120B0[];
extern const u8 gUnknown_081251B0[];
extern const u8 gUnknown_08090EC4[];
/* A six-word script sub_08037200 hands to sub_0801BD00's `void *` slot --
 * 1, 0x8000, 1, 0xc000, 0x40000001, 0 -- and not tile data. The symbol is the
 * script itself, not a pointer to it. */
extern u32 gUnknown_0848B698[];
/* A dispatch table indexed by a proc's +0x35 state byte, each entry called with
 * the proc as its only argument. Its six entries are the sub_080362xx functions
 * of this same block. */
extern void (*const gUnknown_0849BE20[])(ProcPtr);
/* Two blobs sub_08037DC8 installs one after the other: 0x200 bytes of tiles
 * copied to 0x06000020, then the 0x20-byte palette beside them. */
extern const u8 gUnknown_080A0F38[];
extern u16 gUnknown_080A1138[];
/* Four direction vectors in a flat signed halfword array: direction d is at
 * [d * 2] and [d * 2 + 1]. sub_080362E0 reads it. The values are (-1, 0),
 * (1, 0), (0, 1) and (0, -1), and the table ends where gUnknown_0849BE20
 * begins. */
extern const s16 gUnknown_0849BE10[];
/* Two m4a music players in IWRAM, always used as a pair and always by address:
 * sub_08035DF4 passes each to sub_08070610, and sub_08035E24 and sub_08035E6C
 * pass each to sub_08071488. Those are MPlayVolumeControl and
 * MPlayPitchControl, which read ident at +0x34, trackCount at +0x08 and tracks
 * at +0x2c through this pointer.
 *
 * Must stay declared as arrays: all eight C call sites pass the bare name and
 * rely on array-to-pointer decay, which a bare object of this type does not
 * do. */
extern struct MusicPlayerInfo gUnknown_030059E0[];
extern struct MusicPlayerInfo gUnknown_03005BA0[];
/* Five more players of the same kind. sub_0803B414 hands all seven of the group
 * to sub_08071420 in one run, each as a bare address, which is why all seven
 * carry one type. */
extern struct MusicPlayerInfo gUnknown_03005A60[];
extern struct MusicPlayerInfo gUnknown_03005AA0[];
extern struct MusicPlayerInfo gUnknown_03005B20[];
extern struct MusicPlayerInfo gUnknown_03005BF0[];
extern struct MusicPlayerInfo gUnknown_03005C30[];
/* Two more gUnknown_03001470 list scripts, each used only as an address:
 * sub_08036E54 passes gUnknown_0849D1AC to sub_080152EC and sub_08036F20 passes
 * gUnknown_0849D34C to sub_080193B0, whose parameter is already `const u8 *`.
 * Nothing indexes or dereferences either. */
extern const u8 gUnknown_0849D1AC[];
extern const u8 gUnknown_0849D34C[];
/* 0x08090EA8 is not a global. It is another compiler-made address word, this
 * one holding the address of gUnknown_08499590; the word beside it holds the
 * same address again. sub_080359A4 reads that screen pointer three times across
 * two calls, which is why the compiler stored its address here. Use it exactly
 * like gUnknown_08090D88: bind `&gUnknown_08090EA8` to a local and read
 * `**local`. */
extern u8 **const gUnknown_08090EA8;
/* A blob sub_08040640 hands to sub_0801C70C as its first argument; only the
 * symbol's address is used and nothing matched reads through it. The element
 * type is a guess, chosen to agree with the siblings gUnknown_0810A3E8 and
 * gUnknown_0810AFC8, which reach the same parameter from sub_0803F128. */
extern const u16 gUnknown_08111D94[];
/* The tiles and the palette sub_08040430 loads into OBJ VRAM at 0x06010000.
 * Neither may be made const: Decompress and ApplyPaletteExt both take non-const
 * pointers. Only the addresses are used, so the lengths are unknown. */
extern u8 gUnknown_08111000[];
extern u16 gUnknown_08111D74[];
/* A byte flag sub_0802E4B4 clears, along with gUnknown_030040DC and
 * gUnknown_030033E8. Nothing found reads it, so its purpose is unknown. */
extern u8 gUnknown_03000558;
/* At least two bytes: sub_0802E4B4 clears [0] and [1] through one address, so
 * this is an array or a pair of adjacent byte fields and not a scalar. */
extern u8 gUnknown_030033E8[];
/* A byte-wide mode id, at least two bytes long. sub_0802E4B4 stores 4 in [0];
 * sub_0802966C fills [0] and [1] from gUnknown_0849A06C and then passes the
 * symbol itself to sub_0802E7C8 and sub_080357E0. */
extern u8 gUnknown_03003110[];
/* A buffer sub_0805FF64 passes as the source to sub_0803442C, with
 * gUnknown_03003110 as the destination, so it holds the same kind of data as
 * that destination. Nothing matched indexes it, so its length is unknown. */
extern u8 gUnknown_030046CC[];
/* gUnknown_080C1FC4 and gUnknown_080C9FC4 are two tile blobs the twins
 * sub_08021D64 and sub_08021DA0 copy into BG VRAM, one element at a time, at a
 * stride of 0x1000 and 0xc00 respectively. Nothing indexes inside a row.
 *
 * gUnknown_08552680 is the 0x10 words sub_08057138 copies to 0x05000340, i.e.
 * 32 palette entries starting at palette 26.
 *
 * gUnknown_085D64A8, gUnknown_085D6688 and gUnknown_085D6868 are three tables
 * sub_08057D58 chooses between on its second argument (2, 3, anything else) and
 * then indexes twice: a 20-byte row, then a word within the row. The row must
 * stay a struct wrapping `void *[5]` rather than a plain `void *[][5]`. Both
 * describe the same layout, but with the two-dimensional array the compiler
 * scales the column first, where the original adds the row offset to the base
 * first. */
extern const u8 gUnknown_080C1FC4[];
extern const u8 gUnknown_080C9FC4[];
extern const u16 gUnknown_08552680[];
struct Unk085D64A8 /* 0x14 */
{
    /* 0x00 */ void *unk00[5];
};
extern const struct Unk085D64A8 gUnknown_085D64A8[];
extern const struct Unk085D64A8 gUnknown_085D6688[];
extern const struct Unk085D64A8 gUnknown_085D6868[];
/* A source descriptor for one copy into OBJ VRAM. sub_0805521C and sub_0805530C
 * index tables of these as `[b][c]`, three columns to a row. unk00 is a length
 * in BYTES and is always a multiple of 0x20: when it is non-zero, both
 * functions copy unk00 / 4 words from unk04 and return the tile index advanced
 * by unk00 / 32, which is the same length as a count of 4bpp tiles. unk04 and
 * unk08 are both ROM pointers, but neither function reads unk08 or unk0c, so
 * what those hold is unknown. The 0x18 stride is certain; nothing proves the
 * record stops there. */
struct Unk085D70A8 /* 0x18 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ const void *unk04;
    /* 0x08 */ const void *unk08;
    /* 0x0c */ u32 unk0c[3];
};
extern const struct Unk085D70A8 gUnknown_085D70A8[][3];
extern const struct Unk085D70A8 gUnknown_085D7768[][3];
/* The currently-selected unit id -- the same 1-based id gUnknown_08499594 is
 * keyed by. sub_0802E4B4 fills it from the map cell byte at
 * gUnknown_08499590[0x12 + rowOffset[y] + x] and immediately uses it to index
 * that table. Unsigned: the `>> 6` that derives gUnknown_03004480 from it is a
 * logical shift. */
extern u8 gUnknown_03003F38;
/* A word-wide state counter, compared unsigned against 0x10 and 1 by
 * sub_0805C1D8, which also stores 1 into it. sub_0805C208 names it directly,
 * which is what shows it is a real global and not a compiler-made address
 * word. */
extern u32 gUnknown_03004770;
/* A halfword sub_0802E4B4 writes twice in a row: first
 * `(gUnknown_03003F38 >> 6) + 1`, then gUnknown_030033EC. */
extern u16 gUnknown_03004480;

/* ---- the second display-state block ----
 * sub_0801258C copies 43 values out of the display-register shadows that
 * sub_08012420 pushes to the hardware, into the parallel set of symbols below.
 * Each symbol here is therefore the snapshot partner of one shadow, and its
 * width is the width of the copy.
 *
 * Six of them are written as packed words -- `lo | (hi << 16)` in a single
 * store -- which is why some are u32 where their neighbours are u16. And
 * gUnknown_030030B8 really is a 64-bit object, built as
 * `(u64)a | ((u64)b << 16) | ((u64)c << 32) | ((u64)d << 48)`; no 32-bit
 * spelling produces the same code. */
extern u16 gUnknown_0300140C;
extern u16 gUnknown_03001410;
extern u16 gUnknown_03001414;
extern u16 gUnknown_03001424;
extern u32 gUnknown_03001FB0;
extern u16 gUnknown_03001FB4;
extern u16 gUnknown_03001FB8;
extern u16 gUnknown_03001FC0;
/* gUnknown_03001FC8, gUnknown_030024E0 and gUnknown_03002004 are not declared
 * here. They live in include/hardware.h as `union BgCntBuf`, `union BgCntBuf`
 * and `union DispCntBuf`, because sub_08018254 writes them field by field with
 * byte-wide read-modify-writes that a plain u16 cannot produce. Do not add an
 * `extern u16` for them here: it breaks every file that includes both headers
 * with conflicting types, exactly as the gUnknown_03001FE8 note above
 * records. */
extern u16 gUnknown_03001FC4;
extern u16 gUnknown_03001FCC;
extern u8 gUnknown_03001FD8;
/* The HBlank shadow copy of REG_BLDALPHA's high byte. gUnknown_030030C0 holds
 * the low byte, and gUnknown_030030A8 beside them is already declared volatile.
 *
 * sub_08017880, the HBlank raster handler, only reproduces the original when
 * this and gUnknown_030030C0 are read through a `vu16`: the volatile pair makes
 * the compiler work out both addresses before it loads either value. That cast
 * lives in c_08017880.c rather than here, because it is the only function known
 * to need it and qualifying the global would mean re-checking every other user.
 * Move the qualifier onto this declaration if a second function needs it. See
 * docs/agbcc-codegen.md. */
extern u16 gUnknown_03001FEC;
extern u16 gUnknown_03002008;
extern u32 gUnknown_03002010;
extern u16 gUnknown_03002014;
extern u16 gUnknown_03002018;
extern u16 gUnknown_0300201C;
extern u32 gUnknown_03002024;
extern u32 gUnknown_03002030;
extern u16 gUnknown_03002034;
extern u8 gUnknown_030020A4;
extern u16 gUnknown_030024C4;
extern u16 gUnknown_030024CC;
extern u16 gUnknown_03002518;
extern u32 gUnknown_03002B3C;
extern u16 gUnknown_03002B48;
extern u16 gUnknown_03002B50;
extern u16 gUnknown_03002B58;
extern u16 gUnknown_03002B64;
extern u8 gUnknown_03002B70;
extern u16 gUnknown_03002EDC;
extern u8 gUnknown_03002EE8;
extern u8 gUnknown_03002EEC;
extern u16 gUnknown_03002EF4;
extern u16 gUnknown_03002F10;
extern u16 gUnknown_03002F14;
extern u16 gUnknown_03002F28;
extern u8 gUnknown_03002F34;
extern u16 gUnknown_03002F38;
extern u16 gUnknown_03002F40;
extern u32 gUnknown_03003030;
extern u16 gUnknown_03003038;
extern u16 gUnknown_0300303C;
/* The gUnknown_0200E438 slot that is currently running: sub_0801D390 stores its
 * first argument here as its first statement. Nothing matched reads it, so
 * whether the value is signed is unknown. */
extern u16 gUnknown_03003040;
extern u16 gUnknown_03003044;
extern u16 gUnknown_03003090;
extern u8 gUnknown_03003094;
extern u16 gUnknown_03003098;
extern u8 gUnknown_030030AC;
/* The low byte of the REG_BLDALPHA value sub_08017880 writes. It needs the same
 * `vu16` read as gUnknown_03001FEC above; see the note there. */
extern u16 gUnknown_030030C0;
extern u16 gUnknown_030030D8;
extern u16 gUnknown_030030E4;
/* The four window-bound shadows: the WIN0H, WIN1H, WIN0V and WIN1V halves, in
 * that order. All four must stay volatile. sub_0801258C ORs them into one
 * 64-bit value; on a plain read the compiler folds each halfword load straight
 * into the 64-bit low part, while a volatile read leaves a register copy behind.
 * Four terms means four extra copies, and the extra pressure is also why that
 * function has a three-high-register prologue. */
extern volatile u16 gUnknown_030020B0;
extern volatile u16 gUnknown_0300309C;
extern volatile u16 gUnknown_03002B60;
extern volatile u16 gUnknown_03002028;
/* The 8-byte snapshot of those four, written as one 64-bit store pair. */
extern u64 gUnknown_030030B8;

/* Four parallel tables of proc ids. sub_08053860 and sub_08053BB8 walk them
 * five ids at a time, twice each: once at the group-0 offset and once one fixed
 * stride higher, with the palette argument switching from
 * `gUnknown_085523A8[gUnknown_0300450C]` to `[gUnknown_0300450C ^ 1]` in step
 * with the group. So each table holds two records of five s16 ids, at strides of
 * 0xb4, 0x0a, 0x6c and 0x28. Only the id members below are known; every filler
 * is the space between two known offsets and nothing more. */
/* The five-slot record those tables hold. sub_08051DE0, sub_080524C0 and
 * sub_0805297C all read the same two expressions: unk3a[unk2e] is an OBJ tile
 * index, with unk2e as its cursor, and unk30[j] is a row index into
 * gUnknown_08552D80, with j taken from gUnknown_0300451C. The [5] is the slot
 * count the rest of the record uses throughout, and every byte from 0x24 to the
 * end of the record is covered by a five-slot array, which is what makes those
 * bounds believable rather than proved. Nothing sign-extends these reads, so
 * whether the halfwords were declared signed is unknown. */
struct Unk02029808 /* 0x6c */
{
    /* sub_08053670 uses the halfword at +0 as an index into the three
     * halfword arrays at +2, +0xe and +0x1a. Their [5] bounds are the slot count
     * this record uses throughout, which leaves a 2-byte gap before the second
     * and third arrays. The code cannot tell that apart from gapless arrays of
     * 6, 6 and 5, and both add up to the same 0x24 bytes, so the fillers are the
     * cautious reading and nothing depends on the choice. */
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02[5];
    /* 0x0c */ u8 filler_0c[0x02];
    /* 0x0e */ u16 unk0e[5];
    /* 0x18 */ u8 filler_18[0x02];
    /* 0x1a */ u16 unk1a[5];
    /* 0x24 */ s16 unk24[5];
    /* 0x2e */ u16 unk2e;
    /* 0x30 */ u16 unk30[5];
    /* 0x3a */ u16 unk3a[5];
    /* 0x44 */ void *unk44[5]; /* Two more slot-indexed arrays, words
                                * rather than halfwords, running to the end of
                                * the 0x6c record exactly. sub_08051F4C reads
                                * unk44[unk2e] and unk58[unk2e] and passes them
                                * as sub_08015410's third and fourth arguments:
                                * the graphics and palette pair, the same pair
                                * columns 1 and 2 of gUnknown_08557978 supply.
                                * Note the index here is unk2e, not the slot
                                * index used elsewhere. */
    /* 0x58 */ void *unk58[5];
};

struct Unk020296B0 /* 0x28 */
{
    /* 0x00 */ u16 unk00; /* The OBJ tile index for the slot:
                           * sub_08050B70 reads it and assigns it to struct
                           * OamData's 10-bit tileNum. The masking is the
                           * bitfield's, so the stored value may be wider. */
    /* 0x02 */ s16 unk02[5];
    /* 0x0c */ u16 unk0c[6]; /* A command stream: sub_0804D0FC and
                              * sub_0804D4E8 read `unk0c[unk18]` and stop at an
                              * entry of 0xff, bumping unk18 on every visit. The
                              * [6] is only the distance to unk18; the cursor can
                              * run past it at run time. */
    /* 0x18 */ u16 unk18; /* The read cursor into unk0c, bumped by
                           * sub_0804D0FC and sub_0804D4E8 once per tick on
                           * which sub_080156C4 reports 2. */
    /* 0x1a */ u8 unk1a; /* A phase counter, incremented once per
                          * sound emitted: sub_0804E7A8 and sub_0804FCA4 both do
                          * `sub_0803B48C(tbl[..][g[c].unk1a & 1]);
                          * g[c].unk1a++;`, so only bit 0 is ever used. */
    /* 0x1b */ u8 unk1b; /* A second phase counter beside unk1a and
                          * used the same way: sub_08050C8C increments it when
                          * sub_080156C4 returns 3. Nothing bounds it. */
    /* 0x1c */ u8 unk1c; /* A small table selector: sub_08050D44 uses
                          * it to pick a gUnknown_08553B1C row pointer. Nothing
                          * bounds it. Reach it as a named member: through a
                          * `(u8 *)` cast the compiler folds the 0x1c into the
                          * pool word instead of leaving it on the load. */
    /* 0x1d */ u8 filler_1d[0x01];
    /* 0x1e */ u16 unk1e[5]; /* The timestamp row that goes with the unk0c
                              * command row and shares its unk18 cursor:
                              * sub_08054278 compares `unk1e[unk18]` with the
                              * gUnknown_03004508 frame counter and dispatches
                              * the command through gUnknown_08553744 when the
                              * two are equal. gUnknown_020298E0's unk0c and
                              * unk1a play the same two roles with the layout
                              * swapped. The [5] is what the space up to 0x28
                              * allows; like unk0c, the cursor can run past
                              * it. */
};

extern s16 gUnknown_02029668[][5];
/* Row 2 of the table above (gUnknown_02029668 + 0x14), given a linker symbol of
 * its own because sub_0804FF44 indexes it from here with the same 10-byte row
 * stride. Element [0] takes sub_08015410's signed byte result, sign-extended
 * into the halfword. */
extern s16 gUnknown_0202967C[][5];
/* The per-side movement record, 0x58 bytes, indexed by
 * gUnknown_03001470[proc].unk30 (the side, 0 or 1) and holding five parallel
 * slots indexed by that proc's unk34. sub_08050958 steps two 8.8 fixed-point
 * axes once per tick:
 *
 *   unk26[j] += unk30[j];        x, fractional part
 *   (x whole) += unk26[j] >> 8;
 *   unk44[j] += unk4e[j];        y, fractional part
 *   unk3a[j] += unk44[j] >> 8;
 *
 * Those shifts are arithmetic, which is what makes unk26 and unk44 -- and by
 * symmetry unk3a and the row at 0x1c -- signed; the steps unk30 and unk4e are
 * only ever added and stay unsigned. unk16 is a tick threshold compared against
 * the proc's unk2c, and unk18 and unk1a are the x and y bounds the same
 * function tests the two whole coordinates against.
 *
 * The row at 0x1c has a linker symbol of its own, gUnknown_0202972C
 * (== gUnknown_02029710 + 0x1c), and sub_08050958 reaches it from there. It is
 * left as filler in this struct so the two spellings cannot drift apart. The
 * 44-halfword row length is only the 0x58 stride, not a measured extent. */
struct Unk02029710 /* 0x58 */
{
    /* 0x00 */ u16 unk00; /* sub_08050C8C tests it against 0 as the
                           * first of three guards before raising a
                           * gUnknown_02029664 bit. Nothing matched writes it,
                           * so its meaning is otherwise unknown. */
    /* 0x02 */ u8 filler_02[0x02];
    /* 0x04 */ void *unk04; /* sub_080505A4 hands unk04 and unk08
                             * straight to sub_08015410's third and fourth
                             * parameters: the graphics and palette pair, the
                             * same pair gUnknown_02029700's two columns supply
                             * for the other table. */
    /* 0x08 */ void *unk08;
    /* 0x0c */ s16 unk0c[5]; /* One animation handle per slot:
                              * sub_080505A4 stores sub_08015410's signed byte
                              * result here, sign-extended into the halfword and
                              * indexed by the slot -- the same role
                              * gUnknown_020296B0.unk02 plays. */
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u16 unk18;
    /* 0x1a */ u16 unk1a;
    /* 0x1c */ u8 filler_1c[0x0a]; /* == gUnknown_0202972C[i][0..4] */
    /* 0x26 */ s16 unk26[5];
    /* 0x30 */ u16 unk30[5];
    /* 0x3a */ s16 unk3a[5];
    /* 0x44 */ s16 unk44[5];
    /* 0x4e */ u16 unk4e[5];
};
extern struct Unk02029710 gUnknown_02029710[];
/* 0x020297B6 is gUnknown_02029710[1].unk4e and not an object of its own.
 * sub_080566C8 writes all eight rows of the pair as
 * `gUnknown_02029710[side].unkXX[j]`, and the original reaches this one through
 * a pool word only because it is the last of the eight in the chain built off
 * the base register. A pool word on its own is therefore not evidence of a
 * second linker symbol; the note on gUnknown_02029BC4 makes the same point for
 * constant subscripts. */
extern s16 gUnknown_0202972C[][44];
extern struct Unk020296B0 gUnknown_020296B0[];
/* Two rows of halfword slots, one per side, 40 bytes to a row. sub_08055940
 * reads [0][i] and [1][i] for i = 0..4 and counts the entries that are not 0xff
 * into a per-side counter, so 0xff means "empty". The [20] is the byte stride
 * divided by the element size, not a measured extent: only columns 0..4 are ever
 * read. gUnknown_020296CE below is column 9 of these same rows. */
extern u16 gUnknown_020296BC[][20];
/* Column 9 of gUnknown_020296BC's rows (that symbol + 0x12), with a linker
 * symbol of its own because sub_08055768 and sub_08055A38 fill `[side][i]` from
 * here. Spelling it `gUnknown_020296BC[side][i + 9]` does not reproduce the
 * original: agbcc does not distribute the element scaling over `i + 9` and emits
 * `(i + 9) * 2` instead. The 20 columns are the 40-byte stride, not a measured
 * extent. */
extern u16 gUnknown_020296CE[][20];
/* Row 1 of gUnknown_020296BC (that symbol + 0x28) under a linker symbol of its
 * own. sub_08055940 reads both counted rows through this one, working row 0 out
 * by subtracting 0x28, so the original source named this symbol and not
 * gUnknown_020296BC. */
extern u16 gUnknown_020296E4[][20];
/* A pair of counters, five per side, always used as `[side][slot]`.
 * sub_080501DC does `gUnknown_02029B94[i][j] += gUnknown_02029B80[i][j];` and
 * zeroes both when the sum reaches 6, so gUnknown_02029B80 is a per-frame step
 * and gUnknown_02029B94 the accumulator that wraps at six. The [2][5] is exact:
 * the 20 bytes between the two symbols leave no room for a sixth slot or a third
 * side. */
/* Two pointer columns per gUnknown_0300453C side. sub_080505A4 reads both and
 * passes them as sub_08015410's third and fourth arguments: the graphics and
 * palette descriptor pair. */
extern void *gUnknown_02029700[][2];
extern u16 gUnknown_02029B80[2][5];
extern u16 gUnknown_02029B94[2][5];
/* A flag per (side, command), set to 1 by sub_08053FBC as it retires each script
 * command. The [2][5] is exact: it tiles the 20 bytes up to gUnknown_02029C00
 * below. */
extern u16 gUnknown_02029BEC[2][5];
/* One read cursor per side into gUnknown_020296B0[i].unk0c, for sub_08053FBC's
 * script channel: bumped once a command is accepted, and used as the subscript
 * into gUnknown_08551E12. It is a separate cursor from
 * gUnknown_020296B0[i].unk18, which sub_0804D0FC and sub_0804D4E8 drive over the
 * same command array. */
extern u16 gUnknown_02029C00[2];
/* A phase counter per side, bumped once per sound sub_08053FBC emits and used
 * only as `& 1` -- the same role gUnknown_020296B0[i].unk1a plays for the other
 * channel. */
extern u16 gUnknown_02029C04[2];
/* A third read cursor over gUnknown_020296B0[i].unk0c, this one for
 * sub_0805414C's channel, beside gUnknown_02029C00 (sub_08053FBC's) and
 * gUnknown_020296B0[i].unk18 (sub_08054278's). It is bumped once a command is
 * accepted and also subscripts gUnknown_08551E12, exactly as gUnknown_02029C00
 * does. One per side. */
extern u16 gUnknown_02029C08[2];
/* Per-side records of 0x90 bytes. The two five-slot halfword arrays at +0x26 and
 * +0x30 are indexed by gUnknown_03001470[proc].unk34 (the slot) and summed into
 * a column of gUnknown_08553B40.
 *
 * Must stay a struct rather than `u16 [][72]`: the original adds a member's
 * offset to the base register and leaves the load displacement at zero, which is
 * what a struct member holding an array produces, where the flat spelling folds
 * the same constant into the symbol's relocation. */
struct Unk020298E0 /* 0x90 */
{
    /* 0x00 */ u16 unk00; /* A tile base. sub_08054EE0 and
                           * sub_08054F50 store the same value --
                           * `(u16)(i * 0x100 + 0x50)` -- into
                           * gUnknown_020297C0[i].unk00,
                           * gUnknown_020296B0[i].unk00 and this member, back to
                           * back, and the first two are already known to be tile
                           * bases. */
    /* 0x02 */ s16 unk02[5];  /* sub_08050F24 writes
                               * unk02[unk16 - 1] with sub_08015410's signed byte
                               * result, sign-extended into the halfword. The [5]
                               * is bounded by the second halfword row at +0x0c,
                               * which sub_08054488 reads on the same unk16
                               * index; the two views overlap if unk16 ever
                               * exceeds 5, but a declared length never enters
                               * address arithmetic, so the bound changes no
                               * code. */
    /* 0x0c */ u16 unk0c[5];  /* The timestamp row for the unk1a
                               * command row below, on the shared unk16 cursor:
                               * sub_08054488 compares `unk0c[unk16]` with the
                               * gUnknown_03004508 frame counter. The same pair
                               * as gUnknown_020296B0's unk0c and unk1e, with the
                               * roles of the two offsets swapped. */
    /* 0x16 */ u16 unk16;     /* The fill level of unk02 above: one is
                               * subtracted from it to get the subscript, i.e.
                               * the last slot written. */
    /* 0x18 */ u16 unk18;     /* A three-state phase counter, one per side
                               * rather than per slot (the address has no slot
                               * term): sub_08050FF8 increments it and zeroes it
                               * when it reaches 3, and sub_08051454 bumps it the
                               * same way and uses it to index
                               * gUnknown_08553BFC. */
    /* 0x1a */ u16 unk1a[6];  /* The command row this record's unk16
                               * cursor walks: sub_08054488 reads
                               * `unk1a[unk16]`, stops on the 0xff terminator and
                               * otherwise dispatches through gUnknown_0855374C.
                               * It is gUnknown_020296B0's unk0c for the other
                               * channel. The [6] is the span up to 0x26; the
                               * cursor can run past it. */
    /* 0x26 */ u16 unk26[5];
    /* 0x30 */ u16 unk30[5];
    /* 0x3a */ s16 unk3a[5]; /* A third five-slot halfword array on the
                              * same [side][slot] index as unk26 and unk30: a
                              * countdown. Must stay signed -- sub_0804BDD8 and
                              * sub_0804BECC decrement it and test the result
                              * against 0 on the sign of the low half, which an
                              * unsigned member cannot produce. The extent
                              * follows unk26 and unk30 and is what the 0x44
                              * boundary allows. */
    /* 0x44 */ u16 unk44[5]; /* The same halfwords as
                              * gUnknown_02029924[i][0..4], which has a linker
                              * symbol of its own -- the way gUnknown_0202972C
                              * overlaps gUnknown_02029710. Both spellings are
                              * real: sub_080517BC indexes gUnknown_02029924
                              * flat, while sub_08051454 reaches the same
                              * halfword as this member. The two views must keep
                              * the same offset and length, and the element type
                              * agrees with gUnknown_02029924. */
    /* 0x4e */ s16 unk4e[5]; /* The whole-pixel half of a second 8.8
                              * fixed-point axis, stepped by sub_080517BC as
                              * `unk6c[j] += unk76[j];
                              * unk4e[j] += (s16)unk6c[j] >> 8;`. Must stay
                              * signed: the same function re-reads it
                              * sign-extended to hand sub_08050528 its s16 fourth
                              * argument. */
    /* 0x58 */ u16 unk58[5]; /* The sub-pixel accumulator of the first
                              * axis: `unk58[j] += unk62[j]`, and the sum shifted
                              * right by 8 is added to gUnknown_02029924[i][j].
                              * Only ever added to and stored back. */
    /* 0x62 */ u16 unk62[5]; /* the first axis's per-tick step. */
    /* 0x6c */ u16 unk6c[5]; /* the second axis's sub-pixel accumulator, feeding
                              * unk4e the same way. */
    /* 0x76 */ u16 unk76[5]; /* the second axis's per-tick step. */
    /* 0x80 */ u16 unk80[5]; /* A per-slot frame counter, incremented
                              * once per sub_080517BC visit and compared for
                              * equality against unk8a below. */
    /* 0x8a */ u16 unk8a;    /* The value unk80[j] is compared against.
                              * One per side, not per slot: the address has no
                              * slot term. */
    /* 0x8c */ u8 unk8c; /* The alternating phase counter that goes
                          * with unk8d: sub_08053520 does
                          * `sub_0803B48C(gUnknown_085643A8.unk04[unk8c & 1]);
                          * unk8c++; unk8d = 0;`, so only bit 0 is ever used --
                          * the same shape as gUnknown_020296B0.unk1a. */
    /* 0x8d */ u8 unk8d; /* sub_0805131C increments it. Nothing in the
                          * matched code reads it back, so what it counts for is
                          * unknown. */
    /* 0x8e */ u8 filler_8e[0x02];
};
extern struct Unk020298E0 gUnknown_020298E0[];
/* gUnknown_020298E0 + 0x44 under a linker symbol of its own: the whole-pixel
 * half of the first 8.8 fixed-point axis, whose sub-pixel accumulator is
 * gUnknown_020298E0.unk58. sub_080517BC holds a pool word for this symbol and
 * another for gUnknown_020298E0 in the same function, so both spellings are
 * real. The 72 halfwords are the 0x90 stride, not a measured extent.
 *
 * Must stay unsigned: sub_080517BC adds a u32 to it and narrows the sum to s16
 * for sub_08050528. An unsigned element makes that a halfword load plus a
 * narrowing, which is what the original does; a signed element turns the
 * addition into a 32-bit one and changes the load. */
extern u16 gUnknown_02029924[][72];
/* gUnknown_020298E0 + 0x26 under a linker symbol of its own -- the unk26 row,
 * just as gUnknown_02029924 is the unk44 row. sub_08050FF8 indexes it flat from
 * this symbol (`i * 0x90 + k * 2`, three times), which is what fixes the
 * spelling; reaching it as a member of gUnknown_020298E0 produces a different
 * address chain. The 72 halfwords are the 0x90 stride, not an extent. */
extern u16 gUnknown_02029906[][72];
/* Wave 49, W49-M. == gUnknown_020298E0 + 0x0c, i.e. the unk0c row through its
 * own linker symbol, the third of these after gUnknown_02029924 (+0x44) and
 * gUnknown_02029906 (+0x26). sub_0805634C holds a pool word for THIS symbol
 * and indexes it FLAT (`j * 2 + side * 0x90` off the symbol, zero `ldrh`
 * displacement) in the same inner loop where it reaches unk1a through the
 * member form (`=gUnknown_020298E0` plus `adds rB,#0x1a`) -- two spellings of
 * two rows of one record, side by side, which is what fixes both. The
 * 72-halfword row IS the 0x90 stride, not a proved extent. */
extern u16 gUnknown_020298EC[][72];
extern struct Unk02029808 gUnknown_02029808[];
/* Wave 48, W48-G. == gUnknown_02029808 + 0xe, i.e. the unk0e row through its
 * own linker symbol, exactly as gUnknown_02029924 and gUnknown_02029906 are
 * rows of gUnknown_020298E0. sub_080564B8 holds a pool word for THIS symbol
 * for its two `= 1` fill loops (flat `i * 2 + side * 0x6c` off the symbol) and
 * a SECOND one for gUnknown_02029808 in the same function for the `+=` that
 * follows, where the ROM shows the member form's `adds rB, #0xe`. The
 * 54-halfword row IS the 0x6c stride, not a proved extent. */
extern u16 gUnknown_02029816[][54];
/* Wave 48, W48-G. Two more rows of gUnknown_02029808 through their own linker
 * symbols, on the same 0x6c stride: gUnknown_0202980A == +0x02 (the unk02 row)
 * and gUnknown_02029822 == +0x1a (the unk1a row). sub_08056638 bubble-sorts
 * gUnknown_02029822[side][0..4] and carries gUnknown_0202980A along as the
 * payload, holding a pool word for each and indexing both flat off the symbol
 * (`j * 2 + side * 0x6c`). 0xFF is that sort's empty-slot sentinel and the
 * comparisons are unsigned (`bls`), which is the whole evidence for u16. */
extern u16 gUnknown_0202980A[][54];
extern u16 gUnknown_02029822[][54];
/* Wave 33, W33-D. A 0x24-byte per-side record indexed by gUnknown_0300453C
 * (`((i * 9) << 2)` at both sites). Two readers, both in the 0x08051 block:
 *   unk00     sub_08051A44 reads it `ldrh` and drops it straight into
 *             OamData.tileNum, so it is a tile base and at most 10 bits wide.
 *   unk02[j]  sub_0805198C stores sub_08015410's s8 result here with `strh`,
 *             j being gUnknown_0300451C -- the same "one animation handle per
 *             slot" role gUnknown_02029808[i].unk24[j] plays for the other
 *             descriptor family, hence the [5] and the signed type.
 *   unk18     the two `void *` arguments sub_0805198C hands sub_08015410 as
 *   unk20     parameters 3 and 4; whole-word `ldr` at both.
 * Every filler below is unreached space between two proved offsets. */
struct Unk020297C0 /* 0x24 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ s16 unk02[5];
    /* 0x0c */ u16 unk0c[5];
    /* 0x16 */ u8 filler_16[0x02];
    /* 0x18 */ void *unk18;
    /* 0x1c */ void *unk1c;
    /* 0x20 */ void *unk20;
};
extern struct Unk020297C0 gUnknown_020297C0[];
/* Wave 56, W56-O. == gUnknown_020297C0 + 0x0c, i.e. the unk0c row of the record
 * above through its own linker symbol, on the same 0x24 stride -- the same
 * split gUnknown_02029816 / gUnknown_02029822 are of gUnknown_02029808.
 * sub_080546F0 holds a pool word of exactly 0x020297CC and indexes it FLAT
 * (`side * 0x24 + j * 2` off the symbol, zero `strh` displacement) in the same
 * inner loop where it reaches unk02 through the MEMBER form
 * (`=gUnknown_020297C0` plus `adds rB,#2`), which is what fixes both
 * spellings. u16 from the `strh` of 0; nothing here signs it. The 18-halfword
 * row IS the 0x24 stride, not a proved extent. */
extern u16 gUnknown_020297CC[][18];
/* A halfword cell per gUnknown_0300453C side, read `gUnknown_02029BE8[s ^ 1]`
 * -- the OTHER side's slot -- and tested `!= 0` by sub_0804BDD8/sub_0804BECC
 * before they clear a per-slot counter. Bare `ldrh` off `index * 2`, nothing
 * sign-extends, so a plain u16 array; the extent is unknown (aw2bhr.lds puts
 * the next symbol 4 bytes later, which bounds it at 2 if that symbol is real)
 * (wave 21, W21-A). */
extern u16 gUnknown_02029BE8[];
/* Set to 1 with a bare `strh` by sub_08053BB8 and by nothing else matched. */
extern u16 gUnknown_030045B0;
/* Two ROM u16 tables indexed by gUnknown_0300450C (and, for the second, by its
 * complement): 08551E7C supplies sub_0805741C's argument in sub_08053860,
 * 085523A8 supplies the palette sub_08053614 pushes into bits 10-11 of an OAM
 * word. Both are read with a bare `ldrh` off `index * 2`, so they are plain u16
 * arrays rather than aggregates -- the same reasoning as gUnknown_085538AE. */
/* Two ADJACENT byte tables scoring the 5-bit terrain code that
 * gUnknown_08499590 + 0x1432 holds, read `tbl[cell & 0x1f]` by the twins
 * sub_0804B42C (0x08551CA0) and sub_0804B4C4 (0x08551CBD).
 *
 * The ODD address of the second is real and not a mis-attributed pool word:
 * 0x08551CA0 + 0x1d == 0x08551CBD and 0x08551CBD + 0x1f == 0x08551CDC, so the
 * three symbols tile the region exactly, and the two tables are byte-for-byte
 * the same in ROM apart from element 5 (3 vs 0) -- parallel tables of 29 and
 * 31 explicit initialisers, not one table read at two offsets. A 32-entry
 * reading is what the 0x1f mask suggests and it is WRONG: it would make the
 * two overlap. Values are 0..4, read `ldrb` into an UNSIGNED comparison (the
 * running best is `bhs`, not `bge`), so the running maximum is `u32` in the
 * source while the table itself is only ever zero-extended (wave 21, W21-A). */
/* Wave 35, W35-F. sub_0804B55C indexes both with `lsls #0x18; lsrs #0x17` off
 * an int-returning callee -- a `(u8)` cast scaled by 2 -- and reads them
 * `ldrh`, so they are u16 tables in the same 0..0xff index space
 * gUnknown_08551CA0 scores. A zero element means "no substitution". */
extern u16 gUnknown_08551C00[];
extern u16 gUnknown_08551C3A[];
/* Wave 35, W35-F. A genuine ROM POINTER TABLE, not a -fforce-addr word: the
 * entries at 0x08551C88..0x08551C9C hold 0x08551C78, 0x08551C78, 0x08551C7C,
 * 0x08551C80, 0x08551C84, 0x08551C74 -- five DISTINCT targets, which a
 * force-addr constant pool cannot be. sub_0804B55C loads one with
 * `lsls #2; ldr` and then reads `[ptr + (rand & 1) * 2]` with `ldrh`, so each
 * row is a two-element u16 pair. */
extern u16 *gUnknown_08551C88[];
extern const u8 gUnknown_08551CA0[];
extern const u8 gUnknown_08551CBD[];
/* The four cardinal neighbour offsets as (dx, dy) word pairs -- (1,0), (-1,0),
 * (0,-1), (0,1) -- walked with `ldr [r1]; ldr [r1,#4]; adds r1,#8` by both
 * twins above and added to signed cell coordinates. Words and signed: the
 * elements ARE -1 in ROM. */
extern const s32 gUnknown_08551CDC[][2];
/* Wave 54, W54-H. The FOUR halfwords between gUnknown_08551E64's four-word
 * pointer table (which ends at 0x08551E74) and gUnknown_08551E7C, read on
 * exactly the index gUnknown_08551E7C takes: sub_08055768 and sub_08055A38
 * both do `ldrh` at `gUnknown_08551E74[side * 2 + gUnknown_0300450C]` and use
 * the value as a ROW index (`* 5 + i`) into the u16 row gUnknown_08551E64
 * hands them. A parallel table to gUnknown_08551E7C rather than the same one:
 * the 8-byte gap fits four halfwords exactly and both symbols carry their own
 * pool word. Nothing signs the load. */
extern const u16 gUnknown_08551E74[];
extern const u16 gUnknown_08551E7C[];
extern const u16 gUnknown_085523A8[];
/* Wave 49, W49-M, block 0x08056. A ROM table of FIVE-halfword rows, and the
 * ROM content is the proof rather than a stride guess: 0x08551D22 reads
 * 0,50,60,110,111 / 0,50,60,110,120 / 0,50,20,180,190 / 0,50,50,170,180 --
 * four rows that each restart at 0 on a 10-byte period, running to
 * gUnknown_08551E12 (0xf0 == 24 rows).
 *   Both sub_0805634C and sub_080560A4 subscript it with
 * `gUnknown_030045A0[side ^ 1]`, a computed subscript ADDED to the loaded word
 * before any dereference, so this is a real table and not a -fforce-addr pool
 * word.
 *   gUnknown_08551D2A == gUnknown_08551D22 + 8, i.e. that table's column 4
 * through its own linker symbol, exactly as gUnknown_02029924 and friends are
 * rows of gUnknown_020298E0. It is a SEPARATE declared object and not the same
 * array read at another column: sub_0805634C reaches column 3 as
 * `gUnknown_08551D22[x][3]` -- bare pool word plus a run-time `adds rB,#6`,
 * the nested-array form -- and column 4 as a pool word that already carries
 * the +8, in the same statement and off the SAME CSEd `x * 10`. One declared
 * array cannot produce both spellings for two of its own columns.
 * Read `ldrh`, nothing signs them; the values are small y offsets that are
 * subtracted and added around gUnknown_020298EC. */
extern const u16 gUnknown_08551D22[][5];
extern const u16 gUnknown_08551D26[][5];
extern const u16 gUnknown_08551D2A[][5];
/* Wave 49, W49-M. A genuine ROM POINTER TABLE of four words, not a
 * -fforce-addr pool run: the words at 0x08551E64..0x08551E70 hold 0x08551E12,
 * 0x08551E26, 0x08551E3A and 0x08551E4E -- four DISTINCT targets spaced 0x14
 * apart, which a force-addr constant pool cannot be. sub_0805634C and
 * sub_080560A4 both index it `gUnknown_030045A0[gUnknown_0300450C] * 4` and
 * `ldr` the word into a local held across a loop, then read the pointed-to row
 * `ldrh` at `gUnknown_08551E7C[side * 2 + gUnknown_0300450C] * 5 + i` -- ten
 * halfwords, which is exactly the 0x14 spacing. Non-const element type to
 * match gUnknown_08551C88, the other pointer table here. */
extern u16 *gUnknown_08551E64[];

/* The screen-setup blobs sub_08065990 / sub_0806D944 hand out. Typed from the
 * callee that receives each, which is the discriminating use in every case:
 * the two Decompress sources are `u8 *` because that is Decompress's parameter
 * and const would break it, and 082344CC is `u16 *` because it goes to
 * ApplyPaletteExt. */
/* A u16 palette source: sub_0806A054 hands it to ApplyPaletteExt with
 * (0x20, 0xa0), i.e. ApplyPalettes(src, 1, 5) -- five palettes starting at BG
 * palette 1. */
extern u16 gUnknown_0822FE50[];
extern u8 gUnknown_0822F9AC[];
extern u8 gUnknown_0822FEF0[];
extern u16 gUnknown_082344CC[];
/* Wave 44, W44-F. sub_0807F434's two blobs, both reached through the
 * 0x081D936C .rodata address-constant pool rather than a direct `ldr rN, =sym`.
 * 08234B10 is a Decompress SOURCE unpacked into gUnknown_0200FC50, so `u8 []`
 * and NOT const -- Decompress's first parameter is `u8 *` (same model as
 * gUnknown_0823DE38). 082352DC goes straight to ApplyPaletteExt's `u16 *`
 * first parameter with no arithmetic, 0x20 bytes at palette word 0x280, i.e.
 * one OBJ palette. Neither extent is pinned. */
extern u8 gUnknown_08234B10[];
extern u16 gUnknown_082352DC[];
/* A destination buffer in EWRAM, not ROM: sub_08065238 and sub_0806D944 both
 * pass it as sub_08073304's second argument alongside a ROM first argument. */
extern u8 gUnknown_0200FC50[];
/* Wave 44, W44-F. The EWRAM tile source sub_080801A8 CpuFastSets into the two
 * BG screen blocks (0x100 words each, eight times), so `const void *` is all
 * that is pinned and `u8 []` is the weakest model -- the same one
 * gUnknown_0200FC50 above carries. It sits exactly 0x400 bytes after
 * gUnknown_0200FC50, which is the 0x400 bytes that function copies out of
 * gUnknown_0200FC50 in its tail (0x80 words at +0 and at +0x200), so the two
 * are adjacent buffers rather than one. */
extern u8 gUnknown_02010050[];
/* A small step counter, `ldr`/`cmp` only. SIGNED: sub_080852A8 gates its first
 * block with `cmp r5, #3; bgt`, and it doubles as an array index. */
extern int gUnknown_03005940;
/* The sub_08080498 screen-setup blobs. 082391E8 / 080A36C8 / 08239DE4 are u16
 * palettes (ApplyPaletteExt at 0x00, 0x100 and 0x200 with 0x20 bytes each, i.e.
 * BG palettes 0 and 8 and OBJ palette 0); 08236294 / 08235D30 / 08239228 are
 * Decompress sources; 080A29A4 goes to sub_08011E54 and 080A31A4 to
 * sub_08012B70; 086168BC is a proc script Proc_Start runs under the caller.
 * Non-const on the blobs because Decompress and sub_08011E54 both take plain
 * `u8 *` / `void *`. */
extern u8 gUnknown_08235D30[];
extern u8 gUnknown_08236294[];
extern u8 gUnknown_08239228[];
/* Wave 33, W33-E. The three assets sub_080800B0 loads, typed off the call it
 * hands each one to: 08235558 and 082352FC go to `Decompress(u8 *, void *)`,
 * and 08235D10 to `ApplyPaletteExt(u16 *, u32, u16)` for 0x20 bytes -- one
 * palette. Non-const for the same -Werror reason the other two families here
 * carry: both prototypes take non-const pointers. */
extern u8 gUnknown_08235558[];
extern u8 gUnknown_082352FC[];
extern u16 gUnknown_08235D10[];
extern u16 gUnknown_082391E8[];
extern u16 gUnknown_08239DE4[];
extern u8 gUnknown_080A29A4[];
/* Wave 39 (W39-A): retyped u8 -> u16. sub_08012B70 is its ONLY reader and the
 * only thing any caller does with it (c_080399F8.c, c_0807FE90.c,
 * c_08080498.c all just hand it over), and that function walks it as
 * halfwords: `ldrh` for the first word, then `ldrh`/`adds #2` through the
 * body. The leading halfword is a size header -- low byte width, high byte
 * height -- and the rest is one halfword per cell. Byte-neutral at all three
 * call sites, which pass only the base address; all three re-verified. */
extern u16 gUnknown_080A31A4[];
/* Wave 34 (W34-H). A RUN of 16-colour palettes, not a single one: sub_08039948
 * addresses it `lsls #5; adds` off the bare symbol with proc->unk54 as the
 * index and hands the element to ApplyPaletteExt(.., 0x100, 0x20), i.e. 0x20
 * BYTES per entry. `u16 []` because that is ApplyPaletteExt's first parameter,
 * which makes the index `* 0x10` in ELEMENTS -- the same reasoning as
 * gUnknown_081213F4. gUnknown_080A36C8 just below is this run's ENTRY 1
 * (0x080A36A8 + 0x20), carved under its own name by whichever function reads
 * that entry by itself; the two are not independent objects. Non-const:
 * ApplyPaletteExt takes a plain pointer. */
extern u16 gUnknown_080A36A8[];
/* Wave 34 (W34-H): sub_08039A5C's second tile blob, handed to
 * `Decompress(u8 *, void *)` by name with no arithmetic, so `u8 []` and
 * non-const -- Decompress takes a plain pointer. */
extern u8 gUnknown_080A534C[];
extern u16 gUnknown_080A36C8[];
extern const struct ProcCmd gUnknown_086168BC[];
/* Read once with `ldr` and handed straight to sub_08043BA4(int, ...) by
 * sub_08080498. `int` is the weakest model that the single use supports -- it
 * is never dereferenced here, so pointer-ness is unproved. */
extern int gUnknown_03005970;
/* Wave 30, W30-D. sub_08080E74 writes gUnknown_03005970 and gUnknown_03005904
 * with a plain `str` from its first two parameters and then starts
 * gUnknown_08616794 or gUnknown_08616844 depending on which of 1 / 2 the second
 * one is. Word-sized from the `str`; `int` for the same weakest-model reason
 * gUnknown_03005970 above carries -- neither is dereferenced anywhere yet. */
extern int gUnknown_03005904;
/* Wave 30, W30-D. The three proc scripts of the 0x08080xxx block.
 * gUnknown_08616794 / gUnknown_08616844 are the pair sub_08080E74 starts with
 * Proc_StartBlocking and sub_08080EB0 queries with Proc_Find -- an install /
 * query pair naming the same two symbols, which is what fixes them as scripts
 * rather than as blobs. gUnknown_086167EC is Proc_Find'd by sub_0808006C.
 * All three decode as ProcCmd triples in the ROM (086167EC opens
 * {0x1000e, 0, 2}; 08616794 and 08616844 both open {2, 0x8034f7d, ...},
 * a code pointer in the dataPtr slot). */
extern const struct ProcCmd gUnknown_086167EC[];
/* Wave 51, W51-G. A fourth script of the same block, and the one that fixes
 * gUnknown_086167EC's role: sub_0807FCF8 ends its animation by
 * `Proc_Start(gUnknown_08616814, PROC_TREE_3); Proc_BreakEach(gUnknown_086167EC);`
 * -- a start / break-each pair naming both symbols in one statement pair, the
 * same kind of evidence W30-D used. Proc_Start's first parameter is what fixes
 * it as a script, and the ROM bears it out: the words at 0x08616814 decode as
 * ProcCmd pairs {0x11, 0}, {3, 0x0808006D}, {9, 0x08616794}, {2, 0x08080095} --
 * two THUMB code pointers (T bit set) in dataPtr slots and a reference to the
 * already-declared gUnknown_08616794. The symbol is reached by VALUE (`ldr r0,
 * =gUnknown_08616814; bl Proc_Start`, one level), so it is a real object and not
 * a -fforce-addr word; 0x08616814 is not bound by aw2bhr.lds. */
extern const struct ProcCmd gUnknown_08616814[];
/* Wave 34, W34-E. Proc_Start'd by sub_0806AF44 with the caller's own proc as
 * parent; the child's +0x2c/0x30/0x34/0x38/0x39 are filled in immediately
 * afterwards, so this script's proc is at least 0x3a bytes. */
extern const struct ProcCmd gUnknown_085818F4[];
extern const struct ProcCmd gUnknown_08616794[];
extern const struct ProcCmd gUnknown_08616844[];
/* Wave 33, W33-E. Three more scripts of the same 0x08616xxx block, each reached
 * only by `Proc_Start(script, proc)`: 086167BC from sub_0807FA34 (started
 * alongside gUnknown_086167EC on the same proc) and 0861693C from sub_080806B0.
 * Proc_Start's first parameter is what fixes them as scripts. */
extern const struct ProcCmd gUnknown_086167BC[];
extern const struct ProcCmd gUnknown_0861693C[];
/* Wave 33, W33-E. NOT a script despite sitting between two of them: it is
 * sub_0807F8FC's second parameter, and that function walks it with
 * `adds r1, r7, r2; ldrb r1, [r1]` one byte at a time until a zero byte, so it
 * is a byte string. Non-const because sub_0807F8FC's parameter is `u8 *` --
 * both call sites (sub_0807FA88, sub_080805E0) pass it by name with no cast. */
extern u8 gUnknown_08616750[];
/* Wave 33, W33-E. A pair of ROM words holding POINTERS to u16 counters in RAM:
 * sub_0807FF78 and sub_0807FFF0 each do `ldr r2, [r0]; ldrh r1, [r2]` and then
 * store back through the same pointer, so the ROM word is the pointer and the
 * pointee is a halfword. 081D937C counts UP to 0x10 (sub_0807FF78) and 081D9380
 * counts DOWN to 0 (sub_0807FFF0), each moving gUnknown_03002B28 the other way
 * -- a blend-coefficient fade in and its mirror.
 *
 * The POINTEE is VOLATILE, and that is measured, not decorative: both functions
 * compare the halfword and then immediately increment it, and the ROM RE-loads
 * it (`ldrh r1,[r2]; cmp; ... ldrh r0,[r2]; adds #1; strh r0,[r2]`) where a
 * plain u16 lets GCC forward the compared value into `adds r0,r2,#1`. Measured
 * wave 33: non-volatile is -4 bytes on sub_0807FFF0 and +8 on sub_0807FF78 (the
 * forwarding frees a register, which changes the address allocation with it);
 * volatile is byte-exact on both. Same tell as gUnknown_03001FFC in hardware.h.
 * The POINTER itself is not volatile -- it is reloaded after the merge only
 * because the store through it may alias. */
extern volatile u16 *gUnknown_081D937C;
extern volatile u16 *gUnknown_081D9380;
/* Wave 34, W34-E. A POINTER cell in the same 0x081D9xxx run as the two above,
 * reached the same way: `ldr rN, =gUnknown_081D940C; ldr rM, [rN]; ldrb
 * [rM, #1]`. sub_08085F94 reads unk01 twice and compares it against 2 to pick
 * between the gUnknown_03005900/30 pair and the gUnknown_03005990/80 pair (the
 * note on gUnknown_03005900 describes that choice). Only +0x01 is pinned down;
 * `u8 filler_00[1]` holds the offset without claiming what is at +0x00. */
struct Unk081D940C
{
    /* 0x00 */ u8 filler_00[0x01];
    /* 0x01 */ u8 unk01;
};
extern struct Unk081D940C *gUnknown_081D940C;
/* Wave 30, W30-D. An EWRAM byte array indexed by gUnknown_084C30F8->unk01e in
 * sub_0804931C, whose element goes straight into sub_080487B4's fourth
 * parameter. `u8 []` from the `ldrb`; nothing writes it in matched code yet, so
 * neither the extent nor const-ness is proved. */
extern u8 gUnknown_02028E1C[];
/* Wave 34, W34-E. A 4-byte-stride EWRAM table -- sub_0806AF44 indexes it with a
 * proc's u16 field (`lsls #2`) and reads a u16 at offset 0, which it then uses
 * as the `muls #0x5c` row index into gUnknown_085C77A0[]. So each entry maps a
 * slot to a chapter/unit row. The address is NOT a pool word: aw2bhr.lds binds
 * `gUnknown_0202F214` at 0x02F214 already, this declaration only gives it a
 * type. Stride 4 is proved by the shift; only +0x00 is read anywhere matched so
 * far, so unk02 is a placeholder holding the stride rather than a claim. */
struct Unk0202F214 /* 0x04 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02; /* Wave 48 (W48-C): no longer just a stride
                           * placeholder -- sub_0806AD04 is the first reader of
                           * +0x02 and reads it TWICE, as two fields packed in
                           * one halfword: the low 2 bits are a mode it stores
                           * into its child proc's `int` +0x4c, and `unk02 >> 2`
                           * is a count it renders as three decimal digits.
                           *   TYPE DELIBERATELY UNCHANGED HERE, but wave 73
                           * (W73-C) corrected WHY, and the old wording was
                           * wrong. The two reads are `ldrb [.,#2]; lsls #0x1e;
                           * lsrs #0x1e` and `ldrh [.,#2]; lsrs #2`, which is
                           * exactly what a `u16 lo : 2; u16 hi : 14;` pair
                           * looks like -- and the field split REALLY IS that
                           * pair. sub_0806B120, the writer, MATCHED in wave 73
                           * with those bitfields; its two stores are textbook
                           * `store_bit_field`, and nothing else reproduces
                           * either the full-word `~3` mask (`movs #4; rsbs #0`)
                           * or the register pressure that mask creates.
                           *   What is ruled out is only putting the bitfields
                           * in THIS SHARED DECLARATION, because a bitfield READ
                           * widens to the enclosing word (`ldr`) and would
                           * break sub_0806AD04's `ldrb [.,#2]`. The resolution
                           * is that a bitfield view can be LOCAL TO THE WRITER:
                           * sub_0806B120 declares its own
                           * `struct Unk0202F214Rec` and reaches it by a cast,
                           * so this type is untouched and both functions match.
                           * Same 4-byte size and alignment either way, so the
                           * `lsls #2` index is unaffected -- the 4-to-8-byte
                           * inflation applies to a UNION carrying a bitfield
                           * struct, not to a plain local view. See the
                           * Bitfields chapter of docs/agbcc-codegen.md.
                           *   So the halfword is genuine and the narrowing
                           * lives at the USE: sub_0806AD04 spells the 2-bit
                           * read `(u32)(u8)e->unk02 << 30 >> 30`, which is
                           * byte-exact. Note `& 3` is NOT it -- that gives
                           * `movs #3; ands`. Signedness still unproved: both
                           * reads are unsigned-shaped, but nothing writes it in
                           * matched code. */
};
extern struct Unk0202F214 gUnknown_0202F214[];
/* Proc scripts handed to sub_080152EC(script, 3). */
extern const u8 gUnknown_08580CB4[];
extern const u8 gUnknown_08580CC4[];
/* sub_08073304's first argument -- the sibling of gUnknown_085802A4, which
 * sub_08065238 passes in the same position with the same 0x230 third argument. */
extern const u8 gUnknown_085802AC[];
/* Wave 46 (W46-F): the gUnknown_085802AC above had been described from
 * sub_08065238's call site before this symbol itself was declared. It is
 * sub_08073304's `const void *` first argument in sub_08065238, sub_0806530C
 * and sub_0806540C, all three with the identical (0x230, 0xf, 1, 1, 3) tail --
 * one shared ROM blob, not three. Element type copied from the sibling; only
 * "address of a const ROM object" is proved. */
extern const u8 gUnknown_085802A4[];
/* A word-sized row/line counter. sub_080876B4 is the only reader so far and
 * uses it purely arithmetically -- `gUnknown_03005928 * 16 + 0x28` (or +0x48),
 * stored into the u8 gUnknown_030020B8 -- so the 4-byte `ldr` fixes the width
 * and nothing yet fixes the sign. */
extern u32 gUnknown_03005928;

/* ---- wave 13 (A8), third block ----
 * 0x0808D800 -- one more slot of the `-fforce-addr` .rodata address-constant
 * block described on gUnknown_0808D6DC above, holding &gActiveMap.
 * NOT a global of the original source; the original says gActiveMap and
 * agbcc parked the address here.
 *
 * NON-const, and that is the whole difference between a match and a near-miss:
 * sub_080085E0 re-reads the ROM word AND the pointer at every use
 * (`ldr r0,[r4]; ldr r0,[r0]`) while keeping only the word's ADDRESS in r4.
 * A `**const` spelling -- the one gUnknown_0808D6DC needs, because that
 * function's reference lives across a loop -- lets agbcc CSE the outer read
 * into a register and drops one `ldr` per use site. Probed both ways in one
 * call against the target listing. */
extern struct ActiveMap **gUnknown_0808D800;

/* ---- wave 13 (A6) ----
 * The four-direction step tables, read together and indexed by the same 0..3
 * direction in sub_0800F564 (`lsls r3, r6, #1` is CSEd across both):
 *   gUnknown_0848894C = { -1, +1,  0,  0 }   dx
 *   gUnknown_08488954 = {  0,  0, -1, +1 }   dy
 * SIGNED and 16 bits wide -- both are read with `ldrsh rN, [base, rIdx]`, which
 * is the only THUMB form for a signed halfword and needs the scratch zero in a
 * register; the values in the ROM confirm the sign. Extent is 4 from the four
 * `dir` cases the one reader tests; the words after them at 0x08488958 repeat
 * the same two patterns, so this is one row of a longer table and the bound is
 * not proved. */
extern const s16 gUnknown_0848894C[];
extern const s16 gUnknown_08488954[];

/* ---- wave 13 (A7) ----
 * The debug status readout sub_08057464 (matched) paints. Every type below is
 * pinned by that one function and nothing else yet.
 *
 * The 0813617x-0813619C run is ASCII, not a compiler pool: the addresses are
 * handed to sub_080119A0 as its third argument and never dereferenced by the
 * caller, and the bytes read "C", "ATK", "ARMY", "SOLD", "WEP", "TER", "DEF".
 * Contrast the 08136050-081360E0 run just above them in data/data.s, which
 * holds RAM addresses, is loaded THROUGH (`ldr rN, [rM]`) and is agbcc's own
 * -fforce-addr constant pool -- see docs/agbcc-codegen.md.
 *
 * 08551A48/4C/50 are `u16 []`: indexed `* 2` and read with a bare `ldrh` whose
 * result goes straight into a `u16` parameter. The five 08551A60.. tables are
 * indexed `* 2 * 2` and read with `ldr`, and what comes out is sub_080119A0's
 * string argument, so they are pointer tables. 03004550 is `s16 []` -- every
 * one of its sixteen slots is read with `ldrsh`, including the eight that feed
 * sub_08011A20's unnarrowed `int` third parameter. */
extern u16 gUnknown_03004514;
extern u16 gUnknown_03004524;
/* Wave 37, W37-B. One halfword per gUnknown_0300453C side, `strh` of the
 * constant 1 by sub_08054500 off a bare `i * 2` from the symbol -- so the
 * symbol IS the array. Nothing matched reads it back, so the `strh` is the
 * whole evidence for the width and the signedness is unconstrained. */
extern u16 gUnknown_03004548[];
extern u16 gUnknown_03004540;
/* Wave 50, W50-F: RESHAPED from a flat [] to [][8]. sub_08052F84 subscripts it
 * as `[gUnknown_03004514][gUnknown_03004524]` with a `lsls #4` row term and a
 * `lsls #1` column term -- the flat spelling would have to add before shifting
 * and cannot produce that pair -- and then copies it element-for-element into
 * gUnknown_03004580, which is already `u16 [][8]`, with two nested loops
 * bounded at 2 and 8. The 16-byte row stride was already recorded below.
 * The reshape is byte-neutral: its one promoted reader, src/decomp/c_08057464.c,
 * used constant subscripts 0..15 and now uses [0][0]..[1][7] (re-verified). */
extern s16 gUnknown_03004550[][8];
/* Wave 50, W50-F. The per-column CEILING for gUnknown_03004550, same 2 x 8
 * shape: sub_08052F84 reaches it with the SAME byte offset it just used for
 * gUnknown_03004550 (one `adds r1, r1, base` off the shared index), wraps the
 * value past it back to 0 and wraps a negative value back up to it. 0x20 bytes
 * in data/data-0848B688.s, which is exactly 2 x 8 halfwords. Read `ldrh`. */
extern const u16 gUnknown_08551A28[][8];
/* Wave 45, W45-H. sub_08052F3C's ROM initialiser: it copies 2 x 8 halfwords
 * from here into gUnknown_03004550 with `ldrh`/`strh` and a 16-byte row stride,
 * so the element type is inherited from that already-typed destination. Extent
 * 16 is what the copy touches, not a proved bound. */
extern const s16 gUnknown_08551A08[];
extern u16 gUnknown_030045AC;
extern const u16 gUnknown_08551A48[];
extern const u16 gUnknown_08551A4C[];
extern const u16 gUnknown_08551A50[];
extern const char *const gUnknown_08551A60[];
extern const char *const gUnknown_08551A74[];
extern const char *const gUnknown_08551AD4[];
extern const char *const gUnknown_08551AE0[];
extern const char *const gUnknown_08551B8C[];
extern const char *const gUnknown_08551B98[];
extern const char gUnknown_08136170[];
extern const char gUnknown_08136174[];
extern const char gUnknown_08136178[];
extern const char gUnknown_08136180[];
extern const char gUnknown_08136188[];
extern const char gUnknown_0813618C[];
extern const char gUnknown_08136190[];
extern const char gUnknown_08136194[];
extern const char gUnknown_08136198[];
extern const char gUnknown_0813619C[];

/* Two free-running frame counters, each bumped once per call by sub_08021DD8
 * and immediately re-read for a `% 0x70` / `% 0x64` phase test. VOLATILE, and
 * that is measured rather than assumed: without it CSE satisfies the modulo's
 * operand from the register the increment just stored and the `ldr rN, [rM]`
 * after the `str` disappears, costing two bytes each. Nothing between the
 * store and the load can alias them, so `volatile` is the only reason agbcc
 * would reload. */
extern volatile u32 gUnknown_03004078;
extern volatile u32 gUnknown_030043F0;
/* WORD-WRITTEN, HALFWORD-READ -- declared u32 because the writers are what is
 * matched, and a u16 declaration would give them `strh`. Every one of the
 * eleven writers is `str` of a value the prologue narrowed with
 * `lsls #24; lsrs #24`, i.e. a u8 parameter widened to a word (family F045:
 * sub_0802C5B8, sub_0802C5D4, sub_0802C7DC, sub_0802CD28, sub_0802CE54,
 * sub_0802CE94 and neighbours), plus one `str r4` zeroing pair in sub_08021554
 * and sub_08034810. The only two READERS, sub_0802D4A0's body at 0x0802D4D8
 * and its twin at 0x0802D52C, do `ldrh` and pass the result as the fifth
 * (stacked) argument of sub_08019F2C(menuDef, ...) -- so whoever matches those
 * two needs a `(u16)` cast or a union here, NOT a narrower declaration.
 * The value is a menu-item selector: the tables handed to sub_08019F2C
 * alongside it are gUnknown_0849AC60 and gUnknown_0849ABC0, 0x20-byte records
 * whose +0x14/+0x18 callback slots hold the F045 functions themselves.
 * Wave 14. */
extern u32 gUnknown_030040F0;
/* The byte source sub_080344F0 packs into the command block: it copies [0],
 * [1] and [2] into the block's +7, +0x0c and +0x0d, then runs a four-iteration
 * pointer loop from [4] into +0x0e. Plain `ldrb` throughout and the loop
 * indexes it with a variable, so it is a u8 array at least 8 long. */
extern u8 gUnknown_03004490[];
extern u32 gUnknown_030044A0;
/* NOT OBJECTS -- these two are THUMB FUNCTION POINTER VALUES, and they are
 * here only because a `.c` may not declare its own externs.
 *
 * Both appear exactly once, as the pool word of a `ldr r1,=X; bl _call_via_r1`
 * indirect call (sub_0801B6EC and sub_0801B6FC). The addresses are ODD, which
 * is what a stored THUMB entry point looks like, and they are in IWRAM. No
 * object is allocated at either: aw2bhr.lds defines them because the SPLITTER
 * saw the pool word, and aw2bhr.map shows *fill* across the range. So the
 * original source had a constant or a macro, and `&gUnknown_0300619D` is a
 * spelling forced by the relocation the split produces -- see the comment in
 * work/sub_0801B6EC/. Do not read these as evidence that IWRAM holds a
 * variable at an odd address, and do not give them a wider type: the only
 * legal use is taking the address and casting it to a function pointer.
 * The pointee is code copied into IWRAM at run time; nothing in the ROM's
 * .text writes either address, so what lands there is not yet identified.
 * Wave 14. */
extern u8 gUnknown_0300619D;
extern u8 gUnknown_03006511;

/* Three more blobs of the 0817D874 group, all handed to sub_080718F8 as its
 * second argument by sub_08068BE4 (matched) exactly as 0817D874 is by its
 * sibling. Same `u8 []` model and the same reason for not being const --
 * sub_080718F8's declared second parameter is a plain `u8 *`.
 *
 * gUnknown_08581414 is SIGNED bytes and that is proved, not assumed: both
 * elements sub_08068BE4 reads end up in a `u16` argument, and the ROM
 * sign-extends each one to 32 bits first (`ldrb; lsls #24; asrs #24` for the
 * first, a bare `ldrsb` for the second) before re-narrowing to 16. A `u8 []`
 * would have gone straight from `ldrb` to the argument with no extension at
 * all. */
extern u8 gUnknown_0817D6FC[];
extern u8 gUnknown_0817D7B8[];
extern u8 gUnknown_0817D910[];
extern s8 gUnknown_08581414[];

/* Two adjacent halfword tables read by sub_0802216C. gUnknown_0809097C is a
 * base tile id per terrain class, indexed by `(v >> 6) + 1` with slot 0 used
 * for the v == 0x100 case, so at least 5 entries -- which is exactly the 10
 * bytes between it and gUnknown_08090986. gUnknown_08090986 is SIGNED: its
 * only use is `movs r2,#0; ldrsh r0,[r1,r2]; cmp r0,#0; bge`, a sign test that
 * a u16 table could not produce. */
extern const u16 gUnknown_0809097C[];
extern const s16 gUnknown_08090986[];

/* The 0x0804CEF8 unit-graphics loader's tables. Every type here is proved by a
 * candidate that compiles to bytes IDENTICAL to the ROM (see the wave-13 A7
 * report); only the relocation of agbcc's own .rodata pool word blocks that
 * function, so the layouts are as solid as a match.
 *
 * 08552178 is 10-byte rows -- `(i*4 + i) * 2` -- with u16 at +4 and +6, the
 * same shape as the RAM table gUnknown_02029668 declared above, and the two are
 * indexed in lockstep. The three 0855797x tables are 12-byte rows of three
 * words each (`(i*2 + i) * 4`): column 0 is a Decompress source, columns 1 and
 * 2 are sub_08015410's third and fourth arguments. 08552FB0 is a plain word
 * table indexed `i * 4` whose element is Decompress's destination.
 * 085533E4/085533FC/08553414 are 0x18 apart and are only ever named as
 * sub_08015410's first argument.
 *
 * WHAT IS IN THEM, dereferenced in baserom.gba in wave 17, because "opaque
 * blob" was hiding something useful. All five of these (the three above plus
 * gUnknown_085536A4 and gUnknown_085536BC below) are the SAME uniform 0x18-byte
 * record, and it is a table of CALLBACKS:
 *
 *   +0x00  an odd (THUMB) function pointer     +0x04  0x00020000
 *   +0x08  an odd (THUMB) function pointer     +0x0c  0x00010000
 *   +0x10  0x00000000  (a null terminator)     +0x14  0x00040000
 *
 * and the functions named are exactly the sprite setters in the surrounding
 * address range: 085533E4 -> sub_0804D818, 085533FC -> sub_0804D738,
 * 08553414 -> sub_0804D928 (matched), all three sharing sub_0804D8F8 in the
 * second slot; 085536A4 -> sub_08051DE0 (matched) and sub_08051F48;
 * 085536BC -> sub_08052154 and sub_08052270. So sub_08015410's first argument
 * is an animation descriptor, not a graphics blob, and these five records are
 * the reason those setters have no `bl` caller anywhere in `asm/`.
 * Declared `u8 []` anyway -- only the ADDRESS is ever used, the two u32s per
 * entry have no reader, and the record shape above is read off the data rather
 * than off any code. */
/* Wave 36, W36-C. A ROM halfword per script step, read
 * `gUnknown_08551E12[gUnknown_02029C00[i]]` and compared for equality against
 * the u16 gUnknown_03004508 -- the gate that decides whether sub_08053FBC
 * retires the command the cursor is sitting on. u16 from the `ldrh` and from
 * the unsigned compare against a u16 global. */
extern u16 gUnknown_08551E12[];
/* Wave 37, W37-B. The two ROM dispatch tables of the 0x08054xxx cutscene
 * player, one per channel: sub_08054278 and sub_08054488 pick a row with
 * `gUnknown_085D6A48[gUnknown_03004580[i][1]][2]`, load the word with `lsls #2`
 * and call it `bl _call_via_r2` with (side, command) already in r0/r1 -- the r2
 * index counts the TWO arguments here, and both are values the caller already
 * holds. Nothing narrows either at the call, so the parameter types are the
 * callers' own u16s. */
extern void (*const gUnknown_08553744[])(u16, u16);
extern void (*const gUnknown_0855374C[])(u16, u16);
/* Wave 37, W37-B. A halfword table sub_08054500 reads with
 * `gUnknown_02029A10[i].entries[j].unk00 * 2 + .unk01` -- the FACTORED index
 * (`(a*2+b)*2`), which is the flat-array form; a `[][2]` spelling would scale
 * the two subscripts separately. Only `== 1` is ever tested. u16 from the
 * `ldrh`. Extent unproved. */
extern u16 gUnknown_08553838[];
/* Wave 54, W54-G. A halfword remap table: sub_080553C8 rewrites both sides'
 * gUnknown_03004580[i][0] through it in place -- `ldrh; lsls #1; adds; ldrh;
 * strh` -- so the index and the element are both the same u16 the row holds.
 * `lsls #1` is the whole width evidence and the extent is unproved. It sits
 * 0x0e bytes into gUnknown_08553838's range above; the two are separate linker
 * symbols and the ROM references this one by its own address, so whether they
 * are one object is not settled here. */
extern u16 gUnknown_08553846[];
/* Wave 37, W37-B. Two CpuFastSet SOURCE blobs for the cutscene player's OBJ
 * VRAM at 0x06010000: sub_08054EE0 copies 0x288 words of gUnknown_08540FBC and
 * sub_08054F50 copies 0x348 words of gUnknown_0854E67C, both to
 * `0x06010000 + (u16)(side * 0x2000 + 0xa00)`. Only the symbol address is used,
 * so the element type is the weakest thing CpuFastSet's `const void *` accepts;
 * the word counts are transfer lengths, not proved extents. */
extern const u8 gUnknown_08540FBC[];

/* Wave 45 (W45-G).  gUnknown_08540FBC's opposite number for the third of the
 * three sub_08054E8C arms: sub_08055004 copies 0x2C8 words of it to
 * 0x06010000 + a1 * 0x2000 + 0x1400, exactly as sub_08054EE0 copies 0x288
 * words of gUnknown_08540FBC to the +0xA00 slot of the same block.  ROM, and
 * only ever a CpuFastSet source, so `const u8 []` for the same reason -- the
 * element type is unconstrained and the array form is what makes the bare
 * name the address. */
extern const u8 gUnknown_0855089C[];
extern const u8 gUnknown_0854E67C[];
/* Wave 37, W37-B. A ROM table of 20-byte rows of words, reached at a FIXED
 * +0x68 into the object: sub_08054F50 loads
 * `gUnknown_08553D80.unk68[gUnknown_03004580[i][0]][0]` as one pool word plus a
 * run-time `adds rB,#0x68`, with the row's `* 20` added after it and a ZERO
 * `ldr` displacement -- the struct-member-array form. The word is handed
 * straight to CpuFastSet as a source, hence `const void *`. Both extents are
 * unproved: one column of one row has a reader, and the 0x68 is a constant in
 * the address chain rather than a proved member boundary. */
/* Wave 51, W51-M. What filler_00 holds: ROWS OF FIVE 20-byte {length, source}
 * records, exactly the shape gUnknown_085D70A8 and gUnknown_085D7768 carry for
 * sub_0805521C / sub_0805530C. sub_08055288 subscripts them TWO-DIMENSIONALLY
 * -- a 100-byte outer stride off gUnknown_085D6A48[..][10] and a 20-byte inner
 * stride -- and reads +0 as a CpuFastSet length and +4 as its source, with the
 * `adds rB,#4` on the pool word that only the struct-member spelling produces.
 *
 * This CONFIRMS unk68 rather than competing with it: 0x68 == 4 + 5 * 20, i.e.
 * unk68[k][0] is row 1 column k's unk04, the same CpuFastSet source column read
 * with a flat index. unk68 is left exactly where it is -- src/decomp/c_08054E8C.c
 * reads it and reproducing that function's flat `adds rB,#0x68` through the 2D
 * member would take a `/5` and a `%5` it does not have.
 *
 * The outer extent 1 is a FLOOR, not a bound: sub_08055288's subscript is a
 * run-time table value and the rows plainly continue into unk68's territory.
 * It is written [1][5] so the member stops at 0x64 and nothing below moves. */
struct Unk08553D80Row /* 0x14 */
{
    /* 0x00 */ u16 unk00;      /* CpuFastSet length in BYTES: divided by 4 for
                                * the word count and by 0x20 for the tile
                                * advance, and only ever tested against 0. */
    /* 0x02 */ u8 filler_02[0x02];
    /* 0x04 */ const void *unk04;
    /* 0x08 */ const void *unk08; /* Wave 60, W60-H. sub_080566C8 reads unk08
                                   * and unk0c with whole-word `ldr`s off
                                   * `unk00[gUnknown_085D6A48[..][10]][col]`
                                   * (a `* 100` outer stride and a `* 20` inner
                                   * one) and stores them straight into
                                   * gUnknown_02029710[i].unk08 and .unk04,
                                   * which are already `void *` -- the same
                                   * graphics/palette pair gUnknown_02029700's
                                   * two columns supply. Carved out of
                                   * filler_08; unk00/unk04 did not move. */
    /* 0x0c */ const void *unk0c;
    /* 0x10 */ u8 filler_10[0x04];
};

struct Unk08553D80
{
    /* 0x00 */ struct Unk08553D80Row unk00[1][5];
    /* 0x64 */ u8 filler_64[0x04];
    /* 0x68 */ const void *unk68[8][5];
};
extern const struct Unk08553D80 gUnknown_08553D80;
extern u16 gUnknown_08552148[];
/* Wave 54, W54-H. TEN 4-byte cells whose low halfword is the only part read --
 * a slot index, exactly the gUnknown_0855218C / gUnknown_085521B4 shape.
 * sub_08055768 and sub_08055A38 read it `ldrh` at a FLAT `(side * 5 + i) * 4`
 * (the `side * 5` is CSEd with the `side * 40` row of gUnknown_020296BC in the
 * same loop, which is what rules out a `[2][5][2]` spelling -- that one splits
 * the index into `side * 40 + i * 4`), and the value subscripts
 * gUnknown_02029A10[side].entries. The extent is PROVED by the addresses:
 * 0x08552178 - 0x08552150 = 0x28 = 2 * 5 * 4, so it tiles exactly up to
 * gUnknown_08552178. Written [][2] because only the low halfword of each cell
 * is ever touched. */
extern u16 gUnknown_08552150[][2];
/* Wave 50, W50-F. A 0x50-byte ROM table (data/data-0848B688.s) of TWENTY THUMB
 * function pointers -- every word is odd and resolves to one of six routines:
 * sub_08052E04, sub_08051BEC, sub_08052718, sub_08051F4C, sub_08052BBC,
 * sub_080523E8. sub_080536D8 subscripts it with a `lsls #2` and calls the
 * result through `bl _call_via_ip` with three arguments, and sub_08052E04 is
 * already declared `void (u16, u16, int)`, which is the signature the whole
 * table takes -- three of the six others are the known callers of
 * sub_08052818, so the table is the per-slot action dispatch for one side. */
extern void (*const gUnknown_085535C0[])(u16, u16, int);
/* Wave 49, W49-M, block 0x08056. Two ADJACENT ROM tables of 0x28 bytes each --
 * 0x0855218C..0x085521B4 and 0x085521B4..0x085521DC -- holding a slot
 * PERMUTATION per side: 0,1,3,2,4 / 1,0,2,4,3 for the first and 0,1,3,4,2 /
 * 1,0,2,4,3 for the second, each value in a 4-byte cell whose upper halfword
 * is zero. sub_0805653C and sub_080560A4 read the low halfword `ldrh` at
 * `side * 20 + i * 4` and use it to subscript gUnknown_02029A10[side].entries,
 * so the elements are u16 with a 4-byte stride; 0x28 == 2 * 5 * 4 fixes both
 * extents exactly.
 *   The [5][2] spelling rather than a flat [10] is what produces the ROM's
 * address chain: `side * 20` expands as `(side * 4 + side) * 4` and the
 * intermediate `side * 5` is CSEd with gUnknown_02029C14[side]'s own
 * `side * 5` in sub_080560A4 -- a flat u16 table would have had no such
 * intermediate. Both are indexed by a computed subscript before any
 * dereference, so neither is a -fforce-addr pool word. */
extern const u16 gUnknown_0855218C[][5][2];
extern const u16 gUnknown_085521B4[][5][2];
/* Wave 49, W49-M. A ROM table of 4-byte rows read at a FIXED +400 halfwords:
 * sub_080560A4 binds `gUnknown_08554A00[side * 5 + slot]` to a pointer local
 * (the ROM spills that address to the stack and reloads it) and then reads
 * [200] and [201] off it into gUnknown_02029A10[side].entries[slot].x and .y
 * in consecutive statements. The +400/+402 are built as `movs #0xc8/#0xc9;
 * lsls #1`, i.e. halfword indices, and the pair at 0x08554B90 onwards reads
 * 63,107 / 51,129 / 28,92 / 8,115 / -4,149 -- plausible screen coordinates.
 * The row width and the 0xa50 extent are unproved; only the 4-byte stride and
 * the two columns 400 halfwords in are. */
extern const u16 gUnknown_08554A00[][2];
/* Wave 37, W37-J. One ROM halfword per (gUnknown_02029664 bit0 + bit3) sum,
 * i.e. an index in 0..2: sub_08050FF8 reads it with a bare `ldrh` off `n * 2`
 * from the symbol and drops the value straight into OamData.priority, so only
 * the low two bits are consumed. Extent unproved. */
extern u16 gUnknown_08552394[];
/* Wave 37, W37-J. A ROM halfword per gUnknown_0300453C side, `ldrh` off a bare
 * `lsls #1` from the symbol; sub_0804FFFC copies it into gUnknown_02029C0C[i]
 * (u16) in the same statement that sets gUnknown_02029C10[i] to 0x96. */
extern u16 gUnknown_0855357C[];
/* Wave 37, W37-J. Two ROM tables 4 bytes apart that sub_08051454 divides
 * element [1] of one by element [1] of the other (`ldrh [rN,#2]` on each, then
 * __udivsi3 -- an UNSIGNED divide, which is what keeps both u16). The quotient
 * becomes gUnknown_020298E0[side].unk8a. gUnknown_08553668 is 4 bytes further
 * on and is a per-(unk16 - 1) y offset added to a sprite coordinate in the same
 * function; all three extents are unproved. */
extern u16 gUnknown_08553660[];
extern u16 gUnknown_08553664[];
extern u16 gUnknown_08553668[];
/* Wave 37, W37-J. A per-side halfword pair, read `gUnknown_08553B58[side]` off
 * a bare `lsls #1`, and the four-halfword rows immediately after it, indexed
 * `gUnknown_08553B5C[gUnknown_085D6A48[..][4]][side]` with an 8-byte row stride
 * (`lsls #3`) and the side term folded into the same index register. Both are x
 * offsets summed into gUnknown_02029A10[..].entries[..].x by sub_08050FF8. The
 * [4] is the measured stride, not a proved extent; the two symbols overlap the
 * way gUnknown_03004580/gUnknown_03004582 do. */
extern u16 gUnknown_08553B58[];
extern u16 gUnknown_08553B5C[][4];
/* Wave 37, W37-J. Eight-byte ROM rows indexed by gUnknown_020298E0[side].unk18
 * (the 0..2 phase counter). sub_08050FF8 and sub_08051454 both read unk04 as
 * `adds rB,rIdx; ldrh [rB,#4]` -- the offset in the LOAD DISPLACEMENT, which is
 * the struct form; a flat `u16 [][4]` folds the +4 onto the symbol instead. The
 * value is a y offset added to gUnknown_02029A10[..].entries[..].y. Nothing
 * reaches the other three halfwords. */
struct Unk08553BFC /* 0x08 */
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
};
extern struct Unk08553BFC gUnknown_08553BFC[];
/* Wave 37, W37-J. A ROM table of 0x24-byte rows, the row indexed by
 * `gUnknown_085D6A48[gUnknown_085D6A48[gUnknown_03004580[i][1]][0]][10]` -- the
 * two-step chain sub_080505A4 and sub_08050F24 already walk. The 36-byte stride
 * is `((m * 8) + m) * 4` at all four sites.
 *   unk08 is a MEMBER ARRAY indexed by gUnknown_0300453C and unk10 is a plain
 * scalar, and that asymmetry is the ROM's: sub_080506B0 reads the first as
 * `adds rB,#8` with the side term in the index register and a zero `ldrh`
 * displacement, and the second as a bare `ldrh [rB,#0x10]` with no side term at
 * all -- the same split struct Unk085D7E28 records. unk16/unk1c are a second
 * (x, y) pair read by sub_08051454, unk16 being the one that gets negated for
 * side 0. All u16 from bare `ldrh`s; the sign appears only at the use, as an
 * explicit `lsls #0x10; asrs #0x10` on the local. */
/* Wave 60, W60-H. sub_080566C8 reads ELEVEN more halfwords of the same 0x24
 * record off the same `((m * 8) + m) * 4` stride, all with bare `ldrh`s and
 * none of them signed by any use. unk02 is a MEMBER ARRAY indexed by the side
 * (`adds rB,#2` with the side term in the index register and a zero `ldrh`
 * displacement -- the same split unk08 records); the rest are plain scalars at
 * fixed displacements. The four pairs unk0c/unk0e, unk12/unk14, unk18/unk1a
 * and unk1e/unk20 are (x, y) offset pairs: the FIRST member of each pair is the
 * one that gets NEGATED (`rsbs r0,r0,#0`) for one of the two sides, exactly as
 * unk16 is in sub_08051454, and they land in gUnknown_02029710's
 * unk26/unk30/unk44/unk4e and gUnknown_020298E0's unk58/unk62/unk6c/unk76
 * rows. All carved out of filler; no existing member moved. */
struct Unk08553C18 /* 0x24 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02[2];
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08[2];
    /* 0x0c */ u16 unk0c;
    /* 0x0e */ u16 unk0e;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u16 unk18;
    /* 0x1a */ u16 unk1a;
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u16 unk1e;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u8 filler_22[0x02];
};
extern struct Unk08553C18 gUnknown_08553C18[];
/* Wave 36, W36-C. Two ROM descriptors whose ADDRESS is used and never
 * dereferenced -- each is sub_08015410's `void *` first argument and nothing
 * else, in sub_080505A4's two arms. `u8 []` for the same reason every other
 * member of that family carries it: the width is unconstrained because no load
 * reaches inside. */
extern u8 gUnknown_08553610[];
extern u8 gUnknown_08553628[];
extern u16 gUnknown_08552178[][5];
/* Wave 33, W33-E. The two ROM tables sub_08050F24 chains: gUnknown_08553640 is
 * a halfword index table read at `gUnknown_03004580[c][2] * row[8]`, and its
 * element then subscripts gUnknown_085535B8, whose WORD element goes straight
 * into sub_08015410's `void *` first parameter -- which is what makes that one
 * an array of pointers rather than of scalars. */
extern u16 gUnknown_08553640[];
extern void *gUnknown_085535B8[];
/* Wave 33, W33-E. An EWRAM pair of word slots per side, stride 8: sub_08050F24
 * loads both off `c * 8` and hands them to sub_08015410's third and fourth
 * `void *` parameters. Pointee unknown -- the word width and the pairing are
 * what the two `ldr`s prove. */
extern void *gUnknown_02029A00[][2];
extern void *gUnknown_08557978[][3];
extern void *gUnknown_085579B4[][3];
extern void *gUnknown_085579F0[][3];
extern void *gUnknown_08552FB0[];
/* Two more of the `void *[3]` graphics/palette/animation record tables the
 * gUnknown_08557978 family is made of, and ONE table reached through two
 * symbols: 0x08557B94 is 0x08557B58 + 0x3c, i.e. row 5 of the same 12-byte
 * rows. sub_0804DB14 uses both in one function -- gUnknown_08557B58[A] for the
 * first sub_08015410 and gUnknown_08557B94[A] for the second -- and indexes
 * 08557B58 a third time by gUnknown_08562128[..] for a CpuFastSet source, so
 * the low rows and the high rows are genuinely separate records rather than
 * one array read at two biases. Columns: [0] a CpuFastSet/Decompress source,
 * [1] and [2] sub_08015410's third and fourth arguments -- the same
 * graphics/palette pair the 08557978 columns supply. */
extern void *gUnknown_08557B58[][3];
extern void *gUnknown_08557B94[][3];
/* Two more animation descriptors of the gUnknown_085533E4 kind: only the
 * ADDRESS is used, each is sub_08015410's first argument, and sub_0804DB14
 * passes 08553444 with the gUnknown_08557B58 record pair and 08553474 with the
 * gUnknown_08557B94 one. `u8 []` for the same reason as that family. */
extern u8 gUnknown_08553444[];
extern u8 gUnknown_08553474[];
extern u8 gUnknown_08562128[];
/* Wave 37, W37-J, from sub_0804FF44 -- the same CpuFastSet-source family as
 * gUnknown_08557B58 above, but a flat pointer table rather than 3-column rows:
 * `gUnknown_08552138[gUnknown_08562128[..]]` is `lsls #2` plus a word `ldr`
 * handed straight to CpuFastSet. baserom.gba holds 08504FD8, 085051D8,
 * 085053D8, 085055D8 at 0x08552138 -- four sources 0x200 apart -- and
 * gUnknown_08552148, already declared, starts immediately after, so the extent
 * is four. */
extern void *gUnknown_08552138[];
/* Wave 60, W60-H. An 8-byte-row ROM matrix sub_080566C8 reads BOTH WAYS ROUND
 * in consecutive statements -- `gUnknown_08552118[e[0]][e[1]]` and
 * `[e[1]][e[0]]` -- so it is a per-(side, side) lookup keyed twice by the same
 * pair. The result is the byte it writes to gUnknown_020296B0[side].unk1c (the
 * gUnknown_08553B1C row selector) and hands to sub_08056D70's second
 * parameter. `(a * 8) + b * 2` at both sites, so four halfwords per row. Bare
 * `ldrh`; nothing signs it and only two columns are witnessed. */
extern u16 gUnknown_08552118[][4];
/* Wave 37, W37-J. Two more of sub_08015410's first/fourth argument pair, from
 * sub_0804FF44, which passes the ADDRESS of each and nothing else.
 * gUnknown_08553580 is an animation descriptor of the gUnknown_08553444 kind
 * (baserom.gba: 080404FD, 00020000, 080500D1, ... -- odd THUMB pointers
 * interleaved with counts), so `u8 []` for the same reason as that family.
 * gUnknown_08559420 is a table of pointers into its own neighbourhood
 * (08559414, 08559418, ...), which is why it is spelled as an array of void *
 * rather than u8 -- but only its base address is ever taken, so the element
 * type is inferred from baserom, not proved by a use. */
extern u8 gUnknown_08553580[];
extern void *gUnknown_08559420[];
/* Wave 34 (W34-A). A ROM table of POINTERS to 3x4 grids of (x, y) halfword
 * pairs, selected by gUnknown_020296B0[side].unk1c. Every stride is read off
 * sub_08050D44's address chain rather than guessed: the row index scales by 48
 * (`x*3` then `<<4`), the column index by 16 (`<<4`) and the innermost index by
 * 4 (`<<2`), so the pointed-to object is `struct Unk08553B1CPt [3][4]` and the
 * two halfwords come out of `ldrh [rN]` / `ldrh [rN,#2]` off one address.
 *
 * The pair is SIGNED on the consumer's evidence and not on the loads: both
 * halves are sign-extended (`lsls #0x10; asrs #0x10`) before being added to
 * gUnknown_02029A10[..].entries[..].x / .y, and sub_08050D44 negates the x half
 * outright for side 1. Nothing in the matched tree writes the table.
 *
 * Wave 36 (W36-C) tested u16 here and it is WORSE, not neutral: both consumers
 * hold each half in a 16-bit local, so the load is `ldrh` either way and the
 * member's signedness is invisible -- but retyping it forces the local to `int`
 * to keep the `ldrh`, and an `int` local makes every `(s16)` cast at a use fold
 * away (measured, sub_08050E08 went 52.8% -> 51.8% and -12 -> -28 bytes). s16
 * members with s16 locals is the only shape that reproduces the ROM's paired
 * sign extensions. Do not retype this. */
/* Wave 36, W36-C. A halfword per gUnknown_0300453C side, read
 * `gUnknown_08553B18[a]` with a bare `ldrh` off `base + a * 2` by sub_08050E08
 * and added to a coordinate taken out of the gUnknown_08553B1C grid when
 * gUnknown_03004580[a][2] == 2. u16 from the load; nothing signs it. The [2]
 * extent is bounded by the address: 0x08553B1C - 0x08553B18 = 4 = 2 * u16, so
 * it tiles exactly up to gUnknown_08553B1C and there is no room for a third. */
extern u16 gUnknown_08553B18[2];
/* WAVE 36 (orchestrator): the tag above was REFERENCED by this extern but its
 * body was never defined anywhere in the tree, so every draft that actually
 * dereferences gUnknown_08553B1C failed to compile with "dereferencing pointer
 * to incomplete type" while its siblings, which only pass the pointer around,
 * compiled and matched. That reads exactly like a bad decompilation and is not
 * one. Members are read off the evidence already recorded above: stride 4, two
 * halfwords at +0 and +2 (`ldrh [rN]` / `ldrh [rN,#2]` off one address), signed
 * because both halves are sign-extended (`lsls #0x10; asrs #0x10`) at every
 * consumer. See the note above for why u16 here is WORSE, not neutral. */
struct Unk08553B1CPt
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
};

extern struct Unk08553B1CPt (*gUnknown_08553B1C[])[3][4];
extern u8 gUnknown_085533E4[];
extern u8 gUnknown_085533FC[];
extern u8 gUnknown_08553414[];
/* Two more blobs of the same kind and reached the same way: sub_08051BEC names
 * 085536A4 and sub_08051F4C names 085536BC, each as sub_08015410's first
 * argument, and nothing else names either. Same `u8 []` model as the three
 * above, for the same reason -- the address is all that is used. */
/* Wave 33, W33-D. gUnknown_085536A4 - 0x18, i.e. the 0x18-byte
 * animation-descriptor record immediately before it, and the sixth of the same
 * family. sub_0805198C is its only reference and hands the ADDRESS to
 * sub_08015410's `void *` first parameter, exactly as sub_08051BEC does with
 * gUnknown_085536A4. Same `u8 []` model, for the same reason. */
extern u8 gUnknown_0855368C[];
extern u8 gUnknown_085536A4[];
extern u8 gUnknown_085536BC[];
/* Three more of the same 0x18-byte animation-descriptor record, each named as
 * sub_08015410's first argument and nothing else, so `u8 []` for the same
 * reason as the family above (wave 20, W20-C).
 *
 * Dereferenced in baserom.gba, the run 085536A4..0855371C is SIX consecutive
 * records and slot 0 / slot 2 of each name a (draw, install) pair of the
 * 0x08052 sprite cluster:
 *
 *   085536A4  sub_08051DE0 (matched) + sub_08051F48   <- sub_08051BEC
 *   085536BC  sub_08052154 (matched) + sub_08052270   <- sub_08051F4C
 *   085536D4  sub_08052154 (matched) + sub_08052358   <- sub_080520B8
 *   085536EC  sub_080524C0 (matched) + sub_08052650
 *   08553704  sub_0805297C (matched) + sub_08052AF4   <- sub_08052718
 *   0855371C  sub_08052CA4 (matched) + sub_08052E00   <- sub_08052BBC
 *
 * sub_08051F48 and sub_08052E00 are 4-byte `bx lr` stubs, so slot 2 is
 * optional; the arrow marks which function passes the descriptor to
 * sub_08015410, read off the assembly. 085536EC has no passer among the
 * matched or wave-20 set.
 *
 * That table IS why sub_08052650 and sub_08052AF4 have zero `bl` callers and
 * why they are byte-identical to each other: one C body, installed from two
 * different descriptors, so the two copies need distinct addresses. A zero
 * fan-in function in a block full of matched siblings is a CALLBACK, not a
 * dead function. */
extern u8 gUnknown_085536D4[];
extern u8 gUnknown_08553704[];
extern u8 gUnknown_0855371C[];
/* Wave 34, W34-B. Two more tables of the gUnknown_08557978 family -- 12-byte
 * rows of three words, `(i*2 + i) * 4` at every site -- and the two
 * animation descriptors that go with them, one pair per loader.
 * sub_0804C4A8 uses 08557680 with 0855333C and sub_0804C99C uses 08557CFC
 * with 0855339C, each in the identical four-step shape: column 0 is
 * Decompress's source, then the descriptor's ADDRESS is sub_08015410's first
 * argument with column 2 as its third and column 1 as its fourth.
 *   The 2-then-1 column order is read off the register assignment (r2 before
 * r3) and is the same in both functions, so it is the call's real order and
 * not an allocation accident. `u8 []` for the descriptors for the same reason
 * as the gUnknown_085533E4 family above -- only the address is ever used. */
extern void *gUnknown_08557680[][3];
extern void *gUnknown_08557CFC[][3];
extern u8 gUnknown_0855333C[];
extern u8 gUnknown_0855339C[];

/* sub_0806EB5C's screen-setup blobs. 0823BDE0 goes to ApplyPaletteExt, the
 * five 081Axxxx/08239FA4/0823A3D4 ones to Decompress, and 081A3E3C is
 * sub_080718F8's ROM-blob argument -- all `u8 []` and none const, for the same
 * Decompress(u8 *, void *) reason as the 0817Dxxxx group above. 085826E0 and
 * 08582C7C are opaque script/table addresses handed to sub_08073304 and
 * sub_08073FF4. 02010C50 is the RAM buffer sub_08073304 writes into.
 *
 * gUnknown_08582764 is a table of 8-byte rows with a u16 at +4, and the STRUCT
 * form is load-bearing rather than cosmetic: `u16 t[][4]` indexed `[i][2]`
 * folds the +4 into the symbol address (`add rB, #4` then the index), while a
 * struct member -- or a row pointer bound to a local -- keeps it as the
 * `ldrh rD, [rB, #4]` displacement the ROM has. See the wave-13 A7 report. */
/* Wave 32 (W32-C) names +0x00: sub_0806F000 reads it with `lsls rI,#3; add
 * rI,base; ldrh` off the same table, i.e. the row-0 halfword. */
struct Unk08582764 /* 0x08 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02; /* Wave 36, W36-K: carved out of filler_02[0x02].
                           * sub_0806F0EC reads it `ldrh` off the x8 index and
                           * both compares it against and assigns it to the
                           * proc's `int` +0x2c, and separately hands it to
                           * sub_0803B524 through an explicit `(s16)` cast
                           * (`lsls #0x10; asrs #0x10`) -- the cast is in the
                           * source, so the field itself is the unsigned
                           * halfword the `ldrh` says it is. Start offset
                           * unchanged, so the carve is byte-neutral. */
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u8 filler_06[0x02];
};

extern u8 gUnknown_0823BDE0[];
extern u8 gUnknown_081A3E28[];
extern u8 gUnknown_081A3E3C[];
extern u8 gUnknown_081A4000[];
extern u8 gUnknown_081A4450[];
/* The gUnknown_08581F7C twin of gUnknown_0202F200 -- same 0/1 mode flag set by
 * the same wrapper pair shape (sub_0806E160 stores 0, sub_0806E17C stores 1,
 * both immediately before `sub_080152EC(gUnknown_08581F7C, 2)`), and the same
 * evidence: five accesses, all bare `ldrb`/`strb` at displacement zero. */
extern u8 gUnknown_0202F2C8;
extern u16 gUnknown_0202F2D8;
extern u8 gUnknown_085826E0[];
extern u8 gUnknown_08582C7C[];
extern struct Unk08582764 gUnknown_08582764[];
/* Wave 32 (W32-C). A table of `u16 *` -- sub_0806F0A0 scales its index by 4,
 * `ldr`s the word and hands it straight to sub_0806F064's `u16 *list`
 * parameter. The index is the low byte of gUnknown_08582764[].unk00's sibling
 * field, so the element count is at most 0x100 and is not measured here.
 * gUnknown_08582C8C is one of the OAM blobs PutSprite takes (`u16 *`). */
extern u16 *gUnknown_0858273C[];
extern u16 gUnknown_08582C8C[];

/* The four saved-state words sub_0806ED7C copies into its proc, and the script
 * it starts afterwards. 0806ED7C is the resume twin of 0806EB5C: where EB5C
 * hard-codes `proc->unk30 = 1` and clears the halfword at 0202F2D8, ED7C loads
 * the same four proc fields out of this run of globals, which sit two bytes
 * apart in address order and match the fields' widths exactly:
 *
 *   0202F2CC  ldr  -> proc->unk2c   also the guard for the trailing Proc_Start
 *   0202F2D0  ldr  -> proc->unk30   and sub_0806F000's first argument
 *   0202F2D4  ldrh -> proc->unk34
 *   0202F2D6  ldrh -> proc->unk36
 *
 * Widths come from the load instruction and are corroborated by the proc
 * fields they feed, which sub_0806E5CC (u16 first parameter) reads back. The
 * two word ones are `u32` rather than a pointer type on the weakest-type rule:
 * 0202F2CC is only ever compared against zero and 0202F2D0 is only masked with
 * 1 and stored, so nothing dereferences either. 0202F2D8, declared above, is
 * the neighbouring halfword EB5C zeroes and ED7C leaves alone -- so this run is
 * 0202F2CC..0202F2D9 and nothing yet reaches 0202F2D2 or 0202F2DA. */
/* sub_0806C52C's two parallel graphics sets -- one per arm of a single
 * `if (IsHardCampaignMode())`. The two arms are the same five calls with five
 * different symbols each, which is what fixes the types: the 081A2xxx/081A3Dxx
 * pair in each arm goes to ApplyPaletteExt so `u16 []`, the other three go to
 * Decompress(u8 *, void *) so `u8 []`, and none can be `const` because neither
 * prototype takes one (the -Werror reason recorded on the 0817Dxxxx group).
 * Sizes are unknown -- only the bare symbol is ever passed. The first
 * ApplyPaletteExt in each arm is (0, 0x200), i.e. ApplyPalettes(src, 0, 16), so
 * those two are at least 16 palettes; the second is ApplyPalette(src, 16). */
extern u16 gUnknown_081A21B4[];
extern u16 gUnknown_081A3D64[];
extern u8 gUnknown_0819C454[];
extern u8 gUnknown_081A1C50[];
extern u8 gUnknown_081A31C4[];
extern u16 gUnknown_0819C254[];
extern u16 gUnknown_081A3D44[];
extern u8 gUnknown_08195318[];
extern u8 gUnknown_0819BCF0[];
extern u8 gUnknown_081A2A04[];

extern u32 gUnknown_0202F2CC;
extern u32 gUnknown_0202F2D0;
extern u16 gUnknown_0202F2D4;
extern u16 gUnknown_0202F2D6;
extern const struct ProcCmd gUnknown_08582C5C[];
/* Wave 36, W36-K. Two more proc scripts sub_0806F0EC starts:
 *   08582E54  Proc_StartBlocking, and the caller writes +0x58 of the returned
 *             proc with a word -- either the gUnknown_08582764 halfword or -1.
 *   08582C3C  Proc_Start, no field written. */
extern const struct ProcCmd gUnknown_08582E54[];
extern const struct ProcCmd gUnknown_08582C3C[];

/* sub_0806BB08's screen blobs (parked near-miss, 94.2% at exact size, so the
 * types below are as good as the ones above it -- only one CSE decision on the
 * constant 0 separates that candidate from the ROM). 081951F4/08195214 are
 * ApplyPaletteExt sources so `u16 []` and non-const; the rest are Decompress
 * or sub_080718F8 arguments so `u8 []`. 0858175C is Proc_EndEach's script and
 * 085819D4 is Proc_Start's; 085819E4 goes to sub_080670F8(const u8 *).
 *
 * 081D9424 is 0x12 bytes memcpy'd onto a stack `u16 [9]` by sub_080867BC and
 * indexed as halfwords there. */
extern u8 gUnknown_08191ADC[];
extern u8 gUnknown_08194614[];
extern u8 gUnknown_08194A9C[];
extern u8 gUnknown_0819507C[];
extern u16 gUnknown_081951F4[];
extern u16 gUnknown_08195214[];
extern u8 gUnknown_08195234[];
extern u8 gUnknown_081952D4[];
extern u8 gUnknown_081B9A38[];
extern const u16 gUnknown_081D9424[];
extern const struct ProcCmd gUnknown_0858175C[];
extern const struct ProcCmd gUnknown_085819D4[];
extern const u8 gUnknown_085819E4[];

/* The five ROM blobs sub_08040CA4 hands to the graphics helpers. Each is typed
 * from its consumer and not from its address: 081214B4 goes to
 * `Decompress(u8 *, void *)` and 0812189C to `ApplyPaletteExt(u16 *, ...)`, so
 * neither can be `const`; 08121344, 08121870 and 081240BC are the three
 * animation scripts sub_0801C210 takes as `void *`. Sizes are unknown -- the
 * function only ever passes the bare symbol. */
extern u8 gUnknown_08121344[];
extern u8 gUnknown_081214B4[];
extern u8 gUnknown_08121870[];
extern u16 gUnknown_0812189C[];
extern u8 gUnknown_081240BC[];

/* wave 13 (A5), all six from sub_0805D438 -- the per-step body of the
 * gUnknown_030046B0 script interpreter.
 *
 * gUnknown_030044D8 is a gate: the step returns immediately unless it is zero.
 * gUnknown_03004680 has only its ADDRESS taken, as sub_08071908's sole
 * argument, so `u8 []` is the weakest model that still gives the clean pool
 * word -- same reasoning as gUnknown_084C38BC. */
/* Wave 34, W34-C ADDS the volatile. sub_080335CC reads it into a local and the
 * ROM materialises the load into a scratch and then COPIES it to the
 * callee-saved register (`ldrb r0,[r0]; adds r5,r0,#0`); a non-volatile read
 * lets gcc load straight into r5 and is 2 bytes short. That extra copy is the
 * volatile-read signature -- the MEM cannot be merged into the consuming insn.
 * The plain `= 0` / `= 1` stores in sub_0803355C and src/decomp/c_08033638.c are
 * byte-identical either way (a scalar volatile store is a bare `strb`); both
 * were re-verified with try_match after the change. */
extern volatile u8 gUnknown_030044D8;
/* Wave 31, W31-B. Two RAM blocks reached only by ADDRESS, so `u8 []` is the
 * weakest model that still gives the clean pool word -- the gUnknown_03004680
 * reasoning just above.
 *
 * gUnknown_03003F70 is the link-session record sub_08033230, sub_08033404 and
 * sub_08033470 hand to sub_08062FF4 as its only argument; that function reaches
 * +0x14..+0x4b of it and nothing else names the type.
 *
 * gUnknown_03004400 is at least 0x80 bytes -- sub_0802F03C clears [0..0x7f] --
 * and unknown-functions.h already records it reaching sub_080309AC's `void *`
 * first parameter, so no one struct type covers it. sub_08033678 reads its
 * first four bytes with `ldrb`. */
extern u8 gUnknown_03003F70[];
/* Wave 56, W56-M. The link handshake's per-slot snapshot, written and read by
 * sub_08062FF4 (the AGB SDK's MultiBootMain over the gUnknown_03003F70 record).
 * THREE u16 elements and no more: every access is `gUnknown_030005EC[i - 1]`
 * with i counting 3, 2, 1, so indices 0..2, and both the write (`lsls #1; adds;
 * strh`) and the two reads (`ldrh`) are halfwords off a `lsls #1` scale, which
 * is what fixes the element width at 2 rather than the byte planes either side.
 * Slot i's SIOMULTI value is stored at [i - 1], so the array is the three
 * CLIENT slots with the server's own slot 0 omitted.
 * Unsigned: the stored value comes straight from a `ldrh` of SIOMULTI and the
 * reads are compared for equality only, so nothing sign-extends it. */
extern u16 gUnknown_030005EC[];
/* Wave 34, W34-C. Three cells sub_08033194 seeds from one ROM blob and
 * sub_08033470 consumes: gUnknown_030032DC is the blob's base, gUnknown_03003F28
 * its byte LENGTH (`gUnknown_085867D8 - gUnknown_08584CE8`, a bare `subs` with
 * no scaling, which is what fixes both endpoints as byte-addressed) and
 * gUnknown_03003F44 a fixed +0x2b0 into it that goes straight to Decompress's
 * `u8 *` source. sub_08033470 hands base+0xc0 and length-0xc0 to sub_08063454. */
extern u8 *gUnknown_030032DC;
extern int gUnknown_03003F28;
extern u8 *gUnknown_03003F44;
/* Wave 34, W34-C ADDS the volatile. sub_0803355C packs four bytes into [0..3]
 * before handing the buffer to sub_080308B4, and EVERY one of those four plain
 * `strb`s is preceded by a dead `ldrb` of the very address being stored to --
 * the same tell that settled struct Unk0849B018's members and
 * gUnknown_030044C4. Declared non-volatile the function is 8 bytes short, two
 * per store, and nothing else closes the gap.
 *
 * Byte-neutral at the only promoted reader (sub_08033678 in
 * src/decomp/c_08033638.c, four `ldrb`s), but that file also passes the array
 * to sub_080309AC's `void *`, which now needs an explicit cast to drop the
 * qualifier. Both functions in it were re-verified with try_match after the
 * change. */
extern volatile u8 gUnknown_03004400[];
extern u8 gUnknown_03004680[];
/* Two 1-bit flags at bits 0 and 1 of byte 0. BITFIELDS rather than a scalar
 * mask, and the evidence is the constant form: sub_0805D438 clears them with
 * `movs #2; rsbs` and `movs #3; rsbs`, whereas a scalar `&= ~1` narrows to a
 * bare `movs #0xfe`. That is the `mov #N; neg` tell recorded in the bitfield
 * rules of docs/agbcc-codegen.md. */
struct Unk030045CC
{
    /* 0x00 */ u8 unk00_0:1;
               u8 unk00_1:1;
               u8 :6;
};
extern struct Unk030045CC gUnknown_030045CC;
/* Wave 45, W45-F. A whole word: sub_0805BD40 reads it with a plain `ldr` and
 * compares it for equality against its own fourth (register) argument, and
 * rejects the candidate cell when the two are equal -- i.e. it is a
 * currently-selected id that the reachability probe must skip. Only ever
 * loaded and compared, so nothing constrains it beyond the width; `int` is
 * the weakest model that fits, and the signedness is unproved because `beq`
 * says nothing about it. */
extern int gUnknown_030045C8;
/* sub_0805D438 zeroes +0x00, +0x13, +0x06 and +0x07 in that order when it
 * begins a step, and gates its whole tail on +0x13 afterwards. Nothing else
 * about the object is proved; every filler is the gap between two proved
 * offsets. */
/* Wave 31 (W31-C): unk01/unk02/unk03 are three more plain `ldrb`s, read
 * together by sub_080600F0 as sub_08025E08(unk02, unk03, unk01) and by
 * sub_08060474 as the first two arguments of sub_08042C24. Byte-wide loads
 * only, so nothing narrower than u8 is possible and nothing wider fits. */
struct Unk030046C0
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
    /* 0x04 */ u8 filler_04[0x02];
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ u32 unk08; /* Wave 54, W54-G. Named out of filler_08, which had
                           * no reader anywhere in src/ or work/. A whole-word
                           * `ldr r0,[r5,#8]` in sub_0805FE0C, handed straight
                           * to sub_080129D4 as its single wide argument (the
                           * RNG-seed entry point -- unknown-functions.h records
                           * its other caller passing the literal 0x0A6B99CD).
                           * Width is proved by the `ldr`; SIGNEDNESS IS NOT
                           * PROVED -- the value is only forwarded, never
                           * compared or shifted, so `u32` is inherited from the
                           * seed reading and is byte-neutral either way. */
    /* Wave 49, W49-E. sub_08060170 snapshots unk07, unk0c, unk0d and then
     * unk0e[0..3] into gUnknown_03004490[0..2] and [4..7]. unk0c/unk0d are
     * plain `ldrb r0,[r2,#0xc]` / `[r2,#0xd]` off the struct base, so they are
     * two scalars, not part of the array; the array itself is proved by the
     * loop, which hoists `adds r2,#0xe` as its base and walks it with
     * `adds r2,#1` for four iterations (`cmp r3,#3; ble`). Extent 4 is from the
     * trip count. Widths from the loads; signedness unsettled, nothing
     * compares any of them. */
    /* 0x0c */ u8 unk0c;
    /* 0x0d */ u8 unk0d;
    /* 0x0e */ u8 unk0e[0x04];
    /* 0x12 */ u8 unk12; /* Wave 54, W54-G. Named out of filler_12, which had no
                          * reader anywhere in src/ or work/. sub_0805FE0C reads
                          * it `ldrb` at two sites and stores it into the 7-bit
                          * bitfield gUnknown_030040D8->unk06, so it is a u8
                          * whose top bit never survives; the `movs r1,#0x7f;
                          * ands` at both sites belongs to the BITFIELD STORE,
                          * not to this load, so it says nothing about the
                          * member's own width. Signedness unproved. */
    /* 0x13 */ u8 unk13;
};
extern struct Unk030046C0 gUnknown_030046C0;
/* Wave 32, W32-A. A whole-word `str` of 0 in three of the 0x08060 block's
 * state handlers (sub_080601F0, sub_08060264, sub_080602C4), always paired with
 * the gUnknown_030045D4 state advance right after a sub_08029088 call, and a
 * whole-word `ldr`/`adds #1`/`str` counter in sub_08060324, sub_08060384 and
 * sub_080603D4. SIGNED: those three re-read it after the increment and compare
 * `cmp r0, #0x1e; bgt`, and `bgt` is the signed condition -- an unsigned
 * counter would have taken `bhi`. The `= 0` writers are byte-identical either
 * way, so the compare is the only evidence and it is decisive.
 *
 * VOLATILE, and that is measured too: all three re-read it with a fresh
 * `ldr r0, [r1]` IMMEDIATELY after the `str` that increments it, for the
 * compare. A plain `int` keeps the stored value in r0 and drops that load --
 * 4 bytes short in each of the three. A re-read surviving a store to the same
 * address is docs/agbcc-codegen.md's volatile tell, and nothing else here
 * could produce it.
 *
 * The ROM words the disassembly calls gUnknown_0816DAD4 and gUnknown_0816DAD8
 * hold 0x030046D4 and 0x030046C0 -- verified against baserom.gba -- so they are
 * agbcc's own -fforce-addr address constants for this counter and for
 * gUnknown_030046C0, NOT objects in ROM. sub_08060894's apparent double
 * indirection (`ldr r0, [r5]; ldr r1, [r0]`) is one of them plus the volatile
 * read. Same reading as gUnknown_0808E540 and gUnknown_08136030 above. */
extern volatile int gUnknown_030046D4;
/* Wave 32, W32-A. A whole-word `ldr`, tested against 0 and then used as the
 * divisor of `gUnknown_030046D4 * 100` in sub_08060894 -- `__divsi3`, so
 * signed. Nothing writes it in the matched tree yet. */
extern int gUnknown_03004674;
/* Wave 47, W47-E. The AI's per-unit-type score array, written by sub_08060F00
 * (a scaled ratio, or 0xFF when the type's weight is zero) and by sub_08060F74
 * (0xFF to veto a type), and read back by sub_08060FFC. Index 1..24 in both
 * writers -- `ldr rN,=gUnknown_03004640; adds rM,rN,#2` seeds the giv -- so
 * element 0 is never touched and 25 is the extent the writers prove.
 *
 * VOLATILE, and this one is measured rather than inferred. All four `strh`s
 * across the two functions are preceded by an `ldrh` of the SAME address whose
 * result is immediately dead (it lands in a register the next instruction
 * overwrites, or in a scratch nothing reads). A plain `u16` array emits the
 * bare `strh`; `volatile u16` emits `ldrh` then `strh` for a simple assignment,
 * with no source-level read anywhere. Probed in isolation -- see the
 * "Volatile narrow stores" chapter in docs/agbcc-codegen.md. Note the contrast
 * with the volatile SImode gUnknown_030046D4 above, whose store in sub_08060718
 * is a bare `str`: the dead load is specific to the narrow store. */
extern volatile u16 gUnknown_03004640[];
/* Wave 47, W47-E. A whole-word counter incremented in sub_08060718's 64-cell
 * sweep once per cell whose gUnknown_08499594 record has a unit type that
 * gUnknown_08576877 classes non-zero -- the "units that matter" tally beside
 * gUnknown_03004674's "occupied cells" tally, which the same loop keeps. Both
 * are cleared together by one chained `a = b = 0`. `ldr`/`adds #1`/`str`, so
 * word-wide; nothing compares it in matched code, so the signedness is unproved
 * and `int` is the weakest fit (gUnknown_03004674 beside it is already `int`). */
extern int gUnknown_030045D0;
/* Wave 47, W47-E. A 25-byte ROM classification table indexed by a unit type
 * byte out of gUnknown_08499594[i].unk00, read by sub_08060718 as
 * `add r0, r8; ldrb r0, [r0]` with the symbol's address LICM-hoisted into r8 --
 * i.e. a bare `ldr rN, =sym`, which is what fixes the symbol as the array
 * itself rather than a pointer to one. The bytes at 0x08576877 are
 * 00 00 00 01 01 01 01 00 00 01 01 01 00 01 01 01 01 01 01 01 00 01 01 00 01,
 * i.e. a 0/1 flag over exactly the 1..24 type range the two AI scoring loops
 * walk, and 0x0857689C onwards is unrelated pointer data. Only tested against
 * zero, so nothing narrows it past `u8`. */
extern const u8 gUnknown_08576877[];
/* A ROM word holding a pointer to 12-byte records -- the stride is
 * `((n*2)+n)*4` in sub_0805D438, i.e. exactly 12. Only the +4 member has a
 * user: the step parks `&g[gUnknown_030040D8->unk00].unk04` in
 * gUnknown_03004784, so that member is an array rather than a scalar. */
/* Wave 32, W32-A: extent raised from 0x0c to at least 0x23 -- sub_08060A20
 * reads +0x22 as `adds r1, r0, #0; adds r1, #0x22; ldrb`, past `ldrb`'s 5-bit
 * displacement, so the split is addressing and not a member array. The first
 * three bytes are named at the same time: sub_08060894 reads +0 and +2 (and +1
 * after its second divide) with plain `ldrb`, all three as AI difficulty
 * thresholds compared against percentages. unk04[] keeps its array form --
 * elements 0..3 are the thresholds sub_0806096C, sub_080609B8 and sub_08060A20
 * test, and a displaced `ldrb` is byte-identical either way, so nothing here
 * discriminates array from members. */
struct Unk085766E0 /* >= 0x23 */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03; /* Wave 31 (W31-C): `ldrb r0, [r0, #3]` in
                          * sub_08060D78, multiplied by the u16
                          * gUnknown_03004080 with a bare `muls` (both
                          * operands narrow, so shorten_binary_op keeps it a
                          * single instruction). */
    /* 0x04 */ u8 unk04[0x08];
    /* 0x0c */ u8 unk0c; /* Wave 51, W51-E. Carved out of filler_0c, which used
                          * to span 0x0c..0x0d -- same start offset, one byte,
                          * so byte-neutral for every existing reader.
                          * sub_0805EE40 and sub_0805EF00 both read it
                          * `ldrb [r0,#0xc]` off the loaded pointer and pass it
                          * as sub_08059B4C's SECOND argument -- exactly the
                          * slot sub_08059978 and sub_080598BC fill from
                          * unk04[6] and unk04[7]. So this is almost certainly
                          * unk04[8], i.e. the same percentage table is longer
                          * than it is declared. RECORDED, NOT ACTED ON:
                          * widening unk04 reshapes a member two matched
                          * functions already read, while carving the byte out
                          * of filler is byte-neutral for everyone. Plain
                          * `ldrb`, so unsigned by the load. */
    /* 0x0d */ u8 filler_0d[0x01];
    /* 0x0e */ u8 unk0e; /* Wave 36, W36-D. A multiplier: sub_08058F30 scales
                          * sub_08042D1C's result by it with a bare `muls` and
                          * stores the product as the u8 it then clamps to 0x78,
                          * so it is a per-unit damage/percentage factor. Plain
                          * `ldrb`; carved out of filler_0c, which used to span
                          * 0x0c..0x21. */
    /* 0x0f */ u8 unk0f; /* Wave 45, W45-F. Carved out of filler_0f, which used
                          * to span 0x0f..0x21 -- same start offset, one byte,
                          * so byte-neutral for every existing reader.
                          * sub_0805BE10, sub_0805BE54 and sub_0805BEF0 each
                          * read it `ldrb [r0,#0xf]` off the loaded pointer and
                          * pass it as the FOURTH argument of the
                          * gUnknown_030013EC indirect call, beside a
                          * 1/gUnknown_030046D4 selector -- so it is a small
                          * per-unit parameter of whatever that dispatch is.
                          * Plain `ldrb`, so unsigned by the load; signedness
                          * otherwise unproved. */
    /* 0x10 */ u8 filler_10[0x11];
    /* 0x21 */ u8 unk21; /* Wave 60, W60-E. CARVED out of the tail of
                          * filler_10, which used to span 0x10..0x21 -- same
                          * start offset, one byte off the end, so byte-neutral
                          * for every existing reader (nothing reads filler).
                          * sub_08060AB0 reads it `ldr rN,=g; ldr rN,[rN];
                          * adds rN,#0x21; ldrb` and compares the `int` budget
                          * ratio against it with `bgt`, as the ceiling above
                          * which a class-4 unit is refused. Plain `ldrb`, so
                          * unsigned by the load; it sits on the SIGNED side of
                          * the comparison only because the other operand is an
                          * int, so the signedness of the member itself is
                          * unproved. */
    /* 0x22 */ u8 unk22; /* Wave 32, W32-A. sub_08060A20 compares
                          * sub_08057F54(7) against it with `ble` before the
                          * percentage test, so it is a COUNT floor rather than
                          * one of the unk04 percentages. Bare `ldrb`. */
    /* 0x23 */ u8 unk23; /* Wave 60, W60-E. EXTENDS the tag from 0x23 to 0x24
                          * bytes -- it does not move or reshape any existing
                          * member, and the CONFLICT note below records that
                          * `sizeof` has never been exercised (every promoted
                          * reader uses `gUnknown_085766E0->m`, i.e. element 0,
                          * and the two subscripting readers already use
                          * file-local views), so the extent change is inert.
                          * sub_08060AB0 reads it the same way as unk21 and
                          * compares an `__udivsi3` quotient against it with
                          * `bls`, so UNSIGNED by the comparison as well as by
                          * the load: it is a percentage ceiling on
                          * cost * 100 / funds.
                          *   NOTE FOR WHOEVER RESOLVES THE CONFLICT BELOW:
                          * 0x21 and 0x23 ALIAS the 12-byte row table this same
                          * pointer carries at +0x14 (0x21 is row 1 byte 1,
                          * 0x23 is row 1 byte 3). That is not a mistake here --
                          * it is the same overlap the conflict note describes,
                          * and sub_08060AB0 reads BOTH views in one function,
                          * which is the clearest evidence yet that the ROM
                          * object is a union of a scalar header and the row
                          * table rather than one flat struct. */
};
extern struct Unk085766E0 *gUnknown_085766E0;
extern u8 *gUnknown_03004784;
/* Wave 44, W44-H. CONFLICT, recorded and NOT acted on: the extent above
 * (">= 0x23", wave 32) cannot be reconciled with the 12-byte stride the note
 * above cites for sub_0805D438, and two more subscripts confirm the stride --
 * sub_08061178 forms `base + i * 12 + 0x14` and sub_08061E98 forms
 * `base + unit->unk00 * 12 + 4` (the latter is exactly the
 * `&g[type].unk04` the note describes, and gUnknown_03004784 receives it).
 * A `sizeof` of 0x23 would rescale every one of those. The six promoted
 * readers only ever use `gUnknown_085766E0->m`, i.e. element 0, so sizeof has
 * never been exercised and either extent compiles them identically -- which is
 * why the conflict survived. Shrinking the tag to 12 would delete unk0e and
 * unk22 out from under c_08058BB4.c and c_0806096C.c, so the tag is LEFT ALONE
 * and the two subscripting functions cast to `u8 *` / to a file-local view
 * instead. Someone with the wave-32 assembly in front of them should decide
 * whether +0x0e and +0x22 are really element 1 and element 2 bytes. */
/* Wave 44, W44-H. The ROM pointer word four bytes past gUnknown_085766E0,
 * holding 0x0202EF78 (read out of baserom.gba). It addresses a
 * 0xFF-TERMINATED array of 4-byte records: sub_08061668 walks it with
 * `adds r1, #4` until unk00 == 0xFF, so the stride is proved by the walk and
 * the terminator by the exit test. unk00/unk01 are a coordinate pair -- both
 * are `ldrb`ed and `strh`ed into the caller's u16 pair -- unk02 is matched for
 * equality against gUnknown_0857680F[gUnknown_030046C0.unk06], and unk03 is a
 * cost/priority compared with `bhs` (unsigned) and then overwritten with 0xFE
 * on the winning record, i.e. "mark consumed".
 * The disassembly's gUnknown_0816DAFC is agbcc's -fforce-addr pool word for
 * THIS symbol (0x0816DAFC holds 0x085766E4, verified against baserom.gba) and
 * must not be declared -- same species as the gUnknown_0816DB00..0C run
 * documented beside gUnknown_085771C4. */
struct Unk085766E4
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 unk03;
};
extern struct Unk085766E4 *gUnknown_085766E4;
/* Wave 44, W44-H. A byte accumulator: sub_08061F34 clears it and then ORs one
 * bit-mask byte into it per map cell that sub_0802700C rejects, with the
 * `ldrb; orrs; strb` read-modify-write. Its twin is the already-declared
 * gUnknown_030046B8, which the same loop accumulates unconditionally -- so the
 * pair is "reachable terrain kinds" vs "all terrain kinds". `u8` from the
 * `ldrb`/`strb`; nothing compares it in matched code, so nothing narrows the
 * signedness further. */
extern u8 gUnknown_030045C0;
/* Wave 44, W44-H. A ROM byte table indexed by a terrain cell's low five bits
 * (`ands #0x1f`) in sub_08061F34, whose result is halved (`lsrs #1`) and used
 * as the subscript of a 5-entry {0,0,1,2,4} bit-mask array -- so it maps a
 * terrain kind to twice a small class index, 0..8. The bytes at 0x085767F2 are
 * 00 00 00 00 00 00 02 00 00 00 04 06 00 00 02 ... and the largest seen is 8,
 * which is consistent with that 0..4 subscript range. Extent unbounded: only
 * the masked subscript reaches it, and 0x1f is a mask, not a proved length. */
extern const u8 gUnknown_085767F2[];
/* Wave 44, W44-H. A five-byte ROM template that sub_08061F34 copies onto its
 * own stack with `sub_0808B6E8(buf, gUnknown_0816DB20, 5)` -- the same idiom
 * as gUnknown_0816E0C8 above and the dozen other stack-copy sources in this
 * header. The bytes read 00 00 01 02 04 out of baserom.gba, and the loop
 * subscripts the copy with `gUnknown_085767F2[terrain & 0x1f] >> 1`, so they
 * are the bit masks that indexing table selects between.
 *   This is a REAL ROM object (data.s defines the symbol) and NOT one of the
 * -fforce-addr pool words at 0x0816DB00..0x0816DB1C that the gUnknown_085771C4
 * note warns against declaring -- those hold addresses of other globals; this
 * one holds data. Dereference the candidate in baserom.gba before deciding
 * which kind a 0x0816Dxxx word is; this block contains both. */
extern const u8 gUnknown_0816DB20[5];

/* Three more agbcc `.LC` constant-pool words, NOT globals -- same species as
 * gUnknown_08090D88 above, found in the 0x08136050-0x081360E0 block. They hold
 * &gUnknown_03001FBC, &gUnknown_0300451C and &gUnknown_0300453C respectively.
 *
 * CORRECTION (wave 19, W19-B). This entry used to say "use them the way
 * gUnknown_08090D88 is used: bind `&gUnknown_081360Ax` to a local and read
 * `**local`. Spelling the underlying global directly compiles to the right
 * BYTES but the wrong relocation, because agbcc then builds its own unplaceable
 * pool word." Both halves are now wrong. The word is placeable since wave 18
 * (tools/split_rodata.py), and naming the globals directly DOES make agbcc emit
 * all three words, in the ROM's own slot order -- but only once the function's
 * two-bit guard is spelled so that it does not fold into one test. With the
 * `&&` spelling agbcc emits no `.rodata` at all and the reroute is invisible;
 * with the nested-`if` spelling the three words appear. So the reachability of
 * a `.LC` word is decided by the CANDIDATE's control flow, not by whether the
 * object behind it is a scalar -- see docs/agbcc-codegen.md. sub_0804F18C is
 * still the worked case and is still open, but on register allocation now, and
 * its `c_local` draft is kept only as _clocal_nearmiss.c.
 *
 * These three declarations are therefore unused by any draft and are kept only
 * as documentation of what lives at those addresses. */
extern s16 *const gUnknown_081360A0;
extern u16 *const gUnknown_081360A4;
extern u16 *const gUnknown_081360A8;
/* Wave 85, W85-C (sub_08050FF8). Four more cells from the 0x08136050-0x081360E0
 * run, read with a genuine DOUBLE load in the ROM -- `ldr r4,=gUnknown_081360E0
 * ; ldr r2,[r4] ; ... adds r0,r0,r2 ; ldrh r0,[r0,#4]` -- the loaded value is
 * used as a BASE, which a -fforce-addr word never is (force-addr words are used
 * directly as the address). The wave-37 note in work/sub_08050FF8 called all
 * four force-addr constants and the draft built on that; it missed by -20
 * bytes. Contents confirmed by the strides the ROM applies after each chase:
 *   0x081360D8 -> 0x020298E0  (+ side*0x90, .unk8c at 0x8c)
 *   0x081360DC -> 0x0300453C  (plain u16)
 *   0x081360E0 -> 0x085D6A48  (+ row*24, columns 4 and 8)
 *   0x081360E4 -> 0x03004580  (+ row*16, columns 1 and 2) */
extern struct Unk020298E0 *const gUnknown_081360D8;
extern u16 *const gUnknown_081360DC;
extern u16 (*const gUnknown_081360E0)[12];
extern u16 (*const gUnknown_081360E4)[8];
/* TWO halfwords, not one. sub_0804F18C writes [0] with a bare `strh` (it
 * stores OamData.priority into it straight after setting that field), and
 * sub_08050958 writes BOTH -- `strh r3,[r6]` and `strh r0,[r6,#2]` off a
 * single pool word for gUnknown_0300454C -- with the pair copied out of
 * gUnknown_03001470[proc].unk30 / .unk34, i.e. the current side and slot. It
 * also reads [1] back (`ldrh r0,[r6,#2]`). One symbol reached at +2 is an
 * array or an aggregate, never two globals; `u16 []` is the weaker of the two
 * and nothing distinguishes them here. Was `extern u16 gUnknown_0300454C;`;
 * no promoted file names it, so nothing needed re-verifying. */
extern u16 gUnknown_0300454C[];
/* Wave 34, W34-L, all from sub_08053434 / sub_080534A0.
 * gUnknown_030045A4 and gUnknown_03004510 are PAIRS, not scalars: sub_08053434
 * fills [0] and [1] of each from the two rows of gUnknown_03004580, and
 * sub_080534A0 subscripts gUnknown_030045A4 with gUnknown_0300450C. Same
 * `strh r0,[r3]` / `strh r0,[r3,#2]` off one pool word that settled
 * gUnknown_0300454C above. */
extern u16 gUnknown_030045A4[];
extern u16 gUnknown_03004510[];
/* A frame or step counter: sub_080534A0 does `ldrh; adds #1; strh` and elsewhere
 * masks it with 1 to pick a column, and sub_08053434 zeroes it. Halfword from
 * the read-modify-write; signedness unproved, nothing sign-extends it. */
extern u16 gUnknown_03004530;
/* Wave 50, W50-F: SIGNED, retyped from u16. sub_080532D8 loads it `ldrsh` with
 * no narrowing context, compares it against -1 and only then passes it to
 * sub_080153F0/sub_08015328 -- the same "-1 is no proc" guard those two get from
 * struct Unk02029A10's s16 unk18. As a u16 the `!= -1` test could never be
 * false and gcc would have folded the branch away. Its one writer,
 * src/decomp/c_08053434.c, stores gUnknown_03001FBC with `strh` either way, so
 * the retype is byte-neutral there (re-verified). */
extern s16 gUnknown_03004570;
/* Two parallel 11-entry ROM u16 tables 0x16 bytes apart, both indexed by
 * gUnknown_03004580[side][5] and copied into the pairs above. */
/* A third table of the same 11-entry shape, 8 bytes below gUnknown_085537F4.
 * sub_080531D4 reads it through a second indirection,
 * gUnknown_085537EC[gUnknown_030045A0[gUnknown_0300450C]]. */
extern const u16 gUnknown_085537EC[];
extern const u16 gUnknown_085537F4[];
extern const u16 gUnknown_0855380A[];
/* Wave 34, W34-L. sub_080534A0 reads this with `ldrsh` at a byte offset of
 * `v * 12 + gUnknown_03004520 * 4 + (gUnknown_03004530 & 1) * 2`, i.e. halfword
 * index `v * 6 + x * 2 + y` -- so the extents are [3][2] and not a flat table.
 * SIGNED on the `ldrsh`, which is also the only evidence of the width. */
/* Wave 36, W36-C. The 8 bytes immediately before gUnknown_085643B0, holding two
 * halfword pairs on the same "alternating phase bit" index gUnknown_085643B0's
 * innermost subscript uses. sub_08053520 reads [1][phase & 1] with a bare
 * `movs r1,#0; ldrsh r0,[r0,r1]` into sub_0803B48C's s16 parameter, which is
 * what signs it. c_08051454.c reads [0][phase & 1].
 * WAVE 86 (W86-C): THIS WAS `struct Unk085643A8 { const s16 unk00[2]; const
 * s16 unk04[2]; }` AND THE COMMENT HERE HAD ITS MEASUREMENT BACKWARDS. It
 * said "A STRUCT and not `s16 [][2]` with a constant outer index ... which
 * only a struct member array gives". A controlled probe of the two spellings
 * over identical address arithmetic (constant 4 plus a variable m*2) says the
 * exact opposite:
 *   ARRAY  gUnknown_085643B0[0][1][m] -> ldr r2,=sym / adds r2,#4 /
 *                                       adds r0,r0,r2 / movs r1,#0 / ldrsh
 *   STRUCT gUnknown_085643A8.unk04[m] -> ldr r3,=sym / adds r0,r0,r3 /
 *                                       movs r1,#4 / ldrsh
 * -fforce-addr FORCE_REGs an ARRAY's address BARE at the subscript, so a
 * later constant offset has to be a runtime add on that register and the
 * ldrsh indexes with zero; a scalar struct's member offset stays a link-time
 * constant and folds into the ldrsh index instead. The ROM has the FIRST
 * form, so this object is an array. The 'the flat spelling sinks it into the
 * relocation' observation the old comment rested on was measured on
 * `const s16 *p = g.unk04;` -- a POINTER TO THE MEMBER, which is a different
 * construct from a 2-D array with a constant outer index. Extent unproved;
 * only rows 0 and 1 are reached. */
extern const s16 gUnknown_085643A8[][2];
extern const s16 gUnknown_085643B0[][3][2];
/* A ROM halfword per side, indexed [gUnknown_0300453C]. Two readers,
 * sub_0804F658 and sub_0804E584, and both do nothing but pass it as
 * sub_0804BCB8's third argument. `u16 []` rather than `s16 []`: the `ldrsh` at
 * both sites is the conversion to that parameter's narrow SIGNED type, not the
 * object's own width -- combine folds sign_extend(truncate(zero_extend(mem:HI)))
 * into one sign-extending load, so `ldrsh` says nothing about the table here.
 * Extent unproved; only [gUnknown_0300453C] is ever reached. */
extern u16 gUnknown_0855214C[];
/* ROM u16 pairs indexed [gUnknown_0300453C][gUnknown_0300450C] -- `i * 4 + j * 2`
 * off the symbol, the same [2]-wide row shape as gUnknown_0855239C. The value is
 * an x offset: sub_0804F18C sign-extends it and adds it to a sprite x. */
extern u16 gUnknown_085644E0[][2];
/* The u16 ROM table gUnknown_02028E5C's column 1 indexes, named in that
 * global's own comment since wave 13 and declared here in wave 18 from
 * sub_0804E7A8, which does
 * `gUnknown_02029A10[i].entries[j].y -= gUnknown_085644D4[row[1]]`
 * with `row = gUnknown_02028E5C[i]`. A y correction, read `ldrh`, extent
 * unproved. */
/* A ROM table of (x, y) pixel offsets indexed [side][phase * 2] and
 * [side][phase * 2 + 1], where `phase` is gUnknown_02029B94[side][slot], the
 * six-step counter above. Row stride is 24 bytes = 12 u16, emitted as
 * `(c * 2 + c) * 8`, and the two reads are 4 bytes apart within the row -- so
 * the row holds six (x, y) pairs, one per counter value, which is what makes
 * the [12] a real extent rather than a stride. `ldrh`; the x half is added to
 * gUnknown_02029A10[..].x and the y half is added to .y after a u16
 * truncation, so nothing here sign-extends. */
extern u16 gUnknown_085523B0[][12];
/* Wave 32, W32-A. A 0x90-entry cycle of SIGNED y offsets: sub_0804EDAC steps
 * the slot's own unk28 counter, wraps it at 0x90, and adds
 * gUnknown_085523E0[counter] into gUnknown_02029A10[..].entries[..].y. Signed
 * from the `movs r3, #0; ldrsh r0, [r2, r3]` that reads it -- the
 * register-offset form an `ldrsh` is forced into -- and the wrap is what bounds
 * the table. */
extern s16 gUnknown_085523E0[];
/* Wave 33, W33-D. The same table species as gUnknown_085523E0 above, with a
 * 0xc0-entry cycle instead of 0x90: sub_0804EE08 steps gUnknown_03001470[c]
 * .unk28, wraps it at 0xc0, and adds gUnknown_08552500[counter] into
 * gUnknown_02029A10[..].entries[..].y. SIGNED from the same
 * `movs r3, #0; ldrsh r0, [r2, r3]` register-offset read. */
extern s16 gUnknown_08552500[];
/* Wave 33, W33-D. A ROM halfword PAIR per side, and FLAT rather than `[][2]`:
 * sub_0804EE08 reads `[a * 2]` with the byte offset `a << 2` CSEd out of the
 * `a * 5 + b` chain it also needs, and `[a * 2 + 1]` as `((a * 2 + 1) * 2)` --
 * one index chain. The `[][2]` spelling reassociates the second read to
 * `base + 2 + a * 4` and is different code. The pair goes into
 * struct Unk56E28's unk04/unk06, i.e. it is a position. Plain `ldrh`. */
extern const u16 gUnknown_085534EC[];
/* Wave 37, W37-J. Three more of gUnknown_085534EC's species -- ROM halfword
 * (x, y) PAIRS, and FLAT for the same reason: the byte offset of the first
 * element of a pair is CSEd out of an index chain the function already needs,
 * and the second element is `(n + 1) * 2` off it, one chain.
 *   gUnknown_085534BC  a pair per SIDE. sub_0804EB78 reads `[c * 2]` with the
 *                      byte offset `c << 2` shared with gUnknown_02028E5C and
 *                      gUnknown_084C3F78, and `[c * 2 + 1]`; the pair becomes
 *                      struct Unk56E28's unk04/unk06, i.e. a position.
 *   gUnknown_085534C4  a pair per (side, frame): sub_0804EB78 reads
 *                      `[(c * 5 + f) * 2]` and `+ 1`, with `c * 10` shared
 *                      with the gUnknown_02029B94[c][e] row offset. The two
 *                      halves are added to another entry's x and y.
 *   gUnknown_08553524  gUnknown_085534C4's exact counterpart in sub_0804F3C8,
 *                      same index shape and same two consumers.
 * Plain `ldrh` at all six reads, so nothing signs them; the extents are the
 * index shapes, not proved bounds. */
extern const u16 gUnknown_085534BC[];
extern const u16 gUnknown_085534C4[];
extern const u16 gUnknown_08553524[];
/* Wave 32, W32-A. Indexed by gUnknown_02028E5C[side][1] after that counter is
 * bumped, `ldrh` off a `lsls #1` element -- sub_0804B3E0 hands the value
 * straight to `*gUnknown_084C3F78[side]`, which is `u16`. Nothing bounds the
 * table; the comment on gUnknown_02028E5C names the two readers. */
extern u16 gUnknown_085644C8[];
extern u16 gUnknown_085644D4[];

/* The twelve 0x14-byte animation descriptors sub_08022BB8 picks between, and
 * the halfword offset table it pairs them with. The descriptors are named one
 * at a time rather than indexed, so each gets its own declaration; they are
 * only ever passed on as `void *`. gUnknown_080909B8 is indexed with i and
 * i + 1 for an (x, y) pair, so the entries alternate. */
extern u16 gUnknown_080909B8[];
extern u8 gUnknown_08499B8C[];
extern u8 gUnknown_08499BA0[];
extern u8 gUnknown_08499BB4[];
extern u8 gUnknown_08499BC8[];
extern u8 gUnknown_08499BDC[];
extern u8 gUnknown_08499BF0[];
extern u8 gUnknown_08499C04[];
extern u8 gUnknown_08499C18[];
extern u8 gUnknown_08499C2C[];
extern u8 gUnknown_08499C40[];
extern u8 gUnknown_08499C54[];
extern u8 gUnknown_08499C68[];

/* The 0x08083 sprite-builder triple (sub_080831FC, sub_08083484, sub_08083738)
 * and their caller sub_080829B0 all key off this SIX-byte table, and the extent
 * is proved rather than guessed: the ROM bytes are 00 05 02 04 03 01, a
 * permutation of 0..5, and every reader indexes it with `DivRem(x, 6)`, so a
 * seventh element could never be reached. What sits at +6 is a DIFFERENT table
 * -- the s16 offsets that sub_080829B0 names separately as gUnknown_08616972 --
 * so the boundary is a type change, not an inference from the index.
 *   Left non-const: the three builders each read it four or five times with a
 * `bl DivRem` between every pair of reads and the ROM re-`ldrb`s every time,
 * which by the const-across-a-call tell in docs/agbcc-codegen.md is what an
 * unqualified declaration produces. The reads have different indices in
 * principle, so this is corroborating rather than decisive -- flagged. */
/* Wave 54, W54-C. The two s16 SIGN tables that sit immediately BEFORE
 * gUnknown_0861696C, in the same run as the two coordinate tables declared just
 * after it. sub_08080BF0 reads both as `g[DivRem(i, 4)]` and multiplies each by
 * proc->unk2c to make a sprite x and y offset.
 *   s16 is proved twice over: the ROM holds {1, -1, -1, 1} and {1, -1, 1, -1},
 * so the elements are negative, and both reads are `ldrsh` at a register offset
 * with no following shift pair.
 *   The extents are proved by each other and by the next symbol -- 0x08616964 -
 * 0x0861695C = 8 bytes and 0x0861696C - 0x08616964 = 8, i.e. four elements
 * each, which is also exactly the DivRem(i, 4) modulus. Not const: the ROM
 * re-loads across the `bl DivRem` between the two reads. */
extern s16 gUnknown_0861695C[4];
extern s16 gUnknown_08616964[4];
extern u8 gUnknown_0861696C[6];
/* The two s16 coordinate tables that sit immediately after gUnknown_0861696C,
 * read by the 0x08082-0x08083 sprite block (sub_080829B0, sub_08082C0C,
 * sub_08083034, sub_08083A44) as `gUnknown_08616972[i + 1]` /
 * `gUnknown_08616980[i + 1]` for i in a five- or six-step loop, plus a
 * constant-folded [3] on the loop's special index.
 *   s16 rather than u16 is PROVED, not assumed, and the discriminator is a pair
 * of sibling expressions in sub_080829B0's two i == 2 arms over the same
 * element of gUnknown_08616980:
 *     `g[3] | 0x100`        -> ldrh ; orr ; lsl #16 ; asr #16
 *     `(g[3] - 8) | 0x100`  -> ldrsh ; sub ; orr        (no re-extension)
 * The C front end's `shorten` fires on BIT_IOR when both operands narrow, so
 * the first form computes in HImode and needs a sign-extension back to SImode
 * -- which only exists if the element type is SIGNED. The second form's left
 * operand is a MINUS in SImode, which get_narrower cannot narrow, so no
 * shortening happens and the sign already came from the `ldrsh`. A u16 table
 * gives neither the `ldrsh` nor the trailing `asr`. `g[3] & 0x1FF` in the same
 * arms loads with `ldrh` and needs no extension because combine drops both
 * (the mask is below 0x8000) -- that one is NOT evidence either way.
 *   gUnknown_08616972's extent is proved by the next symbol: 0x08616980 -
 * 0x08616972 = 14 bytes = seven elements, ROM
 * {-0x80, -0x28, -0x08, 0x10, -0x08, -0x28, -0x80}, i.e. period six with the
 * wrap element materialised so `[i + 1]` at i = 5 need not be reduced.
 * gUnknown_08616980 is left unsized -- nothing bounds it; ROM begins
 * {0x83, 0x80, 0x70, 0x50, 0x30, 0x20, 0x1D}. Neither is const: every read is a
 * fresh load with a `bl DivRem` or `bl Interpolate` between. */
extern s16 gUnknown_08616972[7];
extern s16 gUnknown_08616980[];
/* Wave 44, W44-F. sub_0807F57C's source table: a run of `ldrb`-read byte ids
 * split into groups by an 0xFF sentinel, with ONE extra byte after each
 * sentinel that the function copies into gUnknown_03005958 (a group label) and
 * a second 0xFF terminating the whole run. u8 from every access; extent
 * unbounded, since the walk is sentinel-driven rather than counted. Not const:
 * the ROM re-`ldrb`s every element across the `bl sub_0803CAB8` in the inner
 * loop. */
extern u8 gUnknown_086166F0[];
/* Wave 44, W44-F. One more of the OAM sprite blobs on PutSprite's `u16 *`
 * fourth parameter, drawn once per column by the twins sub_0807FC70 and
 * sub_08080A70 and reaching nothing else. `u16 *` for that reason and NOT
 * const -- PutSprite's parameter is a plain `u16 *`. Extent unknown; nothing
 * indexes it. */
extern u16 gUnknown_0848B6E6[];
/* Two more of the OAM sprite blobs described above, on the `u16 *` fourth
 * parameter of PutSprite and PutSpriteExt and reaching nothing else. Their
 * extents are proved by each other and by gUnknown_0861696C's neighbours:
 * gUnknown_08615C76 is 0x0002 plus two (y, x, tile) triples = 14 bytes, which
 * lands exactly on gUnknown_08615C84, and that one is 0x0004 plus four
 * triples = 26 bytes. Same model, and the same single piece of evidence, as
 * gUnknown_08615C04 above. */
/* One more OAM sprite blob on the same model: ROM is 0x0001 followed by one
 * (y, x, tile) triple, and its single reader hands it straight to
 * sub_0801BD00's `void *` third parameter with no arithmetic. Not const --
 * that parameter is not. */
extern u16 gUnknown_085806F2[];
/* Wave 50, W50-M. One more of the same species, the record immediately BELOW
 * gUnknown_085806F2 and pinned by it: data-0848B688.s gives it 8 bytes and
 * baserom.gba holds 0x0001 followed by one (y, x, tile) triple, exactly the
 * gUnknown_085806F2 shape. sub_08063CCC hands it straight to sub_0801BD00's
 * `void *` third parameter with no arithmetic, so not const. */
extern u16 gUnknown_085806EA[];
/* Wave 50, W50-M. Two 16-entry ROM tables of POINTERS to blobs of the kind
 * above, and the one place in this file where the addresses are proved rather
 * than assumed: data-0848B688.s gives each table 0x40 bytes, and dereferencing
 * baserom.gba gives sixteen consecutive words stepping by 8 --
 * gUnknown_0858081C holds 0x0858071C..0x08580794 and gUnknown_0858085C holds
 * 0x0858079C..0x08580814 -- i.e. two runs of sixteen 8-byte sprite blobs, the
 * second run ending exactly where gUnknown_0858081C begins.
 *
 * These are REAL TABLES, not `-fforce-addr` pool words, by wave 49's inverse
 * test: sub_08063CCC indexes each by a computed subscript (`lsls rN,a4,#2;
 * adds; ldr`) before dereferencing, where a pool word is loaded and
 * dereferenced immediately with no index. Element type is `void *` because the
 * loaded word goes straight to sub_0801BD00's `void *` third parameter. Not
 * const, for the same reason as the blobs. */
extern void *gUnknown_0858081C[];
extern void *gUnknown_0858085C[];
/* Wave 30, W30-D. One more OAM sprite blob of the same species, one record
 * lower: sub_08087220 is a single `PutSprite(1, 0x1FE, y + x * 16,
 * gUnknown_08615C4E, 0x5470)`, and PutSprite's fourth parameter is `u16 *`.
 * The ROM there is 0x0003 followed by three (y, x, tile) triples, exactly the
 * gUnknown_08615C76 shape described below. */
extern u16 gUnknown_08615C4E[];
/* Wave 48, W48-K. The blob BETWEEN those two, and its extent is pinned from
 * both sides in baserom.gba: gUnknown_08615C4E is 0x0003 plus three
 * (y, x, tile) triples = 20 bytes, which lands exactly here, and this one is
 * 0x0003 plus three triples too, landing exactly on gUnknown_08615C76. Typed
 * from its only reader, sub_0808A978's
 * `PutSprite(0, 0x18, 8, gUnknown_08615C62, 0x40)` -- PutSprite's fourth
 * parameter is `u16 *`. Not const, for the same reason as its neighbours. */
extern u16 gUnknown_08615C62[];
extern u16 gUnknown_08615C76[];
extern u16 gUnknown_08615C84[];
/* Two OAM sprite blobs, both typed only by the `u16 *` fourth parameter of
 * PutSprite / PutSpriteExt, the same model as gUnknown_0849B6C8 and
 * gUnknown_08499E10. Each is an OBJ count followed by that many (y, x, tile)
 * triples: gUnknown_08615C04 begins 0x0002 and gUnknown_0848B6CE begins 0x0001,
 * which is why the count is not part of the type. Neither is const -- those
 * parameters are not. */
extern u16 gUnknown_08615C04[];
extern u16 gUnknown_0848B6CE[];
/* Wave 45 (W45-I). A word table sub_0807B51C indexes with its fourth argument.
 * The element is NOT a pointer: it is added to `digit * 4`, OR'd with 0x1000
 * and handed to PutSprite's `u32` fifth parameter, so it is an OAM attribute-2
 * base -- a tile number, four tiles per decimal digit, palette 1 forced on.
 * NOT const: sub_0807B51C hoists only the ADDRESS `&gUnknown_0861617C[i]` out
 * of its digit loop and re-`ldr`s the element every pass; with `const` GCC
 * hoists the load too. Same evidence as gUnknown_0861433C. */
extern int gUnknown_0861617C[];
/* Wave 54, W54-A. sub_0807B148's six ROM tables, and the record they belong to.
 * They are the SECOND of two parallel groups of six tables with an IDENTICAL
 * size sequence -- 8, 0xc, 0x10, 6, 6, 8 bytes:
 *   group 1  086160A4 086160AC 086160B8 086160C8 086160CE 086160D4  (sub_0807AE94)
 *   group 2  086160DC 086160E4 086160F0 08616100 08616106 0861610C  (sub_0807B148)
 * That the two runs tile with the same six sizes, in the same order, back to
 * back, is what identifies them as one record used twice rather than twelve
 * unrelated tables -- there is no shared symbol and no shared callee between
 * the two functions to say so. Inside a group the tables pair up as
 * (base, step), and the pairing is pinned by the subscript, which is the same
 * `gUnknown_0202FDEC.unk08 - 1` (or `.unk09 - 1`) in both members of a pair:
 *   086160DC u16[] base  with  08616100 u16[] step   (unk08-bounded loop)
 *   086160E4 int[] base  with  08616106 u16[] step   (unk09-bounded loop)
 *   086160F0 int[] base  with  0861610C u16[] step   (unk08-bounded loop)
 * Element widths are read straight off the loads -- `lsls #2` + `ldr` for the
 * two int tables, `lsls #1` + `ldrh` for the four u16 ones. Left UNSIZED on
 * purpose: the extents the arithmetic implies (4, 3, 4, 3, 3, 4) tile the
 * 0x086160DC..0x08616114 run exactly, and 0x08616114 does begin a ProcCmd, but
 * 08616100 would need a FOURTH entry to cover unk08 == 4 and the run leaves no
 * room for one. So either unk08 <= 3 on the path that reads it or one extent is
 * wrong; nothing here settles which, and an extent would be a guess. */
extern u16 gUnknown_086160DC[];
extern int gUnknown_086160E4[];
extern int gUnknown_086160F0[];
extern u16 gUnknown_08616100[];
extern u16 gUnknown_08616106[];
extern u16 gUnknown_0861610C[];
/* Wave 54, W54-A. GROUP 1 of the same record -- sub_0807AE94's six tables. The
 * types below were NOT read off sub_0807AE94's assembly first; they were
 * PREDICTED from group 2 above (identical 8, 0xc, 0x10, 6, 6, 8 size sequence,
 * same (base, step) pairing, same subscripts) and then confirmed instruction by
 * instruction. Both functions run the same three loops -- an unk08-bounded
 * descending one, an unk09-bounded one with a `(2 - i)` step and an
 * unk08-bounded one with a `(3 - i)` step -- over their own group:
 *   086160A4 u16[] base  with  086160C8 u16[] step   (unk08 loop, descending)
 *   086160AC int[] base  with  086160CE u16[] step   (unk09 loop, `2 - i`)
 *   086160B8 int[] base  with  086160D4 u16[] step   (unk08 loop, `3 - i`)
 * sub_0807AE94 differs from sub_0807B148 only by adding `proc->unk34 - 0xf0`
 * to each result -- a scroll offset -- so the two are one routine parameterised
 * by group, which is what the size sequence predicted. Unsized for the same
 * reason as group 2. */
extern u16 gUnknown_086160A4[];
extern int gUnknown_086160AC[];
extern int gUnknown_086160B8[];
extern u16 gUnknown_086160C8[];
extern u16 gUnknown_086160CE[];
extern u16 gUnknown_086160D4[];
/* sub_0807AE94's two alternative Decompress sources, picked on proc->unk3c,
 * both unpacked into *gUnknown_0849957C. Passed by name with no arithmetic, so
 * `u8 []` off `Decompress(u8 *, void *)` and non-const; extents unpinned. */
extern u8 gUnknown_0822B944[];
extern u8 gUnknown_0822BCF0[];
/* Wave 55 (W55-B). sub_0807AA84's four blobs, one pair per arm of the same
 * `proc->unk3c` branch the two above are picked by -- 0822AC80/0822BB60 on the
 * arm that also uses gUnknown_0822B944, 0822BB80/0822BDFC on the arm that uses
 * gUnknown_0822BCF0.
 *
 * 0822AC80 and 0822BB80 are `Decompress(u8 *, void *)` sources, unpacked
 * straight to VRAM at `chr_block * 0x4000 + 0x06000000`; 0822BB60 and 0822BDFC
 * are `ApplyPaletteExt(u16 *, u32, u16)` sources, both called as
 * `(blob, 0, 0x20)`. All four are passed by name with no arithmetic, so the
 * element types come only from the parameter each one lands on and the extents
 * are unpinned. Non-const on the same grounds as gUnknown_0822B944 above:
 * neither callee's pointer parameter is const. */
extern u8 gUnknown_0822AC80[];
extern u8 gUnknown_0822BB80[];
extern u16 gUnknown_0822BB60[];
extern u16 gUnknown_0822BDFC[];
/* Proc script -- Proc_Start's first argument. sub_0807AE94 starts it as a child
 * of its own proc and then writes the CHILD's +0x3c from its own +0x3c, which
 * is the only thing known about the started proc's record. */
extern const struct ProcCmd gUnknown_08616034[];
/* Wave 30, W30-E. The OAM sprite blob sub_080859A0 hands to PutSprite's
 * `u16 *` fourth parameter -- its only reader, so nothing constrains it
 * beyond that. Not `const`, because PutSprite's parameter is not (same model
 * as gUnknown_08499E10). */
extern u16 gUnknown_0848B690[];
/* Wave 48 (W48-E), the 0x0807B block.
 *
 * 0848B6C6 is an OAM sprite blob on PutSprite's `u16 *` fourth parameter, same
 * model as gUnknown_0848B690 eight bytes below it. sub_0807B690 reaches it
 * ONLY through its own .rodata address-constant pool (-fforce-addr): the ROM
 * spells the read `ldr r4, =gUnknown_081D9328; ldr r3, [r4]`, and the word at
 * 081D9328 dereferences to 0848B6C6 in baserom.gba. 081D9324 holds the SAME
 * address for a neighbouring function's private copy. gUnknown_081D9328 is
 * therefore NOT a global and must never be declared as one. */
extern u16 gUnknown_0848B6C6[];
/* Wave 55 (W55-B). Another OAM sprite blob on PutSprite's `u16 *` fourth
 * parameter, same model as gUnknown_0848B690 and gUnknown_0848B6C6 either side
 * of it. sub_0807C2D4 passes it by name with no arithmetic to two PutSprite
 * calls that differ only in the Y coordinate (0x30 / 0x32), so nothing pins the
 * extent and nothing signs it. Not const, for the same reason as
 * gUnknown_0848B690: PutSprite's parameter is not. */
extern u16 gUnknown_0848B6A8[];
/* An ApplyPaletteExt `u16 *` source. sub_0807B690 indexes it with a bare
 * `lsls #1` off DivRem(Div(frame, 3), 0x10) and hands the address to
 * ApplyPaletteExt(u16 *, 0x238, 2) -- 2 bytes at palette word 0x238, i.e. one
 * COLOUR of BG palette 0x11, cycled through 16 source entries. `u16 []` from
 * the index scale; not const, ApplyPaletteExt takes a plain pointer. Left
 * unsized: 0x10 entries is what the DivRem bounds, but nothing pins the
 * element count beyond the modulus. */
extern u16 gUnknown_0822AC60[];
/* A per-column u8 table sub_0807BFB8 indexes with its loop counter and folds
 * into a sprite X as `(t * 5) >> 1` (a bare `asrs`, so the shift is on the
 * promoted int, NOT a divide). `u8` from the `ldrb`; unsigned is unproved --
 * the only consumer is that multiply, and `ldrb` would be the same for either
 * signedness with no sign-extend in sight because the value is only widened
 * through the multiply. Unsized: the bound is proc->unk_4c, a runtime count. */
extern u8 gUnknown_0202FF78[];
/* sub_0807BD5C's Decompress SOURCE, unpacked into gUnknown_0200FC50, so `u8 []`
 * and NOT const -- Decompress's first parameter is a plain `u8 *`. Same model
 * as gUnknown_08234B10. */
extern u8 gUnknown_0822DC08[];
/* Wave 48 (W48-E). An EWRAM tile buffer, and the object it sits in extends
 * BACKWARDS from the symbol: sub_0807BD5C CpuFastSets 0x20 words FROM
 * gUnknown_02010450 itself, then runs a ten-iteration double copy whose two
 * sources start at gUnknown_02010450 - 0x680 and gUnknown_02010450 - 0x280
 * (pool words 0xFFFFF980 and 0xFFFFFD80 added to the symbol). Those are real
 * subtractions, not a different base -- the ROM names 02010450 and offsets
 * from it, so the original source did too. The -0x280 run ends exactly at the
 * symbol. `u8 []` is the weakest model that fits; CpuFastSet takes
 * `const void *` and nothing here pins an element type. Extent unknown in
 * BOTH directions, which is why it is left unsized. */
extern u8 gUnknown_02010450[];
/* Wave 48 (W48-H). sub_08076B20's lookup table: 8-byte records, a key byte
 * followed by SEVEN payload bytes, terminated by a key of 0xFF. The stride is
 * the `adds r2, #8` that walks it and the payload base is `adds r4, r2, #1`,
 * so the split is 1 + 7 and not 8 loose bytes. Read straight out of
 * baserom.gba, the first six records are
 *   07 | 00 01 02 03 04 05 06      0F | 08 09 0A 0B 0C 0D 0E
 *   17 | 10 11 12 13 14 15 16      1F | 18 19 1A 1B 1C 1D 1E
 *   21 | FE FE FE FE FE FE FE      29 | 22 23 24 25 26 27 28
 * then the 0xFF terminator row.
 *
 * unk01 is `u8` and the SIGN lives at the use, not in the member: sub_08076B20
 * reads it `ldrb` and then `lsls #0x18; asrs #0x18`. A declared `s8` array
 * folds to `ldrsb rD, [rB, rO]` here (the index is a register), so the shift
 * pair proves an explicit `(s8)` cast over an unsigned member -- and the 0xFE
 * row is exactly the -2 that cast produces. */
struct Unk0861515C
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01[7];
};

extern const struct Unk0861515C gUnknown_0861515C[];
/* Wave 48 (W48-E). sub_0807B7BC's glyph table: a 12-byte record per drawable
 * character, walked with `adds r5, #0xc` and terminated by a zero unk00.
 *   unk00  the character code, compared against a `u8 *` string byte (`ldrb`).
 *   unk04  a Decompress SOURCE (`u8 *`, Decompress's first parameter).
 *   unk08  the glyph's advance WIDTH. Read with `ldr`, so a word; it is both
 *          accumulated into a u16 running total and stored back through a
 *          `u8 *` out-array with `strb`, which pins neither signedness nor a
 *          narrower width. `int` is the weakest word that fits.
 * filler_01 is real padding: nothing in the ROM touches +1..+3. Unsized --
 * the terminator is the only bound. */
struct Unk08616194
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[3];
    /* 0x04 */ u8 *unk04;
    /* 0x08 */ int unk08;
};
extern struct Unk08616194 gUnknown_08616194[];
/* Same OAM-blob model again, and the same single piece of evidence: it reaches
 * nothing but PutSpriteExt's `u16 *` fourth parameter (sub_0807567C and one
 * neighbour, fanin 2). ROM begins 0x0001 followed by one (y, x, tile) triple. */
extern u16 gUnknown_086143D8[];
/* Another of the same OAM blobs, fanin 13, at the head of the 0x0848B6xx run
 * that gUnknown_0848B6CE also belongs to: ROM is 0x0001 plus one triple, i.e.
 * 8 bytes, and a second blob starts at +8. Only ever a PutSpriteExt `u16 *`.
 *
 * Wave 48, W48-H: gUnknown_081D92F8 is NOT A GLOBAL and neither is
 * gUnknown_081D92FC -- both ROM words hold 0x0848B6A0, i.e. &gUnknown_0848B6A0,
 * read straight out of baserom.gba. They are agbcc's own `-fforce-addr`
 * address-constant pool, a PAIR because the reference sits inside a loop. The
 * words at 0x081D9324 and 0x081D9328 are the same thing one blob along -- both
 * hold 0x0848B6C6. sub_0807A1B8 matched by naming gUnknown_0848B6A0 directly
 * (the promotion carries `"rodata": ["0x081D92F8"]`); do not declare a symbol
 * for any of the four addresses. */
extern u16 gUnknown_0848B6A0[];
/* Wave 43 (W43-D). The NEXT blob in that same 0x0848B6xx run: gUnknown_0848B6CE
 * is 0x0001 plus one (y, x, tile) triple, i.e. 8 bytes, so the next one starts
 * at 0x0848B6D6. Typed `u16 []` from the run it belongs to and from its
 * neighbours, NOT from its own use -- sub_08037170 is its only reader and hands
 * it to sub_0801BD00's `void *` third parameter, which constrains nothing.
 * Not const, matching every other blob in the run.
 *
 * Found from the ROM side: sub_08037170's literal pool points at 0x08090EEC,
 * whose contents are 0x0848B6D6. That word is agbcc's -fforce-addr copy of this
 * symbol's address, which is why naming this blob directly reproduces the
 * ROM's two-load sequence exactly. */
extern u16 gUnknown_0848B6D6[];
/* A TWELVE-byte u8 table, {2,3,4,5,2,3,4,5,2,3,4,5}, read as
 * `gUnknown_08615E40[proc->unk52]` with a plain `ldrb` by sub_08079EA4 and used
 * as `(entry + 4) << 12` in a PutSpriteExt OAM word -- so the entries are OBJ
 * palette or tile-bank indices, and the repeat of period 4 is what the caller's
 * +0x52 counter cycles through. The extent is proved by a type change rather
 * than by the index: +0x0c starts a run of address-shaped words
 * (0x081D9484, 0x081DA8D0, 0x081DBEC8), which cannot be part of a byte table.
 *   NOTE for tools/lc_screen.py: this symbol is reported as a `.LC` pool word
 * (fanin 1, density 4) and it is NOT one. The use is a direct base-plus-index
 * `ldrb`, not the double indirection a rerouted address constant produces, and
 * the ROM bytes are 02 03 04 05..., not an address. Dereference before
 * believing the classification -- the screen splits on fan-in and position in
 * a .rodata run, and a genuine low-fan-in ROM table inside such a run looks
 * identical to it. */
extern u8 gUnknown_08615E40[12];
/* Two more OAM blobs, this pair on PutSprite's `u16 *` rather than
 * PutSpriteExt's, drawn one after the other by sub_080748A0 through two
 * different OBJ affine slots. Each is 0x0001 plus one (y, x, tile) triple, i.e.
 * exactly 8 bytes, which the 8-byte gap between the two addresses confirms.
 *   NOTE for tools/lc_screen.py: BOTH are reported as `.LC` pool words and
 * neither is one -- same misclassification as gUnknown_08615E40 above, and the
 * spacing should have been the giveaway, since a -fforce-addr pool pair is 4
 * bytes apart and these are 8. Three of the four `.LC` words the screen
 * attributed to this batch were ordinary ROM tables. */
extern u16 gUnknown_081CC4C4[];
extern u16 gUnknown_081CC4CC[];
/* Wave 32, W32-A. Three more OBJ lists of the same kind, all handed to
 * PutSprite's `u16 *` fourth parameter by sub_08077620 -- hence not const. The
 * first goes with a sub_0804402C sprite at a different layer, the other two are
 * a pair drawn at x 0x80 and 0x78 off one y. */
extern u16 gUnknown_081CC5B0[];
extern u16 gUnknown_081CC5D0[];
extern u16 gUnknown_081CC5DE[];

/* ---- wave 20 (W20-B), the 0x08084580-0x080848B4 loader block ----
 * An array of TWO-POINTER records, and the stride is proved by the two
 * functions that read it rather than by the ROM bytes: sub_080845A8 reads
 * `[i][0]` (`lsls r0,#3; adds; ldr`) and sub_080845C4 reads `[i][1]`
 * (`lsls r2,#3` with `adds r0,#4` applied to the BASE, which is what agbcc
 * emits for the second member of an 8-byte record and NOT what a flat pointer
 * array with index `i*2+1` emits -- that spelling reassociates into
 * `lsl #1; add #1; lsl #2` and is three instructions longer). Both elements
 * go straight to Decompress's `u8 *` first parameter, so the element type is
 * `u8 *` and neither is const (Decompress does not take const). Member 0 is
 * decompressed to 0x06013300 and member 1 to 0x06013B00 + i*0x400, i.e. the
 * pair is (a shared/base tile blob, a per-index 1 KB tile blob). The count is
 * not part of the type -- nothing in this block bounds i. */
extern u8 *gUnknown_08616AC0[][2];
/* A single compressed blob, sub_080845E8's only argument to Decompress
 * (destination 0x06015300). `u8 *` for the same reason as above. */
extern u8 gUnknown_0823D980[];
/* A single compressed blob, sub_0803F374's only argument to Decompress
 * (destination 0x06013940). Same shape and same `u8 []` model as
 * gUnknown_0823D980 above -- the two accessors are a loose duplicate pair that
 * differs only in the blob and the destination (wave 20, W20-C). */
extern u8 gUnknown_081169D0[];
/* A run of 16-colour PALETTES -- 32 bytes = 16 u16 each -- flat rather than
 * 2-D on purpose. Retyped from u8 to u16 in wave 20 when a second agent settled
 * sub_08084864's return from its caller (sub_08082660 hands it straight to
 * ApplyPaletteExt's `u16 *`); the `u8` model was byte-neutral in both accessors
 * and had no oracle, which is exactly the case docs/agbcc-codegen.md warns has
 * none. The element COUNT per entry is what the 32-byte stride pins. sub_08084864
 * returns `gUnknown_0823DC38 + i * 32` and sub_0808488C returns
 * `gUnknown_0823DC38 + (i + 6) * 32`; declared as `u8 [][32]` the second one
 * DOES NOT MATCH, because fold distributes the element-size multiply over the
 * `+ 6` and folds the product into the symbol (`.word gUnknown_0823DC38+0xc0`,
 * with the runtime `adds r0,r4,#6` gone). Written as explicit pointer
 * arithmetic on a flat array the `+ 6` survives, which is what the ROM has.
 * See docs/agbcc-codegen.md, wave 20 (W20-B). The 32-byte stride and the fact
 * that entry 6 is where the second function starts are the only things pinned;
 * the element is not otherwise typed here because nothing in this block reads
 * through it. */
extern u16 gUnknown_0823DC38[];
/* Two compressed tile blobs, each Decompressed into *gUnknown_08499580 by one
 * of the duplicate pair sub_080858C0 / sub_08085908. `u8 []` and NOT `const`:
 * Decompress's first parameter is `u8 *`, the same reason gUnknown_0823D980
 * above is not const (wave 21, W21-A). */
extern u8 gUnknown_0823DE38[];
extern u8 gUnknown_0823DF48[];
/* The two fixed fallbacks returned by sub_08084864 and sub_0808488C when
 * sub_08084858(i) is non-zero -- same role, same shape, 32 bytes apart, so
 * they are almost certainly two entries of one table that the source names
 * individually. Same `u16 *` as gUnknown_0823DC38, and not proven further:
 * every caller of the two functions is still asm-resident. */
extern u16 gUnknown_0812596C[];
extern u16 gUnknown_0812598C[];
/* Only byte 1 is ever touched here, by sub_08084600 (`ldrb r0,[r0,#1]`, tested
 * against 0) and by sub_0808135C. An array rather than a struct because nothing
 * yet reads a second field, and `u8` because both reads are `ldrb`. Byte 0 is
 * untouched by either, so the extent is not pinned -- do not widen this to a
 * scalar on the strength of the two reads. */
extern u8 gUnknown_0300591C[];
/* Two u16 tables of animation frame ids, both indexed by the same 0..7 selector
 * sub_08084600 computes, and picked between by gUnknown_0300591C[1]. The first
 * is one-dimensional (`lsls r0, r5, #1`); the second is TWO frames per selector
 * (`lsls r1,r5,#1; adds r1,r1,r0; lsls r1,r1,#1` -- i.e. [i * 2 + parity]) with
 * the parity coming from DivRem(unk66, 2). u16 from the `ldrh`, and both are
 * handed to sub_08014668's fourth parameter. Neither is const: they sit 16 bytes
 * apart, which bounds gUnknown_08616FA4 at 8 entries and is consistent with the
 * selector's 0..7 range. */
extern u16 gUnknown_08616FA4[];
extern u16 gUnknown_08616FB4[];

/* ---- wave 24 (W24-C), the 0x08078 block ---- */

/* An 8-byte record table. sub_0807831C is the only reader: it indexes it with
 * gUnknown_0202FDFC.unk0c + its own proc's +0x58 (`lsls #3` = 8-byte stride)
 * and hands word 0 straight to sub_08074AAC, whose first parameter is the
 * 0xFF-terminated `const u8 *` id list -- same type as gUnknown_08615984 and
 * gUnknown_08615988, and settled by that call rather than by any read here.
 * Word 4 is not read anywhere in the ROM, so the record is bounded at 8 bytes
 * by the stride alone and filler is the honest spelling for it. */
struct Unk861500C /* 0x08 */
{
    /* 0x00 */ const u8 *unk_00;
    /* 0x04 */ void *unk_04; /* wave 30 (W30-C): sub_08076BF0 `ldr`s it and hands
                              * it straight to sub_08078480's `void *` first
                              * parameter, which that callee only parks at +0x54
                              * of the proc it starts -- so a word-wide pointer
                              * whose pointee nothing in the ROM dereferences.
                              * NOT const-qualified: sub_08078480 takes a plain
                              * `void *` and -Werror rejects the call otherwise.
                              * Was filler. */
};

extern const struct Unk861500C gUnknown_0861500C[];

/* What sub_0807831C's proc keeps at +0x54 and hands to sub_080782C0. That
 * function walks word 0 as a SIGNED byte list (`ldrsb` off a zero index
 * register, terminated by -1) using each entry to index
 * gUnknown_0202FDFC.unk12, and compares the resulting count against the two
 * `ldrb`s at +4 and +5. It is a pointer PARAMETER's type and would normally
 * live in the .c, but sub_080782C0's prototype needs it: typing the parameter
 * directly rather than assigning it to a local inside the body is what puts
 * `adds r3, r0, #0` in the prologue ahead of `mov ip, r1`, and a local costs
 * exactly that ordering. */
struct Unk80782C0
{
    /* 0x00 */ const s8 *unk_00;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
};

/* Three ROM blobs handed to sub_080785CC's fourth parameter and to nothing
 * else -- sub_08078404, sub_08078420 and sub_080783BC respectively. That
 * callee only stores the pointer to +0x54 of the proc it starts, so nothing in
 * the ROM dereferences them and `const void *` is all the evidence supports.
 * Their sizes are unpinned. */
extern const u8 gUnknown_084B9F00[];
extern const u8 gUnknown_084BA210[];
extern const u8 gUnknown_084BA480[];

/* Two more proc scripts, both only ever handed to Proc_StartBlocking:
 *   08615ACC  sub_080785CC, which fills +0x2c/+0x30 (its first two arguments),
 *             +0x54 (the blob) and +0x58 (the third). sub_08078558 is one of
 *             its methods and reads +0x2c and +0x30 back.
 *   08615BBC  sub_08078968, no fields written.
 * `const struct ProcCmd []` for the same reason as every other entry in this
 * group -- it is proc.h's parameter type and nothing here reads the data. */
extern const struct ProcCmd gUnknown_08615ACC[];
extern const struct ProcCmd gUnknown_08615BBC[];

/* The two scripts sub_08078ED4 picks between on gPlaySt.unk01 == 3.
 * Both go to Proc_Start under the caller's own parent. */
extern const struct ProcCmd gUnknown_08615D88[];
extern const struct ProcCmd gUnknown_08615DD8[];

/* NOT GLOBALS: gUnknown_081D92D8, gUnknown_081D92DC, gUnknown_081D92E0 and
 * gUnknown_081D92E4 are four consecutive `-fforce-addr` address-constant words
 * (addends 0, 4, 8, 0xc, the textbook pattern), and all four hold the SAME
 * value -- 0x03001FFC, i.e. &gUnknown_03001FFC, the volatile BLDY shadow
 * declared in include/hardware.h. Dereferencing them in baserom.gba is what
 * settles it; the splitter invents a `gUnknown_<addr>` for a pool word and a
 * real global alike and cannot tell them apart.
 *
 * So sub_08078BDC and sub_08078C18 are a fade-in / fade-out pair written
 * against gUnknown_03001FFC directly, and their apparent double indirection
 * (`ldr r2, =sym; ldr r1, [r2]; ldrh`) is the pool word, not a pointer
 * variable. Declaring a `u16 *` here instead compiles to THREE loads and does
 * not match. */

/* ---- wave 24 (W24-C), the 0x08078 block, second pass ---- */

/* Four 4-byte records, read out of baserom.gba as
 *     {0x61, 0x69, 0x28, 0xff}  {0x62, 0x6a, 0x0e, 0xff}
 *     {0x63, 0x6b, 0x16, 0xff}  {0x64, 0x6c, 0x1e, 0xff}
 * and walked by sub_08078358 alone (`adds r4, #4`, i = 0..3). Bytes 0 and 1 are
 * tags handed to sub_0803CBD8; bytes 2 and 3 are a 0xFF-terminated id list
 * handed to sub_08074AAC, and byte 2 is ALSO read on its own to index
 * gUnknown_0202FDFC.unk12. `s8` for that pair because the read is `ldrsb` and
 * because 0xff is the -1 terminator sub_080782C0 walks its own list against --
 * the same element type, one indirection down. The `const u8 *` cast at the
 * sub_08074AAC call is the cost of that choice and is byte-free.
 *
 * This is the record the gUnknown_08615984 comment above predicted: the
 * 0x08615974-0x0861598B block really is two unrelated things at 4-byte
 * granularity, and reading the ROM confirms it rather than inferring it. */
struct Unk8615974 /* 0x04 */
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ s8 unk_02[2];
};

extern const struct Unk8615974 gUnknown_08615974[];

/* The fourth script sub_08078198 tests for existence, alongside 086147FC,
 * 0861485C and 08614894 which are already declared above. */
extern const struct ProcCmd gUnknown_08614614[];
/* Proc_Start'd by sub_08078E94 under a caller-supplied parent when
 * sub_08078E20's predicate is false. */
extern const struct ProcCmd gUnknown_08615D70[];

/* One of the word-stride run of RAM pointers at 0x08499588..0x084995A0, of
 * which 0849959C and 084995A0 are already declared above. NOT an -fforce-addr
 * pool word: three functions in two translation units (sub_08017994,
 * sub_080179AC, sub_080185A0) load this same address, and -fforce-addr gives
 * each function its own private copy. `u16 *` because sub_080179AC hands it
 * straight to sub_08012BC8(u16 *, ...) and sub_080179D0(u16 *) clears a
 * 23-column, 4-row halfword window through it. Wave 26, W26-C. */
extern u16 *gUnknown_08499588;
/* The next word of that same 0x08499588..0x084995A0 run. Wave 35 (W35-H):
 * sub_0802C214 is two sub_0802C1F0 byte-copies whose sources are
 * gUnknown_08499588 and this word and whose destinations are gUnknown_08499578
 * and gUnknown_08499580 at the same +0x380 offset and the same 0x180 length --
 * so 08499588/0849958C pair up exactly as 08499578/08499580 do, and it takes
 * the same `u16 *` those two carry. NOT an -fforce-addr pool word: it sits in
 * the middle of a declared run of RAM pointers and holds a different address
 * from each of its neighbours. */
extern u16 *gUnknown_0849958C;

/* Wave 35 (W35-H). Two more gUnknown_03001470 scripts: sub_0802D2A0 hands
 * gUnknown_0849A9F8 to Proc_Start on PROC_TREE_3 and sub_0802D5E8 hands
 * gUnknown_0849AFE8 to sub_080152C0, the same two entry points every other
 * script symbol in this block reaches. */
extern const struct ProcCmd gUnknown_0849A9F8[];
extern const struct ProcCmd gUnknown_0849AFE8[];

/* Wave 35 (W35-H). A table of 4-byte records whose FIRST byte is all anything
 * reads: sub_0802D9B8 does `lsls #2; adds; ldrb` off a proc member and
 * sub_0802D918 reads element 0 the same way, both handing the byte to
 * sub_0803A9C8 / sub_0803AA78's u8 parameter. Declared as the byte array the
 * accesses actually show rather than as a struct -- the stride is visible, the
 * other three bytes are not. */
/* Wave 41, W41-D. The build-menu entry list sub_0802D67C fills: 4 bytes per
 * entry, [0] the unit id and [1] a "greyed out" flag, terminated by a final
 * entry whose [0] is 0. The stride is a bare `lsls #2` on this flat spelling
 * and the two promoted readers (c_0802D918.c, c_0802D9B8.c) index it flat.
 *   `u8 [][4]` WAS TRIED and is WORSE, which is worth recording because the
 * row form looks obviously right: it does put the flag's `+1` in the store
 * displacement where the flat form spends an `adds r0,#1`, but agbcc then
 * hoists `base + 1` into its own callee-saved pseudo for the whole loop, which
 * costs two more instructions than it saves and displaces the `-1` sentinel
 * that the ROM keeps in sl. Left flat; see work/sub_0802D67C/. */
extern u8 gUnknown_02023830[];

/* Wave 35 (W35-H). The link-error screen's graphics, all three confirmed
 * against the ROM image. 0x0816DB88 is 8 WORDS (sub_0802E960 CpuFastSets it to
 * 0x05000000, i.e. one 16-colour BG palette), and the next blob starts at
 * 0x0816DBA8 -- exactly 0x20 bytes later, which is what fixes the extent.
 *
 * 0x0849B020 and 0x0849B024 are POINTER VARIABLES and not -fforce-addr pool
 * words: they hold 0x0816DBA8 and 0x0816DF40, two DIFFERENT addresses neither
 * of which is their own, and sub_0802E960 reaches each with the `ldr =sym;
 * ldr [sym]` a real ROM pointer emits. Each is handed straight to Decompress,
 * whose first parameter is `u8 *`. */
/* Wave 35 (W35-H): a halfword count. sub_0802D918 reads it `ldrh`, subtracts 1
 * and `strh`s the result into gUnknown_03001470[i].unk22, which is s16 -- so
 * this is the extent that member is initialised from. Width from the load;
 * nothing sign-extends it, so u16 is the weakest fit. */
extern u16 gUnknown_0300055A;
extern const u16 gUnknown_0816DB88[];
extern u8 *gUnknown_0849B020;
extern u8 *gUnknown_0849B024;
/* A gUnknown_03001470 script blob: sub_08017688 is
 * `sub_080152EC(gUnknown_0848A1EC, 0)->unk1e = a`. The ROM words there are
 * {0, 0x001E001E, 0x0803B5E9, 0x00020000}, exactly the gUnknown_0849A00C
 * shape, so `const struct ProcCmd []` like its 0848A140 / 0848A150
 * neighbours. */
extern const struct ProcCmd gUnknown_0848A1EC[];
/* 0x0808E558 is NOT a global -- do not declare one. It is agbcc's own
 * -fforce-addr address-constant pool and the ROM proves it: the four words
 * there are 0x0200C420, 0x0200C078, 0x0200C528, 0x0200C528, a run of the
 * ADDRESSES of globals that are already declared above. sub_080176C0 reads it
 * as `ldr r1, =0x0808E558; ldr r4, [r1]`, which is the two-level -fforce-addr
 * load and not a pointer variable. The honest `gUnknown_0200C420.unk00`
 * spelling reproduces that function exactly, including the `ldr r1, [r1]`
 * reload before the second field -- the pool word's address is what stays live
 * in r1, so each use re-loads the object address from it. Wave 26, W26-C. */

/* ------------------------------------------------------------------ *
 * Wave 27, W27-B: address-locality block 0x08013                      *
 * ------------------------------------------------------------------ */

/* The proc script sub_080130DC starts, and the ROM names the proc's own
 * callback: the words are {0x00000011, 0, 0x00000004, 0x0801311D, 0x0000000E,
 * 0}, which as ProcCmds is PROC_CMD_END_IF_DUP, then PROC_CMD_ONEND with
 * dataPtr 0x0801311D (&sub_0801311C with the THUMB bit set), then
 * PROC_CMD_SLEEP. That is what ties the two together: sub_080130DC stores three
 * halfwords at +0x64/+0x66/+0x68 of the proc it starts here, and sub_0801311C
 * is the ONEND handler that reads +0x68 back as an `ldrsh`. */
extern const struct ProcCmd gUnknown_0848936C[];

/* A word table of proc-script POINTERS, not a script itself: the first three
 * words are 0x084893DC, 0x084893F0 and 0x08489404, all inside the 0x08489
 * proc-script run, and the fourth is 0. sub_08013338 indexes it by words and
 * stores the word whole at +0x4c of the proc it just started, so the elements
 * are `const struct ProcCmd *` -- the same reasoning as gUnknown_086147FC's
 * neighbours. */
extern const struct ProcCmd *gUnknown_0848950C[];

/* The proc script sub_08034CD4 starts on tree 3. ProcCmds: {ONEND, 0,
 * 0x08034F8D}, {CALL, 0, 0x08034F7D}, {CALL, 0, 0x0803ED55}. */
extern const struct ProcCmd gUnknown_0849F790[];

/* A byte flag sub_08013580 tests against the literal 1 before re-uploading the
 * whole gPal shadow to palette RAM -- a "palette is dirty" latch. Plain
 * `ldrb`, and nothing sign-extends or arithmetically compares it, so `u8`. */
extern u8 gUnknown_03000048;

/* Wave 35, W35-K: the key-repeat timing pair, read only by sub_0801348C (the
 * key-state update it feeds gUnknown_03002090 through sub_08013510).
 * gUnknown_030030C8 is the INITIAL delay -- it is reloaded on every frame the
 * held mask changes or is empty -- and gUnknown_03002F94 the repeat INTERVAL,
 * reloaded each time the countdown at ->unk10 reaches 0. Both are reached with
 * a whole-word `ldr` and truncated by the `strh` into that u16 member, so the
 * objects are 32 bits wide; signedness is unproved, nothing compares them and
 * the store is byte-identical either way. Neither is ever written in the ROM,
 * so the values are set at their definition. */
extern int gUnknown_03002F94;
extern int gUnknown_030030C8;

/* Wave 35, W35-K. The key-state buffer sub_08013510 refreshes every frame.
 *
 * It is the SAME TWENTY BYTES as `struct KeySt` in hardware.h -- 0x03002090 is
 * `((struct KeySt *)&gUnknown_03002040)[4]`, the same array c_08064410.c
 * indexes -- and it is deliberately NOT spelled that way, because
 * sub_0801348C is the object's PRODUCER and it contradicts KeySt's names for
 * every member from 0x04 on. Those names were inherited from FE8 and no AW2
 * function has ever corroborated them; what sub_0801348C actually does is
 *
 *   unk08  the raw held mask this frame (the value handed in)
 *   unk0e  the raw held mask LAST frame (unk08 copied before it is overwritten)
 *   unk0c  newly-pressed = unk08 & ~unk0e
 *   unk0a  the repeat output: newly-pressed, plus the whole held mask again on
 *          each tick of the unk10 countdown
 *   unk10  that countdown, reloaded from gUnknown_030030C8 (initial delay)
 *          whenever the mask changes or is empty and from gUnknown_03002F94
 *          (repeat interval) each time it expires
 *   unk00/unk02/unk04/unk06  published copies of unk08/unk0a/unk0c/unk0e
 *
 * so 0x04 is newly-pressed rather than KeySt's `held`, 0x06 is last frame
 * rather than `repeated`, 0x08 is the held mask rather than `pressed`, and
 * 0x10 is a countdown rather than `pressed2`. The two 0x00/0x02 names KeySt
 * does carry AGREE with this: its own note has consumers testing unk00 against
 * L and R (a held mask) and unk02 against Right|Up and Left|Down to step a
 * cursor (a repeat mask).
 *
 * Renaming KeySt is the right fix and is byte-neutral -- same offsets, same
 * widths -- but it touches every promoted file that uses `.held`/`.repeated`/
 * `.pressed`, so it is left for a wave that can re-verify all of them rather
 * than done mid-wave beside three other agents. */
struct Unk03002090 /* 0x14 */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
    /* 0x0c */ u16 unk0c;
    /* 0x0e */ u16 unk0e;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
};
extern struct Unk03002090 gUnknown_03002090;

/* ------------------------------------------------------------------ *
 * Wave 27, W27-B: address-locality block 0x0803C                      *
 * ------------------------------------------------------------------ */

/* A ROM dispatch table of 24-byte rows. The stride is measured, not assumed:
 * sub_0803C864 indexes it `((a << 1) + a) << 3`. Two members are readable,
 * and the ROM names the callee outright -- the words of row 0 are
 * {0x0AEE0014, 0x000003E8, 0x0803C40D, 0x0803CBA1, 0x00000020, 0x084C2F88}
 * and of row 1 {0x0AEF0014, 0x00001388, 0x0803C435, 0x0803CBA1, 0x00000028,
 * 0x084C2FB8}. 0x0803CBA1 is &sub_0803CBA0 with the THUMB bit set, and
 * sub_0803CBA0 is already declared `void (int, int)`, which is exactly how
 * sub_0803C864 calls through unk0c: `f(row->unk10, 1)`, a two-argument
 * indirect call (`bl _call_via_r2`, and the register index counts the
 * arguments). unk10 is 0x20 / 0x28, i.e. the bit ids sub_0803CBA0's third
 * range accepts.
 *
 * unk08 is a second callback (0x0803C40D / 0x0803C435) and unk14 a ROM
 * pointer (0x084C2F88 / 0x084C2FB8), but nothing matched reads either, so
 * they stay in the filler rather than being guessed at. */
struct Unk0849EDB0 /* 0x18 */
{
    /* 0x00 */ u8 unk00; /* Wave 35, W35-L. `ldrb`, handed straight to
                          * sub_0801F2AC's `int` first parameter by
                          * sub_080487B4. */
    /* 0x01 */ u8 filler_01[0x01];
    /* 0x02 */ u16 unk02; /* Wave 35, W35-L. `ldrh`, used by sub_080487B4 as
                           * the index into the `u8 *` table
                           * gTextTable. */
    /* 0x04 */ u32 unk04; /* Wave 35, W35-L. A word (`ldr`) that sub_080487B4
                           * passes as sub_08014B0C's fourth argument. INTEGER
                           * and not a pointer: sub_08047B98 fills the same
                           * parameter with `p->unk1f + 1`, and the two call
                           * sites are what settle it -- nothing dereferences
                           * this field.
                           *   Wave 56, W56-K: retyped `int` -> `u32`, which is
                           * byte-neutral at sub_080487B4's call site because
                           * sub_08014B0C's fourth parameter is already `u32`.
                           * The sign comes from sub_08049360, which compares it
                           * twice against SIGNED members of Unk084C30F8
                           * (`bhs` against unk028, `bls` against the literal
                           * 999) -- an unsigned compare between an s32 and this
                           * field only happens if this field is the unsigned
                           * one. c_080487B4.c, c_0803C670.c, c_0803C864.c and
                           * c_0803C890.c re-verified byte-exact. */
    /* 0x08 */ int (*unk08)(int); /* Wave 37, W37-Q1. The second callback
                          * (0x0803C40D / 0x0803C435). sub_0803C814 calls it
                          * `bl _call_via_r1` -- the trampoline index counts the
                          * arguments, so exactly ONE -- with unk10 as that
                          * argument, and compares the result against -1;
                          * sub_0803C784 calls it the same way and compares
                          * against 1. So `int (*)(int)`. sub_0803C784 also
                          * hands the POINTER ITSELF to sub_0803C750, which
                          * `str`s it into gUnknown_02027FB0[].unk00, and that
                          * is what fixes the field as a pointer rather than an
                          * int the two callers happen to call through. */
    /* 0x0c */ void (*unk0c)(int, int);
    /* 0x10 */ int unk10;
    /* 0x14 */ const u8 *unk14; /* Wave 56, W56-K. Was filler_14[0x04]; same
                          * extent, the struct stays 0x18. sub_08049360 loads
                          * it `ldr` off `base + i * 0x18 + 0x14` and hands it
                          * straight to sub_080485DC, whose parameter is
                          * `const u8 *` -- the same reasoning that typed
                          * Unk084C30F8's unk850. */
};
/* NOT `const`, and sub_0803C784 is the only thing in the tree that can tell:
 * it reads `gUnknown_0849EDB0[i].unk08` twice with an indirect call in
 * between, and the ROM keeps the ADDRESS of that member in r4 across the call
 * and RE-LOADS the pointer (`ldr r0, [r4]; bl sub_0803C750`). A `const` array
 * is TREE_READONLY, so gcc treats the MEM as unchanging, keeps the VALUE in
 * r4 and emits `adds r0, r4, #0` -- one instruction shorter and the wrong
 * bytes. Every other reader loads each member once and is byte-neutral either
 * way. Wave 37, W37-Q1. */
extern struct Unk0849EDB0 gUnknown_0849EDB0[];

/* 0x08091140 is NOT a global -- do not declare one. It is agbcc's own
 * -fforce-addr address-constant pool, the same case as 0x0808E558: the word
 * there is 0x02000000, i.e. &gUnknown_02000000, and its neighbours are
 * 0x08499590, 0x030033EC and 0x03003F2C, a run of the ADDRESSES of other
 * globals. sub_0803CF04 reads it as `ldr r4, =0x08091140; ldr r1, [r4]` and
 * then RE-LOADS `ldr r1, [r4]` for the second use, which is the two-level
 * -fforce-addr pattern -- the pool word's address is what stays live, so each
 * use reloads the object address from it. The honest `gUnknown_02000000`
 * spelling reproduces both loads. Its sibling sub_0803CF3C names the same
 * object once and gets the ordinary one-level `ldr r0, =gUnknown_02000000`,
 * which is the whole difference between the two forms: how many uses. */

/* ------------------------------------------------------------------ *
 * Wave 27, W27-B: address-locality block 0x08037                      *
 * ------------------------------------------------------------------ */

/* A gUnknown_03001470 script blob: sub_08037D64 is
 * `sub_080152EC(gUnknown_0849D53C, 0)->unk1e = a`, the same instruction stream
 * as sub_08017688 with this pool word in place of gUnknown_0848A1EC.
 * `const u8 []` for the gUnknown_0849A0F0 reason -- nothing indexes it and
 * sub_080152EC takes `const void *`.
 *
 * NOTE on gUnknown_0848A1EC, which the sub_08017688 comment types
 * `const struct ProcCmd []`: the ROM does NOT support that, and this blob is
 * the counter-example. The four words at 0x0849D53C are
 * {0x080375BD, 0x00020000, 0, 0x00000001} -- word 0 is &sub_080375BC with the
 * THUMB bit set, i.e. a callback in the FIRST word, where struct ProcCmd would
 * put {s16 opcode, s16 dataImm}. Read as ProcCmd that is opcode 0x75BD, which
 * is not in proc.h's enum. gUnknown_0848A1EC's words {0, 0x001E001E,
 * 0x0803B5E9, 0x00020000} read as ProcCmd give entry 0 = PROC_END, which would
 * make the script a no-op. Both are 16-byte {callback, flags, ptr, imm}
 * records of the gUnknown_03001470 subsystem, not proc scripts. The typing is
 * byte-neutral (both reach a `const void *` parameter), so nothing catches it;
 * flagged rather than changed, since sub_08017688 is already promoted. */
extern const u8 gUnknown_0849D53C[];

/* Two proc scripts sub_08037124 hands to Proc_EndEach. Genuine ProcCmds, and
 * the ROM words prove it here: 0x0855379C is {opcode 0x0E SLEEP, dataImm 1,
 * dataPtr 0} then {opcode 0x02 CALL, dataImm 0, dataPtr 0x080531D5}. */
extern const struct ProcCmd gUnknown_0855379C[];
/* A gUnknown_03001470 blob, NOT a proc script: sub_08037124 hands it to
 * sub_0801537C(const void *), the stopper half of the start/stop pairing
 * documented at gUnknown_0849A108. Its words are {0x08053435, 0x00020000,
 * 0x080534A1, 0x00010000, ...}, i.e. THUMB callbacks in word 0 of each 8-byte
 * record -- the same non-ProcCmd shape as gUnknown_0849D53C above. `const u8 []`
 * as the weakest model that gives the clean pool word. */
extern const u8 gUnknown_08553820[];

/* An LZ77 blob: sub_08037150 is `Decompress(gUnknown_08124478, dst)`, and the
 * ROM header word 0x00048010 is the LZ77 (type 0x10) marker with an
 * uncompressed size of 0x480. `u8 []` and non-const because Decompress's first
 * parameter is a plain `u8 *`. */
extern u8 gUnknown_08124478[];

/* A small ROM byte table sub_080375A4 indexes with its own u8 parameter and
 * hands to sub_08037448 -- `adds r0, r0, r1; ldrb r0, [r0]`, so `u8` elements.
 * The first eight bytes are 00 01 07 02 07 00 00 00; the words that follow at
 * 0x08090EF8 are agbcc's own -fforce-addr address pool (0x03003F68,
 * &gPlayers, ...), which is what bounds the table's size. */
/* Wave 43 (W43-D): gUnknown_08090EEC is NOT a global and must never be
 * declared as one. The word at 0x08090EEC holds 0x0848B6D6 -- an ADDRESS -- and
 * sub_08037170 is its only referencer, which is the signature of agbcc's own
 * -fforce-addr .rodata pool word rather than a pointer variable. Its
 * neighbours in this run are the same thing (0x08090EF8 holds 0x03003F68,
 * i.e. &gUnknown_03003F68; the brief records 0x08090A60 and 0x08090B00 holding
 * &gPlaySt), so check each gUnknown_0809xxxx against baserom.gba
 * before typing it. gUnknown_08090EF0 immediately below is the counter-example
 * and IS a real table.
 *
 * Declared as a pointer global, sub_08037170 needs THREE loads
 * (pool -> .LC -> value) where the ROM has two, and +12 bytes. Naming
 * gUnknown_0848B6D6 directly lets agbcc emit this very word itself. */
extern const u8 gUnknown_08090EF0[];

/* A whole-word scratch slot in the 0x08037 palette-cycling code, and it is
 * already in aw2bhr.lds. sub_08037750 stores its argument here with a plain
 * `str`; sub_08037790 reads it back with `ldr` and computes
 * `unk * 0x20 + 0x1c` as a palette byte offset. Word-wide both ways, so `int`
 * is the weakest fit -- nothing sign-extends or compares it. */
extern int gUnknown_0300057C;

/* Two palette blobs, and they are CONTIGUOUS: 0x081253F0 + 0x20 = 0x08125410.
 * sub_08037750 hands 081253F0 to ApplyPaletteExt(u16 *, u32, u16) for 0x20
 * bytes, so `u16 []` and non-const (ApplyPaletteExt takes a plain pointer).
 * 08125410 is the cycling table behind it: sub_08037790 adds a BYTE offset of
 * `(gGameClock & 0x3c) >> 1` to it and hands it to sub_0801368C for 2
 * bytes, i.e. one colour picked by a 16-phase frame counter. `u16 []` for the
 * same consumer reason; the byte-offset arithmetic is spelled through a
 * `(u8 *)` cast at the one call site because the ROM shifts by 1, not 2. */
extern u16 gUnknown_081253F0[];
extern u16 gUnknown_08125410[];

/* --- the 0x0803B block (wave 27, W27-C) ---------------------------------- */
/* An 8-byte dispatch table: sub_0803BBA8 indexes it with gUnknown_030033FC
 * (`lsls #3`), calls the word at +0 through `_call_via_r3` with no arguments,
 * and `strb`s the word at +4 into gUnknown_0200C420.unk0d. The stride is read
 * off the shift and is not a guess; the second member being a full word is
 * read off the `ldr`, and the truncation to a byte happens at the store. */
struct Unk849EACC
{
    /* 0x00 */ void (*unk00)(void);
    /* 0x04 */ int unk04;
};
extern const struct Unk849EACC gUnknown_0849EACC[];
/* The script sub_0803BA4C starts on tree 3 after setting
 * gPlaySt.unk01 = 1 -- the same shape as the 0849EBBC / 0849EC1C /
 * 0849ECE0 starters listed above, with 1 as its mode constant. */
extern const struct ProcCmd gUnknown_0849EB34[];

/* --- the sub_08036B34 / AgbMain unit (wave 27, W27-C) --------------------- */
/* All four of the RAM cells below are cleared by sub_08036B4C's boot reset and
 * every write there is a bare 0, which fixes only the WIDTH, never the
 * signedness. Each carries the weakest type its width allows. */
/* Whole word (`str`). sub_08000DA8 and sub_08001038 also write it -- the
 * comment on sub_0801F00C in unknown-functions.h describes sub_08001038 as
 * "raising" it, so a level/counter rather than a pointer. */
extern int gUnknown_030040A0;
/* One byte (`strb`). */
extern u8 gUnknown_03004094;
/* Whole word. sub_08022064 reads it back with `ldr` and tests it against a
 * value, so a word and not a narrower cell. */
/* Wave 36 (W36-L): RETYPED from `int` to `volatile u32`, both halves read off
 * sub_08022048's `gUnknown_03003330++; switch (gUnknown_03003330 % 0x32)`.
 * UNSIGNED because the modulo calls `__umodsi3` and not `__modsi3`; VOLATILE
 * because the ROM RE-LOADS the word after the increment's `str` (`ldr; adds #1;
 * str; ldr`) instead of reusing the value it just stored. Same pair as the
 * gUnknown_030043F0 / gUnknown_03004078 frame counters sub_08021DD8 drives.
 * The only promoted user, src/main.c, just stores 0 to it, which
 * is one `str` either way; re-verified byte-identical after the change. */
extern volatile u32 gUnknown_03003330;
/* Halfword (`strh`); sub_0802BC40 is the other writer. No signed reader. */
extern u16 gUnknown_030033F0;
/* Three EWRAM/IWRAM regions AgbMain hands to other subsystems as bare
 * addresses and never dereferences itself: gUnknown_02003000 is the 0x8000-byte
 * buffer it passes to sub_08014DA8, and gUnknown_02000000 / gUnknown_03003064
 * are arguments 3 and 5 of sub_0801A79C. `u8 []` is the weakest spelling that
 * makes the address available; NOTHING here settles an element type. */
extern u8 gUnknown_02000000[];
extern u8 gUnknown_02003000[];
extern u8 gUnknown_03003064[];

/* Wave 53, W53-B. gUnknown_02000000 IS ONE STRUCTURED RECORD, at least 0xDAC
 * bytes long, and it is the map-editor / battle SAVE-STATE block. Keep the
 * symbol `u8 []` -- the same discipline gUnknown_08499590 carries -- but reach
 * it through a struct declared LOCALLY in the .c and cast onto it. The layout
 * is not guessed: it is forced by arithmetic, and sub_08016F38 (save) and
 * sub_08017208 (restore) are exact mirrors of each other over every field, so
 * each offset has two independent witnesses.
 *
 *   0x0000 u16                        <-> gUnknown_03004080
 *   0x0002 u16                        <-> gUnknown_030033EC
 *   0x0004 struct Unk802C57C          <-> gUnknown_030033E4   (one word)
 *   0x0008 struct Unit         <-> gUnknown_03004490   (ldm/stm, 12 B)
 *   0x0014 u8 [5][0x3c]               <-> gUnknown_02023284   (5 memcpys)
 *   0x0140 u8 [0x48]                  <-> gPlaySt   (one memcpy)
 *   0x0188 struct Unit [4*51]  <-> gUnknown_02022684[i*64+j]
 *   0x0b18 int [4]                    <-> gUnknown_030033F4[] (u8 <-> word)
 *   0x0b28 (0x70 unaccounted)
 *   0x0b98 struct Unk03002F08         <-> gUnknown_03002F08   (8 B)
 *   0x0ba0 void (*)(void)             <-> gUnknown_03002F20
 *   0x0ba4 bool8 (*)(void)            <-> gUnknown_03001FF0
 *   0x0ba8 u32                        <-> gUnknown_03001FD4
 *   0x0bac u8                          = sub_08016F38's parameter, as a flag
 *   0x0bae..0x0bb6 five u16           <-> *gUnknown_08499590 +0,+2,+4,+6,+0x10
 *   0x0bb8 { u8 y; u8 x; u16 v; } []   = a 0xFFFF-terminated CHANGE LIST of
 *                                        map cells that differ from the base
 *                                        plane, replayed on restore
 *   0x0d28 struct Unk02028360 [16]    <-> gUnknown_02028360
 *   0x0da8 ...                         = handed to sub_08045700 / sub_080456B8
 *
 * Three of the extents are exact and self-checking rather than assumed:
 * 0x14 + 5*0x3c = 0x140, 0x140 + 0x48 = 0x188, and 0x188 + 4*51*12 = 0xB18.
 * The 12-byte stride at 0x188 is `struct Unit` because that is what
 * gUnknown_02022684 is declared as and the copy is a whole-element ldm/stm.
 *
 * TWO SPELLING RULES fall out of this and both were measured (see
 * docs/agbcc-codegen.md, "A pointer LOCAL bound to a symbol"):
 *   - bind the base to a POINTER LOCAL. Written flat off the symbol,
 *     `&gUnknown_02000000[i*0x3c + 0x14]` folds the 0x14 into the pool word's
 *     ADDEND and emits two instructions where the ROM has three; with a pointer
 *     local in a register no addend is available and the runtime `adds #0x14`
 *     comes back.
 *   - the 0x188 array must be FLAT `[4*51]` indexed `[i*51 + j]`, not
 *     `[4][51]` indexed `[i][j]`. The 2-D form scales i by 51*12 in the outer
 *     preheader; the ROM keeps i*51 unscaled there and multiplies (i*51 + j) by
 *     12 in the inner body.
 *
 * Do NOT declare gUnknown_0808E550 or gUnknown_0808E554: both ROM words hold
 * 0x03003FC0, verified in baserom.gba, so they are agbcc's own -fforce-addr
 * address constants for &gPlaySt -- the same case 0x0808E558 already
 * records two comments below. Naming gPlaySt honestly reproduces the
 * two-level `ldr rA,=word; ldr rB,[rA]` chain in both functions. */
/* The 5 x 0x3c staging area gUnknown_02000000 +0x14 mirrors. `u8 []` because
 * both readers reach it as `&gUnknown_02023284[i * 0x3c]` with a clean pool
 * word and a runtime add, and hand it to sub_0808B6E8, whose parameters are
 * `void *` / `const void *`; non-const because sub_08017208 writes it. The
 * 0x12c extent is the copy length, not a proved bound. Wave 53, W53-B. */
extern u8 gUnknown_02023284[];
/* The proc script sub_08036C2C starts on PROC_TREE_3. */
extern const struct ProcCmd gUnknown_08553754[];

/* ---- wave 27 (W27-A) ---- */

/* The m4a sound driver's XCMD dispatch table. sub_080717C4 (`ply_xcmd` in the
 * Fire Emblem decomps, on the shared Intelligent Systems engine) fetches one
 * byte from the track's command cursor, scales it by 4 and `ldr`s a function
 * pointer out of this table, then calls it through `_call_via_r2` -- so the
 * entries are `void (*)(mplay, track)` and the index is a plain byte.
 * `const` because it is ROM and nothing writes it. */
extern void (*const gUnknown_081BA00C[])(void *, void *);

/* A single RAM function pointer, NOT a table: sub_080717E4 is
 * `ldr r2,=gUnknown_03005740; ldr r2,[r2]; bl _call_via_r2`, i.e. one load of
 * the pointer VALUE with no index. Two arguments, off the `_call_via_r2`
 * register index, and both are this wrapper's own parameters passed through
 * untouched, so neither is typed by anything here. Already bound at
 * 0x03005740 by aw2bhr.lds, so this only declares it. */
extern void (*gUnknown_03005740)(void *, void *);

/* Two 0x20-byte ROM palettes, one 16-colour bank each, and NOT proc scripts --
 * see the sub_08071B28 note in unknown-functions.h, which is where the
 * mis-reading was. Each is CpuSet 0x10 halfwords into &gPal[a * 16] by
 * sub_08071C84 / sub_08071CA4, and handed to sub_08071B28 as the value it
 * stashes at the fade record's +0x20. `const u16 []` is the weakest model that
 * fits: 16 colours is what the 0x10-halfword count says, and nothing indexes
 * either symbol. */
extern const u16 gUnknown_08613F54[];
extern const u16 gUnknown_08613F74[];

/* ROM blob handed straight to sub_0801BD00's `void *` third parameter by
 * sub_08011704, with no arithmetic -- the same slot, and the same reasoning,
 * as gUnknown_0849957C and the other entries that note names. */
extern const u8 gUnknown_0848930C[];

/* ROM blob handed to sub_080152EC as its `const void *` first argument by
 * sub_08012FB8, with the literal 0 second argument -- the same install shape as
 * gUnknown_0849D41C and the rest of that group. */
extern const u8 gUnknown_08489354[];

/* Five more halfword IWRAM cells, all of them `strh`-only so far and all of
 * them addressed by aw2bhr.lds already.
 *
 * gUnknown_03000042 sits between the two named neighbours gUnknown_03000040
 * and gUnknown_03000044, and sub_0801220C copies gUnknown_03000044 into it
 * with a plain `ldrh`/`strh` pair -- so it is the same width as its source and
 * a snapshot of it, not a third member of an aggregate (three separate pool
 * words, the same reading gUnknown_03000044/46 already carry).
 *
 * The other four are only ever CLEARED, by that one function, so nothing here
 * settles their signedness or what they count -- `u16` is the store width and
 * no more. Flagged rather than claimed. */
extern u16 gUnknown_03000042;
extern u16 gUnknown_03001FD0;
extern u16 gUnknown_030024C8;
extern u16 gUnknown_03002F04;
extern u16 gUnknown_030030B0;

/* ---- the 0x0801B000 block (wave 28) ---------------------------------- */

/* 0x0200CD0C -- a one-byte gate read by all six wrappers at 0x0801B598 ..
 * 0x0801B66C (sub_0801B598, sub_0801B5C0, sub_0801B5E8, sub_0801B618,
 * sub_0801B648, sub_0801B66C). Every one of them does `ldrb; cmp #1` and
 * dispatches only when it equals 1; five of the six return a constant 1 and the
 * sixth returns its own second parameter when it does not. u8 is the access
 * width in all six. NOT const -- nothing in this block writes it, so whatever
 * arms it lives elsewhere; and nothing here says what the value 1 means, so the
 * comparison against a small constant is all that is claimed. */
/* Wave 31, W31-B. The five arguments sub_0801A79C parks before it starts the
 * link scan -- four whole words (`str`) and one byte (`strb`). Nothing that is
 * matched reads them back: the only other users, sub_0801A7D8, sub_0801ABF8 and
 * sub_0801ADC8, are all still asm, so `int` is the weakest model that fits the
 * store width and the shape is otherwise unproved. */
/* Wave 31, W31-B. The flash chip's "program one byte" routine, installed by
 * sub_0808AB8C from +0x0c of the descriptor it selects. sub_0808B184 calls it
 * `bl _call_via_r3`, so THREE arguments -- r0 a literal 1, r1 the destination
 * and r2 the byte -- and re-narrows the result with `lsls #0x10; lsrs #0x10`,
 * which is a `u16` return. */
extern u16 (*gUnknown_03005C70)(int, u8 *, u8);
/* Wave 31, W31-B. A whole-word "already initialised" latch: sub_0808BBA4 runs
 * the 0x086170EC constructor list only while it is still zero, and sets it to 1
 * on the way in. `int` is the weakest model -- the only test is `cmp r0, #0`,
 * so nothing here fixes the signedness. */
extern int gUnknown_03000F80;
extern int gUnknown_0200CC24;
extern int gUnknown_0200CC28;
extern int gUnknown_0200CC2C;
extern u8 gUnknown_0200CC30;
extern int gUnknown_0200CC34;
extern u8 gUnknown_0200CD0C;

/* Wave 40 (W40-F). 0x02002000 -- the 0x1000-byte staging buffer the flash
 * wrappers fill and verify (sub_0801B66C writes it, sub_0801B648 verifies it,
 * sub_0801B6A8 0xff-fills it). sub_0801B09C both checksums it byte by byte and
 * reads a save-block header out of its first eight bytes plus its last one, so
 * `u8 []` is the declared type and the header layout is a cast view local to
 * src/decomp/c_0801B09C.c rather than this array's type.
 *
 * gUnknown_0808EF60, which asm/code.s shows in sub_0801B09C's literal pool, is
 * NOT a variable and must not be declared as one: 0x0808EF60 is a word in
 * data/rodata.s whose contents are 0x02002000, i.e. agbcc's own -fforce-addr
 * address-constant slot for THIS array. `ldr r0, =gUnknown_0808EF60;
 * ldr r3, [r0]` is the ordinary two-load materialisation of the array's
 * address. Measured, not assumed: modelling it as
 * `extern struct X *gUnknown_0808EF60;` makes the compiler emit its own
 * address-constant slot on top of the pointer load and costs one extra `ldr`
 * at every read. */
extern u8 gUnknown_02002000[];

/* Wave 40 (W40-F). 0x0200CC38 -- six parallel per-slot arrays of 0x10 slots
 * each, reset together by sub_0801B4C0 and indexed one at a time elsewhere
 * (sub_0801B018 touches only unk20).
 *
 * ONE struct rather than six separate arrays, and the codegen is what says so:
 * sub_0801B4C0 keeps the base address in r6 across its whole loop and reaches
 * every member as `base + <const>` computed at RUNTIME, which is -fforce-addr
 * on a single object. Six separate arrays would each fold their own pool word.
 *
 * unk20 is also reachable as the folded address constant 0x0200CC58, which
 * asm/ prints as a separate symbol gUnknown_0200CC58. That is not a second
 * object: it is what LICM does to `gUnknown_0200CC38.unk20[i]` when `i` is
 * loop-invariant, so sub_0801B018's two spellings of the same member -- one
 * hoisted out of its retry loop, one after it -- are one member.
 *
 * Widths are the access widths and nothing here fixes any signedness: unk00
 * and unk10 take `|= 0xff`, unk20 is a bit field set and cleared a bit at a
 * time (bit 0 and bit 2), unk30 and unk40 are plain byte stores.
 *
 * The struct STOPS at 0x50, and that boundary is measured rather than assumed.
 * The two word arrays at 0x0200CC88 are reset by the same loop and would look
 * like two more members, but sub_0801B4C0 initialises their induction variable
 * from the FOLDED address constant 0x0200CC88 while it builds unk30's from the
 * common base register (`movs r1, #0x30; adds r1, r1, r6`). A member of this
 * struct folds against the base; a separate object folds against itself. */
struct Unk0200CC38
{
    /* 0x00 */ u8 unk00[0x10];
    /* 0x10 */ u8 unk10[0x10];
    /* 0x20 */ u8 unk20[0x10];
    /* 0x30 */ u8 unk30[0x10];
    /* 0x40 */ u8 unk40[0x10];
};

extern struct Unk0200CC38 gUnknown_0200CC38;

/* Wave 40 (W40-F). 0x0200CC88 -- two more per-slot arrays alongside
 * gUnknown_0200CC38 above, whole words this time and 0x10 slots each, cleared
 * together by sub_0801B4C0's reset loop. ONE object rather than two: the ROM
 * reaches both through a single induction variable stepping by 4, with the
 * upper array as a `str [rN, #0x40]` displacement off it, which is what
 * strength reduction does to two subscripts of one object and not to two
 * objects.
 *
 * A struct, not `u32 [0x20]`. sub_0801B2FC fills sectorGeneration[i] with the
 * generation word (+8) of slot i's own sector header, and slotGeneration with
 * the newest sector's copy of the whole table (its +0x10 block);
 * sub_0801A7D8 bumps slotGeneration[slot] per write attempt and writes the
 * table back into every sector. The struct spelling is measured, not
 * cosmetic: sub_0801A7D8's copy loop reads slotGeneration through the
 * object's base register + 0x40, hoisted out of its segment loop and spilled
 * at sp+0xb4. `(&gUnknown_0200CC88[16])[i]` folds 0x0200CCC8 into one pool
 * constant and `gUnknown_0200CC88[i + 16]` never hoists the base; only the
 * member access reproduces the ROM's frame. */
struct SaveSlotGenerations
{
    /* 0x00 */ u32 sectorGeneration[0x10];
    /* 0x40 */ u32 slotGeneration[0x10];
};

extern struct SaveSlotGenerations gUnknown_0200CC88;

/* Wave 50 (W50-K). 0x0200CD08 -- a single WORD, written by sub_0801B2FC and by
 * nothing else that has been read: cleared to 0 before the slot scan, then set
 * to `gUnknown_02002000's +8 word + 1` each time a newer save slot wins. It is
 * only ever stored, never loaded here, so nothing fixes the signedness beyond
 * the store width; u32 agrees with gUnknown_0200CC88, which holds the same +8
 * word and IS compared unsigned (`blo`/`bhs`) in the same function.
 *
 * It is reached through the -fforce-addr pool word at 0x0808EF54, which is part
 * of the 0x0808EF54-0x0808EF60 run; the address itself is bound by aw2bhr.lds,
 * which is why naming it directly links. */
extern u32 gUnknown_0200CD08;

/* 0x03005C74 / 0x03005C80 / 0x03005C84 -- three FUNCTION POINTER VARIABLES.
 * All three sit at even addresses and all three are read as
 * `ldr rN, =sym; ldr rN, [rN]` before the indirect call, i.e. a genuine
 * dereference -- unlike the odd-address pool words below, which are the
 * pointers' values.
 *
 * Their ARITIES are read straight off the veneer register, which is the one
 * wrapper-side tell that has ever held in this tree: gcc parks the callee
 * pointer in the first FREE scratch register, so the index counts the
 * arguments.
 *   sub_0801B5C0  ldr r0,[0x03005C80]; bl _call_via_r0  -> 0 arguments
 *   sub_0801B5E8  ldr r1,[0x03005C84]; bl _call_via_r1  -> 1 argument
 *   sub_0801B618  ldr r2,[0x03005C74]; bl _call_via_r2  -> 2 arguments
 * Each result is re-narrowed with `lsls #0x10; lsrs #0x10` after the call,
 * which is agbcc re-narrowing a u16-returning callee, so the return is u16.
 * The leading argument of the one- and two-argument forms is the wrapper's own
 * u16 parameter passed through. The second argument of the two-argument form is
 * invisible -- it rides r1 untouched -- and `int` is the weakest type that
 * costs no instruction there. */
/* Wave 32 (W32-B): the flash chip descriptor that sits beside the
 * gUnknown_03005C7x routine pointers. sub_0808B074, sub_0808B0E8 and
 * sub_0808B3C0 read two members -- unk10, the WAITCNT low two bits this chip
 * wants (`REG_WAITCNT = (REG_WAITCNT & 0xfffc) | unk10`), and unk08, the sector
 * shift (`0x0E000000 + (sector << unk08)`). Both are bare loads, `ldrh` and
 * `ldrb`, so the widths come from the loads and the signedness is unproved.
 * Nothing bounds the record; the fillers are "what fits below the next one".
 *
 * Wave 46 (W46-D): unk14 added -- an EXTENSION past the old end, no existing
 * member moved. sub_0808ADA4 reads `ldrh r1, [r0, #0x14]` and compares it
 * against 0x1CC2 to decide whether a timed-out write needs the 0xF0 reset
 * command, so +0x14 is a u16 chip ID. It is the SAME halfword as
 * gUnknown_0848548C's unk28, reached the other way round and that is what
 * makes it more than a guess: sub_0808AB8C sets gUnknown_03005C78 to
 * descriptor + 0x14, so this record's +0x14 is descriptor +0x28 -- and +0x28
 * is independently the id sub_0808AB8C matches ReadFlashId's result against.
 * A producer and a consumer of one field agreeing without being derived from
 * each other. */
/* Wave 47 (W47-D): unk04 added -- an EXTENSION inside the old filler_00, no
 * existing member moved or rewidened. sub_0808B2E0 reads `ldr r1, [r0, #4]`
 * and counts DOWN from it while walking a byte pointer, and sub_0808B31C reads
 * the same word and stores it to gUnknown_03005C7C with a truncating `strh`
 * before using it as that same walk's counter. A WORD at both sites, used only
 * as an unsigned count (`cmp #0` / `subs #1`), hence u32: it is the sector's
 * SIZE IN BYTES.
 *
 * It is the same halfword-pair as gUnknown_084856A4 +0x18, and that
 * correspondence is what makes the whole record legible -- see the
 * gUnknown_084856A4 note below. */
struct Unk03005C78
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ u32 unk04;
    /* 0x08 */ u8 unk08;
    /* 0x09 */ u8 filler_09[0x07];
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 filler_12[0x02];
    /* 0x14 */ u16 unk14;
};

extern struct Unk03005C78 *gUnknown_03005C78;
/* Wave 32 (W32-B): sub_0808B3C0 reads the halfword at +0x24 and uses it exactly
 * the way its twin sub_0808B074 uses gUnknown_03005C78->unk10 -- as the WAITCNT
 * low two bits. Spelled as a u16 subscript because a subscript is all the ROM
 * shows; whether +0x24 is one member of a larger flash record is not settled.
 *
 * Wave 47 (W47-D) SETTLES IT, and the declaration is deliberately left alone
 * anyway. 0x084856A4 is one `struct Unk0848548C` -- a flash chip descriptor of
 * the same shape as the entries of the gUnknown_0848548C table -- and the three
 * offsets the erase path reads off it are exactly the three members
 * `struct Unk03005C78` already names, shifted by the +0x14 that sub_0808AB8C
 * adds when it installs gUnknown_03005C78:
 *
 *     gUnknown_084856A4 +0x18  ldr   ==  Unk03005C78 +0x04  unk04  sector bytes
 *     gUnknown_084856A4 +0x1c  ldrb  ==  Unk03005C78 +0x08  unk08  sector shift
 *     gUnknown_084856A4 +0x24  ldrh  ==  Unk03005C78 +0x10  unk10  WAITCNT bits
 *     gUnknown_084856A4 +0x28  ldrh  ==  Unk03005C78 +0x14  unk14  chip id
 *
 * Four independent offsets agreeing in width AND in use across two symbols
 * nobody derived from the other. It is NOT respelled as a struct here because
 * `struct Unk03005C78` is declared 0x16 bytes wide against a 0x14-byte
 * reservation (see gUnknown_0848548C's note), so embedding it would move +0x28;
 * the subscript and the two casts in c_0808B430.c / c_0808B4B4.c are the honest
 * statement of what the ROM shows. Retyping this is a separate job that has to
 * bound Unk03005C78 first. */
extern const u16 gUnknown_084856A4[];
/* Wave 47 (W47-H). 0x08485678 -- read at ONE site only, sub_0808B5B8, which
 * does `ldr r0,=gUnknown_08485678; ldr r0,[r0,#0x18]` and stores the word
 * truncated into gUnknown_03005C7C as the byte count of a whole save slot,
 * then walks it down by gUnknown_084856A4 +0x18 (one sector) per iteration.
 * So +0x18 is a 32-bit member and it is NOT the same member gUnknown_084856A4
 * +0x18 is, despite the identical offset -- one is the slot size, the other
 * the sector size, and the loop would run exactly once if they agreed.
 * 0x084856A4 - 0x08485678 = 0x2C, so this is a DIFFERENT record, not the same
 * one at another offset; nothing else in the tree reads it and no other
 * member is observed, so the width is spelled the way the one access reads it
 * and the shape is left open. */
extern const u16 gUnknown_08485678[];
/* Wave 47 (W47-D). 0x03005C7C -- the flash sector-program loop's REMAINING
 * BYTE COUNT, and a genuine global rather than a local: sub_0808B31C seeds it
 * from gUnknown_03005C78->unk04 with a truncating `strh`, then re-loads it
 * (`ldrh [r6]`) at the top of every iteration and stores it back after every
 * `subs #1`. It needs no `volatile` for that -- the reload is forced by the
 * sub_0808B184 call in the loop body, which agbcc must assume can touch it.
 * u16 by every access, and unsigned by the `cmp #0` exit test. */
extern u16 gUnknown_03005C7C;
extern u16 (*gUnknown_03005C74)(u16, int);
extern u16 (*gUnknown_03005C80)(void);
extern u16 (*gUnknown_03005C84)(u16);

/* Wave 46 (W46-D). gUnknown_081D9478 and gUnknown_081D947C ARE NOT VARIABLES
 * AND MUST NOT BE DECLARED AS ANY. They are the two words of ONE agbcc
 * -fforce-addr address-constant block belonging to sub_0808A3DC -- adjacent,
 * addends 0 and 4, exactly the shape the .rodata chapter of
 * docs/agbcc-codegen.md describes. Dereferenced in baserom.gba they hold
 * 0x0849957C and 0x03001FE8, i.e. the addresses of gUnknown_0849957C (declared
 * above) and gUnknown_03001FE8 (`union BgCntBuf` in hardware.h) -- two globals
 * that already exist under real names.
 *
 * So the `ldr rN, =gUnknown_081D9478; ldr rN, [rN]` that asm/ shows is agbcc's
 * ordinary two-step materialisation of ONE global's address, not a pointer
 * variable being read. Declaring them costs an extra indirection at every use:
 * measured, `u16 **gUnknown_081D9478` compiles to four chained `ldr`s where the
 * ROM has three. Name the real globals and agbcc rebuilds this block itself.
 *
 * WAVE 60 (W60-I): THE LAST SENTENCE ABOVE IS REFUTED BY MEASUREMENT. Naming
 * the real globals does NOT rebuild the block. sub_0808A3DC's draft names them
 * and agbcc emits pool words relocating DIRECTLY -- `R_ARM_ABS32
 * gUnknown_0849957C` and `R_ARM_ABS32 gUnknown_03001FE8` -- giving TWO chained
 * loads where the ROM has three, and rematerialising each address at its later
 * uses instead of holding them, so the candidate pushes {r4,r5,lr} against the
 * ROM's {r4,r5,r6,r7,lr}. That accounts for nearly all of the function's 71
 * differing bytes. Verified identical under all four of -O1/-O2 x
 * `-fforce-addr` kept/removed, so the flag is not the trigger even though it is
 * live in this tree (removing it is decisive for sub_0808AF00, 0xB20 bytes
 * away).
 *
 * Everything else in the wave-46 note stands, and the two halves together are
 * the useful statement: naming the real globals is ONE indirection short of the
 * ROM, declaring the block as `u16 **` is one too many, and no spelling
 * measured so far lands on three. Whatever makes agbcc force these two symbol
 * addresses into the constant pool has not been identified. Do NOT declare
 * these two symbols as variables to close the gap -- that is the four-load
 * spelling and it is further away, not nearer.
 *
 * This is the wave-27 rule ("before declaring a gUnknown_08xxxxxx that is only
 * ever reached by a double ldr, read the ROM word at that address") paying off
 * twice in one function, and it is worth doing FIRST rather than after a failed
 * attempt: both symbols look exactly like ROM pointer tables in the index. */
/* Two LZ77 blobs handed to Decompress, hence `u8 []` per that prototype. These
 * two ARE real objects -- both are reached only by address (`ldr r0, =sym`,
 * never a load through them) and both relocate directly rather than through the
 * .rodata block above. */
extern u8 gUnknown_0823E684[];
extern u8 gUnknown_0823E7A0[];

/* Wave 46 (W46-D). 0x0848548C -- the flash CHIP DESCRIPTOR TABLE: an array of
 * pointers to records, scanned by sub_0808AB8C, which then installs five of
 * each record's members into the gUnknown_03005C7x / gUnknown_03000F68
 * routine-pointer globals above and points gUnknown_03005C78 at the record's
 * +0x14. An array of POINTERS and not of records: the scan steps its cursor by
 * 4 and re-loads `[r2]` at every use.
 *
 * Every member's TYPE here is copied from the global it is stored into, which
 * is why nothing new is being typed -- unk00, unk04, unk08, unk0c and unk10
 * are just gUnknown_03005C74, gUnknown_03005C80, gUnknown_03005C84,
 * gUnknown_03005C70 and gUnknown_03000F68 before installation, in that order.
 *
 * unk28 is a UNION and the ROM proves it: the loop's terminator test reads a
 * BYTE at +0x28 (`adds r0, #0x28; ldrb`) while the chip-id compare one
 * instruction later reads a HALFWORD at the same +0x28 (`ldrh r0, [r1, #0x28]`).
 * Two access widths at one offset in one basic block is the discriminating use
 * that a single scalar member cannot explain.
 *
 * unk14 is spelled as a RAW 0x14 BYTES rather than as an embedded
 * `struct Unk03005C78`, deliberately. sub_0808AB8C only ever takes its address
 * (`adds r0, #0x14`) and hands it to gUnknown_03005C78, so nothing here reads
 * the record's interior, and embedding the struct would tie this table's +0x28
 * offset to a shared declaration this function has no evidence about: measured,
 * embedding it puts unk28 at +0x2C and not the +0x28 the ROM loads. Whether
 * Unk03005C78's declared 0x12 or this reservation of 0x14 is the true width is
 * left where it was; the cast at the one use is the honest statement of what is
 * known. */
struct Unk0848548C
{
    /* 0x00 */ u16 (*unk00)(u16, int);
    /* 0x04 */ u16 (*unk04)(void);
    /* 0x08 */ u16 (*unk08)(u16);
    /* 0x0c */ u16 (*unk0c)(int, u8 *, u8);
    /* 0x10 */ u16 *unk10;
    /* 0x14 */ u8 unk14[0x14];
    /* 0x28 */ union
    {
        u16 joined;
        struct
        {
            u8 makerId;
            u8 deviceId;
        } separate;
    } unk28;
};

extern struct Unk0848548C *gUnknown_0848548C[];

/* 0x03005C88 .. 0x0300677C -- an IWRAM CODE OVERLAY, not a data buffer.
 * sub_0801B6BC copies 0x086173F0 over it with
 * `CpuSet(src, 0x03005C88, ((0x0300677C - 0x03005C88) / 4) | 0x04000000)`, and
 * three wrappers in the same block then CALL into the copied region through
 * pool words holding ODD addresses -- 0x03005C89, 0x03005E8D, 0x03006029 --
 * with no load in between. The low bit set is a THUMB entry point, and
 * aw2bhr.map allocates no object at any of the three (`*fill*`), so those words
 * are the pointers' VALUES that gen_lds.py invented symbols for, exactly as
 * already documented for sub_0801B6EC's 0x0300619D. They are spelled `&symbol`
 * at the call sites because the honest literal emits a bare `.word` where the
 * ROM has `.word 0` plus a relocation.
 *
 * u8 arrays and not u32: the length the ROM computes carries the
 * `cmp #0; bge; adds #3` rounding correction of a signed divide by four, which
 * a `u32 *` subtraction would not need because the compiler would know the
 * difference is already a word count.
 *
 * Arities of the three entry points, by the same _call_via_rN readout as above:
 *   0x03005C89  _call_via_r4 (sub_0801B738)  -> 4 arguments, first u8
 *   0x03005E8D  _call_via_r3 (sub_0801B724)  -> 3 arguments, third u16
 *   0x03006029  _call_via_r3 (sub_0801B70C)  -> 3 arguments, second and third u16
 * Nothing types the pass-through arguments; `int` is what costs no
 * instruction. */
extern u8 gUnknown_03005C88[];
extern u8 gUnknown_0300677C[];
extern u8 gUnknown_03005C89;
extern u8 gUnknown_03005E8D;
extern u8 gUnknown_03006029;
extern const u8 gUnknown_086173F0[];

/* 0x03000058 / 0x0300005C -- a pair of word counters advanced in step.
 * sub_0801B964 adds its parameter to BOTH and then wraps only 0x03000058:
 * `cmp #7; bgt` on the running total, and `-= 8` on the taken side, returning 1
 * when it wrapped and 0 when it did not. SIGNED and 32 bits: every access is a
 * bare `ldr`/`str` and the compare is `bgt`, not `bhi`. What they count is not
 * settled here -- a 3-bit sub-position and its unwrapped twin is consistent
 * with the shape but is not evidence. */
extern int gUnknown_03000058;
extern int gUnknown_0300005C;

/* 0x03002F2C / 0x0300141C / 0x030030D4 -- latches recording the source of the
 * most recent OAM flush. sub_0801BBC4 and sub_0801BCA8 store
 * gOamTransferTail.src into 0x03002F2C and gUnknown_03002520 (the OAM
 * shadow) into 0x030030D4; sub_0801BC08 stores gOamTransferHead.src into
 * 0x0300141C. A plain word `str` of a pointer in every case. Nothing in this
 * block READS any of the three, so `void *` records the store width plus the
 * one thing the stored value is known to be, and no more. */
extern void *gUnknown_0300141C;
extern void *gUnknown_03002F2C;
extern void *gUnknown_030030D4;

/* 0x030024C0 -- zeroed with `strh` by sub_0801BBC4 and sub_0801BCA8 in the same
 * breath as the two latches above, i.e. a halfword cursor reset once the
 * pending flush has been consumed. u16 is the store width; no reader appears in
 * this block, so the sign is NOT settled. */
extern u16 gUnknown_030024C0;

/* 0x080A5524 -- a 0x500-byte ROM blob. sub_0801B780 hands it to sub_08011E54
 * with a destination built as
 * `BG char base * 0x4000 + ((id + 1) & 0x3FF) * 32 + 0x06000000`, which is a
 * VRAM tile address, so this is tile graphics. Referenced by address only
 * (`ldr r0, =sym`, never a load through it). Not spelled const: sub_08011E54's
 * first parameter is a plain `void *`. */
extern u8 gUnknown_080A5524[];

/* ---- the 0x0806E000 block (wave 28) ---------------------------------- */

/* 0x08582754 -- an array of PALETTE POINTERS, not palette data. sub_0806EB28
 * indexes it with `lsls #2` and then LOADS through the result
 * (`adds r1, r1, r0; ldr r0, [r1]`) before handing it to ApplyPaletteExt, so
 * the stride is a word and the element is the pointer. At least four entries:
 * the index is `gGameClock & 3`, bumped by one when it collides with the
 * proc's current selection, so 0..4 are reachable. */
extern u16 *gUnknown_08582754[];

/* 0x081A47E4 -- a halfword lookup table read at `(gGameClock & 0x1F) / 2`
 * by sub_0806E7FC, whose result goes straight into gPal at halfword 0x1EC. The
 * `lsrs #1; lsls #1` pair is the /2 fused with the u16 array's own scale and is
 * NOT a mask -- counted as `(u32)x >> 1 << 1` it is the byte offset of element
 * x/2. So the table is at most 16 entries as reached from here. */
extern const u16 gUnknown_081A47E4[];

/* 0x08582BFC -- the proc script sub_0806E7C0 starts blocking. Its proc carries
 * unk2c/unk30/unk34/unk38 as words, which is the same field set sub_0806E740
 * steps, so 0x08582BFC's proc and sub_0806E740's are the same object. */
extern const struct ProcCmd gUnknown_08582BFC[];

/* ---- the 0x0801E000 block (wave 28) ---------------------------------- */

/* 0x0300054E -- a SIGNED halfword index, and the `ldrsh` off a ZERO index
 * register (`movs r0, #0; ldrsh r1, [r4, r0]`) is what pins it: an unsigned
 * halfword would have been `ldrh [r4, #0]` with no index register at all.
 * sub_0801E968 reads it TWICE in one body -- once to select an entry of
 * gUnknown_03001470 and once as the first argument of the callback it then
 * dispatches -- and the second read is a fresh load, so it is not const. */
extern s16 gUnknown_0300054E;

/* 0x03000548 -- Wave 54, W54-G. RETYPED from the flagged `u32` above it, which
 * the previous note explicitly recorded as an alignment guess rather than a
 * claim ("only ever referenced BY ADDRESS ... Flagged rather than claimed").
 * sub_0801E9B0 is the first function found that loads and stores THROUGH it,
 * and it is decisive: the whole OAM triple is written `strh` at +0, +2 and +4
 * and read back the same way, +2 is additionally reached with a `ldrb` for its
 * low byte and masked with 0xC000 for the OBJ SIZE field, and +0 is masked with
 * 0xC000 for the OBJ SHAPE field -- the two indices of the width/height tables
 * gUnknown_0848B6F6/gUnknown_0848B6F8. So it is a scratch OAM entry, three
 * halfwords, not one word.
 *   Retyping is safe: the only other reference in the tree is
 * src/decomp/c_0801E930.c passing `&gUnknown_03000548` to gUnknown_03000550,
 * whose declared type is `void (*)(s16, void *)` -- a `void *` parameter, so
 * the address converts implicitly and the caller's bytes are unchanged
 * (re-verified by exit code). sub_080169A4's second parameter is `void *` too.
 * Left as a bare 3 x u16 rather than reusing struct OamData because nothing
 * here reads it as OAM fields. */
struct Unk03000548
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
};
extern struct Unk03000548 gUnknown_03000548;

/* 0x03000550 -- the most recently dispatched handler. sub_0801E968 loads the
 * pointer out of gUnknown_0200E438[...].unk44, stores it here, and then calls
 * it through the SAME register without re-reading the global, which is the
 * ordinary non-volatile store-forwarding and rules volatile out.
 *
 * The signature is read off the dispatch: `bl _call_via_r2` with r0 and r1 set
 * up means TWO arguments (gcc parks the pointer in the first free scratch
 * register). r0 is gUnknown_0300054E sign-extended, r1 is
 * `&gUnknown_03000548`. The stored value comes from a `u32` member, so the
 * assignment casts. */
extern void (*gUnknown_03000550)(s16, void *);

/* Wave 29, W29-A. A run of 16-colour palettes in ROM: sub_0802D5CC indexes it
 * with `lsls r0,#5` (32 BYTES per row) and hands the address straight to
 * ApplyPaletteExt's `u16 *` first parameter, so `u16 []` with 16 entries per
 * row. NOT const, for the same reason as every other ApplyPaletteExt source in
 * this header -- that prototype takes a plain `u16 *`. The row count is not
 * bounded by anything readable; the only caller passes a variable. */
extern u16 gUnknown_080D4188[];
/* Wave 29, W29-A. A 2-D halfword table with a 0x32-BYTE row, i.e. 0x19
 * halfwords per row -- sub_080261A4 scales the row index by `muls #0x32` and
 * the column by `lsls #1` and adds both to the symbol directly, so this is an
 * array and not a pointer. Its one reader shifts the loaded halfword left by 2
 * and sub_08002844 masks the result with 0x3ff, so the entries are tile
 * indices. Non-const is unproved either way: nothing writes it and nothing
 * takes its address. */
/* 5 rows of 25 halfwords, subscripted [set][unitType]. The 'Uncompressed
 * Unit Sprite Editor' Nightmare module (community) names the 25 columns and
 * they are unit types in the SAME numbering struct UnitType uses, column 0
 * being the module's 'Blank' -- independent corroboration that this tree is
 * right to start gUnknown_085D5ABC one record early with a dummy at 0. */
extern u16 gUnknown_08499608[][0x19];
/* Wave 29, W29-A. The proc script sub_0802A54C starts, parented to its own
 * second argument; the new proc's +0x4c is filled with the struct Unit *
 * the caller passed, and sub_0802A588 reads it back out of the same slot. */
extern const struct ProcCmd gUnknown_0849A198[];
/* Wave 29, W29-A. Two gUnknown_03001470 list scripts of the gUnknown_0849A108
 * shape: sub_08005560 and sub_08005598 are one body twice, each
 * `sub_080152EC(blob, 0)` after clearing gActiveMap->state, and these are
 * the only two words they differ in. `const u8 []` because sub_080152EC's first
 * parameter is `const void *` and nothing dereferences either symbol. */
extern const u8 gUnknown_08488444[];
extern const u8 gUnknown_0848846C[];

/* ---- wave 29 (C): the 0x0808A block ---- */
/* A word-wide mode selector: sub_0808A340 zeroes it with `str` and
 * sub_0808A5C4 branches on it being 0 or 1 to pick between two nearly identical
 * sub_08014A5C calls. `int` -- the compares are `cmp r1, #0` / `cmp r1, #1`
 * with no narrowing anywhere. */
extern int gUnknown_03005908;
/* Handed to sub_080193B0(const u8 *) by sub_0808A844, exactly the way
 * gUnknown_0849A520 and its neighbours are; `const u8 []` on that precedent. */
extern const u8 gUnknown_084A0D58[];
/* A proc script sub_0808A884 starts TWICE under its own proc as parent -- once
 * when sub_08019260 reports false and once on a key press -- from the same
 * pool word. */
extern const struct ProcCmd gUnknown_08617094[];

/* 0x08616EB4 -- the proc script sub_08088004 Proc_Starts under its own argument.
 * Read straight out of the ROM rather than inferred: the first record is
 * `02 00 00 00 / 45 80 08 08`, i.e. opcode 2 with the THUMB entry 0x08088044,
 * followed by `0E 00 40 00` and then opcode 3 records pointing at 0x080880BC
 * and 0x0808844C -- the standard ProcCmd layout, and the callees are all in the
 * 0x08088 block this script drives. */
extern const struct ProcCmd gUnknown_08616EB4[];

/* ---- wave 29 (C): the 0x0806C block ---- */
/* A NULL-terminated table of POINTERS, and the two readings agree: sub_0806C114
 * indexes it with `lsls #2` off the bare symbol -- no load in front, so the
 * symbol IS the array, unlike gUnknown_08499590 -- and then tests the loaded
 * word against 0 to find the end; sub_0806C0E4 dereferences the same word at
 * +0x30 and compares that against the proc's own +0x30 with `blo`, an UNSIGNED
 * ordering, so both are u32. Nothing else about the pointee is readable. */
/* Wave 48 (W48-C): filler_00 CARVED into six 8-byte records. sub_0806BF40 walks
 * `gUnknown_0858265C[proc->unk38]` with a stride of 8 and a bound of `i <= 5`
 * INCLUSIVE, reading a `kind` word at +0 and a string pointer at +4. Six times
 * eight is 0x30 -- exactly where unk30 already sits -- so the extent is fixed by
 * the existing member rather than guessed, the same way struct Unk0200C420's
 * unk38[0x2a] is fixed by the struct size. unk30 is untouched at its old offset
 * and nothing in src/ named this struct before, so the carve cannot regress a
 * matched function.
 *   unk04 is `u8 *` from its use: it is sub_0806BD1C's second parameter, a
 * NUL-terminated byte string it renders a glyph at a time. unk00 is `int` from
 * the `ldr` and is a small tag -- sub_0806BF40 tests it against 1, 3, 4 and 5
 * with four independent `if`s (not a switch: each test re-loads and each arm
 * falls through to the next). */
struct Unk0858265CRec /* 0x08 */
{
    /* 0x00 */ int unk00;
    /* 0x04 */ u8 *unk04;
};
struct Unk0858265C
{
    /* 0x00 */ struct Unk0858265CRec unk00[6];
    /* 0x30 */ u32 unk30;
};
extern struct Unk0858265C *gUnknown_0858265C[];
/* The proc script sub_0806C114 starts blocking under its own proc before
 * jumping that proc back to label 0 -- a per-entry sub-sequence driven by the
 * gUnknown_0858265C cursor. */
extern const struct ProcCmd gUnknown_08581C48[];
/* 0x081A3D84 -- the twin of gUnknown_081A47E4 below: a halfword table read at
 * `(gGameClock & 0x1F) / 2` by sub_0806C1E4, whose result goes into gPal
 * at halfword 0x12c. The `lsrs #1` then `lsls #1` is the /2 fused with the u16
 * array's own scale, not a mask, so at most 16 entries as reached from here. */
extern const u16 gUnknown_081A3D84[];

/* ---- wave 29 (C): the 0x08064 block ---- */
/* An 8-byte ROM image that sub_08064774 copies onto its own stack with
 * sub_0808B6E8 and then indexes as halfwords with obj->unk48, so four u16
 * sprite ids. It sits in the same 0x0816Exxx run as gUnknown_0816E1B8, which is
 * that unit's .rodata, so this is very likely a local array initialiser image;
 * it is spelled as an explicit copy because the ROM's call is a `bl` to the
 * game's own sub_0808B6E8 rather than to a compiler helper. */
extern const u16 gUnknown_0816E0C0[];

/* ---- wave 29 (C): the 0x0804B block ---- */
/* A 0x18-byte ROM record. The stride is read off `lsls #1; adds; lsls #3` in
 * both readers, which is a multiply by 3 << 3 = 24, i.e. an array index and not
 * hand-rolled arithmetic. sub_0804BB28 hands +0x04 to LZ77UnCompVram;
 * sub_0804BB44 indexes a word array at +0x0c with gUnknown_03004520 and hands
 * the entry to CpuFastSet. Three words fit between 0x0c and the 0x18 stride.
 * Nothing yet reads 0x00 or 0x08. Non-const: both consumers take
 * `const void *`, so const buys nothing and would only force casts on the
 * three unmatched readers (sub_0804B8BC, sub_0804BB74, sub_080566C8). */
/* Wave 55, W55-H: 0x02 and 0x08 filled in, both ADDITIVE into existing filler
 * -- no member moves and no member changes width, so every promoted reader
 * compiles to the same bytes. unk02 is a small tag read with a bare `ldrb` by
 * two independent functions: sub_0804B8BC feeds it to sub_0804BA64 as that
 * function's `b`, and sub_0804BB74 range-tests it `(u8)(v - 1) <= 1`. Nothing
 * sign-extends it. unk08 is a second LZ77 blob pointer alongside unk04 --
 * sub_0804BB74 hands it to LZ77UnCompWram exactly as sub_0804BB28 hands unk04
 * to LZ77UnCompVram, through the same `adds rN, #8` on the -fforce-addr base. */
struct Unk08555850 /* 0x18 */
{
    /* 0x00 */ u16 unk00; /* Wave 60, W60-H. sub_080566C8 reads it with a bare
                           * `ldrh` at displacement 0 off the `* 0x18` element
                           * base, indexed by gUnknown_03004580[i][3], and
                           * stores it into the same local that
                           * gUnknown_085D6A48[..][1] otherwise fills -- so it
                           * is the same small id that column holds. Halfword
                           * from the load; nothing signs it. Was filler_00. */
    /* 0x02 */ u8 unk02;
    /* 0x03 */ u8 filler_03[0x01];
    /* 0x04 */ void *unk04;
    /* 0x08 */ void *unk08;
    /* 0x0c */ void *unk0c[3];
};
extern struct Unk08555850 gUnknown_08555850[];
/* Wave 55, W55-H. sub_0804B8BC's two sprite-pointer sources for
 * gUnknown_02029690[side].unk04, which is `u16 *`. gUnknown_085557C0 is a table
 * of pointers -- read `[c]` and `[c + 1]` as a word pair, and 0x085557C0 holds
 * 0x085555E0 in baserom.gba, so the entries point back into ROM. 08555720 is
 * used only by ADDRESS (`ldr r0,=gUnknown_08555720; str r0,[r1,#4]`) as the
 * "empty" value stored in the same slot, so it is an object, not a pool word --
 * the word AT 0x08555720 is 0. Extent unpinned in both cases. */
extern u16 gUnknown_08555720[];
extern u16 *gUnknown_085557C0[];
/* Wave 55, W55-H. Rows of TWO words: sub_0804C0FC scales its index `lsls #3`
 * and reads +0 and +4 off it (the +4 through an `adds rN, #4` on the base, the
 * -fforce-addr member-offset idiom c_0804BB28.c documents), handing both to
 * sub_0804BD20's two `void *` parameters. baserom.gba has 0x05000200 at
 * 0x08551CFC, a palette-RAM address, so the entries are destinations, not
 * counts. Outer extent unpinned -- only the two sides are ever indexed. */
extern void *gUnknown_08551CFC[][2];
/* Rows of FIVE words: sub_0804BD20 scales its second index by
 * `lsls #2; adds; lsls #2` = 5 * 4 and its first by 4, then loads a word and
 * uses it as a CpuFastSet source. */
extern void *gUnknown_08555D30[][5];

/* ---- wave 29 (C): the 0x0803F block ---- */
/* A sprite/animation descriptor blob: sub_0803F400 hands its ADDRESS to
 * sub_0801C210, exactly the way gUnknown_030032CC's neighbours above do.
 * `u8 []` on that precedent; extent unknown, and NOT const because
 * sub_0801C210's first parameter is still a plain `void *`. */
extern u8 gUnknown_081171EC[];
/* Proc scripts. gUnknown_0849F940 is handed to Proc_StartBlocking by
 * sub_0803F510 (which then writes +0x2c/+0x30 of the result);
 * gUnknown_0849FADC to Proc_Start(.., PROC_TREE_3) by sub_0803FEDC and
 * sub_0803FF04, neither of which touches the returned proc. */
extern const struct ProcCmd gUnknown_0849F940[];
extern const struct ProcCmd gUnknown_0849FADC[];
/* Wave 32 (W32-B): a four-entry byte table sub_0803FE50 indexes with
 * `(x & 0x18) >> 3` -- a SIGNED shift, so the index expression is `int` -- and
 * multiplies the result by 8 to make a tile index. Bare `ldrb`, hence u8. */
extern const u8 gUnknown_0849FAD8[];
/* Wave 32 (W32-B): the script sub_0803FF48 both sweeps with Proc_ForEach and
 * then starts a fresh instance of. */
extern const struct ProcCmd gUnknown_0849FB04[];
/* A ROM graphics blob read in 0x20-word chunks: sub_0803F8E0 CpuFastSets
 * `gUnknown_080D20C4 + ((n * 4) & 0x3FF) * 32` to 0x06010900, so the unit is a
 * 32-byte 4bpp tile and the transfer is four of them. `const u8 []` --
 * CpuFastSet's first parameter is `const void *`, so const costs nothing.
 * Extent not pinned. */
extern const u8 gUnknown_080D20C4[];
/* Wave 49, W49-C. Two ROM PALETTE BANKS 0x100 bytes apart, i.e. eight 16-colour
 * palettes each, reached only as whole palettes: sub_0803F80C indexes 0803EE4
 * with `lsls #5` off gPlayers[n].unk1a and also names element 6
 * directly (`adds rB, #0xc0`), and sub_0803F880 picks between the two banks on
 * its first argument and then takes `&bank[unk1a][6]` (`adds rB, #0xc`) as a
 * two-colour source. The `[16]` inner bound is what makes both the 0x20-byte
 * stride and the two member offsets fall out of ordinary subscripts.
 *   NOT const, and this is a type-compatibility choice rather than a codegen
 * one: every consumer is ApplyPaletteExt / sub_0801368C, both of which declare a
 * plain `u16 *` first parameter. Only the ADDRESS is ever computed here -- no
 * element is loaded -- so const would be byte-neutral either way and buys
 * nothing but casts. */
extern u16 gUnknown_080D3DE4[][16];
extern u16 gUnknown_080D3EE4[][16];

/* ---- wave 30 (W30-C): the 0x0806A block ---- */

/* A signed 32-bit horizontal scroll offset for the 0x0806A054 screen, and the
 * word width is proved three ways: sub_0806A158/sub_0806A180 read it with a
 * plain `ldr` before truncating to REG_BG0HOFS, sub_0806A1A8 stores -0xF0 with
 * `str`, and sub_0806A1D0 stores Interpolate's `s32` return into it directly.
 * Signed because both the negate (`rsbs r0, r0, #0`) and the stored -0xF0 want
 * it. Bound by aw2bhr.lds at 0x0202F20C. */
extern s32 gUnknown_0202F20C;
/* A signed 16-bit cursor read `movs r2,#0; ldrsh r1,[r1,r2]` by sub_0806A534 --
 * a register-offset `ldrsh` at displacement zero, which is a plain s16 scalar
 * and not an array index (Thumb has no immediate-offset ldrsh). Bound by
 * aw2bhr.lds at 0x0202F210. */
extern s16 gUnknown_0202F210;
/* The same shape one region up: sub_0806A534 reads it with the identical
 * zero-displacement `ldrsh` and adds 0x206 before masking with 0x1FF, so it is
 * a signed halfword. Bound by aw2bhr.lds at 0x0300060C. */
extern s16 gUnknown_0300060C;
/* A ROM blob handed straight to sub_0801BD00's `void *` third parameter by
 * sub_0806A534, with NO load in front of the pool word -- the symbol IS the
 * data, unlike the pointer globals in this region. Not const, because that
 * parameter is a plain `void *`. Extent not pinned. */
extern u8 gUnknown_085815C8[];
/* A 16-byte ROM row indexed by sub_0806AADC's `proc->unk4c` (`lsls #4` off the
 * bare symbol -- no indirection, so the symbol is the array). +0x00 goes to
 * Decompress's `u8 *` source and +0x04 to ApplyPaletteExt's `u16 *` source,
 * which is what fixes both element types; neither may be const, because both
 * of those parameters are plain pointers. +0x08..+0x0F is unread here. */
/* Wave 32 (W32-C) fills in +0x08 and +0x0C from sub_0806C380 / sub_0806C410,
 * which draw the row: +0x08 goes to PutSpriteExt's `u16 *` OAM argument and
 * +0x0C is read `movs rN,#0xc; ldrsh` -- a REGISTER-offset ldrsh, which in
 * THUMB is the only encoding there is, so the sign is the type's and not a
 * cast. */
struct Unk085816F0
{
    /* 0x00 */ u8 *unk00;
    /* 0x04 */ u16 *unk04;
    /* 0x08 */ u16 *unk08;
    /* 0x0C */ s16 unk0c;
    /* 0x0E */ u8 filler_0e[0x02];
};
extern struct Unk085816F0 gUnknown_085816F0[];
/* Wave 56 (W56-P). THREE SEPARATE sprite object lists, not one object and not a
 * continuation of the struct array above -- sub_0806AB9C hands each of them
 * straight to PutSprite's / PutSpriteExt's `u16 *` fourth parameter with no
 * arithmetic, the same shape as gUnknown_0849B6D6 and gUnknown_08581A98, and
 * declared the same way (that parameter is unqualified, so none may be const).
 *
 * The boundaries are not guessed from the 8 / 0x1a address gaps -- an object
 * list is SELF-DESCRIBING (a halfword count followed by three halfwords per
 * sprite) and the counts in baserom.gba close each blob exactly at the next
 * symbol:
 *   [0x08581730] = 0001 8000 8000 0000                    -> 1 sprite,  8 B
 *   [0x08581738] = 0004 + 4x3 halfwords                   -> 4 sprites, 0x1a B
 *   [0x08581752] = 0001 0000 4000 0314                    -> 1 sprite,  8 B
 * So they are three objects, and gUnknown_085816F0 really does end at index 3
 * (0x08581730), which is consistent with every indexed read of it. */
extern u16 gUnknown_08581730[];
extern u16 gUnknown_08581738[];
extern u16 gUnknown_08581752[];
/* Wave 48 (W48-C). A ROM halfword table indexed by sub_0807B7BC's return value
 * (`lsls #1`) in sub_0806AB24, where the element is a per-line WIDTH in pixels:
 * it is multiplied by `lines - 1`, added to the residual width sub_0807B7BC
 * writes back through its `u16 *` out-parameter, and the total is centred
 * against 0xf0 (the screen width). `u16` from the `ldrh`; signedness unproved,
 * the only consumer is a multiply. Left unsized -- the index is a line count
 * with no checked bound. Not declared const: nothing proves it is never
 * written, and const would be a claim rather than a reading. */
extern u16 gUnknown_085816B4[];

/* ---- wave 32 (W32-C): the 0x0806C block ---- */

/* 0x0202F2C0. A word flag: sub_0806C218 clears it with `str`, and both
 * sub_0806C380 and sub_0806C410 gate a sub_0806C1E4 palette cycle on
 * `!= 0`. Nothing narrows it anywhere, so `int`. */
extern int gUnknown_0202F2C0;
/* sub_0806C218's two fixed blobs: 081A3D24 is an ApplyPaletteExt source
 * (`u16 *`) and 081A3BD4 a Decompress source (`u8 *`). Neither may be const --
 * both parameters are plain pointers. */
/* Wave 33, W33-D. sub_08067BD0's two blobs, typed from their consumers:
 * 08183780 is the `u16 *` ApplyPaletteExt loads into slot 0xa0, and 08581044 a
 * WORD TABLE of Decompress sources indexed by that function's first argument
 * (`lsls #2`, then `ldr`), so its element is `u8 *`. Neither may be const --
 * both of those parameters are plain pointers. The table's extent is not
 * measured; only the one indexed read exists. */
extern u16 gUnknown_08183780[];
extern u8 *gUnknown_08581044[];
/* Wave 33, W33-D. sub_0806C474's three blobs, typed from their consumers in
 * the same way as sub_0806C218's pair below: 081A29E4 is an ApplyPaletteExt
 * source (`u16 *`), 081A23B4 and 081A2854 are Decompress sources (`u8 *`) into
 * 0x06008000 and 0x0600F800. None may be const -- both parameters are plain
 * pointers -- and only the addresses are ever used. */
extern u16 gUnknown_081A29E4[];
extern u8 gUnknown_081A23B4[];
extern u8 gUnknown_081A2854[];
extern u16 gUnknown_081A3D24[];
extern u8 gUnknown_081A3BD4[];
/* OAM blobs. 08581A98 is PutSprite / PutSpriteExt's `u16 *` fourth argument in
 * sub_0806C380 and sub_0806C410; 08581A44 and 08581A5E are the two sub_0806C668
 * stores into the child proc's +0x54, which src/decomp/c_0806C1C8.c already
 * types `u16 *` from its own PutSprite call. */
/* WAVE 35 (W35-D), the 0x0806D block's ROM tables.
 * gUnknown_08581E70: a u16 step table indexed by struct Unk08580934_Obj's s16
 *   unk26 (`lsls #1; adds; ldrh`), added to unk2c by sub_0806D208 and
 *   subtracted by sub_0806D4DC.
 * gUnknown_08581F20: SIGNED halfwords -- sub_0806D688 reads it with
 *   `movs rI,#0; ldrsh` and then computes 0xc0 minus the result.
 * gUnknown_08581F04 / gUnknown_08581F12: two sprite scripts sub_0806D688
 *   selects between and parks in the object's +0x3c, which sub_0806D7A4 then
 *   compares against gUnknown_08581F12 to pick sprite id 0x8c or 0x8d. Only
 *   ever used as an opaque address, so the element type is a floor.
 *   0x0816E1A0 IS NOT A GLOBAL: baserom.gba holds 0x08581F12 there, i.e. the
 *   -fforce-addr pool word for this symbol, emitted because sub_0806D688
 *   references it twice.
 * gUnknown_08581F68 / gUnknown_08581F74: u16 tables sub_0806DF58 indexes with
 *   gUnknown_08580934->unk33 and ->unk02 to pick a graphics id. */
/* WAVE 35 (W35-D): the fade-kind dispatch table behind sub_080722B8
 * (StartFadeCore). Stride 12 (`lsls #1; adds; lsls #2`), and all three words
 * are reached: +0x00 is called with (gUnknown_08613EE4, parent) through
 * `bl _call_via_r2` -- two arguments, and it returns the proc the function
 * then writes +0x54/+0x4c on, so it is a Proc_Start-shaped starter; +0x04 is
 * called with ONE argument through `_call_via_r1`; +0x08 is the per-kind
 * multiplier that argument is scaled by, narrowed to s8 at the call. */
struct Unk81CBF68
{
    /* 0x00 */ ProcPtr (*unk00)(const struct ProcCmd *, ProcPtr);
    /* 0x04 */ void (*unk04)(int);
    /* 0x08 */ int unk08;
};
extern const struct Unk81CBF68 gUnknown_081CBF68[];
/* sub_08073304's two ROM assets: the compressed tile set it hands to
 * Decompress, and the 0x20-byte palette it uploads through ApplyPaletteExt.
 * Element types are the argument types of those two calls, nothing more. */
extern u8 gUnknown_081CC038[];
extern u16 gUnknown_081D2224[];
extern const u16 gUnknown_08581E70[];
extern const s16 gUnknown_08581F20[];
extern const u16 gUnknown_08581F04[];
extern const u16 gUnknown_08581F12[];
extern const u16 gUnknown_08581F68[];
extern const u16 gUnknown_08581F74[];
extern u16 gUnknown_08581A98[];
extern u16 gUnknown_08581A44[];
extern u16 gUnknown_08581A5E[];
extern const struct ProcCmd gUnknown_08581A80[];
/* 0x03000610. sub_0806CFC8 compares it against `gGameClock - 1` and then
 * republishes the frame counter into it -- a "last frame I ran" latch, and the
 * same s32 as the counter it holds. */
extern s32 gUnknown_03000610;
/* 0x03000614 / 0x03000616. sub_0806CFC8's previous-frame cursor position, one
 * signed halfword each (`movs rI,#0; ldrsh rD,[rB,rI]` -- Thumb's only ldrsh is
 * the register-offset form, so a zero displacement is a plain s16 scalar).
 * They sit two bytes apart, right after gUnknown_0300060C / gUnknown_03000610.
 *
 * gUnknown_0816E17C AND gUnknown_0816E180 ARE NOT GLOBALS. They are agbcc's own
 * address-constant pool (`-fforce-addr`): baserom.gba holds 0x03000614 and
 * 0x03000616 at those two ROM addresses, four bytes apart, and gen_lds.py
 * invented a symbol at each. sub_0806CFC8 loads the ROM word and dereferences
 * it, which is exactly what agbcc emits when a global's ADDRESS is CSE'd into a
 * pseudo that lives across a call -- measured in wave 32 (W32-C) with five
 * probes: the same symbol referenced ONCE gets a direct inline pool word, and
 * referenced twice with a call in between gets the .rodata indirection. Naming
 * `gUnknown_0816E17C` as an `s16 *` reproduces the pool words but adds one
 * `ldr` per use, because the ROM word is already the indirection. */
/* WAVE 34 (W34-B) RETRACTS "CANNOT BE DECLARED". The claim was that
 * 0x03000600/0x03000602 and 0x03000614/0x03000616 fall INSIDE regions
 * aw2bhr.lds already names, so declaring them fails the split build with
 * `undefined reference`. The diagnosis was right and the conclusion was wrong:
 * the region boundaries are not a constraint, they are just the set of lines
 * anyone had written. Both linker scripts assign a strictly ASCENDING location
 * counter, so an interior address is only undeclarable until someone adds the
 * line for it -- and overlapping/interior symbols are already normal there (see
 * the gUnknown_0200BC14 / gUnknown_0200BFFC note).
 *
 * Wave 34 added four lines to aw2bhr.lds AND aw2bhr.split.lds:
 *     . = 0x000600 / 0x000602 / 0x000614 / 0x000616
 * which is what wave 32's own park note prescribed but declined to do. The
 * `undefined reference` was never evidence about the source spelling.
 *
 * This unblocks THREE functions that were parked on it across two waves --
 * sub_08064474, sub_08064500 (0x600/0x602) and sub_0806CFC8 (0x614/0x616) --
 * all of which had already been verified byte-for-byte with the honest
 * spelling. NOTE FOR THE NEXT WAVE: both .lds files are generated by
 * tools/gen_lds.py, so if that script is ever re-run these four lines must be
 * re-added or taught to it. */
extern s16 gUnknown_03000600;
extern s16 gUnknown_03000602;
extern s16 gUnknown_03000614;
extern s16 gUnknown_03000616;
/* sub_0806D050's set, one latch further along and the exact twin of
 * gUnknown_03000610 / gUnknown_03000614 / gUnknown_03000616 above: sub_0806D050
 * is sub_0806CFC8's source with a different sprite id (0x44 vs 0x43) and this
 * second triple. Same evidence -- s32 latch compared against
 * `gGameClock - 1`, two s16 coordinates read `movs rI,#0; ldrsh`.
 *
 * WAVE 35 (W35-D): 0x0816E184 and 0x0816E188 ARE NOT GLOBALS -- baserom.gba
 * holds 0x0300061C and 0x0300061E at those two ROM addresses, four bytes
 * apart, i.e. agbcc's own -fforce-addr address-constant pool, exactly as
 * 0x0816E17C / 0x0816E180 are for the 0x614/0x616 pair. Naming the RAM
 * globals directly is the honest spelling and the build places the pool word.
 * Interior to gUnknown_03000618, so both linker scripts gained
 * `. = 0x00061C` / `. = 0x00061E` (see the wave-34 retraction above). */
extern s32 gUnknown_03000618;
extern s16 gUnknown_0300061C;
extern s16 gUnknown_0300061E;
/* sub_08064500's pair, one latch along and interior to gUnknown_03000604 --
 * declarable for the same reason, with the same two lines added. Same s16
 * evidence: `movs rI,#0; ldrsh rD,[rB,rI]`, Thumb's register-offset-only
 * ldrsh at a zero displacement, i.e. a plain signed halfword scalar. */
extern s16 gUnknown_03000608;
extern s16 gUnknown_0300060A;

/* ---- wave 32 (W32-C): the 0x0806E and 0x08049 blocks ---- */

/* Three OAM blobs handed straight to PutSprite's `u16 *` fourth argument
 * (sub_0806E1B8, sub_0806E830), with NO load in front of the pool word -- the
 * symbol IS the data. Confirmed in baserom.gba: each is `0001 xxxx 0000 xxxx`,
 * a one-entry sprite script. Eight bytes apart and therefore genuinely three
 * symbols, not agbcc's four-byte address-constant pool. */
extern u16 gUnknown_0816E7F0[];
extern u16 gUnknown_0816E7F8[];
extern u16 gUnknown_0816E800[];
extern const struct ProcCmd gUnknown_08582B14[];
/* Wave 46 (W46-G). The argument table sub_0806E510 walks to spawn the six
 * gUnknown_08582B14 procs: it is 0x78 bytes in data/data-08581E70.s and the
 * loop runs `i <= 5` with a 0x14 stride, so exactly SIX rows of 20 bytes and
 * the extent is pinned by the ROM's own extent, not inferred.
 *
 * The member widths come from sub_0806E4BC's promoted definition
 * (src/decomp/c_0806E4BC.c), which is authoritative: +0x00/+0x04/+0x08/+0x0c
 * are forwarded to `int` parameters and are read with `ldr`, and +0x10 is read
 * with `ldrh` into the one `u16` parameter. Bytes +0x12..+0x13 are never
 * touched by anything and are tail padding as far as the ROM shows.
 *
 * `const` because it lives in ROM and nothing writes it. */
struct Unk08582A7CEnt
{
    /* 0x00 */ int unk00;
    /* 0x04 */ int unk04;
    /* 0x08 */ int unk08;
    /* 0x0c */ int unk0c;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u8 filler_12[0x02];
};
extern const struct Unk08582A7CEnt gUnknown_08582A7C[];
/* A 3-byte-per-row table sub_0806E830 indexes with `proc->unk60 * 3` (built as
 * `i*2 + i`) and reads with three `ldrb`s: +0 and +1 are two x positions and +2
 * the shared y. A flat u8 array and not a struct -- the row offset is added to
 * the SYMBOL and the element offsets to the row, which a struct member would
 * have folded into the ldrb displacement. */
extern u8 gUnknown_08582C1C[];
/* sub_08049D88's palette source, ApplyPaletteExt's `u16 *`. */
extern u16 gUnknown_081268B8[];
/* Wave 56, W56-H, all four from sub_08027FF4's argument slots.
 * gUnknown_08125A8C is a Decompress SOURCE; gUnknown_080A1178 is a
 * sub_08011E54 source pushed to 0x060045E0; gUnknown_080A12B8 is
 * sub_08012B70's `u16 *` second argument; and gUnknown_080A1238 is a run of
 * 16-colour palettes handed to ApplyPaletteExt one bank at a time, indexed
 * `gPlayers[gUnknown_030033EC].unk1a - 1` -- the `<< 5` on that index
 * is the 0x20-byte bank stride, which is what fixes the [][16] shape. */
extern u8 gUnknown_08125A8C[];
extern u8 gUnknown_080A1178[];
extern u16 gUnknown_080A1238[][16];
extern u16 gUnknown_080A12B8[];
/* 0x02028E3D. The slot counter sub_08049BEC post-increments and sub_08049D88
 * resets, both with bare `ldrb`/`strb` at displacement zero. Immediately after
 * gUnknown_02028E3C above and typed the same way. */
extern u8 gUnknown_02028E3D;
/* 0x03002EF0. sub_08017E0C publishes the script node's +0x08 halfword here with
 * a bare `strh` at displacement zero. */
extern u16 gUnknown_03002EF0;

/* ---- wave 32 (W32-C): the 0x08031 block ---- */

/* 0x02025760. The second gUnknown_0202575C-shaped packet: sub_080319AC and
 * sub_08031A24 fill +0/+1/+2 with the same three stores and hand it to
 * sub_0802F588, whose first parameter is already `struct Unk0202575C *`. */
extern struct Unk0202575C gUnknown_02025760;
/* 0x02025764 -- Wave 45, W45-C. The payload record sub_08031824 builds, sitting
 * immediately after the 4-byte command record gUnknown_02025760. Every offset
 * below is read straight off that function, which is the only writer:
 *
 *   +0x00  2 bytes  <- gUnknown_02028030.unk10
 *   +0x02  3 bytes  <- gUnknown_02028030.unk2a
 *   +0x05  0x18     <- gUnknown_02028030.unk12
 *   +0x22  3 x 0x1c per-slot records
 *
 * The three head arrays are VOLATILE and the slot records are NOT, which is
 * unusual enough to state the evidence: each of the first three copy loops
 * emits a dead `ldrb` of the DESTINATION immediately before its `strb` -- the
 * same tell that settled gUnknown_0300449C and gUnknown_020257E4 -- and the
 * four loops that fill the slot records emit no such load. Both halves are
 * plain `dst[i] = src[i];` in the source.
 *
 * The slot stride is 0x1c for 0x19 bytes of content, and the padding is real:
 * the ROM multiplies the slot index by 28 as `((j << 3) - j) << 2`. The slot
 * mirrors struct Unk020280C0 -- [0x00] takes that type's NUL-terminated
 * unk02[0x11] name (via sub_0803CCB8), [0x12]..[0x16]/[0x17]/[0x18] take its
 * filler_14[0]..[4], [5] and [6] -- so the three unnamed members of
 * filler_14 could be named from here, but nothing in this function
 * discriminates their widths and filler_14 is shared, so it is left alone.
 *
 * THE SLOT RUN CANNOT BE A STRUCT ARRAY, and this is a general readout rather
 * than a fact about this record: agbcc uses the old ARM
 * STRUCTURE_SIZE_BOUNDARY of 32, so EVERY struct has size and alignment
 * rounded to 4 no matter how narrow its members are. A 0x19-byte all-u8 slot
 * therefore gets sizeof 0x1c for free -- which is where the ROM's
 * `((j << 3) - j) << 2` stride comes from -- but it also gets ALIGNMENT 4,
 * and 0x22 is not a multiple of 4, so declaring the run as a struct array
 * silently pushes it to 0x24 and shifts every later member offset by two.
 * A 2-D byte array has alignment 1 and lands where the ROM has it. */
struct Unk02025764 /* >= 0x76 */
{
    /* 0x00 */ volatile u8 unk00[2];
    /* 0x02 */ volatile u8 unk02[3];
    /* 0x05 */ volatile u8 unk05[0x18];
    /* 0x1d */ u8 filler_1d[5];
    /* 0x22 */ u8 unk22[3][0x1c];
};
extern struct Unk02025764 gUnknown_02025764;
/* Wave 42 (W42-K). A third packet header of the same 4-byte shape, 0x88 below
 * gUnknown_0202575C: sub_08030D84 fills unk00 = 0xad, unk01 and unk02, then
 * hands `&gUnknown_020256D4` to sub_0802F588, whose first parameter is already
 * `struct Unk0202575C *`. The 13-byte payload at +6 is a SEPARATE symbol rather
 * than a member of it: the ROM builds the two addresses from two independent
 * pool words, in first-reference order (payload first), where one base plus a
 * displacement would have emitted a single word. */
extern struct Unk0202575C gUnknown_020256D4;
extern u8 gUnknown_020256DA[];
/* sub_08031DC0's ROM blob, sub_080149C0's `u8 *` fourth argument. Real data,
 * not a pool word: baserom.gba holds 0x00003143 there, and the symbol is used
 * as a value with no load in front of it.
 *
 * ITS FIVE NEIGHBOURS 0x08090D04 / 0x08090D08 / 0x08090D0C / 0x08090D10 /
 * 0x08090D14 ARE NOT GLOBALS -- they are agbcc `-fforce-addr` words holding
 * &gUnknown_0849B060, &gUnknown_0300449C, &gUnknown_0849B018 (twice) and
 * &gGameClock, four bytes apart, and every reader of them does the
 * extra `ldr` that gives away the indirection. Wave 42, W42-L verified all five
 * against baserom.gba and ADDED the first two: 0x08090D04 holds 0x0849B060 and
 * 0x08090D08 holds 0x0300449C. sub_08031948 reads the 0x08090D08 word with the
 * double `ldr` and matches with `gUnknown_0300449C[i]` named directly. */
extern u8 gUnknown_08090D18[];
/* 0x08090CF4 -- Wave 45, W45-C. Five halfwords, and REAL DATA rather than
 * another of this neighbourhood's -fforce-addr words: sub_080314A4 hands the
 * symbol straight to sub_0808B6E8 as the source of a 10-byte copy with no
 * load in front of it, which is the same argument that fixed gUnknown_08090D18.
 * The element width comes from the consumer, not the copy -- the destination
 * is a stack buffer that sub_080314A4 then indexes `ldrb; lsls #1; ldrh`, i.e.
 * as u16[5], by gUnknown_0849B018->unk0a[i] when that byte is <= 4. */
extern const u16 gUnknown_08090CF4[];
/* 0x0300449C. Wave 42, W42-L. A four-entry per-army counter, indexed 0..3 by
 * the same army loop that indexes gUnknown_0849B018->unk0a[]: sub_08031948
 * clears all four in that loop and sub_08031FD8 bumps entry i once per
 * non-empty slot it finds for army i.
 *
 * VOLATILE on the dead-`ldrb`-before-`strb` tell, and it carries in BOTH
 * functions independently: sub_08031948's plain `= 0` emits `ldrb r1,[r0]`
 * immediately before the `strb`, and sub_08031FD8's `++` emits a SECOND,
 * entirely dead `ldrb r1,[r3]` between the `adds r0,#1` and the `strb`. That is
 * the same tell that settled struct Unk0849B018's members and
 * struct Unk02025564's unk00/unk02/unk05.
 *
 * u8 from the `ldrb`/`strb` pair; signedness unproved, as neither reader does
 * anything with the value but store it back. The extent 4 is the army-slot
 * range both loops run over and is left off the declaration rather than
 * claimed. */
/*   Wave 42 (W42-K): this is now the SINGLE declaration of the symbol. For a
 * few minutes it was declared twice with conflicting types -- `u8` beside an
 * older extent note earlier in the file and `volatile u8` here -- which made
 * unknown-globals.h uncompilable for the whole wave. W42-L's `volatile` above
 * is the surviving model and the right one: the older entry's "plain ldrb
 * feeding cmp #0" reasoning is byte-neutral and never argued against it. */
extern volatile u8 gUnknown_0300449C[];
/* A three-entry ROM table of sub_0801BD00 blobs, cycled at
 * `(gGameClock >> 3) % 3` by sub_08031CF4 and sub_08031D54. */
extern void *gUnknown_0849B074[];
/* The digit strips sub_08031C58 draws: 084C145E and 084C1466 are fixed blobs,
 * 084C170C and 084C178C two tables of them indexed by the tens digit and by
 * `10 - tens`. All four go to sub_0801BD00's `void *` third parameter. */
extern u8 gUnknown_084C145E[];
extern u8 gUnknown_084C1466[];
extern void *gUnknown_084C170C[];
extern void *gUnknown_084C178C[];
/* The halfword tile table sub_080315E8 hands to sub_08014668, indexed by its
 * own second parameter with a plain `lsls #1` off the bare symbol. */
extern u16 gUnknown_0849B0E2[];
/* A 20-byte ROM row indexed by `proc->unk2c`, and the stride is read off the
 * `lsls #2; adds; lsls #2` multiply-by-5-then-4 chain rather than guessed.
 * Three Decompress sources at +0x00/+0x04/+0x08, one per sibling
 * (sub_0806ADD0/sub_0806ADF0/sub_0806AE18), each written to the same
 * `proc->unk30` destination 0x1B00 bytes further along. +0x0C..+0x13 unread. */
struct Unk0858178C
{
    /* 0x00 */ u8 *unk00;
    /* 0x04 */ u8 *unk04;
    /* 0x08 */ u8 *unk08;
    /* 0x0C */ u8 *unk0c; /* wave 32 (W32-C): sub_0806AEC4 Decompresses all
                           * FOUR of these in a row, +0x0c into the buffer
                           * gUnknown_08499578 points at. */
    /* 0x10 */ u16 *unk10; /* the row's palette, ApplyPaletteExt's `u16 *`.
                            * sub_0806AEC4 reads it in both arms of an
                            * `a1 == 7` test and cse constant-folds the taken
                            * arm's index, which is why the ROM shows a bare
                            * `+0x9c` (== 7 * 0x14 + 0x10) with no `lsls`. */
};
extern struct Unk0858178C gUnknown_0858178C[];

/* ---- wave 30 (W30-C): the 0x08066 and 0x08076 blocks ---- */

/* A ROM blob handed straight to sub_0801BD00's `void *` third parameter by
 * sub_08066340, with no load in front of the pool word. Non-const for the same
 * -Werror reason as gUnknown_085815C8. */
extern u8 gUnknown_08580CD4[];
/* A gUnknown_03001470 script blob: sub_08066580 hands it to
 * sub_080152EC(script, 1) and stashes the returned slot in
 * gUnknown_08580934->unk74[]. `const u8 []` -- sub_080152EC's first parameter is
 * `const void *` -- on the same reasoning as gUnknown_08580DD8. */
extern const u8 gUnknown_08580D0C[];
/* A halfword colour table read at `(gGameClock & 0x3F) / 4` by
 * sub_080763C0, which writes the result into gPal at byte offset 0x2AE. The
 * exact twin of gUnknown_081A47E4 and gUnknown_081A3D84, which the same idiom
 * reads at `& 0x1F` / 2. Sixteen entries are reachable; nothing bounds it. */
extern u16 gUnknown_081D22A4[];
/* The compressed tile blob sub_08076ADC and sub_08076B7C both unpack into
 * gUnknown_08614280. `u8 []` and non-const: Decompress's first parameter is a
 * plain `u8 *`. */
extern u8 gUnknown_081D0BAC[];
/* Wave 34, W34-G. Two more ROM blobs of the same 0x081Dxxxx run, both reached
 * only by sub_080755F0 and typed straight off the parameter each is handed to
 * with no arithmetic in between: 081D20AC goes to ApplyPaletteExt's `u16 *`
 * first parameter (0x20 halfwords at palette byte offset 0x2C0), and 081D18E8
 * is a compressed blob for Decompress's `u8 *` first parameter, unpacked to
 * VRAM at 0x06011480. Neither is const, because neither parameter is. */
extern u16 gUnknown_081D20AC[];
extern u8 gUnknown_081D18E8[];
/* Wave 34, W34-G. The same pair one screen over, for sub_080763F4: 081D2284 is
 * ApplyPaletteExt's `u16 *` at palette byte offset 0x2A0, and 081D33BC a
 * Decompress `u8 *` blob unpacked to 0x06012600. */
extern u16 gUnknown_081D2284[];
extern u8 gUnknown_081D33BC[];
/* Wave 34, W34-G. A ROM palette table of 0x20-byte (0x10-halfword) rows.
 * sub_08075DBC picks row `unk64 - 6` -- `lsls #5` on the s16 at +0x64 of its
 * proc -- and hands the row to sub_080135F4, whose first parameter is `u16 *`;
 * that is what fixes the element type, and the stride then makes the row count
 * the only free variable. Nothing bounds it. */
extern u16 gUnknown_081D1504[];
/* Wave 34, W34-G. The two alternative blobs sub_080767C0 picks between on
 * IsHardCampaignMode's result and hands to sub_08073304's `const void *` first
 * parameter, everything else about the two calls being identical. `u8 []` and
 * non-const on the same model as gUnknown_085826E0 and gUnknown_085802F0, the
 * blobs the already-promoted callers of sub_08073304 pass there. Nothing
 * dereferences either one, so the element type rests on that parameter alone. */
extern u8 gUnknown_0861452C[];
extern u8 gUnknown_08614538[];
/* Wave 34, W34-G. An 8-byte record array indexed by +0x58 of the sub_080763F4
 * proc -- `lsls #3` for the stride, and the ROM reaches the second word by
 * bumping the base pointer (`adds r4, #4`) rather than by a second symbol, so
 * the two words are ONE record and not two parallel tables. unk00 is a
 * compressed blob handed to Decompress's `u8 *`; unk04 is copied straight into
 * +0x60 of the proc, which c_0807662C.c reads as an int x-offset added to the
 * +0x2c sprite coordinate. NOT const: the same reasoning as gUnknown_0861433C,
 * and Decompress's parameter is not const either. */
struct Unk086144DC
{
    /* 0x00 */ u8 *unk00;
    /* 0x04 */ int unk04;
};
extern struct Unk086144DC gUnknown_086144DC[];
/* An EWRAM-ish scratch buffer reached through one indirection -- every user
 * does `ldr rN, =gUnknown_08614280; ldr rN, [rN]` before touching it, so the
 * symbol is a POINTER and not the buffer. sub_08076ADC/sub_08076B7C Decompress
 * into it and then sub_08011E54 0x1000 bytes of it to 0x0600F000; sub_08075F44
 * indexes it as halfwords at `x + y * 32`, optionally biased by 0x7C0. `void *`
 * because the two element views disagree and neither is proved primary. */
extern void *gUnknown_08614280;
/* A fourth ROM blob of the gUnknown_084B9F00 family -- handed to
 * sub_080785CC's `const void *` fourth parameter by sub_08076BC4 and
 * sub_08076C64 and to nothing else, so nothing dereferences it. */
extern const u8 gUnknown_084BA6D0[];

/* ---- wave 30 (W30-C): the 0x08063 block ---- */

/* Two 16-colour ROM palettes, both handed to ApplyPaletteExt for 0x20 bytes:
 * 0812B81C by sub_08063760 at slot `(n & 0xF) + 0x10`, 08131B4C by
 * sub_080639E8 at a slot the caller supplies. `u16 []` and NOT const --
 * ApplyPaletteExt's first parameter is a plain `u16 *`, so const breaks the
 * call under -Werror. Extent not pinned beyond the 0x20 bytes transferred. */
extern u16 gUnknown_0812B81C[];
extern u16 gUnknown_08131B4C[];

/* HOISTED here in wave 30 (W30-C) from src/decomp/c_08062FB8.c, unchanged in
 * layout. sub_08063430 is the other half of the same link-hardware reset -- it
 * reads +0x18, and on the zero arm writes +0x4a, +0x1e and +0x18, all bytes at
 * the same displacements sub_08062FB8 uses -- and it PASSES the pointer to
 * sub_08062FB8, so the two units need one shared type rather than two
 * coincidentally-identical local ones. */
/* Wave 33 (W33-C) added +0x1c, +0x20 and +0x24 from sub_08063454, which sets
 * the record up for a transfer: +0x20 takes the caller's buffer pointer whole
 * (`str`), +0x24 that pointer plus the 16-byte-rounded length, and +0x1c one
 * PACKED BYTE built as `(mode << 1) | 0x81` -- see src/decomp/c_08063454.c.
 * The two words force the struct's alignment to 4, which changes no member
 * offset (0x20 and 0x24 were already aligned) and nothing allocates one.
 *
 * NOT the same record as struct Unk8063BE0 below, despite both carrying +0x24,
 * +0x28 and something at +0x1c: this one is written at +0x1c with `strb` and
 * that one is read with `ldrh`, and Unk8063BE0's consumer sub_08063CCC is
 * SetObjAffine sprite maths rather than link hardware. Address proximity in the
 * 0x08063 block is not kinship. */
/* Wave 46 (W46-B) carved +0x00 and +0x04 out of the old filler_00[0x16] from
 * sub_08063528, the SIO multiplayer poll. Both are whole-word `ldr`/`str`;
 * neither offset moves and the struct was already 4-aligned.
 *   +0x00 is the shift register: it is stored `<< 5`, read back `>> 5` with a
 * LOGICAL `lsrs`, tested against 0 as a full word, and its LOW HALF is read
 * separately with a bare `ldrh` as sub_080633E4's second argument. A word that
 * is both shifted unsigned and truncated to u16 at a use is `int` here -- the
 * ldrh is a (u16) cast at the call, not a narrower member, because the same
 * offset takes 0x100000 whole.
 *   +0x04 is the last value broadcast, compared for equality against each
 * SIOMULTI halfword. Equality only, so the compare does not discriminate the
 * sign; `int` is the weakest fit that also takes the `<< 5`-free whole-word
 * stores. */
struct Unk08062FB8
{
    /* 0x00 */ int unk00;
    /* 0x04 */ int unk04;
    /* 0x08 */ u8 filler_08[0x0e];
    /* 0x16 */ u16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 filler_19[0x03];
    /* 0x1c */ u8 unk1c;
    /* 0x1d */ u8 unk1d;
    /* 0x1e */ u8 unk1e;
    /* 0x1f */ u8 filler_1f[0x01];
    /* 0x20 */ int unk20;
    /* 0x24 */ int unk24;
    /* 0x28 */ u8 *unk28; /* Wave 34, W34-C: sub_08033194 seeds it with the base
                           * of the blob gUnknown_030032DC also points at, a
                           * whole-word `str` at a displacement `strb`/`strh`
                           * cannot reach. Carved out of the old
                           * filler_28[0x20]. */
    /* 0x2c */ u8 filler_2c[0x1c];
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 filler_49[0x01];
    /* 0x4a */ u8 unk4a;
    /* 0x4b */ u8 unk4b; /* Wave 34, W34-C: sub_08033194 clears it immediately
                          * before handing the record to sub_08062FB8, reached
                          * by `adds rB,#0x4b` on the record base. */
};

/* HOISTED here in wave 30 (W30-C) from src/decomp/c_08063BE0.c, unchanged in
 * layout (STRUCT_PAD spelled out, since this header does not use that macro).
 * sub_08063BBC seeds `unk44 = 8` and then tail-calls sub_08063BE0 on the same
 * pointer, which is the counter sub_08063BE0 decrements -- so the same type,
 * and the s16 width is sub_08063BE0's, not a fresh guess. */
/* Wave 33 (W33-C) added +0x2c, +0x42 and +0x48 from sub_08063B50, the frame
 * tick that feeds sub_08063CCC. +0x2c is `ldr`/`str` whole and gets a signed
 * `* 3 / 4` (the `bge`/`adds #3`/`asrs #2` round-toward-zero sequence), so it
 * is a signed word. +0x42 is a second countdown read exactly the way +0x44 is
 * -- `ldrh` for the decrement, `movs rN,#0; ldrsh` for the test -- which is
 * s16 by the same argument that typed +0x44. +0x48 is `ldrb`. */
struct Unk8063BE0
{
    /* 0x00 */ u8 filler_00[0x1c];
    /* 0x1c */ u16 unk1c;
    /* 0x1e */ u8 filler_1e[0x06];
    /* 0x24 */ int unk24;
    /* 0x28 */ int unk28;
    /* 0x2c */ int unk2c;
    /* 0x30 */ u8 filler_30[0x12];
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ u8 filler_46[0x02];
    /* 0x48 */ u8 unk48;
};

/* ---- wave 46 (W46-B): the 0x08064 block ---- */

/* 0x0202F140. EIGHT 0x18-byte entries, each holding TWO three-word vectors.
 * sub_0806412C seeds the first vector of every entry from gUnknown_0858089C
 * with `stm r3!, {r0}` three times at stride 0x18; sub_08064214 then walks the
 * same stride handing sub_08063DDC `&e->unk00` and `&e->unk0c` as two separate
 * arguments (`adds r4, #0xc` off the base, kept in its own register alongside
 * the unbiased one). That pair of arguments is what splits the entry into two
 * vectors rather than one six-word block.
 *
 * `int` throughout: every access is a whole-word `ldr`/`str`/`stm`, and the
 * seeded values are `gUnknown_0858089C[k] << 0xc`, i.e. 20.12 fixed point.
 *
 * The consumer names the same storage `struct Vec3 { s32 x, y, z; }` -- see
 * src/decomp/c_08063DDC.c, which defines that tag file-locally. It is NOT
 * defined here on purpose: a definition in this header would collide with that
 * file's own, so callers cast to `struct Vec3 *` at the call instead. Two
 * three-word vectors and one 0x18 stride is the same fact either way. */
struct Unk0202F140Entry
{
    /* 0x00 */ int unk00[3];
    /* 0x0c */ int unk0c[3];
};
extern struct Unk0202F140Entry gUnknown_0202F140[8];

/* 0x0202F110. SIX 8-byte entries. sub_0806412C writes `unk00` of entries 0..5
 * as six `strh` at displacements 0, 8, 0x10, 0x18, 0x20 and 0x28 off ONE pool
 * word, and fills `unk02` from gUnknown_085808CC four `strb` at a time off a
 * base pre-biased by 2 (`adds r4, r0, #2`, then `+ i * 8`). The trailing two
 * bytes are never touched by that function and are filler, not measured. */
struct Unk0202F110Entry
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u8 unk02[4];
    /* 0x06 */ u8 filler_06[0x02];
};
extern struct Unk0202F110Entry gUnknown_0202F110[6];

/* Wave 50 (W50-H): the two ROM operands of sub_08064288's two sub_0801BD00
 * calls, both undeclared until now.
 *
 * gUnknown_0858092C is the address ITSELF, not a pool word: sub_08064288 loads
 * it and hands it straight to sub_0801BD00's `void *` third parameter with no
 * dereference, where a -fforce-addr word would have shown the `ldr rN,=s;
 * ldr rM,[rN]` pair. Non-const because sub_0801BD00's parameter is `void *`.
 *
 * gUnknown_08580914 is a real table by wave 49's inverse test -- it is INDEXED
 * by a computed subscript before being dereferenced (`lsls r0, rI, #2; adds
 * r0, r0, rBase; ldr r2, [r0]`). Word elements feeding the same `void *`
 * parameter, so an array of pointers. The extent is unmeasured: sub_08064288's
 * first loop makes exactly one trip, so only element 0 is ever proved. */
extern u8 gUnknown_0858092C[];
extern void *const gUnknown_08580914[];

/* Two ROM tables read by sub_0806412C only, both walked linearly by a pointer
 * that is never reset between rows -- so the element counts are the totals it
 * consumes, 8 rows x 3 and 6 rows x 4, and not a stride.
 * gUnknown_0858089C is read with `ldrsh` (SIGNED halfwords, the sign is the
 * instruction's, not a guess) and gUnknown_085808CC with `ldrb`. */
extern const s16 gUnknown_0858089C[24];
extern const u8 gUnknown_085808CC[24];

/* 0x030005F4 / 0x030005F8. Written together at the end of sub_0806412C, one
 * whole-word `str` each, from `(u16)param * 0x1000` -- 20.12 again, the same
 * format the table above is scaled into. Nothing in this wave READS them, so
 * the width is the store's and the signedness is unproven. */
extern int gUnknown_030005F4;
extern int gUnknown_030005F8;

/* ---- Wave 30, W30-B: the 0x08005 / 0x08014 / 0x0801C / 0x08027 / 0x08028
 * address-locality blocks ---- */

/* 0x0200BC14. TWO PARALLEL u16 ARRAYS 0x408 apart, and it has to be one object:
 * sub_080148A0 materialises ONE pool word (=gUnknown_0200BC14), builds 0x408 as
 * `movs r6,#0x81; lsls r6,#3` and adds it, then adds the SAME `i * 2` byte
 * offset to both bases. Naming a second symbol would have emitted a second pool
 * word and folded the displacement into the relocation instead.
 *
 * NOTE THE OVERLAP, because it looks like a bug and is not: aw2bhr.lds binds
 * gUnknown_0200BFFC at 0x0200BFFC, i.e. 0x3e8 bytes in, so the +0x408 member
 * lands 0x20 bytes inside that symbol's range. gen_lds.py only emits
 * `. = addr; sym = .;` -- it reserves no space and two symbols may cover the
 * same bytes -- and the linker script's boundaries are "next symbol some
 * assembly happens to name", not object extents. The codegen above is the
 * evidence about the SOURCE; the linker script is not evidence at all here.
 * The element counts below are therefore a floor from the two known readers
 * (index 0, and an index that arrives as a u8), not a measurement. */
/* Wave 32 (W32-C): a THIRD array sits between them. sub_08014910 and
 * sub_0801496C write `*(u32 *)(g + 0x400 + i * 4)` off the SAME pool word,
 * between the +0x000 and +0x408 stores, so unk000 stops at 0x200 elements
 * (0x000..0x3ff) rather than the 0x204 that was only ever a filler to 0x408.
 * unk408 keeps its offset, which is the part any promoted code depends on.
 * unk400's element count is a floor of two, not a measurement -- the two
 * writers reach it with the same index they use on the u16 arrays, so either
 * that index is tiny here or the three arrays are not really parallel. */
struct Unk0200BC14
{
    /* 0x000 */ u16 unk000[0x200];
    /* 0x400 */ u32 unk400[2];
    /* 0x408 */ u16 unk408[0x100];
};
extern struct Unk0200BC14 gUnknown_0200BC14;
/* 0x0300004C. The allocator's "block being worked on" cursor. sub_08014EF4
 * stores a heap block header pointer through it three times (the block, then
 * its successor, then the block again) and once more with the split block in
 * the split arm. It is reached ONLY through agbcc's -fforce-addr address
 * constant pool, which is why the splitter never invented a name for it and
 * the map shows 0x03000048's `*fill*` covering it: the ROM word at 0x0808E52C
 * holds 0x0300004C, and 0x0808E52C's neighbours at +/-4 hold 0x030030E0 and
 * 0x03000050, i.e. it sits in a run of address constants, not a run of data.
 * `void *` because the pointed-to header type (struct MemBlock) is file-local
 * to src/decomp/c_08014E68.c; every store converts implicitly and costs
 * nothing. The three stores are plain, not volatile -- the reload of the
 * address between them is ordinary aliasing (the store is through an int-ish
 * pointer that may alias the .rodata address constant), not a volatile tell.
 * NO EXTERN CAN EXIST FOR THIS ADDRESS: upstream's aw2bhr.lds IWRAM table
 * jumps 0x48 -> 0x50 and gen_lds.py passes it through verbatim, so the symbol
 * is unlinkable in the split build (proto_check.py catches it). Spell accesses
 * as an offset into the covering symbol instead --
 * `*(void **)((u8 *)&gUnknown_03000048 + 4)` -- which relocates as
 * gUnknown_03000048+4 and resolves to the same address and pool word. See
 * work/sub_08014EF4/sub_08014EF4.c. Wave 42. */
/* 0x03000050. The arena handle sub_08014D7C returns -- sub_08014DA8 stores that
 * function's `int` result here with a plain `str`, and sub_08014E44 /
 * sub_08014ED4 both gate on `!= -1` before forwarding it to sub_08014DCC /
 * sub_08014E68. `int` and not a pointer because -1 is the sentinel and
 * sub_08014D7C is itself declared `int` (src/decomp/c_08014D7C.c). */
extern int gUnknown_03000050;
/* 0x030030F8. sub_080288D8 reads it with a bare `ldrb` and compares it against
 * a u16 team/army id. Width only; nothing in this batch writes it. */
extern u8 gUnknown_030030F8;
/* 0x08090A98. A pair of s16 constants: sub_080276D0 reads [+0] and sub_080276F0
 * reads [+2], both `movs rI,#K; ldrsh r0,[r0,rI]` -- Thumb has no `ldrsh` at an
 * immediate offset, so the register form is what ANY s16 read looks like and is
 * not itself an array tell; the two functions using consecutive elements is.
 * The value goes into the word gUnknown_03003130.unk04. */
extern const s16 gUnknown_08090A98[];
/* 0x08090A84. Wave 41 (W41-C). A REAL byte table, and the counter-example that
 * keeps the -fforce-addr rule honest: its three immediate neighbours
 * (0x08090A70 -> 0x030033EC, 0x08090A80 and 0x08090A8C -> 0x08499598) ARE
 * address words, so the 0x0809xxxx prefix decides nothing and each symbol has
 * to be checked against baserom.gba on its own. The discriminator is the ROM
 * CONTENT: the word here reads 0x04020100, which is no valid address, and the
 * access shape agrees -- sub_08026B28 indexes straight off the symbol
 * (`ldr r1,=sym; mov ip,r1; adds r1,r4,r0; ldrb r1,[r1]`) with no load through
 * it, where an address word always shows the extra `ldr`.
 *   Read as bytes it is {0, 1, 2, 4, 8} at indices 0..4: one BIT per army slot,
 * with index 0 unused, which is exactly the shape of struct PlayerStruct's unk2c
 * ("a FOUR-BIT MASK, one bit per 64-unit army slot"). sub_08026B28 ORs
 * gUnknown_08090A84[j] into army i's unk2c for every other live army j on a
 * different team, so this is the slot-number-to-mask table for that member.
 *   NOT declared `const`, by consistency with gUnknown_084995F4 and
 * gUnknown_084995FE rather than by measurement: sub_08026B28's read is indexed
 * by the inner counter and cannot be hoisted either way, so the LICM oracle
 * those two were settled with has nothing to say here. */
extern u8 gUnknown_08090A84[];
/* Wave 31 (W31-C): gUnknown_08090BC0 and gUnknown_08090BC4 are NOT globals --
 * they are two more of the agbcc `-fforce-addr` address words this 0x0809xxxx
 * run is made of, and the ROM contents settle it: the word at 0x08090BC0 is
 * 0x030033E4 and the word at 0x08090BC4 is 0x03003130. sub_0802BB98's source
 * therefore says `gUnknown_030033E4` and `gUnknown_03003130` directly, and
 * agbcc's own copy supplies the middle `ldr` -- the same mechanism the note on
 * gUnknown_0808D81C records for gUnknown_08499590.
 *
 * Declaring either as a pointer variable and writing `G->field` produces a
 * FOURTH level and is 2 bytes long per use (measured this wave, sub_0802BB98).
 * Note that gUnknown_08090A98 immediately below IS real data -- its first word
 * is 0x00AD0003, not an address -- so proximity is not the test; the content
 * is. */
/* The sprite blob sub_0802BB74 hands to sub_0801BD00's `void *` third
 * parameter, with attr1 masked to its nine coordinate bits and 0x400 set in
 * attr0. `u16 []` is the same model gUnknown_08485CC8's entries carry. */
extern u16 gUnknown_0849A3B8[];
/* An adjacent pair of plain bytes -- both `ldrb` off their own pool word --
 * that sub_08047078 forwards to sub_08046A84 as its two arguments. The odd
 * address of the first rules out an aggregate. */
extern u8 gUnknown_02028DD5;
extern u8 gUnknown_02028DD6;
/* Wave 35, W35-L. The other two bytes of the same run, both written by
 * sub_080470F8 and both plain `strb` through their own pool word.
 * gUnknown_02028DD4 is a 0/1 flag (`movs r0,#1; strb` then `movs r0,#0; strb`
 * on the fail arm of a range test); gUnknown_02028DD7 takes sub_08024984's
 * `int` result narrowed to a byte by the `strb`. Neither is read anywhere
 * matched, so only the width is proved. */
extern u8 gUnknown_02028DD4;
extern u8 gUnknown_02028DD7;
/* Proc/list script blobs, `const u8 []` for the reason gUnknown_0849A108 is:
 * sub_0801537C takes `const void *` and nothing dereferences any of them.
 * gUnknown_0848A398 / gUnknown_0848A3C4 are sub_080145BC's two stops;
 * gUnknown_08489530 is the first of sub_08014878's three (the other two,
 * gUnknown_08489548 / gUnknown_08489568, are already declared above as the
 * blobs sub_08014668 / sub_080146D4 start -- so this block is one start/stop
 * pairing and the const is proved from both ends). */
extern const u8 gUnknown_08489530[];
/* Wave 55, W55-G. Another blob in the same run and the same slot: sub_08014084
 * hands it to `sub_080152EC(blob, 0)` and writes `->unk1e = 0xa` straight into
 * the entry it gets back, which is the start-and-seed shape gUnknown_0848A1EC
 * and gUnknown_0849D53C already record. `const u8 []` for the reason its
 * neighbours are -- sub_080152EC's first parameter is `const void *` and
 * nothing dereferences it. */
extern const u8 gUnknown_08489518[];
extern const u8 gUnknown_0848A398[];
extern const u8 gUnknown_0848A3C4[];
/* Wave 56, W56-F. The same shape one slot further on: sub_08019F90 opens with
 * `sub_080152EC(gUnknown_0848A42C, 0)` and closes with
 * `sub_080152EC(gUnknown_0848A44C, 0)`, so this is the second blob of that
 * pair and takes gUnknown_0848A42C's already-established `const u8 []` for the
 * same reason -- sub_080152EC's first parameter is `const void *` and nothing
 * dereferences it. */
extern const u8 gUnknown_0848A44C[];
/* Blobs reached only as `sub_080152C0` / `sub_080152EC` / `sub_080193B0`
 * arguments in the 0x08005xxx menu block. sub_080152EC and sub_080193B0 take
 * `const void *` / `const u8 *`; sub_080152C0 takes `s32` and its callers cast,
 * as src/decomp/c_0802A508.c already does. */
extern const u8 gUnknown_08487E8C[];
extern const u8 gUnknown_08488594[];
extern const u8 gUnknown_08488614[];
extern const u8 gUnknown_0848863C[];
extern const u8 gUnknown_0848867C[];
extern const u8 gUnknown_084886BC[];
extern const u8 gUnknown_084886CC[];
/* Wave 37, W37-A: seven more of the same 0x08005xxx menu-block blobs, reached
 * only as arguments. gUnknown_08488164 / _224 / _2E4 / _394 are sub_080193B0's
 * four script stops in sub_0800520C, sub_080052D8 and sub_080053A8 (one triple
 * of near-identical functions, all four blobs shared between them);
 * gUnknown_08488494 and gUnknown_08488514 are sub_08019F2C's first argument in
 * sub_080057EC / sub_08005964; gUnknown_08488664 is sub_0801BD00's third
 * argument in BOTH sub_08005D74 and sub_08005E30, and that parameter is
 * declared `void *`, so those two call sites cast the const away. Nothing
 * dereferences any of them, so `const u8 []` is the weakest fit exactly as for
 * the block above. */
extern const u8 gUnknown_08488164[];
extern const u8 gUnknown_08488224[];
extern const u8 gUnknown_084882E4[];
extern const u8 gUnknown_08488394[];
extern const u8 gUnknown_08488494[];
extern const u8 gUnknown_08488514[];
extern const u8 gUnknown_08488664[];
/* An OBJ list of the gUnknown_08499E10 shape -- sub_08027C8C hands it to
 * PutSprite, whose fourth parameter is `u16 *`, so NOT const. */
extern u16 gUnknown_08499E08[];
/* sub_08027560's two ROM blobs: the first goes to `Decompress(u8 *, void *)`
 * and the second to `ApplyPaletteExt(u16 *, u32, u16)`. Both prototypes take
 * non-const pointers, which is what fixes the (missing) const here. */
extern u8 gUnknown_081121D0[];
extern u16 gUnknown_081126E4[];
/* Wave 30, W30-B extension work, 0x08049 block. A POINTER VARIABLE in ROM, the
 * same shape as gPlayers: every reader is `ldr rN, =sym; ldr rN, [rN]`
 * and then a displacement off the loaded word. The extent below is a FLOOR from
 * the three readers in that block (+0x836 and +0x837 in sub_08049B80, +0x83a in
 * sub_08049B28, +0x1e / +0x20 in sub_0804931C), not a measurement -- nothing
 * bounds the object. Only the two bytes wave 30 actually matched are named. */
/* Wave 43, W43-L. ONE ROM table of 8-byte entries beginning at 0x084C24A0,
 * declared TWICE because sub_08048FD8 reaches it through two pool words and
 * data-0848B688.s therefore carves two symbols out of it -- gUnknown_084C24A0
 * is 4 bytes and gUnknown_084C24A4 is the 0xB44 remainder. Each view is
 * offset-0 into its own symbol, so each relocates against exactly the symbol
 * the ROM names; a single struct array at ...A0 would relocate the second use
 * as `gUnknown_084C24A0 + 4` instead.
 *
 * The entry is { const u8 *script; u8 (*predicate)(void); }: sub_08048FD8
 * calls the second word through `_call_via_r0` with no argument and compares
 * the result against 1, then hands the FIRST word of the chosen entry to
 * sub_080485DC, whose parameter is `const u8 *` (src/decomp/c_080485DC.c).
 * The predicate's return width is NOT proved -- the `lsls #0x18; lsrs #0x18`
 * before `cmp #1` is a value comparison, so per the wave-43 W43-F rule it
 * pins only the narrower of (return type, receiving local). u8 is the
 * weakest spelling that fits. The entry COUNT is unbounded; the loop walks
 * indices 3..0x12. */
struct Unk084C24A0
{
    /* 0x00 */ const u8 *unk00;
    /* 0x04 */ u8 (*unk04)(void);
};
extern const struct Unk084C24A0 gUnknown_084C24A0[];

struct Unk084C24A4
{
    /* 0x00 */ u8 (*unk00)(void);
    /* 0x04 */ const u8 *unk04;
};
extern const struct Unk084C24A4 gUnknown_084C24A4[];

struct Unk084C30F8
{
    /* 0x000 */ u8 filler_000[0x1e];
    /* 0x01e */ u16 unk01e; /* Wave 30, W30-D. sub_0804931C reads both `ldrh`
                             * and uses unk01e twice: once as `unk01e - unk020`
                             * (the difference feeds sub_080487B4's u8 second
                             * parameter as `(x * 2 + 7)`) and once as the index
                             * into the u8 array gUnknown_02028E1C. u16 from the
                             * `ldrh`; nothing constrains the sign because the
                             * only arithmetic is the subtraction, which
                             * promotes either way. */
    /* 0x020 */ u16 unk020;
    /* 0x022 */ u8 filler_022[0x06];
    /* 0x028 */ s32 unk028; /* Wave 56, W56-K. RETYPED from `u16` (wave 36,
                             * W36-J read it as `ldrh [rN, #0x28]` in
                             * sub_080499F8 and inferred a halfword). It is a
                             * WORD: sub_08049360 loads it `ldr r1,[r3,#0x28]`
                             * and stores it back `str r0,[r3,#0x28]`, which a
                             * u16 member cannot produce, and sub_08049944's
                             * first parameter is `u16` so the earlier `ldrh`
                             * is just shorten_binary_op truncating the load at
                             * the use. SIGNED: sub_08049360 compares it
                             * `bge` against unk02c (both members, so the
                             * comparison's signedness is the members' own),
                             * while the `bhs` against gUnknown_0849EDB0[].unk04
                             * is what forces THAT field unsigned rather than
                             * this one. c_080499F8.c re-verified byte-exact
                             * after the retype. */
    /* 0x02c */ s32 unk02c; /* Wave 56, W56-K. Was inside filler_02a; the
                             * extent is unchanged and unk030 does not move.
                             * sub_08049360 sets it once as
                             * `unk028 - gUnknown_0849EDB0[i].unk04` (`str
                             * r1,[r3,#0x2c]`) and then uses it as the floor
                             * unk028 counts down to. Signed for the same
                             * `bge` as unk028. */
    /* 0x030 */ u8 unk030; /* Wave 35, W35-L. A small state/flag byte:
                            * sub_08048F4C gates its whole body on it being 0
                            * (`ldrb`; `cmp #0`; `bne`) and then writes the
                            * literal 2 or 3 into it with `strb`. Reached
                            * through `adds rN, rM, #0x30` -- past `ldrb`'s
                            * imm5 -- and byte-wide from the loads. */
    /* 0x031 */ u8 filler_031[0x01];
    /* 0x032 */ u16 unk032[0x400]; /* Wave 43, W43-L. A 32x32 halfword tilemap
                             * scratch buffer. sub_08048850 hands `&unk032[0]`
                             * to sub_08012BC8(u16 *, ...) as its `u16 *` first
                             * parameter and to sub_080487B4's `u16 *` third;
                             * sub_08049178 and sub_08049264 both read it
                             * `ldrh` at `row * 32 + col` with col bounded by
                             * `bls #0x13`, so the row stride is 32 halfwords.
                             * The EXTENT is inferred, not measured: 0x400
                             * halfwords is exactly the gap to unk832 and is
                             * the standard GBA 32x32 BG map, but nothing in
                             * the four readers bounds the row count. Reached
                             * through `adds rN, #0x32`, which is why the
                             * address decays before the index scale. */
    /* 0x832 */ s16 unk832; /* Wave 31, W31-A. SIGNED, and measured: sub_08048F10
                             * tests it `< 0` with `movs rI,#0; ldrsh rD,[rB,rI]`
                             * -- a plain u16 member cannot produce the ldrsh.
                             * Reached through a pool word (`adds rB, 0x832`),
                             * past ldrh's imm5*2 limit. sub_08048F10 is a
                             * saturating step: `if (x < 0) x += 8; else x = 0;`
                             * and it returns the new value, so 8 is the step of
                             * a countdown that rests at 0. */
    /* 0x834 */ u8 unk834; /* Wave 56, W56-K. Was filler_834; same extent. THE
                            * STATE BYTE: sub_08049360 switches on it over
                            * cases 0..11 through a jump table, `unk834++`s it
                            * at the end of most arms and writes 0 / 7 / 8 / 9
                            * / 0xa / 0xb into it at the rest. `ldrb`/`strb`
                            * through `adds rN, rM, #0x834`, past ldrb's
                            * imm5. */
    /* 0x835 */ u8 unk835; /* Wave 36, W36-J. A one-shot "repaint me" flag:
                            * sub_080499F8 passes it to sub_08049944 and then,
                            * if it is non-zero, clears it and calls
                            * sub_08013AEC. ldrb/strb through
                            * `adds rN, rM, #0x835`, past ldrb's imm5. */
    /* 0x836 */ u8 unk836; /* compared for equality against unk837 by
                            * sub_08049B80, which picks between sub_0803BD60
                            * and sub_0803BD54 on the result. Both are plain
                            * `ldrb` through `adds rN, rM, #0x836` / `#0x837`,
                            * past `ldrb`'s imm5, so the pair is adjacent and
                            * byte-wide and nothing else about them is proved. */
    /* 0x837 */ u8 unk837;
    /* 0x838 */ u8 unk838; /* Wave 43, W43-L. sub_08049178 does `unk838++`
                            * (ldrb; adds #1; strb) on the frame it kicks the
                            * list redraw off, right beside sub_080490BC's
                            * `unk839++`. Only the width is proved. */
    /* 0x839 */ u8 unk839; /* Wave 35, W35-L. sub_080490BC does `unk839++`
                            * (ldrb; adds #1; strb) once per accepted frame and
                            * nothing matched reads it back, so only the width
                            * is proved. */
    /* 0x83a */ u8 unk83a; /* Wave 30, W30-D. sub_08049B28 reads it `ldrb`
                            * through `adds rN, rM, #0x83a` (a pool word, past
                            * ldrb's imm5) and uses it as the index into the
                            * four-entry pointer table gUnknown_084C30E8. u8
                            * from the load; the table has four ROM entries, so
                            * the value is small. */
    /* 0x83b */ u8 unk83b; /* Wave 56, W56-K. Was filler_83b; same extent. A
                            * frame countdown: sub_08049360 sets it to 0 in one
                            * state and 0x2d in another, and its case 6 does
                            * `if (unk83b != 0) { unk83b--; break; }` before any
                            * other work. ldrb/strb through
                            * `adds rN, rM, #0x83b`. */
    /* 0x83c */ u8 unk83c; /* Wave 35, W35-L. sub_080490BC increments it when
                            * gpKeySt->last carries 0x2 (right after calling
                            * sub_080485F8) and tests it `!= 0` in the same
                            * function, paired with unk836, to decide whether to
                            * Proc_Break. ldrb/strb through
                            * `adds rN, rM, #0x83c`. */
    /* 0x83d */ u8 unk83d[0x13]; /* Wave 43, W43-L. A compaction buffer:
                            * sub_08048FD8 walks the 16 predicates at
                            * gUnknown_084C24A4 and `strb`s each PASSING index
                            * into unk83d[n++], then picks one at random
                            * (`__umodsi3(gGameClock, n)`) and stores
                            * `entry + 3` into unk839. So it holds indices, and
                            * n is bounded by 16 -- the 0x13 extent is only the
                            * gap to unk850 and is not measured. */
    /* 0x850 */ const u8 *unk850; /* Wave 31, W31-A. sub_080485DC stores its own
                                   * parameter here (`movs r2,#0x85; lsls #4`
                                   * for the 0x850 displacement) and then hands
                                   * the SAME pointer to sub_080193B0, whose
                                   * parameter is `const u8 *` -- so the member
                                   * takes the callee's type. Word-wide from the
                                   * `str`. */
};
extern struct Unk084C30F8 *gUnknown_084C30F8;
/* Wave 30, W30-D. A four-entry table of script-blob pointers (the ROM words at
 * 0x084C30E8 are 084C3068 / 084C3088 / 084C30A8 / 084C30C8, evenly spaced by
 * 0x20; the fifth word is a RAM address and belongs to something else).
 * sub_08049B28 indexes it with `gUnknown_084C30F8->unk83a` and hands the
 * element straight to sub_0801930C, whose parameter is `const u8 *`, so the
 * element type is `const u8 *` and the table itself is const. */
extern const u8 *const gUnknown_084C30E8[];
/* Wave 56, W56-K. Four more script blobs in the SAME 0x20-spaced run as the
 * four gUnknown_084C30E8 points at (084C3068 / 3088 / 30A8 / 30C8) -- these
 * are the four immediately before it. sub_08049360 hands each to
 * sub_080485DC's `const u8 *`, and it names all four as BARE symbols
 * (`ldr r0,=sym`), with no scaled index anywhere in the function. So the
 * 0x20 spacing is the blob SIZE and not an array stride: there is no
 * `lsls #5` and nothing subscripts across them, which is what would be needed
 * to call this one object. Declared separately for that reason. */
extern const u8 gUnknown_084C2FE8[];
extern const u8 gUnknown_084C3008[];
extern const u8 gUnknown_084C3028[];
extern const u8 gUnknown_084C3048[];
/* Wave 56, W56-K. A list script blob: sub_08049360's last state hands it to
 * sub_080152C0 with the `(s32)` cast at the call site, the same spelling
 * gUnknown_08488614 uses. */
extern const u8 gUnknown_084C3118[];
/* Wave 56, W56-K. NOT a -fforce-addr pool slot, despite sitting between two
 * that are (0x0812A164 holds &gUnknown_084C30F8, 0x0812A168 holds &gpKeySt).
 * The ROM word here is 0x00590005 -- two u16s, 0x0005 and 0x0059 -- and
 * sub_08049360 hands the ADDRESS straight to sub_0808B6E8 as a copy source
 * (`ldr r1,=gUnknown_0812A160`, one load, no dereference) to fill a 4-byte
 * stack pair it then indexes by a flag. This is W56-G's rule: read how the
 * word is USED, because a pool slot is always `ldr rN,=w; ldr rN,[rN]`.
 * Spelling it as a local `u16 v[2] = {5, 0x59};` would be right on every
 * axis except that agbcc has no THUMB movstrsi and emits a call relocating
 * against `memcpy`, which does not exist in this tree. The symbol is already
 * emitted by data/data.s, so nothing needs carving. */
extern const u8 gUnknown_0812A160[];
/* A list script blob stopped FOUR TIMES IN A ROW by sub_08049EB4, after one
 * stop of gUnknown_084C325C. `const u8 []` for the reason gUnknown_0849A108 is:
 * sub_0801537C takes `const void *` and nothing dereferences it. The repeat is
 * in the ROM, not a transcription slip -- the address is held in r4 across all
 * four calls, which is what pays for the `push {r4, lr}`. */
extern const u8 gUnknown_084C3244[];

/* Wave 31, W31-A. The proc script sub_08001038 starts with
 * `sub_080152EC(gUnknown_084858DC, 0)` before writing that entry's unk1e --
 * the same shape as the gUnknown_08485D8C call recorded on Unk03001470.unk42.
 * `const struct ProcCmd []` follows sub_080152EC's first parameter and the
 * house convention for every other blob it is handed. */
extern const struct ProcCmd gUnknown_084858DC[];

/* Wave 31, W31-A. sub_08010EC4's memcpy DESTINATION: `sub_0808B6E8(this,
 * gUnknown_03003050, 0x3d)`, and sub_0808B6E8's first parameter is the
 * destination (sub_0802FA9C copies a ROM table into a stack buffer through it
 * and then reads the buffer). 0x3d bytes, no interior structure visible from
 * here, hence `u8 []`. NOT const: it is written, even though the address lies
 * in the cartridge window, so the store is inert on hardware -- flagged rather
 * than explained, since nothing in the tree reads it back. */
extern u8 gUnknown_08489200[];

/* Wave 31, W31-A. 0x03007FFC is the BIOS interrupt-vector word; src/crt0.s
 * writes it too. sub_08010EC4 is a one-line setter that `str`s its parameter
 * here and nothing narrows or dereferences it, so `void *` is the weakest type
 * that fits. The real content is a handler address -- flagged, not proved:
 * nothing in the C tree calls through it. */
extern void *gUnknown_03007FFC;

/* Wave 31, W31-A. An 8-byte ROM table sub_0802FA9C copies WHOLE onto an
 * 8-byte stack buffer with sub_0808B6E8 and then indexes as `ldrsh` at
 * `index * 2` -- so four SIGNED halfwords. The copy-then-index is the ROM's
 * own idiom, not an artefact: the buffer is what the `sub sp, #8` pays for. */
extern const s16 gUnknown_08090CA0[];

/* Wave 31, W31-A. 0x03007FF8 is the BIOS IntrCheck halfword. sub_0802E920
 * stores 1 into it with a bare `strh` before draining the DMA queue. u16 from
 * the store width; VOLATILE IS UNPROVEN -- a single unconditional store is
 * byte-identical either way, and no reader is known in the C tree. */
extern u16 gUnknown_03007FF8;

/* Wave 31, W31-A. A matched pair of 0x80-byte blobs that sub_08022878 pushes to
 * the same VRAM address 0x06003600, picking between them on bit 0 of
 * gGameClock -- a two-frame animation. Non-const because sub_080228B8
 * hands 0x0809181C to sub_08011E54, whose first parameter is a plain `void *`;
 * 0x08091B9C gets the same type so the pair stays one kind of object. */
extern u8 gUnknown_0809181C[];
/* Wave 32 (W32-B): two 15-entry u16 palette-cycle tables sub_080228D8 picks
 * between, indexed by `(gGameClock >> 2) % 15` (a LOGICAL shift and
 * __umodsi3, so the frame counter is read unsigned there) and handed to
 * sub_08013664, whose first parameter is `u16 *`. */
extern u16 gUnknown_08091C5E[];
extern u16 gUnknown_08091C9E[];
extern u8 gUnknown_08091B9C[];

/* Wave 31, W31-A. Two LZ blobs sub_08022A34 unpacks to 0x06016CA0 and
 * 0x06016A40. `u8 []` and non-const because Decompress takes `u8 *`. */
extern u8 gUnknown_081019C4[];
extern u8 gUnknown_08124268[];

/* Wave 31, W31-A. A 0x20-byte palette sub_08022A34 hands to ApplyPaletteExt at
 * offset 0x220; `u16 []` and non-const for the same reason gUnknown_08582754
 * is -- ApplyPaletteExt takes a plain `u16 *`. */
extern u16 gUnknown_08101904[];

/* Wave 31, W31-A. A 16-entry halfword colour table, and then a SECOND one
 * 0x20 bytes above it: sub_08022A6C reads
 * gUnknown_08101984[(gGameClock >> 2) & 0xF] into gPal halfword 0x228
 * and gUnknown_08101984[0x10 + same] into 0x238. The same 16-phase frame
 * counter idiom as gUnknown_081A47E4, one octave slower. At least 32 entries.
 * `u16 []` non-const because sub_0801368C takes a plain `u16 *`. */
extern u16 gUnknown_08101984[];

/* Wave 31, W31-A. A list script blob sub_08022A08 hands to sub_0801537C, which
 * takes `const void *` and never dereferences it -- the same reading as
 * gUnknown_0849A108 and gUnknown_084C3244. */
extern const u8 gUnknown_08499B4C[];

/* Wave 31, W31-A. A proc script blob sub_08007B54 starts with
 * `sub_080152C0((s32)this, 0)`, keeping the s8 slot id it returns in
 * gActiveMap->spriteId. `const u8 []` plus the cast at the call site is the
 * spelling src/decomp/c_08005838.c and c_080059B4.c already use for
 * gUnknown_08488614 and gUnknown_0848863C, the neighbouring blobs. */
extern const u8 gUnknown_08488890[];

/* Wave 31, W31-A. A GLOBAL FUNCTION POINTER, called through
 * `bl _call_via_r4` -- the libgcc trampoline agbcc uses for every indirect
 * THUMB call, and r4 because r0-r3 are all taken by arguments. Five parameters:
 * sub_0802026C forwards its own last five, and sub_0802032C calls it as
 * `(unit->unk02, unit->unk03, unit->unk00, 0x78, 0)`, which is a cell column, a
 * cell row and a unit-type id. The widths are NOT settled -- every argument is
 * either an untouched register or a `ldrb` that a wider parameter accepts for
 * free -- so `int` is the weakest that fits. Void because both callers discard
 * the result. */
extern void (*gUnknown_030013EC)(int, int, int, int, int);

/* Wave 31, W31-A. Proc scripts. gUnknown_08615E08 is started as a child of
 * sub_0807A36C's own proc when A is held; gUnknown_08616A08 likewise in
 * sub_08081334, guarded by a Proc_Find on gUnknown_08616A40 so the two are a
 * "start unless that other tree is already up" pair. */
extern const struct ProcCmd gUnknown_08615E08[];
extern const struct ProcCmd gUnknown_08616A08[];
extern const struct ProcCmd gUnknown_08616A40[];

/* Wave 31, W31-A. A proc script blob sub_08029948 starts with
 * `sub_080152C0((s32)this, 0)`, then writes the caller's argument into
 * gUnknown_03001470[slot].unk22 of the entry it hands back. `const u8 []` plus
 * the cast at the call site, the same spelling gUnknown_08488614 uses. */
extern const u8 gUnknown_0849A080[];

/* Wave 31, W31-A. The link-session descriptor sub_08030C98 fills in and hands
 * to sub_0802EA5C. Every field below is read off sub_0802EA5C's own body, which
 * takes this object in r0: `ldr [r2]` at +0 into gUnknown_03000564, `ldrh
 * [r2,#6]` tested against 0 and defaulted to 10, `ldrh [r2,#8]` masked with 3
 * into gUnknown_03000560. sub_08030C98 adds +6 and +0xa, both `strh` of 0x66 --
 * so +6 is a timeout in frames and +0xa is its twin. */
struct Unk030040C0 /* >= 0x0c */
{
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u8 filler_04[0x02];
    /* 0x06 */ u16 unk06;
    /* 0x08 */ u16 unk08;
    /* 0x0a */ u16 unk0a;
};
extern struct Unk030040C0 gUnknown_030040C0;
/* Wave 33, W33-B: the 12-byte ROM initialiser sub_0802F348 copies wholesale
 * into gUnknown_030040C0 (`ldm!/stm!` of three words = one struct assignment).
 * Same type, const because it is ROM. */
extern const struct Unk030040C0 gUnknown_08090C38;

/* Wave 31, W31-A. Two adjacent bytes read as a pair by sub_080468D4: [0] is
 * multiplied by 8 and added to the caller's pixel offset before a signed
 * divide by 8, and [1] goes straight into sub_08012BC8's third u16 parameter.
 * A scroll origin in tiles and a palette or window id. Plain `ldrb`, so u8;
 * nothing settles the signedness. */
extern u8 gUnknown_084C211C[];

/* Wave 49, W49-J. Six more plain `ldrb` byte tables in the same 0x084C2xxx
 * block, all read by the sub_08046778/sub_08046A84 pair that draws the seven
 * CO/army slots. Sizes are not proved -- every user indexes them with a runtime
 * loop counter -- so they stay incomplete arrays.
 *   gUnknown_084C20C0[i], i < 3  -- a unit-type id: both users multiply it by
 *     0x5c and index gUnknown_085D5ABC[] with it.
 *   gUnknown_084C20C3[i], i <= 6 -- goes straight into sub_0801F34C's first
 *     (x) argument.
 *   gUnknown_084C2112[]  -- read as (x, y) PAIRS, `[i*2]` and `[i*2+1]`, both
 *     into sub_0801F34C coordinates; the bound is
 *     gUnknown_085D583C[unit].unk10.
 *   gUnknown_084C212A[i], i <= 6 -- a slot index scaled by 32 into a
 *     struct CoModeData.unk18 movement-cost row.
 *   gUnknown_084C2131[] -- the same (x, y) pair layout as gUnknown_084C2112,
 *     indexed by a separate counter that only advances on drawn slots.
 * Nothing settles the signedness of any of them; `ldrb` alone is byte-neutral
 * between u8 and a value that is never compared. */
extern u8 gUnknown_084C20C0[];
extern u8 gUnknown_084C20C3[];
extern u8 gUnknown_084C2112[];
extern u8 gUnknown_084C212A[];
extern u8 gUnknown_084C2131[];

/* Wave 49, W49-J. An OAM template blob whose ADDRESS is passed, unmodified, as
 * sub_0801BD00's `void *` third argument by sub_08046A84 -- the same slot every
 * other caller fills from a gUnknown_08485Cxx pointer table. Only the address
 * is proved, so the element type is the weakest one that can spell it. */
extern u8 gUnknown_084C20A0[];

/* Wave 31, W31-A. A vertical scroll offset in pixels: sub_080771C0 pushes it
 * straight into REG_BG0VOFS and REG_BG2VOFS, and sub_0807774C and sub_08077C70
 * both pass `0xA8 - it` to sub_08077620. Word-wide (`ldr`); the `strh` into the
 * registers is the hardware's own truncation, not the variable's width, and
 * nothing settles the signedness. */
extern int gUnknown_0300064C;

/* Wave 31, W31-A. A 0x460-byte blob sub_08002E3C pushes to VRAM 0x06014D40
 * through sub_08011E54, whose first parameter is a plain `void *`. */
extern u8 gUnknown_0808D8AC[];

/* Wave 31, W31-A. An LZ blob sub_08086E2C unpacks to the tile block
 * gUnknown_03001FE8's chr_block selects. `u8 []` non-const because Decompress
 * takes `u8 *`. */
extern u8 gUnknown_0823FD7C[];

/* Wave 31, W31-A. The twin of gUnknown_030058F4 and written in the same breath
 * by sub_08086DF4: the two bytes at +0 and +1 of gUnknown_03003F68 are widened
 * into this pair with `ldrb` then `str`, so both are word-wide holders of a
 * byte value. */
extern u32 gUnknown_03005918;

/* ---- wave 33 (W33-C): the 0x08063 address-locality block ---- */

/* 0x08614258, 0x10 bytes -- FOUR word-sized VRAM base pointers, selected by the
 * u8 gUnknown_02028E40. sub_0806370C does `ldr` through the indexed slot and
 * hands the result (plus a 0x80-byte row offset) to sub_08011E54 as its source,
 * which is the only use in the ROM. `u8 *[]` rather than `void *[]` so the
 * `+ n * 0x80` arithmetic is defined, and not const for the same reason
 * gUnknown_080A5524 is not: sub_08011E54's first parameter is a plain `void *`
 * and passing a `const` would warn under -Werror. */
extern u8 *gUnknown_08614258[];

/* 0x03000604. The SECOND copy of the gUnknown_030005FC latch above, one slot
 * along and identical in shape: sub_08064500 compares it against
 * `gGameClock - 1` and then assigns from it, and the s16 coordinate pair
 * that goes with it sits at 0x03000608/0x0300060A. Those two ARE INTERIOR
 * ADDRESSES and must not be declared, for exactly the reason the
 * gUnknown_030005FC note gives: aw2bhr.lds names gUnknown_03000604 and then
 * gUnknown_0300060C, so both fall inside this symbol's 8-byte region. Spell
 * them `*(s16 *)((u8 *)&gUnknown_03000604 + 4)` and `+ 6` -- that reproduces the
 * ROM's -fforce-addr words at 0x0816E0B8 and 0x0816E0BC, which hold 0x03000608
 * and 0x0300060A. See src/decomp/c_08064500.c. */
extern s32 gUnknown_03000604;

/* 0x0816E0B0..0x0816E0BC -- FOUR ROM WORDS HOLDING RAM POINTERS, and this is
 * the spelling that finally makes sub_08064474 both match and link. The ROM
 * words are 0x03000600, 0x03000602, 0x03000608 and 0x0300060A: the two s16
 * coordinate pairs that belong to the gUnknown_030005FC and gUnknown_03000604
 * latches. Those four addresses are INTERIOR to their latch symbols and cannot
 * be declared (see both notes above), and wave 32 recorded the resulting
 * deadlock -- naming them matched but failed the link, and reaching them as
 * `*(s16 *)((u8 *)&gUnknown_030005FC + 4)` links but does NOT match: agbcc CSEs
 * the one base and folds +4/+6 into `ldrsh rN,[base,rM]` displacements, which
 * is 96 bytes against the ROM's 140.
 *
 * DEREFERENCING THESE FOUR DOES NOT CLOSE IT EITHER, and the reason is the
 * useful part: `*gUnknown_0816E0B0` costs ONE EXTRA `ldr` per access (148
 * bytes, 40.7%), because agbcc force-addrs `&gUnknown_0816E0B0` into a .rodata
 * slot of its own and the read becomes three levels where the ROM has two.
 * These four words ARE agbcc's own -fforce-addr slots, holding
 * &gUnknown_03000600 &c -- the ROM loads them with a bare `ldr r6,=sym` and no
 * accompanying move, which is the signature of a compiler address constant
 * rather than a user local. They are declared here only so a draft can name
 * them; the real fix is two lines in aw2bhr.lds. The full measurement of all
 * four ruled-out spellings is in work/sub_08064474/sub_08064474.c.
 *
 * `s16 *const`: the pointee is written (`strh`), the pointer is not, and it
 * lives in the cartridge window. */
extern s16 *const gUnknown_0816E0B0;
extern s16 *const gUnknown_0816E0B4;
extern s16 *const gUnknown_0816E0B8;
extern s16 *const gUnknown_0816E0BC;

/* 0x08234AF0, 0x20 bytes -- the exact twin of gUnknown_08239F84 below, and read
 * by the same idiom: sub_0807C994 indexes it with `DivRem(Div(proc->unk48, 4),
 * 0x10) * 2` and hands the element's address to ApplyPaletteExt, so sixteen
 * halfwords is proved by the `% 0x10`, not guessed. The ROM bytes are a
 * symmetric brightness ramp (7FFF, 7BDF, 77BF, 6F7F ... 525F ... 7BDF) that
 * walks down and back up over the sixteen steps, which is what a 16-step
 * palette cycle looks like. Non-const for the same -Werror reason as
 * gUnknown_08239F84: ApplyPaletteExt's first parameter is a plain `u16 *`. */
extern u16 gUnknown_08234AF0[16];

/* 0x08239F84, 0x20 bytes -- sixteen halfwords, and the extent is proved rather
 * than guessed: sub_08064500 indexes it with `DivRem(Div(gGameClock, 4),
 * 0x10) * 2` and hands the element's address to ApplyPaletteExt, whose first
 * parameter is a plain `u16 *` (hence not const, the same -Werror reason as
 * gUnknown_0809181C). A 16-step palette cycle driven off the frame counter. */
extern u16 gUnknown_08239F84[16];

/* 0x0823DE18, 0x20 bytes -- the third member of the pair above, read by the
 * same idiom and with the same evidence for the extent: sub_08084700 indexes it
 * with `DivRem(Div(proc->unk4a, 2), 0x10)` and hands the element's address to
 * ApplyPaletteExt, so the `% 0x10` proves sixteen halfwords. data/data.s
 * already carves it at exactly 0x20 bytes. Non-const for the same -Werror
 * reason as its two twins. */
extern u16 gUnknown_0823DE18[16];

/* 0x03005950, five bytes. sub_0807C9EC clears it with a descending loop that
 * strength-reduces to a pointer walking from `&g[4]` down to `&g[0]` and a
 * signed `cmp; bge` against the base, so the extent is read off the loop rather
 * than guessed. `u8` from the `strb`; nothing matched reads it back yet, so the
 * signedness is unsettled. The address is already bound by aw2bhr.lds under
 * this exact name. */
extern u8 gUnknown_03005950[5];

/* 0x085802D8, 0x18 bytes -- TWO twelve-byte NUL-padded name strings,
 * "SELECT*MODE" at +0 and "SELECT*CO" at +0xC, carved as one blob by
 * data/data-0848B688.s (its neighbour gUnknown_085802CC holds "SELECT*MAP" in
 * the same 12-byte slot shape). sub_0807C9EC passes the SECOND one to
 * sub_08073304 as its `const void *`, which is why the reference there is
 * spelled `gUnknown_085802D8 + 0xC`: the ROM's -fforce-addr pool word at
 * 0x081D9340 holds 0x085802E4, and the split has no label at that address, so
 * an offset off the enclosing symbol is the only spelling that links. Anyone
 * who splits the blob should give +0xC its own symbol and simplify that call.
 * `u8 []` on the model of gUnknown_085802F0 above. */
extern u8 gUnknown_085802D8[];

/* ---- wave 33 (W33-C): the 0x08050 address-locality block ---- */

/* 0x02029C0C / 0x02029C10 -- a per-side halfword pair (`lsls #1` on the
 * gUnknown_03001470 slot's unk30) that sub_080500D0 subtracts a ROM-supplied
 * origin from to make sub_080155C0's screen coordinates. u16 from the `ldrh`;
 * the SUBTRACTION is what carries the sign, and it is re-narrowed
 * `lsls #0x10; asrs #0x10` into sub_080155C0's s16 parameters, so nothing here
 * needs a signed member. */
extern u16 gUnknown_02029C0C[];
extern u16 gUnknown_02029C10[];
/* 0x085644A0 -- a u16 lookup keyed by gUnknown_02029808[i].unk30[unk2e], with
 * 0xFF meaning "no entry": sub_080504A8 tests it against 0xFF and falls back to
 * its own argument. `ldrh` at the test and `ldrsh` at the use, which is combine
 * folding sign_extend(truncate(zero_extend(mem:HI))) into sub_0803B48C's s16
 * parameter and NOT evidence of a signed object -- see the note on
 * struct Unk02029A10's `x` for the same readout. */
extern u16 gUnknown_085644A0[];

/* ---- wave 33 (W33-F): address-locality blocks 0x08002 and 0x08057 ---- */

/* The gUnknown_08485D44 sprite-pointer table one and two tables along.
 * sub_08002964 and sub_080029F4 index them by their first argument and hand the
 * word straight to sub_0801BD00's `void *` third parameter with no arithmetic
 * in between, which is exactly how the matched sub_08002844 uses
 * gUnknown_08485D44 -- same model, same reason. */
extern void *const gUnknown_08485CF4[];
extern void *const gUnknown_08485D68[];
/* A word table of OBJ-graphics pointers selected by the u8 gUnknown_02028E40
 * (`ldrb` then `lsls #2`), read by sub_080029F4 on its 0x19 path and handed to
 * sub_08011E54's `void *` source parameter. */
extern void *const gUnknown_08489190[];

/* Three more of the gUnknown_0808D8AC group: sub_08002E5C pushes them to VRAM
 * 0x06016180 (0x200 bytes), 0x06016140 and 0x06016160 (0x20 each) through
 * sub_08011E54, whose first parameter is a plain `void *`. */
extern u8 gUnknown_0808DD0C[];
extern u8 gUnknown_0808DF0C[];
extern u8 gUnknown_0808DF2C[];

/* ONE 0x1c-stride ROM table seen through THREE overlapping linker symbols
 * (0x08553A1C, +8, +0xc), the way gUnknown_03004580 and gUnknown_03004582
 * overlap: sub_080576D4, sub_08057A24 and sub_080577E4 each hold a pool word
 * for their own view and then index it by the raw row number, so the column
 * offset is folded into the pool word and never appears as an `adds`.
 *   Each view is a signed tile x/y pair added to the caller's own x/y before
 * the `<< 5` row stride. s8 is measured, not assumed: unk00 is read with the
 * register-offset `ldrsb` (the only `ldrsb` addressing mode THUMB has) and
 * unk01 as `ldrb [r,#1]` + `lsls #0x18; asrs #0x18`, which is what a signed
 * byte at a non-zero constant displacement costs. The 0x1a filler is the
 * measured stride, not a proved layout. */
struct Unk8553A1C /* 0x1c */
{
    /* 0x00 */ s8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 filler_02[0x1a];
};
extern struct Unk8553A1C gUnknown_08553A1C[];
extern struct Unk8553A1C gUnknown_08553A24[];
extern struct Unk8553A1C gUnknown_08553A28[];
/* Wave 48, W48-G. A FOURTH view of the same 0x1c-stride table, at +4:
 * sub_0805772C holds its own pool word for 0x08553A20 and reads unk00/unk01
 * off it with exactly the ldrsb / ldrb+shift pair described above. */
extern struct Unk8553A1C gUnknown_08553A20[];
/* Wave 49, W49-D. A FIFTH view, at +0x10. sub_08057860 reads TWO s8 pairs off
 * it, at +0 and +4, and 0x08553A30 is deliberately NOT declared beside it: the
 * ROM carries a single pool word for 0x08553A2C and derives the +4 base with
 * `adds r0,#4` (cse.c's use_related_value, which only relates constants built
 * from the same symbol). A second linker symbol emits a second pool word and
 * costs 4 bytes -- measured. See work/sub_08057860/sub_08057860.c for the
 * spelling that reproduces it and for the two others that do not. */
extern struct Unk8553A1C gUnknown_08553A2C[];
/* Wave 49, W49-D. A view of the same table FOUR BYTES BELOW gUnknown_08553A1C,
 * and the only one whose columns are halfwords: sub_08057BDC reads `ldrh [p]`
 * and `ldrh [p,#2]` off gUnknown_08553A18[idx] to build a `y * 32 + x` VRAM
 * offset, and hands the SAME element to sub_0805772C and sub_080577E4, whose
 * third parameter is `struct Unk8057Pos *` -- so this pair is that type's x/y.
 * It gets its own tag because struct Unk8057Pos is spelled per-file in
 * src/decomp/ (c_080576D4.c, c_080577E4.c, c_08057A24.c) rather than shared. */
struct Unk8553A18 /* 0x1c */
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u8 filler_04[0x18];
};
extern struct Unk8553A18 gUnknown_08553A18[];
/* Wave 49, W49-D. A signed byte per gUnknown_03004508 value, guarded by
 * `cmp #8; bhi` at sub_08057BDC's head, so at least nine entries. `ldrsb` with
 * the register-offset form -- the s8-OBJECT tell -- and the value is both
 * subtracted from a tile column and passed as sub_08071900's third (width)
 * argument. */
extern s8 gUnknown_08553B04[];
/* Wave 49, W49-D. Three parallel u16 tables sub_08057164 reads with ONE index,
 * `side * 10 + row * 5 + slot`, into gUnknown_02029A10[row].entries[slot].unk1a:
 * 0x0855203C when the gUnknown_085D6A48 row's column 2 is 1, otherwise
 * 0x08551F60 or 0x08551E84 on that row's column 8. Declared flat because the
 * ROM forms the whole element index and scales it by 2 once at the end, rather
 * than summing three separately-scaled byte offsets. */
extern u16 gUnknown_08551E84[];
extern u16 gUnknown_08551F60[];
extern u16 gUnknown_0855203C[];
/* Wave 49, W49-D. The byte counterpart, indexed `row * 55 + side * 5 + slot`
 * (the 55 is `((y << 3) - y << 3) - y`) and stored into the same entry's unk00
 * and unk01 -- the two sides' values come from ONE table read twice with
 * different `side`. Flat for the same reason as the halfword trio above; the
 * 55 factors as 11 * 5, so the natural shape is [.][11][5]. */
extern u8 gUnknown_085521DC[];

/* Two word tables of ROM blobs, each entry going straight to sub_080718F8's
 * `u8 *` second parameter -- so `u8 *` and not const, for the same reason the
 * 0817D874 group above is not. 0x08553AC0 is indexed by sub_080576D4's raw row
 * number; 0x08553AD8 by `(gUnknown_02029B78[i & 1] + 9) / 10` in sub_080577E4,
 * a round-up divide by ten. */
extern u8 *gUnknown_08553AC0[];
extern u8 *gUnknown_08553AD8[];
/* A halfword pair read as `[i & 1]` by both sub_080576D4 and sub_080577E4, each
 * of which uses it as `(u16)(x * 0x1000)` -- `lsls #0x1c; lsrs #0x10`, the
 * palette nibble of the tile reference handed to sub_080718F8. Read as a
 * MULTIPLY, not a mask plus a shift; the 4-bit field is the u16 truncation of
 * the product. */
extern u16 gUnknown_08562124[];
/* Wave 48, W48-G. A ROM table of SEVEN-byte rows of tile numbers -- the seven
 * tiles of one digit strip -- read a byte at a time with a register-offset
 * `ldrb`. sub_0805772C picks the row `gUnknown_02029B78[i & 1] >> 1` (the `* 7`
 * is `lsls #3; subs`), except when that halfword is exactly 1, where it holds a
 * SECOND pool word for 0x085538B9 -- which is row 1 of the same table, the
 * overlapping-linker-symbol pattern again. The row count is unproved. */
extern u8 gUnknown_085538B2[][7];
extern u8 gUnknown_085538B9[];
/* The RAM counterpart, also indexed `[i & 1]` and also halfwords: sub_080577E4
 * divides `gUnknown_02029B78[i & 1] + 9` by ten to pick a gUnknown_08553AD8
 * row. Signed division (`__divsi3`), so the sum is an `int`. */
extern u16 gUnknown_02029B78[];
/* Wave 48, W48-G. The TARGET counterpart of gUnknown_02029B78, two elements
 * further on (0x02029B78 + 4) and reached through its own linker symbol and its
 * own -fforce-addr constant at 0x081361A8: sub_08057AE8 subtracts it from
 * gUnknown_02029B78 element-wise to get the s16 delta it turns into a per-frame
 * step. `ldrh`, so u16; the difference is narrowed to s16 by the reader. */
extern u16 gUnknown_02029B7C[];
/* Two more sub_080718F8 ROM blobs, selected by bit 0 of sub_08057A24's second
 * argument. `u8 []` and not const, as with the rest of that group. */
extern u8 gUnknown_0816D900[];
extern u8 gUnknown_0816D91C[];
/* Wave 51, W51-K. 0x0816D96C -- REAL DATA, not a -fforce-addr word: the symbol
 * goes straight to sub_0808B6E8 as the source of a 10-byte copy with no load in
 * front of it (sub_08059E00), the same argument that settled gUnknown_08090CF4
 * and gUnknown_08090D18. baserom.gba holds f4 01 e8 03 d0 07 a0 0f ff ff at
 * that offset -- 500, 1000, 2000, 4000, 0xFFFF -- so five ASCENDING u16
 * thresholds ending in a sentinel, which is also what the consumer does with
 * them: sub_08059E00 copies all ten bytes to a stack buffer and walks it
 * `while (v > buf[i]) i++`, returning i + 1 as a 1..5 band index. Unsigned
 * because that walk compares with `bls`. */
extern const u16 gUnknown_0816D96C[];
/* Wave 56, W56-M. 0x0816D9AC -- REAL DATA, the same class as gUnknown_0816D96C
 * twelve lines up and settled the same way: sub_0805A9AC loads the symbol
 * straight into r1 with no `ldr` in front of it and hands it to sub_0808B6E8 as
 * the source of an 8-byte copy onto its own stack. baserom.gba holds
 * 00 01 00 00 00 01 01 01 there, which the consumer then indexes
 * `t[a1][(unk09 >> 3) & 7]` off a `u8 [2][4]` stack buffer -- a1 being that
 * function's 0/1 selector -- so two rows of four flags.
 *   THE COPY IS IN THE SOURCE, NOT COMPILER-GENERATED, and that is what rules
 * out reading it as an aggregate initialiser's .rodata template: agbcc has no
 * THUMB movstrsi and expands an auto aggregate's initialiser through `memcpy`,
 * whereas this ROM relocates against sub_0808B6E8, the game's own copier. The
 * relocation target is the discriminator between the two readings whenever a
 * stack-copy template is in question.
 *   `const` because sub_0808B6E8's source parameter is `const void *`; the
 * first word reads 0x00000100, which is why a scalar-literal reading looks
 * plausible until the copy LENGTH is read. */
extern const u8 gUnknown_0816D9AC[];
/* Wave 48, W48-G. Two more of the same group, both used as BASE ADDRESSES with
 * no load in front of the pool word (checked against baserom.gba: the words at
 * these addresses are 0x00240010 and 0x7FFF6AF7, i.e. payload, not pointers --
 * unlike the 0x081361A0 run). gUnknown_0816CABC is sub_08057AE8's Decompress
 * source into VRAM 0x06004000, so `u8 *`-compatible and non-const like the rest
 * of the group. gUnknown_0816D498 is a palette bank: the same function
 * CpuFastSets 8 words from `gUnknown_0816D498 + gUnknown_03004500[k] * 32` into
 * PLTT 0x05000140 and 0x05000120, which fixes the 0x20-byte stride. */
extern u8 gUnknown_0816CABC[];
extern u8 gUnknown_0816D498[];
/* Halfword tables sub_08057048 reads into the six-halfword record it hands to
 * sub_080570C4: 0x0855388C by its own (u16) first argument, 0x085538A2 by
 * `gUnknown_085D6A48[gUnknown_03004580[i][1]][1] * 2 + i`. */
extern u16 gUnknown_0855388C[];
extern u16 gUnknown_085538A2[];
/* `strh 1` at the tail of sub_08057048, immediately after the sub_080570C4
 * call -- a request flag in the same RAM run as gUnknown_03004538. Halfword. */
extern u16 gUnknown_03004534;

/* Wave 34, W34-K -- block 0x08069 / 0x0806B.
 *
 * gUnknown_08580E60 is a ROM-resident POINTER, not a buffer: sub_080697CC and
 * sub_08069FD0 both hold its ADDRESS in r4 across the whole body and reload
 * `ldr r1, [r4]` at each use, exactly as the already-declared gUnknown_08499580
 * and gUnknown_0849957C are used. The pointee is a VRAM-bound work buffer -- it
 * is CpuFastSet's 0x400-word fill destination, a Decompress destination, and
 * then sub_08011E54's source for 0x1000 bytes to 0x0600F000. `void *` rather
 * than `u16 *` because nothing in either function indexes through it; if a
 * later caller does, narrow it then. Not const: it is a destination. */
extern void *gUnknown_08580E60;
/* Six compressed blobs reached only by address (`ldr r0, =sym`, never a load
 * through them) and handed to Decompress's source parameter by sub_080697CC
 * and sub_08069FD0, which are near-twins differing only in the first
 * destination (0x06000000 vs 0x06008000) and one extra blob. Not spelled
 * const because Decompress's first parameter is a plain pointer. */
extern u8 gUnknown_08183A00[];
extern u8 gUnknown_08184FF4[];
extern u8 gUnknown_08185F0C[];
extern u8 gUnknown_0818616C[];
extern u8 gUnknown_0818633C[];
extern u8 gUnknown_08186460[];
/* Wave 50 (W50-C). Two more compressed blobs on the same model, reached only by
 * address in sub_08068E60 and handed to Decompress's source parameter -- 081837A0
 * to 0x06008000 and 081838EC to whatever gUnknown_08499580 points at. */
extern u8 gUnknown_081837A0[];
/* Wave 51, W51-H. Two more Decompress blobs from the same ROM run as
 * gUnknown_081837A0, both handed to Decompress by sub_080694EC as the bare
 * symbol -- CA8 to VRAM at 0x06004800 and A74 to whatever gUnknown_08499578
 * points at. Non-const because Decompress takes a plain `u8 *`. */
extern u8 gUnknown_08183CA8[];
extern u8 gUnknown_08184A74[];
extern u8 gUnknown_081838EC[];
/* Wave 50 (W50-C). ApplyPaletteExt's `u16 *` first parameter in sub_08068E60,
 * count 0x80 at palette 0 -- four palettes' worth, i.e. ApplyPalettes(x, 0, 4).
 * Typed from that parameter; the address is never dereferenced. */
extern u16 gUnknown_08183C28[];
/* ApplyPaletteExt's `u16 *` first parameter in sub_080697CC and sub_08069FD0
 * (count 0x20 at palette 0xc0 in both). Typed from that parameter, not from
 * any access -- the address is never dereferenced here. */
extern u16 gUnknown_081866D8[];
/* PutSpriteExt's `u16 *` fourth parameter in sub_0806BD84, the same model as
 * the other OAM blobs noted above. Handed over with no dereference. */
extern u16 gUnknown_0816E174[];
/* sub_080670F8's `const u8 *` argument in sub_08069864 -- same shape as the
 * 085819E4 entry noted elsewhere in this file. */
extern const u8 gUnknown_08581438[];
/* Two proc scripts: gUnknown_085819C4 is Proc_Start's in sub_0806B87C and
 * gUnknown_08581A24 is Proc_Start's in sub_0806BE7C. The proc gUnknown_08581A24
 * starts is the one sub_0806BD84 runs -- sub_0806BE7C writes +0x2c/+0x30/+0x58/
 * +0x5c/+0x64 and sub_0806BD84 reads exactly those five, which is a producer/
 * consumer pair agreeing independently on the layout. */
extern const struct ProcCmd gUnknown_085819C4[];
extern const struct ProcCmd gUnknown_08581A24[];
/* Wave 48 (W48-C). A third proc script in the same run: Proc_Start's first
 * argument in sub_0806BED8. The proc it starts is the SAME one gUnknown_08581A24
 * starts -- sub_0806BED8 seeds +0x2a[]/+0x54/+0x58/+0x5c/+0x60 and sub_0806BE7C
 * (already promoted, src/decomp/c_0806BD84.c) reads exactly those, so the two
 * agree on struct Unk6BE7CParent independently. sub_0806BED8 is the string
 * loader for it: it converts a NUL-terminated byte string into the +0x2a
 * halfword table and stores the length in +0x54, which is the bound
 * sub_0806BE7C then counts up to. */
extern const struct ProcCmd gUnknown_08581A34[];
/* A FLAT u16 table, not an array of 2-halfword structs. sub_08069D3C reads
 * element `a1 * 2` as `lsl #2; add; ldrh` and element `a1 * 2 + 1` as
 * `lsl #1; add #1; lsl #1; add; ldrh` -- the second keeps the +1 inside the
 * index expression instead of folding 2 into the load displacement, which a
 * struct member access could not do. Both results are stored to BYTE globals
 * (gUnknown_03002B40 and gUnknown_03002B4C). */
extern u16 gUnknown_08581478[];

/* ---- Wave 35 (W35-A): the 0x08040000-0x08045000 neighbourhood ---- */
/* A u16 counter/mode word in IWRAM. sub_08043834 zeroes it with `strh`;
 * sub_08043590 reads it `ldrh` twice, once as `(u16)(g - 1) <= 3` and once
 * straight into sub_0804423C(int). Named in the unknown-functions.h note on
 * sub_080436DC, which stores its third argument here. */
extern u16 gUnknown_030005D0;
/* Two ROM blobs sub_08043834 copies through sub_08011E54: 0x740 bytes to OBJ
 * VRAM 0x06010000 and 0xc0 bytes to 0x06010840. `u8 []` because sub_08011E54's
 * source parameter is `void *` and neither is indexed. Non-const on the same
 * grounds as gUnknown_081218BC above -- sub_08011E54 takes plain pointers. */
extern u8 gUnknown_08102824[];
extern u8 gUnknown_081259CC[];
/* A table of 16-colour palettes indexed by `gPlayers[n].unk1a - 1` in
 * sub_08043834 -- exactly the gUnknown_0810EA60 shape and declared the same
 * way: `[][16]` and not a flat `u16 []`, because the ROM's index math is
 * `(v - 1) << 5` off the bare symbol and a flat array folds the -1 into the
 * relocation's addend and loses the `subs`. */
extern u16 gUnknown_08104264[][16];
/* Two 16-entry u16 ramps sub_08043590 picks between on sub_0804423C's result,
 * indexed `(gGameClock >> 2) & 0xf` and `(gGameClock >> 1) & 0xf`
 * respectively and handed to sub_0801368C(u16 *, u16, u16). The shift is
 * LOGICAL (`lsrs`) in both, which is what makes the local holding
 * gGameClock unsigned even though that global is declared s32. */
extern u16 gUnknown_08104304[];
extern u16 gUnknown_08104324[];
/* Sprite object-list blobs in ROM, each handed straight to PutSprite's or
 * PutSpriteExt's `u16 *` fourth parameter with no arithmetic -- the same shape
 * as gUnknown_084A0730 above and declared the same way. gUnknown_084A0790 and
 * gUnknown_084A07DA are the pair that note already predicted. */
extern u16 gUnknown_084A0024[];
extern u16 gUnknown_084A0052[];
extern u16 gUnknown_084A005A[];
extern u16 gUnknown_084A0756[];
extern u16 gUnknown_084A0790[];
extern u16 gUnknown_084A07DA[];

/* ---- Wave 36 (W36-B): the 0x08042-0x08045 blocks ---- */
/* Two more of the PutSprite `u16 *` blobs above, drawn one after the other by
 * sub_080436DC with OAM words 0x7000 and 0xE03A. They are 8 bytes apart, the
 * same one-blob-per-8-bytes packing as gUnknown_084A0052/gUnknown_084A005A. */
extern u16 gUnknown_084A0032[];
extern u16 gUnknown_084A003A[];
/* The tile blob sub_080436DC copies to OBJ VRAM 0x06010740, 0x100 bytes at a
 * time, at the byte offset `((gPlayers[n].unk1d * 8) & 0x3ff) * 0x20`
 * -- i.e. 8 tiles per record, wrapping at 0x400 tiles. `u8 []` for
 * sub_08011E54's `void *` source, the same model as gUnknown_08102824. */
extern u8 gUnknown_08102F64[];
/* NOT an address word, even though it sits inside the 0x08091350-0x0809138C
 * `-fforce-addr` pool block: the word at 0x0809136C is 0x0000003F, i.e. the
 * one-character string "?". sub_080436DC passes it to sub_080119A0's
 * `const char *` third parameter as the placeholder for a hidden funds value.
 * Dereferencing the address in baserom.gba is what separates it from its
 * neighbours, all of which hold 0x03xxxxxx/0x08xxxxxx pointers. */
extern const char gUnknown_0809136C[];

/* ---- wave 36 (W36-E): the 0x08075-0x08077 and 0x0807A-0x0807B blocks ---- */

/* Two 16-entry WORD tables in the 0x081CC4F8 run, both subscripted in
 * sub_08075F44 by the same value -- the top nibble of a map halfword -- with
 * one shared `lsls #2`. gUnknown_081CC4F8's element goes to sub_0802D5CC's
 * `int` first parameter and is then re-used to subscript gUnknown_081CC578[];
 * gUnknown_081CC538's goes to sub_0801F2AC's `int` first parameter. The plain
 * word `ldr` off an `lsls #2` index is the whole width evidence, and nothing
 * narrows or sign-tests either element, so `int` is the weakest type that
 * fits. The two run back-to-back, 0x40 bytes each, and end exactly where
 * gUnknown_081CC578 begins, which is what bounds them at 16 entries. */
extern int gUnknown_081CC4F8[];
extern int gUnknown_081CC538[];
/* Six halfwords immediately before the 0x081CC584 literal pool (see the
 * force-addr note below), subscripted by gUnknown_081CC4F8[]'s element with
 * `lsls #1; ldrh` in sub_08075F44 and used in turn to index
 * gTextTable[]. Values 0x0CEF..0x0CF3 -- string ids, the same role the
 * gTextTable subscripts elsewhere in the ROM carry. */
extern u16 gUnknown_081CC578[];
/* FORCE-ADDR, NOT OBJECTS: 0x081CC584, 0x081CC588 and 0x081CC58C hold
 * 0x0202FDFC, 0x03002B6C and 0x030030B4 -- the addresses of gUnknown_0202FDFC,
 * gUnknown_03002B6C and gUnknown_030030B4. They are the -fforce-addr pool of
 * sub_08075F44 and sub_08076888, continuing the run W35-B audited at
 * 0x081CC590/594/598/59C, and both functions match with the objects named
 * directly. Deliberately NOT declared here: naming them would re-invent the
 * fictional globals that note is about. */
/* sub_08075F44 hands it to sub_080718F8's `u8 *` second parameter for 0x360
 * bytes and nothing dereferences it, so that parameter is the only evidence --
 * the same reading gUnknown_081D0BAC carries. */
extern u8 gUnknown_081D22C4[];
/* The twin of gUnknown_08614458 one record along, read exactly the same way:
 * sub_08076298 does `ldrsb` off a zero index register at
 * gUnknown_0861445C[proc->unk40] and hands the result to sub_08071900's `int`
 * third parameter. `s8` on the `ldrsb`, as for gUnknown_08614458. */
extern s8 gUnknown_0861445C[];
/* CORRECTED in wave 38 (W38-B). This was `extern u16 gUnknown_0202FE38;`,
 * generalised from the single `strh 0xFFFF` sub_08076888 aims at the address.
 * That store is real, but it writes the TERMINATOR of a 12-byte record array,
 * and the array's shape is now pinned by four functions that walk it:
 *   - Layout: sub_08074754 fills a record -- +0x00 the s16 id, +0x02/+0x04 the
 *     world x/y copied out of gUnknown_08615194[id].unk06/.unk08, +0x08 a
 *     `struct Unk0801C210 *` sprite handle -- and then stores -1 into the NEXT
 *     record's +0x00 with `strh [r6,#0xc]`, which is what fixes the stride at
 *     12. sub_08074834 copies a whole record with `ldm/stm {r5,r6,r7}`.
 *     +0x06 is untouched by every function that walks the array.
 *   - Terminated by -1 in +0x00, read `ldrsh`: sub_08074670 and sub_08074754
 *     both scan until that field is -1, and sub_080745C0 (matched) empties the
 *     list by storing -1 into the first record.
 *   - 16 entries: sub_08074834 scans 0..15 and compacts the tail over the same
 *     range, and gUnknown_0202FEF8 is allocated at 0x0202FE38 + 16*12.
 * NOT A SEPARATE OBJECT, which is the half the old comment had backwards: it
 * is `&gUnknown_0202FDFC.unk3c` reached as a record array, and sub_08074670
 * proves the original source spelled it that way. That function materialises
 * 0x0202FE38 in ONE pool word and then derives the struct base with
 * `adds r6,r5,#0 / subs r6,#0x3c` rather than loading a second pool word --
 * a CSE agbcc can only do when both addresses come from one symbol_ref plus a
 * compile-time offset. Two independent externs emit two pool words and cost
 * that function 4 bytes. sub_08074754 does load both as separate words, but
 * only because its own copy of the base has been advanced by the scan loop and
 * is dead by the time it needs gUnknown_0202FDFC.unk12.
 * Hence NO extern is declared here -- the type below is the whole declaration,
 * and the callers spell the object `(struct Unk0202FE38 *)&gUnknown_0202FDFC
 * .unk3c`, exactly as the matched c_080745C0.c already does for this address.
 * Do NOT fold the array into struct Unk0202FDFC: that struct is shared and
 * promoted code already reads +0x00..+0x3c through it. */
struct Unk0202FE38
{
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ struct Unk0801C210 *unk08;
};
/* FORCE-ADDR, NOT OBJECTS: the ROM words at 0x081CC4C0, 0x081CC4D4 and
 * 0x081CC4E4 hold 0x0202FE38, 0x0202FEF8 and 0x081D2930 -- the addresses of
 * the record array above, of gUnknown_0202FEF8 and of gUnknown_081D2930. They
 * are the -fforce-addr pools of sub_08074834, sub_08074AD0 and sub_08074D28
 * respectively (each function's base is `ldr rN,=<word>; ldr rM,[rN]`).
 * sub_08074834 and sub_08074D28 match with the objects named directly;
 * sub_08074AD0 is parked at 140/144 for an unrelated reason (see
 * work/sub_08074AD0). Deliberately not declared, for the same reason as the
 * 0x081CC584 run above. Note the batch index still lists them as `data_refs`,
 * so they read like ROM tables.
 *
 * WAVE 38 (W38-E) -- 0x081CC4D4 RE-TESTED AS A REAL POINTER OBJECT AND IT IS
 * NOT ONE. sub_08074AD0 reloads its base `ldr r5,[r7]` on every iteration of a
 * loop that stores through it, which argues for a genuine `s8 *` global whose
 * reload is forced by aliasing, since a constant-pool MEM is RTX_UNCHANGING_P
 * and LICM would hoist it. Declaring `extern s8 *gUnknown_081CC4D4;` and
 * indexing it does NOT replace the indirection -- agbcc applies force-addr to
 * the pointer variable as well and emits three levels,
 * `ldr r7,=<pool>; ldr r6,[r7]; ldr r0,[r6]`, plus an `R_ARM_ABS32 .rodata`
 * word the ROM does not have (53.5%, size exact). The force-addr reading above
 * is confirmed and this axis is closed; the in-loop reload is explained by
 * where the `ldr r7` lands relative to the loop guard, not by aliasing. See
 * docs/agbcc-codegen.md, "A `-fforce-addr` word is NOT a pointer object". */
/* sub_0801C210's sprite/animation script for the 0x08074000 block's overworld
 * markers: sub_08074754 and sub_08074D28 both pass it as that function's first
 * argument and nothing dereferences it, so the `void *` parameter is the only
 * evidence and `u8 []` follows the modelling used for every other
 * sub_0801C210 script. */
extern u8 gUnknown_081D2930[];
/* sub_08076888's setup blobs, each typed straight off the parameter it is
 * handed to and nothing else: 08614548 is sub_08012C58's `void *`; 081CC5F0,
 * 081D1644, 081D17A4 and 081D2A54 are Decompress `u8 *` sources for four fixed
 * VRAM destinations; 081D208C and 081D20CC are ApplyPaletteExt `u16 *` sources
 * for 0x20 bytes each, the pair it picks between on IsHardCampaignMode. None is
 * const, because none of those parameters is. */
extern u8 gUnknown_08614548[];
extern u8 gUnknown_081CC5F0[];
extern u8 gUnknown_081D1644[];
extern u8 gUnknown_081D17A4[];
extern u8 gUnknown_081D2A54[];
extern u16 gUnknown_081D208C[];
extern u16 gUnknown_081D20CC[];
/* Wave 54 (W54-F). sub_08077304's own blobs, each typed straight off the
 * parameter it is handed to and nothing else, exactly as the run above:
 * 081D1F74, 081D20EC and 081D35A8 are Decompress `u8 *` sources; 081D2264 is an
 * ApplyPaletteExt `u16 *` source for 0x20 bytes; 081D2330 and 081D249C are
 * sub_080718F8's `u8 *` second parameter for 0x360 bytes each. None is const,
 * because none of those parameters is. */
extern u8 gUnknown_081D1F74[];
extern u8 gUnknown_081D20EC[];
extern u8 gUnknown_081D35A8[];
extern u16 gUnknown_081D2264[];
extern u8 gUnknown_081D2330[];
extern u8 gUnknown_081D249C[];
/* Wave 54 (W54-F). Reached only through the -fforce-addr word at 0x081CC5AC,
 * which holds 0x086145CE (read out of baserom.gba); the object itself is a
 * halfword table sub_08077304 subscripts with `6 - sub_08037D80(...)` and hands
 * to sub_0801F234 and sub_0801F2AC. `ldrh` off a scaled index, so u16; the
 * extent is unproved (the index is a small non-negative count). */
extern u16 gUnknown_086145CE[];
/* A byte table sub_0807A0C4 subscripts with the u16 at +0x52 of its proc and
 * whose element it turns into an OAM attribute-2 tile index (`+ 4`, `<< 12`,
 * or'd with 0x240). `ldrb`, so u8; nothing bounds it. */
extern u8 gUnknown_08615E48[];
/* Wave 51, W51-F. The same byte table one record earlier, subscripted by the
 * proc's +0x52 counter by sub_08079FAC and turned into the identical OAM
 * attribute-2 tile index (`+ 4`, `<< 12`, or'd with 0x240) as gUnknown_08615E48
 * above and gUnknown_08615E40 further up.
 *   THE EXTENT ON gUnknown_08615E40 ABOVE ("[12]") IS PROBABLY WRONG, and this
 * symbol is the evidence. baserom 0x08615E40 reads `02 03 04 05 02 03 04 05 02
 * 03 04 05` and then a run of address words -- so the wave that declared it
 * bounded the RUN correctly but read it as one 12-byte array. It is three
 * IDENTICAL 4-byte tables at 0x08615E40, 0x08615E44 and 0x08615E48, one per
 * caller, and all three offsets are separately referenced pool words in three
 * different functions. gUnknown_08615E48 was already declared separately for
 * the same reason. I have NOT narrowed gUnknown_08615E40 -- an extent on an
 * extern u8 array is byte-neutral and no oracle would catch a mistake either
 * way -- but the three-table reading is what the ROM bytes say. */
extern u8 gUnknown_08615E44[4];
/* 0x14-byte records: four Decompress `u8 *` blobs and one ApplyPaletteExt
 * `u16 *` palette, picked by sub_0807A99C on a 0..17 index (the stride is
 * `x * 5 << 2`). unk00/unk04/unk08 go to three fixed offsets inside the BG
 * char block gUnknown_0300251C.bits.chr_block selects, unk0c to the buffer
 * gUnknown_08499584 points at, and unk10 is the palette -- except on index 7,
 * where cse's record_jump_equiv folds the subscript and the ROM reads the
 * record-7 palette slot as a bare `+ 0x9c`. */
struct Unk08615E4C /* 0x14 */
{
    /* 0x00 */ u8 *unk00;
    /* 0x04 */ u8 *unk04;
    /* 0x08 */ u8 *unk08;
    /* 0x0c */ u8 *unk0c;
    /* 0x10 */ u16 *unk10;
};
extern const struct Unk08615E4C gUnknown_08615E4C[];
/* The proc script sub_0807BCF0 hands to Proc_Start with its own proc as the
 * parent, immediately before breaking itself. */
extern const struct ProcCmd gUnknown_08616548[];

/* ---- wave 36 (W36-J): the 0x08046-0x08049 address-locality block ---------- */

/* 0x02028E18. A one-bit toggle: sub_08047F70 does
 * `gUnknown_02028E18 = (gUnknown_02028E18 + 1) & 1;` (ldrb; adds #1; movs #1;
 * ands; strb) when L or R is held. u8 from the load; nothing else reads it in
 * matched code, so only the width and the 0/1 range are proved. */
extern u8 gUnknown_02028E18;

/* 0x084C30FC. sub_080499F8 hands the ADDRESS straight to sub_0801BD00's
 * `void *` third parameter with no arithmetic -- the same model as
 * gUnknown_0848B698 above. The first ROM word is 0x00000004, so it is a data
 * blob and not a pointer variable. */
extern u32 gUnknown_084C30FC[];

/* Two OAM sprite blobs sub_080499F8 draws four times each (x = 0, 0x20, 0x40,
 * 0x60), 0848B6C6 on row 0x1c and 0848B6BE on row 0x2c. `u16 []` because that
 * is PutSprite's fourth parameter, and NOT const for the same reason -- the
 * parameter is a plain `u16 *`. gUnknown_0848B6C6 is also sub_08048158's
 * case-1 blob. */
extern u16 gUnknown_0848B6BE[];
extern u16 gUnknown_0848B6C6[];

/* sub_08048158's case-3 and case-2 OAM blobs, on PutSprite's `u16 *` fourth
 * parameter with no arithmetic, alongside gUnknown_08615C4E (its case-0 blob,
 * declared above). Same non-const reason. */
extern u16 gUnknown_08615C12[];
extern u16 gUnknown_08615C20[];

/* Wave 54, W54-C. The fourth row of the same OAM-blob run as gUnknown_08615C20
 * seven halfwords above it: sub_08079618 and sub_0807974C each draw the C20
 * blob at y = 0x50/0x60/0x70/0x88 and this one at y = 0x80, all five straight
 * into PutSprite's `u16 *` fourth parameter with no arithmetic and no
 * dereference (`ldr r3, =gUnknown_08615C2E`). Same `u16 []`, same non-const
 * reason as its neighbours. */
extern u16 gUnknown_08615C2E[];

/* Wave 51, W51-F. sub_080794E8's second PutSprite passes this straight into the
 * `u16 *` fourth parameter (`ldr r3, =gUnknown_08615BE4` with no dereference and
 * no arithmetic), exactly like the gUnknown_08615C12/C20 blobs above -- an OAM
 * blob, and a real symbol rather than a pool word for the same reason. Same
 * non-const reason as its neighbours. */
extern u16 gUnknown_08615BE4[];

/* 0x0823E550, a 16-colour palette on ApplyPaletteExt's `u16 *` first
 * parameter. sub_08048644 and sub_08048158 both index it
 * `DivRem(Div(0x40 - DivRem(gGameClock, 0x40), 4), 0x10)` -- a 16-step
 * cycle, same shape as gUnknown_08239F84 -- and both also pass the base
 * unindexed, so the extent is 16 and the element is `u16`. The 0x20 bytes at
 * 0x0823E550 read as palette data (first word 0x7BBF77BF). Not const:
 * ApplyPaletteExt takes a plain pointer. */
extern u16 gUnknown_0823E550[16];

/* 0x0812AF68. sub_08046E48's only use is sub_08071948's `const void *` fourth
 * parameter, by name with no dereference. NOT a -fforce-addr word despite the
 * 0x0812Axxx address: the ROM word there is 0x0007130D, which is not an
 * address, and the call site never does the second `ldr` a force-addr word
 * needs. (The neighbours 0x0812A118/11C/120/13C/154/16C ARE force-addr words --
 * see the note on those below.) */
extern u8 gUnknown_0812AF68[];

/* 0x0812C024, 0x80 bytes of tile data sub_08046E48 uploads to VRAM in four
 * sub_08011C68 calls: +0x00 (0x60 bytes) to 0x06014EE0, +0x20 (0x40) to
 * 0x06014F40, and +0x60 (0x20) to BOTH 0x06014F80 and 0x06014FA0. `u8 []`
 * because the offsets are byte offsets off the base (`adds r0, #0x20`,
 * `adds r4, #0x60`); sub_08011C68's first parameter is `const void *`, so the
 * element type is not constrained further. */
extern u8 gUnknown_0812C024[];

/* Two more tables of the same 20-byte record as gUnknown_085D583C, selected by
 * gUnknown_02028DD6 in sub_08046E48 (== 6 picks 084998A4, == 8 picks 0849982C,
 * anything else falls back to gUnknown_085D583C itself) and then indexed by
 * gUnknown_02028DD7 with the identical `lsls #2; adds; lsls #2` stride-20
 * arithmetic. Only +0x00 (a Decompress `u8 *`) and +0x04 (an ApplyPaletteExt
 * `u16 *`) are reached through these two; the tag is shared because the index
 * arithmetic and both member offsets are byte-identical across all three
 * arms. */
extern const struct Unk085D583C gUnknown_0849982C[];
extern const struct Unk085D583C gUnknown_084998A4[];

/* THE 0x0812Axxx WORDS ARE -fforce-addr ADDRESS CONSTANTS, NOT OBJECTS.
 * Wave 36, W36-J dereferenced all six in baserom.gba:
 *     0x0812A118 -> 0x03002EE0   0x0812A13C -> 0x03002EE0   (both &gpKeySt)
 *     0x0812A11C -> 0x02028DD5   0x0812A120 -> 0x02028DD6
 *     0x0812A154 -> 0x084C30F8   0x0812A16C -> 0x084C3240
 * 0x0812A118 and 0x0812A13C holding the SAME address is the decisive tell from
 * the brief -- N scattered symbols, one address. aw2bhr.lds puts gpKeySt at
 * 0x03002EE0, so the honest spellings are `gpKeySt->...` (sub_08046D30,
 * sub_08047F70, sub_08048F4C), `gUnknown_02028DD5` / `gUnknown_02028DD6`
 * (sub_08046E48) and `gUnknown_084C3240` (sub_08049C38). Do NOT declare a
 * `gUnknown_0812Axxx` object for any of them; an inherited draft that did
 * (`struct Unk03002EE0 **const gUnknown_0812A118`) was the fictional-global
 * pattern, and the honest spelling matched. */

/* THE 0x08090AC4-0x08090C30 RUN IS ONE TRANSLATION UNIT'S `.rodata`, NOT DATA.
 * Wave 36, W36-M dereferenced every word of it in baserom.gba. It is the
 * `.rodata` of the unit holding sub_080281F0..sub_0802DCB4 (code-0801D390.s),
 * and it interleaves STRING LITERALS with `-fforce-addr` address constants,
 * exactly as agbcc emits them -- one address word per (function, symbol) pair,
 * in address order:
 *     0x08090AC4  const char *[2] = { "OFF", "ON" }  (-> 0x08090AD0/0x08090ACC)
 *     0x08090ACC "ON"   0x08090AD0 "OFF"  0x08090AD4 "O"
 *     0x08090AD8 " MAP:%02d"   0x08090AE4 "SNOW:%s"  0x08090AEC "SAKU:%s"
 *     0x08090AF4 -> 0x03002EE0  &gpKeySt
 *     0x08090AF8 -> 0x03001FBC  &gUnknown_03001FBC
 *     0x08090AFC -> 0x03001470  &gUnknown_03001470
 *     0x08090B00 -> 0x03003FC0  &gPlaySt        (sub_080281F0)
 *     0x08090B04 "R: SAKUTEKI ON"   0x08090B14 "R: SAKUTEKI OFF"
 *     0x08090B28 "1P"  0x08090B2C "CP"  0x08090B30 "2P"  0x08090B34 "VS"
 *     0x08090B38 "--"  0x08090B3C "PAUSE"
 *     0x08090B44 -> 0x03003FC0   0x08090B48 -> 0x03001FBC
 *     0x08090B4C -> 0x03001470                            (sub_080283E4)
 *     0x08090B5C -> 0x08499590  &gUnknown_08499590        (sub_080290B0)
 *     0x08090B60 -> 0x0849A00C   0x08090B64 -> 0x03002EE0 (sub_0802925C)
 *     0x08090B90 -> 0x0849A2A6   0x08090B94 -> 0x0849A284 (sub_0802A8DC)
 *     0x08090C18 -> 0x03001FBC   0x08090C1C -> 0x03002EE0 (sub_0802DA18)
 *     0x08090C20 -> 0x03002EE0  &gpKeySt                  (sub_0802DCB4)
 * Do NOT declare a `gUnknown_08090Bxx` object for any of these: write the
 * honest spelling (`gpKeySt->held`, `gUnknown_03001470[i]`, ...) and let agbcc
 * place its own copy. Note that the same source spelling gives TWO levels of
 * indirection in one function and THREE in another -- sub_0802DCB4 reads
 * gUnknown_08499590 with a plain inline pool word while sub_080290B0 gets the
 * `.rodata` reroute -- and the discriminator is reference count across a
 * control-flow merge, not the spelling. Neighbouring words 0x08090B50/B54/B58
 * (&gPlayers, &gUnknown_08499590, &gUnknown_020237B0) belong to
 * functions between these and are the same class. */

/* 0x0201E450. Only +0x04 and +0x06 are reached: sub_08028F84 `strh`s a scroll
 * position pair into them -- first a Q16 value divided down by 0x10000, then,
 * on the last frame of the slide, the destination pair straight out of the
 * proc. Nothing reads them in asm/, so the width is from the `strh` and the
 * signedness follows the signed division feeding it. Wave 36, W36-M. */
struct Unk0201E450
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
};
extern struct Unk0201E450 gUnknown_0201E450;

/* 0x020237B0, and 0x80 bytes below gUnknown_02023830. A per-frame step table:
 * sub_08028F84 indexes it with a countdown halfword and adds the byte to an
 * accumulator (`ldrb`, added to a u16, no sign extension anywhere), so `u8 []`.
 * Wave 36, W36-M. */
/* Wave 38, W38-I. A step-ramp scratch buffer: sub_08028EF0 fills [0..n] with a
 * 1,2,3,...,8,8,8 acceleration ramp summing to a cursor-slide distance, one
 * `strb` per entry, then writes the remainder as a final short step -- so `u8 []`
 * is right and the extent is bounded by that distance, not pinned here.
 *   Its ROM pool word is 0x08090B58, which holds 0x020237B0 (dereferenced out of
 * baserom.gba, and 0x08090B54 / 0x08090B5C on either side both hold 0x08499590,
 * i.e. this is the middle of one unit's `.LC` block). agbcc's -fforce-addr copy
 * is what the ROM's two-level `ldr r5,=<word>; ldr r4,[r5]` is; the HONEST
 * `gUnknown_020237B0[...]` spelling reproduces it exactly and the build places
 * the word, per wave 18. Do NOT declare gUnknown_08090B58 as a `u8 **` and
 * dereference it -- that adds a fourth level, the error wave 20 recorded for
 * this same idiom. */
extern u8 gUnknown_020237B0[];

/* 0x08499F4C. Six halfword bounds read in pairs by sub_080281F0 -- [0]/[1] for
 * the gPlaySt.unk02 field, [2]/[3] for .unk2c and [4]/[5] for .unk0d,
 * each pair being the lower and upper clamp of one debug-menu row. UNSIGNED and
 * measured: every compare against the u8 field is `bhs`/`bls`, never `bge`.
 * Wave 36, W36-M. */
extern const u16 gUnknown_08499F4C[];

/* Two OAM/sprite blobs handed to sub_0801BD00 as its `void *` third argument by
 * sub_0802A8DC, alongside the 0x11CA / 0x61E6 attribute words. Declared `const
 * u16 []` and cast at the call site, the same way c_08022A5C.c spells
 * gUnknown_0848930C. Wave 36, W36-M. */
extern const u16 gUnknown_0849A1F0[];
extern const u16 gUnknown_0849A240[];

/* Wave 37 (W37-E). Two more sub_080193B0 script openers, picked by
 * sub_080005FC on sub_08004E44()'s result. `const u8 []` because that is
 * sub_080193B0's declared parameter and nothing here dereferences them. */
extern const u8 gUnknown_084856FC[];
extern const u8 gUnknown_084857AC[];

/* Wave 37 (W37-E). Three ROM tables sub_080032EC memcpys to its own stack with
 * sub_0808B6E8, one call each, so the element type is what the STACK COPY is
 * read as and the extent is the byte count divided by it:
 *   0808D760  8 bytes  28 00 48 00 68 00 88 00  -> read `ldrh`, so u16[4].
 *             These are sub_0800C6E8's kind codes (0x28/0x48/0x68/0x88).
 *   0808D768  4 bytes  00 12 00 12              -> read `ldrb; lsls #0x18;
 *   0808D76C  4 bytes  D0 D0 E2 E2                 asrs #0x18`, so s8[4].
 *             An x and a y offset per lane; 0xD0/0xE2 are -48 and -30, which
 *             only make sense signed.
 * The neighbouring 0x0808D770 is NOT one of these -- it is an -fforce-addr
 * address constant holding &gActiveMap (dereferenced in baserom.gba),
 * the same as 0x0808D6DC/0x0808D70C/0x0808D710/0x0808D714 in the same run. */
/* Wave 42, W42-A. Two sub_0808B6E8 (memcpy) sources copied to a caller's stack
 * buffer. gUnknown_0808D728 is 0x28 bytes read back as 20 HALFWORDS by
 * sub_08002F1C (`ldrh` off a stride-2 pointer over a 5x4 nest), so `u16 [20]`.
 * gUnknown_0808D750 is 4 bytes read back as 4 BYTES by sub_08002FE4 (`ldrb`,
 * masked `& 0x1f`); the 8-byte gUnknown_0808D754 immediately after it confirms
 * the extent. */
extern const u16 gUnknown_0808D728[];
extern const u8 gUnknown_0808D750[];
extern const u16 gUnknown_0808D760[];
extern const s8 gUnknown_0808D768[];
extern const s8 gUnknown_0808D76C[];

/* Wave 37 (W37-E). sub_08001DAC's two blobs: gUnknown_08485B2C is PutSprite's
 * `u16 *` fourth argument and gUnknown_084891C0 is ApplyPaletteExt's `u16 *`
 * first, 0x20 bytes applied at palette byte offset 0x260. Neither is
 * dereferenced here, so the element type is the parameter's and the extent is
 * unknown. */
extern u16 gUnknown_08485B2C[];
extern u16 gUnknown_084891C0[];

/* Wave 37 (W37-F). The tile-id table sub_0800F418 indexes with the 8-neighbour
 * bitmask it builds, so 0x100 entries. Read `movs r2,#0; ldrsh r0,[r1,r2]`,
 * and the ROM run at 0x084865C4 is full of 0xFFFF entries interleaved with
 * small positive ids (0x61, 0x83, 0x41, 0x42, 0xe0), so the 0xFFFF are -1
 * reject markers and the element type is SIGNED s16, not u16. */
extern s16 gUnknown_084865C4[];

/* Wave 37 (W37-H). sub_08039C70's OBJ blob: handed straight to
 * sub_0801BD00's `void *` third parameter with no arithmetic in front of it,
 * exactly like gUnknown_08499B6C and gUnknown_0849A3B8. Nothing indexes or
 * dereferences it here, so the element type is unconstrained and `u8 []` is the
 * weakest spelling that converts to `void *` without a const cast. */
extern u8 gUnknown_0849D824[];

/* 0x03000060 CANNOT BE DECLARED -- wave 37 (W37-G), the same class as
 * 0x03000600/0x03000602 above. aw2bhr.lds names gUnknown_0300005C at 0x5c and
 * then jumps to gUnknown_03000068, so 0x03000060 falls INSIDE the
 * gUnknown_0300005C region and an invented symbol has nowhere to live;
 * tools/proto_check.py flags it and the split build would fail with
 * `undefined reference` even though trymatch reports a match.
 *   Spell it `((u32 *)&gUnknown_0300005C)[1]`, which is what
 * work/sub_0801BB10 does.
 *   What it IS: the interrupt-enable shadow. sub_0801BB10 is its whole
 * interface -- a three-case switch that SETs / ANDs / ORs it with the caller's
 * mask and then pushes it to the hardware as
 * `REG_IE = g; REG_IME = (g & 0x10000) != 0`. A WORD (`ldr`/`str` at every
 * access) even though only the low halfword reaches REG_IE; bit 16 is the
 * software IME flag, which is what makes it 32 bits rather than a u16. Not
 * volatile: the single load at the tail feeds both the `strh` to REG_IE and
 * the 0x10000 test, i.e. agbcc CSEd it. Reached through the -fforce-addr
 * .rodata word at 0x0808F088, whose ROM content is 0x03000060, because the
 * switch names it on four separate arms. */

/* Wave 37 (W37-H). sub_08038C98 copies 0xa00 bytes of it to 0x06013940 with
 * sub_08011E54, with no arithmetic on the symbol -- a single tile blob, the
 * same shape as the 08122xxx/08123xxx run. `u8 []` is the weakest spelling
 * that reaches sub_08011E54's `void *` without a const cast. */
extern u8 gUnknown_080A1C24[];

/* Wave 37 (W37-N). The two-byte glyph pairs sub_080724D4 stamps when it
 * formats an integer into a text buffer: 08613F24 is the '0' glyph and
 * 08613F28 the '-' sign. Both are read a byte at a time (`ldrb [r0]`,
 * `ldrb [r0, #1]`) and the second byte of the '0' pair has the digit added to
 * it, so the pair is (attribute, character) and the characters are
 * consecutive. `u8 []` from the `ldrb`s; const because they live in ROM and
 * nothing writes them. */
extern const u8 gUnknown_08613F24[];
extern const u8 gUnknown_08613F28[];

/* Wave 37 (W37-N). The 21-entry scale ramp sub_08072CE4 indexes with
 * `unk64 - i * 4` (0..0x14 inclusive, anything above clamped to 0x100).
 * WORDS -- `ldr r2, [r0]` off a `<< 2` index -- and the value is handed
 * straight to `Div`'s s32 divisor, so `s32`. The array bound is exact:
 * 0x081CC01C - 0x081CBFC8 = 0x54 = 21 words, and 0x081CC01C is the next
 * symbol. */
extern const s32 gUnknown_081CBFC8[0x15];

/* Wave 37 (W37-N). An OAM sprite blob in ROM, reaching nothing but the `u16 *`
 * fourth parameter of PutSprite / PutSpriteExt in the 0x08072CE4-0x08073070
 * proc bodies -- the same model as gUnknown_0849B6C8. NOT const, because that
 * parameter is not. */
extern u16 gUnknown_081CC01C[];

/* Wave 37 (W37-G). sub_08018254 hands it to sub_08071948's `const void *`
 * fourth parameter with no arithmetic in front of it, the same shape as
 * gUnknown_0849D824 above.  The ROM word at 0x080D445C is 0x081F051D, an ODD
 * value, so it is data and not an address table; nothing here dereferences
 * it, which leaves `const u8 []` as the weakest spelling that fits. */
extern const u8 gUnknown_080D445C[];

/* Wave 37 (W37-H). sub_08035BC4's sprite pair: parameters 1 and 3 of the same
 * `sub_08015438(void *, int, void *, ...)` call that src/decomp/c_0803B264.c
 * fills with gUnknown_0849E700 / gUnknown_0849E6F8 and c_08027A50.c with
 * gUnknown_08499DE8 / gUnknown_08499DDC. Non-const `u8 []` on the same
 * reasoning as gUnknown_08499B6C: the callee's parameters are plain `void *`,
 * nothing here indexes or dereferences either symbol, and `u8 []` is the
 * weakest spelling that converts without a const cast. UNVERIFIED --
 * sub_08035BC4 did not close this wave, so no matched code reads them yet. */
extern u8 gUnknown_0849BDE0[];
extern u8 gUnknown_0849BFD8[];

/* Wave 37 (W37-I). The "turn banner" text block at 0x08090D90..0x08090E24,
 * read straight out of baserom.gba rather than guessed:
 *   0x08090D90  0000 0001 0002 0003 0003 0003 0002 0001 0000 0000  (u16 x 10)
 *   0x08090DA4  "NEXT TURN"        0x08090DB0  "PRESS A BUTTON"
 *   0x08090DC0/DD0, 0x08090DE0/DF0 the other two language pairs, and
 *   0x08090E04  " PR'OXIMA FASE"   0x08090E14  " PULSA BOT'ON A"
 * sub_08034AF8 indexes the first table with `(gGameClock / 3) % 10` and
 * adds 0x68 to the entry to get the x of the second string, and hands the eight
 * string addresses to sub_08034A58's `const char *`. The u16 table is a
 * bare `lsls #1; adds; ldrh` off its own pool word, so it is a halfword array
 * and NOT a force-addr word; 0x08090E24 immediately after it IS one (it holds
 * 0x030033EC). */
extern const u16 gUnknown_08090D90[];
extern const char gUnknown_08090DA4[];
extern const char gUnknown_08090DB0[];
extern const char gUnknown_08090DC0[];
extern const char gUnknown_08090DD0[];
extern const char gUnknown_08090DE0[];
extern const char gUnknown_08090DF0[];
extern const char gUnknown_08090E04[];
extern const char gUnknown_08090E14[];

/* Wave 37 (W37-I). sub_080409E8 starts this script on tree 3 and then hands the
 * proc it gets back to Proc_Start(gUnknown_0849FD44, proc) as the parent, so the
 * two are the halves of one pair and take the same `const struct ProcCmd []`
 * gUnknown_0849FD44 already has. */
extern const struct ProcCmd gUnknown_0849FD84[];

/* Wave 37 (W37-I). Two four-entry s16 offset rows read only as
 * `movs rI,#{0,2,4,6}; ldrsh` off their own pool words by sub_0803EF70, which
 * adds entry i of each to a screen x/y before masking to 0x1ff / 0xff. SIGNED
 * from the `ldrsh`s; the [4] is what the four indices prove and the 8-byte gap
 * between the two symbols allows exactly that. */
extern const s16 gUnknown_0849F820[4];
extern const s16 gUnknown_0849F828[4];

/* Wave 37 (W37-I). sub_0803EF70's four graphics blobs: BE0 and 3D0 go to
 * Decompress's `u8 *` source, E34 to ApplyPaletteExt's `u16 *`, and F84/740 to
 * sub_0801C70C's `const void *` first parameter. */
extern u8 gUnknown_08113BE0[];
extern const u8 gUnknown_08113F84[];
extern u8 gUnknown_081143D0[];
extern const u8 gUnknown_08114740[];
extern u16 gUnknown_08114E34[];

/* Wave 37 (W37-I). sub_0803F6BC's shared 0x40-byte tail block: it is
 * sub_08011E54's `void *` source in six of the seven switch arms and never
 * indexed, so `u8 []` is the weakest spelling that converts. */
extern u8 gUnknown_08485A2C[];

/* Wave 37 (W37-I). gUnknown_08551D18 is the second COLUMN VIEW of
 * gUnknown_08551D0C's three-halfword rows (0x08551D0C + 0xc), reached through
 * its own pool word by sub_0804B180 with a bare `lsls #1` on the side index --
 * the same "one symbol per column" split gUnknown_0202972C / gUnknown_02029710
 * and gUnknown_03004582 / gUnknown_03004580 already have. Masked with 0x3ff and
 * placed at bit 0 of the OAM attr2 word, i.e. a tile number. */
extern u16 gUnknown_08551D18[];

/* Wave 37 (W37-I). The sub_0804C5A4 / sub_0804CA98 script tables, all four
 * indexed off the `side` (0/1) key:
 *   gUnknown_08553318  a flat halfword row indexed by gUnknown_020296B0.unk18,
 *                      compared for equality against gUnknown_03004508
 *   gUnknown_08553354  a PAIR per side, both halves stored into a
 *                      struct Unk56E28's unk04/unk06
 *   gUnknown_0855335C  five PAIRS per side (`side * 10 + step * 2 + half` in
 *                      halfword units), added to an entry's x and y
 *   gUnknown_08553C14  a BYTE pair per gUnknown_0300450C, added to
 *                      gUnknown_08553318's entry before the compare
 * The row lengths are what the index arithmetic proves; nothing bounds the
 * outer dimension. Signedness unproved -- every read is a bare `ldrh`/`ldrb`
 * feeding an add. */
extern u16 gUnknown_08553318[];
extern u16 gUnknown_08553354[][2];
extern u16 gUnknown_0855335C[][5][2];
extern u8 gUnknown_08553C14[][2];

/* Wave 37 (W37-I). sub_0804ABDC's four sprite blobs. The three `u16 []` ones
 * are sub_08013664's first parameter and are indexed by a 4-bit animation
 * phase scaled `lsls #1`, so they are halfword arrays; the two `u8 []` ones are
 * sub_0801BD00's `void *` third parameter and are never indexed.
 * gUnknown_0813204C is NOT gUnknown_0812A294 -- that address is a
 * `-fforce-addr` .rodata word holding this one (dumped from baserom.gba), and
 * the same is true of 0x0812A290 -> gUnknown_030044E0, 0x0812A298 ->
 * gUnknown_030030A0 and 0x0812A29C -> gUnknown_0848B6C6. */
extern u16 gUnknown_0813204C[];
extern u16 gUnknown_08131D8C[];
extern u16 gUnknown_08131DEC[];
extern u8 gUnknown_0848B688[];
/* Wave 54, W54-G. The second of the pair, 0x28 bytes past gUnknown_0848B688 and
 * reached exactly the same way: sub_0802B4D4 hands it to sub_0801BD00 as that
 * function's `void *` third argument and never indexes it, so `u8 []` on the
 * same evidence as its neighbour above. */
extern u8 gUnknown_0848B6B0[];
/* Wave 54, W54-G. The GBA OBJ DIMENSION tables, and the ROM values settle it:
 * dumped from baserom.gba the two interleave as (8,8) (16,16) (32,32) (64,64) /
 * (16,8) (32,8) (32,16) (64,32) / (8,16) (8,32) (16,32) (32,64) -- the standard
 * square/wide/tall OBJ size grid, so 0848B6F6 is the WIDTH and 0848B6F8 the
 * HEIGHT. sub_0801E9B0 indexes both with the same byte offset,
 * `((attr0 & 0xC000) >> 10) + ((attr1 & 0xC000) >> 12)`, i.e. SHAPE selects the
 * 0x10-byte row and SIZE the 4-byte column, hence `[][8]` with the column
 * subscript written `(attr1 & 0xC000) >> 13` (the `* 2` scaling merges into the
 * shift). SIGNED: every read is `ldrsh`, and the values are added to and
 * subtracted from signed screen coordinates.
 *   They are kept as TWO separate arrays rather than one array of pairs
 * because the ROM loads two independent pool words and adds the same index to
 * each; a pair struct would compute one element address and use displacements
 * 0 and 2, which is not what is there. */
extern const s16 gUnknown_0848B6F6[][8];
extern const s16 gUnknown_0848B6F8[][8];
extern u8 gUnknown_084C3B72[];

/* Wave 37 (W37-I). Two byte lookup rows sub_0804ABDC indexes with the s16
 * gUnknown_030044E0->unk1e / ->unk20 to build sub_0801BD00's x and y. Bare
 * `ldrb`, so byte-wide; nothing signs them. */
extern u8 gUnknown_084C3D14[];
extern u8 gUnknown_084C3D5C[];

/* Wave 37 (W37-I). The two `side` bytes sub_08041978 derives from
 * sub_08041D40's result and its complement (`1 - x`) and then uses as the row
 * index into gUnknown_03004580 and gUnknown_03004528. Bare `strb`/`ldrb`
 * pairs, so byte-wide; unsigned is unproved. */
extern u8 gUnknown_03003F50;
extern u8 gUnknown_03004484;

/* Wave 37 (W37-I). Two byte tables inside one blob at 0x08091318 (0x0809131E is
 * 0x08091318 + 6), both indexed with a bare `ldrb`: the first by the s16 at
 * gUnknown_030013D0 + 0x18 / gUnknown_030013B0 + 0x18, the second by the u8 at
 * the head of a gUnknown_08499594 record. The ROM bytes are
 * 00 01 01 01 01 02 | 00 00 01 02 03 04 05 06 07 06 ... -- a small remap, and
 * the two symbols overlap exactly as gUnknown_08551D0C / gUnknown_08551D18 do. */
extern const u8 gUnknown_08091318[];
extern const u8 gUnknown_0809131E[];

/* Wave 37 (W37-J3), block 0x08056. Six ROM tables that sub_08056D8C and
 * sub_08056EEC read, all with plain `ldrh`/`ldrb` and none of them signed by
 * any use:
 *   0x08553858  u8[12] = 0,0,0,1,1,2,2,3,3,4,4,0 -- subscripted by
 *               gUnknown_03004580[i][5] and [i][6] and used as the LOW index
 *               of gUnknown_08557914.
 *   0x08553864  u16[8], the multiples of 25 (0, 0x19, 0x32 .. 0xAF), indexed
 *               by sub_08056EEC's third argument.
 *   0x08553874  u16 = 0x0050, 0x0122; 0x08553884 u16 = 1, 0; 0x08553888 u16 =
 *               7, 8 -- all three indexed by the side index i, so a pair each.
 *   0x08553878  u16[6], indexed
 *               `gUnknown_085D6A48[gUnknown_03004580[i][1]][1] * 2 + i`, the
 *               same subscript sub_08057048 uses on gUnknown_085538A2.
 * All five halfword tables feed the six-halfword record sub_08056EEC hands to
 * sub_08056F8C, exactly as gUnknown_0855388C / gUnknown_085538A2 feed
 * sub_08057048's. */
/* Wave 60, W60-H. A per-side pair of Decompress DESTINATIONS: sub_080566C8
 * reads `gUnknown_08553850[i]` with a word `ldr`, hands it to
 * `Decompress(u8 *, void *)`'s second parameter and then re-reads it TWICE
 * more into gUnknown_02029BA8[i].unk18[0] and .unk18[1], which are already
 * `void *`. An array of POINTERS, not of data -- the word is
 * dereferenced by nothing here and used only as an address. Non-const for the
 * same reason every Decompress argument in this header is: the prototype takes
 * plain pointers. The extent is the two sides. */
extern void *gUnknown_08553850[];
extern u8 gUnknown_08553858[];
extern u16 gUnknown_08553864[];
extern u16 gUnknown_08553874[];
extern u16 gUnknown_08553878[];
extern u16 gUnknown_08553884[];
extern u16 gUnknown_08553888[];
/* Ten words -- 3, 5, 6, 7, 8 twice over -- read with `ldr` by sub_08056D8C as
 * `[gUnknown_03004580[i][0] * 5 + gUnknown_08553858[...]]` (five per row, two
 * rows) and stored straight into struct Unk02029BA8's `void *` unk10.
 * UNPROVEN: the CONTENTS are small integers and not addresses, so unk10 is
 * very probably an id rather than a pointer; `void *` is spelled here only so
 * the store agrees with the already-promoted struct, which nothing else
 * contradicts. Left flat rather than `[][5]` because the ROM computes
 * `(row * 5 + col) * 4` as one scaled index, not `row * 20 + col * 4`. */
extern void *gUnknown_08557914[];
/* Wave 48, W48-D. A [10][12] grid of 0x2c-byte records in EWRAM. Extents are
 * hard: sub_0806279C clears it with `for (i = 0; i <= 9; i++) for (j = 0;
 * j <= 0xb; j++)` and the row stride it computes is `((i << 5) + i) << 4` ==
 * i * 0x210 == 12 * 0x2c, so the inner dimension is exactly 12.
 * The record: ten words at 0x00 and two halfwords at 0x28/0x2a. The ten words
 * are ONE array and not two of five -- the clearing loop runs k = 0..4 storing
 * `unk00[k + 5]` then `unk00[k]` off a SINGLE induction variable
 * (`str r4,[r0,#0x14]; stm r0!,{r4}`). Spelling it as two five-word members
 * folds the 0x14 into the address constant instead, costs a second iv and a
 * stack slot, and does not match (probed, wave 48).
 * Signedness of the words and of unk28/unk2a is UNPROVEN -- the only promoted
 * reader stores zero. sub_0805F0EC, sub_080627F4 and sub_08062AE4 are the
 * three remaining users and are still assembly. */
struct Unk0202DAD8 {
    /* 0x00 */ int unk00[10];
    /* 0x28 */ u16 unk28;
    /* 0x2a */ u16 unk2a;
};
extern struct Unk0202DAD8 gUnknown_0202DAD8[][12];

/* Wave 37, W37-O1. The eight-slot array immediately below gUnknown_0200C528
 * (0x0200C528 - 0x0200C508 == 0x20 == 8 * 4), and aw2bhr.lds names both.
 * Element type is read off two independent sites: sub_08019348 stores its own
 * `const u8 *` parameter -- the one it also hands to sub_080193B0 -- into the
 * first NULL slot, and sub_08019380 loads a slot and passes it straight to
 * sub_080193B0(const u8 *) with no intervening instruction. A producer and a
 * consumer agreeing independently, so the element really is the script pointer
 * and not an opaque word. sub_080191B0 clears all eight with a `str` of 0. */
extern const u8 *gUnknown_0200C508[];

/* Wave 37, W37-K2. A ROM table with TWO-BYTE elements, indexed by
 * gUnknown_03003F40: sub_0806056C hands `&gUnknown_08576900[gUnknown_03003F40]`
 * to sub_080357E0's fifth (void *) parameter and the only scaling is `lsls #1`.
 * The other arm of the same call passes the u8 buffer gUnknown_03003110, so the
 * callee proves nothing about the element type -- only the stride is evidence,
 * and the signedness is unknown. */
extern u16 gUnknown_08576900[];
/* Wave 37, W37-K2. A whole-word counter incremented by sub_0806056C with
 * `ldr; adds #1; str` and NOT re-read after the store, so plain `int` rather
 * than the volatile shape gUnknown_030046D4 has. Signedness unproved. */
extern int gUnknown_03004774;

/* Wave 37, W37-K3. The block at 0x08061788..0x08061E98 carries an agbcc
 * `-fforce-addr` address-constant pool in ROM at 0x0816DB00..0x0816DB0F, and
 * data/data.s currently names the four words gUnknown_0816DB00/04/08/0C. They
 * are NOT globals: dumping baserom.gba gives 0x02029C54, 0x030046B4,
 * 0x03004784 and 0x085D5ABC -- three of which already have real declarations
 * above. So the honest spelling names the object and lets the build place the
 * pool word; nothing should ever declare a gUnknown_0816DBxx. (0x0816DB10 is
 * the same TU's 12-byte `.LC` initialiser that sub_08061E98 `ldm`s onto its
 * stack, three THUMB function pointers into this same block.) */

/* Wave 37, W37-K3. A 0x130-byte record. sub_08061A40 is its whole-struct
 * assignment (`*dst = *src`, 0x130 bytes of byte copy: 0x10 unrolled pairs then
 * a 24-trip loop of 0xc), and sub_08061788 subscripts the ROM table with a
 * `* 0x130` stride synthesised as `((x*4+x)*4-x)*16`. The contents are not
 * modelled -- only the size is proved, by the copy length and the stride
 * agreeing. */
struct Unk085771C4
{
    /* 0x00 */ u8 filler_00[0x130];
};
/* The AI build table: 122 records of 0x130 bytes, selected through
 * gUnknown_0857690C[gUnknown_085C77A0[gPlaySt.mapID]].
 *
 * The body is left as filler ON PURPOSE. The 'aw2aibuild' Nightmare module (a
 * community ROM-editor definition) names 160 fields in it, but NOTHING in this
 * tree reads one, so there is no access to check a layout against and carving
 * the module's offsets in would be asserting a shape on no local evidence.
 * What the module says, for whoever decompiles the AI and can then verify it:
 * a short header -- +0x00 minimum infantry, +0x04 T-Copter, +0x06 APC and
 * +0x07 Lander priority-build flags -- followed by per-unit-type records on a
 * 0xc stride, with the build rates running from +0x1b (infantry, mech,
 * md tank, unit 0x4, tank, recon, APC, neotank, ...) and 5-byte 'value 0x5 to
 * 0x9' runs from +0xec (... B-Copter, T-Copter, battleship, cruiser, lander,
 * sub at +0x128). */
extern const struct Unk085771C4 gUnknown_085771C4[];
/* The RAM destination of the second sub_08061A40 in sub_08061788; one whole
 * record, and the linker script gives it its own symbol. */
extern struct Unk085771C4 gUnknown_02029D84;
/* Wave 37, W37-K3. The scratch copy that sub_08061788 fills and then hands to
 * gUnknown_02029D84 lives at 0x02029C54, which is gUnknown_02029C20 + 0x34 --
 * aw2bhr.lds names NO symbol there, so it must be reached through this one or
 * the split build breaks on an undefined reference (the wave-37 W37-G finding).
 * 0x02029C54 + 0x130 == 0x02029D84 exactly, i.e. the two records are adjacent
 * and the second one is the one the splitter happened to name. Typed `u8 []`
 * because nothing here reads gUnknown_02029C20's own first 0x34 bytes; the two
 * remaining users (sub_08060AB0, sub_08062C94) are still assembly. */
extern u8 gUnknown_02029C20[];

/* Wave 37, W37-K3. Four ROM bytes, indexed by gUnknown_030046B8 with an
 * explicit `<= 3` guard in sub_08061788 (`cmp #3; bhi`) that supplies 4 as the
 * out-of-range fallback -- so the extent 4 is proved by the bound, not guessed.
 * The byte it yields is the same row selector gUnknown_085C77A0.unk27 supplies
 * on the other arm. */
extern const u8 gUnknown_08576908[4];
/* Wave 37, W37-K3. Rows of NINETEEN bytes: sub_08061788 forms
 * `selector * 19 + gPlayers[army].unk1d` (the `(x*4+x)*4-x` synthesis
 * of *19) and `ldrb`s the result, then uses that byte as the subscript of
 * gUnknown_085771C4[]. So it maps (mode, unk1d) to a record id. The row count
 * is unknown; the column count is the one that is proved. */
extern const u8 gUnknown_0857690C[][19];
/* Wave 50, W50-M. The six printf FORMAT STRINGS of the map-editor debug
 * overlay, read straight out of baserom.gba: "UNIT  C0  C1", "PSQ(%1d)",
 * "ASQ(%1d)", "USQ(%1d)", "CPT(%1d)" and "(%1d)". sub_08062DF0 hands each to
 * sub_08013428, whose third parameter is `const char *` and which is VARARGS --
 * so the type is not inferred, it is the declared one.
 *   Their ROM spacing (0x10, 0xc, 0xc, 0xc, 0xc) is exactly agbcc's 4-byte
 * alignment of 13, 9, 9, 9, 9 and 6 bytes, which independently confirms both
 * the reading and each string's extent.
 *   These are NOT the -fforce-addr pool words at 0x0816DB00..0x0816DB1C: the
 * ADDRESS is the argument (`ldr r2,=sym; bl`), never loaded-then-dereferenced.
 *   RECORDED AS A COMPROMISE, not as a model of the original source: the honest
 * spelling is a string literal, the way matched c_080281F0.c spells its
 * "SNOW:%s" siblings, and it compiles to a .rodata section byte-identical to
 * ROM 0x0816DB40..0x0816DB86 -- but tools/trymatch.py rejects it. See the
 * docs/agbcc-codegen.md entry; when that is fixed, switch sub_08062DF0 to
 * literals and delete these six declarations. */
extern const char gUnknown_0816DB40[];
extern const char gUnknown_0816DB50[];
extern const char gUnknown_0816DB5C[];
extern const char gUnknown_0816DB68[];
extern const char gUnknown_0816DB74[];
extern const char gUnknown_0816DB80[];

/* Wave 37, W37-K3. Two ROM tables of nullary function pointers, stepped by the
 * shared cursor gUnknown_03004770: sub_08061B00 runs
 * `table[gUnknown_03004770++]()` and picks the table on
 * gPlaySt.unk0d. `bl _call_via_r0` with no argument register set up,
 * which is the arity readout. gUnknown_085766E8 has 20 slots before
 * gUnknown_08576738 begins; neither extent is otherwise bounded. */
extern void (*const gUnknown_085766E8[])(void);
extern void (*const gUnknown_08576738[])(void);

/* Wave 45, W45-H. A third ROM table of nullary function pointers in the same
 * run, stepped by gUnknown_030040D8->unk07[4]: sub_0805F4CC clamps that byte to
 * 1 when it exceeds 7 and then runs `gUnknown_085768E0[byte]()` --
 * `lsls #2; adds; ldr; bl _call_via_r0` with no argument register set up, which
 * is the arity readout (see the _call_via_rN rule). The clamp bounds the extent
 * at 8 slots but does not prove it, so the extent is left open like its two
 * neighbours above. */
extern void (*const gUnknown_085768E0[])(void);

/* Wave 37, W37-K3. A whole-word accumulator: sub_08061CF8 zeroes it and then
 * adds `sub_08061DA8(n)` into it once per set bit of
 * gPlayers[gUnknown_030033EC].unk2c, with `ldr; adds; str` each time.
 * `int` because that is sub_08061DA8's declared return type; nothing compares
 * it, so the signedness is inherited rather than proved. */
/* Wave 52, W52-B ADDS the volatile, and it is a byte-exact measurement rather
 * than a style choice. sub_08059B4C's `if (a2 > gUnknown_03004788) a2 =
 * gUnknown_03004788;` compiles, non-volatile, to one `ldr` plus `adds r1,r0,#0`
 * -- CSE reuses the compared value for the assignment, and no spelling of that
 * statement (plain `if`, `?:`, a temporary, two divides) stops it. The ROM
 * holds the ADDRESS in r2 and issues a SECOND `ldr r1,[r2]`, which only a
 * volatile read produces. With the volatile the function is byte-for-byte
 * identical; without it, 97.8% with those 4 bytes as the entire residual.
 * Byte-neutral at the only other reader, sub_08061CF8 (src/decomp/c_08061CF8.c),
 * whose `= 0` and four `+= sub_08061DA8(n)` already emit ldr/add/str around a
 * call -- re-verified with trymatch after this edit. */
extern volatile int gUnknown_03004788;


/* Wave 37, W37-O2. The gUnknown_0200C528 script-command DISPATCH TABLE.
 * sub_08019404 indexes it with the current node's byte 0
 * (`gUnknown_0200C528[a].unk04->filler_00[0]`), loads a word and calls it
 * through `bl _call_via_r1` with the sign-extended slot index in r0, looping
 * while the result tests nonzero after `lsls #0x10`. The twelve entries read
 * out of baserom.gba as sub_08017A80, sub_08017A58, sub_08017B50 x3,
 * sub_08017B60, sub_08017B64, sub_08017B8C, sub_08017BB0, sub_08017BD4,
 * sub_08017BFC, sub_08017C24 -- exactly the `s16 (s16)` handler family
 * unknown-functions.h documents beside sub_08017A80, which is what fixes the
 * element type. ROM data, so `const`; the extent is unbounded because only the
 * indexed load reaches it. */
extern s16 (*const gUnknown_0848A244[])(s16);

/* Wave 37, W37-K3, and this is the `c_local` workaround, not a model of the
 * source: the two words below are the SAME `-fforce-addr` pool block described
 * beside gUnknown_085771C4 above, holding &gUnknown_03004784 and
 * gUnknown_085D5ABC. The honest spelling does not reproduce them in
 * sub_08061DCC and the measured reason is new: force-addr's reroute is decided
 * AFTER CSE has merged the references, and a POINTER global read twice around a
 * `bl __divsi3` collapses to ONE reference because a libcall does not clobber
 * memory. The honest draft then loses the ROM's recompute-per-statement as
 * well. Naming the pool words gives back both, and the relocation is checkable
 * because data/data.s really does define these symbols. Do NOT read them as
 * evidence that objects live at 0x0816DB08 / 0x0816DB0C.
 * The `volatile` is likewise a reproduction device and NOT a claim about the
 * object: it is what restores the ROM's re-dereference at every use (+8 bytes
 * over the plain declaration, measured this wave). `const` changes nothing
 * either way, as the -fforce-addr chapter of docs/agbcc-codegen.md records.
 * Only sub_08061DCC's parked draft names these two; nothing else should. */
extern u8 **volatile gUnknown_0816DB08;
extern struct UnitType *volatile gUnknown_0816DB0C;


/* Wave 37, W37-O2. The 0x0808E59C..0x0808E5B8 run is agbcc's `-fforce-addr`
 * ADDRESS-CONSTANT POOL for the 0x08017xxx-0x08019xxx block, NOT a table of
 * globals. Dereferenced in baserom.gba:
 *
 *   0x0808E59C -> 0x0200C528   &gUnknown_0200C528
 *   0x0808E5A0 -> 0x0200C528   &gUnknown_0200C528   (a second private copy)
 *   0x0808E5A4 -> 0x03002EF0   &gUnknown_03002EF0   (sub_08019470)
 *   0x0808E5A8 -> 0x03002EE0   &gpKeySt
 *   0x0808E5AC -> 0x030033EC   &gUnknown_030033EC   (sub_080196F4)
 *   0x0808E5B0 -> 0x03003F2C   &gUnknown_03003F2C   (sub_080196F4)
 *   0x0808E5B4 -> 0x08499598   &gPlayers   (sub_08019940)
 *   0x0808E5B8 -> 0x03002EE0   &gpKeySt             (a second private copy)
 *
 * Two words holding the SAME address is the giveaway -- -fforce-addr gives
 * each function its own copy. Write the honest spelling (`gUnknown_030033EC`,
 * `gpKeySt->held`, ...) and let agbcc emit the word into the unit's own
 * `.rodata`; trymatch then reports `name different symbols that resolve to
 * the same address` and prints the address promotion must carry. Contrast
 * gUnknown_08499588/8C/98 at 0x08499xxx, which ARE real globals -- same
 * block, opposite answers, so dereference the candidate in baserom.gba
 * before deciding.
 *
 * Note the SECOND indirection this creates for gPlayers, which is
 * itself a pointer: sub_08019940 reads `ldr r0,[pool]` (= 0x08499598) then
 * `ldr r1,[r0]` (= the array base), both INSIDE its loop, and that is one
 * plain `gPlayers[i]` subscript, not two source dereferences. */

/* ---- wave 37 (W37-Q4): the 0x080439A8-0x08043F92 run ---- */

/* Two more of the 8-byte PutSprite object-list blobs packed through
 * 0x084A0024..0x084A0069 (see gUnknown_084A0052/gUnknown_084A005A above).
 * sub_080439A8 picks between them on its sixth argument and hands the chosen
 * one to PutSprite's `u16 *` fourth parameter with no arithmetic, so `u16 []`
 * exactly like its neighbours. Each holds a leading count of 1 followed by one
 * three-halfword OAM entry, which is what fixes the 8-byte stride. */
extern u16 gUnknown_084A0042[];
extern u16 gUnknown_084A004A[];
/* A 32-entry byte table filling 0x084A006E..0x084A008D, i.e. right up to
 * gUnknown_084A0090. sub_080439A8 subscripts it with `DivRem(x, 0x20)`, which
 * is what makes the extent exactly 32, and reads it with a bare `ldrb` whose
 * result is SUBTRACTED from a y coordinate -- a per-frame vertical bob offset.
 * Values are 06 00 01 01 02 02 02 01 01 and then zeros. */
extern u8 gUnknown_084A006E[];
/* Nineteen bytes at 0x084A077C, ending exactly where gUnknown_084A0790 begins:
 * `00 01 02 04 0f 03 05 10 06 07 12 08 09 11 0b 0c 0d 0e 0a`, a permutation of
 * 0x00..0x12. sub_08043CA0 walks i = 0..0x12 over it and feeds each byte to
 * sub_0803CAB8(u32) -- a CO display order, filtered down to the unlocked ones.
 * Reached through the 0x08091378 force-addr word, not by a direct pool `ldr`. */
extern u8 gUnknown_084A077C[];
/* The 0x180-byte (192 halfword) EWRAM staging buffer sub_08043E8C fills with
 * sub_08011C68 and then rewrites nibble by nibble into its caller's tile
 * buffer, 12 rows of 16 halfwords. `u16 []` from the `ldrh`/`adds r6,#2` walk;
 * 0x180 is both sub_08011C68's explicit byte count and 12*16*2, which is what
 * bounds it. */
extern u16 gUnknown_02017C50[];

/* Wave 41 (W41-A): the window-frame painter's four tile tables. sub_0801A240
 * indexes each with `lsls #0x10; asrs #0xf` on its s16 `kind` argument, i.e.
 * a 2-byte stride, and `ldrh`s the result -- so `u16 []`. data/data.s gives
 * 0848A46C/70/74 exactly 4 bytes each, so each table is two entries, and
 * sub_0801A474 only ever passes kind = 0 or 1. gUnknown_0848A478's run in
 * data.s is longer only because nothing after it is named yet. */
extern const u16 gUnknown_0848A46C[];
extern const u16 gUnknown_0848A470[];
extern const u16 gUnknown_0848A474[];
extern const u16 gUnknown_0848A478[];
/* Wave 41 (W41-A): the 4-byte template sub_0801A2E4 memcpy's onto its stack
 * and then reads as two alternating u16 tilemap entries -- so `u16 [2]`, and
 * 4 bytes is exactly the size data/rodata.s gives it. */
extern const u16 gUnknown_0808E5C0[];
/* Wave 41 (W41-A): 0x0808E5C4 IS NOT AN OBJECT and must not be declared as
 * one. It is agbcc's own -fforce-addr word for `&gUnknown_0849958C` (the ROM
 * word dereferences to 0x0849958C in baserom.gba), which is why sub_0801A474's
 * `ldr r0,[r0]` appears twice: one level is the force-addr slot, the other is
 * the pointer global's own read. MEASURED: declaring it `u16 **` and writing
 * `**gUnknown_0808E5C4` reproduces the instruction stream exactly and still
 * misses, because the honest name `gUnknown_0849958C` also creates its allocno
 * at the top of the function and that is what fixes the register allocation.
 * Write the global; trymatch places the pool word (rodata 0x0808E5C4).
 * Same for 0x0808EF58 in sub_0801ABF8 -- it holds &gUnknown_0200CC2C. */

/* Wave 42 (W42-C). The five-entry (dx, dy) direction table sub_08010ADC walks,
 * dereferenced in baserom.gba rather than inferred:
 *   0x0848897C: { 0, -1, 1, 0, 0 }   (dx)
 *   0x08488986: { 0,  0, 0, -1, 1 }  (dy)
 * so index 0 is the cell itself and 1..4 are left, right, up and down. They are
 * TWO separate arrays, not one 2x5 table: sub_08010ADC materialises dy through
 * its own inline pool word rather than indexing off dx's base register, which a
 * `tbl[1][i]` spelling cannot produce. s16 is measured, not assumed -- both are
 * read with `ldrsh` and the -1 entries need the sign.
 *
 * 0x0808D8A0 IS NOT AN OBJECT: it is agbcc's own -fforce-addr word holding
 * 0x0848897C, one slot before the 0x0808D8A4/0x0808D8A8 pair c_08010B34.c
 * already documents for &gUnknown_08499590. Naming gUnknown_0848897C honestly
 * reproduces sub_08010ADC's two-level load and promotion carries the rodata
 * entry. NOTE FOR PROMOTION: data/data.s currently swallows 0x0848897C inside
 * gUnknown_08488974's 0x12-byte blob, so that blob must be split at 0x0848897C
 * to give the symbol somewhere to live. */
extern const s16 gUnknown_0848897C[];
extern const s16 gUnknown_08488986[];
/* Wave 42 (W42-C). Two u16s at 0x0300308C: the base tile ids for the digit and
 * letter glyph runs, added to a character code to form a tilemap entry.
 * sub_08010F38 reads both (`ldrh [r4]` at +0 for characters <= 0x40, i.e.
 * digits, and `ldrh [r4,#2]` at +2 for letters); sub_08010EF8 reads +0 only.
 * NOT a pointer -- both are plain ldrh on the symbol's own address, and the
 * linker script already places it (aw2bhr.lds: `. = 0x00308C;`).
 * 0x0808DF8C is the -fforce-addr word holding 0x0300308C, not an object: its
 * neighbours at +4 and +8 BOTH hold 0x03001408, and one word per
 * (function, symbol) pair is what marks the whole run as address constants.
 * sub_08010EF8 (DrawNumberRightAligned) reaches it through that word with the
 * honest `value % 10 + gUnknown_0300308C[0]`; see src/decomp/c_08010EF8.c. */
extern u16 gUnknown_0300308C[];
/* Wave 43, W43-K -- blocks 0x0804A / 0x0804B.
 *
 * gUnknown_084C3D12 is a single ROM byte (0x20) read with a bare `ldrb` off a
 * plain address and compared for equality against gUnknown_030044E0->unk2c[i]
 * in sub_0804A6D8, so it is a u8 sentinel for that array's element type.
 * Nothing signs it. It sits immediately before the already-declared
 * gUnknown_084C3D14 table and is declared non-const to match its neighbours;
 * sub_0804A6D8's loop stores nothing, so const/non-const is byte-neutral here
 * and the choice is NOT evidence.
 *
 * gUnknown_084C398C is a proc script -- sub_0804A6D8's `ldr r0,=...; bl
 * sub_080193B0`, the same shape and the same declared `const u8 *` parameter
 * as gUnknown_084C38BC two lines of C away in sub_0804AE10.
 *
 * gUnknown_085519FC is a POINTER VARIABLE in ROM holding 0x02016450, and it is
 * NOT const: sub_0804B850 and sub_0804BB74 both re-load the pointer VALUE on
 * every iteration of a loop that stores through it (`ldr r1,[r5]; strh
 * r3,[r0]`), which is the aliasing reload a const pointer would have let LICM
 * hoist. Its address additionally appears as TWO private -fforce-addr .rodata
 * words, 0x08136040 (sub_0804BAC4) and 0x08136044 (sub_0804BB74), both holding
 * 0x085519FC -- one copy per function, exactly as the literal-pool chapter
 * predicts. Do not mistake either for a separate global.
 *
 * gUnknown_02028E64 is the 64-halfword scratch buffer sub_0804BD58 fills and
 * then hands to CpuFastSet. The ROM reaches it through the -fforce-addr word
 * at 0x08136048 (dumped from baserom.gba). */
extern u8 gUnknown_084C3D12;
extern const u8 gUnknown_084C398C[];
/* Wave 55, W55-H. A proc script: sub_0804A760 hands it to sub_080193B0 on the
 * arm that sets gUnknown_030044E0->unk63 = 3, exactly the shape and the
 * declared `const u8 *` parameter of gUnknown_084C398C two entries up. */
extern const u8 gUnknown_084C3A5C[];
extern u16 *gUnknown_085519FC;
/* NO EXTERN CAN EXIST FOR 0x02028E64: upstream's aw2bhr.lds IWRAM table jumps
 * 0x028E5C -> 0x029664 and gen_lds.py passes it through verbatim, so the symbol
 * is unlinkable in the split build. Spell accesses as an offset into the
 * covering symbol instead -- `(u16 *)((u8 *)gUnknown_02028E5C + 8)` -- which
 * relocates as gUnknown_02028E5C+8 and resolves to the same address and pool
 * word. See work/sub_0804BD58/sub_0804BD58.c. Wave 43. */

/* Nothing lives at 0x081D9440 or 0x081D9444. The two words there are the
 * address constants the compiler emits for sub_08087040's two sprite-data
 * arguments, and they hold the addresses of gUnknown_0848B690 and
 * gUnknown_0848B6A8. That function names both arrays directly and the build
 * places the words, so neither address needs -- or may have -- a declaration:
 * reading one as a pointer variable adds a load at every use. The word at
 * 0x081D9328 is the same thing for gUnknown_0848B6C6, a few entries below. */
/* Wave 44, W44-D. A table of 0x5c-byte records in ROM, indexed by a
 * gUnknown_02027F74.unk04[] unit id. sub_08087B74 reads
 * `gUnknown_085C77DC[id].unk01[i]` with a single `ldrb` and hands the byte to
 * sub_08043FA8's `int` first parameter.
 *
 * SETTLED IN WAVE 46 (W46-C), AND sub_08087B74 IS MATCHED. The 0x5c stride and
 * the byte at +1 were always solid; what was open was where the `+ 1` belongs,
 * and the answer is NEITHER spelling tried in wave 44. sub_08087B74 reaches the
 * table FLAT, as `((u8 *)gUnknown_085C77DC)[i + k]`, with the record offset
 * bound to a local in a statement of its own (`k = id * 0x5c + 1;`). That
 * statement is the whole trick: it stops fold re-associating the `+ 1` past the
 * loop counter, so the constant stays a runtime `adds r0, #1` against a CLEAN
 * pool word instead of migrating into the word's ADDEND
 * (`.word gUnknown_085C77DC+0x1`) or into the load displacement
 * (`ldrb r0, [r0, #1]`). Written inline in ANY association it does one or the
 * other -- that is what parked the function -- and the flat form only cost the
 * 0xc-stride giv because the `+ 1` was still inline to be folded into the biv's
 * own update. `k` is dead at the end of each iteration, so it costs no
 * register. The struct below is therefore NOT how the one known reader spells
 * the access; it is kept only as a description of the record's layout, and
 * nothing else names this symbol.
 *
 * NOTE the splitter also emits gUnknown_085C77E0 four bytes later -- that is
 * inside this table, not a separate object; the pool word here is clean
 * `.4byte gUnknown_085C77DC`. Only unk01 is evidenced; the extent is unproved. */
struct Unk085C77DC /* 0x5c */
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01[0x5b];
};
/* WAVE 54 (W54-F): THIS SYMBOL IS THE SAME TABLE AS gUnknown_085C77A0, 0x3c
 * BYTES IN, and struct Unk085C77A0 above is the real record. 0x085C77A0 +
 * 0x3c == 0x085C77DC, both are walked with a 0x5c stride, and the fields the
 * two models describe line up exactly: what this struct calls unk01[i] is
 * `gUnknown_085C77A0[id].unk3c[i + 1]`, and gUnknown_085C77E0 -- which the
 * splitter emits four bytes further on -- is `gUnknown_085C77A0[id].unk40`,
 * reached by sub_08077A14. That is the W53-A rule (two globals whose address
 * difference the ROM folds are one symbol) with the ROM folding it into a pool
 * word's addend. NOT unified here because c_08087B74.c is matched against this
 * spelling and its own note records exactly which association survives; the
 * unification is a separate, verifiable change and should be done as one. */
extern struct Unk085C77DC gUnknown_085C77DC[];
/* Wave 44, W44-D. A 16-colour palette (0x20 bytes in data-08581E70.s) which
 * sub_08087B74 hands to ApplyPaletteExt(u16 *, u32, u16) for 0x20 bytes with no
 * arithmetic, so `u16 []` and non-const on the usual grounds -- ApplyPaletteExt
 * takes a plain pointer. */
extern u16 gUnknown_08614238[];
/* Wave 44, W44-G. The SRAM/backup-media descriptor, in ROM (data/data.s gives
 * it 0x128 bytes at 0x08485550). Only three functions in the ROM read it and
 * all three are the self-relocating SRAM trampolines sub_0808AE54 /
 * sub_0808AF00 / sub_0808AF74, so only two members are evidenced:
 *
 *   +0x18  u16, `ldrh`  -- the default transfer length sub_0808AF00 hands to
 *                          sub_0808AED0 as its byte count (sub_0808AF74 is the
 *                          same call with a caller-supplied length instead).
 *   +0x1c  u8,  `ldrb`  -- a SHIFT AMOUNT: all three do `lsls r4, rN` with it
 *                          on the u16 first parameter and add 0x0E000000, so
 *                          the parameter is a bank/sector index and this is
 *                          log2 of the bank size.
 *
 * The extent (0x128 bytes) is the splitter's, not measured; nothing here says
 * this is one object rather than a header followed by a table. Not `const`:
 * nothing proves it, and the reads are `ldrb`/`ldrh` through a pool word
 * either way. */
struct Unk08485550
{
    /* 0x00 */ u8 unk00[0x18];
    /* 0x18 */ u16 unk18;
    /* 0x1a */ u8 unk1a[2];
    /* 0x1c */ u8 unk1c;
};
extern struct Unk08485550 gUnknown_08485550;

/* Wave 46 (W46-A). An 8-byte-record ROM table that sub_08073228 LINEAR-SEARCHES
 * with no bound: for each byte of its source string it walks records until
 * `unk00 == c`, so the table is terminated by whatever key the strings can
 * contain and the extent is not pinned. `unk04` is then added to a u16
 * accumulator that becomes a per-character byte written into the proc.
 *
 * gUnknown_08614028 is NOT a second object -- it is this table's `unk04`
 * member. agbcc folds the +4 into the address constant, so the splitter
 * invented a symbol at 0x08614028 and lists it as a separate data_ref. */
struct Unk08614024
{
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 filler_01[3];
    /* 0x04 */ int unk04;
};
extern const struct Unk08614024 gUnknown_08614024[];

/* Wave 46 (W46-A). 28 halfwords sub_08073658 copies into
 * gUnknown_0202F8DC[0x84 .. 0x9F] -- the bottom 28 entries of the 0xA0-entry
 * HBlank scanline table that DMA0 then feeds to REG_BLDY. Halfword stride is
 * from the `ldrh`; the count is the `cmp #0x1b; ble` bound. */
extern const u16 gUnknown_08614154[];

/* Wave 55 (W55-A). 0x086141FC is a LONE ROM WORD whose contents are 0x03004790,
 * i.e. &gUnknown_03004790, the one struct SoundInfo. sub_08073E0C -- the PCM
 * oscilloscope that drives REG_WIN1H per scanline -- reads it as
 * `ldr rN, =gUnknown_086141FC; ldr rM, [rN]` and then adds 0x350 and 0x630 AT
 * RUNTIME (`movs #0xd4; lsls #2` and `movs #0xc6; lsls #3`) to reach
 * SoundInfo::pcmBuffer.
 *
 * That runtime addition is what says it is a POINTER OBJECT and not one of
 * agbcc's `-fforce-addr` address words (0x081CC030/34 next door are those):
 * naming gUnknown_03004790 directly folds the 0x350/0x980 into the address
 * constant and emits `.word gUnknown_03004790+0x980` in the literal pool with
 * no `movs/lsls` pair at all -- measured, both spellings probed. Under
 * -fforce-addr agbcc builds the .rodata word only for a STORE destination; a
 * plain LOAD of a global takes the direct literal-pool path (visible twice in
 * this same function for gUnknown_0202FDE4). So the original source named a
 * pointer here.
 *
 * Not const-qualified: the pointer is re-loaded from memory on every loop
 * iteration because the halfword store through gUnknown_0202FDE4 may alias it,
 * and a `const` would let agbcc hoist the load out of the loop. */
extern struct SoundInfo *gUnknown_086141FC;

/* Wave 47 (W47-G). 0x085768B8, 0x28 bytes = TEN function pointers.
 * sub_0805BF3C selects an entry with `lsls #2` off a u8 index and calls it
 * through `_call_via_r3` with three arguments (its own x, y and out-pointer),
 * so the element type is a pointer to a three-argument function. Both call
 * sites in that function pass the same triple, and the second one indexes with
 * a byte taken out of a 25-byte stack table, which is what fixes the extent at
 * ten rather than at whatever the largest constant index happens to be.
 * Not const-qualified: the ROM section it lives in is shared with the
 * gUnknown_085768E0 / gUnknown_08576900 tables and nothing has proved it. */
extern void (*gUnknown_085768B8[])(int, int, u16 *);

/* Wave 50, W50-D. sub_080324C4's screen assets. The two palettes are the two
 * arms of one `if` and go to ApplyPaletteExt(u16 *, 0, 0x20); the three
 * compressed blobs go to Decompress(u8 *, void *). Not const, on the same
 * model as every other Decompress/ApplyPaletteExt source in this header --
 * the parameter types are non-const and nothing has proved the section. */
extern u16 gUnknown_081D8A14[];
extern u16 gUnknown_081D8A34[];
extern u8 gUnknown_081D3EE8[];
extern u8 gUnknown_081D6458[];
extern u8 gUnknown_081D2660[];
/* Wave 50, W50-D. sub_08032BCC's tile blob (its palette gUnknown_081D2224 is
 * already declared above). */
extern u8 gUnknown_081D2554[];
/* Wave 50, W50-D. Argument 1 of sub_08073304(const void *, void *, ...) in
 * sub_080324C4, i.e. the same slot as gUnknown_085802F0 / gUnknown_085826E0,
 * both of which this header already models as `u8 []`. */
extern u8 gUnknown_0849B644[];
/* Wave 50, W50-D. The link-lobby child proc's script: sub_08031638 hands it to
 * Proc_EndEach(const struct ProcCmd *), and work/sub_080312AC/NOTES.md reads
 * the same symbol going into Proc_Start there. */
extern const struct ProcCmd gUnknown_0849B1A0[];
/* Wave 50, W50-D. sub_080312AC's five lobby tables. Every one of them is
 * INDEXED by a computed subscript before being dereferenced, which is the
 * wave-49 inverse of the pool-word tell, so none of them is a -fforce-addr
 * address constant. The width each access fixes:
 *   gUnknown_0849B0C4  `ldrb` at `x % 30`, subtracted from 0x58  -> u8, >= 30
 *   gUnknown_0849B258  `ldr` at i*4, i = 0..3, straight into
 *                      PutSpriteExt's `u16 *` argument           -> u16 *[4]
 *   gUnknown_0849B268  same, but indexed by the RUNNING COUNT    -> u16 *[5]
 *   gUnknown_0849B27C  `ldrh` at i*2, i = 0..3                   -> u16 [4]
 *   gUnknown_081D3E68  `ldrh` at k*2, k = ((x >> 1) & 0xf)       -> u16 [16] */
extern u8 gUnknown_0849B0C4[];
extern u16 *gUnknown_0849B258[];
extern u16 *gUnknown_0849B268[];
extern u16 gUnknown_0849B27C[];
extern u16 gUnknown_081D3E68[];
/* Wave 50, W50-D. NOT a -fforce-addr pool word: baserom.gba holds FOUR
 * consecutive ROM pointers here (0x086103F8, 0x08610408, 0x0861041C,
 * 0x08610440), none of them a named symbol, so this is a real string table.
 * sub_08032BCC only ever reads element 0 (`ldr r1,[r0]`, no index) and hands
 * it to sub_0808B678(char *, const char *) as the source, i.e. a default
 * player name. Extent 4 is from the ROM run, not from a reader. */
extern const char *gUnknown_08613CDC[];

/* Wave 51, W51-O. sub_080790D0's three unnamed operands, all reached by a bare
 * `ldr rN, =sym` and used as the symbol itself with no dereference, so each is
 * the object rather than a pointer to one.
 *   08227F3C  handed straight to `Decompress(u8 *, void *)` with gUnknown_0200FC50
 *             as the destination, so `u8 []` and NOT const (Decompress's first
 *             parameter is non-const) -- the same model gUnknown_08234B10 carries.
 *   0822AA80  handed straight to `ApplyPaletteExt(u16 *, u32, u16)` for 0x40
 *             bytes at palette word 0x280 with no arithmetic, so `u16 []` and
 *             non-const for the same reason.
 *   02012790  an EWRAM source buffer: two `CpuFastSet(const void *, ...)` calls
 *             read it at +0 and +0x400 (0x10 words each) into 0x06015F00 /
 *             0x06015F40. Byte-addressed (`adds r4, r4, #0x400` off the symbol),
 *             so `u8 []`; extent unpinned. It sits 0x2B40 past gUnknown_0200FC50
 *             but the ROM holds its own pool word for the address rather than
 *             adding a displacement, which is what makes it a separate symbol. */
extern u8 gUnknown_08227F3C[];
extern u16 gUnknown_0822AA80[];
extern u8 gUnknown_02012790[];

/* Wave 53, W53-A. sub_08047C04's art set. Types are read straight off the
 * callee each symbol is handed to, with no arithmetic in between except where
 * noted:
 *   0812A2AC  sub_08011C68(const void *, void *, u16)'s source for 0x80 units
 *             into 0x06013940, by name with no arithmetic.
 *   0812A8C8  sub_08071948(u16 *, int, int, const void *, u16)'s `const void *`
 *             source blob. Its first two bytes are 0x1b/0x13, i.e. the
 *             (width-1, height-1) header that function reads.
 *   0823E140  `Decompress(u8 *, void *)`'s source, into gUnknown_0200FC50.
 *   0823FFA8  the same, into gUnknown_0200FC50 a second time.
 *   0849F658  indexed `lsls #1; ldrh` by gPlayers[].unk1a, so `u16 []`;
 *             the halfword it yields is then the subscript of gTextTable.
 *   084C3F38  sub_080149C0's `u8 *` fourth parameter, passed by name.
 * None can be `const`: Decompress, sub_08011C68 and sub_080149C0 all take
 * non-const pointers and -Werror rejects the qualifier being dropped. Note that
 * 0812A2AC and 0812A8C8 are REAL DATA even though the 0x0812A2xx run around
 * them holds this unit's `-fforce-addr` .rodata words (0812A274/78/7C,
 * 0812A290/94/98/9C) -- the ROM content settles it, theirs is not an address. */
/* Wave 53, W53-A: gUnknown_0200FE50 is NOT a symbol -- it is
 * gUnknown_0200FC50 + 0x200, and gUnknown_02010450 is gUnknown_0200FC50 + 0x800.
 * sub_08047C04 settles it: it reaches gUnknown_0200FC50 with
 * `ldr r2, =0xFFFFF800; adds r4, r4, r2` off the gUnknown_02010450 already in
 * r4, and agbcc can only fold the difference of two address constants when they
 * are the SAME symbol. Every offset that function touches then lands on a clean
 * 0x400 front/back pairing (0x100/0x500, 0x200/0x600, 0x240/0x640, 0x2c0/0x6c0,
 * 0x800/0xc00, 0x880/0xc80), which three separate buffers do not explain.
 * gUnknown_02010450 and gUnknown_0200FE50 are both left declared -- matched
 * functions already use those spellings and all three addresses are bound in
 * aw2bhr.lds, so the emitted word is identical either way and try_match reports
 * it as "different symbols that resolve to the same address". The note on
 * gUnknown_0200FE50 at the end of this file says the same thing from the other
 * side: sub_08085B30 Decompresses into it at -0x200, i.e. into
 * gUnknown_0200FC50, and "the declared symbol is the middle of a larger
 * buffer". Prefer `gUnknown_0200FC50 + 0x200` in new work; it is the only
 * spelling that lets agbcc fold the difference. */
extern u8 gUnknown_0812A2AC[];
extern u8 gUnknown_0812A8C8[];
extern u8 gUnknown_0823FFA8[];
extern u16 gUnknown_0849F658[];
/* Wave 56, W56-G. A FOUR-BYTE ROM TEMPLATE, not an agbcc -fforce-addr address
 * constant, even though it sits in the middle of the 0x0812A1xx pool run whose
 * neighbours 0x0812A130/134/138 are exactly that. The word is 0x00010000, i.e.
 * the two u16s { 0, 1 }, and sub_08047920 copies it onto its own stack with
 * `sub_0808B6E8(pal, gUnknown_0812A12C, 4)` before indexing it by a one-bit
 * flag -- the same hand-written-memcpy idiom gUnknown_0816DB20 records.
 *
 * A `u16 pal[2] = {0, 1};` local initialiser is byte-identical EXCEPT for the
 * relocation: agbcc emits its own .rodata template and calls `memcpy`, which is
 * not a symbol in this tree, where the ROM relocates against sub_0808B6E8. The
 * reloc is the whole tell, so spell the copy by hand. */
extern const u8 gUnknown_0812A12C[4];
extern u8 gUnknown_084C3F38[];
/* Wave 56, W56-G. sub_08047920's second sub_080149C0 label source, and the
 * exact sibling of gUnknown_084C3F38 four bytes below it: both are passed BY
 * NAME as that function's `u8 *` fourth argument with no arithmetic, from the
 * same panel row. `u8 []` for the same reason gUnknown_084C3F38 carries it. */
extern u8 gUnknown_084C3F3C[];

/* Wave 53, W53-A. sub_0804A260's art set, same reasoning:
 *   0812AD2C/38/44/50  four sub_08071948 `const void *` source blobs, each with
 *             the (width-1, height-1) byte header that function reads out of
 *             its first word (0x0812AD50's is 0x12/0x0d, a 19x14 panel; the
 *             other three are 1x4 edge pieces).
 *   0812B21C  ApplyPaletteExt(u16 *, u32, u16)'s first parameter, handed over
 *             seven times at 0x20 bytes each with no arithmetic, so `u16 []`.
 *   084C3B2C  a table of `Decompress(u8 *, void *)` sources: `ldr r0,[base +
 *             gUnknown_02028E40 * 4]` and the loaded word goes straight to
 *             Decompress, so an array of pointers rather than of data -- the
 *             same model as its neighbour gUnknown_084C3B3C, and `const` on the
 *             pointers for the same reason.
 *   084C3D1C  sub_08012C58(void *)'s argument, passed by name. Modelled `u8 []`
 *             to match gUnknown_0849D16C, which is what almost every other
 *             caller of that function passes; its first two words are VRAM
 *             addresses 0x06000000 and 0x06007000. */
extern u8 gUnknown_0812AD2C[];
extern u8 gUnknown_0812AD38[];
extern u8 gUnknown_0812AD44[];
extern u8 gUnknown_0812AD50[];
extern u16 gUnknown_0812B21C[];
extern u8 *const gUnknown_084C3B2C[];
extern u8 gUnknown_084C3D1C[];

/* WAVE 53 (W53-C). The globals of the sub_0807C614 / sub_08081060 /
 * sub_08085B30 screen-setup cluster. Every one of these was undeclared; none of
 * them is bound under a real name in aw2bhr.lds (checked before declaring).
 *
 * NO gUnknown_081D93xx SYMBOL IS DECLARED HERE, AND NONE MAY BE. The whole
 * 0x081D92D8-0x081D940F run is this translation unit's -fforce-addr
 * address-constant pool -- the same fact c_08078D40.c already records for
 * 081D92D8/DC/E0 -- and the splitter invents a `gUnknown_081D93xx` for every
 * word of it, which reads exactly like a ROM-resident pointer variable. It is
 * not one. Dereference the word in baserom.gba and name what it points at:
 *   081D933C -> 0x030058E0  gUnknown_030058E0[]  (sub_0807C614)
 *   081D9398 -> 0x0861696C  gUnknown_0861696C[]  (sub_08081060)
 *   081D939C -> 0x03005934  gUnknown_03005934    (already noted above)
 *   081D93FC -> 0x03005928  gUnknown_03005928    (sub_08085B30)
 *   081D9400 -> 0x0300596C  gUnknown_0300596C
 *   081D9404 -> 0x03005990  gUnknown_03005990[]
 *   081D9408 -> 0x03005980  gUnknown_03005980
 * Wave 54 (W54-A) confirmed this independently on five more words of the same
 * run, which is the whole 0x0807Axxx-0x0807Bxxx block's share of the pool:
 *   081D931C -> 0x0202FDEC  gUnknown_0202FDEC   (sub_0807AE94)
 *   081D9320 -> 0x0202FDEC  the SAME address again (sub_0807B148) -- two words
 *               of the pool holding one address, which is -fforce-addr giving
 *               each function its own private copy, exactly as the brief's
 *               "several pool words in a run can hold the SAME address" says.
 *   081D9324 -> 0x0848B6C6  (sub_0807B574)
 *   081D932C -> 0x030030B4  gUnknown_030030B4   (sub_0807BA90)
 *   081D9330 -> 0x0300592C                      (sub_0807BA90)
 * sub_0807B148 is the sharpest demonstration in the tree that these are pool
 * words and not variables: it reads gUnknown_0202FDEC.unk08 in BOTH arms of one
 * `if`, and the arm with several reads goes through the 081D9320 pool word
 * (`ldr; ldr; ldrb`) while the arm with one read gets a plain `ldr rN,
 * =gUnknown_0202FDEC` (`ldr; ldrb`). One global, one honest spelling, two
 * different instruction sequences chosen by agbcc -- so the two-level read is
 * provably a property of the reference count, not of the symbol's type.
 * All seven were already declared here with exactly the types the pool-word
 * contents imply, so the honest spelling needs no new symbol at all -- it needs
 * one fewer `ldr` than the pointer-variable reading, which is what gives the
 * two readings different bytes and settles it. The tell in the candidate is an
 * extra indirection at every use: agbcc emits `.LCn: .word <real global>` in
 * this unit's own .rodata and `ldr rN, =.LCn`, so a draft that declares
 * `u8 *gUnknown_081D933C` produces `ldr; ldr; ldr` where the ROM has `ldr; ldr`.
 *
 * The ROM blobs below are all first arguments of `Decompress(u8 *, void *)` or
 * of `ApplyPaletteExt(u16 *, u32, u16)`, passed by name with no arithmetic, and
 * are typed and left non-const on exactly the grounds recorded for
 * gUnknown_08234B10 and gUnknown_0812B21C above. Extents are unpinned. */
extern u8 gUnknown_0823456C[];
extern u8 gUnknown_0823468C[];
extern u8 gUnknown_082346D0[];
extern u16 gUnknown_08234AD0[];
extern u8 gUnknown_0823BE40[];
extern u8 gUnknown_0823BF28[];
extern u16 gUnknown_0823BFD4[];
extern u16 gUnknown_0823DDB8[];
extern u8 gUnknown_0823E140[];
/* Wave 60, W60-E. Six more of the 0x0823xxxx screen-setup blobs, all read off
 * sub_080489CC and typed by the CALLEE each is handed to rather than by their
 * contents -- the same discipline the gUnknown_0823E140 / gUnknown_0823E550
 * pair above already carries.
 *   Decompress's `u8 *` source: gUnknown_0823E7D4 (into gUnknown_08499580),
 *   gUnknown_0823EA40 and gUnknown_0823E8E8 (both into gUnknown_0200FC50).
 *   ApplyPaletteExt's `u16 *` first parameter with a 0x20-byte third argument,
 *   i.e. one 16-entry palette: gUnknown_0823BE00, loaded to palette offset 0.
 *   gUnknown_0823FB7C is a TABLE of those palettes, not one: sub_080489CC
 *   indexes it `lsls #5` off sub_08017860(0xf)'s result and still asks for
 *   0x20 bytes, so the row is 16 u16 and the two-dimensional spelling is what
 *   reproduces the byte stride. Its extent is not proved -- sub_08017860
 *   returns `a % 24`, so 24 rows is the floor its one caller implies.
 * None is marked const: ApplyPaletteExt and Decompress both take plain
 * pointers, and nothing here discriminates. */
extern u8 gUnknown_0823E7D4[];
extern u8 gUnknown_0823E8E8[];
extern u8 gUnknown_0823EA40[];
extern u16 gUnknown_0823BE00[16];
extern u16 gUnknown_0823FB7C[][16];
/* Wave 60, W60-E. sub_080489CC's only use is as sub_08073304's first argument,
 * whose parameter is `const void *` -- a proc script blob, the same role every
 * other first argument of that function plays. Typed `const u8 []` to match the
 * parameter without claiming a layout; nothing reads it element-wise yet. */
extern const u8 gUnknown_085802B4[];
extern u8 gUnknown_0823E654[];
extern u16 gUnknown_084892EC[];
/* Wave 55, W55-I. A 16-entry u16 template sub_08012C58 copies to the first
 * 16 halfwords of all four BG tilemaps at once (`p0[i] = p1[i] = p2[i] =
 * p3[i] = gUnknown_08489334[i]` over i = 0..0xf, plain `ldrh` at `i * 2`).
 * The extent is exact: data/data.s gives it 0x20 bytes, 0x08489334..0x08489354.
 * NOT const, on the same model as gUnknown_084892EC above -- nothing here
 * proves it either way and the neighbours in this run are non-const. */
extern u16 gUnknown_08489334[];
/* sub_08073304(const void *, ...)'s first argument; the note on
 * data/data-0848B688.s above already records that this address holds the
 * "SELECT*MAP" blob. */
extern const u8 gUnknown_085802CC[];
/* Proc scripts -- Proc_Start's first argument, same model as
 * gUnknown_08616A40 above. */
extern const struct ProcCmd gUnknown_08616A58[];
/* Wave 56, W56-R. A proc script, same model as gUnknown_08616A40 above:
 * sub_08081D30 hands it to Proc_Start with its own proc as the parent, at both
 * of the two places it commits a menu selection, and does nothing with the
 * returned ProcPtr. Nothing else in the ROM references it, so the type is read
 * straight off Proc_Start's prototype and the extent is unprovable from the
 * call sites. It is NOT part of the 0x0861696C run above -- that run is the
 * u8 permutation plus two s16 coordinate tables ending well before 0x08616A00,
 * and gUnknown_08616A58 (also a script) sits between the two. */
extern const struct ProcCmd gUnknown_08616A68[];
extern const struct ProcCmd gUnknown_08616CF4[];
/* 0x0200FE50 is bound in aw2bhr.lds. sub_08085B30 CpuFastSets 0x10 words from
 * +0 and from +0x400 into OBJ VRAM and Decompresses into it at -0x200, so the
 * declared symbol is the middle of a larger buffer; `u8 []` for the byte
 * arithmetic, non-const because Decompress writes it. */
extern u8 gUnknown_0200FE50[];

/* Wave 53, W53-D. The ROM blobs of sub_0806938C / sub_0806B1A8 / sub_08087C94 /
 * sub_0808A6CC. Each is passed by name with no arithmetic, so the type is read
 * straight off the callee's prototype: `u8 []` for `Decompress(u8 *, void *)`
 * sources and `u16 []` for `ApplyPaletteExt(u16 *, u32, u16)` palettes. None
 * can be const -- neither prototype takes a const pointer and the build is
 * -Werror. */
extern u8 gUnknown_08183B14[];   /* -> *gUnknown_08499580  (sub_0806938C) */
extern u8 gUnknown_081933F4[];   /* -> 0x0600CC00          (sub_0806B1A8) */
extern u16 gUnknown_08194280[];  /* palette 0x80, 0x20 B   (sub_0806B1A8) */
extern u8 gUnknown_081942A0[];   /* -> *gUnknown_08499584 and *gUnknown_08499580,
                                  * decompressed twice in a row (sub_0806B1A8) */
extern u8 gUnknown_081A3DA4[];   /* -> 0x06005000          (sub_0806B1A8) */
extern u16 gUnknown_0823BE20[];  /* palette 0x40, 0x20 B   (sub_08087C94) */
extern u8 gUnknown_0823FFBC[];   /* -> gUnknown_0200FC50   (sub_0808A6CC) */
extern u16 gUnknown_08240AD4[];  /* palette 0x200, 0x20 B  (sub_0808A6CC) */
/* Wave 53, W53-D. A 0x20-entry u16 lookup: sub_0806B1A8 reads it as
 * `gUnknown_08581984[i & 0x1f]` with a plain `ldrh` for i over 0..0x27f and
 * biases each entry by 0x2280 (0x1280 when the entry is > 7) into
 * *gUnknown_0849957C, i.e. it is a tilemap template. The `bls` on the raw
 * entry is what makes it UNSIGNED; the extent is the mask and not a proved
 * bound. */
extern u16 gUnknown_08581984[];
/* Wave 53, W53-D. A CpuFastSet(const void *, ...) source in EWRAM -- 0x20 words
 * to OBJ VRAM 0x06010B00 in sub_0808A6CC. Byte-addressed only, so `u8 []`. */
extern u8 gUnknown_0200FED0[];
/* Proc scripts -- Proc_Start's first argument, same model as gUnknown_08616A40
 * above. Both are started by sub_0808A6CC with its own proc as the parent. */
extern const struct ProcCmd gUnknown_0861707C[];
extern const struct ProcCmd gUnknown_086170D4[];

/* Wave 53, W53-E. A ROM byte table indexed by `gUnknown_030040D8->unk00 - 1`,
 * i.e. by the unit-class record selector BIASED BY ONE -- sub_0805F4F8 reads it
 * as `ldrb r0,[r0]; subs r0,#1; adds r0,r0,r2; ldrb r0,[r0]` and only tests the
 * byte against zero, so nothing constrains it beyond the `ldrb`. */
extern const u8 gUnknown_085767A0[];

/* Wave 53, W53-E. A 0x14-stride ROM record table indexed by the LOW FIVE BITS
 * of the gUnknown_08499590 +0x1432 terrain byte -- the same `cell & 0x1f`
 * subscript the +0x1432 plane's other readers use. sub_0805F7B8 reads word +0
 * and multiplies it by 10 to build a score it then compares SIGNED (`ble`), so
 * the member is `int` and not `u32`; an unsigned member makes that comparison
 * `bls`. Only +0 has a reader, so the rest is filler and the extent is
 * unproved. */
struct Unk085D584C /* 0x14 */
{
    /* 0x00 */ int unk00;
    /* 0x04 */ u8 filler_04[0x10];
};
extern const struct Unk085D584C gUnknown_085D584C[];

#endif // UNKNOWN_GLOBALS_H

/* Wave 55 (W55-E), from sub_0803CFA4. Two more ROM words in the 0x0809xxxx
 * -fforce-addr run, both verified against baserom.gba:
 *
 *     0x08091144 -> 0x08499590   &gUnknown_08499590
 *     0x08091150 -> 0x08499590   &gUnknown_08499590  (a second private copy)
 *
 * sub_0803CFA4 reaches the map through the first of them, so its listing shows
 * `ldr r2,=gUnknown_08091144; ldr r1,[r2]; ldr r0,[r1]` -- three levels, one
 * more than the two-level `ldr rN,=gUnknown_08499590; ldr r0,[rN]` that
 * c_08008B70.c and friends emit. Do NOT declare 0x08091144 as an object and do
 * NOT declare it `u8 **`: writing the plain `gUnknown_08499590` produces the
 * third level on its own, and declaring the pool word adds a FOURTH. This is
 * the same result data/parked.json records for 0x0808D88C on sub_0800F8D4.
 * 0x0809113C (declared as `const u8 gUnknown_0809113C[]` above) is genuine
 * data and is NOT part of this run -- adjacency is not evidence here. */

/* Wave 56, W56-L, from sub_08007DD0. A 0x400-byte table (data/data.s sizes it
 * exactly, 0x485DC4..0x4861C4) read as `ldrsh` off `mask * 2` -- so 512 s16
 * entries, and the index confirms the extent independently: sub_08007DD0
 * builds `mask` out of NINE bits (the 3x3 neighbourhood around (x, y), bit 8
 * for the top-left cell down to bit 0 for the bottom-right), so its range is
 * exactly 0..511. SIGNED on the `ldrsh`, which is the discriminating read --
 * the value is returned from an s16-returning function, so a u16 table would
 * have needed a separate narrowing. Not marked const: nothing here proves it,
 * and gUnknown_084861C4 immediately after it is a second table of the same
 * shape that no promoted file has typed yet. */
extern s16 gUnknown_08485DC4[];
/* Wave 56, W56-P. The second table W56-L's note above predicted, now typed from
 * its reader: sub_0800B61C builds the SAME nine-bit 3x3 neighbourhood mask as
 * sub_08007DD0 (identical shift ladder, 8 down to 0), clears bit 4 with
 * `& ~0x10`, and indexes this table with `lsls #1` + `ldrsh` off a zero
 * register. Same 512-entry s16 shape, same signed read, and the sign is
 * load-bearing here rather than incidental: the very next instruction is
 * `cmp r2, #0; bge`, an early `return v;` for a negative entry, which a u16
 * table could not produce. Not const, for the same reason as its neighbour. */
extern s16 gUnknown_084861C4[];
/* Wave 56, W56-S. The decompressor dispatch table, typed from its only reader,
 * Decompress @ 0x08011CAC (matched this wave). Two entries per compression
 * type: Decompress indexes it with `((src[0] & 0xF0) >> 3) + notVram`, i.e.
 * `type * 2` plus a 0/1 selecting the VRAM variant, where the VRAM test is
 * `((u32)dst - 0x06000000) <= 0x17FFF` -- the 0x18000-byte GBA VRAM window.
 * A NULL entry is not a terminator but a fall-back marker: Decompress calls the
 * slot when it is non-NULL and does a plain CpuFastSet copy when it is NULL, so
 * the table is sparse by design and its length cannot be read off the code.
 *   The element type is settled by the call, not by guesswork: the indirect
 * call is `bl _call_via_r2`, and per the brief's register-index rule that is a
 * TWO-argument indirect call, matching `func(src, dst)`. The ROM reaches the
 * slot with a single pool word plus `adds r0, r0, r1; ldr r2, [r0]`, one load --
 * so the SYMBOL IS THE ARRAY, and it must NOT be declared as a pointer, which
 * would emit the two-level `ldr rN,=&g; ldr rN,[rN]` that gUnknown_08499590
 * legitimately needs. Not marked const: nothing in the one reader proves it. */
extern void (*gUnknown_08489314[])(const void *, void *);
