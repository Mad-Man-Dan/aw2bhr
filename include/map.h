#ifndef GUARD_MAP_H
#define GUARD_MAP_H
// 0x0201E450 
enum { MAP_POOL_SIZE = 0x508 };
struct Map
{
    /* 0x0000 */ u16 width;
    /* 0x0002 */ u16 height;
    /* 0x0004 */ s16 scrollX; // unk04
    /* 0x0006 */ s16 scrollY; // unk06
    /* 0x0008 */ u16 unk08;
    /* 0x000A */ u16 unk0a;
    /* 0x000C */ u16 camX; 
    /* 0x000E */ u16 camY; 
    /* 0x0010 */ u16 unk10;
    /* 0x0012 */ u8 unit[MAP_POOL_SIZE]; // unk0012
    /* 0x051A */ u8 unitUnk[MAP_POOL_SIZE]; // often the same as unit[]
    /* 0x0A22 */ u16 tile[MAP_POOL_SIZE];
    /* 0x1432 */ u8 terrain[MAP_POOL_SIZE];
    /* 0x193A */ s8 property[MAP_POOL_SIZE]; // property index for use with gProperty. -1 if invalid. 
        // player interface terrain window loads a palette (obj pal 6) based on MAP_OBJ_ARMY(gMap->property)
    
    /* 0x1E42 */ u8 visible[MAP_POOL_SIZE]; 
    /* 0x234A */ u8 unk234A[MAP_POOL_SIZE];
    /* 0x2852 */ s8 move[MAP_POOL_SIZE]; // unk2852. s8: 3 signed reads (< 0, >= 0, > 0; -1 = blocked) vs 1 unsigned
                                       // (`(u8)move > 0x78`, so the -1 sentinel counts as "too far")
    /* 0x2D5A */ s8 danger[MAP_POOL_SIZE]; // unk2D5A. s8: 6 signed reads (sentinel/<= 0 tests, `best = danger`) vs 4 unsigned
                                       // (min-search compares `(u8)danger > best`, where 0xFF must rank as the worst value)
    /* 0x3262 */ u8 dangerMask[MAP_POOL_SIZE]; 
    /* 0x376A */ u8 unk376A[MAP_POOL_SIZE];
    /* 0x3C72 */ u8 unk3C72[MAP_POOL_SIZE]; // might be signed (0x7f sentinel, ++, sums), but NO signed read exists and s8 changes
                                       // the code of c_0805A744/c_0805B980/c_0805F4F8, so it stays u8
    /* 0x417a */ u16 rowOffset[(0x421a - 0x417a) / 2];
    /* 0x421a */ u8 unk421a[0x4232 - 0x421a];

    /* 0x4232 */ u8 filler_4232[1];
    /* 0x4233 */ u8 unk4233;
};

/* `gMap` is a linker alias for `gUnknown_08499590` (include/unknown-globals.h),
 * the same 4-byte ROM data slot given a second name with the honest type --
 * see aw2bhr.lds for the linker symbol assignment that ties them together
 * (a plain `.set` can't alias across translation units at assembly time).
 * `gUnknown_08499590` itself is deliberately left declared `u8 *`: the huge
 * majority of already-matched files index it with raw byte-offset arithmetic
 * (`gUnknown_08499590 + tileMap_417A`, etc.), and per the Wave 50 note beside
 * its declaration, agbcc's codegen for that arithmetic depends on it staying
 * a `u8 *` -- switching the canonical symbol's type would silently change
 * addressing math in ~300 call sites without a compile error. New code that
 * wants a typed `struct Map *` (no cast, no manual offset math) should use
 * `gMap` instead of casting `gUnknown_08499590`. */
extern struct Map *gMap;

/* Wave 39 (W39-E), from sub_08028580. A 0xFF-terminated list of at most 0x5c
 * four-byte entries. Byte 0 packs two fields -- `& 0xe0` selects the owning
 * army (compared against gUnknown_084995F4[army]) and `& 0x1f` a terrain code,
 * with 0xFF terminating -- and bytes 1 and 2 are the map x and y handed to
 * sub_080240B4 / sub_0802419C. Byte 3 is never read here.
 *
 * IT MUST BE A STRUCT, not the flat `u8 [][4]` this declaration started as, and
 * the difference is byte-visible: the ROM forms the element address ONCE
 * (`lsls #2; add base`) and takes the three fields off it as `ldrb [r, #0]` /
 * `[r, #1]` / `[r, #2]` displacements. Only a COMPONENT_REF preserves that --
 * the flat array spells each field as its own address expression, and agbcc
 * answers with separate `gProperty + 1` and `+ 2` force-addr pool words
 * plus enough extra register pressure to spill a value the ROM keeps in a
 * register. Same rule as the map header in src/decomp/c_08024058.c. */
