#ifndef UNKNOWN_FUNCS_H
#define UNKNOWN_FUNCS_H

#include "global.h"

/* Fade starters. Each starts a fade proc and stores its argument, the fade
 * speed, in the new proc. In gUnknown_0848923C's fade, for example, each frame
 * adds the speed to an accumulator that stops at 0x1000 and publishes
 * accumulator >> 8 to gUnknown_03001FFC as the 0..0x10 blend coefficient, so
 * the fade lasts 0x1000 / speed frames. Callers pass 4, 0x10 or 0x40 (slow,
 * normal, fast). The parameter must stay `int`: a narrower type adds a
 * narrowing the original code does not have. */
void sub_08011550(int);
void sub_0801156C(int);
void sub_08011588(int);
void sub_080115B4(int);
void sub_080115E0(int, ProcPtr);
void sub_080115F8(int, ProcPtr);
void sub_08011610(int, ProcPtr);
void sub_0801163C(int, ProcPtr);
/* Two more fade starters of the same shape, with the same `int` speed. */
void sub_08011668(int);
void sub_08011684(int);
/* Returns 1 while any of the four fade procs (scripts gUnknown_084892C4,
 * gUnknown_0848929C, gUnknown_0848925C and gUnknown_0848923C) is running, else
 * 0. Must return bool8, not int: its caller's code depends on the 8-bit return.
 */
bool8 sub_080116A0(void);
void Decompress(u8 *, void *);

/* Two helpers called by the wrappers at 0x08004A60-0x08004B6C. sub_08004A30's
 * parameter is signed: one caller passes -1. */
void sub_08000654(void);
void sub_08004A30(int);
/* Fills `count` halfwords at `dst` with `value`; writes nothing when count <=
 * 0. `value` must stay `int`: as `u16` the function is 4 bytes longer than the
 * original. */
void sub_08001148(u16 *, int, int);
/* Byte version of sub_08001148: fills `count` bytes at `dst` with `value`.
 * `value` stays `int` for the same reason. */
void sub_08001138(u8 *, int, int);

/* Two builders of the command block at gUnknown_030044B0; each fills the block
 * and passes it to sub_080308B4. sub_08034534's first parameter is a command
 * id; sub_080344B4 always builds command 8. The parameter that indexes
 * gUnknown_08499594 (sub_08034534's second, sub_080344B4's first) is u8, and so
 * are sub_08034534's third and fourth, which its caller narrows. */
void sub_08034534(int, u8, u8, u8);
void sub_080344B4(u8, int, int);
/* Copies a 20-byte command block into the ring buffer reached through
 * gUnknown_08090CD8. */
void sub_080308B4(u8 *);

void sub_08012358(void);
/* Declared without a parameter list because the signature is not known yet. The
 * first argument selects one of three cases and the second is passed on. */
void sub_0801BB10();
void sub_0801237C(void);
void sub_08012C58(void *); // possibly "SetupBackgrounds"

/* sub_08011B34 adds an entry to the 16-slot callback list gUnknown_03000000 and
 * sub_08011B5C (below) removes one. The entry is `void *`, so callers that
 * register a function cast it. */
void sub_08011B34(void *);
/* A callback that sub_0807420C registers in the gUnknown_03000000 list; never
 * called directly. */
void sub_08037F1C(void);
void sub_08011B5C(void *);
/* Adds an entry to the 16-slot callback list gUnknown_03002FA0. Callers that
 * register a function cast it to `void *`. */
void sub_08011AAC(void *);
/* Callbacks registered in the gUnknown_03002FA0 list; never called directly. */
void sub_080184A4(void);
void sub_080184C8(void);
/* Callbacks registered in these lists by functions in other files
 * (sub_080111AC, sub_08012A74, sub_08049B70), which need these declarations to
 * take their addresses. */
void sub_080111BC(void);
void sub_08012A34(void);
void sub_08049BAC(void);
/* A callback that sub_08037780 removes from the gUnknown_03000000 list. */
void sub_08037790(void);

/* The script list gUnknown_0200C528. sub_080193B0 installs a script in a free
 * slot and returns the slot, or NULL when the list is full. sub_0801930C
 * removes every slot running the given script and always returns -1. */
struct Unk0200C528 *sub_080193B0(const u8 *);
int sub_0801930C(const u8 *);

/* Stops the script gUnknown_0848A42C with sub_0801537C and returns that
 * function's result: the slot index, or -1 when the script was not running.
 * Returns `int`, like sub_0801537C: if sub_0801537C returned `s8`, this
 * function would gain a narrowing the original does not have. */
int sub_0801A168(void);

/* The parameter is the address of a record like gUnknown_030013D0 (see that
 * global for its layout); the function writes to it. */
void sub_0802505C(void *);
/* Callers pass the ids 0xC9A-0xC9D; the value is passed on to sub_080146D4 as a
 * u16. */
void sub_0802D35C(int);
/* Called with 0 or 1; each value selects a different sound id for sub_0803B4DC.
 */
void sub_0802BFD0(int);

/* Called from the script commands ProcCmd_1D_0801D0E4 and ProcCmd_1E_0801D104
 * in proc.c. The first parameter must stay `int`: as `s16` it adds a narrowing
 * the original does not have. */
void sub_08013098(int, ProcPtr);
void sub_080130B0(int, ProcPtr);
/* Starts the proc gUnknown_0848936C and stores the first three arguments as
 * halfwords at +0x64, +0x66 and +0x68 of it. With a NULL parent the proc starts
 * on tree 3; otherwise it starts as a blocking child of the parent. */
void sub_080130DC(int, int, int, ProcPtr);
/* Same shape for the proc gUnknown_084893AC. The first argument indexes the
 * word table gUnknown_0848950C (the entry is stored at +0x4c), the second is
 * stored as a halfword at +0x44, and the third is the parent or NULL. */
void sub_08013338(int, int, ProcPtr);
void sub_08013AEC(void);
u8 *sub_0801F48C(void);
void sub_08024378(void);
/* Starts the proc gUnknown_08581480 under `parent`, stores the first three
 * arguments at +0x34, +0x38 and +0x3c of it and clears +0x40. */
void sub_08069FAC(int, int, int, ProcPtr);
void sub_08067820(void);
/* One of the four identical palette wrappers; see ApplyPaletteExt below. */
void sub_08013664(u16 *, u32, u16);
/* sub_08011228 is the HBlank handler that sub_08011298 and sub_0801137C install
 * through sub_080111C8. sub_080111C8 starts a proc and stores its five
 * arguments in it: two addresses at +0x2c and +0x30, two halfwords at +0x34 and
 * +0x36, and the handler at +0x38. */
void sub_08011300(void);
void sub_08011354(void);
void sub_08011228(void);
void sub_080111C8(void *, void *, u16, u16, void (*)(void));
/* Adds `delta` to every halfword of the buffer `dst`, which is `size` bytes
 * long. sub_08045358 and sub_080453CC pass a size of 0x800 and a delta of
 * 0x82b0. */
void sub_08012B00(u16 *, u16, u16);
/* Writes tile entry `c` with palette 12 at column x, row y of the BG0 tilemap
 * buffer. */
void sub_08012E74(u16, u16, u16);
/* Draws the ASCII character `c` at (x, y) in the BG0 tilemap buffer, mapping it
 * to a font tile with sub_08012E74. */
void sub_08012E9C(u16, u16, u8);
void sub_08013C00(void);
void sub_08024268(void);
/* Callbacks registered in the gUnknown_03000000 list; never called directly. */
void sub_080246B4(void);
void sub_08024720(void);

/* Copies `size` bytes from `src` to `dst` in halfwords with CpuSet (it halves
 * the size into CpuSet's count). ApplyPaletteExt and sub_080135F4 call it. */
void sub_08011C58(const void *, void *, u16);
/* sub_0801F00C sets gUnknown_03001FE0 to 1. */
void sub_0801F00C(void);
void sub_08036B4C(void);
void sub_080135A4(void);
/* The last parameter must stay s8: the function stores it as a byte and also
 * uses its sign bit as a flag (it selects a brightness bias). */
void sub_080136DC(u16 *, u16, u16, s8);
/* Copies `size` bytes of colours from `src` into the palette buffer gPal at
 * `offset`. One of four identical wrappers, with sub_080135F4, sub_08013640 and
 * sub_08013664. `size` must stay u16: as u32 its narrowing moves after the
 * address arithmetic and the code no longer matches. */
void ApplyPaletteExt(u16 *, u32, u16);
void sub_080136C4(void);
/* Palette-fade setters. sub_080137AC stores its argument, a signed step, in the
 * s8 gUnknown_0200B5F4. It must stay `s8`: callers pass -1, and as `u8` the
 * call in src/title-screen.c (upstream source, which cannot be edited) would
 * compile differently. */
void sub_080137AC(s8);
/* The parameter is a full word; it is narrowed to s8 only where it is passed to
 * sub_0801394C. */
void sub_080139C4(s32);
/* Single-row forms of the row-loop setters above, and the per-frame applier.
 * sub_080138B0 takes (u8 row, s8 step); its caller sub_08013928 depends on both
 * types, so do not widen either. */
void sub_080138B0(u8, s8);
void sub_080139E0(void);

/* Starts a script in a free gUnknown_03001470 slot, like sub_080152EC below,
 * and returns the slot index, or -1 when no slot is free. The first parameter
 * is really the script pointer; callers cast it to s32. */
s8 sub_080152C0(s32, u8);
/* Starts a script in a free gUnknown_03001470 slot and returns the slot, or
 * NULL when no slot is free. */
struct Unk03001470 *sub_080152EC(const void *, u8);
/* Stops a script: finds the gUnknown_03001470 slot running the given script,
 * ends it with sub_08015328 and returns the slot index, or -1 when no slot runs
 * it. Returns `int`, not `s8`, to keep sub_0801A168 matching (see there). */
int sub_0801537C(const void *);
s8 sub_08015BD0(s32);
/* sub_08015BD0 (declared just above) scans the gUnknown_03001470 slots and
 * returns a slot index, or -1; sub_080152C0 and sub_080152EC use it to pick a
 * slot. Its parameter is really a script address; callers cast it to s32. */
/* Return the main-loop callback gUnknown_030040EC, which AgbMain calls once per
 * pass of its main loop before checking for a soft reset. sub_080366DC is a
 * second name for GetMainLoopCallback, set up in c_080366DC.c. sub_08014BE8
 * compares the result against sub_080369BC. */
void (*GetMainLoopCallback(void))(void);
void (*sub_080366DC(void))(void);
void sub_080369BC(void);
/* Sweeps over the whole gUnknown_0200E438 list, each called through a one-line
 * forwarder (sub_08015544, sub_08015550, sub_0801555C). */
void sub_0801D8B4(void);
void sub_0801DED8(void);
void sub_0801DF20(void);
/* Two more sweeps of the same kind, called from sub_08052EA8's main loop. */
void sub_0801D8E4(void);
void sub_0801D924(void);
/* Per-slot workers of the four sweeps above, called as f(slot, 0) or f(slot,
 * 1). */
void sub_0801D390(int, int);
void RunSpriteScript(int, int); /* sub_0801D390's readable name; see
                                 * src/decomp/c_0801D390.c. */
void sub_0801DCD4(int, int);
void RunSimpleSpriteScript(int, int); /* sub_0801DCD4's readable name; see
                                      * src/decomp/c_0801DCD4.c. */
void sub_0801DB04(s16);
int sub_0801DC50(s16, u32 *, s16, int);
/* Allocate and free a block in the heap at gUnknown_03000050. sub_08014E44
 * returns NULL when no heap is installed. */
void *sub_08014E44(int);
void sub_08014ED4(void *);

/* Called together on the same unit; each takes a u8 flag and returns an
 * accumulated total. */
int sub_08029978(struct Unit *, u8);
int sub_08029A48(struct Unit *, u8);
void sub_08029088(s16, s16);

void sub_0801BD00(s32, s32, void *, s32);
void sub_0802BAFC(u16, u16, int);
/* Parameters: x offset, y offset, and a row index into gUnknown_0849A2A6. */
void sub_0802B4D4(s16, s16, s16);
/* Called by sub_0802AA78 right after sub_0802B4D4, with the same three
 * arguments (defined in c_0802B3AC.c). */
void sub_0802B3AC(s16, s16, s16);
void sub_0802B868(void);                                 /* c_0802B768.c */
void sub_0802B8C4(s16, s16, u16);                        /* c_0802B768.c */
void sub_0802A8DC(int, int, int, int, int);              /* c_0802A8DC.c */
void sub_0802AA14(int, int, int, int);                   /* c_0802AA14.c */
void sub_0802B91C(s16, s16, s16, s16, u8, u8, u8, s16);  /* c_0802B91C.c */
void sub_0802BB74(u16, u16);                             /* c_0802BB74.c */
/* Called by sub_0801E508. sub_0808B710 is sinf and sub_0808B91C is cosf. */
int sub_0801E3B4(int);
float sub_0808B710(float);
float sub_0808B91C(float);
/* sub_080169A4 is called by sub_0801E9B0. The five after it (sub_080555F0 to
 * sub_08057164) are cutscene-player helpers called by sub_080553C8. */
void sub_080169A4(s16, void *);
void sub_080555F0(u16, u16);
void sub_08055654(u16, u16);
void sub_08055940(u16, u16);
void sub_08055D4C(u16, u16);
void sub_08057164(int, int, u16);
/* The other branch of sub_0801BF2C: takes the same four arguments as
 * sub_0801BD00, built from one SpriteEntry. */
void sub_0801C090(s32, s32, void *, s32);
/* Tests whether a box is on screen: sub_08039DBC skips its sub_0801BD00 call
 * when this returns 0. The parameters are (x, y, size), with x and y
 * camera-relative and not yet wrapped to the screen. Returns int; callers that
 * test only the low byte cast the result. */
int sub_0801306C(int, int, int);
/* Searches gUnknown_03001430 for a free affine-matrix slot. Declared with an
 * empty parameter list on purpose: one caller passes a proc and another passes
 * nothing, and only an empty list compiles both callers to the original code.
 */
int sub_0801DAB0(/* ProcPtr */);
/* Only referenced as a value: sub_0801F4A4 stores it in the function pointer
 * gUnknown_030013EC. Takes five `int`s, the fifth on the stack. */
void sub_0801F4B4(int, int, int, int, int);
void PutSprite(u32, u32, u32, u16 *, u32);
void PutSpriteExt(u32, u32, u32, u16 *, u32);

void SetObjAffine(s32 index, s16 pa, s16 pb, s16 pc, s16 pd);
/* One of the four identical palette wrappers; see ApplyPaletteExt. */
void sub_080135F4(u16 *, u32, u16);

void sub_08030ED4(void);

/* The gUnknown_085D3DD0 lookup family. Each returns a fallback value when
 * gPlaySt.unk08 is 0 and a table entry otherwise, and each has a one-line
 * forwarder beside it that passes (unk1d, unk1e) from gPlayers. sub_08042F34
 * and sub_08042F7C ignore their second argument, but keep it: their forwarders
 * pass it. */
int sub_08042DCC(int);
/* Returns bool8 and takes s16s: its caller sub_08029D3C's code depends on both.
 */
bool8 sub_0804209C(s16 x, s16 y);
/* Returns int, not u8, although it reads a byte: sub_0807F630 passes the result
 * on without narrowing it. */
int sub_08042E18(int);
int sub_08042E2C(int, int);
int sub_08042E84(int, int);
int sub_08042EDC(int, int);
int sub_08042F34(int, int);
int sub_08042F7C(int, int);
int sub_08042FC4(int, int);
u32 sub_0804301C(int, int);

void sub_08035144(u8);

void sub_08039F58(void);
/* Writes `value` to the cell at (column, row) when both lie inside the extents
 * stored in gUnknown_08499590. Column and row are signed `int`s: sub_08044994
 * passes values up to 2 outside the range. */
void sub_08044854(int, int, int);

/* Reads flag `id`: a bit in one of three bit arrays, chosen by the range `id`
 * falls in (0x20..0x5f select bit id - 0x20 of gUnknown_02028030.unk00).
 * Returns int, not bool8: sub_080485C4 tests the full register. */
int sub_0803CBD8(int);
/* Return gPlayers[i].unk3a and gPlayers[i].unk3b. They return int, not u8:
 * sub_080264BC uses the full results without narrowing. */
int sub_08025CF0(int);
int sub_08025D08(int);
void sub_080260D0(struct Unit *, int);
u8 sub_080263A4(u8);
u8 sub_080264BC(u8);
u32 sub_08026368(u8);
int sub_08037D80(int);
/* sub_08026254 returns the chosen entry of the 0xff-terminated list
 * gUnknown_020288A0. */
u8 sub_08026254(void);
u8 sub_08026424(u8);
bool8 sub_080261E8(int);

/* The gUnknown_02028030 single-bit readers. Each returns `(1 << (id & 7)) &
 * base[id >> 3]` for one byte range of the struct (unk10, unk2a, unk2d, ...),
 * so the result is the mask bit itself, not 0 or 1. They return u8 because
 * their callers narrow the result. sub_0803CAB8 (below) is the exception: one
 * of its callers, sub_0807F57C, uses the full register, so it returns int and
 * its other callers cast the result to u8. */
u8 sub_0803CA9C(u32);
/* Returns int, not u8, unlike the rest of the family: sub_0807F57C uses the
 * full result, and the other callers cast it with `(u8)`. */
int sub_0803CAB8(u32);
u8 sub_0803CAD4(u32);
/* Same bit-reader family. */
u8 sub_0803CAF0(u32);
/* Same bit-reader family. */
u8 sub_0803CB24(u32);
/* Same bit-reader family, but it reads the bit through sub_080206B0 instead of
 * indexing directly. */
u8 sub_0803CA70(u32);
/* sub_08072B54's first parameter is a full word; it is narrowed to s16 only
 * where it is passed to sub_0803B4DC. */
void sub_08072B54(int, int);

/* Sets one animated palette colour: `(phase & 0x1f) / 2` indexes the 16-entry
 * u16 table gUnknown_081D1624, the colour goes to gPal + 0xb2, and sub_080135A4
 * flushes it. The argument is a 5-bit animation phase. */
void sub_08075340(int);
void sub_08075AC4(int, int);
/* One of the 0x08079xxx proc helpers. The proc argument is unused. The second
 * argument, a scroll offset, must stay u32: the function compares it unsigned,
 * and as `u16` it gains a narrowing the original does not have. */
void sub_080795A8(ProcPtr, u32);
/* sub_08079B04 is deliberately not declared here. It is `static` in the one
 * file it shares with sub_0807974C and its three callers, which must stay in
 * address order; a global declaration changes the three calls to it. Its code
 * starts at 0x08079B38, after sub_0807974C's literal pool. */
/* sub_0803BC7C, sub_0803BC88 and sub_0803BC94 return bytes +1, +3 and +5 of
 * gUnknown_03003F30. They return u8: the sprite builders sub_080831FC,
 * sub_08083484 and sub_08083738 depend on the narrowing. */
int sub_0803BD14(void);
u8 sub_0803BC7C(void);
u8 sub_0803BC88(void);
u8 sub_0803BC94(void);
int sub_08044374(int);

bool8 sub_0803B18C(void);
/* sub_0803B4DC's argument is a sound id. Both parameters must stay `int`: as
 * `s16` the calls in sub_08016104 and sub_08016130 change, and as `u16` the
 * calls in proc.c change. */
void sub_0803B4DC(int);
void sub_0803B524(int);
/* The 0x08039xxx block. sub_08039A58 is sub_080399F8's sibling: sub_08039948
 * calls the two from different cases of one switch with the same arguments
 * (0x2b0, 8). sub_08039544 takes the pointer sub_08039F18 returns: sub_080396F4
 * calls sub_08039544(sub_08039F18(proc->unk54)). */
void sub_080168BC(int);
/* Overworld-marker helpers called by sub_08039188. Their u8 parameters and u8
 * returns are what sub_08039188's code depends on. */
u8 sub_08039064(u8);
u8 sub_080390CC(u8);
/* sub_08039140 tests whether a box of size (w, h) at (x, y) is on screen. It is
 * declared without a parameter list on purpose, and defined K&R style in
 * c_08039140.c as (u16 x, s16 y, u8 w, u8 h): this way its caller sub_08039188
 * passes x and y as plain ints without narrowing them, as the original code
 * does. Do not add a prototype. */
u8 sub_08039140();
void sub_0803941C(int, int);
void sub_08039544(u8 *);
void sub_08039930(int, ProcPtr);
void sub_080399F8(int, int);
void sub_08039A58(int, int);
u8 *sub_08039F18(int);
void sub_0803B55C(int);
/* The parameter must stay `int`: as `s16` or `u16` the function gains a
 * narrowing at entry that the original does not have. */
void sub_0803B5A4(int);
void sub_0803B5E8(void);

/* sub_0803B118 takes a gUnknown_03001470 slot: it passes the slot to
 * sub_080153B8 in one branch and reads its unk1e in the other. sub_0803B3B0's
 * argument indexes gUnknown_080910FC. */
void sub_0803B0EC(void);
void sub_0803B118(struct Unk03001470 *);
void sub_0803B198(void);
void sub_0803B37C(void);
void sub_0803B3B0(int);
void sub_0803B4EC(int);
/* sub_0803B578 is deliberately not declared. It is a proc callback reached only
 * through a proc script, and its definition types its parameter with a struct
 * local to its file, which a ProcPtr declaration here would conflict with. */
void sub_0803B588(void);
void sub_0803B640(void);
void sub_0803B0D8(void);
void sub_0803B350(u16);

/* sub_0803ABD8 is an empty function. */
void sub_0803ABD8(void);
/* Three forwarders and a blitter. sub_0803A190 registers sub_0803A174 as a
 * callback; sub_0803AFA0 calls the other three. */
void sub_0803A174(void);
void sub_0803AF78(void);
void sub_0803AF84(void);
void sub_0803AF90(void);
void sub_0803AF5C(void);

/* Sound driver (m4a) entry points, with the names of their equivalents in
 * Nintendo's MP2K library:
 *
 *   sub_080703F4  m4aSoundInit
 *   sub_08070478  m4aSongNumStart(songNum)
 *   sub_080705AC  m4aMPlayAllStop
 *   sub_08070610  m4aMPlayFadeOut(mplayInfo, speed); only
 *                 gUnknown_03005AE0 is ever passed
 *   sub_08071420  MPlayVolumeControl(mplayInfo, trackBits, volume) */
void sub_080703F4(void);
void sub_08070478(u16);
void sub_080705AC(void);
void sub_08070610(void *, u16);
void sub_08071420(struct MusicPlayerInfo *, u16, u16);
/* m4aMPlayFadeOutPause (sub_08070620) and m4aMPlayFadeInContinue
 * (sub_08070640), both (mplayInfo, speed). */
void sub_08070620(struct MusicPlayerInfo *, u16);
void sub_08070640(struct MusicPlayerInfo *, u16);

/* Sets gUnknown_030040A0 to 1, starts the script gUnknown_084858DC with
 * sub_080152EC and stores the argument as a halfword at +0x1e of the new slot,
 * which sub_0803B118 reads back to drive a timeout. */
void sub_08001038(int);
/* Starts the proc gUnknown_0849BC98 under `parent` and discards the result. Its
 * one caller passes 3 (PROC_TREE_3). */
void sub_0803433C(ProcPtr);

/* The m4a entry points wrapped by the forwarders at 0x0803B3C8-0x0803B408:
 *
 *   sub_0806FD98  m4aSoundVSync
 *   sub_08070990  m4aSoundMode(mode): the reverb, channel-count and
 *                 master-volume fields in one word
 *   sub_08070A7C  m4aSoundVSyncOff
 *   sub_0807046C  m4aSoundMain, a forwarder to sub_0806F744
 *   sub_08070AF8  m4aSoundVSyncOn */
void sub_0806FD98(void);
void sub_08070990(u32);
void sub_08070A7C(void);
void sub_0807046C(void);
void sub_08070AF8(void);

/* Calls m4aSoundMode(n << 8), which sets the number of mixer channels to n.
 * sub_0803B3C8 calls it with 8. */
void sub_0803B3D4(int);

/* The shared bodies of the predicate wrappers at 0x0803C574-0x0803C644. Both
 * return -1, 0 or 1.
 *
 *   sub_0803C52C(id, n)  -1 if sub_0803CAB8(id); else 1 if flag 0x21 is
 *                        set and sub_08037DA4(gUnknown_0200C420.unk10)
 *                        >= n; else 0.
 *   sub_0803C5E8(id)     -1 if sub_0803CAD4(id), 1 if sub_0803CAB8(id),
 *                        else 0. */
int sub_0803C52C(u32, int);
int sub_0803C5E8(u32);

/* The constants these wrappers pass are two different things. 0x21, 0x22 and
 * 0x26, passed to sub_0803CBD8, are global flag ids. The 3, 4 and 5 passed as
 * `n` to sub_0803C52C are thresholds on sub_08037DA4's rank of 2..5, not ids.
 * The five wrappers sub_0803C614 to sub_0803C644 are identical `return
 * sub_0803C5E8(id);` forwarders. */

/* Maps a value (gUnknown_0200C420.unk10) to a rank: up to 0xc7 gives 2, up to
 * 0xf9 gives 3, up to 0x117 gives 4, and anything larger gives 5. */
int sub_08037DA4(int);

/* Returns 0 or 1. Must return bool8, not int: every caller narrows the result.
 */
bool8 sub_0803E388(int);

bool8 sub_08045650(void);
/* Takes an index into gPlayers. */
void sub_08044AB8(int);

/* Called by the five wrappers at 0x08044C44-0x08044D34. Starts the proc script
 * given first, under the parent proc given last, and fills it: the next two
 * arguments (a data block and a palette) go in words at +0x4c and +0x50, and
 * the five after them in bytes at +0x2c..+0x30.
 *
 * The small integers are `int` because the wrappers pass -1. The eighth
 * parameter must stay u8 and the seventh `int`: with other types the two byte
 * stores swap places. */
void sub_08044D70(const struct ProcCmd *, void *, void *, int, int, int, int, u8, ProcPtr);
void sub_0801DA94(void);
void *sub_08043A80(int);
void *sub_08043A90(int);
/* The third and fourth parameters must stay u16: the function narrows them at
 * entry, before any other code. */
void sub_08039A5C(void *, void *, u16, u16);

/* Returns -1 by default. Must return s8: sub_0803B904 sign-extends the result.
 */
s8 sub_08016D04(u8);
/* Starts a proc and stores the callback at +0x4c of it. The callers
 * (sub_08038548, sub_08038568, sub_08045770) pass sub_0803BA00, sub_0803B8B8
 * and sub_0803B8A0. */
void sub_0803D73C(u8, void (*)(void));
void sub_0803B8A0(void);
/* The callback that sub_08038548 passes to sub_0803D73C. */
void sub_0803BA00(void);

/* Callbacks stored in gUnknown_03004778 by sub_0805CDF0 and sub_0805CE20; never
 * called directly. */
void sub_0805DB64(void);
void sub_0805DB70(void);
/* The rest of the gUnknown_03004778 callback set, stored by the list builders
 * at 0x0805CA60-0x0805D1F0. */
void sub_0805D888(void);
void sub_0805DA84(void);
void sub_0805DB0C(void);
void sub_0805DB50(void);
void sub_0805DCA4(void);
void sub_0805DCD4(void);
void sub_0805DFB8(void);
void sub_0805DFE8(void);
void sub_0805DFF4(void);
void sub_0805E160(void);
/* Another member of that callback set, stored by sub_0805D2A0. */
void sub_0805E3BC(void);

/* Sorts the id list gUnknown_030045F0 after the 0x0805Cxxx builders fill it.
 * The argument is only tested against zero; every caller passes
 * gUnknown_0300477C. */
void sub_0805D344(u32);

void sub_08063994(void);
void sub_0806A454(void);
void sub_0806CC4C(void);
void sub_0806CC64(void);

void sub_080718F0(void);
/* Returns the byte its argument points to. Never called in place; see
 * sub_0808AD6C. */
u8 sub_0808AD68(u8 *);
/* Copies sub_0808AD68's code into the given buffer and publishes the copy
 * through gUnknown_03000F6C, so it can be called from RAM. */
void sub_0808AD6C(u16 *);
/* ProgramFlashSector: writes a buffer to one flash sector. Same unlock sequence
 * and sector-address computation as sub_0808B430, which fills the sector with
 * 0xFF instead. Returns u16: its caller sub_0808B5B8 narrows the result. */
u16 sub_0808B540(u16, const u8 *);
/* ReadFlashId: copies sub_0808AD68 into a stack buffer with sub_0808AD6C, calls
 * the copy to read the maker and device bytes, and returns (maker << 8) |
 * device. */
u16 sub_0808AAF4(void);
/* sub_0806377C walks the gUnknown_03001470 slots from 29 down to 0 and calls
 * sub_08015C30 on each slot whose script (.unk00) is the given one;
 * sub_0801537C does the same scan upwards with sub_08015328. sub_08067504 calls
 * Proc_Break on every proc in sProcArray running the given script (an
 * open-coded Proc_BreakEach). */
void sub_0806377C(const void *);
/* Two more scans of the same kind. sub_080637D8 calls sub_08015A30 instead of
 * sub_08015C30; sub_08063814 calls sub_08015328 on every slot that is NOT
 * running the script. sub_080637AC (below) returns the matching slot instead of
 * acting on it. */
void sub_080637D8(const void *);
void sub_08063814(const void *);
/* Waits until bit 7 of REG_SIOCNT clears, giving up after 0x795C tries, then
 * calls sub_08063614(0x258). */
void sub_0806362C(void);
/* Busy-waits for a delay measured in CPU cycles. Written by hand in assembly
 * (see data/asm-resident.json): it reads the PC to choose its per-iteration
 * cost. */
void sub_08063614(int);
void sub_08067504(const struct ProcCmd *);
/* The screen-fade driver gUnknown_08613EE4, plus the routines that the 41
 * wrappers at 0x08071F88-0x08072288 pass to it or call beside it.
 *
 * sub_080722B8(kind, speed, parent, onDone):
 *   kind    0..7, an index into gUnknown_081CBF68: eight 12-byte records
 *           of { start function (Proc_Start or Proc_StartBlocking),
 *           setup function (sub_080137AC, sub_08013830, sub_080138B0 or
 *           sub_0801394C), direction (+1 or -1, fade in or out) }.
 *   speed   signed. Each frame sub_08072344 adds it to an accumulator
 *           that finishes at 0x200, so the fade lasts 0x200 / speed
 *           frames. 0x10 is the normal rate.
 *   parent  the parent proc.
 *   onDone  called with no arguments when the fade ends; NULL for none.
 *           It must stay a function pointer, not an int, so that the
 *           wrappers' pool words refer to the callbacks' symbols. */
void sub_080722B8(int, int, ProcPtr, void (*)(void));
void sub_080723DC(void);
void sub_08072454(void);
void sub_08072394(void);
void sub_08013780(u16, u16, u8);
void sub_080723C0(void);
/* The per-frame step of the gUnknown_08613EE4 fade, called by sub_08072320.
 * Returns 1 while the fade is still running, else 0. It reads +0x54, +0x58 and
 * +0x5c of the proc. */
u8 sub_08072344(ProcPtr);
/* Starts the gUnknown_08613F2C proc under `parent` (the fifth argument) and
 * returns it; sub_080725E4 and sub_080725FC write +0x3a of the result. */
ProcPtr sub_080725A8(int, int, int, int, ProcPtr);
/* The first parameter is really a function pointer (sub_08072948 calls it from
 * the field where it is stored), so callers cast it to u32. */
void sub_0807298C(u32, u32, u32);
/* Sets the scroll pair (x, y) of background `bg` (0..3); other values of `bg`
 * do nothing. All three parameters must stay u16: the function narrows each at
 * entry. */
void sub_08072C40(u16, u16, u16);
s32 Interpolate(s32, s32, s32, s32, s32);

s32 Div(s32, s32);

/* ---- Callees of three wrapper families ----------------------------------- */

/* Runs or defers a callback. If gUnknown_03001FE0 is nonzero it calls
 * fn(gUnknown_03001FE0, arg) at once; otherwise it queues the pair with
 * sub_0801EDC0. The callback is `void *` (the functions passed ignore both
 * arguments), so callers cast it. */
int sub_0801F024(void *, u16);
/* The queueing half of sub_0801F024: passes the callback to sub_0801ECE8 with
 * bit 31 set as a tag. Returns a value. */
int sub_0801EDC0(void *, s16);

/* ---- The 0x0801F000 block ------------------------------------------------ */
/* Loads palette `index` of the table gUnknown_0848B738 into palette slot
 * `slot`. Both parameters must stay `int`: as `u8` the second adds narrowings
 * the original does not have. */
void sub_0801F178(int, int);
/* Called from one branch of sub_0801F084. sub_0801BF2C walks the
 * gUnknown_0200D510 layer list selected by its argument, a SpriteEntry index.
 */
void sub_0801BF2C(int);
void sub_0801EE10(void);
void sub_0801F084(void);
/* Two halves of one mapping between a tile or palette id and the six-entry
 * table gUnknown_0848B738: sub_0801F400 returns the table's third column, and
 * sub_0801F3D4 is the inverse. For an out-of-range argument, sub_0801F400
 * returns the argument unchanged. */
int sub_0801F3D4(int);
/* Returns the source address of a graphics block for CpuFastSet; callers use it
 * as sub_0801F444(a, sub_0801F3D4(a)). */
void *sub_0801F444(int, int);
/* Submits a filled sprite request (a gUnknown_0200ED20 entry) with an s16, and
 * returns an s16 handle, or -1. */
s16 sub_0801A718(struct Unk0200ED20 *, s16);
/* sub_0801A700 takes an entry from the gUnknown_0200ED20 free list and
 * sub_0801A6C0 resets the list. sub_0801A700 returns u32, as defined, so
 * callers cast the result to struct Unk0200ED20 *. */
u32 sub_0801A700(void);
bool8 sub_0801A6C0(void);
/* Draws one sprite request. Its seven arguments are fields of one struct
 * Unk0200ED20 entry; the last three are passed on the stack. */
int sub_0801E9B0(s16, s16, s16, u16 *, long long, s16);
/* sub_0801E2A4 advances the interpolation of the 32 records at
 * gUnknown_0808F0B4 each frame. sub_0801E3E8 expands OAM attributes: its fourth
 * argument is a u16 command stream and its fifth an affine-parameter index. */
void sub_0801E2A4(void);
int sub_0801E3E8(int, int, int, u16 *, int);
/* The third case of sub_0801DFE8's switch. It takes the same arguments as
 * sub_0801E3E8; the result is ignored. */
int sub_0801E508(int, int, int, u16 *, int);
/* The reset at the end of sub_080466DC. */
void sub_08045FC8(void);
/* The 56-frame cursor advance that sub_080466DC runs first. */
void sub_080466A4(void);
int sub_0801F400(int);

/* An empty function; sub_080252EC calls it once with gUnknown_030013B0 and once
 * with gUnknown_030013D0. */
void sub_080252E8(void *);

/* Single-slot callback setters. SetMainLoopCallback sets gUnknown_030040EC,
 * which AgbMain calls once per main-loop pass; SetVBlankCallback sets
 * gUnknown_030040D0, which runs through RunVBlankCallback. sub_080366C4 and
 * sub_080366D0 are second names for the two, set up in c_080366C4.c. They take
 * a function pointer, not `void *`, so callers pass a function by name without
 * a cast.
 *
 * The void functions after them are called by the wrapper functions
 * sub_08002EB4, sub_08028154, sub_0802CD00, sub_0802CD14, sub_08034FD8,
 * sub_08034FEC and sub_0805DB50, each of which makes three separate calls, and
 * by the two sub_0801F024 wrappers. */
void SetMainLoopCallback(void (*)(void));
void SetVBlankCallback(void (*)(void));
void sub_080366C4(void (*)(void));
void sub_080366D0(void (*)(void));
void sub_08002D7C(void);
void sub_08002DEC(void);
void sub_08002E5C(void);
void sub_0801A614(void);
void sub_08023348(void);
void sub_08023354(void);
void sub_08024584(void);
void sub_0802BC5C(void);
void sub_0802D4B0(void);
void sub_0802D504(void);
void sub_0803662C(void);
void sub_08036884(void);
void sub_080368E8(void);
void sub_0803A07C(void);
/* Registered through sub_0801F024 by sub_0803A440, like sub_0803A07C. */
void sub_08039F80(void);
void sub_0805E5AC(void);
void sub_0805E718(void);
void sub_0805F4CC(void);
/* Stops DMA0. sub_0806F2C0, sub_080735D0 and sub_080736D8 register it through
 * sub_08011AAC. */
void sub_080735B0(void);

/* ---- Callees of two wrapper families ----------------------------------------
 * Family F003: `void f(void) { g(K); }`
 * Family F005: `void f(void) { g(); h(); }`
 * (family numbers from data/families.json) */

void sub_0800056C(u16);
void sub_0803B6E8(int);
void sub_08073900(s32);
void sub_08073C88(s32);

/* Called with 1 (by sub_08023348) or 0 (by sub_08023354). */
void sub_08023360(int);
void sub_0802776C(u8);
void sub_0803B930(u8);
/* Stores its argument in gUnknown_030005CE and calls sub_08071420
 * (MPlayVolumeControl). */
void sub_0803B35C(int);
/* The parameter is a full word; it is narrowed to s16 only where it is passed
 * to sub_08023168 and sub_08043418. */
void sub_08023274(int);
/* Same shape as sub_080230DC: three s16 inputs and two s16 results returned
 * through the pointers. */
void sub_08023168(s16, s16, s16, s16 *, s16 *);
void sub_0801B780(int);
/* The u16 parameter is not certain: it may be an `int` that is narrowed only
 * where it is passed to sub_0801A548. Its only known caller passes 0. */
void sub_0801A5B0(u16);

/* Takes four arguments that its body never reads; they are declared because
 * sub_08019DA8 passes them (0, 1, 6, 0xC). sub_08085298 (below) likewise takes
 * a proc pointer it never reads. */
void sub_0801A538(int, int, int, int);
/* Twin draw sweeps over gUnknown_080909A4 and gUnknown_080909B0; sub_0803A460
 * and sub_08047094 call them one after the other. */
void sub_08022580(void);
void sub_080227A8(void);
/* The per-tile helpers of those two sweeps; each takes a map cell (x, y). */
void sub_08022428(u16, u16);
void sub_08022618(u16, u16);
/* The same 16-step draw sweep as sub_08022580, one row at a time. */
void sub_08021D10(void);
/* sub_08021750's argument is an integer, not a pointer: it passes (u8)(arg +
 * 0x4c) to sub_0803CF3C. sub_080212AC's argument is an army slot that indexes
 * gPlayers and the halfword tables gUnknown_084995F4 and gUnknown_084995FE. */
void sub_08021750(int);
void sub_080212AC(u16);
/* Both take a pointer to an overlay plane of the gUnknown_08499590 map (from
 * +0x1E42, 0x508 bytes per plane). sub_080206E4's second parameter must stay
 * `int`: as `u16` each call in sub_080213AC gains a narrowing. */
void sub_08020754(u8 *);
void sub_080206E4(u8 *, int);
/* sub_08021810 returns two bytes through its two pointers. */
void sub_080213AC(void);
void sub_08021810(u8 *, u8 *);
void sub_08085298(ProcPtr);
void sub_080853B0(void);

/* Callees of the F005 wrappers. They take no arguments, so each wrapper is two
 * separate calls, not g(f()). All are void except sub_0801B4C0 (below). Three
 * more of them (sub_08023348, sub_08024584, sub_0803662C) are declared earlier
 * in this file. */
void sub_0800485C(void);
void sub_08012A74(void);
void sub_08016E3C(void);
void sub_08017208(void);
void sub_0801759C(void);
void sub_080199F8(void);
void sub_0801A664(void);
void sub_080258CC(void);
void sub_0803442C(u8 *src, u8 *dst);

/* The twelve cases of sub_0805FD64's jump table on gUnknown_030045D4
 * (sub_0805FE0C to sub_08060554). sub_0805FE0C and sub_0805FFA0 are not
 * decompiled yet, so their empty parameter lists are unconfirmed. The
 * declarations after the twelve, from sub_08034890 on, are more F005 callees.
 */
void sub_0805FE0C(void);
void sub_0805FF64(void);
void sub_0805FFA0(void);
void sub_08060324(void);
void sub_08060384(void);
void sub_080603D4(void);
void sub_08060424(void);
void sub_0806044C(void);
void sub_08060474(void);
void sub_080604A4(void);
void sub_0806050C(void);
void sub_08060554(void);
void sub_08034890(void);
void sub_08034F48(void);
void sub_08034F7C(void);
void sub_08035224(void);
void sub_08035354(void);
void sub_08042B70(void);
void sub_08042C10(void);
void sub_08013C54(void);
void sub_08013AFC(void);
void sub_0803B3EC(void);
void sub_080553C8(void);
void sub_08054C04(void);
void sub_0805AC88(void);
void sub_08061B4C(void);
/* The map editor's debug overlay: reads the unit under the cursor from the
 * +0x12 plane of the gUnknown_08499590 map, stores it in gUnknown_03003F38 and
 * gUnknown_030040D8, and prints five lines about it with sub_08013428, plus
 * four for each of the two units it carries. */
void sub_08062DF0(void);
/* The one F005 callee that returns a value. Its only caller ignores the result,
 * so the `int` width is a guess. */
int sub_0801B4C0(void);

u16 sub_0801B598(u8, void (**)(void));
u16 sub_0801B5E8(u16);
u16 sub_0801B618(u16, int);
int sub_0801B648(u16, int);
void sub_0801B66C(u16, int, int, int);
void sub_0801B6A8(u8 *, u32);

/* Checks the 0x1000-byte staging buffer: a four-byte magic, 0x55 and 0xaa at
 * the two ends, a byte checksum and its complement, and a constant 0xf. Returns
 * 0 when every check passes and 1 when any fails. */
int sub_0801B09C(void);


/* ---------------------------------------------------------------------------
 * Callees of the one-line forwarders `void f(void) { g(); }` (family F001
 * in data/families.json).
 * -------------------------------------------------------------------------- */

/* Functions marked with a file name are defined in that file. */
void sub_080039E4(void);
void sub_08002FE4(void);
void sub_080123EC(void);
void sub_08022A08(void);
void sub_08024830(void);
void sub_0802481C(void);
void sub_08026290(void);
void sub_080267AC(void);
void sub_0802D3B0(void);
void sub_08011B18(void);            /* src/decomp/c_08011B18.c */
void sub_08013378(void);
void sub_08037678(void);
void sub_0803B774(void);            /* src/decomp/c_0803B774.c */
void sub_0803C890(void);
void sub_08049BD8(void);            /* src/decomp/c_08049BD8.c */
void sub_08052F3C(void);
/* Called through the function table in data/data-0848B688.s, whose entries take
 * (u16, u16, int), like sub_08052E04. This function never reads its third
 * parameter. */
void sub_080523E8(u16, u16, int);
void sub_08057464(void);
void sub_080736D8(void);
void sub_080767A8(void);            /* src/decomp/c_080767A8.c */
/* The parameter is a byte id (sub_0803BBD4 passes gUnknown_0200C420.unk0d) that
 * is compared against the table gUnknown_081D938C, but it must stay `int`: as
 * `u8` the function gains a narrowing at entry. */
void sub_08080F54(int);
void sub_08078790(void);
void sub_08078864(void);
void sub_0808A6A0(void);
void sub_0808A3DC(void);
void sub_0808A47C(void);

/* Runs at the end of the "animation finished" branch of sub_08089F90 and
 * sub_08089C14, which pass it the struct they were called with. `void *`
 * because that struct is only defined locally in those files. */
void sub_08089464(void *);
/* Takes the proc that sub_08089A04 was called with; `void *` for the same
 * reason. */
void sub_08088ECC(void *);
/* Only the struct tag is declared here: c_080895E4.c defines the struct, and
 * callers cast to the incomplete type. */
struct Unk080895E4Proc;
void sub_080895E4(struct Unk080895E4Proc *);

/* The rest of sub_0808844C's dispatch table. Each takes one proc whose struct
 * is defined in the function's own file, so only the tags are declared here and
 * sub_0808844C casts to them. */
struct Unk08088CDC;
void sub_08088CDC(struct Unk08088CDC *);
struct Unk08088DA4;
void sub_08088DA4(struct Unk08088DA4 *);
struct Unk08089C14;
void sub_08089C14(struct Unk08089C14 *);
struct Unk08089F90;
void sub_08089F90(struct Unk08089F90 *);
struct Unk8A2F4Proc;
void sub_0808A2F4(struct Unk8A2F4Proc *);
struct Unk080897C8;
void sub_080897C8(struct Unk080897C8 *);
struct Unk08089A04;
void sub_08089A04(struct Unk08089A04 *);

/* Redraws the record screen. Only the tag is declared here: c_0807DA98.c
 * defines the struct. It is the same object that sub_0807D800, sub_0807D860 and
 * sub_0807D918 describe under their own local tags. */
struct Unk7DA98;
void sub_0807DA98(struct Unk7DA98 *);

/* Two empty functions, called from sub_0803AFA0 (through sub_0803AF78 and
 * sub_0803AF84). */
void sub_0801B4B8(void);
void sub_0801B4BC(void);

/* strcpy-like: copies the string `src` to `dst`. */
char * sub_0808B678(char *, const char *);

/* Takes an object, not a proc, that holds at +0x20 a pointer to an array of
 * 0x20-byte records with a callback at +0x0c, and at +0x24 one byte per record;
 * it also uses +0x31 and +0x42. sub_08019B80 walks the same object. */
void sub_08019B50(void *);

/* Proc callbacks. Each takes its proc as ProcPtr until the proc gets its own
 * struct; the trailing comments list the proc fields each one uses. */
void sub_08035F68(ProcPtr);         /* +0x36, and +0x39 via sub_08035E90 */
void sub_08035FA8(ProcPtr);         /* +0x36 */
void sub_0803927C(ProcPtr);         /* +0x2c, +0x30, +0x64 */
void sub_0807BF74(ProcPtr);         /* +0x58, a counter it decrements by 0x100 */

/* m4a sound driver. sub_0806F744 is SoundMain; it was written in assembly and
 * cannot come from C (see data/asm-resident.json). sub_080703B8 is
 * MPlayContinue; sub_080705D8 and sub_080705E4 are m4aMPlayContinue and
 * m4aMPlayAllContinue. */
void sub_0806F744(void);
void sub_080703B8(struct MusicPlayerInfo *);
/* More MP2K functions: sub_08070BAC is MPlayStart(mplayInfo, songHeader),
 * sub_08070C90 is MPlayStop(mplayInfo), and sub_080703D4 (below) is
 * MPlayFadeOut(mplayInfo, speed). */
void sub_08070BAC(struct MusicPlayerInfo *, struct SongHeader *);
void sub_08070C90(struct MusicPlayerInfo *);
/* Two per-track MP2K helpers taking (mplayInfo, track); both ignore mplayInfo.
 * sub_0807004C is TrackStop: it detaches every sound channel on the track,
 * silencing the CGB channels with CgbOscOff first. sub_080702C0 is ply_endtie:
 * it reads the next command byte and ends the tie on the channel whose key
 * matches. */
void sub_0807004C(struct MusicPlayerInfo *, struct MusicPlayerTrack *);
void sub_080702C0(struct MusicPlayerInfo *, struct MusicPlayerTrack *);
/* The rest of the m4a driver core, with their MP2K names:
 *
 *   sub_0806F7C8  code that sub_080703F4 copies into IWRAM; never called
 *                 in place, so its signature is unconfirmed
 *   sub_0806FDE4  MPlayMain, stored in SoundInfo::func by sub_08070B34
 *   sub_0807004C  TrackStop
 *   sub_080706B0  MPlayExtender
 *   sub_080707E0  Clear64byte
 *   sub_080707F4  SoundInit
 *   sub_08070A28  SoundClear
 *   sub_08070B34  MPlayOpen; the track count is at most 0x10 */
void sub_0806F7C8(void);
void sub_0806FDE4(struct MusicPlayerInfo *);
void sub_0807004C(struct MusicPlayerInfo *, struct MusicPlayerTrack *);
void sub_080706B0(struct CgbChannel *);
void sub_080707E0(void *);
void sub_080707F4(struct SoundInfo *);
void sub_08070A28(void);
void sub_08070B34(struct MusicPlayerInfo *, struct MusicPlayerTrack *, u8);
void sub_080703D4(struct MusicPlayerInfo *, u16);
/* Handlers that MPlayExtender (sub_080706B0) patches into the m4a command table
 * at gUnknown_03005740. Only their addresses are taken, so the empty parameter
 * lists of sub_08070328, sub_0807033C and sub_08070FAC are unconfirmed; whoever
 * matches one should fix its type and the cast in c_080706B0.c. */
void sub_08070328(void);
void sub_0807033C(void);
void sub_08070CD0(struct MusicPlayerInfo *);
/* The first parameter is never read. */
void sub_08070D98(struct MusicPlayerInfo *, struct MusicPlayerTrack *);
void sub_08070FAC(void);
/* The two parameter structs are defined in work/sub_0807166C/sub_0807166C.c;
 * only their tags are declared here. a->unk18 is a data page and b->unk40 the
 * command cursor. */
struct Unk0807166C_A;
struct Unk0807166C_B;
void sub_0807166C(struct Unk0807166C_A *, struct Unk0807166C_B *);
/* Sound hooks and helpers used by MPlayExtender. sub_080717C4's second
 * parameter struct is defined in c_080717C4.c; only its tag is declared here.
 */
void sub_08070EF4(u8);
/* CgbModVol: recomputes a CGB channel's volume and panning. sub_08070FAC calls
 * it. */
void sub_08070F44(struct CgbChannel *);
int sub_08070E4C(u8, u8, u8);
struct Unk80717C4Track;
void sub_080717C4(void *, struct Unk80717C4Track *);

/* Finds the gUnknown_0200C528 slot (of ten) running the given script and
 * returns its index, or -1. */
s16 sub_08019290(const u8 *);

/* Used by the gUnknown_0200C528 cursor-advance functions at
 * 0x08017E8C-0x0801903C. sub_08017E74 and sub_08017E80 set gUnknown_03001404 to
 * 1 and to 0. The empty parameter lists are required: those callers' code
 * depends on them. */
void sub_08017E74(void);
void sub_08013D40(void);
void sub_080198AC(void);
/* Returns 0 or 1. Must return bool8: its callers sub_08014400 and sub_08019510
 * test only the low byte. */
bool8 sub_08014BE8(void);
void sub_08017E80(void);
void sub_08042B9C(void);
/* sub_0801820C registers it as a callback through sub_08011AAC. */
void sub_08017EEC(void);

/* The six redraw passes that the four wrappers at 0x08023DCC-0x08023EA4 pass
 * their arguments to. All the arguments are u16. */
void sub_08023A4C(u16, u16, u16, u16);
void sub_08023BAC(u16, u16, u16, u16);
void sub_08023D14(u16, u16, u16, u16);
void sub_08023D48(u16, u16, u16, u16);
void sub_08023D7C(u16, u16, u16, u16);
void sub_08023DA4(u16, u16, u16, u16);


void sub_08037628(void);

/* Builders of the display list gUnknown_030058E0. sub_080785FC resets the
 * cursor gUnknown_03005944 to 0, and sub_08078758 sets the five words of
 * gUnknown_030059C0 to 1. The four `int f(int)` builders each write three or
 * four bytes starting at the given index and return the next index;
 * sub_08078864 chains them, passing each result to the next. */
/* sub_080781F0 returns 0 or 1. Must return bool8: sub_0803BBD4 narrows the
 * result. */
bool8 sub_080781F0(void);
void sub_080785FC(void);
int sub_08078608(int);
int sub_08078658(int);
int sub_080786A4(int);
int sub_080786F0(int);
/* Sets the five words of gUnknown_030059C0 to 0; the opposite of sub_08078758.
 */
void sub_08078740(void);
void sub_08078758(void);

/* Accessors of gUnknown_0200E438 used by the ~40 sprite-attribute setters at
 * 0x0804B180-0x08053614. The eight bytes sub_08015608 takes by value are a
 * struct OamData; see the note on that type in unknown-globals.h. */
void sub_0801566C(s16, struct UnkVec *);
void sub_08015608(s16, struct UnkVec);
void sub_08015928(s16, u32);

/* ---- Key input, and the 0x08015 block ---------------------------------------
 * sub_0801348C updates the key state and sub_08013510 calls it once per frame.
 * See struct Unk03002090 in unknown-globals.h for why its parameter is not
 * struct KeySt *.
 *
 * sub_0801DC04 indexes gUnknown_0200E438; its caller sub_08015438 then writes
 * those entries. It returns int: sub_08015438 casts the result to s8 itself.
 *
 * sub_080303B0, sub_080303C8 and sub_08030234 are called only by sub_08013510.
 */
struct Unk03002090;
void sub_0801348C(struct Unk03002090 *, s16);
int sub_0801DC04(void *, s16, s16);
bool8 sub_080303B0(void);
u16 sub_080303C8(void);
void sub_08030234(void);
/* ---- Callees of the 0x08015 slot accessors ----------------------------------
 *   sub_080151B0  initialises a slot: the script goes in .unk00 and
 *                 .unk04, the last argument in .unk14
 *   sub_08015A30  runs a slot's script through gUnknown_0848A160
 *   sub_08015224  twin of sub_0801527C; stores 0 in the slot's .unk12
 *                 where sub_0801527C stores 4
 *   sub_08015438  its fourth parameter is really a small signed
 *                 integer, not a pointer; the third and fourth are
 *                 `void *` to agree with its caller sub_08015410
 *   sub_0801D778  takes four arguments; sub_0801D804 takes three
 *   sub_0801D9E4  returns two s16 values through its two pointers */
void sub_080151B0(const void *, u8, u8);
void sub_08015A30(u8);
s8 sub_08015224(const void *, s16, u8);
s8 sub_08015438(void *, int, void *, void *, int);
int sub_0801D778(int, int, int, int);
int sub_0801D804(int, int, int);
void sub_0801D96C(int, s16, s16);
void sub_0801D9E4(int, s16 *, s16 *);
u32 sub_0801DA44(int);
u32 sub_0801DA54(int);
/* The rest of the 0x08015 block:
 *
 *   sub_0801527C  twin of sub_08015224 and the worker behind
 *                 sub_080152EC; returns its second argument as an s8
 *   sub_08015578  calls sub_0801D778 (four arguments) when the first
 *                 argument is not 0xff, else sub_0801D804 (three,
 *                 without it)
 *   sub_080155A0  sub_08015578(0xff, a, b, c)
 *   sub_080155E8  passes its two pointers on to sub_0801D9E4
 *   sub_080156A0  like sub_080156C4, but through sub_0801DA44
 *   sub_08015CE4  calls sub_08015328(a) and returns 0 */
s8 sub_0801527C(const void *, u8, u8);
s8 sub_08015578(s16, int, int, u8);
s8 sub_080155A0(int, int, u8);
void sub_080155E8(s16, s16 *, s16 *);
int sub_080156A0(s16);
int sub_08015CE4(u8);
/* sub_08015158 counts the free gUnknown_03001470 slots (.unk00 == 0).
 * sub_08015CF4 runs one step of a slot's command list: it advances the .unk04
 * cursor by 8 and returns 1. */
int sub_08015158(void);
int sub_08015CF4(u8);
/* Returns sub_0801DA54(gUnknown_03001470[slot].unk26). Returns int: both
 * callers narrow the result to u16 themselves. */
int sub_080156C4(s16);
/* A continuation callback, passed to sub_08015928 cast to u32. Like
 * sub_0804E4CC below, it passes its first argument, an id, to sub_0801566C and
 * keeps only the top six bits of the halfword at +4 of its second. */
void sub_0804DA40(s16, u16 *);
/* Same shape as sub_0804E100: the second parameter is never read, but the
 * caller passes the slot index. */
void sub_0804D25C(u16, int, u16);
/* Takes a side (0 or 1), which indexes gUnknown_020296B0 and gUnknown_030045A4.
 */
void sub_08053520(u16);
void sub_080535E0(void);
void sub_08053820(void);
void sub_0804BA4C(void);
/* Called at the end of sub_08053670 with a side index; the third argument is a
 * loop count. The second parameter is never read. */
void sub_080536D8(u16, u16, u16);
/* Takes the side index from sub_08053670. Must stay `int`: as `u16` it gains a
 * narrowing at entry. */
void sub_08057BCC(int);
/* The second parameter is never read, but the caller sub_0804E050 passes the
 * slot index. */
void sub_0804E100(u16, int, u16);
/* A continuation callback: passes its first argument, an id, to sub_0801566C
 * and keeps only the top six bits of the halfword at +4 of its second. Callers
 * pass its address cast to u32. */
void sub_0804E4CC(s16, u16 *);
/* sub_0804E214's continuation, the same shape as sub_0804E4CC. Its second
 * parameter struct is defined in c_0804E334.c; only the tag is declared here.
 */
struct Unk4E334;
void sub_0804E334(s16, struct Unk4E334 *);

/* ---- The 0x08003/0x08004 map-stamp block ------------------------------------
 * sub_08003B8C calls sub_08003ED0, sub_08004724, sub_080040C8 and sub_0800449C.
 * sub_0800401C stamps a rounded rectangle: (cx, cy, w, h, value). */
/* sub_08003B6C is the block's random-number helper, called as
 * sub_08003B6C(range, base). */
int sub_08003B6C(int, int);
void sub_08003DC4(int, int, int);
void sub_08003F44(int, int, int);
void sub_08003ED0(void);
void sub_0800401C(int, int, int, int, int);
void sub_080040C8(void);
void sub_0800449C(void);
void sub_08004724(void);

/* sub_08001D04 looks up its argument in the byte pairs at gUnknown_084859E0 and
 * returns the second byte of the matching pair, or 14 when there is none.
 * sub_0803F6BC loads graphics into VRAM: the first argument selects a case
 * (6..17), the third is the destination, and a zero fourth argument skips
 * everything. */
int sub_08001D04(int);
void sub_0803F6BC(int, int, void *, int);
/* The argument is a map tile value; the result is passed to sub_0802BD54. */
u32 sub_0800C8A0(int);

/* The deferred-copy queue (58 callers): appends (src, dst, size) to
 * gUnknown_0200B3B4 and returns the slot index, -1 when all 0x30 slots are
 * full, or 0 when gUnknown_030044D0 says to copy at once. Returns s16, like its
 * five siblings on the same queue (sub_08011D10, sub_08011D7C, sub_08011DE8,
 * sub_08011EF0, sub_08011F70). */
s16 sub_08011E54(void *, void *, u16);
/* Copies the tiles of picture `id` to VRAM with CpuFastSet: the size comes from
 * the (width, height) byte pair at gUnknown_0848B780 + 4 * id, and the
 * destination is base + (tile & 0x3FF) * 0x20. */
void sub_0801F19C(int, void *, int);
/* Return gUnknown_0810BE60 and gUnknown_0810E820. */
u8 *sub_08026190(void);
u8 *sub_08026198(void);
/* Returns gUnknown_08499608[sub_08042DE0(a) - 1][b] << 2, a tile index (the
 * rows are 0x32 halfwords). */
int sub_080261A4(int, int);

void sub_08001D8C(void);
void sub_08001D9C(void);
void sub_08003934(void);
void sub_08003948(void);
/* sub_08003A80 is defined in c_080039E4.c. */
void sub_08003A80(int, int, int, int);
void sub_080039BC(void);
void sub_080039D0(void);
void sub_08012BC8(u16 *, u16, u16, u16, u16, u16);
void sub_08023518(void);
void sub_08023824(void);
void sub_08023908(int);
void sub_0803CE28(int, int);
void sub_0803CEAC(void);

/* Callees of the sub_08023360 screen setup. sub_08011C68 copies `size` bytes
 * to a VRAM address: with CpuFastSet when size is a multiple of 32, with
 * CpuSet otherwise. Its source is `const void *` because callers pass every
 * kind of pointer. */
void sub_08010FE0(void);
void sub_08011018(void);
void sub_080116E8(void);
/* The size must stay u16: spelled int with casts, the body compiles
 * differently. sub_08011C90 takes u16 to match. */
void sub_08011C68(const void *, void *, u16);
void sub_080128D0(void);
void sub_0801A57C(u16);
void sub_08022A34(void);
void sub_08023860(void);
void sub_08024C58(struct BattleUnit *, int, u8);
void sub_08024A2C(struct BattleUnit *, s16);
/* sub_08024ABC and the three sub_080433xx accessors below take struct
 * BattleUnit: every caller passes gBattleAttacker or gBattleDefender. */
struct BattleUnit;
void sub_08024ABC(struct BattleUnit *, struct BattleUnit *, s16, u8);
/* sub_08024F20 passes gUnknown_030013D0 and gUnknown_030013B0 here. Every
 * file must use this same struct tag: with two different tags for one
 * object, agbcc emits two pool words where the ROM shares one. */
void sub_08024ED8(struct BattleUnit *, struct BattleUnit *);
void sub_08024E60(struct BattleUnit *, struct BattleUnit *);
int sub_08043304(struct BattleUnit *);
int sub_0804334C(struct BattleUnit *);
int sub_0804338C(struct BattleUnit *);
int sub_08042CF8(int, int);
int sub_08042E64(int);
int sub_08042EBC(int);
void sub_08023DCC(u16, u16, u16, u16);
void sub_08023E14(u16, u16, u16, u16);
void sub_08023E5C(u16, u16, u16, u16);
void sub_08023EA4(u16, u16, u16, u16);
int sub_080261A0(void);
void sub_0802D2EC(void);
void sub_08035020(u16);
void sub_080354FC(void);
void sub_08035568(void);
void sub_08037150(int);
void sub_0803F80C(int);
void sub_08043834(int);
void sub_080546BC(void); /* src/decomp/c_080546BC.c */
/* Returns its second argument, as u16. The body never reads the third
 * parameter, but sub_080339B0 passes one. */
u16 sub_080315E8(u16, u16, int);
/* Signature read from its call site only; the body may take parameters that
 * the call site cannot show. */
void sub_08031824(void);
void sub_08033930(void);
void sub_0803D48C(void);
void sub_08085AF4(void); /* src/decomp/c_08085AF4.c */
/* sub_08027B10 starts a proc: arguments 1-4 are stored at +0x2c..+0x38 and the
 * fifth is the parent. sub_0802813C returns
 * gUnknown_08499E38[gUnknown_02028E40]. */
void sub_08027B10(int, int, int, int, ProcPtr);
void *sub_0802813C(void);
/* Callees of sub_0802966C. sub_0802E7C8's first, second and fourth parameters
 * must stay int although its body narrows them: sub_0802966C passes
 * sign-extended values and -1, which narrow parameters would change. */
void sub_08015328(s16);
void sub_08015C30(u8);
void DebugVersusPauseScreen(void); /* sub_080283E4; see src/decomp/c_080283E4.c. */
void sub_080294FC(void);
void sub_08029570(void);
void sub_08029868(u8);
void sub_0802D558(void);
int sub_0802E7C8(int, int, void *, int);
void sub_08034F8C(void);
int sub_080357E0(u16, u16, u16, u16, void *);
bool8 sub_0802706C(u8, u16, u16);
/* Classifies the map cell at (x, y): 0 when it is empty, 1 when its unit is
 * idle, 2 otherwise. */
u8 sub_0802B6C8(u8, u8);
void sub_0802B750(void);
bool8 sub_0802CBC8(void);
void sub_080428F0(s16);
void sub_080272B4(void);
void sub_08028EE4(void);
void sub_0803446C(void);
/* The body never reads its parameter, but sub_080293C8 passes its own proc, so
 * the parameter stays. */
bool8 sub_08029490(ProcPtr);
void sub_0802B768(void);
void sub_0802B7E8(void);
/* Returns 0 or 1, but as s16: sub_0802A258 tests the result as a halfword,
 * which a byte-wide return would not match. */
s16 sub_0802A1E4(s16, s16);
/* Allocates a sub_080152EC slot and fills it from a cursor pair, a distance
 * and a flag. The second parameter is never read but must stay, so that the
 * later arguments keep their positions. */
void sub_08029CB8(struct Unk802C57C *, u8, int, u8);
int sub_08029D1C(void *);
int sub_08029DBC(int, int);
void sub_08029DF8(struct Unk03001470 *);
void sub_08029FC4(void);
void sub_08029FE4(void);
/* Only its address is used (sub_0802A7C4 passes it to sub_0801F024), so this
 * signature is a placeholder; the caller casts. */
void sub_0802AA78(void);
/* Set gUnknown_030030F0.unk02 to 1 and to 0. */
void sub_0803BD54(void);
void sub_0803BD60(void);

/* Helpers of the sprite-attribute setters reached from sub_0804D290 and
 * sub_0804DCA8. sub_080155C0 sets a position. sub_08057D44 returns
 * gUnknown_08555450[a2][a1], which callers use as the base of an array of
 * halfword pairs. */
void sub_080155C0(s16, s16, s16);
/* Argument 3 is s16: two callers pass it sign-extended. The other three are
 * u16 for lack of a caller that shows a sign. */
void sub_0804BCB8(u16, u16, s16, u16);
/* Takes a struct Unk56E28 record; sub_0804BCB8 builds one on its stack. */
void sub_08056E28(struct Unk56E28 *);
u32 sub_08057D44(int, int);
/* Callbacks handed to sub_08015928 by sub_0804C6DC and sub_0804CC38, which use
 * only their addresses and cast. Each takes a proc id (passed on to
 * sub_0801566C) and a pointer whose halfword at +4 it masks with 0xfc00. They
 * are sub_0804E4CC's shape with tile pitches 0x30 and 0x38. */
void sub_0804C8C8(s16, u16 *);
void sub_0804CE24(s16, u16 *);

/* ---- sub_0802E4B4's callees ----
 * sub_08074320, sub_08035584 and sub_080202A4 take a pointer to one
 * gUnknown_08499594 element. They are declared on struct Unk030040D8 *, the
 * type of the global every caller passes (see unknown-globals.h). */
u8 sub_080242B0(s16, s16);
void sub_0802D5E8(s16, s16);
void sub_0802D458(void);
/* Returns u8, not void: it is one of the six identical null-guards described
 * at sub_080742FC, and void changes its body. Callers ignore the result. */
u8 sub_08074320(struct Unk030040D8 *);
void sub_08074754(s16);
/* sub_08074C84 picks the camera-relative pair (sub_08074BDC, sub_08074C1C) or
 * the absolute pair (sub_08074C5C, sub_08074C70) on its fourth argument and
 * clamps a scroll target with it. */
int sub_08074BDC(int);
int sub_08074C1C(int);
int sub_08074C5C(int);
int sub_08074C70(int);
/* sub_08035584 returns the proc from sub_080355CC, or NULL. */
ProcPtr sub_08035584(struct Unk030040D8 *);
void sub_08024454(void);
/* sub_0801F92C rebuilds the gUnknown_03003340 row-pointer table for one plane
 * of the map: row y starts at a1 + rowOffset[y]. Callers pass
 * gUnknown_08499590 plus a plane offset (0x2852, 0x2D5A or 0x193A). Keep it
 * `u8 *`, not `u8 **`: the ROM's extra load comes from a compiler-made pool
 * word holding &gUnknown_08499590. */
void sub_0801F92C(u8 *);
void sub_080202A4(struct Unk030040D8 *);
void sub_08022990(int, int, u16);
void sub_08038C98(void);