struct Property /* 0x04 */
{
    /* 0x00 */ u8 flags;      /* `& 0xe0` selects the owning army and is compared
                               * against gUnknown_084995F4[army]; `& 0x1f` is a
                               * terrain code, with 8 distinguished; 0xFF
                               * terminates the list. Plain masks off a `ldrb`,
                               * not bitfields -- both are extracted with an
                               * `ands` against a register-held constant. */
    /* 0x01 */ u8 x;          /* map column, passed to sub_080240B4's s16 first
                               * parameter with no narrowing in between */
    /* 0x02 */ u8 y;          /* map row; `lsls #1` indexes rowOffset[] at
                               * +0x417A, the c_08001158.c idiom */
    /* 0x03 */ u8 filler_03[0x01];
}; // 3003150
extern struct Property gProperty[]; // previously gUnknown_03003150 


void MakeTileSimple(int x, int y, int tileID);
int IsTerrainAtCoordsType(int x, int y, int terrainID);
void SetTerrainAt(int x, int y, int terrainID);
int GetDesignRoomOption(int id);
int GetDefaultTileForTerrain(int terrainID);
int IsTerrainLand(int x, int y);
int IsTerrainNotWater(int x, int y);
int IsTerrainNotWaterOrRiver(int x, int y);
int GetTileWithShadowAt(int x, int y);
int GetTileWithShadow(int x, int y, int tile);
int GetTileWithShadow2(int x, int y, int tile);
void EnsureValidTile(int x, int y);
int GetUnitTypeAt(int x, int y);
int CountArmyUnits(int army);
int GetPropertyKindForTerrain(int terrainID);
int GetPropertyKindAt(int x, int y);


enum TerrainKind {
    TERRAIN_NONE = 0,
    TERRAIN_PLAIN = 1,
    TERRAIN_RIVER = 2,
    TERRAIN_MOUNTAIN = 3,
    TERRAIN_WOOD = 4,
    TERRAIN_ROAD = 5,
    TERRAIN_CITY = 6,
    TERRAIN_SEA = 7,
    TERRAIN_HQ = 8,
    TERRAIN_UNK = 9, // Terrain.txt (Nightmare) calls entry 8 "Umi?" -- umi is
                      // Japanese for sea, so a sea variant of some kind
    TERRAIN_AIRPORT = 0xA,
    TERRAIN_PORT = 0xB,
    TERRAIN_BRIDGE = 0xC,
    TERRAIN_SHOAL = 0xD,
    TERRAIN_BASE = 0xE,
    TERRAIN_PIPE = 0xF,
    TERRAIN_PIPE_SEAM = 0x10,
    TERRAIN_SILO = 0x11,
    TERRAIN_SILO_USED = 0x12,
    TERRAIN_REEF = 0x13,
    TERRAIN_LAB = 0x14,
    TERRAIN_MINICANNON_S = 0x15,
    TERRAIN_MINICANNON_N = 0x16,
    TERRAIN_MINICANNON_W = 0x17,
    TERRAIN_MINICANNON_E = 0x18,
    TERRAIN_LASER = 0x19,
    TERRAIN_CANNON_S = 0x1A,
    TERRAIN_CANNON_N = 0x1B,
    TERRAIN_VOLCANO = 0x1C,
    TERRAIN_FACTORY = 0x1D,
    TERRAIN_DEATHRAY = 0x1E,
    TERRAIN_BLOCKED = 0x1F, 
    
    // Property terrain repeats per owning faction: the army is OR'd into the
    // top three bits (see MAP_OBJ_ARMY). Black Hole reuses one of these.
    TERRAIN_CITY_OS = 0x26, // Orange Star
    TERRAIN_CITY_BM = 0x46, // Blue Moon
    TERRAIN_CITY_GE = 0x66, // Green Earth
    TERRAIN_CITY_YC = 0x86, // Yellow Comet
    TERRAIN_HQ_OS = 0x28, // Orange Star
    TERRAIN_HQ_BM = 0x48, // Blue Moon
    TERRAIN_HQ_GE = 0x68, // Green Earth
    TERRAIN_HQ_YC = 0x88, // Yellow Comet
    TERRAIN_AIRPORT_OS = 0x2A, // Orange Star
    TERRAIN_AIRPORT_BM = 0x4A, // Blue Moon
    TERRAIN_AIRPORT_GE = 0x6A, // Green Earth
    TERRAIN_AIRPORT_YC = 0x8A, // Yellow Comet
    TERRAIN_PORT_OS = 0x2B, // Orange Star
    TERRAIN_PORT_BM = 0x4B, // Blue Moon
    TERRAIN_PORT_GE = 0x6B, // Green Earth
    TERRAIN_PORT_YC = 0x8B, // Yellow Comet
    TERRAIN_BASE_OS = 0x2E, // Orange Star
    TERRAIN_BASE_BM = 0x4E, // Blue Moon
    TERRAIN_BASE_GE = 0x6E, // Green Earth
    TERRAIN_BASE_YC = 0x8E, // Yellow Comet
};