/* sub_0802E2D0's callees sub_08024404, sub_0802E2BC, sub_080201E0,
 * sub_0803E9F8, sub_08041FE0, sub_0804203C and sub_0801FE68 are defined in
 * src/decomp/ and deliberately not declared here. Before adding any prototype,
 * copy the signature from the definition if one exists: a call site can hide
 * parameters and return widths. When a caller passes an argument that a
 * matched callee ignores, fix the caller's own declaration, never the callee.
 */

/* ---- sub_080345C8's state table (gUnknown_030032D8) ----
 * Its handlers are all void(void). */
void sub_0802DC2C(void);
void sub_08034350(void);

/* ---- Callees of the 0x08034000 block ---- */
void sub_0802150C(void);
void sub_0802BB98(void);
void sub_08034FA4(void);
void sub_08025E74(void);
void sub_0803DE68(void);
void sub_08021598(void);
void sub_080215B8(void);
void sub_080215D0(void);
void sub_0802FA64(void);
void sub_0805FD64(void);
void sub_08034394(void);
void sub_080343D8(void);
void sub_08034598(void);
void sub_08034780(void);
void sub_08034838(void);
int sub_08020824(u16, u16);
int sub_08020864(u16);
int sub_080208C8(int);
void sub_08020634(u8 *, u8 *);
void sub_0802163C(int);
void sub_080638D0(int);
int sub_08034380(u8 *);
void sub_08034400(u8 *, u8 *);
/* The first parameter is void * because callers pass different objects
 * (gUnknown_03004400, &gUnknown_030046C0). The second is a predicate run over
 * the object, or 0. Callers compare the result with -1. */
s16 sub_080309AC(void *, int (*)(u8 *));
/* The twin of sub_080309AC. Its body tests the predicate's result as a byte,
 * so the predicate's declared int return (kept to agree with sub_080309AC) is
 * uncertain. */
s16 sub_08030B00(void *, int (*)(u8 *));
/* ---- Callees of the 0x08033000 block ---- */
/* Resets every per-army table behind gUnknown_0849B018 and gUnknown_0849B01C.
 */
void sub_0802F03C(void);
/* sub_0802F348's two other reset helpers. */
void sub_0802F23C(void);
void sub_0802F28C(void);
/* The link-record reset. */
void sub_0802F348(void);
/* sub_08030CCC's predicate; returns 0 or 1. */
int sub_08030D1C(void);
/* The blend-setup tail that sub_08030F60 calls. */
void sub_08030F20(void);
/* Sends a halfword over the serial link: writes *a1 to SIOMLT_SEND until the
 * register reads back the same value. Returns -4 on the early exit and 5
 * otherwise; callers ignore it. */
int sub_0802F8FC(u16 *, int);
/* Link-state entry points in the 0x08030000 block. sub_08030838 copies one
 * struct Unk08090CD8Entry, header and payload, into
 * gUnknown_0849B018->unk12c[]. */
void sub_08030584(void);
void sub_08030600(void);
void sub_08030670(void);
void sub_080306E4(void);
void sub_08030768(void);
void sub_08030838(struct Unk08090CD8Entry *);
/* More entry points that sub_0802FACC reaches. sub_0802F6A0 pops the receive
 * queue; its second parameter is the packet buffer at gUnknown_0849B018 +
 * 0x2c, void * because callers read it through several packet layouts.
 * sub_080307E0 writes a length to *a1 and returns
 * &gUnknown_0849B018->unk12c[unk1aac]; sub_0802FACC casts that before passing
 * it to sub_0802F588. */
void sub_0802EA24(void);
s16 sub_0802F6A0(s8, void *);
struct Unk08090CD8Entry *sub_080307E0(int *);
void sub_0802FA9C(u8);
void sub_08030038(s16, struct Unk02025564 *);
/* Arms timer 3 with a reload of -cycles. */
void sub_0802ECEC(int);
/* Declared without a prototype: its parameter points at the link-session
 * record at gUnknown_03003F70, which has no struct type yet. Returns a status
 * code that all three callers ignore. */
int sub_08062FF4();
void sub_08031430(void);
void sub_08031CE4(void);
void sub_08031E6C(void);
void sub_08032468(void);
void sub_08032D60(void);

/* ---- Callees of the 0x0801A000 block ---- */
void sub_0801AFF4(void);
bool8 sub_0808AB8C(void);
void sub_0801B2FC(int);

/* ---- The 0x0800B000 and 0x0808B000 blocks ---- */
/* Takes the (x, y) cell pair, like sub_0800BC98. */
int sub_0800BCD0(int, int);
/* Returns without Thumb interworking, as sub_0808BBA4 does; the rest of this
 * block interworks. Its one caller ignores the result. */
void sub_0808BB58(void);
/* Calls sub_0808BB58 once only, guarded by gUnknown_03000F80. */
void sub_0808BBA4(void);

/* ---- Callees of the 0x08053000 block ---- */
/* Per-channel step functions that sub_08053F50 and sub_08053F90 call with 0
 * and 1, like sub_08053660. */
void sub_08053FBC(u16);
void sub_0805414C(u16);
void sub_08054278(u16);
void sub_08054488(u16);
/* Block 0x08054000: the cutscene player's per-side steps. sub_08054E8C returns
 * 1, 2 or 3 from its switch, or its fourth argument unchanged. */
void sub_080541F0(u16, u16);
void sub_080542EC(u16, u16);
void sub_080543E0(u16, u16);
void sub_08054500(u16, u16);
void sub_08054BA0(void);
void sub_08054C5C(void);
u16 sub_08054E8C(u16, u16, u16, u16);
void sub_08054EE0(u16, u16);
void sub_08054F50(u16, u16);
void sub_08055004(u16, u16);
/* The argument order differs: sub_08055768 takes (side, count) and
 * sub_08055A38 takes (count, side). */
void sub_08055768(u16, u16);
void sub_08055A38(u16, u16);
void sub_08057AE8(void);
void sub_0805198C(u16, u16);
void sub_08050F24(u16, u16);
void sub_0804B744(u16, u16);
void sub_0804B330(u16);
void sub_08057BDC(void);
/* The four battle-animation entry points that sub_0805DCA4 and sub_0805DFB8
 * select between. */
void sub_08059760(void);
void sub_08059824(void);
void sub_080598BC(void);
void sub_08059978(void);
void sub_08053F0C(void);
void sub_0804B3CC(void);
void sub_08053860(void);
void sub_08053BB8(void);
void sub_08053660(u16);
/* Returns a string's length in characters; sub_08034A44 centres text with it.
 */
u32 sub_0808B6B0(const char *);
/* More void(void) callees of the 0x08034000 block. */
void sub_08028CF4(void);
void sub_08037F80(void);
/* sub_080742FC and sub_08074460 return u8, not void: they are two of six
 * identical null-guards around one slot of struct Unk08074584. Callers ignore
 * the result, but void changes the bodies. */
u8 sub_080742FC(void);
u8 sub_08074410(int, struct Unk030040D8 *);
u8 sub_0807443C(void);
const struct Unk08074584 *sub_08074584(void);
const struct Unk085C77A0 *sub_08035000(int);
u8 sub_08074484(u8 *, struct Unk030040D8 *, int);
/* Returns a pointer into the same 8-byte-stride record array it is passed;
 * sub_08074484 keeps walking the array from the result. */
u8 *sub_08074570(u8 *);
void sub_0802817C(void);
u8 sub_08074460(void);
bool8 sub_0803B628(void);
/* Declared int although it has no return statement: each switch arm ends in a
 * call to a void function, and callers ignore the result. void changes its
 * epilogue, so it must stay int. */
int sub_08043DAC(u8);
/* Returns the x that centres string s on a 240-pixel line. */
int sub_08034A44(const char *);
void sub_08034938(void);
void sub_080349E4(void);
void sub_08034AF8(void);
void sub_08034C90(void);
void sub_08034CA4(void);
void sub_08034CB8(void);
void sub_08034CD4(void);
void sub_08034D18(void);
void sub_08034DB0(void);
void sub_08034DCC(void);
void sub_08034DF8(void);
void sub_08034EA4(void);
void sub_08034ED0(void);
void sub_08034EF0(void);
void sub_08034F1C(void);
void sub_0806171C(void);
/* Returns int, not s8: sub_080345C8 tests the result without narrowing it. */
int sub_08034F6C(void);

/* ---- sub_080355CC's callees ----
 * sub_0803649C returns the index of a free slot in gUnknown_03003124[0..2], or
 * -1. sub_0801C210 returns sub_0801C6E8's allocation or NULL. sub_08035B3C
 * returns the compressed graphic that sub_080355CC decompresses. */
s16 sub_0803649C(void);
void *sub_080364C4(void);
void *sub_08035B68(u16);
struct Unk0801C210 *sub_0801C210(void *, u16, u8);
void sub_0801C4D4(struct Unk0801C210 *, int);
/* Called by sub_080272C4 and sub_08027428 with three proc words and a
 * sub_0801C210 result, which it passes on to sub_0801C4D4. */
void sub_08027560(int, int, int, struct Unk0801C210 *);
s16 sub_08035AE8(s16);
s16 sub_08035B00(u16);
u8 *sub_08035B3C(ProcPtr);
void sub_080359A4(ProcPtr);
/* sub_080359A4's callees. */
/* Both parameters must stay s16: with int the body needs an extra local and
 * saves more registers. */
void sub_080358C4(s16, s16);
/* Parameter 1 is a unit record in gUnknown_08499594 and may be NULL. */
u8 sub_080255F4(struct Unit *, s16, s16);
/* Returns sub_0801C2DC's result, or 0 on the early exit. */
u8 sub_0801C254(struct Unk0801C210 *, int, int);

/* ---- The neighbour scans of sub_080255F4 and sub_080257C0 ----
 * All four return a byte. sub_08025598 and sub_08025744 have the same body but
 * different parameter types (s16 and int); each type matches its callers, so
 * do not unify them. */
bool8 sub_08026F5C(s16);
u8 sub_08025598(s16, s16);
u8 sub_08025744(int, int);
u8 sub_080257C0(u16);

/* ---- sub_08040640's callees ----
 * sub_0801C70C takes six arguments, two on the stack, and returns a value. Its
 * first is const void * because callers pass ROM data. */
void sub_08026100(int, int, int);
int sub_0801C70C(const void *, int, int, int, int, u16);
/* Returns the first gUnknown_02028360 record whose 4-bit field at bits 6..9 of
 * +0x02 equals the argument, or NULL; the walk stops at a record whose field
 * is 0. */
struct Unk02028360 *sub_0803E354(int);
void sub_0803FF2C(ProcPtr);

/* ---- Callees of sub_08053860 and sub_08053BB8 ----
 * sub_08053614 takes (procId, palette); procId is signed and compared with -1.
 */
void sub_08053614(s16, u16);
void sub_0805741C(u16);

/* Both take the (x, y) cell pair as int. sub_08007F14's third argument is a
 * tile value from the u16 array at +0x0A22. */
int sub_080015E4(int, int);
void sub_08007F14(int, int, int);

/* ---- Callees of sub_08000E48 ----
 * sub_08001124 clears a block; sub_08000E48 casts its argument to u8 *. */
void sub_08001124(u8 *, int);
void sub_08003910(void);
int sub_08007328(void);
/* The second parameter must stay int: sub_08005F4C passes it sign-extended,
 * and a narrow parameter would re-extend it. */
void sub_080077EC(int, int);
/* s8, not u8: sub_08005F4C passes gActiveMap->selectionAnimKind sign-extended.
 */
void sub_080078D4(s8);
void sub_080078E4(int, int);
void sub_08002EB4(void);

/* ---- The tile-edit helpers that sub_0800B244 drives ----
 * All take the (x, y) cell pair as int. sub_0800B1FC returns -1, 0 or 1. */
void sub_08001158(int, int, int);
int sub_0800119C(int, int, int);
int sub_08001704(int, int, int);
int sub_08001A04(int, int, int);
int sub_0800AFCC(int, int);
int sub_0800B1FC(int, int);

/* ---- More cell predicates on (x, y) ----
 * All return a signed int; sub_0800A798 returns -1 on one path. */
int sub_080094EC(int, int);
int sub_08009B38(int, int);
int sub_08009B84(int, int);
int sub_0800A798(int, int);

/* Three more on the same (x, y) key, from sub_0800BF78. */
int sub_0800BC98(int, int);
void sub_0800C124(int, int);
void sub_0800C22C(int, int);

/* Two more, from sub_0800977C. */
int sub_08009720(int, int);
int sub_08009BF4(int, int);

/* ---- Window frames ----
 * sub_0801A368 draws a window frame into one of the four 0x800-byte tilemap
 * buffers: a top strip (sub_0801A1D8), `height - 2` middle rows (sub_0801A240)
 * and a bottom row (sub_0801A2E4), 32 entries apart. It then marks that buffer
 * for copying with sub_08013AD4(0..3). */
void sub_0801A1D8(u16 *, int, s16, int);
void sub_0801A240(u16 *, int, s16, s16, int);
void sub_0801A2E4(u16 *, int, s16, int);
void sub_0801A368(int, int, int, int, u16 *, int);

/* ---- Screen-setup callees of sub_08065990 and sub_0806D944 ---- */
void sub_08013B0C(void);
void sub_08013B1C(void);
void sub_08013CA8(void);
void sub_0801A444(s16, s16, s16, s16);
/* Callees of the 0x0807B000 proc tree. Each takes the calling proc as void *,
 * because each caller models the proc with its own local layout. */
void sub_0807C034(void *);
void sub_0807C278(void *);
void sub_0807C2D4(void *);
void sub_0807C46C(void *);
void sub_0801F114(void);
/* Argument 4 must stay int: the body stores it with a byte store, and a u8
 * parameter would add a narrowing. */
void sub_0801F150(int, void *, u16, int);
void sub_0801F234(int);
/* A PutSpriteExt front end: arguments 2 and 3 are the x|flags and y|flags
 * words, and arguments 1 and 5 select the OBJ data from gUnknown_0848B780 and
 * gUnknown_0848BAE4. */
void sub_0801F34C(int, int, int, int, int);
u8 *sub_0801F49C(void);
void sub_0802D5A0(void *, int, int);
void sub_0802D5CC(int, int);
void sub_08065238(void);
void sub_0806574C(void);
void sub_0806D268(void);
void sub_0806D53C(void);
void sub_0806D620(void);
void sub_0806D820(void);
void sub_0806D850(void);
void sub_0806DE38(void);
void sub_0806DF20(void);
void sub_0806DF58(void);
void sub_0806D8B8(void);
/* Starts a gUnknown_086140D4 proc and returns it; current callers ignore the
 * result. */
ProcPtr sub_08073304(const void *, void *, u16, u16, u16, u8, int);
void sub_080733B8(void);
/* Called only by sub_080772B8, with an id from gUnknown_08615194[..].unk00,
 * the ROM table gUnknown_086145C8, and an 8-byte buffer that the caller reads
 * back as five u8 digits. */
void sub_080733C8(int, const void *, void *);
/* Writes the decimal digits of its second argument backwards from its first,
 * one halfword per digit: the digit + 0x32, a tile index. */
void sub_0807728C(u16 *, int);
/* sub_08073930 is the HBlank handler that sub_08073A00 installs through
 * sub_08063928; nothing calls it directly. sub_08073998 is the window-line
 * generator that sub_08073B00 drives; its fifth argument is 0 or 1. */
void sub_08073930(void);
void sub_08073998(int, int, int, int, int);
/* Swaps the gUnknown_0202FDE0 / gUnknown_0202FDE4 double buffer. */
void sub_08073AE8(void);
void sub_08063928(int);

/* The two payload handlers that sub_0804E8F0 and sub_0804FE10 pick between. */
void sub_0804EA54(u16, u16, u16);
/* Called by sub_0804EB78 and sub_0804F3C8 with a (side, slot) pair, plus
 * gUnknown_03001FBC for the three-argument ones. */
void sub_0804EDAC(u16, u16, s16);
void sub_0804EE08(u16, u16, s16);
void sub_080520B8(u16, u16);
void sub_0804EAEC(u16, u16, u16);

/* ---- fabsf ----
 * sub_0808BB0C is fabsf; the ROM's sinf (sub_0808B710) calls it. */
float sub_0808BB0C(float);

/* ---- The tile-action handlers that sub_080085E0 dispatches to ----
 * All take the (x, y) cell pair as int. sub_0800B528's result is signed;
 * callers test it for < 0. */
void sub_080011F4(int, int, int);

/* Returns int, not u16 (values up to 0x1D7); its caller sub_0800C454 narrows
 * the result itself. */
int sub_080012DC(int);
void sub_08007CA0(int, int);
/* Blocks 0x08007000 and 0x08008000. The (int, int) pairs are the (x, y) cell
 * coordinates that the rest of the tile code takes. */
void sub_08007354(void);
void sub_080079B8(int);
void sub_08007A30(void);
void sub_08007BA4(int, int);
void sub_08007C04(int, int);
void sub_080080F8(int, int);
int sub_08008928(void);
/* sub_08008A8C's first parameter is a mode flag (1 = install the window and
 * palette state, 0 = clear the record's unk00); the other two are the (x, y)
 * cell pair. */
int sub_08008A8C(int, int, int);
int sub_08008D70(int, int);
void sub_08008E3C(int, int);
void sub_0800A588(int, int);
void sub_0800ABD0(int, int);
void sub_0800B048(int, int);
s16 sub_0800B61C(int, int);
int sub_0800F418(int, int);
void sub_08010664(int, int);
void sub_08010ADC(int, int);
void sub_080088F0(void);
int sub_08008B70(int, int);
int sub_08008CB8(int, int);
int sub_08008D14(int, int);
void sub_08008BB8(int, int);
void sub_08008F6C(int, int);
int sub_08009F10(int, int);
void sub_0800AF74(int, int);
int sub_0800B528(int, int);
/* sub_0800BA9C returns 0 or 1; its only caller ignores the result. */
int sub_0800BA9C(int, int);
void sub_0800BEE4(int, int);
int sub_0800BF78(int, int);
void sub_0800C454(int, int, int);
void sub_0800C608(int, int);
int sub_0800C840(int, int);
void sub_0800CF28(int, int);
void sub_0800EC20(int, int);
void sub_0800F4E0(int, int);
void sub_08010D28(int, int);
void sub_08010D80(int, int);

/* ---- sub_080430B0 ----
 * Takes three indices, each scaled into its own table. Returns a small signed
 * value; sub_08085410 switches on it over -30..80. */
int sub_080430B0(int, int, int);

/* sub_08069EAC passes 0 or 1, a direction or side flag. */
void sub_08069D3C(int);

/* The glyph blitter that sub_0801172C dispatches to. */
void sub_08011704(u16, u16, u16);

int sub_0801172C(u16, u16, u8);

/* The 0x0806A054 screen-setup group. sub_080677BC starts gUnknown_08580FE4
 * under its fourth argument and stores the first three at +0x58, +0x34 and
 * +0x38. */
int sub_080674F4(int);
void sub_080670F8(const u8 *);
void sub_08069FD0(void);
ProcPtr sub_080677BC(s32, s32, s32, ProcPtr);

/* The two debug-text primitives that sub_08057464 drives: sub_080119A0 draws a
 * string at (x, y) and sub_08011A20 a number. */
void sub_080119A0(u16, u16, const char *);
/* The value is u32: the body divides it with unsigned division. */
void sub_08011A20(u16, u16, u32);
/* The hex sibling of sub_08011A20. */
void sub_080119D4(u16, u16, u32);

/* sub_0806775C starts a proc under its second argument. sub_080718F8 is not a
 * function: it is the linker's THUMB-to-ARM veneer for the ARM routine
 * sub_0800043C, and C must call the veneer because the build places those four
 * bytes as their own unit. Its declared parameters are sub_0800043C's: a byte
 * cursor into gBG3TilemapBuffer, ROM data and 0. */
void sub_0806775C(int, ProcPtr);
void sub_080718F8(void *, u8 *, int);

/* Declared without a parameter list on purpose: c_0801C240.c defines it on
 * struct Unk_0801C240, a type local to that file, and a prototype naming any
 * other pointer type breaks that file with `conflicting types`. That struct is
 * the object sub_0801C2DC works on. */
void sub_0801C240();

/* sub_08011BD4 returns s16. The two sub_08021Dxx animation starters take a
 * selector 0..7. */
s16 sub_08011BD4(void);
void sub_08021D64(int);
void sub_08021DA0(int);

/* The BIOS fast fill/copy. Bit 24 of the third argument selects fill; the low
 * 21 bits are the word count. */
void CpuFastSet(const void *, void *, u32);
/* The BIOS affine-matrix helper. gUnknown_030024D0, the usual destination, is
 * declared volatile u32 [4] in hardware.h, so callers cast it. */
void BgAffineSet(struct BgAffineSrcData *, struct BgAffineDstData *, s32);
void sub_08013928(int);
/* Takes a graphics slot index (scaled by 0x44 into gUnknown_084A0090) and a
 * tile number that becomes a VRAM offset. */
void sub_08043BF8(int, int);
/* ---- The rest of the 0x08043A00 graphics-slot block ----
 * Each takes a slot index into gUnknown_084A0090 as its first argument.
 * sub_08043AA0 and sub_08043AC0 reduce it modulo 24. sub_08042FFC is defined
 * in src/unit.c. */
void sub_08043AA0(int, int);
void sub_08043AC0(int, int, int);
void sub_08043B14(int, int);
void sub_08043B44(int);
void sub_08043BC8(int, int);
void sub_080436DC(int, int, int);
void sub_0804365C(int, int);
void sub_08043D00(void);
/* sub_08017860 returns the byte at gUnknown_0200C420 + 0x20 + i, as int, not
 * u8: sub_08043AA0 passes the result on without narrowing it. */
int sub_08017860(int);
int sub_08042FFC(int);
u16 sub_08043D84(u8);
void sub_080658AC(void);
/* BG-control field setters, like those on gUnknown_030030B4. They take the
 * struct Unk8012C30 * their .c files define; only the tag is declared here,
 * and callers cast their union BgCntBuf *. */
struct Unk8012C30;
void sub_08012C1C(struct Unk8012C30 *, u32);
void sub_08012C30(struct Unk8012C30 *, u32);
void sub_08012C48(struct Unk8012C30 *, u32);
/* Starts the follow-up proc for sub_080688E4 under its fourth argument. */
void sub_08067898(u32, u32, u32, ProcPtr);
/* Sets +0x60 of the gUnknown_08580FF4 proc (see that script's note in
 * unknown-globals.h). */
void sub_080678BC(u32);

/* More of the sub_0806B708 group. sub_08072C28 takes (dst, count, value); its
 * caller passes gBG1TilemapBuffer. sub_0806AF44 takes the caller's proc. */
void sub_08072C28(u16 *, u32, u16);
void sub_0806B120(void);
/* Returns 0 or 1, as int to match its definition; sub_0806B910 narrows the
 * result with a (u8) cast at its call. */
int sub_0806AF44(ProcPtr);

void sub_08087884(int, ProcPtr);
void sub_08087974(int, ProcPtr);

/* Returns the remainder from the BIOS division (SWI 6). */
int DivRem(int, int);
/* More gUnknown_030058E0 display-list builders; each takes a byte from that
 * array. sub_08043E3C's second argument is a VRAM tile address. */
void sub_08043BA4(int, int, int);
void sub_08043E3C(int, void *, int);

/* Starts a proc under its third argument (see the gUnknown_08580E94 note in
 * unknown-globals.h). */
void sub_080673B0(u32, u32, ProcPtr);

/* Returns sub_08015438's result as s8. */
s8 sub_08015410(void *, u8, void *, void *, u8);
void sub_0804C400(u16);
/* Called by sub_0804E7A8 and sub_0804FCA4 with (side, slot) from
 * gUnknown_03001470[gUnknown_03001FBC].unk30 / .unk34. */
void sub_08056E9C(u16, u16);
int sub_0804BDD8(u16, u16, s16);

/* The 0x0806E000 screen's helpers, all called only by sub_0806EB5C. Each
 * ProcPtr is the parent passed on to Proc_Start. */
void sub_0806F000(int, int);
void sub_0806EB28(ProcPtr);
void sub_0806E5CC(u16, ProcPtr);
void sub_0806E8C8(int, ProcPtr);
/* sub_0806E6C8 starts gUnknown_08582BB4 as a blocking proc under its second
 * argument and stores its first at +0x5c. sub_0806E8E4 finds the
 * gUnknown_08582C24 proc and uses its +0x5c or +0x58, depending on whether the
 * argument is nonzero. */
void sub_0806E6C8(int, ProcPtr);
void sub_0806E8E4(int);
u16 sub_0806F064(u16, u16 *);
/* Defined in c_0806C1C8.c. */
void sub_0806C1E4(void);
/* Defined in c_0806E740.c and c_0801F48C.c. */
void sub_0806E7FC(void);
u8 *sub_0801F494(void);
/* Starts a proc under its second argument and stores the unit record at the
 * new proc's +0x4c. */
void sub_0802A54C(struct Unit *, ProcPtr);
void sub_08031018(void);
/* ---- Callees of the 0x0803B000 block ----
 * sub_08015900 and sub_080158D4 get and set a halfword at +0x40 of the
 * gUnknown_0200E438 record that a gUnknown_03001470 slot names. sub_08016E04
 * tests a 16-bit value and returns 0 or 1. */
s16 sub_08015900(s16);
void sub_080158D4(s16, s16);
struct UnkVec sub_08015638(s16);
bool8 sub_08016E04(u16);
void sub_08016EA4(void);
/* sub_08065700 calls sub_0806377C(gUnknown_08580C7C) and nothing else. */
void sub_08064B68(int);
void sub_08065700(void);
void sub_0806E510(ProcPtr);
void sub_0806E728(ProcPtr);
void sub_08073FF4(int, const void *, ProcPtr);
/* Calls Proc_EndEach(gUnknown_08614220). */
void sub_08074028(void);
/* The 0x08073CB8 blit chain: three nested levels over one bitmap. src is a u8
 * nibble source, dst a u32 tile row, and the int is the bitmap width in tiles,
 * passed down unchanged. */
void sub_08073D1C(u8 *, u32 *, int);
void sub_08073CF4(u8 *, u32 *, int);
void sub_08073CB8(u8 *, u32 *, int, int);

/* Callees of sub_0806BB08 and sub_080867BC. sub_0808B6E8 is memcpy(dst, src,
 * size). sub_08086BF8 and sub_08086CE0 take the same three arguments at both
 * call sites. */
/* sub_0806B9CC (below) stores four byte values, taken as int; same shape as
 * sub_0806BA6C. */
/* Clears a 20 x 22 halfword window of *gBG0TilemapBuffer and flushes it. */
void sub_0806C8A0(void);
void sub_0806B9CC(int, int, int, int);
/* Draws a NUL-terminated byte string into two tilemap rows and returns its
 * width in pixels (8 per glyph). dst is a u16 * tilemap cursor, not a struct.
 */
int sub_0806BD1C(u16 *, u8 *);
/* Starts the gUnknown_08581A34 proc under parent and loads a byte string into
 * its +0x2a halfword table. The first parameter is a row index; the proc's
 * +0x58 gets 24 * row + 8. */
void sub_0806BED8(int, u8 *, ProcPtr);
void sub_0806BA6C(int, int, int, int);
void *sub_0808B6E8(void *, const void *, int);
/* Packs its first two arguments into PutSpriteExt's coordinate words (the
 * second biased by -0x30), passes the third as PutSpriteExt's fifth argument
 * and the fourth as its first, with the OAM data at gUnknown_084A0790. */
void sub_08043FD8(int, int, int, int);
void sub_08064DDC(int, int, int);
void sub_08064E1C(int, int, int);
void sub_08086EB0(int);
void sub_08087104(void *);
int sub_08087248(void);
/* Draws its third argument as a decimal number at (x, y), right to left, in
 * the second digit font; the twin of sub_0802BCF0. The value is u32: the body
 * divides it with unsigned division. */
void sub_0802BD54(u16, u16, u32);
/* sub_0803CA54 returns int, as its definition does; sub_08087104 narrows the
 * result with a (u8) cast at its call. */
void sub_0802BDBC(u8, s16, u16);
int sub_0803CA54(u32);
u16 sub_08087298(void);
/* The second parameter is int, not u32: the body compares it with signed
 * branches. */
void sub_08086BF8(u32, int, int);
void sub_08086CE0(u32, int, int);

/* Returns a string from gTextTable. */
u8 *sub_08024944(u16);

/* ---- More (x, y) cell functions ----
 * sub_0800164C returns 1 when the cell's byte in the +0x1432 plane of
 * gUnknown_08499590 is 7, 0xD or 0x13, and 0 otherwise. */
int sub_0800164C(int, int);
int sub_08008C34(int, int);
/* sub_08008C7C is sub_08008C34's second test: it returns 0 when the cell's
 * +0x0A22 tile is 0x13 or 0x16, else 1. sub_08025308 takes a 1-based army
 * number. sub_0802BBDC takes an s16 element of gUnknown_08090A98. */
int sub_08008C7C(int, int);
int sub_08025308(int);
void sub_0802BBDC(s16);
void sub_08046A84(u8, u8);
/* Scans gUnknown_02028DD8 like sub_0804769C and returns a count. */
u16 sub_08047740(struct Unk0804769C *, u16, u16, u16);
void sub_08007F9C(int, int);

/* The pair sub_08040CA4 starts with, both (id, tileBase, paletteNum). */
void sub_0804103C(int, int, int);
void sub_08041128(int, int, int);

/* Callees of the sub_0805D438 script step. sub_080129E0 is the random number
 * generator (a linear congruential generator). */
u32 sub_080129E0(void);
void sub_0805A95C(void);
void sub_0805E9DC(void);
int sub_08071908(void *);

/* Screen-setup callees of sub_08080498. */
/* The body never reads its parameter, but callers pass their proc
 * (sub_0808A6CC reloads it just for this call), so the parameter stays. */
void sub_0807898C(ProcPtr);
void sub_08071B88(void);
void sub_08012B70(u16 *, u16 *, u16, u16, u16);
void sub_08073574(int, int, int, int, int, int);

/* The two text/graphics emitters that sub_080852A8 chooses between. They
 * differ in the fourth argument: sub_08014668 takes a u16 tile value,
 * sub_080149C0 a gTextTable string. */
/* Returns sub_080152EC's result; current callers ignore it. */
struct Unk03001470 *sub_08014668(int, int, u16 *, u16, u16, u16);
void sub_080149C0(int, int, u16 *, u8 *, int, int);
/* Returns the width in pixels of a string, with one pixel between characters.
 * sub_0804A1E4 passes the buffer sub_080149C0 fills and narrows the result to
 * a byte itself. */
int sub_08014CEC(u8 *);

/* Takes the raw, unclamped Interpolate result from sub_080737EC; the copy
 * clamped to 0..0xF0 goes to gUnknown_030024E4. */
void sub_08073714(int);

/* The per-frame tail of sub_08076494 and sub_0807662C. */
void sub_080763C0(void);

/* The fourth parameter is an 8-byte struct passed by value. struct UnkVec and
 * struct OamData are the same eight bytes; sub_08022BB8 fills the OamData view
 * and passes the UnkVec view. */
void sub_0801C01C(u16, u16, void *, struct UnkVec, int);

/* Each takes one int that its body never reads; callers pass it, so the
 * parameter stays. */
void sub_08043DF4(int);
void sub_08043E18(int);

/* Walks two byte strings; the third parameter is unused, but callers pass
 * their proc. Returns a 16-bit count. */
u16 sub_0807F8FC(u8 *, u8 *, void *);
/* The VBlank callback that sub_080800B0 installs with sub_08011AAC. */
void sub_080801A8(void);
/* Called last by sub_0807FA34; the sibling of sub_08080EE4. */
void sub_08080EF8(void);

u8 sub_080743E8(struct Unk030040D8 *);

/* Dispatches on the first argument's range: 0x60..0x9f to sub_0803C9D4,
 * 0x20..0x5f to sub_0803CA00, 0x00..0x1f to sub_0803CB40; anything else does
 * nothing. The second argument is passed on as a u8. */
void sub_0803CBA0(int, int);

bool8 sub_08019260(void);
void sub_0804A760(void);
/* Returns a byte from one of four ROM byte tables, indexed by its argument. */
u8 sub_0804A18C(u8);
bool8 sub_08019850(void);
void sub_0804018C(void *);
void sub_08074AAC(const u8 *, ProcPtr);

/* ---- Callees of the one-line wrapper families ---- */

/* First callees of the wrappers shaped `f(); h(g, N);`, all void(void).
 * sub_0801A168, sub_080116E8 and sub_08023348, declared above, are the same
 * kind. */
void sub_08016ED8(void);
void sub_08037F18(void);
void sub_08038D7C(void);
void sub_08044BB0(void);
void sub_080745C0(void);

/* Registered as a callback by sub_08039264, which hands its address to
 * sub_0801F024 with a `(void *)` cast. It ignores what sub_0801F024 passes, so
 * the cast is correct. */
void sub_08039188(void);
void DrawMovePathArrow(void); /* sub_08039188; see src/decomp/c_08039188.c. */

/* Walks the byte-stream script at its first argument until a 1, calling
 * sub_0801B7C0(cursor, arg) on each opcode and advancing by
 * sub_0808B6B0(cursor) + 1. */
void sub_0801B8A8(const u8 *, int);

/* Stores its second argument at entry->unk04 and zeroes unk08 and unk10.
 * Callers that hold a void * convert implicitly. */
void sub_08063A30(struct Unk03001470 *, const void *);

/* Takes an int offset from gUnknown_02027F74 + 4 and stores it at +0x54 of the
 * gUnknown_08616D94 proc, where sub_0808789C reads it back. */
void sub_08087B74(int);

/* Second callees of the `sub_0801A168(); f();` wrappers. sub_0802D4A0 calls
 * sub_0801A664 and sub_08034F7C; the other two call sub_080193B0 on a
 * gUnknown_0849A5xx table. */
void sub_0802C144(void);
void sub_0802C1B0(void);
void sub_0802D4A0(void);

/* ---- Callees of more one-line wrapper families ---- */

/* sub_08013AD4(n) marks tilemap buffer n for copying; sub_08011218 calls
 * Proc_EndEach. sub_0806CC00 is defined in src/title-screen.c, upstream's own
 * source, which is not edited. */
void sub_08013AD4(u8);
void sub_08011218(void);
void sub_08034308(ProcPtr);
void sub_0806CC00(s32);

void sub_0802465C(void);
void sub_0803BCA0(void);

/* Takes s16: callers pass sign-extended values, and with s16 none of them
 * needs a cast. */
void sub_0803B48C(s16);

void sub_0801D84C(int);
void sub_08015568(int);
void sub_08072BBC(int);

/* The per-army funds block. Each takes the army slot index into gPlayers, as
 * int. sub_0804415C returns a byte. */
u32 sub_08044094(int);
int sub_0804419C(int);
int sub_080441D4(int);
int sub_08044208(int);
u8 sub_0804423C(int);
void sub_08044354(int);
u8 sub_0804415C(int);
/* The second parameter is u32, as its definition has it. */
void sub_08044080(int, u32);
void sub_080440A8(int, int);
void sub_0804438C(int, int);
/* Callees of the 0x08060000 block. sub_08042C24's fifth argument is the parent
 * proc; sub_08060FFC returns a byte. */
/* Returns sub_08025C98's pointer, or NULL. */
void *sub_08025E08(int, int, int);
void sub_080425FC(u8);
void sub_08042634(int, int);
void sub_08042C24(int, int, int, int, ProcPtr);
void sub_0802C0CC(void);
void sub_0802C0D8(void);
void sub_0806096C(void);
void sub_080609B8(void);
void sub_08060A20(void);
void sub_08060F00(void);
void sub_08060F74(void);
int sub_08057FA8(int);
/* Returns a count over the units in gUnknown_08499594, testing its parameter
 * as a mask against each unit type's gUnknown_085D5ABC[type].unk1a. */
int sub_08057F54(int);
int sub_08060ED4(int);
/* From the 0x08060000 AI block. sub_08060718 takes one s16 and passes it on to
 * sub_08060894. sub_08060D78 takes the address of an s16 local, as an out
 * parameter. sub_08060DAC returns a signed loop count. */
void sub_08060718(s16);
void sub_08060D4C(void);
void sub_08060D78(s16 *);
void sub_08060894(s16);
int sub_08060DAC(void);
int sub_08057FE8(int);
void sub_08060930(void);
void sub_08060A7C(void);
void sub_08060AB0(void);
int sub_08061DA8(int);
u8 sub_08060FFC(void);
void sub_08039634(int, int);
void sub_08044560(void);
void sub_08039ACC(u16, u16, u16, int);
/* sub_080447EC and sub_0804483C pass their own argument straight to
 * sub_080443C4. */
void sub_080443C4(ProcPtr);
void sub_080447EC(ProcPtr);
void sub_0804483C(ProcPtr);
void sub_08044B08(u8, u8, u8);
/* Returns bool8, not int: all three callers test the result as a byte. */
bool8 sub_08044BA0(int);
int sub_0804440C(struct Unk030040D8 *);
int sub_0804443C(struct Unk030040D8 *);
int sub_08044460(struct Unk030040D8 *);
int sub_08044488(struct Unk030040D8 *);
int sub_080444B4(struct Unk030040D8 *);

/* Returns 0 on every path. The parameter is int (0, 1 or 2): the body narrows
 * it only where it passes it to sub_0803CCB8. */
int sub_08005474(int);

void sub_0804B3E0(u16);
void sub_0804FF44(u16);

/* sub_0808606C and sub_08086688 take an object pointer and read it at several
 * offsets. sub_08087C14 takes an int offset from &gUnknown_02027F78, like
 * sub_08087B74. */
/* Takes its own struct, not ProcPtr: through a ProcPtr the body needs a
 * converted copy, which costs a register. The layout lives in c_0808606C.c;
 * only the tag is declared here. */
struct Unk8606CProc;
void sub_0808606C(struct Unk8606CProc *);
void sub_08086688(ProcPtr);
void sub_08087C14(int);
/* Callees of sub_08086688. sub_08087168 is defined in c_08087104.c.
 * sub_080867BC's parameter struct is local to c_080867BC.c; only its tag is
 * declared here. */
struct Unk080867BCProc;
void sub_080867BC(struct Unk080867BCProc *);
void sub_08087040(void);
void sub_080870B8(int, int, int, int);
void sub_08087168(int);
void sub_08087220(int, int);
void sub_080872D0(int);
/* Argument 2 must stay int: the body compares it with signed branches and
 * counts its loops down, and a u32 changes both. */
void sub_08086A58(int, int, int);
void DrawMapList(int, int, int); /* sub_08086A58's readable name; see
                                 * src/decomp/c_08086A58.c. */
/* sub_08086A58 passes its own three parameters straight through. */
void sub_08087548(int, int, int);
void sub_08085950(int, int);
void sub_080858C0(void);
/* The 0x08085000 tree's per-mode redraws. sub_08085168, sub_080851CC,
 * sub_08085208 and sub_08085244 take the proc as a halfword array (p[0x33] is
 * +0x66). sub_08084F44 and sub_08085044 keep their parameter structs in their
 * own .c files; only the tags are declared here. */
void sub_08085168(s16 *);
void sub_080851CC(s16 *);
void sub_08085208(s16 *);
void sub_08085244(s16 *);
void sub_08085908(void);
struct Unk8084F44;
void sub_08084F44(struct Unk8084F44 *);
struct Unk8085044;
void sub_08085044(struct Unk8085044 *);
void sub_08037780(void);
void sub_0803BCD0(u8);
void sub_080876B4(void);
void sub_08087B60(int);

int sub_080432E0(int);
int sub_0800B4F0(int, int);
int sub_0800B5C0(int, int);
bool8 sub_0802C62C(void);
bool8 sub_0802C660(void);

/* The 0x0802C0E8 block's callees. sub_0802C0E8 never reads its u8 parameter,
 * but its caller passes one. The void(void) entries are called with no
 * arguments set up, so they may take parameters that the call sites do not
 * show. */
void sub_0802C0E8(u8);
void sub_0802C154(int);
void sub_08016D30(u16, u8);
int sub_08078E14(void);
void sub_0803B828(void);
void sub_080366A4(void);
void sub_08028CD8(void);

/* sub_080442AC and sub_08044280 return 0 or 1 as int: callers use the result
 * without narrowing it. */
void sub_08016DB8(u16);
void sub_080344F0(int);
int sub_080442AC(int);
int sub_08044280(int);
void sub_08034F10(void);
void sub_080485AC(void);
void sub_08046764(void);
void sub_0802C280(void);

/* sub_08043898 and sub_080438FC are defined in c_08043834.c. */
void sub_08043898(int, int, int);
void sub_080438FC(int, int, int);
bool8 sub_080442E4(int);

/* sub_08042F14 is defined in src/unit.c. */
int sub_08042F14(int);
void sub_080265B0(u8, u8);
void sub_08024058(s16, s16);
void sub_080409E8(int, int, int, int, int);

/* Coordinate predicates used by the functions at 0x0802CB00..0x0802CDFF.
 * sub_080421D0 and sub_0804223C also take the struct Unk030040D8 record. */
bool8 sub_080422A8(s16, s16);
bool8 sub_080421D0(struct Unk030040D8 *, s16, s16);
bool8 sub_0804223C(struct Unk030040D8 *, s16, s16);
/* Two more of the same predicates. sub_0804236C and sub_0804247C are one shape
 * over different tables, and both return bool8: their callers differ only in
 * which one they call. */
bool8 sub_0804236C(s16, s16);
bool8 sub_0804247C(s16, s16);
bool8 sub_0802C8F8(void);
bool8 sub_0802C958(void);
bool8 sub_0802CBA0(void);

/* An (x, y) cell predicate on gUnknown_08499590. Returns int, not bool8:
 * callers use the result in arithmetic without narrowing it. */
int sub_0800977C(int, int);

/* The CpuFastSet-only sibling of sub_08011C68: copies `size` bytes as words. */
void sub_08011C90(const void *, void *, u16);

int sub_080433F8(int, int, int);
/* sub_08043070 returns int: the u16 narrowing at sub_08024ABC's call sites is
 * a cast there. */
int sub_08043070(int, int, int, int, int);
int sub_08042D50(int, int);
/* Draws its third argument as a decimal number at (x, y), right to left;
 * sub_08039F80 calls it four times. The value is u32: the body divides it with
 * unsigned division. */
void sub_0802BCF0(u16, u16, u32);
/* Saves its first argument in gUnknown_03004480, passes the rest to the
 * gUnknown_030013EC callback, then restores gUnknown_03004480 from
 * gUnknown_030033EC. */
void sub_0802026C(int, int, int, int, int, int);
/* Returns the smaller of sub_08042D1C's result for unit->unk00 and the unit's
 * 7-bit field unk06_0. */
int sub_08058224(struct Unit *);
/* Passes its arguments to PutSpriteExt: the first two as coordinates, the
 * third as its fifth argument and the fourth as its first. */
void sub_08043B60(int, int, u32, u32);
/* Callees in the 0x08021000, 0x08038000, 0x08057000 and 0x08058000 blocks.
 * sub_08038474 returns sub_08037DA4's result. */
void sub_0803CA28(u32, u8);
void sub_08038484(void);
/* The per-map setup routine that fills gUnknown_0202FDEC. Nothing in the ROM
 * calls it directly. */
void sub_08038240(void);
void sub_08038548(void);
void sub_08038568(void);
int sub_08038434(void);
int sub_08038474(void);
/* Appends one gUnknown_0200C420.unk38[] record: a1 is the id searched for in
 * the live run, a2 fills bits 8..19 and a3 bits 20..31. */
void sub_08038368(int, int, int);
/* Returns bool8, not int: sub_08038240 tests the result directly as a byte. */
bool8 sub_080381C0(void);
/* Adds its argument to the two words of gUnknown_0808E558's record, clamping
 * each at 9999. */
void sub_080176C0(u32);
/* Callees of sub_08038484. sub_0803BADC and sub_08045790 each start a proc.
 * sub_0807823C does nothing. sub_0807821C returns whether
 * gUnknown_08615194[a].unk02 has bit 0x10 set; its bool8 return is a
 * convention, not forced by its callers. */
void sub_0803BADC(void);
void sub_08045790(void);
void sub_0807823C(int);
bool8 sub_0807821C(int);
void sub_080346FC(void);
void sub_0803BCB8(void);
void sub_0803B8B8(void);
/* The first two parameters must stay int: sub_0805D648 passes sign-extended
 * values without converting them. */
void sub_0802042C(int, int, u8 *);
void sub_080386EC(int);
void sub_08038B84(void);
void sub_080389D8(void);
void sub_0803832C(void);
void sub_08038BE0(void);
/* sub_08038C08 returns 0 or 1 as int, as its definition does; its caller
 * narrows the result with a (u8) cast. */
int sub_08038960(s8, s8);
int sub_08038C08(void);
void sub_08026768(void);
void sub_08026924(void);
void sub_08026BAC(void);
void sub_08035490(void);
void sub_0803E3D8(void);
void sub_080455CC(void);
void sub_080452C0(int, int, int);
void sub_0804C0FC(u16);
/* Callees of sub_0804C0FC. */
void sub_0804BD20(u16, u16, void *, void *);
void sub_0804C098(u16);
void sub_0804C488(u16);
void sub_0804C498(u16);
void sub_0804C4A8(u16);
void sub_0804C578(u16);
void sub_0804C99C(u16);
void sub_0804CEF8(u16);
void sub_0804DB14(u16);
void sub_080566C8(int);
/* sub_08056D70 returns its third argument unless one of its two tests picks
 * another value. */
u16 sub_08056D70(u16, u16, u16);
void sub_08057138(void);
void *sub_08057D58(int, int, int);
int sub_08042D1C(int, int);
void sub_0801F838(u8);
/* Movement-range helpers. sub_0801F6F0 is one flood-fill step to the cursor
 * position plus (dx, dy); sub_0801FD9C spreads value `a` one cell outward
 * across the gUnknown_03003340 overlay. */
void sub_0801F6F0(u8, u8, u8);
void sub_0801F888(int);
void sub_0801FD9C(int);
/* Writes two overlay values at cell (x, y) for one unit: the unit's current
 * value, then its type's gUnknown_085D5ABC[].unk0e minus one. The unit pointer
 * points into gUnknown_08499594. */
void sub_08020354(u16, u16, struct Unit *);
/* sub_080203C0 clears the four neighbours of cell (x, y) in the
 * gUnknown_03003340 overlay, skipping any that fall off the map. */
void sub_080203C0(int, int);
int sub_08058744(void);

/* Predicates with no arguments. They return int, not bool8: callers test the
 * whole register without narrowing it. */
int sub_0804151C(void);
int sub_08041758(void);
int sub_080416A4(void);
/* Builds the option list: walks the 0x20-byte entry table up to its 0xff
 * terminator and asks each entry's unk04 predicate whether it is available. */
void sub_08019E68(void);

/* sub_08019C40 draws the option list, one sub_08014A5C row per selectable
 * entry, then copies the text buffer to 0x06007000. struct Unk8019A60 is
 * defined in unknown-globals.h; the forward declaration keeps this header
 * self-contained, so do not define the struct here. */
struct Unk8019A60;
void sub_08019C40(struct Unk8019A60 *);

/* sub_0802D40C to sub_0802D43C are wrappers that each pass one id, 0xC9A to
 * 0xC9D. */
void sub_0802C1F0(const u8 *, u8 *, int);
void sub_080425E0(u8);
void sub_08042618(int, int);
void sub_0802D40C(void);
void sub_0802D41C(void);
void sub_0802D42C(void);
void sub_0802D43C(void);

/* sub_0802D33C returns unk48 of the gUnknown_03001470 slot running
 * gUnknown_0848A42C. sub_0802E698 and sub_0802E6F8 each finish by storing a new
 * state id in gUnknown_03003334. */
int sub_0802D33C(void);
void sub_0802DFC8(void);
void sub_0802E698(void);
void sub_0802E6C0(void);
void sub_0802E6F8(void);

/* State handlers: sub_0802DC2C calls one of these for each value of
 * gUnknown_03003334. */
void sub_0802DCB4(void);
void sub_0802DE1C(void);
void sub_0802DEFC(void);
void sub_0802E260(void);
void sub_0802E278(void);

/* sub_08020D50 is sub_08020354 with signed coordinates and a different overlay
 * write; its unit pointer also points into gUnknown_08499594. sub_0802E60C
 * stores its (x, y) in gUnknown_03003100 and passes them to the test
 * sub_0802E724. */
void sub_08020D50(s16, s16, struct Unit *);
void sub_08024500(void);
void sub_0802D67C(u8);
void sub_0803AA78(u8);
bool8 sub_0802E724(s16, s16);
void sub_0802E60C(s16, s16);

/* sub_08034F54 clears gUnknown_030030F0[1]. sub_08038AD8 writes the move stack
 * to gUnknown_03003110 as a direction string ending in 4. sub_08025BB4 passes
 * its pointer straight to sub_08035740. sub_0802E940 checks for A or RIGHT,
 * then waits for the next VBlank. */
void sub_08034F54(void);
void sub_08038AD8(void);
void sub_08025BB4(void *);
void sub_0802E940(void);

/* sub_08039264 calls sub_08038D7C, then sub_0801F024(sub_08039188, 2).
 * sub_0802D7B0 is empty. */
void sub_08039264(void);
void sub_0802D7B0(void);
void sub_08024274(void);
void sub_0804256C(void);
void sub_0803A9C8(u8);
void sub_0802EC64(void);
/* sub_0802ED00 and sub_0802ED40 are the IRQ handlers sub_0802EA5C installs in
 * slots 7 and 6. sub_080146D4 is sub_08014668 with a different script
 * (gUnknown_08489568); its first two parameters are int, not s16. */
void sub_0802ED00(void);
void sub_0802ED40(void);
struct Unk03001470 *sub_080146D4(int, int, u16 *, u16, u16, u16);

/* Returns bool8, not int: sub_0802CC40 narrows the result to a byte before
 * testing it. */
bool8 sub_08042084(u8 *);

/* sub_08026FD0 and sub_08026F9C below compare the gPlayers[].unk2a field of two
 * entries. The first parameter is s16, not u16: the definition compiles the
 * same either way, but its callers load the argument sign-extended. */
bool8 sub_08026FD0(s16, u8);
/* sub_080225CC fills the 2x2 gBG2TilemapBuffer block of map cell (x, y) with
 * tile 0x360. */
bool8 sub_08026F9C(s16, s16);
void sub_080225CC(u16, u16);
/* Writes the 2x2 gBG0TilemapBuffer block of map cell (x, y): tiles 0x81b0 to
 * 0x81b3, or zeroes. */
void sub_080227F4(u16, u16);

/* Is the unit with this id boxed in? Looks the unit up and asks sub_080255F4
 * about its own cell. */
bool8 sub_0802571C(u16);

/* Callees of sub_08042998. */
void sub_08025B58(u16, u32);
void sub_08025B80(struct Unit *, u8);
void sub_080424E4(void);
/* Both parameters are int; declared narrower, the definition no longer
 * matches. */
int sub_08042C9C(int, int);

void sub_080616F0(void);

/* Joins the active unit (gUnknown_030040D8) with the unit on the target
 * tile. */
void sub_08042998(void);
void JoinUnits(void); /* sub_08042998's readable name; see src/decomp/c_08042998.c. */

/* sub_0803CCB8 copies the NUL-terminated string at gUnknown_020280C0[id].unk02
 * into the buffer, through sub_0803CC84. sub_0803CDBC returns 0 or 1, but no
 * caller reads the result, so its int return type is a guess. */
bool8 sub_0803CCB8(int, u8 *);
int sub_0803CDBC(int, int, u8);

/* ---- Assorted small helpers ---- */
void sub_0801E0C8(int, int);
void sub_0801EFD8(void);
void sub_08015550(void);
void sub_0801555C(void);
void sub_0802A538(void);
void sub_0802A7B0(void);
void sub_0802C57C(void);
void sub_0802C594(void);

/* sub_0801E0F0 and sub_0801EFA8 hide a run of OAM entries, then reset a
 * counter. sub_0801BBC4 and sub_0801BC08 flush a pending-copy descriptor with
 * CpuFastSet. sub_0801EFF4 copies the OAM shadow to OAM. sub_08026D68 counts
 * the terrain each army holds. */
void sub_0801E0F0(void);
void sub_0801EFA8(void);

void sub_0801BBC4(void);
void sub_0801BC08(void);
void sub_0801BCA8(void);
void sub_0801EFF4(void);
void sub_080219AC(void);
void sub_080245D4(void);
void sub_08026D68(void);
void sub_080424FC(void);
void sub_08061F34(void);
void sub_08062038(void);
void sub_0807F238(void);

/* Runs sub_08028874 for every live army whose gPlayers[].unk2a differs from
 * army `a`'s, then calls FinalizeBattle. */
void sub_08019940(u8, u8);

int sub_080413E8(void);
/* sub_0804138C sets gUnknown_030040A8 to 0. sub_080413B4(x, y, unitId, kind) is
 * a copy of sub_0803E560 that counts in gUnknown_030040A8. */
void sub_0804138C(void);
void sub_080413B4(int, int, int, int);

/* sub_0801537C matched by slot address: finds the gUnknown_03001470 slot at
 * this address, tears it down and returns its index, or -1 if there is none. */
int sub_080153B8(struct Unk03001470 *);

/* ---- Accessors, screen setup and map-cell tests ----
 * sub_0803BB44, sub_0803BB5C and sub_0803BB74 each return one element of the
 * s8 array gUnknown_03003F30, as a u8. */
u8 sub_0803BB44(void);
u8 sub_0803BB5C(void);
u8 sub_0803BB74(void);

/* The screen setup behind sub_08032688. The second argument can be -1. */
void sub_080324C4(int, int, u8);
/* sub_08029AF8 works on the unit's unk00 type index and its unk04_0:7 field,
 * capping that at 100; it returns a total that no caller reads. sub_08053670
 * takes a unit index, like sub_0804C400. */
int sub_08029AF8(struct Unit *, u16, u8);
void sub_08053670(u16);

/* sub_08042424(x, y) tests the terrain byte of cell (x, y) against the army in
 * gUnknown_03004084. sub_08043574(x, y, n) returns n plus 1, 2 or 3, depending
 * on whether x > 0xcf and y > 0x7f. sub_0801C7DC(table, index, count, x, y,
 * oam, layer) draws a sprite from the table with PutSpriteExt; no caller reads
 * its result. */
u8 sub_08042424(s16, s16);
int sub_08043574(int, int, int);
int sub_0801C7DC(const u16 *, int, int, int, int, int, int);

/* ---- Sprite position and slot helpers ----
 * sub_08050528(side, procId, x, y) subtracts the scroll origin from (x, y)
 * and passes the result to sub_080155C0. */
void sub_08050528(u16, s16, s16, s16);
/* sub_080513FC(side, slot, procId) sets the redraw bit (bit 6) of
 * gUnknown_02029664 when the proc has finished and its entry is idle. */
void sub_080513FC(u16, u16, s16);
/* sub_080513FC for the other side, with the screen test inverted. */
void sub_08051920(u16, u16, s16);
void sub_080540F0(u16, u16);

/* Stores its first two arguments in gUnknown_0300453C and gUnknown_0300451C,
 * then calls sub_08051D74. The third parameter is unused, but every caller
 * passes 0, so keep it. */
void sub_08052E04(u16, u16, int);

/* The two emitter routines for struct Unk08580934_Obj. sub_080645AC also calls
 * the callback at +0x4c, which the struct does not describe yet. */
void sub_080645AC(struct Unk08580934_Obj *);
void sub_08064E5C(struct Unk08580934_Obj *);
/* sub_08064BF4 draws an object: it calls sub_0801F34C with the id
 * gUnknown_08580934->unk11[obj->unk1c]. sub_08030178 runs when the emitters'
 * counter runs out. */
void sub_08064BF4(struct Unk08580934_Obj *);
void sub_08030178(void);
/* Defined in c_08030178.c. */
void sub_080301E8(void);

/* Called by sub_08050958 when a moving unit reaches its bound. Arguments: two
 * indices into gUnknown_02029A10, then a proc id that can be -1. */
void sub_08050AEC(u16, u16, s16);

/* Callees of sub_08051F4C. sub_08051D74 tears a slot down. */
void sub_080504A8(u16, u16);
void sub_08051D74(u16, u16);
/* sub_080153F0 takes a proc id. sub_080156E8's second parameter must stay
 * `void *`: callers pass a whole word with no narrowing, although
 * sub_080156FC uses the value as a small table index. */
bool8 sub_080153F0(s16);
void sub_080156E8(s16, void *);
/* sub_08016824 gives an OBJ a free affine matrix slot and switches it to
 * rotate/scale mode. sub_08016944 hides an OBJ; sub_08016974 shows it again. */
void sub_08016824(s16);
void sub_08016944(int);

/* ---- Smooth mover for gUnknown_03001470 slots ----
 * sub_080162A4 is the per-frame step of a smooth move. It treats the slot's
 * 0x3c..0x5f tail as floats: unk3c/unk40 position, unk4c/unk50 velocity,
 * unk54/unk58 acceleration, and unk5c the frames left. It moves the slot,
 * calls sub_080155C0 with the whole-pixel position and ends the command
 * when the count reaches 0. */
void sub_080162A4(u8);
void sub_08016F38(u8);
void CaptureBattleSaveState(u8); /* sub_08016F38's readable name; see
                                  * src/decomp/c_08016F38.c. */
/* Frees an affine matrix slot that sub_0801DAB0 handed out from
 * gUnknown_03001430. */
void sub_0801DAE8(s16);

/* ---- Team test and the gUnknown_02028360 map records ---- */
/* Calls sub_080266DC(i) for each army 1 to 4. */
bool8 sub_0803861C(void);
/* Starts the gUnknown_0849E7D8 proc if gUnknown_030005CA is still 0xFFFF
 * (unset). */
void sub_0803B7B4(void);
/* sub_0803DE14 clears all 16 gUnknown_02028360 records. sub_0803DE94(x, y)
 * returns the first record whose rectangle contains (x, y), stopping at the
 * first record of kind 0. */
void sub_0803DE14(void);
struct Unk02028360 *sub_0803DE94(int, int);
/* sub_0803DE94 with a third test: the record's kind (unk02_6) must equal the
 * third argument. Returns the record, or NULL. */
struct Unk02028360 *sub_0803DEEC(int, int, int);
/* Fills *pos with the {u16, u16} pair for a mode id from 2 to 8. */
void sub_0803DF98(int, struct Unk02028360Pos *);
/* sub_0803E560 appends one record to *gUnknown_03003338; its parameters must
 * stay u16, because its callers only match with u16. sub_0803E088 allocates an
 * object through sub_0803E01C, fills in its fields and returns it. All ten of
 * its parameters are int: arguments 7 and 10 look like u8, but declaring them
 * u8 changes the code. */
void sub_0803E560(u16, u16, u16, u16);
void *sub_0803E01C(int, int, int, int, int, int);
void *sub_0803E088(int, int, int, int, int, int, int, int, int, int);
void *sub_0803E7C0(int, int);
void *sub_0803E7E4(int, int);
/* Map decoration builders. sub_0803E6C4 runs sub_0803E560 on every unit in a
 * 3-column strip below (x, y). sub_0803E764 does the same for each cell of a
 * 0xFFFF-terminated {x, y} list. */
void sub_0803E108(int, int, int, int);
void sub_0803E158(int, int, int, int);
void sub_0803E1B0(int, int, int, int, int, int);
void sub_0803E208(int, int, int, int, int, int, int);
void sub_0803E260(int, int, int, int, int);
void sub_0803E2B8(int, int, int, int, int, int);
void sub_0803E310(int, int, int, int, int, int);
void sub_0803E554(void);
void sub_0803E594(int, int, int);
void sub_0803E6C4(int, int, int);
void ScanUnitsBelowStrip(int, int, int); /* sub_0803E6C4; see src/decomp/c_0803E6C4.c. */
void sub_0803E764(struct Unk02028360Pos *, int);
void sub_0803E808(int, int, int, int, int);
void sub_0803EF44(int, int, ProcPtr);
void sub_0803F0A4(int, int, int, int, int, int, ProcPtr);
void sub_0803F2B8(int, int, int, ProcPtr);
void sub_0803F510(int, int, ProcPtr);
/* sub_08020DBC(id, x, y) is a yes/no test on map cell (x, y); what it tests is
 * unknown. */
bool8 sub_08020DBC(u8, u8, u8);
void sub_0803D3F0(void);
/* sub_0803DFE0 decodes one gUnknown_02028360 record's position into *pos and
 * returns whether it knows the record's kind (unk02 bits 6..9). */
bool8 sub_0803DFE0(struct Unk02028360 *, struct Unk02028360Pos *);
struct Unk02028360 *sub_0803DF54(int, int);
void sub_0803D724(u8);

/* ---- Flag, save-block and link helpers (0x08016000 area) ---- */
void sub_08016A14(void);
/* Returns the address of one of gUnknown_0200C420's three byte flags, chosen by
 * the argument. For any other argument the result is undefined. */
u8 *sub_08016C9C(s8);
/* Getter and setter for the flag sub_08016C9C selects: sub_08016CD8 returns it
 * as an s8, sub_08016CEC stores a u8 in it. */
s8 sub_08016CD8(s8);
void sub_08016CEC(s8, u8);
/* Tears down every gUnknown_0200CC38 slot tagged with `id`, then reruns the
 * link scan. Returns 1 when id is 0, else sub_0801A7D8's result. */
int sub_0801ABF8(u8);
/* Argument 1 is a u8 and argument 3 a signed byte count. Argument 2 is never
 * read (the body uses gUnknown_0200CC34 instead), but callers pass a buffer, so
 * keep it. Returns int. */
int sub_0801A7D8(u8, void *, int);
/* sub_08016BC0 copies the two blocks back from the buffer that sub_08016B2C
 * filled, and returns their size, 0x5CC bytes. */
int sub_08016BC0(void *);
void sub_08016C70(u8);
void sub_08016E14(void);
void sub_08016E8C(void);
void sub_08017870(int, u8);

/* ---- Assorted callees (0x0803A000 and 0x08084000 areas) ---- */
/* Two gUnknown_03004100 consumers that sub_0803AA78 calls in turn. Argument 1
 * is gUnknown_0849D89C->unk00; argument 2 is the unit that sub_08025BE0 sets
 * up. */
void sub_0803A190(int, struct Unit *);
void sub_0803A2BC(u8, struct Unit *);
/* Walks the gUnknown_0849EDB0 list until a row's unk08 callback returns
 * something other than -1. */
bool8 sub_0803C814(void);
/* struct Unk080852A8 is left incomplete here on purpose: c_080852A8.c defines
 * it. sub_080157A4 and sub_080157F4 each set one field (unk3c or unk3e) of a
 * gUnknown_0200E438 entry. */
struct Unk080852A8;
void sub_080852A8(struct Unk080852A8 *);
void sub_080157A4(s16, s16);
void sub_080157F4(s16, s16);
/* Set one bit of a slot's stashed OBJ attributes: sub_080154C4 writes `mosaic`
 * and sub_08015504 writes `bpp` (bits 12 and 13 of attribute 0; neither is
 * vFlip). Each reads the attributes with sub_0801566C and writes them back with
 * sub_08015608. */
void sub_080154C4(s16, u8);
void sub_08015504(s16, u8);
/* The second argument is an index into the table at
 * gUnknown_0200E438[].unk48. */
void sub_080156FC(s16, u16);
void sub_08052818(u16, u16);
void sub_08050424(u16, u16, int);
/* The simplest of the sprite position setters: moves one gUnknown_02029A10
 * entry's sprite with sub_080155C0. */
void sub_0804DC5C(u16, u16, int);

/* Calls sub_0803B3D4(8). */
void sub_0803B3C8(void);

/* Returns 1 when byte 1 of gPlaySt is 1 and sub_0803CBD8(0x60) is non-zero,
 * else 0. Returns int, not bool8: some callers use the result as an array index
 * without narrowing it. */
int sub_0803866C(void);

/* ---- Proc and palette front ends ----
 * sub_08071B0C and sub_08071AF0 start a proc through sub_08071B28, each with
 * its own palette; the third argument becomes the new proc's parent. */
void sub_08071B0C(int, int, ProcPtr);

void sub_08071AF0(int, int, ProcPtr);

/* Reads byte `i` of gUnknown_03000650. */
u8 sub_08084858(int);

/* (x, y) are OAM coordinates, cut to 9 and 8 bits as in sub_0801F34C. Arguments
 * 3 and 4 are passed on unchanged; argument 5 is a flag. */
void sub_08043C28(int, int, int, int, u8);

/* Decompresses member 0 of record `i` in gUnknown_08616AC0, an array of
 * two-pointer records. */
void sub_080845A8(int);

/* Returns a 16-colour palette: &gUnknown_0823DC38[i * 16] when sub_08084858(i)
 * is 0, else gUnknown_0812596C. */
u16 *sub_08084864(int);

/* Map-tile helpers for sub_0800CFDC, each taking a cell (x, y) of the
 * gUnknown_08499590 map. sub_0800E8CC returns a mask of matching neighbours;
 * sub_0800E9F4 returns which corner of a 2x2 tile block the tile at (x, y) is
 * (1 = top-left, 2 = top-right, ...). sub_0800EAF4 and sub_0800EB5C rewrite 2x2
 * and 3x3 blocks of tiles. */
int sub_0800E8CC(int, int);
int sub_0800E9F4(int, int);
void sub_0800EAF4(int, int);
void sub_0800EB5C(int, int);

/* The map-tiling driver that uses the helpers above. It always returns 0, and
 * its one caller ignores the result. */
int sub_0800CFDC(int, int);

/* ---- Script starters and the gUnknown_0200C020 text record ---- */
/* Option-list starters. sub_08019F2C passes its five arguments to
 * sub_08019F90, which builds the option list (sub_08019E68 rebuilds it
 * later); sub_08019F50 calls sub_0801A604 first. The first argument is a
 * data blob that sub_08019F2C and sub_08019F50 pass on without reading.
 * No caller reads the int result. */
int sub_08019F2C(const void *, u16, u16, u16, u16);
int sub_08019F50(const void *, u16, u16, u16, u16);
int sub_08019F90(const void *, u16, u16, u16, u16);
/* sub_0801A104 returns sub_08019F50's result. sub_08014074 and sub_080147B4
 * fill the text record struct Unk08014074 (gUnknown_0200C020); argument 5 of
 * sub_080147B4 indexes gTextTable[]. sub_080147B4's parameter types are what
 * make its callers sub_08014668 and sub_080146D4 match: do not change them. */
int sub_0801A104(const void *, u16, u16, u16);
void sub_08014074(struct Unk08014074 *);
void sub_080147B4(struct Unk08014074 *, s16, s16, u16 *, u16, u16, u16);
/* Advances the text cursor by `a2` sub-tile units: adds to the fractional
 * accumulator unk40 and, for every whole 8, steps unk34 by 2 and unk32 by 1.
 * Returns whether a fraction is left over. */
int sub_08014CA4(struct Unk08014074 *, int);

/* ---- Proc helpers (0x08074000-0x0807A000 area) ---- */

/* sub_08074744, sub_08074F1C, sub_08075304 and sub_080755E0 each end every proc
 * running one script, with Proc_EndEach. */