/* Tile ids stored in gMap->tile[] (see MakeTileSimple). Names and values come
 * from SRR_AW2's C_code.c, where each is written as a byte offset `>> 2`.
 *
 * The property tiles run five to a block, always in the order HQ, base, city,
 * airport, port: the neutral set at 0x1C0 and then one block per army at +5
 * each (OS 0x1C5, BM 0x1CA, GE 0x1CF, YC 0x1D4). SRR names only the HQ and
 * base of each block; the other fifteen are filled in from sub_080012DC in
 * src/design.c, a byte-matched terrain-to-tile table that spells all 25 of
 * these ids out as literals, so none of the block is guessed. */
enum TileKind {
    TILE_PLAIN = (0x4 >> 2),
    TILE_SEA = (0x20 >> 2), // one tile of sea in plains
    TILE_BRIDGE_H = (0x50 >> 2),
    TILE_BRIDGE_V = (0x58 >> 2),
    TILE_MOUNTAIN = (0x80 >> 2),
    TILE_SEA_C = (0xA8 >> 2), // sea connecting to other sea
    TILE_ROAD_V = (0x100 >> 2),
    TILE_ROAD_H = (0x184 >> 2),
    TILE_FOREST = (0x21C >> 2),
    TILE_REEF = (0x5A0 >> 2),
    TILE_SILO = (0x600 >> 2),
    TILE_LASER = (0x604 >> 2),
    TILE_CANNON_D = (0x608 >> 2),
    TILE_CANNON_U = (0x60C >> 2),
    TILE_CANNON_L = (0x610 >> 2),
    TILE_CANNON_R = (0x614 >> 2),
    TILE_BLACK_CANNON_D = (0x61C >> 2),
    TILE_BLACK_CANNON_U = (0x628 >> 2),
    TILE_FACTORY = (0x634 >> 2),
    TILE_DEATH_RAY = (0x640 >> 2), // also the missile silo thing
    TILE_VOLCANO = (0x69C >> 2),
    TILE_RUIN = (0x6AC >> 2),
    TILE_X = (0x6BC >> 2),
    TILE_HQ = (0x700 >> 2), // neutral
    TILE_BASE = (0x704 >> 2),
    TILE_CITY = (0x708 >> 2),
    TILE_AIRPORT = (0x70C >> 2),
    TILE_PORT = (0x710 >> 2),
    TILE_HQ_OS = (0x714 >> 2), // Orange Star
    TILE_BASE_OS = (0x718 >> 2),
    TILE_CITY_OS = (0x71C >> 2),
    TILE_AIRPORT_OS = (0x720 >> 2),
    TILE_PORT_OS = (0x724 >> 2),
    TILE_HQ_BM = (0x728 >> 2), // Blue Moon
    TILE_BASE_BM = (0x72C >> 2),
    TILE_CITY_BM = (0x730 >> 2),
    TILE_AIRPORT_BM = (0x734 >> 2),
    TILE_PORT_BM = (0x738 >> 2),
    TILE_HQ_GE = (0x73C >> 2), // Green Earth
    TILE_BASE_GE = (0x740 >> 2),
    TILE_CITY_GE = (0x744 >> 2),
    TILE_AIRPORT_GE = (0x748 >> 2),
    TILE_PORT_GE = (0x74C >> 2),
    TILE_HQ_YC = (0x750 >> 2), // Yellow Comet
    TILE_BASE_YC = (0x754 >> 2),
    TILE_CITY_YC = (0x758 >> 2),
    TILE_AIRPORT_YC = (0x75C >> 2),
    TILE_PORT_YC = (0x760 >> 2),
};

/* gUnknown_080BFBC4 maps a gMap->tile[] id to the four 8x8 quadrants that draw
 * it. Column order is from the '88TerrainEditor' Nightmare module (a community
 * ROM-editor definition) and is confirmed by the writers in c_08023A4C.c and
 * c_08023BAC.c, which store the four columns at dst +0, +1, +32 and +33 -- one
 * screen row being 32 entries. The high byte of each u16 carries the palette
 * and animation bits the module names beside each quadrant. */
enum TileQuadrant {
    TILE_QUAD_TOP_LEFT = 0,
    TILE_QUAD_TOP_RIGHT = 1,
    TILE_QUAD_BOTTOM_LEFT = 2,
    TILE_QUAD_BOTTOM_RIGHT = 3,
};

#define MAP_OBJ_TERRAIN(f)   ((f) & 0x1f)
#define MAP_OBJ_ARMY(f)      (((f) & 0xe0) >> 5)
#define MAP_OBJ_ARMY_MASK(f) ((f) & 0xe0)

#endif /* GUARD_MAP_H */