void sub_08074744(void);
void sub_08074F1C(void);
void sub_08075304(void);
void sub_080755E0(void);
void sub_0801C1F8(void);

/* Defined in c_0803BD54.c. */
u8 sub_0803BD6C(void);

/* Defined in c_08014BB4.c. */
void sub_08014BC0(ProcPtr);

/* Starts the gUnknown_08615ACC proc under `parent` and stores its four other
 * arguments at +0x2c, +0x30, +0x58 and +0x54 of it. */
void sub_080785CC(s32, s32, s32, const void *, ProcPtr);

/* Starts a blocking proc under `parent` if that is not NULL. Arguments 2 and 3
 * are stored as halfwords; argument 4 is a flag. Returns 0 or 1. */
s32 sub_08074C84(ProcPtr, s32, s32, u8);

/* Walks the `const s8 *` at word 0 of the record and compares a count with its
 * bytes at +4 and +5; on one path it writes +0x58 of the proc. Returns a u8
 * truth value. */
u8 sub_080782C0(struct Unk80782C0 *, ProcPtr);

void sub_08019818(u16, u8, u8);

/* sub_08078770 is defined in c_08078758.c. */
void sub_08076770(s32, s32, s32, ProcPtr);
void sub_08078770(void);


/* Tests a value against a set of six. Returns u8, not s32: its caller narrows
 * the result before testing it. */
u8 sub_08078E20(void);

/* Starts a blocking proc; the second argument is its parent. */
void sub_08075E68(s32, ProcPtr);

/* Returns the first team index whose IsPlayerAliveAndActive test holds, or 0.
 * Returns int, not u16: sub_0807A860 uses the result as an array index without
 * narrowing it. */
int sub_0807A908(void);

void sub_0807A99C(s32, u8);

void sub_08078AF0(void);

/* ---- Option list, text rows and script slots (0x08019000 area) ---- */

void sub_08019C24(void);
void sub_08022ADC(void);
void sub_0801A604(void);

/* sub_08014878 ends three scripts with sub_0801537C. sub_08019380 starts the
 * most recently queued script in gUnknown_0200C508. */
void sub_08014878(void);
void sub_08019380(void);
void sub_0803670C(void);

/* Draws one row of text; sub_08019C40 calls it once per option. The third
 * argument is the same object pointer sub_08019578 takes. */
void sub_08014A5C(int, int, void *, int, int, int);

/* sub_08019A60 and sub_08019B80 walk the 0x48-byte object sub_08019B50
 * describes; sub_08019578 walks the object sub_080195C8 owns. */
void sub_08019A60(void *);
u8 sub_08019B80(void *);
void sub_08019578(void *);
void sub_080196F4(void *);

/* Proc callbacks that sub_08019F90 installs. sub_08019D78 and sub_08019DA8 both
 * pass their proc on to sub_08019D48. */
void sub_08019D48(ProcPtr);
void sub_08019D78(ProcPtr);
void sub_08019DA8(ProcPtr);

/* Takes a gUnknown_0200C528 slot pointer; sub_080192EC(i) is the index form. */
void sub_080192C4(struct Unk0200C528 *);

/* sub_08022AD0's parameters are s16, not u16: the definition compiles the same
 * either way, but its callers only match with s16. */
void sub_08022AD0(s16, s16);
void sub_0802323C(s16, s16, int);

/* ---- Callees of the gUnknown_0200C528 script commands ----
 * sub_08026798 zeroes gUnknown_030032C0, then calls sub_08020984.
 * sub_080185A0 copies gUnknown_08499588 to VRAM at 0x06006800. */
void sub_08026798(void);
void sub_080185A0(void);
/* Clears a 23x4 window of a 32-tile-wide tilemap. */
void sub_080179D0(u16 *);
void sub_080192EC(s16);
/* Calls sub_080290B0(x, y, 1); sub_08029088 is the same with 0. */
void sub_0802909C(s16, s16);
/* Returns the object sub_08025C5C finds, or NULL. */
void *sub_08025C98(s16, s16, s16);
/* Is the gUnknown_08499EE4 script running? */
bool8 sub_080281A0(void);
/* Returns the index of the gUnknown_085C77A0 entry whose unk2c equals the
 * argument, or 0xbf if there is none. */
u16 sub_080206B0(u32);
/* Starts the gUnknown_08499EE4 script and stores the second argument at +0x18
 * of its slot, where sub_08028190 reads it. The first parameter is unused, but
 * callers pass gPlaySt.unk02, so keep it. */
void sub_080281D8(u8, u32);

/* gUnknown_0200C528 script-command handlers: each takes a slot index and
 * returns whether to go on to the next command. The return is s16, not bool8:
 * sub_08017D30 and sub_08017CF0 return these results unconverted, and only s16
 * matches there. Other handlers in the same table that return constants are
 * still declared bool8 and are probably s16 as well. */
s16 sub_08017A58(s16);
s16 sub_08017A80(s16);
/* sub_08019404 steps the script in one gUnknown_0200C528 slot, calling handlers
 * from gUnknown_0848A244 until one returns 0. sub_08017988 returns s16, not
 * bool8: its caller reuses the sign-extended result. */
s16 sub_08017988(void);
void sub_08019404(s16);
/* sub_08019688(p, term, count, stride) skips `count` records in a byte stream,
 * where a record ends at the first `term` byte found by stepping `stride` at a
 * time, and returns the position after the last one. sub_08019910 walks
 * gUnknown_03003110 to a terminator: TRUE for 10, FALSE for -1 or 4. */
u8 *sub_08019688(u8 *, u8, u8, u8);
bool8 sub_08019910(void);
/* Callbacks installed in a gUnknown_0200C528 slot's unk08, which clear that
 * field again when their test fails: sub_08017ABC when sub_080281A0 fails,
 * sub_08017C4C when gUnknown_0849A00C is no longer running. */
void sub_08017ABC(struct Unk0200C528 *);
void sub_08017C4C(struct Unk0200C528 *);

/* More slot callbacks of the same kind. sub_080180CC counts slot +0x11 up to
 * 0xf, then installs sub_080180A8, which counts it back down and clears the
 * slot. */
void sub_080180A8(struct Unk0200C528 *);
void sub_080180CC(struct Unk0200C528 *);
/* sub_0801853C installs sub_080184EC, which later installs sub_080184E0 in the
 * same slot. sub_080185BC clears its slot's callback when its test fails. */
void sub_080184E0(struct Unk0200C528 *);
void sub_080184EC(struct Unk0200C528 *);
void sub_080185BC(struct Unk0200C528 *);
/* sub_08014004 scans a text-command stream (an element of gTextTable[]): FALSE
 * if it meets token 0x14, TRUE at the end. sub_080185D0 and sub_08018694 are
 * list-script handlers that load a tileset. sub_08018A28, sub_08018AA8 and
 * sub_08018DF8 are slot callbacks; sub_08018DF8 draws the node's position
 * relative to the gUnknown_08499590 viewport. */
bool8 sub_08014004(u8 *);
bool8 sub_080185D0(s16);
bool8 sub_08018694(s16);
void sub_08018A28(struct Unk0200C528 *);
void sub_08018AA8(struct Unk0200C528 *);
void sub_08018DF8(struct Unk0200C528 *);

/* Siblings of sub_080430B0: each takes gPlayers[a].unk1d, gPlayers[a].unk1e and
 * a third value. */
int sub_08043120(int, int, int);
int sub_08043190(int, int, int);
int sub_08043200(int, int, int);
/* Called first by sub_08042D1C and sub_08042D50; their results are added to
 * those of the functions above. */
int sub_080433B8(int);
int sub_080433C8(int);
void sub_08041978(u8, int);
void sub_08041820(int, int, int);
/* sub_080425B8 is the common first call of sub_080424BC's four small
 * wrappers. */
void sub_080424BC(void);
void sub_080425B8(void);
/* sub_080432A8 is a two-argument sibling of sub_080430B0, and sub_080433E8's
 * result is added to it. sub_08043050 returns flags; its caller tests bit
 * 0x80. */
int sub_080432A8(int, int);
int sub_080433E8(int);
u32 sub_08043050(int);
/* Another sibling pair of the same kind, called by sub_08042C9C. */
int sub_08043270(int, int, int);
int sub_080433D8(int);
/* The second argument is gUnknown_08552148[slot]. The body ignores it and reads
 * the table itself, but sub_0804C400 passes it, so keep it. */
void sub_0804C340(u16, u16);

/* The same function as sub_0804C340, but here both parameters are used. */
void sub_0804C268(u16, u16);

/* ---- Map designer screens ---- */

/* sub_08003704 and sub_080037AC have identical code. */
void sub_08003704(void);
void sub_080037AC(void);
void sub_08003040(void);

void sub_08002E3C(void);

/* The argument is signed and can be -1. */
void sub_08003C48(int);

/* Calls sub_08019F2C with 0 as its fifth argument and returns the result. */
int sub_0801A148(const void *, u16, u16, u16);

/* The third argument is a string, such as gDesignRoomName. */
void sub_08004DD4(int, int, u8 *, int);

/* The second argument is a position in a tilemap buffer, such as
 * &gBG0TilemapBuffer[...]. */
void sub_0801F2AC(int, u16 *);

/* Front ends for sub_08004DD4. sub_08004D74 adds the string gTextTable[0x9FA]
 * and 0; sub_08004D90 passes its own string, and first writes a BG0 tilemap
 * cell with sub_0801F2AC. */
void sub_08004D74(int, int);
void sub_08004D90(int, int, u8 *);

/* Meant to test whether a string is non-empty; the original code has bugs,
 * reproduced in c_080051EC.c. Returns bool8: callers test the result as a
 * byte. */
bool8 sub_080051EC(const char *);

void sub_080059E4(void);

/* sub_080059FC opens a window and draws text rows 0x9EF to 0x9F4; sub_08005AA0
 * draws rows 0x9F5 to 0x9F9, then calls sub_080059E4. sub_08005B24 is reached
 * only from a proc script. sub_08005F1C frees the sprite at
 * gActiveMap->spriteId. */
void sub_080059FC(void);
void sub_08005AA0(void);
void sub_08005B24(void);
void sub_08005F1C(void);

/* Build the three-slot screens that sub_080057EC and sub_08005964 show. */
void sub_0800572C(void);
void sub_08005874(void);

void sub_08004C10(void);
void sub_08004C5C(void);
int sub_08004E44(void);

/* ---- Flag bits, callback registry and strings (0x0803C000 area) ---- */

/* Bit setters for the two upper id ranges of sub_0803CBA0's dispatch. The first
 * argument is the id minus the range base (0x20 or 0x60). */
void sub_0803C9D4(u32, u8);
void sub_0803CA00(u32, u8);

/* More bit setters of the same family. */
void sub_0803C8F0(u32, u8);
void sub_0803C950(u32, u8);
void sub_0803C97C(u32, u8);
void sub_0803C9A8(u32, u8);

/* The callback registry gUnknown_02027FB0 (16 slots) and the gUnknown_0849EDB0
 * list. sub_0803C750 registers a callback: it claims a free slot or bumps the
 * use count of the slot already holding it, and returns FALSE when it cannot.
 * sub_0803C784 fills `out` with the indices of the rows whose unk08 callback
 * returns 1 and that sub_0803C750 accepts: at most 32, ended by 0xff. */
bool8 sub_0803C750(int (*)(int));
void sub_0803C784(u8 *);
void sub_0803C670(void);
void sub_0803C1D4(void);

/* Bit setter for the bottom id range of sub_0803CBA0's dispatch: writes bit
 * `id` of the gUnknown_030033F4 block that sub_0803CB74 reads. */
void sub_0803CB40(int, int);

/* Returns a byte; 0xff means an empty slot. */
u8 sub_0802490C(u16);

/* Copies the NUL-terminated string `src` to `dst`. */
void sub_0803CC84(u8 *, const u8 *);

/* Calls sub_0803CF04 with both arguments. The second is an int, not a pointer,
 * so callers that pass an address cast it. */
void sub_0803CF3C(u8, int);

/* Callees of sub_0803CF04; both get &gUnknown_02000000 as their second
 * argument. sub_0801AC58 returns 1 or 0. */
int sub_0801AC58(u8, u8 *);
void sub_0803D2F8(int, u8 *);
void sub_0803D238(u8 *);

/* Fills gMap->rowOffset[y] with y * gMap->width for every row. */
void sub_080215FC(void);
/* Save and restore gUnknown_020288B4 as a run-length-encoded stream at
 * gUnknown_02000000 + 0xDA8. sub_08045700 is the save half. sub_080456B8
 * unpacks: a byte with bit 7 set is one literal (its low 7 bits), any other
 * byte is that many zeroes, and 0xff ends the stream. */
void sub_08045700(u8 *);
void sub_080456B8(u8 *);

int sub_0803D4A8(u8);

/* Passes its second argument on to sub_0803D2F8 unchanged. */
void sub_0803CF04(u8, int);

/* ---- Callees of the 0x08037000 area ---- */

void sub_080169E8(void);
void sub_08036B34(void);
void sub_0803D6B8(void);
void sub_08037DC8(void);

/* Collects every gUnknown_085C77A0 row whose unk1a equals `id` and that
 * sub_080373F0 accepts into gUnknown_02027F78. Returns 1 or 0. */
u8 sub_08037448(u8);

/* The filter sub_08037448 applies: returns 0 when `a` fails the current mode's
 * check, which depends on byte 0x32 of gUnknown_03003FC0. */
u8 sub_080373F0(u16, u16);

/* sub_0803CC64 returns int, not u8: the (u8) at its caller is a cast in that
 * caller's source. */
u8 sub_08026340(void);
int sub_0803CC64(u16);

/* Calls sub_08037448(gUnknown_08090EF0[i]). */
void sub_080375A4(u8);

/* The three arms of sub_080375D4's switch on (p->unk1e++ & 0x3f); each is
 * called as f(p->unk18). */
void sub_0801B6EC(void *);
void sub_0801B6FC(void *);
void sub_08037A78(int);

/* sub_08037610 stores its argument at +0x18 of a new gUnknown_03001470 slot.
 * sub_08037638 calls it with a + ((c & 0x3ff) << 5), then calls sub_0803768C
 * with its own four arguments. */
void sub_08037610(int);
void sub_0803768C(int, int, int, int);

/* Copies `size` bytes of palette from `src` to byte offset `offset` of both the
 * gPal shadow and palette RAM, like ApplyPaletteExt. */
void sub_0801368C(u16 *, u16, u16);

/* Starts one of several scripts with Proc_StartBlocking under `parent`; the
 * first argument selects the script. */
void sub_08049F08(int, ProcPtr);

/* ---- Target pickers used by sub_080448E4 ---- */

/* Calls one of sub_0805C2DC, sub_0805C514 and sub_0805C720, chosen at random,
 * with both arguments, and returns its result. */
u8 sub_0805C290(u16, u8);

/* The three choices. sub_0805C2DC scores every unit of the armies marked in
 * gPlayers[a].unk2c and returns the best one's slot number. */
u8 sub_0805C2DC(u16, u8);
u8 sub_0805C514(u16, u8);
u8 sub_0805C720(u16, u8);

/* ---- Move-target cell search ---- */

/* sub_08058CE8 scores cell (x, y) as a move target; the fourth argument is the
 * running best score and *out gets the best cell. sub_08058E88 writes (x, y)
 * to *out. The out pointer is `u16 *`, not `s16 *`: callers read it back
 * unsigned. */
void sub_08058CE8(int, int, int, int *, u16 *);
void sub_08058E88(int, int, u16 *);

/* Arguments: an army index from 1 to 4, a pointer to the caller's s16 best
 * value, and a pointer that sub_08058F90 passes through. */
void sub_08059050(int, s16 *, void *);

void sub_0806AA80(int, int);

/* ---- Display effects: VCOUNT, palette fades, BG0 scroll ---- */

/* Sets the VCOUNT compare value (REG_DISPSTAT bits 8-15). */
void sub_08063980(int);

/* One step of the red-only palette fade that sub_0806A680 runs each frame. */
void sub_0806A5B8(void);

/* Sets up the proc sub_0806AA80 has just started: words at +0x30..+0x4c and
 * halfwords at +0x58..+0x60. The two ints are shifted left by 12. */
void sub_0806A6F0(ProcPtr, int, int);

/* The two halves of a BG0 scroll ping-pong: each installs the other with
 * sub_080638D0. That function takes an int, so the call casts:
 * sub_080638D0((int)sub_0806A180). */
void sub_0806A158(void);
void sub_0806A180(void);
/* An H-blank callback that sub_0806A578 registers with sub_0801F024. */
void sub_0806A534(void);

/* ---- Proc record helpers (0x08063000 area) ---- */

/* Both take the caller's own proc record; the structs are defined in
 * unknown-globals.h. */
void sub_08062FB8(struct Unk08062FB8 *);
void sub_08063BE0(struct Unk8063BE0 *);

/* Pushes a tag-4 entry onto the gUnknown_0200B3B4 queue: the pointer as a word
 * and the second argument as a halfword. Returns an s16, or -1 when the queue
 * is full. */
s16 sub_08011D7C(void *, int);

/* ---- Effect procs and slot scans (0x08064000-0x08066FFF) ---- */

/* Twins taking (x, y): each has the same gGameClock test, wraps x to 0x1FF and
 * y to 0xFF, and draws with sub_0801F34C, id 0x43 or 0x44. sub_08064500 also
 * writes a palette. */
void sub_08064474(int, int);
void sub_08064500(int, int);

/* sub_080654E8 ends the procs that sub_08065238, sub_0806530C and sub_0806540C
 * start. Neither function takes or returns a value, although sub_08066808 calls
 * them back to back. */
void sub_080654E8(void);
void sub_08064A44(void);

/* Walks the 30 gUnknown_03001470 slots from the last and calls fn(slot) on
 * every slot whose unk00 equals the first argument. */
void sub_08063A00(const void *, void (*)(void *));

/* sub_08063A00 callbacks: each calls one function with the slot and a fixed
 * second argument. */
void sub_08065F68(void *);
void sub_08065F78(void *);
void sub_08066200(void *);
void sub_08066210(void *);

/* sub_08063A3C returns a gUnknown_03001470 slot. Its return type matters:
 * sub_08065F88 passes the result straight on, as sub_08063A30(sub_08063A3C(),
 * gUnknown_08580D90). */
struct Unk03001470 *sub_08063A3C(void);
void sub_08065EF4(void);
void sub_08065EB4(void);
void sub_08066BF4(void);
void sub_08066D30(void);

/* Takes an entry of gUnknown_08580934->unk54[]. */
void sub_08066C70(struct Unk08580934_Obj *);

/* Siblings of sub_08065238: each starts its own proc script. */
void sub_0806530C(void);
void sub_0806540C(void);

/* The argument is an index into gUnknown_08580934->unk54[]. */
void sub_08066B8C(int);

/* sub_08066580 installs a gUnknown_03001470 slot and records it in two per-slot
 * tables. sub_080665BC takes a slot index into gUnknown_08580934->unk74[]. */
void sub_08066580(int, int, int);
void sub_080665BC(int);

/* sub_08065E5C runs sub_08065DAC for every slot whose unk70 mark is clear.
 * sub_080665D4 and sub_0806666C look for a slot the player has just pressed A
 * on. */
void sub_08065E5C(void);
void sub_080665D4(void);
void sub_0806666C(void);

void sub_08065C9C(int);
void sub_08066078(void);
void sub_080660BC(u16, int, u8);

/* The handlers that sub_0806630C and sub_08066B40 dispatch to. sub_08066220,
 * sub_08066D74, sub_08066EBC and sub_08066F20 are declared with empty
 * parentheses because their callers leave a value in r0; their definitions take
 * no arguments. */
void sub_08065F88(void);
void sub_08066220();
void sub_08066874(void);
void sub_08066A20(void);
void sub_08066D74();
void sub_08066EBC();
void sub_08066F20();

/* The dispatch chain: sub_08066B6C picks sub_0806630C or sub_08066B40 by a
 * selector in *gUnknown_08580934, and each of those calls one of the handlers
 * above. */
void sub_0806630C(void);
void sub_08066B40(void);
void sub_08066B6C(void);

/* ---- Camera and screen procs (0x08071000-0x08077FFF) ---- */

/* A four-byte veneer that switches to a hand-written ARM routine, so it cannot
 * come from C. Arguments: two addresses (for example gUnknown_08551A00 + 0x140
 * and a position in gBG0TilemapBuffer), then two small counts. */
void sub_08071900(void *, void *, int, int);
/* Runs sub_0803DDF4 when L is held and B is newly pressed. It reads none of
 * its four parameters, but its callers sub_08077690 and sub_08077DF0 pass
 * four arguments and only match with this prototype, so do not change it to
 * (void). */
void sub_08071918(void *, int, int, int);
void sub_080733A0(int);
void sub_08074EEC(int);
void sub_080752D8(int);
/* Ends every proc running one script, with Proc_EndEach. */
void sub_080763B0(void);
/* sub_08076E20 moves the camera one frame from the key mask in gpKeySt->unk00.
 * sub_08076F34 looks for the record in the gUnknown_0202FE38 list that the
 * camera is currently closing on. */
void sub_08076E20(u16);
void sub_08076F34(ProcPtr);

/* Returns 1, 0 or -1. Returns int, not a narrower type: sub_0807610C uses the
 * whole value without narrowing it. */
int sub_08075EC4(void);

/* Takes the proc sub_0807610C runs, and uses the word at +0x3c of it to detect
 * changes. */
void sub_08075F44(void *);

/* struct Unk807606C is completed in c_0807606C.c. It must stay a struct tag
 * here, not `void *`, or that file stops compiling; callers in other files cast
 * their proc pointer. */
struct Unk807606C;
void sub_0807606C(struct Unk807606C *);

/* Unpacks ten gPal colours into gUnknown_0200B614's three-bytes-per-colour
 * shadow. */
void sub_08075A54(int, int);

/* Helpers of the screen setup at 0x08076000. sub_08076888 ignores its proc
 * argument, but keep the parameter: sub_08076A68 only matches when it passes
 * one. sub_08076B20 finds the gUnknown_0861515C record whose key matches
 * gUnknown_0202FDFC.unk0c. */
void sub_08076888(ProcPtr);
void sub_08076858(void);
void sub_0807681C(void);
void sub_08076B20(void);

/* Finds the entry of the 12-byte gUnknown_0202FE38 list that matches the first
 * argument, copies it to *out and removes it from the list. Returns 1 if found,
 * 0 if not. */
int sub_08074834(s32, struct Unk0202FE38 *);

/* The third argument points to one byte that this function writes. */
u8 sub_080759A0(int, int, u8 *);

/* The X and Y halves of the camera clamp that sub_08076E20 drives. Each takes a
 * signed delta and returns 1 if it moved, else 0; sub_08076E20 adds the two
 * results. */
int sub_08076CAC(s16);
int sub_08076D68(s16);

/* sub_08076F14: is (x, y) within a circle of radius 16? */
u8 sub_08076F14(s16, s16);
void sub_08075298(ProcPtr, int, s16, s16, u16);

/* Starts gUnknown_086143B8 under `parent`. */
void sub_0807548C(s16, s16, int, ProcPtr);

/* sub_0807548C for a gUnknown_086143B8 proc that is already running: it finds
 * the proc instead of starting one. */
void sub_0807553C(s16, s16, int);

/* Starts gUnknown_08614370 under `parent` and returns the new proc. */
void *sub_08075058(ProcPtr parent, u16 a2, s16 a3, s16 a4, u16 a5);

void sub_0807639C(ProcPtr);
void sub_08074ED0(void *, ProcPtr);
void sub_08078480(void *, ProcPtr);
void sub_08078540(void *, ProcPtr);

/* Starts two procs under the caller's own proc. */
void sub_08076C8C(ProcPtr);

/* ---- Palette fades ---- */

/* sub_080137AC without the bias: starts a palette fade from the raw colour
 * values. Callers pass 1 or -1. */
void sub_08013830(s8);

/* ---- Slot cursor handlers (0x08065000 area) ---- */

/* Moves the cursor; one half of sub_08065EB4's dispatch. Arguments: a slot, the
 * key mask (0x40 up, 0x80 down) and a flag that enables the sound effect. */
void sub_08065DAC(int, u16, u8);

/* The other half: L or R flips the selected slot's unk09[] mark between 1 and
 * 2. Declared with empty parentheses because its caller leaves a value in r0;
 * the definition takes no arguments. */
void sub_08065D20();

/* sub_0806502C passes it a gUnknown_03001470 slot from sub_080152EC, with a
 * cast: struct Unk03001470 and struct Unk08580934_Obj probably describe the
 * same object, but they have not been merged. */
void sub_08064BC8(struct Unk08580934_Obj *, int, int, int);


/* ---- Callees around 0x0803B000 ---- */

void sub_08016E74(void);
void sub_08017688(u16);
void sub_08034334(void);
void sub_08034338(void);
void sub_08038690(int);
void sub_0803B83C(void);
void sub_0803B8C4(void);
u8 sub_080846F4(void);

void sub_0803BA1C(void);

/* Takes a hook and two pointers (see c_08012FB8.c); declared with empty
 * parentheses. */
void sub_08012FB8();

/* ---- Boot and main loop (AgbMain's unit) ---- */

/* sub_08036B48, the two-byte endless loop right after sub_08036B34, is a real
 * function that AgbMain calls. It is `static` in main.c and must not be
 * declared here: the ROM's call to it has no relocation, which only a local
 * symbol gives. */

/* Initialisation steps AgbMain runs at boot and reset. For example,
 * sub_0801BABC sets up interrupts, sub_08013434 the keys, and sub_08015184
 * frees all 30 gUnknown_03001470 slots. */
void sub_0801F018(void);
void sub_08036A50(void);
void sub_08036AB8(void);
void sub_08036B28(void);
void sub_08036B34(void);
void sub_08036C08(void);
void sub_08036C4C(void);
void sub_08036E18(void);
void sub_08036E54(void);
void sub_0801BABC(void);
void sub_080128C4(void);
void sub_0801B6BC(void);
void sub_0803486C(void);
void sub_08034848(void);
void sub_0801BCE0(void);
void sub_08015544(void);
void sub_08011C18(void);
void sub_08011A84(void);
void sub_080191B0(void);
void sub_08015184(void);
void sub_08010F94(void);
void sub_08013434(void);
void sub_0801F4A4(void);
void sub_0801295C(void);
void sub_0803B688(void);

/* Takes one argument that it ignores; AgbMain passes 0. */
void sub_08080F90(int);

/* Sets up the memory arena in a buffer of the given size and returns -1 on
 * failure. Declared with empty parentheses; the definition takes (void *buf,
 * u32 size). */
int sub_08014DA8();

/* Declared with empty parentheses; their definitions have the types.
 * sub_0801A79C stores its five arguments in the 0x0200CCxx link block, then
 * runs the link scan. sub_080129D4 takes a u32 seed. */
void sub_0801A79C();
void sub_080129D4();
/* sub_08016B2C copies the two blocks into the buffer and returns their size,
 * 0x5CC bytes; sub_08016BC0 copies them back. */
int sub_08016B2C(void *);
void sub_08016A54();
void sub_080366F4(void);

/* The reset entry at 0x0808AAD4. The argument is a flag word like
 * RegisterRamReset's; sub_08036CB4 passes 0xFE. */
void SoftReset(int);

/* ---- BIOS copy, palette backup and display resets ---- */

/* The BIOS CpuSet call, declared like CpuFastSet. */
void CpuSet(const void *, void *, u32);

/* Starts gUnknown_08613E54 under `parent`, backs up the 16 colours at
 * &gPal[index * 16] into gUnknown_0202F2DC record `index` (0x30 bytes each),
 * then stores that palette address at +0x24 of the record and `pal`, the ROM
 * palette data, at +0x20. Returns the record. */
void *sub_08071B28(const void *pal, int index, int b, ProcPtr parent);

/* Installs `handler` in IRQ handler slot `slot`. */
void sub_0801BB00(int, void *);

/* sub_08010FA0 and sub_08012A24 reset display shadow registers, as do
 * sub_080122EC and sub_08013324 below. */
void sub_08010FA0(void);
void sub_08012A24(void);
void sub_0801224C(u16, u16);
void sub_080122EC(void);
void sub_08013324(void);
void sub_0803DDF4(void);

/* Window/blend openers; each takes the proc it sets up. The struct tags stay
 * incomplete here on purpose: c_08071CF4.c and c_08071DB4.c define them, and
 * callers only pass the pointer on. */
struct Unk08071CF4;
struct Unk08071DB4;
void sub_08071CF4(struct Unk08071CF4 *);
void sub_08071DB4(struct Unk08071DB4 *);

/* ---- Callees of the gUnknown_0200C528 script commands (0x08018000) ---- */

void sub_08012A54(void *);
void sub_0802DCA4(void);
bool8 sub_0802C550(void);
void sub_0803B3E0(void);     /* c_0803B3C8.c */
/* Returns bool8, not s32: all three callers test the result as a byte. */
bool8 sub_08078198(void);
void sub_08043418(int, int, int);

/* Advances slot `a`'s gUnknown_0200C528 link (unk04 = unk04->unk04) and
 * returns 1. Returns int, not bool8: the callers narrow the result to their
 * own s16 return type. */
int sub_08018BAC(s16);

/* Helpers of the 0x08018000 block. sub_08018018 fills the 6x6
 * gUnknown_08499588 tilemap, blanking cells whose column plus the argument
 * passes 5. sub_08014824 reports whether any of sub_08014878's three scripts
 * is still running. */
void sub_08018018(u8);
void sub_0801815C(u8);
void sub_08018254(s16);
int sub_08014824(void);
void sub_0801A548(u16);

/* ---- The 0x08035000 block and its callees ---- */

/* sub_080355CC starts a unit-selection sprite proc at cell (x, y) and returns
 * it, or NULL when no sprite slot is free. sub_08042DE0 returns
 * sub_08042DCC's value for army a1's CO. */
ProcPtr sub_080355CC(u16, u16, u16, u16);
int sub_08042DE0(int);                    /* src/unit.c */
/* Returns &gUnknown_03003338[index]. Callers that hold another view of the
 * record cast the result. */
struct Unk03003338 *sub_080413A4(int);
/* The first argument is an army index into gPlayers. */
void sub_08041258(int, int);
/* Takes a packed u8 selector (bits 5-7 name an army) and returns a 1-based
 * block number into gUnknown_081218BC, 0x400 bytes per block. In src/map.c. */
int sub_08024984(int);
/* Map-cell test used by sub_08041F38: (x, y) is checked against the map
 * bounds, and the third argument picks the gUnknown_085D5ABC record whose
 * unk19 selects the terrain-cost row. */
u8 sub_08041EA8(s16, s16, int);
/* Probes the four cells around (x, y) with sub_08041EA8 and returns a
 * direction mask: 1 = up, 2 = down, 4 = left, 8 = right. x and y are int, not
 * s16: the function computes x - 1 and x + 1 before narrowing them. */
u8 sub_08041F38(int, int, int);
/* Starts the same proc as sub_08041820 when sub_0803DF54 finds no entry, but
 * clears the cell plane itself and stores the two coordinates in the new
 * proc. The third argument is used as a u8. */
void sub_0804189C(int, int, int);
/* Left incomplete on purpose: src/decomp/c_08035828.c completes the struct
 * privately, and callers only pass on a Proc_Find result. */
struct Unk35828Proc;
void sub_08035828(struct Unk35828Proc *);

/* The 0x08035000 block's own helpers. sub_08035124 calls sub_080350E4 when
 * its argument is a non-zero weather id different from gPlaySt.weather.
 * sub_080353E8 adds unk04/unk06 to unk00/unk02 in each of the 32
 * gUnknown_02027DE8 records. */
void sub_080350E4(void);
void sub_08035124(u8);
void sub_080352B4(void);
void sub_080353E8(void);
void sub_0803F5E4(int, int);
/* Returns &gUnknown_02028360[index]. */
struct Unk02028360 *sub_0803F5C8(int);
/* sub_0803F908 passes its third argument straight to PutSprite's `u16 *`
 * sprite data; the 0x0849FAxx blobs callers pass are declared `const u8 []`
 * but probably hold u16 data. sub_08027198 returns the first AI-controlled
 * army (1-4) whose team colour is its argument. sub_0803FC28 walks the
 * gUnknown_02028360 list, skipping records outside the box (a1, a2, a3, a4). */
void sub_0803F908(int, int, const u8 *, int, u8);
int sub_08027198(int);
void sub_0803FC28(int, int, int, int);
/* Decompresses the map's OBJ tile sets into VRAM relative to tile index a2.
 * The first argument is unused. */
void sub_0803FD80(int, int);
/* Mode-id lookups used by sub_0803F140 and its siblings: sub_0803F110 and
 * sub_0803F128 each pick one of two ROM blobs (a Decompress source and a
 * sprite descriptor), sub_0803F27C remaps the id, and sub_0803F29C writes a
 * size pair through its two out-parameters. */
u8 *sub_0803F110(int);
const u16 *sub_0803F128(int);
int sub_0803F27C(int);
void sub_0803F29C(int *, int *, int);
void sub_08035760(ProcPtr, void *);
/* sub_08035850 starts a ProcScr_DesignRoomPlaceUnit proc at cell (a1, a2)
 * when one of the three sprite slots is free. sub_08035BC4 spawns a
 * gUnknown_0849BDE0 sprite at cell (x, y) if that cell is on screen. */
void sub_08035850(int, int, int);
void sub_08035BC4(s16, s16, s16);
void sub_0801BDB4(s32, s32, u16 *, s32);
int sub_08042F5C(int);
int sub_08042FA4(int);
void sub_08035DF4(void *);
void sub_08035E90(ProcPtr);
/* MPlayPitchControl: sets the pitch of the tracks selected by trackBits. */
void sub_08071488(struct MusicPlayerInfo *, u16, s16);

/* ---- Callees of the 0x08036000 block ---- */

/* sub_0802759C reports whether a gUnknown_08499CFC proc is running.
 * sub_0804A010 writes the signature bytes A5 5A C3 3C to gUnknown_02028E41,
 * which sub_08036CB4 reads. */
bool8 sub_0802759C(void);
void sub_08036CB4(void);     /* src/main.c */
void sub_0804A010(void);
/* Forwards its pointer to the heap free routine. */
void sub_080364D4(void *);

/* Dispatches on the proc's gUnknown_0849CD88[unk36].unk1e; 0x8000 calls
 * sub_08035E90. */
void sub_08036024(ProcPtr);
/* Starts the gUnknown_0849D10C script through sub_080152C0. */
void sub_080364E0(void);
/* sub_08027278 starts a gUnknown_08499CFC proc at cell (x, y), positioned
 * relative to the camera. sub_0805C974 returns int, not u8: its callers narrow
 * the result themselves. */
void sub_08027278(int, int);
void sub_080360A4(ProcPtr);
void sub_0803647C(ProcPtr);
int sub_0805C974(void);

/* ---- Callees of the 0x0801B000 block ------------------------------------ */

/* Copies sub_0808AE30 (the SRAM read) onto the stack and calls it there.
 * Four parameters, although the only caller sets up two: with fewer, that
 * caller's register use no longer matches. */
void sub_0808AE54(u16, int, int, int);

/* Installs the timer IRQ handler for timer `id` (0-3) and writes the
 * handler's address, sub_0808AC20, through the second argument. Exactly two
 * parameters: the one caller's register use depends on it. */
u16 sub_0808AC44(u8, void (**)(void));

/* Copies sub_0808AED0 (the SRAM verify) onto the stack and runs it there.
 * The second argument is a source buffer address, typed int because the
 * caller sub_0801B648 also returns it as an int. */
int sub_0808AF00(u16, int);

/* ---- SRAM access ------------------------------------------------------- */

/* The two SRAM primitives: sub_0808AE30 copies n bytes and sub_0808AED0
 * verifies them. Declared here because sub_0808AE54, sub_0808AF00 and
 * sub_0808AF74 copy their code by address. sub_0808AED0 returns a pointer,
 * not a flag: the first mismatching address, or NULL when all bytes match. */
void sub_0808AE30(const u8 *, u8 *, int);
u8 *sub_0808AED0(const u8 *, u8 *, int);

/* sub_0808AF00 with a caller-supplied length instead of
 * gUnknown_08485550.unk18. */
int sub_0808AF74(u16, int, int);

/* ---- Flash driver ------------------------------------------------------ */

/* sub_0808AFE8 with a caller-supplied length: the same three-try
 * program-and-verify loop, using sub_0808AF74 in place of sub_0808AF00. */
int sub_0808B02C(u16, int, int);

/* Erase routines. Each returns a u16 status, 0x80FF for a bad sector number.
 * sub_0808B430 erases one sector and sub_0808B4B4 the 32 sectors of one save
 * slot; sub_0808B0E8 is sub_0808B430's twin, called by sub_0808B31C. */
u16 sub_0808B0E8(u16);
u16 sub_0808B430(u16);
u16 sub_0808B4B4(u16);

/* Programs one flash byte: writes *src to dst. */
u16 sub_0808B184(u8 *, u8 *);

/* Programs one sector: erases it, then writes the source buffer to it a byte
 * at a time through sub_0808B184. */
u16 sub_0808B31C(u16, u8 *);

/* Blank check: counts down gUnknown_03005C78->unk04 bytes from p and returns
 * the count left when it reaches a byte other than 0xFF. */
u32 sub_0808B2E0(u8 *);

/* Runs the chip-specific routine `f` and turns a non-zero result into error
 * code 0x8004. sub_0808B1BC also uses its address as the end of
 * sub_0808B2E0's code when it copies that function into RAM. */
int sub_0808B304(int, int (*)(int));

/* Passes its proc on to sub_08071AF0 as the parent. */
void sub_0808A368(ProcPtr);

/* Starts the timer for `slot`: the start half of the 0x03000F68 timer module
 * described in unknown-globals.h. */
void sub_0808AC7C(u8);
/* The stop half of the same timer module: stops the timer, clears its
 * REG_IE bit and restores REG_IME. */
void sub_0808AD24(void);

/* Parameters 3 and 4 are int, not u16: the function narrows only their
 * sum. */
int sub_0801B9C8(int, u32, int, int);

/* Sine and cosine in degrees: sub_0801BA4C(x) reduces x to 0..359 and reads
 * the gUnknown_0808F048 table, and sub_0801BAA8(x) is sub_0801BA4C(x + 90).
 * The argument is an angle, not a pointer. */
s16 sub_0801BA4C(int);
s16 sub_0801BAA8(int);

/* The sprite-copy flush paths call it with the copied range's address and
 * its halfword count. */
void sub_080718E8(void *, u16);

/* sub_0801B768 sets gUnknown_03002B80.unk358 to a + 1 and unk00 to 1.
 * sub_0801BB88 splits the pending OAM copy at object `a`. sub_0801BE78
 * rebuilds the sprite-list free chain. */
void sub_0801B768(int);
void sub_0801BB88(int);
void sub_0801BE78(void);
void sub_0801DF94(void);

/* ---- Callees of the 0x0806E000 block ------------------------------------ */

/* Returns a u16 pair (x, y) through two out-parameters. */
void sub_08073F90(u16 *, u16 *);

/* Two of sub_0806E11C's teardown calls. sub_0806D840 removes the
 * gUnknown_08581F40 script that sub_0806D820 installs. */
void sub_0806D34C(void);
void sub_0806D840(void);

/* ---- Callees of the 0x0801E000 block ------------------------------------ */

/* Builds the four OAM affine terms for gUnknown_0200F720[a] from the sine
 * and cosine of its angle and its two scale halfwords. It takes the record
 * index: declared (void), its three callers no longer match. */
void sub_0801E18C(int);

/* sub_0801E22C writes the first three halfwords of record `index` and
 * notifies; sub_0801E248 and sub_0801E264 below write prefixes of the same
 * run. sub_0801E0A4 copies the whole OAM shadow to the hardware. */
void sub_0801E22C(int, u16, u16, u16);
void sub_0801E0A4(void);

/* Returns *p. Returns int, not u16: sub_0801E950 passes the result on
 * without narrowing it. */
int sub_0801E334(u16 *);

/* Fills the sprite-request ring entry at gUnknown_03002510 and hands it to
 * sub_0801A718. Six parameters, and the fifth is 64 bits wide: the callers
 * pass a register pair there, and seven ints do not match them. Whether it is
 * one value or two adjacent words is unknown. */
int sub_0801E338(int, int, int, int, long long, int);

/* ---- The 0x0801D000 block ---------------------------------------------- */
/* The two workers the sub_0801D7D4 wrappers forward to. */
int sub_0801D6E8(int, int, int, int, int);
int sub_0801D78C(int, int, int, int, int, int);
/* The other branch of sub_0801D348. */
int sub_0801E4B0(int, int, int, int, int);
/* Wrappers over sub_0801D78C: sub_0801D7D4(a, b, c, d) passes
 * (a, b, c, d, 0, 0x1d) and sub_0801D7EC(a, b, c, d, e) passes
 * (a, b, c, 0, d, e). sub_0801D804 is sub_0801D7D4 with d = 0. */
int sub_0801D7D4(int, int, int, int);
int sub_0801D7EC(int, int, int, int, int);
/* sub_0801D348's seventh argument is 64 bits wide and goes to
 * sub_0801E338's `long long` parameter. sub_0801D81C stops the id held in
 * gUnknown_0200E438[a].unk38 (-1 means none). */
void sub_0801D348(int, int, int, int, int, int, long long, s16);
void sub_0801D81C(int);
void sub_0801E248(int, s16, s16);
void sub_0801E264(int, s16);
void sub_0801E27C(int, s16, s16, s16);
void sub_0801E294(int, s16, s16);
/* Fixed-point readers: each divides the record's s32 members by 256,
 * rounding toward zero, and writes the results as two s16. sub_0801D9AC reads
 * position plus offset, sub_0801D9E4 the offset alone and sub_0801DA14 the
 * position alone. sub_0801ECE8 is the masked twin of sub_0801E338. */
void sub_0801D9AC(int, s16 *, s16 *);
void sub_0801DA14(int, s16 *, s16 *);
int sub_0801ECE8(s16, int, int, int, long long, int);

/* Repacks its arguments for sub_0801ECE8 and reduces the result to 0 or -1.
 * The fourth parameter is the same 64-bit value as sub_0801E338's fifth. */
int sub_0801ED80(int, int, int, long long, s16);

/* ---- Proc starters of the 0x0803F000 block ---- */
/* Ends the stale instances of its script (through sub_0803FF2C), then starts
 * a new blocking instance under `parent`. a1 and a2 are stored in the new
 * proc; a3 goes to gUnknown_030044D4 and is signed (callers pass 0, -1, -2). */
void sub_0803FF48(int, int, int, ProcPtr);
void sub_0803F3E4(int, int, ProcPtr);

/* The two arms of sub_08041958. sub_0804074C takes the entry the proc holds
 * at +0x4c and the parent proc. sub_08040790's first two arguments are a cell
 * column and row. */
void sub_0804074C(struct Unk02028360 *, ProcPtr);
void sub_08040790(int, int, ProcPtr);

/* ---- Cross-unit callees of the 0x08040000 block ---- */
/* Wrappers over sub_0803FF48 that pass -1 (sub_0803FEDC) or -2
 * (sub_0803FF04) as its third argument and their own third argument as the
 * parent. */
void sub_0803FEDC(int, int, ProcPtr);
void sub_0803FF04(int, int, ProcPtr);
/* Takes a cell column and row in the gUnknown_08499590 grid and the parent
 * proc it passes on to sub_0804046C. */
void sub_08040380(int, int, ProcPtr);
/* The sixth argument is the parent proc. sub_08040624 is a four-argument
 * wrapper that fixes the third and fourth at 0x1CA and 5. */
void sub_08040554(int, int, int, int, int, ProcPtr);
void sub_08040624(int, int, int, ProcPtr);

void sub_0808A5C4(void);
/* Starts a script through sub_080152EC, fills in the slot it returns and
 * returns that slot. The third argument is a tilemap pointer. */
struct Unk03001470 *sub_08014740(s16, s16, u16 *, u16, u16, u16);
/* The timer IRQ handler sub_0808AC44 installs. */
void sub_0808AC20(void);

/* The BIOS LZ77 decompressor, VRAM variant: (source, destination). */
void LZ77UnCompVram(const void *, void *);
/* Halves each 5-bit colour channel of the 64 palette entries at src into a
 * scratch buffer, then copies that to dst. */
void sub_0804BD58(volatile u16 *, void *);

/* Callees of the 0x0802D000 block. sub_080637AC returns the
 * gUnknown_03001470 slot whose script (unk00) is `a`, or NULL. */
void sub_08029948(int);
struct Unk03001470 *sub_080637AC(const void *);
void sub_080236E8(void);
void sub_08042650(void);
void sub_08042864(void);
void sub_08060684(void);
void sub_080606A0(void);
/* Cases 0xd and 0xe of sub_0805FFA0's 20-way jump table. Both take their
 * operands from globals; sub_08060110 buys the unit that gUnknown_030046C0
 * describes. */
void sub_08060110(void);
void sub_08060170(void);
/* Nine more arms of the same jump table, then two functions that are not
 * arms of it: sub_0802428C copies the gUnknown_030040A4 cell into the cursor,
 * and sub_08035810 passes the running ProcScr_SelectUnit proc to
 * sub_08035828. */
void sub_080600F0(void);
void sub_080601C8(void);
void sub_080601DC(void);
void sub_080601F0(void);
void sub_08060264(void);
void sub_080602C4(void);
void sub_080606BC(void);
void sub_0802C16C(void);
void sub_08042B84(void);
void sub_0802428C(void);
void sub_08035810(void);
/* sub_080176A4 is empty. sub_08035170's u8 return is a guess: its caller
 * only stores the result into a byte. sub_0802D5B8(dst) decompresses the
 * blob sub_08037250 returns into dst. sub_0802D76C clears rows 5-18,
 * columns 1-15 of the BG0 tilemap. */
void sub_080176A4(void);
void sub_080198D0(void);
u8 sub_08035170(void);
u8 *sub_08037250(void);
void sub_0802D5B8(void *);
void sub_0802D76C(void);
void sub_0802D7B4(int);
/* Wrapper over sub_0802216C: folds cell (x, y) into the tilemap pointer and
 * passes zeroes for the trailing options. */
void sub_0802239C(u16 *, u16, u16, u8, u16, u8, u8);

/* Callees of the 0x08025000 block. sub_08024F20 is CalcBattleDamage
 * (src/battle.c). sub_080254AC and sub_08025AEC each return a free
 * gUnknown_08499594 unit slot, or NULL; sub_08025BE0 is InitUnit, which
 * fills one in. */
void sub_08024F20(s16, s16, struct Unk802C57C *);
void sub_080251D8(int);
void sub_080211DC(u8, s8);
struct Unit *sub_080254AC(void);
struct Unit *sub_08025AEC(void);
void sub_08025BE0(struct Unit *, u8);
void sub_08025D20(int);
void sub_08035740(void *);
/* Takes a free unit slot from sub_08025AEC and returns it, or NULL. The
 * parameters are s16, not u16: sub_08025C98 and CreateUnitAt sign-extend
 * their arguments. */
struct Unit *sub_08025C5C(s16, s16, s16);
/* CreateUnitAt: returns sub_08025C5C's slot, or NULL. */
struct Unit *sub_08025CC8(s16, s16, s16);

/* sub_0803CCEC returns slot `a`'s name string, or the ROM fallback
 * gUnknown_0849F320 when the slot is empty. */
int sub_0803CD14(u8);
u8 *sub_0803CCEC(u8);

/* Empty: returns at once. The second parameter is u16: sub_080265D0
 * narrows its argument before the call. */
void sub_08026584(u8, u16);
/* Two of sub_08037FD0's callees. sub_08017720's parameters are int: narrow
 * ones add conversions the original does not have. */
void sub_080265D0(u8, u8);
void sub_08017720(int, int, int, int);
/* More of sub_08037FD0's callees. sub_08026520 refreshes each army's
 * per-turn summary bytes. sub_08037F94's first parameter is unused.
 * sub_08020984 recomputes every army's gPlayers unk1c byte. sub_0803FECC wraps
 * sub_0803FF48 with a3 = 0. sub_08025D60 walks a 12-byte record list. */
void sub_08026520(void);
void sub_08030574(void);
void sub_08037F94(int, ProcPtr);
void sub_08037FB4(ProcPtr);
void sub_08020984(void);
void sub_0803FECC(int, int, ProcPtr);
void sub_08025D60(int);
/* Compacts a list in place: while the current element's flags have any of
 * 0x3c0 set, the next element is shifted down over it. The struct is
 * completed in src/decomp/c_0803E0D0.c; callers with another view cast. */
struct Unk3E0D0;
struct Unk3E0D0 *sub_0803E0D0(struct Unk3E0D0 *);
/* Two arms of sub_080407E4's jump table. sub_080402B4 is DestroyPipeSeam:
 * it updates the terrain map after the pipe seam at (x, y) is destroyed. */
void sub_08040200(struct Unk02028360 *, ProcPtr);
void sub_080402B4(int, int, ProcPtr);
/* The other two arms of that jump table. Each starts the 0x0849FADC proc at
 * the entry's tile position, through sub_0803FEDC and sub_0803FF04. */
void sub_0804026C(struct Unk02028360 *, ProcPtr);
void sub_08040290(struct Unk02028360 *, ProcPtr);
/* Refreshes the cached terrain byte of every gProperty entry from the map. */
void sub_08021CB4(void);
/* sub_08040430 loads one unit sprite's tiles (to OBJ tile `tile`) and
 * palette (to OBJ palette `pal`). sub_0804046C's fifth argument is the parent
 * proc. */
void sub_08040430(int, int);
void sub_0804046C(int, int, int, int, ProcPtr);
void sub_080232CC(int, int);
void sub_0804096C(ProcPtr);

/* Callees of sub_08052EE4 and sub_08052F20. sub_08012420 is the VBlank flush
 * that copies every shadowed display register to the hardware. */
void sub_08012420(void);
void sub_080546F0(void);
void sub_08054B14(void);
void sub_08057270(void);


/* ---- Callees of the 0x08031000-0x08039000 blocks ---- */

void sub_080337D8(u32, u32, ProcPtr);
/* The five-argument sibling of sub_080337D8. Returns -1 when the size
 * argument exceeds 0x7FFF80, else 0. */
int sub_0803376C(u32, u32, int, u8, ProcPtr);
/* Callees of the 0x08032000-0x08034000 blocks. sub_08026704 advances an army
 * slot index, wrapping from 5 to 1, to the next army that is alive and
 * active. sub_080348B4 is ShouldPromptCountryName. */
u8 sub_0803CD2C(u16, u8);
u16 sub_08026704(int);
bool8 sub_080348B4(void);
s8 sub_0802F4F4(void);
/* Bit readers: each returns bit `index` of a gUnknown_0849B018 member. */
bool8 sub_0802F460(s8);
bool8 sub_0802F480(s8);
/* sub_0802F534 returns s8 even though its caller zero-extends the result; do
 * not change it to u8. sub_08063454 arms a transfer on the link record, or
 * resets it; sub_08063518 tests the link record's state. */
s8 sub_0802F534(void);
void sub_08063454(struct Unk08062FB8 *, int, int, u8, s8);
int sub_08063518(struct Unk08062FB8 *);
/* The BIOS multiboot call (SVC 0x25). struct Unk08062FB8 has the SDK's
 * MultiBootParam layout. */
int MultiBoot(struct Unk08062FB8 *);
/* Queues a 128-byte packet (type 0xAF), copied from the buffer at a1, on the
 * gUnknown_0849B018 link record. The parameter is a byte buffer but stays u32
 * because the caller passes a u32 struct member; the function casts. */
void sub_08030930(u32);
/* sub_0803388C takes a cursor value and the parent proc. sub_0802BFA8 starts
 * the gUnknown_0849A428 script. */
void sub_0803388C(int, ProcPtr);
void sub_080338C0(int);
void sub_08026900(void);
void sub_0802BFA8(void);
void sub_080351F0(void);
/* Draws string `s` centred on row `y`. */
void sub_08034A58(int, const char *);
void sub_080328C0(u16 *);
void sub_08030F60(int);
/* sub_080138B0 with twice the bias: the brighten direction of the palette
 * fade. The second argument is signed. */
void sub_0801394C(u8, s8);
/* sub_0802F588 queues halfwords into a ring and returns how many it queued,
 * or -1 when the ring is full. sub_0803227C steps the army-slot cursor left or
 * right, skipping empty slots. The byte count is a u16: the body converts it
 * at entry, and every caller passes a small constant. */
int sub_0802F588(struct Unk0202575C *, u16);
void sub_0803227C(void);
/* sub_08032340 takes the proc and a pixel position (x, y). sub_08032950 is
 * sub_0803227C's wrap-around twin. */
void sub_08032340(ProcPtr, s16, s16);
void sub_08032950(void);
void sub_08032A00(void);
/* Two HBlank/VCount handlers that install each other with sub_080638D0,
 * so each needs the other's address. */
void sub_08032B84(void);
void sub_08032BA4(void);
/* sub_08044144 is SetPlayerCoPowerStatus. sub_08044B28 is ActivateCoPower,
 * which sets an army's CO-power step and runs that step's callbacks; its
 * third argument is the parent proc. */
void sub_08044144(int);
void sub_08044B28(int, int, ProcPtr);
/* StartCoPowerScript; the third argument is the parent proc. */
void sub_08080E74(int, int, ProcPtr);
void sub_08080E40(ProcPtr);
/* sub_08039820's predicate. */
u8 sub_08039850(ProcPtr);
/* Shows one of six random CO-power quotes for the proc's army through
 * sub_080397F4, and returns 0. */
u8 sub_080398D0(ProcPtr);
void sub_080397F4(u16);
void sub_08039BB4(int, int, int);

/* ---- Callees of the 0x08014000-0x08028000 code ---- */

/* An empty function; its signature comes from its four call sites. Only the
 * second parameter and the u16 return are fixed by them; the int parameters
 * could be narrower. */
u16 sub_0801489C(int, u16, int, int, int);
/* sub_08014D38 measures a string in pixels (gUnknown_084C36E4 holds the
 * character widths); sub_08014D20 converts that width to tiles. The width is
 * signed. */
int sub_08014D38(const char *);
int sub_08014D20(const char *);
/* Two halfword lookups on a table indexed by gUnknown_03001FBC. They return
 * u16, not s16: the callers cast the result to s16 themselves. */
u16 sub_08015820(s16);
u16 sub_080157D0(s16);
/* The animation-handle allocator and initialiser behind sub_0801C210:
 * sub_0801C6E8 returns a free gUnknown_03000288 slot (of 16) or NULL, and
 * sub_0801C69C fills it from sub_0801C210's three arguments. */
struct Unk0801C210 *sub_0801C6E8(int);
void sub_0801C69C(struct Unk0801C210 *, void *, u16, u8);
/* One animation step, as sub_0801C254 runs it: sub_0801C27C draws the
 * handle's current frame at (a2, a3), and sub_0801C2DC advances it and
 * returns a u8. */
void sub_0801C27C(struct Unk0801C210 *, int, int);
u8 sub_0801C2DC(struct Unk0801C210 *);
/* sub_0801C3EC and sub_0801C53C are sub_0801C27C's two conditional side
 * calls; sub_0801C3EC builds the OAM affine parameters for the handle's
 * affine list. sub_0801C640 installs a script in the handle (`void *`: bit 1
 * of +0x20 selects how it is read), and sub_0801C51C passes its second
 * argument on to it. sub_0801C67C runs one sub_0801C2DC step with +0x18 and
 * +0x1a forced. */
void sub_0801C3EC(struct Unk0801C210 *);
void sub_0801C53C(struct Unk0801C210 *);
void sub_0801C640(struct Unk0801C210 *, void *);
void sub_0801C51C(struct Unk0801C210 *, void *);
void sub_0801C67C(struct Unk0801C210 *);
/* IsPlayerAliveAndActive. Spelled bool8 to match the definition's text:
 * tools/proto_check.py compares declarations as text. */
bool8 sub_080266DC(u8);
/* A saturating increment of a u16 global. */
void sub_080176A8(void);
/* Returns sub_08042E18's value for army a1's CO. In src/unit.c. */
int sub_08042DFC(int);
/* Team colours. sub_08026A88(slot, colour) returns whether `colour` is
 * unused by army slots 1 .. slot-1. sub_08026AC0(slot, fallback) picks slot's
 * colour: the chapter preset, or `fallback` when there is none, moving on to
 * another colour if that one is taken. */
bool8 sub_08026A88(int, int);
int sub_08026AC0(int, int);
/* sub_0802672C steps to the next live army like sub_08026704 and returns
 * whether it landed before the one it started from. sub_08026A48 assigns
 * every army its team colour (game modes 1 and 2 only). sub_08026B28
 * rebuilds each army's mask of the armies it is at war with. */
bool8 sub_0802672C(void);
void sub_08026A48(void);
void sub_08026B28(void);
/* sub_08026C6C returns gPlaySt.propertyFunds for six of the ids 6..20 and 0
 * for the rest. sub_08026CD0 applies the chapter record's preset flag pairs
 * to the five armies. */
u32 sub_08026C6C(u8);
void sub_08026CD0(void);
/* sub_08026F28 compares two armies' unk2a team bytes. sub_08028B70 returns
 * the first army slot at or past gPlaySt.unk31, or 0. sub_08028BAC reports
 * whether exactly one team is left. sub_08028A68 runs the per-army end-of-turn
 * checks; sub_08028AEC is MarkDefeatedArmies, the same checks reported to
 * sub_08028874. sub_080271CC takes int, not u16: sub_080289BC passes an
 * unnarrowed int. */
bool8 sub_08026F28(u16, u16);
u8 sub_080271CC(int);
int sub_08028B70(void);
u8 sub_08028BAC(void);
void sub_08028A68(void);
void sub_08028AEC(void);
void sub_08027118(void);
void sub_08025EA0(void);
/* sub_08028874's second parameter is int, not u8: sub_08028894 passes its
 * own int parameter to it unnarrowed. */
void sub_08028874(int, int);
void sub_08028894(int, int);
u8 sub_080288D8(u16);
u8 sub_08028904(u16);
/* sub_08028944 reports whether army `a` is still playable. sub_08028568 is
 * FinalizeMatchResult. */
bool8 sub_08028944(u16);
u8 sub_08028990(u16);
u8 sub_080289BC(int);
void sub_08028568(void);
void sub_08028168(void);
/* sub_080276D0 and sub_080276F0 load gUnknown_03003130.unk04 from
 * gUnknown_08090A98[0] and [1]. sub_08027A08 is sub_08027844's twin on the
 * other axis. sub_08027FBC copies 0x200 bytes to VRAM between two tile
 * numbers. */
void sub_080276D0(void);
void sub_080276F0(void);
void sub_08027844(void);
void sub_08027A08(void);
void sub_08027FBC(void *, u16, u16);
/* The 0x08005000 menu block. sub_08005838 and sub_080059B4 read only their
 * third argument; the first two are unused. */
void sub_08005154(void);
void sub_0800517C(void);
void sub_0800518C(void);
void sub_08005580(void);
void sub_08005838(int, int, u8);
void sub_080059B4(int, int, u8);
void sub_08005D14(void);
void sub_08005EF0(int);
void sub_080145BC(void);
/* The gUnknown_03000050 heap's allocator, one level below sub_08014E44: it
 * takes the heap handle and a size. HeapAlloc and HeapAllocAligned are the
 * names of sub_08014DCC and sub_08014FF8; HeapAllocAligned hands alignments
 * of 16 or less to sub_08014E44. */
void *sub_08014DCC(int, u32);
void *HeapAlloc(int, u32); /* = sub_08014DCC */
void *HeapAllocAligned(int, u32); /* = sub_08014FF8 */
/* Frees a heap block. Returns 0 when it frees, 1 for a null pointer or a
 * block that is already free. */
int sub_08014E68(int, void *);
int sub_08014D7C(void *, u32);
void sub_08028848(u16, u16);
/* Takes the word sub_080281D8 stores at +0x18 of a sub_080152EC slot and,
 * if it is non-zero, passes it to sub_080196F4. */
void sub_08028190(struct Unk03001470 *);
/* Calls sub_080436DC and DrawDaysRemaining with `a`. In src/unit.c. */
void sub_0804360C(int);
/* sub_08022DD4 takes an s16 cell and a switch selector and ends by calling
 * sub_08022BB8 with its three arguments; sub_08022BB8's signature is read
 * from that call. sub_080230C4 forwards to sub_08022DD4. sub_080230DC eases
 * the camera halfway to a target each call; its third parameter is unused and
 * the last two are out-parameters. */
void sub_08022BB8(s16, s16, s16);
void sub_08022DD4(s16, s16, s16);
void sub_080230C4(s16, s16, s16);
void sub_080230DC(s16, s16, s16, s16 *, s16 *);
/* Steps the gUnknown_030033E4 cursor one cell in the direction the held
 * keys select (gUnknown_08499C7C). */
void sub_0802361C(void);
/* GetUnitVisionWithCoBonus (src/unit.c). Takes a gUnknown_08499594 slot
 * number and that slot's unit-type id; the second parameter is int, not u8. */
int sub_08042D84(int, int);
/* sub_080210C8 passes one of three record runs in gMap->visible to
 * sub_08020EDC, chosen by `kind`. */
void sub_080210C8(s16, s16, s16, s16, s8, int);
void sub_08049FB0(void);
void sub_08049FD4(void);
void sub_08049EB4(void);
void sub_08049B80(void);
/* Three parameters although the body reads only the first: its caller
 * sub_08003088 sets up all three. In src/design-panels.c. */
void sub_080030BC(int, int, int);
/* Draws one gUnknown_0849EDB0 row into tilemap a3 at cell (a1, a2), in
 * palette a5. */
void sub_080487B4(u8, u8, u16 *, u16, u16);
/* sub_08075904 indexes 0x30-byte records at gUnknown_08615194 + 0xc and can
 * return 0. sub_080879A0 ends every instance of its script (Proc_EndEach).
 * sub_0803D960's argument is the parent proc. */
int sub_08075904(int);
void sub_080879A0(void);
void sub_08085F40(void);
void sub_0803D960(ProcPtr);

/* ---- Miscellaneous callees (0x08000000-0x0803B000) ---- */

void sub_0802C2B4(void);   /* starts gUnknown_0849A990, hides BG0 */
void sub_0803B3F8(void);   /* one-call forwarder to sub_0806FD98 */
void sub_0803B408(void);   /* one-call forwarder to sub_0807046C */
void sub_08011FF0(void);   /* drains the gUnknown_0200B3B4 copy queue */
void sub_0802E920(void);   /* VBlank handler sub_0802EA24 installs */
void sub_0802E960(void);   /* passed to SetMainLoopCallback */

/* sub_08002EC8, sub_080035C8 and sub_08003640 are defined in src/design.c,
 * src/design-sprites.c and src/design-editor.c. */
void sub_0800057C(void);
void sub_08002EC8(void);
void sub_080035C8(void);
void sub_08003640(void);

/* sub_08034F60 is GetUnitSelectionLock. Both return u8, not int: their
 * callers narrow the result before testing it. */
u8 sub_08034F60(void);
u8 sub_0802DBF8(void);

/* The parameters are s16, not u16: sub_0802DCB4 passes sign-extended
 * values. */
void sub_0802E4B4(s16, s16);

/* The parameters are s16, not u16: the callers pass sign-extended values. */
void sub_08022AAC(s16, s16);

/* The BIOS VBlankIntrWait call (SVC 5). */
void VBlankIntrWait(void);

/* Writes a 2x2 block of tilemap entries through the first argument (+0, +2,
 * +0x40 and +0x42 bytes). */
void sub_0802216C(u16 *, u8, u16, u8, u8, u16, u16, u8);

/* Returns an s16 that can be -1; sub_08007DB0 tests it for >= 0. */
s16 sub_08007DD0(int, int);

/* sub_080016D0 is GetTileWithShadowAt and returns GetTileWithShadow's
 * result. sub_08007D70 is MakeForestSimple: it re-tiles the cell (x, y) and
 * the one to its right. */
int sub_080016D0(int, int);
void sub_08007D70(int, int);

/* ---- More miscellaneous callees ---- */

/* Starts the gUnknown_08499FEC proc as a blocking child of `parent`. */
void sub_08028ED0(ProcPtr);

/* Returns gUnknown_030040A8. u32, not u16: sub_08029234 stores the result
 * without narrowing it. */
u32 sub_08041398(void);

void sub_0802723C(int, int);

/* sub_08029088 and sub_0802909C are this call with 0 and 1 as the third
 * argument. */
void sub_080290B0(int, int, u8);

/* Called with &gUnknown_030040C0. */
void sub_0802EA5C(struct Unk030040C0 *);

void sub_0802EAFC(void);

/* The link handshake's per-frame step. Returns -1 or gUnknown_0300055C's
 * word. */
int sub_0802EB28(void);

void sub_08046030(void);

/* sub_08001DAC, sub_08002AB0 and sub_08002C38 are defined in src/design.c.
 * sub_0801F1EC re-uploads the graphics of the gUnknown_0200F920 entry for
 * tile id a1 to a2. */
void sub_08001DAC(void);
void sub_08002AB0(void);
void sub_08002C38(void);
void sub_0801F1EC(int, int);
/* sub_080247A4 is LoadMapData (src/map.c). */
void sub_080247A4(u16);
void sub_080860DC(ProcPtr);
/* sub_08037A20 fills a tilemap with consecutive tile ids from `base`,
 * covering the map in 2x2-tile blocks. `base` must stay int: a u16 parameter
 * adds a conversion the original does not have. */
void sub_08037A20(u16 *, int);
void sub_080620C0(void);
void sub_080620FC(int, int);      /* (index, value) */
void sub_0806209C(void);
/* Rebuilds the gUnknown_03003F20 cell list from gUnknown_084995A0. */
void sub_08062330(void);
/* Finds a cell and writes its coordinates through the two out-parameters;
 * returns 1 if it found one, else 0. The out-parameters are int *, not u8 *. */
int sub_080623C4(int *, int *);
void sub_08062474(void);
void sub_08062560(u16, u8);
/* sub_080627F4 adds each army's unit strength into the gUnknown_0202DAD8
 * grid. sub_08062AE4 stores a percentage for each 4x4-cell block of the map
 * in gUnknown_0202DAD8[j][i].unk28. */
void sub_080627F4(u8);
void sub_08062AE4(void);
void sub_08062C94(void);
/* sub_08062730 compares the distance between two units against a cost. It
 * returns int; its caller narrows the result to u8 itself. */
int sub_08062730(struct Unit *, struct Unit *);
void sub_08077620(int, int);      /* draws a banner and its icon */

/* sub_08077140 fills a 6x6 block of a 32-wide tilemap with consecutive
 * tiles; sub_08077180 does the same with four rows. sub_080772B8's struct is
 * completed privately in src/decomp/c_0807728C.c: keep the tag, do not retype
 * the parameter to u16 * from the call site. */
struct Unk080772B8;
void sub_08077140(u16 *, u16, int);
void sub_08077180(u16 *, u16, int);
void sub_080772B8(struct Unk080772B8 *);
void sub_080758BC(int, int, int, ProcPtr);
/* Returns its seventh argument narrowed to 16 bits; the only caller ignores
 * it. The parameters are int, with casts at their uses: u16 parameters do not
 * match. */
int sub_08077214(u16 *, int, int, int, int, int, int, int);
/* Twin of sub_08043FD8: draws the gUnknown_084A07DA sprite list through
 * PutSpriteExt at (x, y). */
void sub_0804402C(int, int, int, int);

/* sub_080771C0 and sub_080771F0 are two HBlank handlers that install each
 * other, so each needs the other's address. */
void sub_080771C0(void);
void sub_080771F0(void);
void sub_08002EF8(void);

/* ---- The 0x08063000 block ---------------------------------------------- */

/* Splits `val` into decimal digits written through the three pointers. 0xFF
 * blanks a digit; a zero input gives 10 in all three. */
void sub_08063A58(int, u8 *, u8 *, u8 *);
/* Rotating sprite: four Div calls feed SetObjAffine, then sub_0801BD00.
 * sub_08063B50 is its only caller. */
void sub_08063CCC(int, int, int, int, int);

/* ---- The 0x08064000 block: rotation matrices --------------------------- */

/* Link-port access on the link record; it writes SIOCNT directly. */
int sub_080633E4(struct Unk08062FB8 *, u16);

/* Matrix helpers. sub_08063FEC, sub_08064034 and sub_0806407C each fill one
 * 0x30-byte matrix from an s16 angle; sub_08063E28(a, b, out) composes two
 * matrices; sub_08063DDC(v, m, dst) rotates a three-word vector by m.
 * The tags must be declared at file scope first: a tag first seen inside a
 * prototype has prototype scope and would not match the definitions.
 * sub_08064034 and sub_0806407C use their own tags because their definitions
 * do. */
struct Vec3;
struct Mtx43;
struct Unk64034Mtx;
struct Unk6407CMtx;

void sub_08063FEC(struct Mtx43 *, s16);
void sub_08064034(struct Unk64034Mtx *, s16);
void sub_0806407C(struct Unk6407CMtx *, s16);
void sub_08063E28(struct Mtx43 *, struct Mtx43 *, struct Mtx43 *);
void sub_08063DDC(struct Vec3 *, struct Mtx43 *, struct Vec3 *);

/* ---- The 0x08085000 block ---------------------------------------------- */

/* Builds the twenty-row unit-info table for army a2. The first parameter is
 * unused, but the caller passes it. */
void sub_08085708(s16 *, int);
/* sub_080859A0 is a six-argument forwarder onto PutSprite. sub_08085410 is
 * GetFirepowerIcon. */
void sub_080859A0(u16, u16, int, int, int, int);
int sub_08085410(int, int);
int sub_08085638(int, int);
int sub_080856A0(int, int);
/* Returns army `index`'s palette row in gUnknown_0810E6E0 (0x20 bytes per
 * row, chosen by gPlayers[index].unk1a). */
u8 *sub_080261C8(int);

/* ---- The 0x0804C000 block ---------------------------------------------- */

/* The third argument is signed: callers pass the s16 gUnknown_03001FBC, and
 * it is compared against -1. */
void sub_0804CA98(u16, u16, s16);
/* Same source as sub_0804BDD8, at a second address. */
int sub_0804BECC(u16, u16, s16);

/* ---- The 0x08050000 block ---------------------------------------------- */

/* Stops song `n` (the SDK's m4aSongNumStop). */
void sub_08070544(u16);

/* ---- The 0x08002000 and 0x08057000 blocks ---------------------------- */

/* GetDesignRoomOption (src/design.c). */
int sub_08001230(int);
/* OBJ-graphics lookups for sub_08002964 and sub_080029F4. sub_0802A85C is
 * GetTerrainNameGraphic. */
const u8 *sub_0802A85C(int);
const u8 *sub_0802A838(int);
/* GetTerrainNamePalette; sub_08002DEC passes the result to ApplyPaletteExt. */
const u8 *sub_0802A8AC(int, int);
/* Takes the six-halfword stack record sub_08057048 fills. */
void sub_080570C4(void *);
/* Stores chr, pal and flip into gUnknown_08551A00[offset]. */
void sub_08057110(u16, u16, u16, u16);
/* The gUnknown_08551A04 twin of sub_08057110. */
void sub_0805701C(u16, u16, u16, u16);
/* Sprite-row painters, all called as (dst, index, &pos). The pos record is
 * {u16 x; u16 y;}; each .c completes the tag. */
struct Unk8057Pos;
void sub_080576D4(u16 *, int, struct Unk8057Pos *);
void sub_0805772C(u16 *, int, struct Unk8057Pos *);
void sub_080577E4(u16 *, int, struct Unk8057Pos *);
void sub_08057860(u16 *, int, struct Unk8057Pos *);
void sub_08057A24(u16 *, int, struct Unk8057Pos *);
/* The two per-side fan-outs over the painters above. */
void sub_080579B8(u16 *);
void sub_08057A80(u16 *);
/* Claims the first free of 32 slots and returns its index, or -1. Returns
 * int: sub_0801D6E8 returns the value unchanged. */
int sub_0801E13C(void);


/* ---- The 0x08027000 block ---------------------------------------------- */
/* The two halves of one clamp pair on gUnknown_03003130.unk04: sub_080275B4
 * steps it down towards 3 and sub_08027608 up towards 0xad, the bounds held in
 * gUnknown_08090A98[0] and [1]. */
void sub_080275B4(void);
void sub_08027608(void);
/* Callbacks whose addresses sub_0802776C hands to sub_0801F024.
 * sub_08027658 is DrawInfoBoxCombobox. */
void sub_08027658(void);
void sub_08027710(void);
/* sub_08016944's twin that clears the bit. */
void sub_08016974(int);


/* ---- The 0x08025000 block ---------------------------------------------- */
/* sub_08026588(a, b, c) takes two army numbers and a unit-type byte. Its
 * body reads only the first, but sub_080250E8 passes all three. It keeps a
 * running count in unk16 and its high-water mark in unk18. */
void sub_08025D20(int);
void sub_08025D40(int);
void sub_08026588(u8, u8, u8);
/* sub_08042C68 is defined in src/unit.c. sub_08025B24 is empty; its callers
 * pass a unit record and a displayed-HP value, Div(hp - 1, 10) + 1 or 0. */
int sub_08042C68(int, int);
void sub_08025B24(struct Unit *, int);
void sub_0802A5C4(struct Unit *);
/* The 0x0802A3FC unit-scan block. sub_0802A2E4 and sub_0802A304 are
 * callbacks for sub_0802A38C and take a unit record as void *. sub_0802A38C
 * and sub_0802A258 are not declared here on purpose: their definitions use
 * file-local struct types, so their callers declare them locally. */
int sub_0802A2E4(void *);
int sub_0802A304(void *);
void sub_0802A3FC(void);
void sub_0802A6B0(void);
/* SubtractPlayerFunds. */
void sub_08025B28(u16, u32);
/* Predicates on the gUnknown_085D5ABC[type].unk14 permission table.
 * sub_08025F74 (CanTransportCarry) takes the unit record; sub_08025EF0 takes
 * two unit ids and also requires them to share an army. */
bool8 sub_08025F74(struct Unit *, u8);
bool8 sub_08025EF0(int, int);
bool8 sub_08025FC0(struct Unit *, struct Unit *);
bool8 sub_080253B0(struct Unit *);

void sub_080679D8(int, int, int, int, int, int, int, int, int, ProcPtr);
/* 0x08067000 proc helpers driven by the 0x08069000 timeline functions.
 * sub_080677E8 resets the gUnknown_08580FE4 scroll proc. sub_08067BD0's second
 * argument is signed (callers pass 1 or -1); its fourth is the parent. */
void sub_080677E8(void);
void sub_0806780C(void);
void sub_08067A24(void);
void sub_08067BD0(int, int, int, ProcPtr);
void sub_08067C7C(u32);
void sub_08067D04(int, int, int, ProcPtr);
void sub_08067D4C(void);
void sub_08067DD4(ProcPtr);
void sub_08067ED0(u8, u16, int, int, int, int, u8, u8, ProcPtr);
/* Starts a gUnknown_08581210 proc under `parent` for CO a2 and loads the
 * CO's name graphics through sub_08068038 at tile a3; a4 is the palette. */
void sub_080686E8(int, int, u16, u8, ProcPtr);
/* Loads graphics for a 0xff-terminated id list, 0x20 bytes of VRAM apart
 * from tile a2. Returns the number of entries loaded. */
int sub_08068038(u8 *, u16);
/* sub_080686E8's seven-argument sibling; it fills four more proc bytes. */
void sub_08068810(int, int, int, int, u16, u8, ProcPtr);
void sub_080673D0(u32, u32, ProcPtr);
void sub_080678D4(u32);
void sub_0806978C(void);
void sub_080697A4(void);
void sub_080697BC(void);
void sub_08069924(u8);
void sub_0806974C(ProcPtr);
void sub_0806E7C0(int, int, ProcPtr);
/* sub_0806AEC4 loads one gUnknown_0858178C row's graphics and palette. */
void sub_0806AEC4(int);
void sub_0806E210(int, ProcPtr);
/* The struct is completed privately in src/decomp/c_0806F0A0.c; callers
 * outside that file cast. */
struct Unk0806F0A0Proc;
void sub_0806F0A0(struct Unk0806F0A0Proc *);
/* Starts a gUnknown_08581138 proc under the given parent and stores the three
 * values in it. */
void sub_08068014(int, int, int, ProcPtr);
/* The argument is the calling proc; the function may start a
 * gUnknown_0858175C child proc under it. */
void sub_0806AD04(ProcPtr);
/* Writes tilemap entry v at p[0] and v + 1 one row below at p[0x20], then
 * calls sub_08013AEC. The first parameter must stay `u16 *`: through a struct
 * pointer the compiler orders the two stores differently. */
void sub_0806BD6C(u16 *, u16);
/* sub_080697CC loads one screen's tiles, palette and BG1/BG2 tilemaps (called
 * by sub_08069864). sub_0806B87C starts the gUnknown_085819C4 fade proc under
 * the calling proc. */
void sub_080697CC(void);
void sub_0806B87C(ProcPtr);

/* ---- The 0x08043000 block ---- */
/* Returns the current map's turn limit: the map's gUnknown_085C77A0 timer,
 * or gPlaySt.turnLimit when that is 0. */
int sub_08043630(void);
/* Called only by sub_080438FC. Pointers 1-3 and 8 are values it updates in
 * place; the sixth argument selects a mode and the seventh is a flag. */
void sub_080439A8(int *, int *, int *, int, int, int, int, int *);

/* ---- Callees of the 0x08021000-0x08025000 blocks ---- */
/* Applies the second unit's attack to the first: scales the display damage by
 * the first unit's defence and the attacker's HP (capped at 999), then sets
 * the first unit's hpLoss and remainingHp. Defined in src/battle.c. */
void sub_08024DDC(struct BattleUnit *, struct BattleUnit *);
/* Copies two team-colour entries per army into the palette, from
 * gUnknown_080D3DE4 when a1 is non-zero and gUnknown_080D3EE4 otherwise; a2
 * picks the first destination bank. Defined in c_0803F80C.c. */
void sub_0803F880(int, int);
/* Looks up the map tile at (x, y) in the 0xFFFF-terminated gUnknown_08499B0C
 * list, saves the old tile in gUnknown_030033F8, and replaces it with the
 * variant in the same group of five that the third argument's top three bits
 * select. Returns the group's first index. */
s16 sub_080240B4(s16, s16, u8);
/* sub_080240B4's twin: does not save the old tile, and only rewrites tiles
 * in the first group. Defined in c_080240B4.c. */
s16 sub_0802419C(s16, s16, u8);
/* GetTerrainDefense (src/map.c): terrain a2's defence value times 10, or 0
 * when unit type a3 is one whose gUnknown_085D5ABC deployLocation is 0x10.
 * The first argument is unused. */
int sub_080249EC(int, s8, u8);
/* Per-frame tile animators; the second argument is the game clock or a value
 * derived from it. sub_0803F8E0 copies the group of four gUnknown_080D20C4
 * tiles that argument selects to 0x06010900 (its first argument is unused).
 * sub_0803FE50, every eighth tick, copies two 0x80-byte gUnknown_081245F8 tile
 * runs to OBJ tiles a1 + 0xEA and a1 + 0xF2. */
void sub_0803F8E0(int, int);
void sub_0803FE50(int, int);
/* BIOS LZ77 decompression to work RAM (source, destination), the counterpart
 * of LZ77UnCompVram. Defined in c_0808AAA0.c. */
void LZ77UnCompWram(const void *, void *);
/* Stores its argument in gUnknown_03003F68. */
void sub_08037B84(void *);

/* m4a sound-engine routines. sub_080700C0 and sub_080718E4 (an empty
 * function) are only used by address: sub_080707F4 stores them in SoundInfo.
 * sub_0806F734 is the high word of a 32 x 32 multiply (umul3232H32),
 * sub_0806FBD4 fills the table at its argument (the MPlay jump table),
 * sub_080708EC is SampleFreqSet, sub_08070668 MPlayImmInit and sub_080714FC
 * MPlayPanpotControl (the u16 is the track mask, the s8 the pan).
 * sub_08072B2C maps a 0..0xEF screen x onto -0x60..0x5F, clamped. */
void sub_080700C0(void);
void sub_080718E4(void);
int sub_08072B2C(int);
u32 sub_0806F734(u32, u32);
void sub_0806FBD4(void *);
void sub_080708EC(u32);
void sub_08070668(struct MusicPlayerInfo *);
void sub_080714FC(struct MusicPlayerInfo *, u16, s8);
/* m4a ClearModM: clears one track's modulation. */
void sub_08071564(struct MusicPlayerTrack *);
/* m4a MPlayModDepthSet and MPlayLFOSpeedSet: set `mod` or `lfoSpeed` on each
 * track in the u16 mask. */
void sub_08071584(struct MusicPlayerInfo *, u16, u8);
void sub_080715F8(struct MusicPlayerInfo *, u16, u8);
/* sub_08071C84 and sub_08071CA4 restore one 16-colour gPal bank from two
 * different saved palettes. sub_08073228 loads the sprite glyphs of a text
 * string into VRAM for the given proc.
 *
 * The 0x0806D block: sub_0806CFC8 and sub_0806D050 draw OAM objects 0x43 and
 * 0x44 at (x, y), averaged with last frame's position; sub_0806DC50 draws both
 * for one gUnknown_08580934 object. sub_0806DCB8 is a copy of
 * MatchSetupMoveRuleCursor. struct Unk0806DD34 is completed privately in
 * c_0806DCB8.c; the object is a struct Unk08580934_Obj seen through another
 * struct with the same offsets. */
void sub_08071C84(int);
void sub_08071CA4(int);
void sub_08073228(const void *, void *, u16, ProcPtr);
void sub_0806CFC8(int, int);
void sub_0806D050(int, int);
void sub_0806D0D8(struct Unk08580934_Obj *);
void sub_0806D3AC(struct Unk08580934_Obj *);
void sub_0806DC50(int);
void sub_0806DCB8(void);
struct Unk0806DD34;
void sub_0806DD34(struct Unk0806DD34 *);
void sub_0806DDF4(void);


/* Draws the rows of the gUnknown_084997C8 byte script, one sub_0803AAC0 call
 * per entry pair. */
void sub_0803AB3C(void);
/* Blits a clipped rectangle of tilemap entries into a BG tilemap buffer at
 * (x, y). The fifth argument is added to each entry: palette and tile base,
 * 0x8360 at every caller. */
void sub_08071948(u16 *, int, int, const void *, u16);
/* sub_0803D6FC copies five team colours from the record into
 * gUnknown_030040F8 (defined in c_0803D6B8.c). sub_080376DC is defined in
 * c_0803768C.c. */
struct Unk3D6FC;
void sub_0803D6FC(struct Unk3D6FC *);
void sub_080376DC(void *, int, int, int, int, int);
/* Steps value a by b, stopping at bound c (low) or d (high). With the fifth
 * argument set, stepping past a bound it already sits on wraps to the other
 * bound. */
int sub_0803D990(int, int, int, int, u8);

/* sub_0803CFA4 fills a buffer describing the current map: name, size, each
 * cell's tile and unit, and counts of some terrain kinds. sub_0803D6D0 copies
 * each army's team colour into gUnknown_030040F8 (in c_0803D6B8.c).
 * sub_08026040 deals the four army rosters out of gUnknown_02022684. */
void sub_0803CFA4(const void *, u8 *, u8);
void sub_0803D6D0(void);
void sub_08026040(int, int, int, int);
/* Painters into the gUnknown_03003340 map plane. sub_0801FAC4 paints a
 * widening triangle of value a5 from cell (a1, a2), a4 steps in direction a3
 * (0 down, 1 up, 2 left, 3 right). sub_0801FCE0 paints a three-wide vertical
 * bar of a3 from (a1, a2) to the bottom edge; sub_0801FD30 paints row a2 and
 * column a1 with a3. */
void sub_0801FAC4(u16, u16, u16, u16, u8);
void sub_0801FCE0(int, int, int);
void sub_0801FD30(int, int, int);
/* Screens of the 0x0803A block; sub_0803A338 and sub_0803A4A8 both start by
 * drawing the gUnknown_080D4228 frame into BG2 with sub_08071948. */
void sub_0803A338(void);
void sub_0803A4A8(void);
void sub_0803A5B8(void);
/* sub_0803AAC0 draws one row of OAM objects; its s8 fourth argument is
 * unused. sub_0803CEB8 loads saved map a1 from flash into gUnknown_02000000
 * and applies it; sub_0803CF54 packs the current map there and saves it as
 * map a1. sub_0801AD70 checks a save slot id: 1 = reject, 0 = accept. */
void sub_0803AAC0(u8, u8, u8, s8);
void sub_0803CEB8(u8, const void *);
void sub_0803CF54(u8, const void *, u8);
int sub_0801AD70(u8);
/* Tidies the flash save slots: keeps the newest complete save (writing a
 * fresh one when there is none) and frees every other slot. */
void sub_0801ADC8(void);
/* Loads save slot `a` from flash and validates it, retrying up to four
 * times. Returns 0 when valid, 4 when read but rejected, 1 when every read
 * failed. */
int sub_0801B018(u16);
/* FindNewestCompleteSave: returns the save-table slot holding the newest
 * complete copy tagged `id`, or 0xFFFF when there is none; id 0xFF asks for
 * untagged slots. The result is u16: callers test for 0xFFFF, not -1. */
u16 sub_0801B120(u16);
u16 FindNewestCompleteSave(u16);
void sub_08022AF8(u8, u8, u8, u8); /* corner sprites of box (x, y, w, h) */

/* The 0x0804B block. sub_0804B42C and sub_0804B4C4 score the four neighbours
 * of (x, y) on the map and return the terrain code of the best in-bounds one,
 * each with its own scoring table. sub_0804B55C's third argument is unused.
 * sub_0804B644 remaps a terrain or sprite id when the two ids agree. */
int sub_0804B42C(int, int);
int sub_0804B4C4(int, int);
u16 sub_0804B55C(u16, u8 *, int);
u16 sub_0804B644(u16, u16);
/* Its sixth and seventh arguments are unused. */
void sub_0804B850(u16, u16, void *, void *, void *, void *, void *);
/* sub_0804A1E4 redraws the entered text on BG0. sub_0804AE10 installs one
 * gUnknown_0200C528 list script. */
void sub_0804A1E4(u8);
void sub_0804AE10(void);
/* Text-entry grid helpers on gUnknown_030044E0: sub_0804A64C stores the
 * character under the cursor at the current position, sub_0804A68C clears
 * it. sub_0804A6D8 is defined in c_0804A6A4.c and returns a count. */
void sub_0804A64C(void);
void sub_0804A68C(void);
int sub_0804A6D8(void);
/* sub_0804BB28's third argument is unused. sub_0804BB74 decompresses picture
 * `a` into BG tilemap entries and copies them to the pointer; with the last
 * argument set it draws the picture mirrored. */
void sub_0804BA64(u16, u16);
void sub_0804BB28(int, void *, int);
void sub_0804BB44(int, void *, int);
void sub_0804BB74(int, void *, u32, int);
void sub_0804B8BC(u16, u16);
/* sub_0804D6C8 and sub_0804D6FC copy one 0x400-byte block with
 * sub_08011E54; their second argument is unused. sub_080505A4 takes a group
 * index and an entry index. */
void sub_0804D6C8(u16, int, u16);
void sub_0804D6FC(u16, int, u16);
void sub_080505A4(u16, u16);

/* sub_0803D3D8 is the step sub_0803CEB8 runs on the loaded map buffer.
 * sub_080248F8 returns the byte at gMap +0x4233 (in src/map.c). */
void sub_08037638(int, int, int, int);
void sub_0803D3D8(int, u8 *);
u8 sub_080248F8(void);

/* ---- The 0x08047000-0x08049000 blocks and their callees ---- */
/* Draws the unsigned number n as two-tile-high digits into a 32-wide tilemap,
 * right-aligned ending at (x, y). pal is ORed into each entry; tileGroup
 * picks the digit tiles, 20 tiles per group. */
void sub_08014B0C(int, int, u16 *, u32, u16, u16);
/* sub_08014B60 is sub_08014B0C's signed version with fixed digit tiles
 * (defined in c_08014B0C.c). sub_080199D0 stores its argument in
 * gUnknown_03002EE4. */
void sub_08014B60(int, int, u16 *, int, u16);
void sub_080199D0(u8);
/* sub_08047190 builds the current army's sorted unit list in
 * gUnknown_02028DD8, ended by 0xFF. sub_08047920 draws six rows of that
 * list. */
void sub_08047190(void *, int);
void sub_08047920(void *);
/* sub_0804769C counts the gUnknown_02028DD8 units that carry cargo id
 * `idx`. struct Unk0804769C is declared in unknown-globals.h because the
 * caller and callee are in different files. */
u16 sub_0804769C(struct Unk0804769C *, u16);
void sub_080484CC(struct Unk0804769C *);
/* struct Unk08047B98 is the same object as struct Unk0804769C, seen through
 * c_08047B98.c's private struct; the definitions complete it. sub_08047F70
 * moves the list cursor with the d-pad. */
struct Unk08047B98;
void sub_080482D8(struct Unk0804769C *);
void sub_08047F70(struct Unk08047B98 *);
void sub_08048158(struct Unk08047B98 *);
void sub_080488E0(void);
/* Steps the s16 gUnknown_084C30F8->unk832 up by 8 while it is negative,
 * then pins it at 0. Returns the new value. */
u16 sub_08048F10(void);
/* Stores the script and starts running it. */
void sub_080485DC(const u8 *);
/* Clears a panel and draws up to a1 + 3 rows of the gUnknown_02028E1C list
 * starting at entry a2, stopping at 0xFF. */
void sub_08048850(u16, u16);

/* sub_08048EC4 steps gUnknown_084C30F8->unk832 down by 8 until it reaches
 * -0x38, and returns the distance still to go (unk832 + 0x38). sub_080485F8
 * repaints a window of the BG0 tilemap and re-runs the stored script. */
u16 sub_08048EC4(void);
void sub_080485F8(void);

/* Moves the gUnknown_084C30F8 list cursor with the d-pad. */
void sub_08048F4C(void);

/* sub_08017704 is TrySpendBattleMapPoints. sub_0803C864 calls entry a's
 * handler in gUnknown_0849EDB0. sub_08048644 animates a palette and draws a
 * sprite at (x, y). sub_0804931C redraws the list row under the cursor. */
u32 sub_08017704(u32);
void sub_0803C864(u8);
void sub_08048644(u16, u16);
void sub_0804931C(void);

/* ---- The 0x08074000-0x0807C000 blocks and their callees ---- */

/* Returns &gMap->unk421a, a byte string that callers draw with sub_080149C0.
 * Defined in src/map.c. */
u8 *sub_080248E4(void);
/* Starts a gUnknown_086142B4 proc under the given parent. */
void sub_08074714(ProcPtr);

/* Proc handlers; each takes its own proc. sub_08076298 is the war-map
 * listener's idle handler (WorldMapNationPanel_SlideOutLoop). sub_0807B2F8 draws a
 * line of text and a number on BG0. sub_0807BCF0 runs every frame and calls
 * sub_0807BED8, which advances an affine BG animation. */
void sub_08076298(ProcPtr);
void sub_0807B2F8(ProcPtr);
void sub_0807BCF0(ProcPtr);
void sub_0807BED8(ProcPtr);
/* sub_0807B51C draws `value` as decimal digit sprites leftwards from (x, y);
 * idx picks the digit set. sub_0807B738 draws one sprite at the proc's row.
 * sub_0807B7BC loads a string's glyphs into sprite VRAM: arg 1 is the string,
 * arg 2 receives the total width, arg 3 optionally receives each glyph's
 * width, arg 5 is the caller's proc. It returns the glyph count. */
void sub_0807B51C(int, int, int, int);
void sub_0807B738(ProcPtr);
u16 sub_0807B7BC(u8 *, u16 *, u8 *, int, void *);
/* Starts a sub_08014740 text box on BG0 with the value sub_0807A3AC picks for
 * the current CO and campaign mission. */
void sub_0807A860(void);
/* Picks the value sub_0807A860 shows, from a CO (first argument) and a
 * campaign mission (second). */
int sub_0807A3AC(int, int);
/* Called by sub_0807BCF0 with its own proc once the frame counter passes a
 * threshold. */
void sub_0807BFB8(ProcPtr);

/* Called in long runs by the per-frame main-loop and VBlank callbacks
 * (sub_08036884 to sub_08036AB8). sub_08013510 polls the keys once per frame
 * (defined in c_0801348C.c); sub_08013B2C is FlushBgTilemaps. sub_0801F050 to
 * sub_0801F0FC each call one of two functions depending on gUnknown_03001FE0.
 * sub_08054B7C copies two fields of gUnknown_03002040 into globals. */
void sub_08013510(void);
void sub_08013B2C(void);
void sub_0801F050(void);
void sub_0801F06C(void);
void sub_0801F0AC(void);
void sub_0801F0C8(void);
void sub_0801F0E0(void);
void sub_0801F0FC(void);
void sub_08054B7C(void);

/* sub_08011AD8 runs every callback queued on the one-shot list
 * gUnknown_03002FA0, then clears the list; sub_08011B98 runs the callbacks on
 * gUnknown_03000000. sub_08019470 is the per-frame pump of the
 * gUnknown_0200C528 script list (in c_08019404.c). sub_0802FACC is the
 * per-frame link-cable update. sub_080345C8 is RunMapStateMachine, the per-frame tick
 * of the gUnknown_030032D8 state machine. */
void sub_08011AD8(void);
void sub_08011B98(void);
void sub_08015954(void);
void sub_08019470(void);
void sub_0802FACC(void);
void sub_080345C8(void);
/* sub_08023EEC calls sub_08023DCC when the horizontal scroll crosses into a
 * new 16-pixel column. sub_0803B404 is an empty function (in c_0803B3C8.c). */
void sub_08023EEC(void);
void sub_0803B404(void);
void sub_0803F990(void);

/* Per-frame callbacks that sub_0803662C registers with sub_08011B34, which
 * stores them in the gUnknown_03000000 list. sub_08022A6C (in c_08022A08.c)
 * cycles two gPal colours; sub_0803550C runs the current weather's effect;
 * sub_08043590 (in src/unit.c) flashes a palette colour while a CO power is
 * ready; sub_0803678C is UpdateFuelAmmoGraphics. */
void sub_08021DD8(void);
void sub_08022048(void);
void sub_08022A6C(void);
void sub_0803550C(void);
void sub_08043590(void);
void sub_0803678C(void);

/* sub_08013D00 is BG_GetMapTilePointer: returns a pointer to entry (x, y) of
 * the BG tilemap buffer its first argument selects. sub_080378A8 redraws the
 * minimap into the buffer it is given. sub_08037B90 clears the
 * gBG1TilemapBuffer tilemap and lays a 2x2-tile grid over it. sub_08037170
 * draws its third argument as right-aligned decimal digit sprites, at most
 * 999. */
u16 *sub_08013D00(int, int, int);
void sub_080377C4(void *);
void sub_080378A8(void *);
void sub_08037B90(void);
void sub_08037170(u16, u16, u16, u16);
void sub_080360D0(ProcPtr);

/* Main-loop and VBlank callbacks, passed to SetMainLoopCallback and
 * SetVBlankCallback; they must stay `void (void)` to match the callback type.
 * Defined in c_0803670C.c. */
void sub_08036944(void);
void sub_080369BC(void);

/* ---- Design room (map editor) helpers ----
 * sub_08002F1C is defined in src/design.c. */
void sub_08002F1C(void);
/* sub_0800C8D8 is RegisterArmyHqs; it returns how many entries it touched.
 * sub_0800C958 totals the sub_0800C8A0 counts of an id's variants, after
 * checking the id is valid. Both are in c_0800C874.c. */
int sub_0800C8D8(void);
int sub_0800C958(int);
/* sub_08004E38 copies a string (a forwarder to sub_0808B678). sub_080036A4,
 * sub_0800376C and sub_08003814 draw design-room OAM objects (in
 * src/design-editor.c). */
void sub_08004E38(char *, const char *);
void sub_080036A4(void);
void sub_0800376C(void);
void sub_08003814(void);

/* sub_080055B8, sub_08005634 and sub_080056B0 are one body for slots 0, 1 and
 * 2 (in c_080055B8.c); their three parameters are unused, and the only caller
 * passes 0, 0, 0. sub_0800C874 is CountProperties. sub_0800CAA0 returns the
 * index (0-15) of the palette set whose colours equal gPlaySt.armyColor[1..4],
 * or 0 when none does. */
void sub_080055B8(int, int, int);
void sub_08005634(int, int, int);
void sub_080056B0(int, int, int);
int sub_0800C874(void);
int sub_0800CAA0(void);
/* sub_0800CB30 is called as (0, 0) and then (1, <the first result>).
 * sub_0800C9E8 asks sub_0800C958 about the four armies' ids and returns
 * whether all four are present and more than one is non-empty (in
 * c_0800C874.c). */
int sub_0800CB30(int, int);
int sub_0800C9E8(void);
/* sub_080032EC draws a design-room sprite group (in src/design-sprites.c).
 * sub_0808B694 compares two strings like strcmp, bytes as unsigned. */
void sub_080032EC(int, int, int);
int sub_0808B694(const void *, const void *);

/* ---- The 0x08046000-0x08049000 blocks ---- */

/* sub_080468D4 converts a pixel offset to a tile column, draws one 8x8 cell
 * there and flushes. sub_080470E8 stops the gUnknown_084C2198 script (in
 * c_080470DC.c). */
void sub_080468D4(int);
void sub_080470E8(void);

/* Called with gUnknown_02028DD5 and gUnknown_02028DD6. sub_08046778 draws
 * every army slot that exists. sub_08046914 draws the info panel at x
 * position a for terrain entry b of gUnknown_085D583C. */
void sub_08046778(u8, u8);
void sub_08046914(u8, u8);

/* Draws the number a1 at (a2, a3) with sub_08014B0C when a4 is non-zero,
 * after clearing a 6 x 2 area to its left. */
void sub_08049944(u16, s16, s16, u8, u16);

/* ---- Callees of the 0x08020000-0x08022000 blocks ---- */
/* Clears the 2x2 tile block that (x, y) falls in, on the gBG1TilemapBuffer
 * tilemap. */
void sub_080223E0(u16, u16);
/* Paints value a4 into every cell of the gUnknown_03003340 plane within
 * Manhattan distance a3 of (a1, a2). */
void sub_0801F9C0(u16, u16, u16, u8);
/* The same diamond painter over s16 coordinates: writes value to every cell
 * within Manhattan distance r of (cx, cy). */
void sub_080200EC(s16, s16, s16, s16);
/* Another diamond painter: writes the fourth argument to every cell within
 * Manhattan distance r of (x, y). Defined in c_08020984.c. */
void sub_08020B88(s16, s16, s16, s16);
/* Adds delta to every cell of the given u8 plane within Manhattan distance r
 * of (x, y); r == 0 touches only the centre. The sixth parameter must stay
 * `int`: the definition only matches with it narrowed inside the body. */
void sub_08020EDC(s16, s16, s16, u8 *, int, int);
void StampVisionDisc(s16, s16, s16, u8 *, int, int);

/* Callees of the 0x08028000-0x0802E000 blocks. sub_08012E4C buckets the low
 * five bits of gGameClock into 0, 1 or 2. */
u32 sub_08012E4C(void);
void sub_080251BC(int, int, struct Unk802C57C *);    /* src/battle.c */
void sub_08037200(u16, u16, u16, u16);
u8 sub_0803EED4(int, int);
int sub_080249C8(int);                               /* src/map.c */
const u8 *sub_0802A880(int, int);
void sub_0802E250(void);
void sub_0802A7C4(void);
void sub_0802DBE4(void);                             /* c_0802DBD0.c */
void sub_0803A59C(void);
void sub_0803A8F0(struct Unit *);
void sub_080470F8(u16);
struct Unit *sub_08025580(void);
/* Debug printf at (x, y); the body is empty in this build. Must stay
 * varargs: the debug overlay in sub_080281F0 passes extra arguments. */
void sub_08013428(int, int, const char *, ...);
/* Scrolls a seven-row list so the cursor (unk20) stays visible between unk1e
 * and unk1e + 6, then redraws it. */
void sub_0802D9B8(struct Unk03001470 *);
/* Takes a map cell (x, y); it looks up the unit there with sub_0803DE94. */
u8 sub_0802E2D0(s16, s16);

/* ---- The 0x08007000-0x0800B000 map cell-update blocks ----
 * Every function here takes a map cell as (x, y). sub_0800AA30 counts land
 * cells in one half-plane around the cell (third argument: 0 west, 1 east,
 * 2 north, 4 south). sub_08009918 returns 0 or 1. */
void sub_08007F68(int, int, int);
int sub_0800AA30(int, int, int);
int sub_08009918(int, int);
void sub_0800A098(int, int);
void sub_0800A588(int, int);
int sub_0800A95C(int, int);
/* sub_08009538 returns 1 when the bridge-end tile at (x, y) has no occupied
 * neighbour on its far side. sub_0800A3D4 runs the cell update on each of
 * the four neighbours of (x, y). sub_0800BB2C is in c_0800BA9C.c. */
int sub_08009538(int, int);
void sub_0800A3D4(int, int);
void sub_0800BB2C(int, int);
/* sub_0800168C is IsTerrainNotWaterOrRiver. sub_0800BEB8 sets terrain 7 at
 * (x, y) when the cell is terrain 0x13 and sub_0800BCD0 says no.
 * sub_0800A6AC and sub_0800A884 return a table entry, negative when the cell
 * is rejected. */
int sub_0800168C(int, int);
void sub_0800BEB8(int, int);
void sub_0800A2EC(int, int);
int sub_0800A6AC(int, int);
int sub_0800A884(int, int);
void sub_0800AF24(int, int);

/* The 0x08000000 map-cursor block. These seven are arms of sub_0800057C's
 * switch on gActiveMap->mode. */
void sub_080005FC(void);
void sub_08000650(void);
void sub_08000664(void);
void sub_08000694(void);
void sub_0800081C(void);
void sub_08004CA0(void);
void sub_08005F4C(void);

/* sub_08003B8C is GenerateRandomMap (in src/design-editor.c). sub_08000CCC
 * is DesignRoomSelectItem. */
void sub_08003B8C(void);

void sub_08000BF8(void);
void sub_08000C68(void);
void sub_08000CCC(int);
void sub_08000DF8(int);

/* sub_08001D24 is in src/design.c. sub_080073F8 rebuilds the design-room item
 * ring for one side (0 = terrain list, 1 = unit list). */
int sub_08001D24(int);
void sub_080073F8(int, int);

/* sub_08002510's first parameter is unused; its second becomes
 * DrawOamObject's third argument. Both are in src/design.c. */
void sub_08002298(int, int);
void sub_08002510(int, int);

/* sub_08007B54 starts a proc and keeps its slot id. sub_08007B74 is the same
 * code as sub_08005F1C. */
void sub_08007B54(void);
void sub_08007B74(void);

/* Called by sub_08001DAC. All five are in src/design.c. */
void sub_0800272C(int, int, int, int, int, int, int);
void sub_08002844(int, int, int, int, int, int, int);
void sub_08002964(int, int, int, int, int, int);
void sub_080029F4(int, int, int, int, int, int);
void sub_08003088(int, int);

/* Returns non-zero on success and writes two values through the pointers.
 * Defined in c_0800C574.c. */
int sub_0800C6E8(int, int *, int *);


/* ---- The 0x08035000 block: predicates ----
 * sub_08035CF4 ignores its proc argument, but its caller passes it.
 * sub_08035C90, sub_08035CF4, sub_08035080 and sub_080129F8 return 0 or 1. */
u8 sub_08035C90(ProcPtr);
u8 sub_08035CF4(ProcPtr);
u16 sub_08035D0C(ProcPtr);
u8 sub_08035080(void);
u8 sub_080129F8(u16);

/* ---- The 0x08009000 and 0x0800F000 blocks: map-tile predicates ----
 * All take the map cell as (x, y). sub_08009310 returns whether the cell can
 * join an adjacent bridge or coast tile. sub_08009CF8 is in c_08009BF4.c.
 * sub_0800F318 and sub_0800F368 return 0 or 1; sub_0800F3B8 combines such
 * results into a bit mask. */
void sub_08009264(int, int);
int sub_08009310(int, int);
int sub_08009CF8(int, int);
int sub_0800F2E0(int, int);
int sub_0800F318(int, int);
int sub_0800F368(int, int);
int sub_0800F3B8(int, int);

/* ---- Map-tile helpers in the 0x0800C000-0x0801B000 blocks ----
 * sub_0800F564 classifies the tile one step from (x, y) in direction dir:
 * 0 = off the map or not a road, 1 = road, 2 = road continuing along dir,
 * 3 = road continuing on the other axis. */
void sub_0800C574(int, int, int);
int sub_0800F564(int, int, int);
/* sub_0800F77C returns 0, 2 or a count of 0 to 4 for the road at (x, y) in
 * direction dir. sub_08010604 is GetSeamType: the id a road or bridge cell's
 * neighbours are drawn with. sub_08010B34 picks the connector tile for a road
 * or bridge cell, -1 meaning "leave it"; sub_08010DD4 reports whether a
 * bridge cell is connected (both in c_08010B34.c). sub_0801659C runs one EASE
 * command of a gUnknown_03001470 script. sub_080179AC clears a 30 x 6 area of
 * the gUnknown_08499588 tilemap (in c_08017994.c); sub_08018194 fills a
 * 6 x 6 area of it. sub_0801B7C0 renders a string through the
 * gUnknown_0808EF64 glyph stream and returns -1 when no slot is free.
 * sub_0801BA1C writes n tilemap entries: tile, tile + 2, tile + 4, ... */
int sub_0800F77C(int, int, int);
int sub_0800F8D4(int, int);
int sub_0800FD44(int, int, int);
int sub_08010604(int, int);
int sub_08010B34(int, int);

/* Right-aligned decimal number into a tilemap; see src/decomp/c_08010EF8.c. */
void sub_08010EF8(u16 x, int unused, u16 value, u16 *dest);
void DrawNumberRightAligned(u16 x, int unused, u16 value, u16 *dest); /* sub_08010EF8 */
int sub_08010DD4(int, int);
bool8 sub_0801659C(u8);
void sub_080179AC(void);
void sub_08018194(u8);
int sub_0801B7C0(const char *, int);
void sub_0801BA1C(void *, u16, int);
/* sub_0801B738 calls the third entry point of the IWRAM overlay that
 * sub_0801B6BC copies (defined in c_0801B70C.c). sub_0801B964 advances a pair
 * of counters and returns 1 when the first wraps past 7. */
int sub_0801B738(u8, int, int, int);
int sub_0801B964(int);
/* Renders a NUL-terminated string one glyph at a time with sub_0801B738 and
 * returns its width in half-tiles. The second argument is a VRAM address
 * passed as an int, as sub_0801B738 takes it. Defined in c_0801B7C0.c. */
int sub_0801B8D0(const u8 *, int, int);
/* sub_0801BAB8 is an empty function: sub_0801BABC fills all 15 interrupt
 * handler slots of gUnknown_03002FE0 with it. IrqMain is the ARM interrupt
 * entry in crt0.s, which sub_0801BABC copies into IWRAM. */
void sub_0801BAB8(void);
void IrqMain(void);
/* sub_0804B0CC's twin (defined in c_0804B0CC.c). The first argument is a
 * string address passed as an int; callers cast. */
void sub_0804B10C(int, u8);
/* A gUnknown_0200C528 slot handler, used only by address: sub_08018254
 * installs it in a slot's unk08. It steps the slot's counter by 4 towards 0
 * and updates three display values from it. */
void sub_0801820C(struct Unk0200C528 *);

/* Pushes one (x, y) step onto the gUnknown_0849D5F8 move stack and records
 * the fuel left after it. */
void sub_08038848(s8, s8);

/* Looks up a value for side a in gUnknown_085D6A48, from side a's and the
 * other side's (a ^ 1) gUnknown_03004580 entries. */
u16 sub_08055F68(u16);

/* Defined in c_0805521C.c. Each returns the updated tile cursor, which
 * sub_08054C5C passes to the next one as its last argument. */
u16 sub_0805521C(u16, u16, u16, u16);
u16 sub_08055288(u16, u16, u16, u16, u16);
u16 sub_0805530C(u16, u16, u16, u16);
u16 sub_08055374(u16, u16);
/* Its first two arguments are unused. Returns the updated tile cursor, like
 * the four above. */
u16 sub_08055058(u16, u16, u16, u16);
/* a is the side and b the slot in gUnknown_02029A10[a].entries; both are
 * passed on to sub_08050F24. */
void sub_08054598(u16, u16);

/* Stores a and b in gUnknown_0849D5F8->unk1e and unk1f. */
void sub_080386DC(int, int);

/* Copies size bytes with CpuFastSet when size is a multiple of 32, otherwise
 * with CpuSet: the signed-count sibling of sub_08011C68. */
void sub_08012F6C(const void *src, void *dst, int size);


/* Draws the name string for b at row y, with sprite b + 0x3D on either
 * side. */
void sub_08034A7C(int, int);
/* sub_08026D44 returns TRUE or FALSE. sub_08026F04 is AddPlayerIncomeToFunds
 * and sub_08044178 ClearPlayerCoPowerStatus. sub_080268F4 only calls
 * sub_080267AC. sub_0802BFBC starts the gUnknown_0849A450 script (in
 * c_0802BF80.c). sub_08034C8C is an empty function. */
bool8 sub_08026D44(int);
void sub_08026F04(void);
void sub_080268F4(void);
void sub_08044178(int);
void sub_0802BFBC(void);
void sub_08034C8C(void);
/* Adds delta to each u16 of dst; size is in bytes. Defined in
 * c_08013098.c. */
void sub_080130C8(u16 *, int, int);

/* sub_080560A4 chooses up to a slots for side b's list gUnknown_0202980A[b]
 * and sorts it; only its first two arguments are used. sub_0805634C builds
 * side b's sort keys and sorts them, or does nothing when c is 0. */
void sub_080560A4(u16, u16, u16, u16, u16);
void sub_0805634C(u16, u16, u16);
/* Returns 0, 1 or its first argument. */
u16 sub_0805653C(u16, u16);
/* sub_08056638 sorts side `side`'s five keys, gUnknown_02029822[side], into
 * ascending order and moves the matching gUnknown_0202980A entries with
 * them. */
void sub_080564B8(u16, u16, u16);
void sub_08056638(u16);
void sub_0805601C(u16, u16, u16, u16);
void sub_08056D8C(u16, u16, u16);
/* Takes a six-halfword record its callers build on the stack; c_08056EEC.c
 * describes the layout. */
void sub_08056F8C(void *);
/* Builds that record for sub_08056F8C. Its second argument is unused. */
void sub_08056EEC(u16, u16, u16);
/* Sets window enable bits in gUnknown_030030A4. */
void sub_080573F0(void);

/* ---- The 0x08019000 block and its callees ---- */

/* Called by sub_080191B0 (the main menu). sub_0803CB8C is in c_0803CB74.c.
 * sub_080198C4 clears gUnknown_03001FF0 and sub_0801797C clears
 * gUnknown_03002B38. sub_08017A0C fills the gUnknown_0849958C tilemap with
 * entry 0x360 and sets two scroll values to 0xFF60. */
void sub_0803CB8C(void);
void sub_080198C4(void);
void sub_0801797C(void);
void sub_08017A0C(void);

/* Clears the gUnknown_03001FF0 callback, runs sub_080192EC on every
 * gUnknown_0200C528 slot except `a`, then advances slot a's list cursor.
 * Returns 0. */
int sub_08017F0C(s16);

/* Starts a script now, or queues it in the first free gUnknown_0200C508 slot
 * while any gUnknown_0200C528 slot is busy. Defined in c_0801930C.c. */
void sub_08019348(const u8 *);

/* A gUnknown_0200C528 list-script handler, installed as a table entry and
 * never called directly. Takes the slot index; returns TRUE. */
bool8 sub_0801906C(s16);


/* ---- The 0x0805A000-0x0805D000 AI adjacent-cell probes ----
 * The twin of the sub_08058BB4 family in c_08058BB4.c. sub_0805ACFC tests cell
 * (x, y) and records it through the u16 pair when it is accepted (in
 * c_0805ACA8.c). */
void sub_0805ACFC(int, int, u16 *);
/* sub_0805ACA8 offers the four cells next to (x, y) to sub_0805ACFC and
 * returns 1 when one was accepted. sub_0805C128 is the 0x0805C block's cell
 * tester. sub_0805A854 takes a cell as a u16 pair, updates it in place and
 * returns 0 or 1. sub_0805A5E0 writes a result word through its pointer (in
 * c_0805A514.c). */
u8 sub_0805ACA8(int, int, u16 *);
void sub_0805C128(int, int, u16 *);
int sub_0805A854(u16 *);
void sub_0805A5E0(int *);
/* sub_0805A268, sub_0805A388 and sub_0805A514 are deliberately not declared
 * here: their definitions take a file-local struct (struct Unk5A514Cell, the
 * same object as struct Unk03003338), and any other pointer type would
 * disagree with them. Callers repeat the struct and cast.
 *
 * sub_0805E2AC sends the active unit towards the nearest AI property-list
 * entry of kind 6, or falls back to sub_0805F7B8 when there is none. */
void sub_0805E2AC(void);
/* Issues the AI's action for cell (a1, a2). It does not return: it ends with
 * a longjmp through sub_08071910. */
void sub_0805D648(s16, s16, u8, u8, u8);
/* Checks the current unit's move cost for the terrain at (x, y), then offers
 * the four neighbours to sub_08058E88, which writes the first it accepts.
 * Returns 0 on success, -1 otherwise. Defined in c_08058BB4.c. */
int sub_08058DEC(int, int, u16 *);
/* Copies the AI command record gUnknown_030046C0 into gUnknown_030044B0. */
void sub_0805D5EC(void);
/* sub_08071910 is the linker's THUMB-to-ARM veneer for sub_08000554, a
 * longjmp. It must stay noreturn: sub_0805D648 ends with a call to it and has
 * no epilogue. Call the veneer, not sub_08000554, or the linker adds a second
 * veneer. sub_08058BB4 probes the four cells next to unit `id` and returns the
 * best, or -1. */
void sub_08071910(u8 *, int) __attribute__((noreturn));
int sub_08058BB4(u16, u16 *);
/* sub_08058BB4's twin, taking the cell as (x, y). Defined in
 * c_08058BB4.c. */
int sub_08058C54(int, int, u16 *);
/* AI: scores the battle just simulated between gBattleAttacker and
 * gBattleDefender and writes the score through the pointer. Returns 0, or -1
 * when the attacker's losses reach the limit in gUnknown_03004784. */
int sub_08058A2C(int *);
/* Walks the 8-byte gUnknown_03003338 records and returns the one with the
 * highest unk02, preferring records with bit 15 of unk00 clear. */
struct Unk03003338 *sub_0805878C(void);
/* Returns the number of records it wrote to gUnknown_03003338. It does take
 * an argument: its caller sub_0805E718 passes sub_08058744's result straight
 * through. */
int sub_080587FC(int);

/* ---- The 0x0805D000-0x08062000 AI blocks ----
 * sub_080606D0, sub_08061868, sub_08061AC4, sub_08061B00 and sub_0805D438 are
 * arms of sub_0806171C's switch. sub_080606D0 is the AI turn's outer driver,
 * sub_08061868 is AiBeginTurn, and sub_0805D438 runs one step of the current
 * army's unit list. sub_08061178 is PickWeightedAiUnit. sub_080611D8 picks a
 * map cell for a unit of type gUnknown_030046C0.unk06, writes its x and y
 * through the pointer, and returns 1, or 0 when it found none. */
void sub_080606D0(void);
void sub_08061868(void);
void sub_08061AC4(void);
void sub_08061B00(void);
void sub_0805D438(void);
u8 sub_08061178(u8);
u8 sub_080611D8(void *);
/* sub_08061668 picks the cheapest unused entry of the 0xFF-terminated
 * gUnknown_085766E4 cell list that suits unit type gUnknown_030046C0.unk06,
 * writes its x and y through `out`, marks it used, and returns whether it
 * found one. sub_08061308 chooses a target cell for the current unit type
 * with search mode a2; it returns 1 when it found one. */
int sub_08061668(u16 *);
u8 sub_08061308(u8, u8, u16 *);
void sub_080610D0(void);
void sub_0806056C(u8);
void sub_0805E440(void);
void sub_0805F4F8(void);
void sub_0805FB70(void);
/* sub_0805FC1C finds a transport in the current army that the active unit
 * can board and writes it through the pointer. sub_0805C988 walks the record
 * list at 0x02028360; callers use its result as a byte. */
void sub_0805FC1C(int, void *);
void FindTransportForSelectedUnit(int, void *); /* sub_0805FC1C; see src/decomp/c_0805FC1C.c. */
int sub_0805C988(int, int);
/* sub_0805A8C0 is deliberately not declared: its definition takes u16
 * parameters, while its caller sub_08059674 passes ints with no prototype in
 * scope, and no prototype produces both. c_08059674.c declares it K&R-style.
 *
 * sub_08059464 (in c_080591E4.c) scans every cell for the lowest-threat
 * reachable one and issues the move with sub_0805D648. */
void sub_08059C60(void *);
void sub_08059464(void *);
/* sub_08058058 appends records to the gUnknown_03003F20 list from index
 * `start` and returns the index after the last one, or 0 when its guard
 * fails. sub_08057F00 is CountUnitsWithTypeTag. sub_08058144 is declared
 * `struct Unit *` to agree with its definition, but it returns a
 * gUnknown_084995A0 record, a struct PropertyListEntry. sub_080591E4 finds
 * the lowest-threat reachable cell and issues the move with sub_0805D648.
 * sub_080591E4 and sub_0805C0AC take the address of a (u16, u16) cell
 * record. */
int sub_08058058(int);
int sub_08057F00(int);
struct Unit *sub_08058144(int, int);
void sub_080591E4(void *);
int sub_08059A0C(void *);
void sub_0805BFDC(int, int, int, int);
void sub_0805C0AC(void *);
/* AI: a1 (0 or 1) selects a unit filter. Over the current army's unused
 * units of type 1 or 2, finds the reachable empty cell with the lowest danger
 * value and writes it through the pointer. */
void sub_0805A9AC(int, void *);
/* sub_0805A6DC writes 4-byte {u8 x, u8 y, s16 v} cell records through `out`
 * and returns how many it wrote. sub_08059B4C's last argument is a (u16, u16)
 * cell record. */
int sub_0805A6DC(u8 *);
void sub_08059B4C(int, int, int, void *, void *);
void sub_0805EB58(void);
void sub_0805F914(void);
/* sub_08059674 returns non-zero when cell (x, y) is usable by the active
 * unit: empty or holding that unit, and with accepted terrain. Callers carry
 * no prototype for sub_0805A8C0; see above. */
u8 sub_08059674(s16, s16);
void sub_0805F6D4(void);
/* AI movement strategies, in c_0805ECDC.c: sub_0805ED70 is
 * AiMoveWithFrontLine and sub_0805F074 AiMoveUpConservatively; sub_0805EE40
 * and sub_0805EF00 are two more. sub_08059E3C collects every cell the current
 * army may act on into the gUnknown_03003F20 list, ended by v = 0xFFFF.
 * sub_08059F24 and sub_0805A008 (both in c_08059F24.c) take the same list. */
void sub_08059AEC(void);
void sub_08059E3C(void *);
void sub_08059F24(void *);
void sub_0805A008(void *);
void sub_0805ED70(void);
void sub_0805EE40(void);
void sub_0805EF00(void);
void sub_0805F074(void);
/* sub_0805ECDC is AiChargeAggressively. sub_080590DC is sub_080591E4's twin,
 * used for units whose gUnknown_085D5ABC entry has unk1a == 0x20.
 * sub_08058F90 (in c_08058BB4.c) runs sub_08059050 for each army in the
 * current army's four-bit mask and returns -1 when none found a cell. */
void sub_0805ECDC(void);
void sub_080590DC(void *);
int sub_08058F90(void *);
/* sub_0805E778 and sub_0805E87C are called through sub_0805E9DC's two-entry
 * function-pointer table. sub_0805EA54 is defined in c_0805E9DC.c. */
void sub_0805E778(void);
void sub_0805E87C(void);
void sub_0805EA54(void);
/* Returns the live record with the lowest v in the 4-byte {u8 x, u8 y, s16 v}
 * array at *gUnknown_03003F20 and marks it used (v = 0x7FFF), or NULL when
 * none is live. These are not the 8-byte struct Unk03003338 records that
 * gUnknown_03003F20 is declared with. */
void *sub_08057EC0(void);
/* Returns 0 or 1; sub_0805BB8C adds four results together. Defined in
 * c_0805B980.c. */
int sub_0805BBF8(int, int);
/* Writes a cell's x and y as two halfwords through the fourth argument;
 * callers set the first to 9999 beforehand to detect "none". The third
 * argument is a unit type. Defined in c_0805B980.c. */
void sub_0805BAFC(int, int, int, u16 *);
/* AI census counters. sub_08058318 is the same code as sub_08058254 (in
 * c_08058254.c). sub_0805848C counts the units of the armies not masked out
 * whose unk00 is 1 or 2 (in c_080583DC.c). sub_080585D4 sums over the
 * passable cells the AI accepts: 0x1E for terrain 8, 1 for any other. */
int sub_08058318(void);
int sub_0805848C(void);
int sub_080585D4(void);
/* sub_08058254 counts the deployed units of every army not masked out by
 * gPlayers[gUnknown_030033EC].unk2c; sub_080583DC counts those of the armies
 * that are masked out. */
int sub_080583DC(void);
int sub_08058254(void);
/* Counts the live class-2 units of the current army whose unk09 low field is
 * 1 and that stand on a passable cell. */
int sub_080586CC(void);
/* Writes a per-unit value to *out: 0x78 by default, clamped to 0x78. Defined
 * in c_08058BB4.c. */
void sub_08058F30(u8 *);

/* sub_08061CDC clears the three unk03 bytes of all 92 gUnknown_084995A0
 * records. sub_08062028 and sub_08062C7C each call two functions in turn. */
void sub_08061CDC(void);
void sub_08062028(void);
void sub_08062C7C(u8);

/* Copies one 0x130-byte struct Unk085771C4 record (*dst = *src). */
void sub_08061A40(struct Unk085771C4 *, const struct Unk085771C4 *);
/* Builds a struct Unk085771C4 record in dst: the header comes from
 * gUnknown_085771C4[a2], and each byte of the 24 rows is that record's byte
 * plus the one in the record gUnknown_0857690C[a3][gPlayers[a4].unk1d]
 * selects. */
void sub_08061928(struct Unk085771C4 *, u8, u8, u16);

/* sub_080607E8 spawns up to three units in a row, their types taken from
 * gFactoryUnitSchedule. sub_08061E98 sweeps every map cell and builds two byte
 * masks in gUnknown_030045C0 and gUnknown_030046B8. sub_0806279C clears the
 * 10 x 12 grid of 0x2C-byte records at gUnknown_0202DAD8. */
void sub_080607E8(void);
void sub_08061E98(void);
void sub_0806279C(void);

/* The three handlers in sub_08061E98's table, picked by the low three bits
 * of a unit's unk09; each takes one gUnknown_08499594 unit record. The struct
 * tags stay incomplete here: each definition reads the record through its own
 * bitfield view, because struct Unit declares +0x04 and +0x09 as plain
 * bytes. */
struct Unk8061DCC;
struct Unk61E54;
struct Unk61E80;
void sub_08061DCC(struct Unk8061DCC *);
void sub_08061E54(struct Unk61E54 *);
void sub_08061E80(struct Unk61E80 *);

/* Tests cell (a2, a3) for army a1, using the owner bits of its terrain
 * byte. */
bool8 sub_0802700C(int, int, int);

/* sub_08061788 and sub_08061868 (AiBeginTurn) are in c_0806171C.c.
 * sub_08061B00 steps a shared cursor through two ROM tables of functions and
 * calls the entries. sub_08061CF8 sums sub_08061DA8(n) over the set low four
 * bits of a flag byte. sub_08061868 and sub_08061B00 are declared with the
 * other AI blocks above. */
void sub_08061788(u16);
void sub_08061CF8(void);

/* The gUnknown_03001470 wake pass. sub_080159E0 runs one slot's pending wake:
 * clears flag bit 0, calls the slot's callback and steps its script.
 * sub_08015A9C runs every pending slot in order of unk14 & 0x7F, and
 * sub_08015B94 reports whether any slot is still pending (both in
 * c_08015A9C.c). sub_08015994 is sub_08015954's twin that skips slots with
 * unk14 bit 7 set (in c_08015954.c). */
void sub_080159E0(u8);
void sub_08015994(void);
void sub_08015A9C(void);
bool8 sub_08015B94(void);
/* A gUnknown_0848A160 script opcode handler: takes the slot index and
 * returns whether the script keeps running. */
bool8 sub_08015D24(u8);

/* ---- The 0x08043000 CO-list helpers and 0x08087000 callees ---- */
/* Builds the unlocked-CO list in gUnknown_020288A0 and returns how many it
 * wrote, not counting the 0xFF terminator. */
u8 sub_08043CA0(void);
/* Returns gUnknown_020288A0, the unlocked-CO list. */
u8 *sub_08043C98(void);
/* The third argument is a palette slot for sub_08043AA0 when it is below 14,
 * and otherwise a CpuFastSet destination address. The second is a u16 tile
 * buffer. */
void sub_08043E8C(int, u16 *, int);

/* Draws rank a (a GetRankFromScore result, raised to at least 2) into the BG0
 * tilemap at row b * 2 + 5 + c. */
void sub_08087514(u32, int, int);

/* Copies CO a's mini portrait (0x180 bytes) to b and loads its palette into
 * slot c with sub_08043AA0. */
void sub_08043FA8(int, void *, int);

/* ---- The 0x0800C000 block: properties and map cells ---- */

/* GetPropertyKindForTerrain: sorts the low five bits of a terrain byte into
 * 0, 1 or 2. Defined in c_0800C75C.c. */
int sub_0800C7E8(int);

/* Updates cell (x, y); the third argument is a 0/1 flag. Only sub_0800C22C
 * calls it. */
void sub_0800C2D0(int, int, int);

/* RepaintTileRight: re-tiles the cell to the right of (x, y), when there is
 * one. */
void sub_0800CEF8(int, int);

/* The two halves of one pair: both map terrain id 0x28, 0x48, 0x68 or 0x88
 * (one per army) to slot 0-3. sub_0800C75C stores its second and third
 * arguments in gActiveMap's unk17 and unk1b at that slot; sub_0800C7A4 clears
 * them. Both are in c_0800C75C.c. */
void sub_0800C7A4(int);
void sub_0800C75C(int, int, int);

/* Re-tiles (x, y) twice: with the given tile, then with the tile
 * sub_080016D0 picks for the cell. */
void sub_0800EBFC(int, int, int);

/* Clamps x into 0..0xF0 and stores it in entry y of the halfword table at
 * row; rows past 0x9F are ignored. */
void sub_080736F4(int x, u32 y, u16 *row);

/* sub_080736F4's twin for a table with two halfwords per row; c selects the
 * halfword. */
void sub_08073974(int x, u32 y, int c, u16 *base);

/* Starts a gUnknown_08582B14 proc under `parent`, stores the six values in
 * it, and returns it. */
ProcPtr sub_0806E4BC(int a1, int a2, int a3, u16 a4, int a5, int a6, ProcPtr parent);

/* sub_08065200 returns the x position of entry `a` when
 * gUnknown_08580934->unk08 entries, 0x32 pixels wide, are spaced evenly
 * across the screen. */
int sub_08065200(int);
void sub_08065818(void);

/* Sets up a struct Unk08580934_Obj: x position, index and one more value. */
void sub_08064D44(struct Unk08580934_Obj *, int, int, int);

/* Fills the header of a struct Unk08580934 (offsets 0x00-0x20); both callers
 * pass gUnknown_08580934. */
void sub_0803BFBC(void *);

/* ---- The 0x0805B000 AI cursor-target block ----
 * sub_0808B6C4 is memset (dst, fill byte, length). sub_0805B4D8 returns 0 or
 * 1 and writes two int results through its pointers. */
void *sub_0808B6C4(void *, int, int);
int sub_0805B4A8(void);
u8 sub_0805B4D8(int, int *, int *);
int sub_0805BD40(int, int, int, int, s16 *);
/* Each takes a cell (x, y) and writes a cell's x and y as two halfwords
 * through the third argument. sub_0805BC7C returns 0 or 1. */
u8 sub_0805BC7C(int, int, u16 *);
void sub_0805BDE4(int, int, u16 *);
void sub_0805BEA0(int, int, u16 *);
void sub_0805BF3C(int, int, u16 *);

/* ---- The 0x0805B744 AI-turn driver block ---- */
/* sub_08059C00 picks a cell from the gUnknown_03003F20 list, writes it to
 * the (u16, u16) record (callers preset the first halfword to 9999), and
 * returns the winner's s16 value. sub_0805F7B8 is the AI's fallback when no
 * order was given: it scores every cell, issues the best as a sub_0805D648
 * move, and leaves through sub_08071910's longjmp. */
s16 sub_08059C00(void *, u16 *);
void sub_0805F7B8(void);
/* Fills a whole map plane with one byte value. The plane is a pointer into
 * gUnknown_08499590, such as +0x3C72. */
void sub_080581A4(u8 *, int);
/* sub_0805B3F4 is the AI turn-start entry; it dispatches through
 * gUnknown_08576890. sub_0805B5BC and sub_0805B6A0 are sub_0805B4D8's twins
 * over gUnknown_02029ED8: they take the two cursor indices by pointer and
 * advance them, and sub_0805B6A0 skips the terrain test. Both return 0 or 1
 * and write two results through the last two pointers. sub_0805B5BC is in
 * c_0805B4D8.c. */
void sub_0805B3F4(void);
int sub_0805B5BC(int *, int *, int *, int *);
int sub_0805B6A0(int *, int *, int *, int *);
/* sub_0805AF90 is the map-redraw loop driven by sub_0805B5BC (in
 * c_0805AE88.c). sub_0805B744 passes one (u16, u16) cell record to
 * sub_0805B8F4 and then to sub_0805B814 (both in c_0805B814.c); callers test
 * sub_0805B8F4's result against 1. */
void sub_0805AF90(void);
void sub_0805B980(void);
void sub_0805B744(void);
void sub_0805B778(void);
void sub_0805B814(u16 *);
u8 sub_0805B8F4(u16 *);
/* Both return 0 or 1. Defined in c_0805B980.c. */
u8 sub_0805BA34(int, int, u16 *);
u8 sub_0805BB8C(int, int);

/* Fills two 30-tile rows of a tilemap with consecutive tile numbers,
 * starting at tile 0x314 with palette 3. */
void sub_08032484(u16 *);
/* sub_08034290 ends the gUnknown_0849BB80, ProcScr_PutFace and
 * gUnknown_0849BB68 procs. sub_08032AFC sets up window 0 with a fixed
 * rectangle. */
void sub_08034290(void);
void sub_080328EC(void);
void sub_08032AFC(void);
/* Copies a 16-byte name from src to dst and terminates it. sub_08032BCC is
 * not declared here: it is a proc callback whose parameter is a struct
 * private to its own file. */
void sub_08031B6C(u8 *, u8 *);
/* Link-cable helpers. sub_0802F408 is the link watchdog: it counts stalled
 * frames and returns TRUE while the count is under 11. sub_0802F4A0 returns
 * TRUE when as many unk0a slots hold 5 as the unk09 mask has bits set.
 * sub_0802F504 counts how many of the four sub_0802F480 bits are set. */
bool8 sub_0802F408(void);
bool8 sub_0802F4A0(void);
s8 sub_0802F504(void);
void sub_08030D84(void);

/* sub_0803840C returns the length of the live run of gUnknown_0200C420.unk38
 * entries (in c_08038368.c). sub_0805CA24 returns 0 when the active unit has
 * no weapon or has run out of ammo. */
int sub_0803840C(void);
int sub_0805CA24(void);
/* AI: fills the list with candidate cells for the active unit: reachable
 * cells of the current army whose terrain suits the unit's type, skipping
 * cells that hold another unit. */
void sub_0805A0EC(void *);

/* sub_0808488C returns a 16-colour palette for index i: gUnknown_0812598C
 * when sub_08084858(i) is set, otherwise row i + 6 of gUnknown_0823DC38 (in
 * c_08084864.c). sub_0807F8E4 returns whether a gUnknown_08616740 proc
 * exists. sub_08087B20 draws n right to left as digit sprites; base is the
 * sprite id of digit 0. */
u16 *sub_0808488C(int);
int sub_0807F8E4(void);
void sub_08087B20(int, int, int, int);

/* sub_08013034 trims trailing full-width spaces (0x81 0x40) from a two-byte
 * string. sub_0804A6A4 clears every entry of gUnknown_030044E0->unk2c.
 * sub_080741C4 loads a screen's graphics, like part of sub_0806EB5C. */
void sub_08013034(u8 *);
void sub_0804A6A4(void);
void sub_080741C4(int, int, int);

/* Screen-setup callees. sub_08078D80 is in c_08078D40.c and sub_08087938 in
 * c_080878A8.c. sub_08037750 loads palette bank a from gUnknown_081253F0 and
 * registers sub_08037790 as a per-frame callback. sub_08084804 runs the six
 * checks in gUnknown_08616B00 and records each result in gUnknown_03000650.
 * sub_080845C4 and sub_080845E8 (in c_08084580.c) do not read their second
 * argument, but their caller passes it. */
void sub_08078D80(ProcPtr);
void sub_08037750(int);
void sub_08084804(void);
void sub_08087938(void);
void sub_080845C4(int, int);
void sub_080845E8(int, int);
void sub_08086F3C(int);

/* sub_080085E0 is MakeTile: it runs one of nineteen handlers on the cell
 * under the cursor, chosen by gActiveMap->selectedTerrain, and leaves a
 * result code in gActiveMap->unk6a. sub_0800AEAC takes a cell (x, y). */
void sub_080085E0(void);
int sub_0800AEAC(int, int);

/* sub_08004D10 is in src/design-menus.c. sub_0800BC5C reports whether the
 * terrain at (x, y) is one of the two bridge codes. */
int sub_0800105C(void);
void sub_08004D10(void);
int sub_0800BC5C(int, int);
void CompactMapArmies(void); /* sub_0803D558; see src/decomp/c_0803D558.c. */

#endif // UNKNOWN_FUNCS_H
