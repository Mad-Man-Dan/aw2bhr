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
void sub_0801DCD4(int, int);
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
int sub_0801E9B0(s16, s16, s16, void *, long long, s16);
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

/* CORRECTION, wave 28 (W28-A): src/decomp/c_08018BAC.c declared this `bool8`.
 * The body cannot tell -- it is `movs r0, #1` at every width -- and its three
 * callers say otherwise. sub_08018BCC, sub_08018F34 and sub_08018F74 each end
 * `bl sub_08018BAC; lsls r0, #0x10; asrs r0, #0x10`. A `bool8` (QImode) result
 * converted to anything is `lsls #0x18; lsrs #0x18`, and a `s16` result in an
 * `int` context needs no conversion at all; only an INT result converted to the
 * caller's own `s16` return type produces the pair the ROM has. The definition
 * was retyped to `int` and re-verified with trymatch (still byte-identical). */
int sub_08018BAC(s16);

/* The 0x08018000 block's own helpers, none of which had a prototype.
 *
 * sub_08018018 takes `u8`: both callers (sub_080180A8, sub_080180CC) hand it
 * the s16 slot member unk0e through `lsls #0x18; lsrs #0x18`, which is the
 * narrowing a u8 parameter forces and an `int` parameter would not.
 * sub_0801815C likewise -- sub_080180CC reaches gUnknown_03002F08.unk02, a u16,
 * with `ldrb`, which is only a u8-context read.
 *
 * sub_08018254 takes `s16`: sub_08018464 holds its own s16 parameter in r4 and
 * re-derives `lsls #0x10; asrs #0x10` immediately before the `bl`.
 *
 * sub_08014824 returns at least `int`: sub_080185BC truth-tests the result with
 * a BARE `cmp r0, #0` and no narrowing, which rules out every sub-word return.
 *
 * sub_0801A548 mirrors sub_0801A57C above -- same caller shape, same u16 global
 * (gUnknown_030033EC) passed with a plain `ldrh`. All four returns are unused
 * at every known call site, so `void` is a floor, not a measurement. */
void sub_08018018(u8);
void sub_0801815C(u8);
void sub_08018254(s16);
int sub_08014824(void);
void sub_0801A548(u16);

/* ---- wave 28 (W28-A extension): the 0x08035000 block ---- */

/* Copied from the promoted definitions in src/decomp, not re-derived. */
ProcPtr sub_080355CC(u16, u16, u16, u16); /* c_080355CC.c */
int sub_08042DE0(int);                    /* c_08042DE0.c */
/* Wave 34, W34-F. sub_08041DF8 does `bl sub_080413A4; ldrb r2,[r0]` and passes
 * that byte to sub_08034534's u8 third parameter, so the return is a pointer
 * whose first byte is a u8 -- `u8 *` is the weakest model that fits. Its one
 * argument is gUnknown_03003F40, declared `int` above. */
/* Wave 34 integration: returns struct Unk03003338 *, per the promoted
 * definition in src/decomp/c_080413A4.c. The `ldrb r2,[r0]` at sub_08041DF8's
 * call site fits both this and a bare u8 *, so weakest-model does not apply --
 * a promoted definition already names the type. Callers cast; a pointer cast
 * emits nothing. */
struct Unk03003338 *sub_080413A4(int);
/* Wave 34, W34-F. sub_080411FC hands it two bytes off its own proc (+0x44 and
 * +0x4a), both plain `ldrb` with no narrowing at the call site, so `int` is the
 * weakest model. void: the result is never read. */
void sub_08041258(int, int);
/* Wave 34, W34-F. sub_0804103C passes the packed u8 selector and uses the
 * result as `(r - 1) * 0x400`, a byte offset into gUnknown_081218BC, so int. */
int sub_08024984(int);
/* Wave 34, W34-F. A map-cell predicate: (x, y) are compared against the
 * gUnknown_08499590 header's u16 extents SIGNED and against zero, so s16, and
 * the third argument selects the gUnknown_085D5ABC record whose unk19 picks the
 * terrain-cost row. Returns u8 -- its only caller sub_08041F38 re-narrows the
 * result `lsls #0x18; lsrs #0x18` before comparing it to 1. */
u8 sub_08041EA8(s16, s16, int);
/* Wave 34 (W34-F matched the body, W34-I declared it). Probes the four cells
 * around (x, y) with sub_08041EA8 and returns a 4-bit direction mask, so u8 --
 * every caller in block 0x08042 re-narrows the result `lsls #0x18` before
 * testing it. The first two arguments are WIDE: sub_08041F38 does `x - 1` and
 * `x + 1` on them before handing them to sub_08041EA8's s16 parameters, and no
 * re-narrowing appears between the arithmetic and the call. */
u8 sub_08041F38(int, int, int);
/* Wave 34, W34-F. The tail sub_08041820 falls into when sub_0803DF54 finds no
 * entry; same proc start, but it clears the cell plane itself and stores the
 * two coordinates into the new proc. Third argument is used as a u8. */
void sub_0804189C(int, int, int);
/* The tag is FORWARD-DECLARED and left incomplete on purpose, the sub_08071CF4
 * precedent: src/decomp/c_08035828.c completes it privately, and sub_08035810 --
 * its first cross-file caller -- only forwards a `Proc_Find` result. */
struct Unk35828Proc;
void sub_08035828(struct Unk35828Proc *);

/* The block's own helpers.
 *
 * sub_08035124 takes `u8`: sub_080351F0 reaches gPlaySt.unk2e with a
 * plain `ldrb` and the callee's prologue is `lsls #0x18; lsrs #0x18` operating
 * on r0 IN PLACE before any global is touched, which is PROMOTE_MODE and not a
 * cast at a use.
 *
 * sub_0803F5E4's parameters arrive as a bare `adds r0, r4, #0` and a
 * `movs r1, #0x48` with no narrowing between them, so `int` is the weakest
 * reading of both -- and the first is handed the already-narrowed `u16`
 * parameter of sub_08035020, which converts silently either way.
 *
 * sub_08035760's second parameter is what settles its arity at two:
 * sub_080357E0 loads it from [sp, #8], its own FIFTH argument, immediately
 * before the `bl`. sub_08035740 forwards its own r0 there the same way.
 *
 * sub_08071488's third parameter is `s16`: both sub_08035E24 call sites end
 * `rsbs r2, r2, #0; lsls r2, #0x10; asrs r2, #0x10`, and sub_08035E6C passes a
 * bare 0, which settles nothing. The three void returns are floors -- every
 * call site discards the result. */
void sub_080350E4(void);
void sub_08035124(u8);
void sub_080352B4(void);
void sub_080353E8(void);
void sub_0803F5E4(int, int);
/* Wave 44, W44-E: copied verbatim from the promoted definition in
 * src/decomp/c_0803F5C8.c, which had no prototype anywhere; the definition
 * wins. sub_0805C988 and sub_0805C9CC both walk the returned record with
 * `adds r2, #8` and read unk00/unk01/unk02, which is struct Unk02028360's
 * stride and layout -- independent corroboration of the return type. */
struct Unk02028360 *sub_0803F5C8(int);
/* Wave 49, W49-C, both read off sub_0803FC28's call sites (neither is promoted
 * yet). sub_0803F908 takes five whole words, the fifth on the stack, and the
 * third is one of seven ROM blobs in the 0x0849FAxx run -- see the note on those
 * in include/unknown-globals.h for why `const u8 *` is the weakest model that
 * fits. sub_08027198's result reaches sub_0803F908's fourth argument through a
 * bare `adds r3, r0, #0` with nothing re-narrowing it, and the same argument is
 * a plain -1 at the case-2 site, so the parameter and the return are both
 * `int`. */
/* Wave 77, W77-E: FIFTH PARAMETER CORRECTED int -> u8, on the function's own
 * prologue. sub_0803F908 loads its stack argument and immediately zero-extends
 * a BYTE out of it:
 *     ldr  r0, [sp, #0x18]
 *     lsls r0, r0, #0x18
 *     lsrs r1, r0, #0x18
 * That pair IS the sub-word parameter's prologue conversion (PROMOTE_MODE
 * zero-extends every sub-word parameter, so u8 and s8 are identical here); an
 * `int` parameter tested `!= 0` emits the `ldr` and nothing else. Wave 49 read
 * "five whole words, the fifth on the stack" and inferred `int`, but the ABI
 * slot is a word for every sub-word type, so that observation does not
 * discriminate -- and the only caller, now promoted in src/decomp/c_0803FC28.c,
 * passes a literal 0 at all five sites, which is byte-neutral between the two.
 * The own-body conversion is the only hard fact and it says u8.
 *
 * THIRD PARAMETER LEFT AS `const u8 *` -- but read this before trusting it.
 * The parameter is a pure pass-through (`mov ip, r2` and back out to
 * PutSprite), so it is byte-neutral here, and the promoted caller plus
 * include/unknown-globals.h have already settled the 0x0849FAxx blobs as
 * `const u8 []`; agreeing costs nothing and reshaping them unilaterally is
 * exactly what the house rule forbids. HOWEVER the value's discriminating use
 * is PutSprite's fourth parameter, which is `u16 *`, and the blob lengths fit
 * u16 sprite-object data exactly (0x1a = a count word plus four attr triplets,
 * 0x08 = count plus one). They are very likely `const u16 []`. Retyping them is
 * a globals-header job touching a matched file, not a side effect of this
 * function; the draft casts at the call instead. */
void sub_0803F908(int, int, const u8 *, int, u8);
int sub_08027198(int);
void sub_0803FC28(int, int, int, int);
/* Wave 33, W33-B: matched. Two int parameters; the first is a dead parameter
 * (r0 is clobbered before use) and the second is the VRAM tile index. void. */
void sub_0803FD80(int, int);
/* Wave 33, W33-B: the sub_0803F110/sub_0803F128 accessor pair and the
 * sub_0803F27C/sub_0803F29C helpers, called by sub_0803F140 and its siblings.
 * sub_0803F110 returns a Decompress source (`u8 *`); sub_0803F128 returns a
 * sprite descriptor handed to sub_0801C70C's `const void *` (`const u16 *`,
 * matching c_0803F128.c); sub_0803F27C returns an int layer value;
 * sub_0803F29C writes two int out-parameters (read back with word `ldr`). */
u8 *sub_0803F110(int);
const u16 *sub_0803F128(int);
int sub_0803F27C(int);
void sub_0803F29C(int *, int *, int);
void sub_08035760(ProcPtr, void *);
/* Wave 43, W43-B. sub_08035850 spawns the gUnknown_0849BDB8 proc on
 * PROC_TREE_5 and seeds it. All three parameters are `int`: each reaches its
 * use unnarrowed -- `lsls #4` straight off the incoming register for the first
 * two, a bare `strb` for the third -- and an s16 parameter would have needed a
 * sign-extension in front of the shift. `pop {r0}` makes it void.
 *
 * sub_08035BC4's three parameters are s16, and that is PROMOTE_MODE read
 * forwards: each is zero-extended once at entry and sign-extended again at
 * every arithmetic use, while the gUnknown_03001470 unk1e/unk20 stores use the
 * RAW zero-extended copies -- which is exactly `strh` of an s16 parameter.
 * Its own fan-in is 0, so nothing constrains this from the caller side.
 *
 * The last three are the "PROMOTED BUT NEVER DECLARED" trap: all are already
 * matched, in src/decomp/c_0801BD00.c and src/unit.c. The types
 * below are COPIED FROM THOSE DEFINITIONS and were not re-derived. */
void sub_08035850(int, int, int);
void sub_08035BC4(s16, s16, s16);
void sub_0801BDB4(s32, s32, u16 *, s32);
int sub_08042F5C(int);
int sub_08042FA4(int);
void sub_08035DF4(void *);
void sub_08035E90(ProcPtr);
/* Wave 38 (W38-A): first parameter retyped from `void *` to
 * `struct MusicPlayerInfo *` -- see sub_08071420. */
void sub_08071488(struct MusicPlayerInfo *, u16, s16);

/* ---- wave 28 (W28-A extension): the 0x08036000 block ---- */

/* Copied from the promoted definitions in src/decomp; the 0x08036000 block is
 * the first cross-file caller each of them has had.
 * sub_08036CB4 lives in the AgbMain unit (src/main.c) but is an ordinary
 * global, so the relocation is real -- it is NOT the `static` sub_08036B48
 * hazard that unit carries. */
bool8 sub_0802759C(void);    /* c_0802759C.c */
void sub_08036CB4(void);     /* src/main.c */
void sub_0804A010(void);     /* c_0804A010.c */
/* The heap free's forwarder. Its own definition's comment already records the
 * evidence for `void *`: sub_080363D0 does `ldr r0, [r4, #0x48]` immediately
 * before the `bl`, and that is now a real cross-file call rather than a note. */
void sub_080364D4(void *);   /* c_080364D4.c */

/* RETYPED in wave 36 (W36-F) from `void (void)`. The old note reasoned from
 * sub_0803647C setting up no argument, but "no setup" is exactly what a
 * forwarded first parameter looks like: sub_0803647C opens `adds r4, r0, #0`
 * and r0 still holds its own proc at the `bl`, so the call costs zero
 * instructions either way. The BODY settles it -- sub_08036024 opens
 * `adds r3, r0, #0` and dereferences r3 at +0x36 as the proc's u8 state, then
 * hands the same pointer to sub_08035E90(ProcPtr). src/decomp/c_0803647C.c was
 * updated to pass `proc` and re-verified byte-for-byte with trymatch. */
void sub_08036024(ProcPtr);
/* Wave 45, W45-B: this one was DEFINED in src/decomp/c_080364D4.c since wave 30
 * but never declared here, so sub_080364F4 -- its first C caller -- would not
 * compile. Signature copied from that definition, not re-derived. */
void sub_080364E0(void);
/* Wave 45, W45-B: four more that are DEFINED in src/decomp but were never
 * declared here, so sub_080360D0 -- their first C caller -- would not compile.
 * Every signature is copied verbatim from the promoted definition
 * (src/decomp/c_0802723C.c, c_080360A4.c, c_0803647C.c, c_0805C974.c), not
 * re-derived from a call site.
 *   sub_0805C974's `int` return is the definition's, and it is why its callers
 * have to narrow: sub_080360D0 tests it `lsls #0x18; cmp #0`, i.e. `(u8)`. */
void sub_08027278(int, int);
void sub_080360A4(ProcPtr);
void sub_0803647C(ProcPtr);
int sub_0805C974(void);

/* ---- callees of the 0x0801B000 block (wave 28) ------------------------ */

/* sub_0808AE54 takes FOUR arguments, and that is MEASURED, not guessed.
 * sub_0801B66C forwards only two of them and yet still spends
 * `push {r4, lr}; adds r4, r1, #0` parking its second parameter in a
 * callee-saved register before narrowing its first into r1. The same body
 * probed against 2-, 3- and 4-argument declarations reproduces that prologue
 * ONLY at four: with two or three arguments r2 (and r3) are free, the narrowed
 * value lands there, and nothing is saved.
 *
 * This is the direct-call analogue of the `_call_via_rN` arity tell -- a
 * pass-through argument costs no instruction, but it does occupy a register,
 * and when it occupies the last free scratch the pressure is visible in the
 * prologue. It only reads out when the wrapper has something else that must
 * live across a call or a clobber, so it is not a general method; here the
 * u16 narrowing of the first parameter supplies exactly that.
 *
 * The return is unused at the only known call site, so `void` is a floor. */
void sub_0808AE54(u16, int, int, int);

/* sub_0808AC44 takes TWO arguments, by the same register-pressure readout that
 * measures sub_0808AE54 above -- and this one was caught by a failed match
 * rather than predicted, which is what makes it worth writing down.
 *
 * sub_0801B598 sets up r0 only, so the argument count looks like one. But the
 * ROM narrows its u8 parameter into r2, skipping r1, and a one-argument
 * declaration puts it in r1 and misses by exactly those 2 bytes. r1 is reserved
 * because a second, forwarded parameter is riding it. Three arguments would
 * have pushed the narrowed value to r3, so two is exact, not a floor.
 *
 * The widths at both ends are independent facts: `lsls #0x18; lsrs #0x18` at
 * entry is the u8 first parameter, and the `lsls #0x10; lsrs #0x10` after the
 * call is agbcc re-narrowing a u16-returning callee. The u8 agrees with the
 * note on gUnknown_03000F70 in unknown-globals.h, which has sub_0808AC44
 * rejecting an id above 3. Nothing types the second parameter, so `int`. */
/* WAVE 29 (C) types the second parameter. It is not `int`: sub_0808AC44's own
 * body does `ldr r0, =sub_0808AC20; str r0, [r2]`, i.e. it publishes the timer
 * IRQ handler THROUGH the pointer, so the parameter is where the caller wants
 * the installed callback written back. sub_0808AC20 is `void (void)`.
 * The arity readout above is untouched.
 *
 * CORRECTED at wave-29 integration: the note here originally said "the only
 * caller, sub_0801B598, is not promoted, so nothing has to change with it".
 * sub_0801B598 IS promoted (src/decomp/c_0801B598.c, its own single-function
 * unit) and this retype broke `make SPLIT=1 compare` on it -- per-function
 * try_match compiles one unit and cannot see a caller in another file, so the
 * split build is the only thing that catches this. Its four remaining callers
 * are still in asm/code.s, which is what the "not promoted" reading confused
 * it with. sub_0801B598 is a pure forwarder with no C caller and no
 * declaration here, so the pointer type was propagated through it rather than
 * cast at the call site; forwarding a pointer parameter unchanged is
 * byte-neutral and it re-verified. */
u16 sub_0808AC44(u8, void (**)(void));

/* sub_0808AF00 returns at least `int`. sub_0801B648 returns its result
 * directly with NO re-narrowing, and that is decisive here rather than merely
 * suggestive: the function's other arm returns an `int` parameter, so the
 * return type is int, and a u16-returning callee would have been re-narrowed
 * before being widened back. Arity is a floor for the usual reason. */
/* WAVE 44 (W44-G) settles the arity at TWO from the callee's own body, which is
 * what the "floor" above was waiting for: sub_0808AF00 opens `adds r5, r1, #0`
 * and hands r5 to the relocated sub_0808AED0 as its `src`, so r1 is a real
 * parameter. Both un-promoted callers (sub_0808AFC8 at 0x0808B00E and
 * sub_0801B648) set up r0 AND r1. sub_0801B648's promoted body was updated to
 * forward its own `b` explicitly and re-verified byte-for-byte -- a
 * pass-through argument costs no instruction, so that edit is free.
 *
 * The second parameter is a SOURCE BUFFER POINTER by its use (sub_0808AED0's
 * first parameter) but is typed `int` here because sub_0801B648's other arm
 * RETURNS the same value as an `int`; `int` is the spelling every existing
 * caller already agrees with, and sub_0808AF00 casts it at the one use. */
int sub_0808AF00(u16, int);

/* ---- the 0x0808AE30 SRAM block (wave 44, W44-G) ----------------------- */

/* sub_0808AE30 / sub_0808AED0 are the two leaf SRAM primitives, and they are
 * declared here rather than left file-local because sub_0808AE54, sub_0808AF00
 * and sub_0808AF74 take their ADDRESSES as pool words in order to copy their
 * machine code onto the stack. Both signatures are read off c_0808AE30.c's
 * matched body and off the argument set-up at those three call sites.
 *
 * sub_0808AED0 returns a POINTER, not a flag: its mismatch arm is
 * `subs r0, r3, #1`, the second parameter's pre-increment value, and its
 * fall-through arm is `movs r0, #0`. Its callers only ever test it against 0,
 * which is why sub_0808AF00/sub_0808AF74 can return `int`. */
void sub_0808AE30(const u8 *, u8 *, int);
u8 *sub_0808AED0(const u8 *, u8 *, int);

/* sub_0808AF74 is sub_0808AF00 with a caller-supplied length instead of
 * gUnknown_08485550.unk18. Its only caller (sub_0808B02C, still in asm) sets up
 * r0, r1 and r2 and then does `adds r3, r0, #0; cmp r3, #0`, returning that
 * value -- so three arguments and an `int` return, on the same reasoning as
 * sub_0808AF00 above. Parameter 2 is `int` for the same reason too. */
int sub_0808AF74(u16, int, int);

/* ---- the 0x0808B flash driver (wave 47, W47-D) ------------------------ */

/* sub_0808B02C is sub_0808AFE8 with a caller-supplied length: same three-try
 * program-and-verify loop, but sub_0808AF74 in place of sub_0808AF00, so it
 * carries the extra `int` and returns `int` for the same reason. */
int sub_0808B02C(u16, int, int);

/* Erase-sector primitives. Every one of these returns a u16 status -- each
 * caller re-narrows the result with `lsls #0x10; lsrs #0x10`, which is agbcc
 * re-narrowing a u16-returning callee, and 0x000080FF is the shared "bad sector
 * number" code the range checks return.
 *
 * sub_0808B430 writes the JEDEC unlock/erase command for ONE sector and is
 * called only by sub_0808B4B4, which passes it a u16; sub_0808B0E8 is its
 * already-in-asm twin one level up, called by sub_0808B31C the same way. */
u16 sub_0808B0E8(u16);
u16 sub_0808B430(u16);
u16 sub_0808B4B4(u16);

/* sub_0808B184's two parameters are read off its matched body in
 * src/decomp/c_0808B184.c: a byte source and a flash destination, both walked
 * one byte at a time by sub_0808B31C. */
u16 sub_0808B184(u8 *, u8 *);

/* sub_0808B31C programs one sector: sector number then the source buffer it
 * hands to sub_0808B184 unchanged apart from the increment, hence `u8 *`. */
u16 sub_0808B31C(u16, u8 *);

/* sub_0808B2E0 is the "is this sector still blank" scan and is the one function
 * in the block that does NOT return a status: it counts down from
 * gUnknown_03005C78->unk04 and returns whatever is left when the walk hits a
 * non-0xFF byte, with no narrowing at `adds r0, r1, #0`, so the return is the
 * member's own width. */
u32 sub_0808B2E0(u8 *);

/* Wave 56 (W56-N): transcribed from the PROMOTED body in
 * src/decomp/c_0808B304.c, not lifted from a call site. Declared here because
 * sub_0808B1BC both calls it and takes its address -- the address is the END
 * MARKER for the byte length of sub_0808B2E0, which that function relocates
 * into a stack buffer and then runs from RAM. */
int sub_0808B304(int, int (*)(int));

/* sub_0808A368 forwards its only argument unchanged as sub_08071AF0's
 * ProcPtr third parameter, so it is a ProcPtr and nothing narrows it. */
void sub_0808A368(ProcPtr);

/* sub_0808AC7C is the arm/start half of the 0x03000F68 timer module described
 * in unknown-globals.h -- it takes the same u8 slot id sub_0808AC44 does
 * (`lsls #0x18; lsrs #0x18` at entry) and returns nothing. */
void sub_0808AC7C(u8);
/* The disarm half of the same 0x03000F68 timer module: stops the timer, clears
 * the timer's REG_IE bit and restores REG_IME. void/void -- sub_0808ADA4 calls
 * it with no argument set up and ignores r0 afterwards, returning a value it
 * had already parked in r8 across the call. */
void sub_0808AD24(void);

/* sub_0801B9C8 -- four arguments. The result is added to a u8 field and to a
 * word field with no narrowing in between, so the return is at least `int`.
 *   Parameters 3 and 4 WERE `u16` on caller-side evidence only, and the note
 * here already flagged both as unmeasured ("a cast at the argument", "the
 * WRAPPER's width"). Wave 37 (W37-G) measured them at the CALLEE and they are
 * int: sub_0801B9C8's prologue is `adds r7,r0,#0; adds r5,r1,#0;
 * adds r4,r2,#0; adds r1,r3,#0` with NO `lsls #0x10; lsrs #0x10` on either --
 * agbcc's PROMOTE_MODE emits that pair at entry for every sub-word parameter,
 * so its absence is decisive. The `lsls/lsrs` that does appear is on the SUM
 * `record.x + a3`, i.e. the u16 conversion at sub_0801BA1C's own u16
 * parameter, which is the copy-then-narrow shape of an int parameter.
 * Re-verified src/decomp/c_0801B964.c (sub_0801B998, the only caller) after
 * the change: still byte-exact, because a u16 value converts to int free. */
int sub_0801B9C8(int, u32, int, int);

/* sub_0801BA4C returns a SIGNED halfword: sub_0801BAA8 re-narrows the result
 * with `lsls #0x10; asrs #0x10`, and the ARITHMETIC shift is the sign.
 *
 * WAVE 29 (C) REFUTES THE PARAMETER. It was `void *` here and `u8 *` in
 * src/decomp/c_0801BAA8.c, on the reading that sub_0801BAA8's `adds r0, #0x5a`
 * walks 0x5a bytes into a struct. It does not: sub_0801BA4C's body is a
 * DEGREE-BASED SINE lookup on the value it is handed --
 *   while (x < 0)     x += 0xb4 * 2;   (360)
 *   while (x > 0x167) x += -0x168;     (-360)
 *   if (x > 0xb3) x -= 0xb4;           fold the lower half-turn
 *   if (x > 0x5a)  x = 0xb4 - x;       mirror about 90
 *   gUnknown_0808F048[x], negated when the original was >= 180
 * -- every step a SIGNED compare and an add on the value itself. No pointer
 * undergoes modular reduction against 360. So sub_0801BAA8(x) is
 * sub_0801BA4C(x + 90), i.e. cosine, and 0x5a is a quarter turn, not a member
 * offset.
 *
 * Corroborated from a caller, which is the only place it could be seen:
 * sub_08064034 and sub_0806407C build rotation matrices and pass THE SAME
 * sign-extended s16 angle to both functions, storing the two results as the
 * cos/sin entries of a 20.12 matrix. A pointer cannot be that argument.
 *
 * `int` and not `s16`: the prologue is a bare `adds r2, r0, #0` with no
 * narrowing anywhere, and the reduction loops need the full value.
 * src/decomp/c_0801BAA8.c was retyped in the same edit and re-verified
 * byte-exact -- `p + 0x5a` and `a + 0x5a` are the same `adds r0, #0x5a`. */
s16 sub_0801BA4C(int);
s16 sub_0801BAA8(int);

/* sub_080718E8(src, count) -- all three flush paths in the 0x0801B000 block
 * (sub_0801BBC4, sub_0801BC08, sub_0801BCA8) call it with a pending-copy
 * descriptor's unk00 pointer and its unk0a halfword count taken straight from a
 * `ldrh`, either right after the CpuFastSet that copies that same range or, in
 * sub_0801BCA8, instead of it. */
void sub_080718E8(void *, u16);

/* Three the tree already uses but never declared. sub_0801BB88 and
 * sub_0801BE78 are copied from their PROMOTED definitions in
 * src/decomp/c_0801BB88.c and src/decomp/c_0801BE78.c, which win over any
 * weaker model; sub_0801DF94 is still assembly and its `void (void)` is a floor
 * read off sub_0801BCE0, which sets up no argument register. sub_0801B768
 * likewise comes from src/decomp/c_0801B768.c. */
void sub_0801B768(int);
void sub_0801BB88(int);
void sub_0801BE78(void);
void sub_0801DF94(void);

/* ---- callees of the 0x0806E000 block (wave 28) ------------------------ */

/* sub_08073F90 hands out a u16 PAIR through two out-parameters. sub_0806E6F4
 * gives it two adjacent halfword slots of its own frame (`mov r0, sp` and
 * `sp + 2`, with `sub sp, #4` reserving exactly the two) and then reads both
 * back with `ldrh`. The signature was already recorded in the note on
 * gUnknown_03000044/46 in unknown-globals.h; this is the declaration. */
void sub_08073F90(u16 *, u16 *);

/* Two more of sub_0806E11C's teardown calls. `void (void)` is a floor for both
 * -- it sets up no argument register and ignores both results -- but they sit
 * in a run with sub_0806D620, already declared the same way. */
void sub_0806D34C(void);
void sub_0806D840(void);

/* ---- callees of the 0x0801E000 block (wave 28) ------------------------ */

/* sub_0801E18C takes the record INDEX, and that is measured rather than
 * assumed. Its three callers sub_0801E22C, sub_0801E248 and sub_0801E264 all
 * compute `&gUnknown_0200F720[i]` and then call it, and in all three the ROM
 * leaves the index in r0 and puts the computed address in the next register
 * DOWN from the argument block -- r2/r3 in the two-argument caller, r3/r4 in
 * the three-argument one, r4/r5 in the four-argument one. Declared `(void)`,
 * agbcc reuses r0 for the address in every one of them and all three miss.
 * The register that survives is the argument. */
void sub_0801E18C(int);

/* Wave 42, W42-F: the two callees of sub_0801DF94 that had no declaration in
 * this header. Both are `void` -- each ends `pop {r0}; bx r0`, which destroys
 * the callee's result in r0 before returning. sub_0801E0A4 sets up r0/r1/r2 for
 * sub_08011C90 entirely from its own constants, so it takes nothing.
 *
 * sub_0801E22C's three narrow parameters are NOT a caller-side reading: it is
 * already promoted as `void sub_0801E22C(int index, u16 a, u16 b, u16 c)` in
 * src/decomp/c_0801E22C.c, and the promoted definition wins. The call site
 * cannot see the difference -- sub_0801DF94 (its only caller) passes 0x100,
 * 0x100 and 0 as constants, which need no narrowing either way -- so `int`
 * would have compiled and matched here while silently disagreeing with the
 * definition, exactly the class of error trymatch cannot catch because it
 * compiles one unit. tools/proto_check.py is what caught it. */
void sub_0801E22C(int, u16, u16, u16);
void sub_0801E0A4(void);

/* sub_0801E334 returns `int`, NOT the `u16` its promoted definition in
 * src/decomp/c_0801E334.c carried until wave 28 -- the definition has been
 * retyped to agree and re-verified byte-identical (the body is `return *p;`,
 * one `ldrh`, which zero-extends and so needs no extra instruction either
 * way).
 *
 * The evidence is at the CALLER, which is where a return type is settled:
 * sub_0801E950 forwards the result straight into sub_0801E0C8's `int`
 * parameter with NO re-narrowing, and agbcc re-narrows a narrow-returning
 * callee at every call site. Declared `u16`, sub_0801E950 gains an
 * `lsls #0x10; lsrs #0x10` pair it does not have. */
int sub_0801E334(u16 *);

/* SIX parameters, not seven, and the fifth is 64 BITS WIDE. That is the whole
 * story of this little family and it was worth four functions.
 *
 * Read as seven `int`s, all four wrappers miss by exactly one register: three
 * come out 4 bytes SHORT and sub_0801ED80 8 bytes short. The tells, which only
 * make sense together:
 *
 *   * sub_0801E930 materialises the two zero words into TWO callee-saved
 *     registers (`movs r4,#0; movs r5,#0`) and then builds -1 as
 *     `movs #1; rsbs` rather than the one-instruction `subs r4,#1` an
 *     already-live zero would have allowed. Seven `int`s let CSE collapse the
 *     two zeros into one register, which is the missing 4 bytes. A DImode
 *     constant occupies a REGISTER PAIR and cannot be collapsed.
 *   * sub_0801ED80 spends `sub sp, #4` BEFORE its push and then round-trips r3
 *     through `str r3,[sp,#0x20]; ldr r1,[sp,#0x20]`. That is not a spill and
 *     not varargs (agbcc's varargs prologue is `push {r2,r3}`, measured): it is
 *     a 64-bit parameter STRADDLING the register/stack boundary, so gcc
 *     reserves a home slot to make its two words contiguous. agbcc even labels
 *     the reload `@ created by thumb_load_double_from_address`.
 *   * sub_0801ED80 then reads THREE stack slots (0x20/0x24/0x28) where six
 *     `int`s give only two.
 *
 * A 12-byte struct by value was tried first and is refuted: it compiles to
 * `ldmia`/`stmia` block copies that appear nowhere in the ROM.
 *
 * The first parameter of sub_0801ECE8 is s16 -- both wrappers narrow with
 * `lsls #0x10; asrs #0x10` at the call and the ARITHMETIC shift is the sign.
 * Nothing types the rest, so `int` is what costs no instruction. Whether the
 * 64-bit parameter is really one quantity or two words the callers happen to
 * pass adjacently is NOT settled here -- what is measured is its width and its
 * alignment behaviour at a call. */
int sub_0801E338(int, int, int, int, long long, int);

/* ---- wave 29, W29-B: address-locality block 0x0801D --------------------- */
/* The two workers the 0x0801D7xx wrappers forward to. ARITY IS READ OFF THEIR
 * OWN PROLOGUES, not off the wrappers: sub_0801D6E8 reads `[sp,#0x14]` after
 * pushing five registers, so five parameters; sub_0801D78C reads `[sp,#0x18]`
 * and `[sp,#0x1c]` after pushing five and subtracting 4, so six. Both return a
 * value (sub_0801D6E8's early exit is `adds r0,r3,#0` on a -1). Every argument
 * is forwarded unchanged at all five call sites, so `int` throughout is what
 * costs no instruction -- it is the weakest fit, not a proof of the widths. */
int sub_0801D6E8(int, int, int, int, int);
int sub_0801D78C(int, int, int, int, int, int);
/* sub_0801D348's other branch. Its body wants narrower types than this
 * (`lsls #0x10; asrs #0x10` on argument 1, `strh` on 2 and 3, `ldrh` on the
 * stack argument 5) but the declaration is kept wide DELIBERATELY: its one
 * caller forwards all five straight through with no conversion, which a
 * declared-narrow parameter would not have allowed. Re-derive it when
 * sub_0801E4B0 itself is matched. */
int sub_0801E4B0(int, int, int, int, int);
/* Wrappers matched in wave 29. sub_0801D7D4 keeps its own r3 and appends
 * (0, 0x1d); sub_0801D804 is the same call with argument 4 forced to 0, the
 * same relationship sub_08015578's pair has. sub_0801D7EC forces argument 4 to
 * 0 and pushes its own last two along. */
int sub_0801D7D4(int, int, int, int);
int sub_0801D7EC(int, int, int, int, int);
/* Argument 7 is a 64-bit quantity: it is loaded as two adjacent words at
 * [sp,#0x24]/[sp,#0x28] with agbcc's own "created by
 * thumb_load_double_from_address" pairing and lands in sub_0801E338's declared
 * `long long` slot. Argument 8 is `u16` at entry (`lsls #0x10; lsrs #0x10`) and
 * is cast to `(s16)` at the one use. Argument 1 is only ever tested `& 1`. */
void sub_0801D348(int, int, int, int, int, int, long long, s16);
void sub_0801D81C(int);
/* Wave 56, W56-H. Transcribed from the PROMOTED definitions in
 * src/decomp/c_0801E22C.c, c_0801E27C.c and c_0801E294.c -- the definition is
 * the witness, not sub_0801D390's call sites. */
void sub_0801E248(int, s16, s16);
void sub_0801E264(int, s16);
void sub_0801E27C(int, s16, s16, s16);
void sub_0801E294(int, s16, s16);
/* The three fixed-point readers. All divide by 256 -- the `cmp #0; bge;
 * adds #0xff` before the `asrs #8` is a SIGNED DIVIDE rounding toward zero, not
 * a shift, and the members are the s32 unk0c/unk10/unk14/unk18. sub_0801D9AC
 * reads position plus offset, sub_0801D9E4 the offset alone and sub_0801DA14
 * the position alone. */
void sub_0801D9AC(int, s16 *, s16 *);
void sub_0801DA14(int, s16 *, s16 *);
int sub_0801ECE8(s16, int, int, int, long long, int);

/* FIVE parameters: three in registers, then the same 64-bit quantity starting
 * in r3 and continuing on the stack, then an s16. sub_0801EDF8 narrows its own
 * fourth parameter with `lsls #0x10; asrs #0x10` into the slot past it. */
int sub_0801ED80(int, int, int, long long, s16);

/* ---- wave 29 (C) ---- */
/* FOUR parameters and void (`pop {r0}`). r0/r1 are parked in r8/sb untouched
 * and stored to +0x2c/+0x30 of the proc it starts; r2 goes to the `int`
 * gUnknown_030044D4 AND to +0x54 as a word, so it is a full word; r3 is the
 * parent handed to Proc_StartBlocking. sub_0803FECC passes 0 for the third and
 * sub_0803FEDC/sub_0803FF04 pass -1 and -2, which is why it is signed. */
void sub_0803FF48(int, int, int, ProcPtr);
/* src/decomp/c_0803F3E4.c already defines this as
 * `void sub_0803F3E4(int a, int b, ProcPtr parent)`; the declaration is added
 * here because sub_0803F3C8 calls it from another unit. Both its word stores
 * are `str`, and `adds r1, r2, #0` at the call in the definition fixes the
 * parent as the third parameter. */
void sub_0803F3E4(int, int, ProcPtr);

/* The two arms of sub_08041958.
 *
 * sub_0804074C is declared WITHOUT a prototype, the same way sub_0801C240 is
 * above: its first parameter is a pointer to an object whose +0x02 halfword and
 * +0x04 byte it reads, and whoever promotes sub_0804074C will want to name that
 * struct locally. Its second parameter is the parent for Proc_StartBlocking.
 *
 * sub_08040790 takes THREE. The third is not visible at sub_08041958's call --
 * r2 already holds the proc there, so no instruction sets it up -- but
 * sub_08040790's own prologue does `adds r1, r2, #0` before
 * `bl Proc_StartBlocking`, which is a parent arriving in r2. The first two are
 * stored as words at +0x2c/+0x30 of the new proc and are also used as a cell
 * column and a cell row into the gUnknown_08499590 grid, so `int` for both. */
void sub_0804074C();
void sub_08040790(int, int, ProcPtr);

/* ---- wave 31, W31-B: the 0x08040000 block's cross-unit callees ---- */
/* Already DEFINED in src/decomp/c_0803FECC.c with exactly these signatures;
 * declared here because sub_0804026C and sub_08040290 call them from another
 * unit. Both prologues are bare `adds rN, rM, #0` copies with no PROMOTE_MODE
 * narrowing, so all three parameters are word wide, and the third is the
 * ProcPtr they forward to sub_0803FF48's parent slot. */
void sub_0803FEDC(int, int, ProcPtr);
void sub_0803FF04(int, int, ProcPtr);
/* THREE parameters and void (`pop {r0}` after a `bl` whose result is dropped).
 * The first two are a cell column and a cell row into the gUnknown_08499590
 * grid -- r0 is added to a rowOffset entry and r1 is scaled `lsls #1` as the
 * row index -- and the third is untouched all the way through to
 * sub_0804046C's fifth argument, which is a Proc_StartBlocking parent. Its one
 * caller sub_080409B4 passes its own proc there. */
void sub_08040380(int, int, ProcPtr);
/* SIX parameters, the last two on the stack, and void. Nothing narrows any of
 * them: the prologue is four bare register copies plus `ldr r7,[sp,#0x18]` /
 * `ldr r1,[sp,#0x1c]`, and r7 reaches a `strh` only at the store, which is a
 * conversion at a use and not a declared width. The sixth is the
 * Proc_StartBlocking parent. sub_08040624 is the four-parameter wrapper that
 * fixes the third and fourth at 0x1CA and 5. */
void sub_08040554(int, int, int, int, int, ProcPtr);
void sub_08040624(int, int, int, ProcPtr);

/* Wave 29 (C), the 0x0808A block.
 *
 * sub_0808A5C4 reads no argument register before writing it (it opens with a
 * `bl`) and ends `pop {r0}`, so nullary and void.
 *
 * sub_08014740 takes SIX and returns the sub_080152EC slot it allocates (r8 is
 * that result and is what r0 carries out). FIVE of the six are `u16`: its
 * prologue narrows r0, r1, r3 and both stack arguments with `lsls #0x10;
 * lsrs #0x10`, which is PROMOTE_MODE and which an `int` parameter never
 * produces. The third is untouched and is forwarded as a pointer --
 * sub_0808A6A0 passes the dereferenced `u16 *` gBG0TilemapBuffer. */
void sub_0808A5C4(void);
/* CORRECTION, wave 32 (W32-A): arguments ONE and TWO are `s16`, not `u16`.
 * The wave-29 reading above is right that the prologue narrows them -- but
 * PROMOTE_MODE zero-extends EVERY sub-word parameter regardless of signedness,
 * so the prologue proves narrow and says nothing about the sign. sub_08077214
 * is the first caller that hands them a variable rather than a literal, and it
 * sign-extends both (`asrs r5, r5, #0x10; asrs r4, r4, #0x10`) with no further
 * conversion in front of the `bl`. Through a `u16` parameter agbcc would
 * convert that back down with `lsls; lsrs` instead. The other three narrow
 * arguments keep `u16` -- nothing has exercised their sign yet.
 * Byte-neutral for the two existing callers (c_08018758.c and c_0808A664.c both
 * pass literals); both re-verified. */
struct Unk03001470 *sub_08014740(s16, s16, u16 *, u16, u16, u16);
/* Promoted as `void sub_0808AC20(void)` in src/decomp/c_0808AC20.c; declared
 * here because sub_0808AC44 publishes its address through a parameter. */
void sub_0808AC20(void);

/* The BIOS LZ77 decompressor, VRAM variant. Nothing declared it before wave 29
 * even though `asm/` calls it in several places; the shape is the standard BIOS
 * one and sub_0804BB28 passes a ROM blob pointer and a destination. */
void LZ77UnCompVram(const void *, void *);
/* Declared WITHOUT a prototype, like sub_0801C240: its first parameter is
 * walked as a `u16 *` (64 halfwords, each halved per 5-bit channel) into a
 * scratch buffer before a CpuFastSet, and whoever promotes it will want to name
 * that pointer type. Its second parameter is the CpuFastSet destination. */
void sub_0804BD58();

/* Wave 29, W29-A -- the 0x0802D000 address-locality block's callees. Each was
 * already DEFINED or is still asm; none had a declaration, so these publish
 * what their own prologues say.
 *
 * sub_08029948's parameter is `int`, not `u16`: the prologue is
 * `adds r4,r0,#0` FOLLOWED by `lsls #0x10; lsrs #0x10`, i.e. copy-then-narrow,
 * which is the cast-at-a-use shape and not PROMOTE_MODE on a declared-narrow
 * parameter. The narrowed value is `strh`ed into gUnknown_03001470[i].unk22.
 * Its two callers (sub_0802D168, sub_0802D1A0) pass literals, so they cannot
 * discriminate.
 *
 * sub_080637AC is a slot lookup over gUnknown_03001470: it walks the array
 * DOWN from index 0x1d (base + 0xae0, `subs r1,#0x60` per step) and returns
 * the first element whose unk00 equals the argument, else 0 -- so the return is
 * a `struct Unk03001470 *` and the argument is the script address callers hand
 * it (gUnknown_0848A42C at sub_0802D33C). `const void *` is the weakest model:
 * nothing dereferences the argument, only compares it.
 *
 * sub_080236E8 / sub_08042650 / sub_08042864 / sub_08060684 / sub_080606A0 all
 * open by loading a pool word or making another `bl` and never read r0, so
 * `void (void)`; all five end `pop {r0}; bx r0`.
 *
 * sub_0802D5B8 is `Decompress(sub_08037250(), a1)` -- the destination buffer,
 * hence `void *` to match Decompress's second parameter.
 *
 * sub_0802D7B4's parameter is `int`: its one readable caller sub_0802D99C
 * SIGN-extends the value into r0 (`lsls #0x10; asrs #0x10`) immediately before
 * the `bl`, which a declared-narrow parameter would not ask for -- a u16
 * parameter makes the caller emit the zero-extending `lsrs` instead. The
 * `lsls #0x10; lsrs #0x10` in sub_0802D7B4's own prologue is a `u16` local it
 * spills to [sp,#0x1c], not PROMOTE_MODE. */
void sub_08029948(int);
struct Unk03001470 *sub_080637AC(const void *);
void sub_080236E8(void);
void sub_08042650(void);
void sub_08042864(void);
void sub_08060684(void);
void sub_080606A0(void);
/* Wave 55, W55-C. The last two entries of sub_0805FFA0's 20-way jump table
 * (cases 0xd and 0xe) that had no declaration and no promoted body -- every
 * other arm of that dispatcher is either declared above or defined in
 * src/decomp (c_080600F0.c, c_080601C8.c, c_080601F0.c, c_080606BC.c,
 * c_0802C16C.c, src/unit.c), and all of those are `void (void)`.
 * sub_0805FFA0 reaches both through `mov pc, r0` with no argument setup on
 * any path into the table and discards the result, and the two globals notes
 * on struct Unit.unk09 (W32-A) and gUnknown_030046C0 (W49-E) describe
 * both as reading their operands out of globals. */
void sub_08060110(void);
void sub_08060170(void);
/* The other nine arms of that table. Each of these ALREADY HAS A PROMOTED
 * BODY and every one of them is defined `void f(void)`; the declarations below
 * are copied from those definitions rather than inferred from the call site.
 *   sub_080600F0            src/decomp/c_080600F0.c
 *   sub_080601C8/080601DC   src/decomp/c_080601C8.c
 *   sub_080601F0/08060264/080602C4  src/decomp/c_080601F0.c
 *   sub_080606BC            src/decomp/c_080606BC.c
 *   sub_0802C16C            src/decomp/c_0802C16C.c
 *   sub_08042B84            src/unit.c */
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
/* Wave 34 (W34-I). All three are called argument-free by the block-0x08042
 * cursor helpers (sub_080424FC, sub_0804256C, sub_08042B9C). sub_080176A4 and
 * sub_080198D0 discard the result at every call site, so void.
 * sub_08035170's result is stored straight into gPlaySt's byte at
 * +0x2e with a bare `strb` and no re-narrowing -- which does NOT discriminate
 * u8 from int, since `strb` truncates either way. u8 is the weaker guess of
 * the two and is recorded as UNPROVED. */
void sub_080176A4(void);
void sub_080198D0(void);
u8 sub_08035170(void);
u8 *sub_08037250(void);
void sub_0802D5B8(void *);
void sub_0802D76C(void);
void sub_0802D7B4(int);
/* Wave 56, W56-E. Was UNDECLARED although src/decomp/c_0802239C.c has carried
 * a byte-exact definition since wave 30; this declaration is copied from that
 * definition, which is the stronger witness, and not derived from call sites.
 * sub_0802D7B4 is the caller that needed it. */
void sub_0802239C(u16 *, u16, u16, u8, u16, u8, u8);

/* Wave 29, W29-A -- the 0x08025000 address-locality block's callees.
 *
 * TWO OF THESE FIX AN ARITY THAT IS INVISIBLE AT THE CALL, and both are read
 * off the callee's prologue exactly as docs/agbcc-codegen.md says to:
 *
 *   sub_08024F20 takes THREE arguments. r2 is never written before
 *   `ldrh r0,[r2]` / `ldrh r0,[r2,#2]`, whose results are `strb`ed into
 *   gUnknown_08499594[i].unk02 / .unk03 -- the {u16;u16} pair, i.e.
 *   struct Unk802C57C. Its ONE caller, sub_080251BC, never touches r2, so
 *   sub_080251BC has a third parameter too and forwards it for free. r0 and r1
 *   are `s16`: sub_080251BC narrows both with `lsls #0x10; asrs #0x10` in front
 *   of the `bl` and tests its own r1 raw (`cmp r1,#0`, no PROMOTE_MODE), so the
 *   sign-extension is the CONVERSION at the call and not a cast in the caller.
 *
 *   sub_08035740 -- already defined as `void sub_08035740(void *)` in
 *   src/decomp/c_08035740.c but never declared -- reads r0 before writing it,
 *   and sub_08025BB4 opens with a bare `bl sub_08035740`. So sub_08025BB4 has a
 *   `void *` parameter it forwards. `void sub_08025BB4(void)` is refutable, not
 *   just unproved: it would have to pass a literal, and any literal costs a
 *   `movs r0,#N` the ROM does not have.
 *
 * sub_080251D8 likewise reads r0 (`adds r1,r0,#0` then `lsls #0x10; asrs #0x10`
 * at a use -- copy-then-narrow, so `int`), which is why sub_080251BC's else-arm
 * `bl` needs no argument setup at all.
 *
 * sub_08025AEC and sub_080254AC both scan gUnknown_08499594 for a free slot and
 * return the element address or 0, so both return `struct Unit *`; both
 * end `pop {r1}` / `bx lr` with r0 live. sub_08025BE0 initialises one of those
 * records field by field at +0..+0xb, which is the whole 0x0c-byte struct.
 *
 * sub_080211DC is `(u8, s8)` off its own prologue: BOTH arguments are narrowed
 * in place with `lsls #0x18; lsrs #0x18` (PROMOTE_MODE, which zero-extends
 * whatever the signedness), and the second one alone is re-read at its use as
 * `lsls #0x18; asrs #0x18` before going out on the stack -- the second shift
 * pair is where the sign lives. sub_08025340 passing -1 corroborates it. */
void sub_08024F20(s16, s16, struct Unk802C57C *);
void sub_080251D8(int);
void sub_080211DC(u8, s8);
struct Unit *sub_080254AC(void);
struct Unit *sub_08025AEC(void);
void sub_08025BE0(struct Unit *, u8);
void sub_08025D20(int);
void sub_08035740(void *);
/* Wave 29, W29-A. `s16` and not `u16`, and the discriminator is entirely on the
 * CALLER side: sub_08025C5C's own prologue is `lsls #0x10; lsrs #0x10` on all
 * three, which PROMOTE_MODE emits for u16 and s16 alike (probed both). What
 * separates them is sub_08025C98 / sub_08025CC8, which SIGN-extend all three
 * arguments before the `bl`; declaring the parameters `u16` makes those two
 * callers emit `lsrs` there instead and neither one matches. Returns the
 * gUnknown_08499594 slot sub_08025AEC handed out, or NULL. */
struct Unit *sub_08025C5C(s16, s16, s16);
/* Wave 32 (W32-B): the same three `s16` as sub_08025C5C, read straight off its
 * own prologue (`lsls #0x10; asrs #0x10` on r0, r1 and r2 -- SIGN extension, so
 * not PROMOTE_MODE's zero-extend), and it returns sub_08025C5C's slot unchanged
 * or 0. sub_08045564 writes through the result, which is what fixes the return
 * type rather than only the family resemblance. */
struct Unit *sub_08025CC8(s16, s16, s16);

/* Wave 29, W29-A. Both are already DEFINED (src/decomp/c_0803CD14.c,
 * src/decomp/c_0803CCEC.c) and were never declared; these publish the
 * definitions unchanged. sub_0802490C and sub_08024944 are the callers, and
 * they corroborate the `u8` parameter -- each narrows `id + 0x4c` with
 * `lsls #0x18; lsrs #0x18` in front of the `bl`. The `lsls #0x18; lsrs #0x18`
 * AFTER sub_0802490C's call is not a re-narrowing of sub_0803CD14's `int`
 * result; it is sub_0802490C's own `u8` return conversion. */
int sub_0803CD14(u8);
u8 *sub_0803CCEC(u8);

/* Wave 29, W29-A -- the 0x08026000 / 0x0802A000 blocks' callees.
 *
 * sub_0803FECC takes THREE arguments, and the third is the invisible one again:
 * its whole body is `adds r3,r2,#0; movs r2,#0; bl sub_0803FF48`, so it forwards
 * r2 into sub_0803FF48's declared `ProcPtr` fourth parameter and passes 0 for
 * the third. sub_0802A588 opens `adds r2, r0, #0` -- it is parking its own proc
 * pointer in r2 for exactly that argument, which is otherwise unexplained.
 *
 * sub_08026584 is a bare `bx lr`, four bytes. Nothing about its signature is
 * recoverable from the callee.
 *   The second parameter is u16, settled in wave 45 (W45-D) from sub_080265D0,
 * which is the only caller that passes a NON-CONSTANT: it holds the value in a
 * u32 (the `__fixunsdfsi` result is compared against 9999 at full width, with
 * no narrowing anywhere in the body) and then spends `lsls #0x10; lsrs #0x10`
 * on it immediately before the `bl`. An `int` parameter would not pay for that
 * pair. sub_080265B0 passes literals 5 and 0xa, where u16 and int are identical
 * code, which is why the old `int` reading survived -- it had no discriminating
 * caller. c_080265B0.c re-verified as still MATCHED after this change.
 *
 * sub_08025D60 walks a 12-byte record list recursively and takes a signed index
 * -- `asrs r4,r4,#6` on the argument is arithmetic. void (`pop {r0}`).
 * sub_08020984 reads no argument register and ends `pop {r0}`. */
void sub_08026584(u8, u16);
/* Wave 56, W56-I -- both declared from sub_08037FD0, their only caller in the
 * tree, so both are call-site evidence and neither is proved by a body.
 *
 * sub_080265D0: r0 is the 1..4 slot counter narrowed with `lsls #0x18;
 * lsrs #0x18` (the same value the sub_080266DC(u8) guard immediately above the
 * call already fixed as u8), r1 is a plain `ldrb` of gPlaySt.unk02.
 * The result is discarded -- the next instruction is the loop increment.  The
 * second parameter's width is NOT settled: an `ldrb` source needs no narrowing
 * for u8, u16 or int, so u8 is the load width and nothing more.
 *
 * sub_08017720: four arguments, each loaded at its member's own width and
 * handed over with no narrowing -- `ldrb` of gPlayers[n].unk1d,
 * `ldrb` of gPlaySt.unk02, `ldrh` of gPlayers[n].unk38 and
 * `ldrh` of gUnknown_03004080, where n is sub_0807A908() evaluated separately
 * for the first and third.  Result discarded (`bl sub_08030574` follows).
 * Same caveat: the widths are the loads', so they are a floor.  Wave 65 then
 * compiled the complete callee body: declaring the four formals narrow adds
 * entry conversions, a spill and 68 bytes, while four `int` formals reproduce
 * the ROM's untouched r0-r3 values.  The body therefore settles all four as
 * wide; the caller's already-promoted loads remain compatible. */
void sub_080265D0(u8, u8);
void sub_08017720(int, int, int, int);
/* Wave 56, W56-I. Four more of sub_08037FD0's callees, all four copied VERBATIM
 * from their promoted definitions -- src/decomp/c_08026520.c,
 * src/decomp/c_08030574.c and src/decomp/c_08037F94.c (which carries both
 * sub_08037F94 and sub_08037FB4).  None of these was declared anywhere; the
 * definitions are the witness and nothing here is inferred from a call site.
 * c_08037F94.c's own note explains why sub_08037F94's first parameter is dead
 * but kept: the whole (int, ProcPtr) proc-starter family shares the shape. */
void sub_08026520(void);
void sub_08030574(void);
void sub_08037F94(int, ProcPtr);
void sub_08037FB4(ProcPtr);
void sub_08020984(void);
void sub_0803FECC(int, int, ProcPtr);
void sub_08025D60(int);
/* Wave 34, W34-F. sub_08040200 hands it the same struct Unk02028360 * it has
 * just been reading unk00/unk01 off, in r0 and nothing else; the result is
 * dropped. */
/* Wave 34 integration: agrees with the promoted definition in
 * src/decomp/c_0803E0D0.c, which returns its argument's type and is
 * deliberately non-void with no return statement (see the comment there).
 * The tag is completed in that .c; an incomplete type is all a pointer
 * parameter needs. Callers holding another view of the object cast. */
struct Unk3E0D0;
struct Unk3E0D0 *sub_0803E0D0(struct Unk3E0D0 *);
/* Wave 34, W34-F. Both matched this wave and both are dispatched from
 * sub_080407E4's jump table. sub_08040200 takes the proc's entry pointer and
 * the proc itself, the same (entry, parent) pair sub_0804026C and sub_08040290
 * beside it take; sub_080402B4 takes the two cell coordinates instead, and
 * forwards its ProcPtr to sub_0803FF48's declared fourth parameter. */
void sub_08040200(struct Unk02028360 *, ProcPtr);
void sub_080402B4(int, int, ProcPtr);
/* The other two arms of the same jump table, already promoted in
 * src/decomp/c_0804026C.c; declared here so sub_080407E4 can dispatch to them.
 * Signatures copied from that file, not inferred. */
void sub_0804026C(struct Unk02028360 *, ProcPtr);
void sub_08040290(struct Unk02028360 *, ProcPtr);
/* Wave 34, W34-F. Called by sub_08040380 with no argument register set up at
 * the call site and its result dropped, so `void (void)` is the weakest model
 * that fits. This was the only genuinely EXTERNAL undeclared callee across
 * blocks 0x08040 and 0x08041 -- the other four the block screen reported were
 * themselves targets in this batch. */
void sub_08021CB4(void);
/* Already promoted in src/decomp/c_08040430.c; declared here so sub_08040380
 * can drive the pair. Signatures copied from that file, not inferred. */
void sub_08040430(int, int);
void sub_0804046C(int, int, int, int, ProcPtr);
/* Wave 34, W34-F, both off sub_080408A0's call sites. sub_080232CC takes two
 * small literals (2, 0x12) and sub_0804096C the caller's own proc; neither
 * result is read. */
void sub_080232CC(int, int);
void sub_0804096C(ProcPtr);

/* Wave 29, W29-A -- sub_08052EE4 / sub_08052F20's callees. All four read no
 * argument register before writing it and all four end `pop {r0}` (sub_08012420
 * is already matched in src/decomp/c_08012420.c and simply had no declaration).
 * The void returns are floors: sub_08052F20 discards every result. */
void sub_08012420(void);
void sub_080546F0(void);
void sub_08054B14(void);
void sub_08057270(void);


/* ---- Wave 30, W30-A: the 0x08031/0x08032/0x08039 address-locality block ---- */

/* Already promoted as `void sub_080337D8(u32, u32, ProcPtr)`
 * (src/decomp/c_080337D8.c) but never declared here; sub_08031BF0 is the first
 * caller outside its own unit. */
void sub_080337D8(u32, u32, ProcPtr);
/* The five-argument sibling of sub_080337D8: sub_08031C1C passes the same
 * gUnknown_02000000 buffer, a 0xA5C size, two zeros and its own proc on the
 * stack. The proc is last, matching sub_080337D8's third-and-last position.
 *
 * Wave 34, W34-C RETYPES THE RETURN from `void` to `int`, on the body: it has
 * two exits, `movs r0,#1; rsbs r0,r0,#0` (return -1) when the size argument
 * exceeds 0x7FFF80, and a `movs r0,#0` immediately before the shared epilogue
 * on the success path. A void function does not set r0 on the way out, and it
 * certainly does not set it on BOTH paths. Its one promoted caller
 * (src/decomp/c_08031BF0.c) discards the result, so the change is byte-neutral
 * there; re-verified with try_match after the edit. */
int sub_0803376C(u32, u32, int, u8, ProcPtr);
/* Wave 34, W34-C. Undeclared callees reached from the 0x08032..0x08034 block.
 * Every signature below is read off the CALL SITE, not a body.
 *
 * sub_0803CD2C: sub_080328EC passes 0x200 and the s16 gUnknown_0849B060->unk04
 * truncated by a bare `ldrb` -- the load-width fold a u8 parameter forces -- and
 * re-narrows the result with `lsls #24; lsrs #24` before comparing it to 0.
 *
 * sub_08026704: sub_080349E4 passes the u16 gUnknown_030033EC with a plain
 * `ldrh` (no shifts either side, so the parameter is no narrower than 16 bits;
 * `int` is the weakest that fits) and re-narrows the result `lsls #16;
 * lsrs #16`, which only a u16 return emits.
 *
 * sub_080348B4 and sub_0802F4F4 are both tested `lsls #24; cmp #0` under an
 * `if`, the test-the-low-byte form; sub_0802F4F4 is already recorded elsewhere
 * in this header as returning s8 from its readers of
 * gUnknown_0849B018->unk06.
 *
 * sub_08063454 / sub_08063518 take gUnknown_03003F70, the same link-session
 * record sub_08062FB8 and sub_08062FF4 do. Their signatures are NOT read off
 * sub_08033470's call site: both are already promoted
 * (src/decomp/c_08063454.c, src/decomp/c_08063518.c) and the DEFINITIONS win,
 * so these declarations copy them verbatim. Wave 34, W34-C first wrote them as
 * `u8 *` from the caller and tools/proto_check.py caught it -- that is the
 * wave-14 SPLIT=1 breaker, and per-function try_match cannot see it because it
 * compiles one unit. Callers pass gUnknown_03003F70, declared `u8 []`, so the
 * cast is at the call site. */
u8 sub_0803CD2C(u16, u8); /* W35-E: 1st was `int`. The ROM narrows it
                           * `lsls #0x10; lsrs #0x10` BEFORE the u8 second
                           * parameter is touched -- entry-order parameter
                           * conversion, not a use-site cast. Its one caller
                           * (c_080328EC.c) passes the constant 0x200. */
u16 sub_08026704(int);
bool8 sub_080348B4(void);
s8 sub_0802F4F4(void);
/* Wave 41, W41-D. The two `(gUnknown_0849B018->unkNN >> index) & 1` bit
 * readers, copied VERBATIM from their promoted definitions
 * (src/decomp/c_0802F460.c, c_0802F480.c) -- the definitions win. Declared now
 * because sub_0802F504 and sub_0802F534 are their first cross-file callers.
 * Both call sites pass an `int` loop counter and agbcc converts it with
 * `lsls #0x18; asrs #0x18`, which is the s8 parameter and not a source cast. */
bool8 sub_0802F460(s8);
bool8 sub_0802F480(s8);
/* Wave 42, W42-M. Copied from the promoted definition (src/decomp/c_0802F504.c)
 * -- the definition wins. Declared now because sub_0803227C is its first
 * cross-file caller. The caller narrows the result `lsls #0x18; lsrs #0x18`,
 * which looks like a u8 return but is not: PROMOTE_MODE holds the s8 pseudo
 * zero-extended, and every READ of it re-extends signed (`lsls #0x18; asrs`). */
s8 sub_0802F534(void);
void sub_08063454(struct Unk08062FB8 *, int, int, u8, s8);
int sub_08063518(struct Unk08062FB8 *);
/* Wave 56, W56-M. The BIOS multiboot entry (SVC 0x25), status `asm` and never
 * declared until now. NOT matched and not matchable -- it is the BIOS routine,
 * declared here only so its one C caller sub_08062FF4 can name it.
 *   Signature read off that call site alone: `adds r0, r7, #0; bl MultiBoot;
 * adds r5, r0, #0; cmp r5, #0`, so exactly one argument -- the same record
 * every other function in this block takes -- and a result that IS used, hence
 * `int` rather than void. The record is a MultiBootParam: sub_08062FF4 reaches
 * +0x14 (handshake_data), +0x16 (handshake_timeout), +0x18 (probe_count),
 * +0x19..0x1b (client_data), +0x1c (palette_data), +0x1d (response_bit),
 * +0x1e (client_bit), +0x28 (masterp), +0x48 (sendflag), +0x49
 * (probe_target_bit), +0x4a (check_wait) and +0x4b (server_type), which is the
 * SDK layout member for member -- so struct Unk08062FB8 IS MultiBootParam and
 * naming it here costs no cast at the call site. */
int MultiBoot(struct Unk08062FB8 *);
/* Wave 42 (W42-K). The parameter is a BYTE BUFFER and `u32` is very probably
 * the wrong spelling -- the body is sub_080308B4's twin and copies
 * `unk06[i] = a1[i]` for i = 0..127 out of it, and sub_080308B4 is promoted
 * taking `u8 *`. LEFT AS `u32` ANYWAY, deliberately: the one caller,
 * src/decomp/c_0803355C.c, is already matched and passes `proc->unk24`, whose
 * file-local struct member is `u32` and is assigned in a chain with three
 * integer siblings (`p->unk24 = p->unk26 = p->unk28 = p->unk2a = 0`), so it
 * cannot become a pointer without churn there. The question is byte-neutral --
 * the value arrives in r0 either way -- so there is no oracle to settle it and
 * the promoted caller wins. sub_08030930 casts internally. */
void sub_08030930(u32);
/* Wave 50, W50-J. Two arguments, from sub_08033C68's only call site: r0 is the
 * u8 proc->unk36 cursor and r1 is the caller's own proc pointer, which is the
 * (value, proc) order sub_080337D8 above also uses. Return value unused there,
 * and `int` is the weakest first parameter that reproduces the bare `ldrb` at
 * the call -- a `u8` parameter is byte-identical, so the width is NOT settled.
 * Arity is a floor, not a proof: anything past r1 would be invisible here. */
void sub_0803388C(int, ProcPtr);
void sub_080338C0(int);
void sub_08026900(void);
void sub_0802BFA8(void);
void sub_080351F0(void);
/* Already promoted as `void sub_08034A58(int, const char *)`
 * (src/decomp/c_08034A44.c) but never declared here; sub_08034A7C is the first
 * caller outside its own unit. */
void sub_08034A58(int, const char *);
/* Already promoted as `void sub_080328C0(u16 *)` (src/decomp/c_080328C0.c) but
 * never declared here; sub_080328EC is the first caller outside its own unit. */
void sub_080328C0(u16 *);
void sub_08030F60(int);
/* sub_08031E7C passes (0x11, -1). The -1 is `movs r1,#1; rsbs r1,r1,#0`, the
 * constant, NOT a bitfield mask -- it goes straight out as the argument with
 * no `ands` anywhere.
 * WAVE 40 (W40-B): narrowed to (u8, s8). Required by sub_080139C4, which
 * narrows both arguments at the call (`lsls #0x18; lsrs #0x18` on the first,
 * `lsls #0x18; asrs #0x18` on the second) -- neither is emitted against an
 * `int` parameter. Free at this call site: a probe of `g_narrow(0x11, -1)`
 * against `g_int(0x11, -1)` emits the same `movs r1,#1; negs r1,r1;
 * movs r0,#0x11`, so the note above still holds and c_08031E7C.c is unaffected.
 * See sub_080137AC for why the second parameter is signed. */
void sub_0801394C(u8, s8);
/* Wave 49 (W49-F) retypes the RETURN from void to int, on the body rather than
 * on a caller: sub_0802F588 has two exits, `movs r0,#1; rsbs r0,r0,#0` (-1, the
 * ring-full failure) and `adds r0,r6,#0` (the halfword count it queued), and a
 * void definition cannot produce either.  Byte-neutral at every one of its
 * eight callers, all of which discard the result -- an ignored int call and a
 * void call are the same instruction stream. */
int sub_0802F588(struct Unk0202575C *, int);
void sub_0803227C(void);
/* Coordinates: sub_08032420 feeds it `gUnknown_0849B060->unk04 * 40` and
 * `->unk06 * 40`, each emitted as `lsls #2; adds; lsls #0x13; asrs #0x10` --
 * the x5 strength reduction with the x8 folded into the s16 narrowing, which
 * is what a declared s16 parameter costs and an int parameter does not. */
void sub_08032340(ProcPtr, s16, s16);
void sub_08032950(void);
void sub_08032A00(void);
/* Mutually recursive HBlank/VCount handlers: each installs the other with
 * sub_080638D0. Declared so either can name the other's address. */
void sub_08032B84(void);
void sub_08032BA4(void);
/* THREE parameters, and the third is proved rather than guessed: sub_080397BC
 * copies its incoming proc pointer into r2 BEFORE loading either argument out
 * of it (`adds r2,r0,#0; ldr r0,[r2,#0x54]; ldr r1,[r2,#0x58]`). With only two
 * parameters agbcc keeps the base in r0 and moves the first argument in last
 * (`ldr r2,[r0,#0x54]; ldr r1,[r0,#0x58]; adds r0,r2,#0`) -- same 16 bytes,
 * five of them different. The copy lands in r2 because r2 IS the third
 * argument register. Wave 30, W30-A. */
/* Already promoted as `void sub_08044144(int)` (src/decomp/c_08044144.c) but
 * never declared here; sub_08039F58 is the first caller outside its own unit. */
void sub_08044144(int);
void sub_08044B28(int, int, ProcPtr);
/* Same third-parameter proof from sub_08039650, where it additionally forces
 * the r2/r3 split between the proc pointer and the gPlayers base. */
void sub_08080E74(int, int, ProcPtr);
/* Wave 54, W54-C. Already promoted as `void sub_08080E40(ProcPtr proc)` in
 * src/decomp/c_08080DFC.c but never declared here; sub_08080BF0 is its only
 * caller and the first outside that unit. The promoted definition is the
 * stronger witness, so this agrees with it rather than re-deriving from the
 * call site. */
void sub_08080E40(ProcPtr);
/* sub_08039820's predicate. `lsls r0,r0,#0x18` on the result before the `cmp`
 * is a narrow return being re-narrowed, so it is u8/bool8 and not int. */
u8 sub_08039850(ProcPtr);
/* Returns a literal 0 that sub_08039820 discards; the narrow return type is
 * inferred from sub_08039850, the alternative it is selected against. */
u8 sub_080398D0(ProcPtr);
/* The u16 entry narrowing `lsls #0x10; lsrs #0x10` in sub_080397F4's own
 * prologue IS the parameter declaration -- its only argument, an `ldrh` out of
 * gUnknown_085D3DD0[..].unk20[], needs no conversion. */
void sub_080397F4(u16);
/* All three parameters int, read off the promoted definition in
 * src/decomp/c_08039BB4.c (bare `adds rN,rM,#0` saves, no PROMOTE_MODE
 * narrowing). Never declared here before wave 30. */
void sub_08039BB4(int, int, int);

/* ---- Wave 30, W30-B ---- */

/* sub_0801489C IS A FOUR-BYTE `bx lr` AND NOTHING ELSE (0x0801489C, one
 * instruction plus alignment). There is no prologue, so the usual
 * read-the-callee's-narrowing route to its widths does not exist -- every field
 * of this declaration comes from its four call sites, and two of them
 * (sub_080148A0, sub_080148E0) are wave 30's:
 *   - FIVE parameters. Both callers `sub sp, #4` and `str rN, [sp]` a zero
 *     before the `bl`; nothing else in either function needs stack space.
 *   - Parameter 2 is `u16`: the value both callers compute is
 *     `unk000[i] * 2 + unk408[i]`, a 17-bit sum, and both narrow it with
 *     `lsls #0x10; lsrs #0x10` immediately before the call.
 *   - The RETURN is `u16` on the same tell -- both callers re-narrow the result
 *     with `lsls #0x10; lsrs #0x10` before returning it, which is what agbcc
 *     puts at the call of a narrow-returning callee.
 *   - Parameters 1, 3, 4 and 5 are NOT constrained. r0 is forwarded untouched
 *     from the caller's own first argument, r2/r3 are either 0 or values
 *     already zero-extended by the caller's PROMOTE_MODE, and the stack word is
 *     always 0 -- every one of those is byte-identical under `int`, `u8` or
 *     `u16`, so `int` is the weakest model rather than a reading. */
u16 sub_0801489C(int, u16, int, int, int);
/* Measures a string: it walks a NUL-terminated byte sequence, special-cases the
 * range 9..10, and accumulates gUnknown_084C36E4[c] per character. Its only
 * caller sub_08014D20 converts the result to tiles as `(w + 6) / 8` with the
 * signed `bge; adds #7; asrs #3` bias sequence, so the return is a SIGNED word
 * -- an unsigned one would be a bare `lsrs #3`. */
int sub_08014D38(const char *);
int sub_08014D20(const char *);
/* The two halfword-valued queries sub_08027844 / sub_08027A08 run on
 * gUnknown_03001FBC. Both return s16: each caller re-narrows the result with
 * `lsls #0x10; asrs #0x10` and then compares it SIGNED (`cmp #0x10; bgt`,
 * `cmp #4; bgt`). The parameter is s16 for the same reason sub_080157A4 /
 * sub_080157F4's first is -- gUnknown_03001FBC is a declared `s16` global and
 * arrives via `ldrsh`.
 *
 * CORRECTED at wave-30 integration: the return is `u16`, NOT `s16`, and the
 * caller-side reading above is a textbook cast-at-a-use error. Both bodies are
 * a single `return tbl[i].field;` compiled to `ldrh r0, [r0, #60]` flowing
 * straight into `bx lr` -- an UNSIGNED halfword load with no re-narrowing. `s16`
 * forces `ldrsh`, which needs the offset in a register (`movs r1, #60; ldrsh
 * r0, [r0, r1]`) and costs +4 bytes on a 36-byte function; measured at 77.8%.
 * So the callers' `lsls #0x10; asrs #0x10` is an explicit `(s16)` cast in the
 * CALLER's source, which is exactly what the brief's copy-then-narrow rule says
 * a narrowing after a `bl` means when the value is used afterwards. A signed
 * compare downstream constrains the caller's local, not the callee's return.
 * The discriminating evidence here is callee-side (`ldrh` vs `ldrsh`) and it
 * beats the call-site shape. */
u16 sub_08015820(s16);
u16 sub_080157D0(s16);
/* sub_0801C210's allocator and initialiser, read off sub_0801C210 (their only
 * caller) plus their own bodies. sub_0801C6E8 scans gUnknown_03000288's 16
 * slots and returns the free one or NULL, which is the value sub_0801C210
 * NULL-tests and returns. sub_0801C69C takes the handle plus sub_0801C210's
 * three arguments forwarded unchanged -- their widths are invisible at that
 * call (the values are already zero-extended by sub_0801C210's own
 * PROMOTE_MODE, so any narrowing there would be elided), so these mirror
 * sub_0801C210's declared widths rather than measuring anything. */
struct Unk0801C210 *sub_0801C6E8(int);
void sub_0801C69C(struct Unk0801C210 *, void *, u16, u8);
/* The rest of the 0x0801Cxxx animation-handle vocabulary.
 *   sub_0801C27C / sub_0801C2DC  the two halves of "advance one step":
 *     sub_0801C254 calls them in that order and re-narrows only the second's
 *     result (`lsls #0x18; lsrs #0x18`), which is what makes sub_0801C254 `u8`.
 *   sub_0801C640  installs a script: it STORES its second argument into the
 *     handle's +0x00 and derives +0x04/+0x08/+0x0c from it. `void *` because
 *     the body reads it as u16-offset table or as u32 pointers depending on
 *     the handle's +0x20 bit 1, so no single element type describes it.
 *   sub_0801C51C  a PASS-THROUGH wrapper and the arity is only visible that
 *     way: it never touches r1 at all, yet calls sub_0801C640, which reads r1
 *     and stores it. A one-parameter sub_0801C51C would be storing garbage.
 *   sub_0801C67C  re-runs sub_0801C2DC with +0x18/+0x1a forced, restoring
 *     +0x1a afterwards. void -- `pop {r4, r5}; pop {r0}`. */
/* WAVE 36 (W36-H) CORRECTION to the two lines below: sub_0801C27C takes THREE
 * parameters and returns void, and sub_0801C2DC's `u8` is right.
 *   sub_0801C27C's own body opens `adds r5,r1,#0; adds r6,r2,#0` and feeds both
 * to PutSpriteExt (r1 OR-ed with the priority word, r2 forwarded whole), so the
 * arity is a hard readout from the definition. It was invisible at its ONE
 * caller because sub_0801C254 forwards its own a2/a3 untouched -- the
 * pass-through case in the brief -- and c_0801C254.c is updated to pass them.
 * `_0801C2D2` is reached by three paths and sets no r0, so it is void; the `u8`
 * sub_0801C254 re-narrows comes from sub_0801C2DC alone. */
void sub_0801C27C(struct Unk0801C210 *, int, int);
u8 sub_0801C2DC(struct Unk0801C210 *);
/* sub_0801C3EC and sub_0801C53C: sub_0801C27C's two conditional side calls,
 * both `adds r0,r4,#0; bl` on the handle with no other argument register set
 * and no result read. */
void sub_0801C3EC(struct Unk0801C210 *);
void sub_0801C53C(struct Unk0801C210 *);
void sub_0801C640(struct Unk0801C210 *, void *);
void sub_0801C51C(struct Unk0801C210 *, void *);
void sub_0801C67C(struct Unk0801C210 *);
/* The 0x08028xxx block's callees.
 *   sub_080266DC(u8) -> u8   sub_080288D8 and sub_08028904 both narrow the
 *     argument to a byte (`lsls #0x18; lsrs #0x18`) off a u16 parameter and
 *     truth-test the result with `lsls r0,#0x18`.
 *   sub_080271CC(int) -> u8  INT, not u16, and sub_080289BC is what proves it:
 *     it passes a raw `int` parameter bare, where a u16 parameter would have
 *     put `lsls #0x10; lsrs #0x10` in front of the `bl`. Its other caller
 *     sub_08028990 passes an already-zero-extended u16 and so cannot see the
 *     difference. Result re-narrowed to u8 at both sites.
 *   sub_08028B70 returns `int`: sub_08028CF4 tests it with a BARE `cmp r0,#0`
 *     and then casts to u8 (`lsls #0x18; lsrs #0x18`) for sub_08019940's u8
 *     first parameter -- a narrow return would have re-narrowed before the
 *     compare instead.
 *   sub_08028BAC returns a byte (`lsls r0,#0x18; cmp r0,#0` at the one site).
 *   sub_08028A68 / sub_08028AEC / sub_08027118 / sub_08025EA0 are argument-free
 *     and result-discarded at every site in this block. */
/* Spelled `bool8` to agree textually with the promoted definition in
 * src/decomp/c_080266DC.c. `bool8` IS `u8` (include/gba/types.h:27), so this is
 * the same type either way and no caller changes -- but tools/proto_check.py
 * compares declaration TEXT and does not resolve typedefs, so the two spellings
 * read as a mismatch. Wave 30. */
bool8 sub_080266DC(u8);
/* Wave 41 (W41-C). Copied verbatim from the promoted definition in
 * src/decomp/c_080176A8.c so sub_08026768 can call it; it was undeclared. */
void sub_080176A8(void);
/* Wave 41 (W41-C). Copied verbatim from the promoted definition in
 * src/decomp/c_08042DE0.c so sub_08026A48 can call it; it was undeclared. */
int sub_08042DFC(int);
/* Wave 41 (W41-C). The team-colour assignment pair, both from their own
 * definitions (now matched).
 *   sub_08026A88(slot, colour) answers whether `colour` is still free among
 * gPlayers[1 .. slot-1]. Its SECOND parameter is `int`, NOT `u8`:
 * sub_08026AC0 calls it twice, and the second call passes a plain int loop
 * counter with a bare `adds r1, r4, #0` -- a `u8` parameter would have forced
 * an `lsls #0x18; lsrs #0x18` there and none is present. The callee side cannot
 * discriminate (PROMOTE_MODE zero-extends either way, and the value is only
 * compared against a `ldrb`), so the call site is the whole evidence.
 * It returns bool8: both call sites narrow the result with `lsls r0,#0x18`
 * before testing it.
 *   sub_08026AC0(slot, fallback) returns `int` and not `u8`. Its body ends on a
 * bare `adds r0, r4, #0` where r4 may hold the `fallback` argument unchanged; a
 * u8 return would have had to truncate there, and nothing does. Its one caller
 * consumes the result with `strb`, which narrows for free either way, so the
 * callee's missing truncation is the only discriminator. */
bool8 sub_08026A88(int, int);
int sub_08026AC0(int, int);
/* Wave 41 (W41-C), all three from their own definitions (now matched) and all
 * three nullary -- none reads r0-r3 before writing it.
 *   sub_0802672C returns bool8: its two exits are `movs r0,#0` and
 * `movs r0,#1` split across an unconditional `b`, the if/else-return shape.
 *   sub_08026A48 and sub_08026B28 are void -- both end `pop {r0}; bx r0` with
 * nothing setting r0 on any path. */
bool8 sub_0802672C(void);
void sub_08026A48(void);
void sub_08026B28(void);
/* Wave 45 (W45-D), both for sub_08026D68, which was the first caller of either
 * to need a declaration. Copied verbatim from the definitions rather than
 * inferred: sub_08026C6C's is src/decomp/c_08026C6C.c (a leaf whose `u8`
 * parameter is fixed by its dense jump table over ids 6..20, returning the u32
 * gPlaySt.unk28), and sub_08026CD0's is its own body, matched earlier
 * this wave -- it reads and writes only globals and ends `pop {r0}; bx r0`. */
u32 sub_08026C6C(u8);
void sub_08026CD0(void);
/* Wave 39 (W39-E). Was undeclared even though src/decomp/c_08026F28.c has been
 * promoted; sub_08028BAC is its first caller outside its own file, and an
 * implicit declaration there would default-promote both arguments to int and
 * drop the `lsls #0x10; lsrs #0x10` pair the ROM has in front of the `bl`.
 * Text copied from that definition. */
bool8 sub_08026F28(u16, u16);
u8 sub_080271CC(int);
int sub_08028B70(void);
u8 sub_08028BAC(void);
void sub_08028A68(void);
void sub_08028AEC(void);
void sub_08027118(void);
void sub_08025EA0(void);
/* sub_08028874's SECOND PARAMETER IS `int`, NOT `u8` -- corrected in wave 30
 * from the caller, which is the only place it is visible. sub_08028894 saves
 * both of its own `int` parameters with bare `adds rN, rM, #0`, builds
 * SEPARATE u16-narrowed copies for its sub_08028848 call, and then passes the
 * RAW originals to sub_08028874 with no narrowing at all. A `u8` parameter
 * there emits `lsls #0x18; lsrs #0x18` in front of that `bl`; a `u16` one emits
 * `lsls #0x10; lsrs #0x10`. Neither is in the ROM. The already-promoted
 * definition in src/decomp/c_08028874.c could not see this: its only use of the
 * value is a `strb` into a u8 struct member, which is byte-identical for every
 * width, so the definition is the weaker evidence here. Retyped and re-matched
 * with try_match. */
void sub_08028874(int, int);
void sub_08028894(int, int);
u8 sub_080288D8(u16);
u8 sub_08028904(u16);
/* Wave 38, W38-I. Was undeclared even though src/decomp/c_08028944.c has been
 * promoted since wave 32; sub_08028A68 and sub_08028AEC are the first callers
 * outside its own file. Text copied from that definition. */
bool8 sub_08028944(u16);
u8 sub_08028990(u16);
u8 sub_080289BC(int);
void sub_08028568(void);
void sub_08028168(void);
/* sub_080276D0 / sub_080276F0 are the 0 and 1 halves of one two-line body;
 * sub_08027844 / sub_08027A08 are the 0x10 and 4 halves of another. All four
 * are argument-free and end `pop {r0}` / `pop {r4}; pop {r0}`, i.e. void.
 * sub_08027FBC's second and third parameters are u16: each is used as
 * `lsls #0x10; lsrs #0xc`, which is PROMOTE_MODE's zero-extension FUSED by
 * combine with a `* 0x10` -- three instructions collapsed to two, and a shape
 * an `int` parameter cannot produce. */
void sub_080276D0(void);
void sub_080276F0(void);
void sub_08027844(void);
void sub_08027A08(void);
void sub_08027FBC(void *, u16, u16);
/* The 0x08005xxx menu block. sub_08005838 and sub_080059B4 take THREE
 * arguments and read only the third, which arrives `lsls #0x18; lsrs #0x18`,
 * i.e. a `u8` parameter under PROMOTE_MODE. The first two are dead in both
 * bodies, so `int` is the weakest model for them and not a measurement. */
void sub_08005154(void);
void sub_0800517C(void);
void sub_0800518C(void);
void sub_08005580(void);
void sub_08005838(int, int, u8);
void sub_080059B4(int, int, u8);
void sub_08005D14(void);
void sub_08005EF0(int);
void sub_080145BC(void);
/* The gUnknown_03000050 arena's allocate / free pair, one level below
 * sub_08014E44 / sub_08014ED4. Each takes the arena handle in r0 -- its
 * `!= -1` gate is in the caller, not here -- and the caller's own argument
 * untouched in r1. sub_08014DCC's result is what sub_08014E44 returns, so
 * `void *`; sub_08014ED4 discards sub_08014E68's and ends `pop {r0}`. */
void *sub_08014DCC(int, u32);
void *HeapAlloc(int, u32); /* sub_08014DCC's readable name; see
                           * src/decomp/c_08014DCC.c. */
void *HeapAllocAligned(int, u32); /* sub_08014FF8: align > 16, else defers
                                  * to sub_08014E44. See c_08014FF8.c. */
/* Wave 40 (W40-D): sub_08014E68 RETURNS int, corrected from the body. It sets
 * r0 to 1 on both refusal paths (null pointer, or a header already marked
 * free) and to 0 on the path that actually frees, and its epilogue is
 * `pop {r1}; bx r1` -- the value-returning form this header already reads that
 * way for sub_08014668. Byte-neutral at its one caller: c_08014ED4.c discards
 * the result. */
int sub_08014E68(int, void *);
/* Never declared here before wave 30, though both have matched definitions in
 * src/decomp/ -- these two lines just publish what those files already say
 * (c_08014D7C.c, c_08028848.c), so that sub_08014DA8 and sub_08028894 can call
 * them without an implicit declaration. */
int sub_08014D7C(void *, u32);
void sub_08028848(u16, u16);
/* sub_080281D8 parks its second argument in the slot's +0x18 and sub_08028190
 * is what consumes it -- `ldr r0,[r0,#0x18]`, skip if zero, hand to
 * sub_080196F4(void *). That shared displacement on a sub_080152EC slot is why
 * the parameter is `struct Unk03001470 *` and not a Proc. */
void sub_08028190(struct Unk03001470 *);
/* Already MATCHED as src/decomp/c_0804360C.c and simply never declared here;
 * sub_080276D0 / sub_080276F0 need it. Its argument is the same
 * gUnknown_08090A98 element they have just stored into
 * gUnknown_03003130.unk04 -- r0 still holds it at the `bl`. */
void sub_0804360C(int);
/* Wave 30, W30-B extension work.
 * sub_08022DD4's three parameters are s16. Its own prologue zero-extends the
 * first two (PROMOTE_MODE, which says nothing about signedness) and every use
 * inside re-narrows with `lsls #0x10; asrs #0x10`, including the third, which
 * is the switch selector. Its only caller sub_080230C4 forwards three
 * sign-extended values and nothing else in the ROM sees it.
 * sub_080230DC takes FIVE, and the last two are OUT parameters: `push` saves
 * four registers plus lr, so `ldr r0, [sp, #0x14]` is argument 5, and both it
 * and r3 are written with `strh` and never read. Its THIRD parameter is dead --
 * r2 is overwritten by a pool `ldr` before any read -- but sub_0802323C
 * materialises it, so it is declared. */
/* Wave 56, W56-H. sub_08022BB8 is declared from its ONE call site, the tail of
 * sub_08022DD4, and was not matched here. Both arms of that tail converge on
 * `asrs r0,r0,#0x10; asrs r1,r6,#0x10; asrs r2,r7,#0x10; bl sub_08022BB8`, so
 * three arguments, each an ARITHMETIC re-narrowing of the caller's own s16
 * parameter -- s16 throughout. The call is the last thing before the epilogue
 * and r0 is never read after it, so `void`. Its own body may take more; that is
 * not visible from here. */
void sub_08022BB8(s16, s16, s16);
void sub_08022DD4(s16, s16, s16);
void sub_080230C4(s16, s16, s16);
void sub_080230DC(s16, s16, s16, s16 *, s16 *);
/* Wave 33, W33-G. Steps the gUnknown_030033E4 cursor by the gUnknown_08499C7C
 * direction entry gpKeySt selects. `pop {r0}` epilogue, so void. */
void sub_0802361C(void);
/* Wave 33, W33-G, both from sub_080211DC's call sites.
 *
 * sub_08042D84 takes the gUnknown_08499594 slot NUMBER -- recovered there as
 * `e - gUnknown_08499594 + 1`, the *5/*17/*257/*65537 shift-add chain plus
 * `rsbs; asrs #8` that is agbcc's exact division by the 0x0c stride -- and that
 * slot's u8 unk00 unit-type id. Its result is added to a small bonus and only
 * then narrowed, so the return is at least `int`.
 *
 * sub_080210C8's third argument is `s16` (the sum is narrowed `lsls #0x10;
 * asrs #0x10` at the call), the fifth is sign-extended from a byte into a whole
 * stack word so it is taken wide, and the sixth is a plain count. Arguments 1,
 * 2 and 4 arrive as bare `ldrb`s. */
/* Wave 33 orchestrator: second parameter is `int`, not `u8` --
 * src/unit.c is PROMOTED and defines it that way, and a `u8`
 * declaration makes every caller narrow. Its only promoted caller
 * (src/unit.c) re-verified byte-identical after this change. */
int sub_08042D84(int, int);
/* Wave 36 (W36-L): RETYPED from `void sub_080210C8(u8, u8, s16, u8, int, int)`
 * on the CALLEE's own prologue, which is the only side that can show this.
 * Parameters 1-3 are narrowed `lsls #0x10; lsrs #0x10` at entry, not
 * `lsls #0x18; lsrs #0x18` -- halfwords, not bytes -- and each is re-extended
 * `lsls #0x10; asrs #0x10` at every use, which is PROMOTE_MODE's zero-extension
 * at entry plus the signed view at the use, i.e. `s16`. Parameter 4 (the switch
 * discriminator) carries ONE `lsls #0x10; asrs #0x10`, the combine-folded form
 * of the same thing, so it is `s16` and not `u8`. Parameter 5 is zero-extended
 * from a byte at entry and SIGN-extended at its use, so `s8`; parameter 6 is
 * zero-extended once, so `u8`.
 *
 * Byte-neutral for the one promoted caller (src/decomp/c_080211DC.c), which was
 * re-verified: arguments 1, 2 and 4 arrive as `ldrb`s and widen to s16 for
 * free, argument 5 is already that caller's own `s8`, and argument 6 is
 * `(a1 >> 6) + 1` on a u8, whose range agbcc can prove fits. */
void sub_080210C8(s16, s16, s16, s16, s8, int);
void sub_08049FB0(void);
void sub_08049FD4(void);
void sub_08049EB4(void);
void sub_08049B80(void);
/* Wave 30, W30-D. THREE parameters on CALLER-side evidence, which is the only
 * evidence there is: sub_080030BC's own body reads r0 only (r5 = r0, and r1/r2
 * are clobbered by a pool `ldr` before any read), but its sole caller
 * sub_08003088 materialises r1 and r2 from saved registers before each of its
 * four `bl`s. A body that ignores its later arguments is ordinary; the call
 * site is the stronger evidence. void because sub_080030BC tail-calls
 * sub_080032EC and sub_08003088 discards r0. */
void sub_080030BC(int, int, int);
/* Wave 30, W30-D. Five parameters, all typed from sub_080487B4's OWN prologue,
 * which narrows every one of them: r0 and r1 with `lsls #0x18; lsrs #0x18`
 * (u8), r3 and the stack argument at [sp, #0x28] with `lsls #0x10; lsrs #0x10`
 * (u16). r2 is kept whole and used as the base of `adds r1, r7, r1` after the
 * index is scaled `lsls #1`, so it is a halfword pointer -- and sub_0804931C
 * passes gBG0TilemapBuffer, which is already declared `u16 *`. The stack slot
 * is argument five: `push {r4,r5,r6,r7,lr}` + `push {r5,r6,r7}` + `sub sp,#8`
 * is exactly 0x28. Return unused at all three call sites. */
void sub_080487B4(u8, u8, u16 *, u16, u16);
/* Wave 30, W30-D. Three callees of the 0x08075/0x08087 blocks that had no
 * declaration. sub_08085F40 and sub_0803D960 are already PROMOTED
 * (src/decomp/c_08085F40.c, src/decomp/c_0803D960.c) and these two lines just
 * publish the signatures those definitions already have -- sub_0803D960's
 * parameter is the Proc_StartBlocking parent it forwards.
 * sub_08075904 takes an index it scales by 0x30 (`lsls #1; adds; lsls #4`,
 * i.e. a 3<<4-byte record) into gUnknown_08615194 + 0xc, and returns: one arm
 * is a bare `movs r0, #0`. Its two callers both discard the result, so `int`
 * is the widest thing the body supports and nothing narrows it. */
int sub_08075904(int);
void sub_080879A0(void);
void sub_08085F40(void);
void sub_0803D960(ProcPtr);

/* ---- Wave 31, W31-A ---------------------------------------------------- */

/* Read off their own prologues and epilogues. Each is called from this wave's
 * batch with no argument register set up and its result discarded, so the CALL
 * SITES prove nothing about arity -- the void-ness below comes from the
 * callees' `pop {r0}; bx r0` (or bare `bx lr`) and, where the body was read,
 * from no argument register being live on entry. */
void sub_0802C2B4(void);   /* pop {r0}; bx r0; body reads no argument register */
void sub_0803B3F8(void);   /* one-call forwarder to sub_0806FD98 */
void sub_0803B408(void);   /* one-call forwarder to sub_0807046C */
void sub_08011FF0(void);   /* 424-byte DMA-queue drain; nothing read from r0-r3 */
void sub_0802E920(void);   /* matched by this wave */
void sub_0802E960(void);   /* type fixed by contract: it is handed to
                            * sub_080366C4, which takes `void (*)(void)` */

/* ARITY NOT PROVEN. sub_08000664 and sub_08000DC0 call these four with nothing
 * in r0-r3 and discard the result, so `void (void)` is the weakest declaration
 * that fits THIS caller and nothing more. Re-derive from the callee body before
 * relying on it. */
void sub_0800057C(void);
void sub_08002EC8(void);
void sub_080035C8(void);
void sub_08003640(void);

/* u8 return, not int: sub_0802E6C0 and sub_0802E278 re-narrow the result with
 * `lsls #0x18` before testing it, which agbcc emits only for a sub-word return
 * type. sub_08034F60's body is one `ldrb`; sub_0802DBF8's returns 0 or 1. */
u8 sub_08034F60(void);
u8 sub_0802DBF8(void);

/* Defined in src/decomp/c_0802E4B4.c -- declared here for sub_0802E6C0.
 * RETYPED u16 -> s16, wave 36 (W36-M). sub_0802DCB4 reaches
 * gUnknown_030033E4's two halves as s16 OBJECTS (`movs rI,#0; ldrsh`) at this
 * call site, and a u16 PARAMETER makes that impossible: agbcc's call-site
 * promotion follows the parameter's signedness, so a u16 parameter emits
 * `ldrh` no matter how the argument is spelled -- the sign-extending load only
 * survives into a signed parameter. Byte-neutral on both the definition
 * (PROMOTE_MODE zero-extends s16 and u16 alike at entry, and the body's first
 * act is `sx = x` into an s16 local) and on the other caller sub_0802E698,
 * which passes u8 fields with no narrowing either way; both re-verified. */
void sub_0802E4B4(s16, s16);

/* SIGNED parameters, and this is a RETYPE of what src/decomp/c_08022AAC.c
 * carried. Wave 31, W31-A: that definition guessed `u16` from its own body,
 * which cannot tell -- agbcc's PROMOTE_MODE narrows a parameter of EITHER
 * signedness with `lsls #0x10; lsrs #0x10`, and the only uses are `strh` and
 * `<< 4`, both sign-blind. The first two C callers (sub_0802E698 and
 * sub_0802E6F8, this wave) DO discriminate: they read
 * gUnknown_03003100.spos.unk00/.unk02 with `ldrsh` and pass them straight
 * through. A u16 parameter would have forced a zero-extending `lsls/lsrs` pair
 * at each call site that the ROM does not have. Re-verified byte-for-byte
 * against sub_08022AAC after the change. */
void sub_08022AAC(s16, s16);

/* The BIOS IntrWait(1, 1) stub: `movs r2,#0; svc #5; bx lr`. */
void VBlankIntrWait(void);

/* Eight parameters, all read off its own prologue: r0 is untouched (it is
 * `strh`-ed through at +0/+2/+0x40/+0x42, a 2x2 halfword tilemap block, so
 * `u16 *`), r1 `lsls/lsrs #0x18`, r2 `#0x10`, r3 `#0x18`, then the four stack
 * arguments at [sp,#0x34..0x40] narrow to u8, u16, u16, u8 in that order. */
void sub_0802216C(u16 *, u8, u16, u8, u8, u16, u16, u8);

/* s16 return: every exit is an `ldrsh`, a `-1`, or an `asrs #0x10`, and
 * sub_08007DB0 re-narrows the result with `lsls #0x10; asrs #0x10` before
 * testing it >= 0 -- the signed pair, which only a signed narrow return type
 * produces. */
s16 sub_08007DD0(int, int);

/* Both matched by this wave. sub_080016D0 RETURNS sub_08001704's result: its
 * epilogue is `pop {r1}; bx r1`, which leaves r0 alone, where a void function
 * pops the return address into r0 itself. */
int sub_080016D0(int, int);
void sub_08007D70(int, int);

/* ---- Wave 31, W31-A, second batch ------------------------------------- */

/* One parameter, and it is NOT visible from either call site -- sub_080293A0
 * forwards its own r0 without touching it. The callee's body is what settles
 * it: `adds r1, r0, #0; ldr r0, =gUnknown_08499FEC; bl Proc_StartBlocking`, so
 * the incoming r0 is the parent proc. */
void sub_08028ED0(ProcPtr);

/* `ldr r0, =gUnknown_030040A8; ldr r0, [r0]; bx lr` -- a word-wide getter.
 * NOT u16: sub_08029234 `strh`s the result with no re-narrowing, which a
 * sub-word return type would have forced. `u32` rather than `int` to agree
 * with src/decomp/c_0804138C.c, promoted in this same wave -- the width is what
 * the caller proves and the signedness is unconstrained either way. */
u32 sub_08041398(void);

/* Two int parameters: neither is narrowed on entry, and both are scaled `<< 4`
 * before being stored as words. sub_08029C28 passes two `ldrh` values, which a
 * wider parameter takes for free. */
void sub_0802723C(int, int);

/* Read off its own prologue: r0 and r1 arrive untouched (they are sign-extended
 * at the sub_08022AAC call, which is a cast at a use), and r2 narrows
 * `lsls/lsrs #0x18`. `pop {r0}` makes it void. sub_08029088 and sub_0802909C
 * are the same call with 0 and 1 for that last byte. */
void sub_080290B0(int, int, u8);

/* One struct-pointer parameter -- sub_08030C98 calls it with
 * `&gUnknown_030040C0` still sitting in r0 from the two stores above the call,
 * and the body reads +0, +6 and +8 through it. */
void sub_0802EA5C(struct Unk030040C0 *);

/* Defined in src/decomp/c_0802EAFC.c; declared here for sub_08030C98. */
void sub_0802EAFC(void);

/* Returns a value (`pop {r1}; bx r1`) and reads no argument register. The
 * result is either -1 or gUnknown_0300055C's word, so `int`; sub_08030D1C
 * `strb`s it into a volatile s8 with no narrowing, which is the truncating
 * store and not evidence of a narrow return. */
int sub_0802EB28(void);

/* ARITY NOT PROVEN beyond "reads no argument register before writing it": its
 * first instructions load r0 from a pool word. 1,556 bytes, so only the
 * prologue was read. */
void sub_08046030(void);

/* ARITY AND WIDTHS NOT PROVEN -- every one of these is declared from its CALL
 * SITE only, and the call sites in this batch cannot see past what they load.
 * Each declaration below is the weakest one that reproduces the ROM bytes at
 * the caller; re-derive from the callee body before relying on any of them.
 *
 * The three void ones are called with nothing in r0-r3 and their results
 * discarded, so `void (void)` is byte-identical to any other shape here.
 * sub_08001DAC is 1,260 bytes and was deliberately NOT read. */
void sub_08001DAC(void);
void sub_08002AB0(void);
void sub_08002C38(void);
void sub_0801F1EC(int, int);      /* (0xAA, 0xAA or 0xAB) from sub_08002EF8 */
/* RETYPED `int` -> `u16` in wave 35 (W35-C), from the CALLEE's own body, which
 * is the evidence this declaration never had -- the note above says each of
 * these is "the weakest one that reproduces the ROM bytes at the caller" and
 * asks for exactly this re-derivation. sub_080247A4 opens
 * `lsls r0,r0,#0x10; lsrs r5,r0,#0x10`, the u16-parameter tell: the promoted
 * entry value IS the truncated one, it feeds the `muls #0x5c` row index
 * directly, and the shifted form left in r0 is reused for the 0xb4..0xbf range
 * test (`adds r0,r0,#0xFF4C0000; lsrs #0x10`) with the constant pre-shifted --
 * which only happens when the parameter's own mode is HImode. Caller-neutral:
 * the one caller passes a zero-extended `ldrb`, which needs no narrowing for
 * either type, so no promoted file changes. */
void sub_080247A4(u16);           /* one `ldrb` from a byte table */
void sub_080860DC(ProcPtr);       /* sub_0808603C hands it the same proc it
                                   * gives sub_08086688, which IS declared
                                   * ProcPtr -- that is the whole argument */
/* Wave 43 (W43-D) CONFIRMED `int`, do not "fix" it to u16. The body copies the
 * second argument with a bare `adds r6, r1, #0` and never narrows it: a u16
 * parameter would arrive with PROMOTE_MODE's `lsls #0x10; lsrs #0x10` instead,
 * which is +2 bytes and was measured as the function's only diff.
 * (gBG1TilemapBuffer, 0x6200) */
void sub_08037A20(u16 *, int);
void sub_080620C0(void);
void sub_080620FC(int, int);      /* (0,1) (1,6) (2,5) from sub_0806209C */
/* Matched in src/decomp/c_0806209C.c as `void sub_0806209C(void)`; this
 * declaration only makes it visible to its one caller, sub_08062038. */
void sub_0806209C(void);
/* Wave 48 (W48-D).  `pop {r0}; bx r0` in the epilogue, so void: the return
 * register is destroyed restoring lr.  Rebuilds the gUnknown_03003F20 cell
 * list from gUnknown_084995A0 and takes nothing. */
void sub_08062330(void);
/* Wave 48 (W48-D), MATCHED.  Two OUT-parameters, both `int *` from the `str`
 * at each -- not u8 *, though the values stored are u8 map coordinates.
 * Returns 1 when it found a cell and 0 when it did not (`movs r0,#1` / `movs
 * r0,#0` across a `b`, the two-arm form). */
int sub_080623C4(int *, int *);
/* Wave 48 (W48-D).  void: `pop {r0}; bx r0`, and it reads no argument
 * register before writing it. */
void sub_08062474(void);
/* Wave 48 (W48-D), from the CALL SITE in sub_08062474 only -- not matched, so
 * this is the weakest contract that fits, NOT ground truth.  Two arguments:
 * r0 is a small literal 1..4 (`movs r0,#N`) and r1 is the u8
 * gUnknown_085D5ABC[..].unk1d passed with no narrowing in front of it, which
 * an int parameter and a u8 parameter both explain.  Result unused at the
 * only call site, and there are four of them. */
/* Wave 50 (W50-A), RETYPED from (int, int).  Both parameters are narrowed by
 * PROMOTE_MODE at entry before anything else happens -- `lsls r0,#0x10;
 * lsrs r0,#0x10; str r0,[sp,#4]` and `lsls r1,#0x18; lsrs r1,#0x18;
 * str r1,[sp,#8]` -- which is the sub_080247A4 readout above: the spilled
 * value IS the truncated one, so the parameters' own modes are HImode and
 * QImode.  Caller-neutral: the one caller (sub_08062474) is still assembly. */
void sub_08062560(u16, u8);
/* Wave 50 (W50-A), RETYPED from int.  Same readout: `lsls r0,#0x18;
 * lsrs r0,#0x18; str r0,[sp,#4]` at entry and the spilled value is the
 * truncated one.  Caller-neutral: src/decomp/c_08062C7C.c passes its own u8
 * parameter, which needs no narrowing for either type. */
void sub_080627F4(u8);
void sub_08062AE4(void);
/* Wave 50 (W50-A).  void: `pop {r0}; bx r0` destroys the return register, and
 * it reads no argument register before writing it. */
void sub_08062C94(void);
/* Wave 50 (W50-A).  Types copied from the already-promoted definition in
 * src/decomp/c_08062730.c -- NOT re-derived.  sub_08062560's call site hands it
 * `gUnknown_030040D8` (declared `struct Unk030040D8 *`) as the first argument
 * and casts; the result is tested `lsls #0x18` at that site, so the caller
 * narrows the `int` return to u8 itself. */
int sub_08062730(struct Unit *, struct Unit *);
void sub_08077620(int, int);      /* (0, 0xA8 - gUnknown_0300064C), twice */

/* Wave 54 (W54-F). sub_08077140 already has a promoted body in
 * src/decomp/c_08077140.c as `void sub_08077140(u16 *dest, u16 base, int pal)`
 * and this declaration only publishes it. sub_08077180 is its twin: the same
 * call in sub_08077304's other arm, `(gUnknown_08551A00 + 0x41, 0x46, 1)`
 * against sub_08077140's `(gUnknown_08551A00 + 1, 0x46, 1)`, so the same three
 * parameter types.
 * sub_080772B8 ALSO already has a promoted body, in src/decomp/c_0807728C.c,
 * and that definition wins: its parameter is `struct Unk080772B8 *`, a tag
 * defined inside that file. Declared here through an incomplete tag so the
 * declaration cannot disagree with it -- the caller (sub_08077304) passes
 * `gBG0TilemapBuffer + 0x280`, which is where that struct's +0x92 halfword
 * grid lives. Do NOT retype this to `u16 *` from the call site. */
struct Unk080772B8;
void sub_08077140(u16 *, u16, int);
void sub_08077180(u16 *, u16, int);
void sub_080772B8(struct Unk080772B8 *);
void sub_080758BC(int, int, int, ProcPtr);
                                  /* FOUR arguments: sub_08077E9C sets r0, r1
                                   * and r2 and leaves its own r0 sitting in r3
                                   * from `adds r3, r0, #0` at the top -- the
                                   * fourth argument is invisible except as
                                   * that copy */
/* CORRECTION, wave 32 (W32-A): it RETURNS a value. Wave 31 declared this
 * `void` from the call site, where sub_08077790 discards the result -- which is
 * no evidence either way. The body settles it: `adds r0, r7, #0` immediately
 * before an epilogue that pops the return address into r1 (`pop {r1}; bx r1`)
 * and leaves r0 intact. A void function pops into r0 itself and clobbers it.
 * The value is the seventh argument narrowed to a halfword, so it is already in
 * u16 range and nothing re-narrows it; `int` is the weakest type that fits and
 * the width is otherwise unconstrained.
 * The eight PARAMETER types are unchanged and were re-checked: the prologue
 * copies every one with a bare `adds` and each narrowing is at its use. Making
 * the last three `u16` was TRIED and is 8 bytes WORSE -- agbcc then needs a
 * third high register (sl) and pushes it, because a narrow parameter's
 * PROMOTE_MODE copy lives from the prologue to its last use. `int` with the
 * casts at the uses is right. */
int sub_08077214(u16 *, int, int, int, int, int, int, int);
/* Wave 32, W32-A: sub_08077620's other callee, still assembly. Four `int`s --
 * every one arrives as a bare `adds rN, rM, #0` with no PROMOTE_MODE narrowing
 * -- and void: `pop {r0}; bx r0`. It masks the first two into OBJ x/y fields
 * and forwards to PutSpriteExt with its own gUnknown_084A07DA object list. */
void sub_0804402C(int, int, int, int);

/* Matched by this wave. sub_080771C0 and sub_080771F0 hand each other's ADDRESS
 * to sub_080638D0, so each needs the other declared; sub_08002EF8 is called by
 * sub_08002E3C in the same block. */
void sub_080771C0(void);
void sub_080771F0(void);
void sub_08002EF8(void);
                                  /* EIGHT, four of them on the stack. Widths
                                   * are wide open: the ROM passes literals and
                                   * one `ldrh`, and every narrower parameter
                                   * type accepts those for free. */

/* ---- wave 33 (W33-C): the 0x08063 / 0x08064 address-locality blocks ---- */

/* Splits a value into three decimal digits written back through three separate
 * u8 pointers -- ones, tens, hundreds in that argument order reversed (the
 * ones pointer is the LAST parameter). `strb` at every store site pins u8; the
 * value is used whole by __divsi3/__modsi3 with no narrowing, so `int`.
 * 0xFF is its "blank this digit" code and 10 is what all three get for a zero
 * input. Matched in wave 33, src/decomp/c_08063A58.c. */
void sub_08063A58(int, u8 *, u8 *, u8 *);
/* sub_08063B50's only callee and its only caller. Five arguments, the fifth
 * pushed with `str r4, [sp]`. The first three arrive as full words straight out
 * of `ldr` on +0x24/+0x28/+0x2c of struct Unk8063BE0; the fourth is an `ldrb`
 * and the fifth an `ldrh`, both of which widen into `int` for free, so `int`
 * five times is the weakest model that fits every site. Its own body (272 B,
 * Div/SetObjAffine/gSinLut) is still asm and may narrow these later. */
void sub_08063CCC(int, int, int, int, int);

/* ---- wave 46 (W46-B): the 0x08064 block -------------------------------- */

/* CORRECTION TO THE WAVE-46 BRIEF, and it cuts BOTH ways -- record both halves,
 * because the first half alone is the dangerous reading.
 *
 * The brief said block 0x08064000 has ZERO undeclared callees and that no new
 * prototype would be needed. Grepping include/ for the five names returns
 * nothing, which reads exactly like "undeclared" -- and acting on that reading
 * is how this nearly broke the build. All five are already DEFINED, and matched,
 * in src/decomp/ (c_08063DDC.c, c_08063E28.c, c_08063FEC.c, c_08064034.c); they
 * are simply not declared in any shared header. So the brief was right that
 * nothing needed deriving and wrong that nothing needed writing.
 *
 * GREP src/decomp/ AND NOT ONLY include/. `trymatch` compiles ONE unit and
 * cannot see a prototype that disagrees with a promoted definition in another
 * unit, so a wrong signature here passes every per-function check and breaks
 * only the SPLIT build -- the wave-14 failure, repeated. The first draft of
 * this block declared all five as `int *` from the assembly alone and matched
 * sub_08064214 byte-for-byte with it.
 *
 * sub_08063FEC, sub_08064034 and sub_0806407C each fill one 0x30-byte matrix
 * (twelve words) from an angle. sub_08064214 gives them three consecutive 0x30
 * slots of its own frame and an `(s16)` cast of each of its three parameters --
 * `adds rN, r0, #0` then `lsls #0x10; asrs #0x10`, which is copy-then-narrow,
 * so its OWN parameters are int and the cast is at the use. The angle argument
 * is declared int for the same reason sub_0801BA4C's is: those two build
 * rotation matrices by handing it straight to sub_0801BA4C / sub_0801BAA8.
 *
 * sub_08063E28(a, b, out) composes two matrices. sub_08064214 calls it twice
 * with out == b, which is why the destination is a third argument and not a
 * return value.
 *
 * sub_08063DDC(v, m, dst) transforms one three-word vector by the rotation part
 * of one matrix; sub_08064214 passes a gUnknown_0202F140 entry's two vectors
 * and the composed matrix.
 *
 * EVERY SIGNATURE BELOW IS COPIED FROM THE PROMOTED DEFINITION, not derived
 * from the call site -- the definitions win. Note that sub_08064034 and
 * sub_0806407C take their own per-function matrix tags rather than
 * `struct Mtx43`: c_08064034.c re-declared the identical 4x3 layout under
 * separate tags so promote.py could merge that draft into a unit that already
 * defined Mtx43, and a declaration here has to agree with what is written
 * there. The angle is `s16` at all three matrix builders for the same reason. */
/* sub_080633E4 is likewise DEFINED in src/decomp/c_080633E4.c and merely never
 * declared in a shared header -- see the correction above. Its signature is
 * that file's, `(struct Unk08062FB8 *, u16)` returning int, NOT the
 * `(..., int)` the call site alone suggests. sub_08063528 tail-returns its
 * result from three separate sites (jump.c cross-jumps the three into one
 * `bl`). The second argument arrives either as a bare `movs r1, #0` or as a
 * `ldrh` of the link record's +0x00, which is a (u16) cast at the call and not
 * a narrow
 * parameter -- `int` takes both without a narrowing anywhere. */
int sub_080633E4(struct Unk08062FB8 *, u16);

/* THE TAGS MUST BE FORWARD-DECLARED AT FILE SCOPE FIRST. A tag that appears for
 * the first time inside a prototype has PROTOTYPE scope in C89, so the later
 * file-scope definition in src/decomp/ would be a DIFFERENT type and every one
 * of these would become an incompatible redeclaration. These four lines are
 * what make the declarations agree with the definitions. */
struct Vec3;
struct Mtx43;
struct Unk64034Mtx;
struct Unk6407CMtx;

void sub_08063FEC(struct Mtx43 *, s16);
void sub_08064034(struct Unk64034Mtx *, s16);
void sub_0806407C(struct Unk6407CMtx *, s16);
void sub_08063E28(struct Mtx43 *, struct Mtx43 *, struct Mtx43 *);
void sub_08063DDC(struct Vec3 *, struct Mtx43 *, struct Vec3 *);

/* ---- wave 33 (W33-C): the 0x08085 address-locality block ---- */

/* The twenty-row unit-info table builder sub_08085244 calls. TWO parameters,
 * and the first one is UNUSED inside the body -- its prologue is `mov sb, r1`
 * with r0 written (`movs r0,#0`) before it is ever read. The call site is what
 * proves the arity: sub_08085244 sets up r0 (its own `s16 *`) and r1
 * (`p[0x33]`, the army index every one of the callees below takes) immediately
 * before the `bl`. r1 is used unnarrowed as a gPlayers index and as
 * sub_08085410 / sub_08085638 / sub_080856A0's first argument, so `int`.
 * `pop {r4,r5,r6,r7}; pop {r0}` makes it void. */
void sub_08085708(s16 *, int);
/* Wave 55 (W55-D). Both already have PROMOTED, byte-matching bodies in
 * src/decomp and neither had ever been declared, because sub_08085708 is their
 * only caller and it was still asm. Types copied from the definitions, which
 * are the stronger witness: src/decomp/c_080859A0.c and src/decomp/c_08085410.c.
 * sub_080859A0's first two parameters really are narrow -- sub_08085708 emits
 * an `lsls #16; lsrs #16` pair on each of them at the call and on nothing else
 * it passes, which is the re-narrowing a u16 parameter forces and an `int` one
 * cannot produce. */
void sub_080859A0(u16, u16, int, int, int, int);
int sub_08085410(int, int);
/* Same wave, same reason: promoted and matching in src/decomp/c_08085638.c,
 * never declared because sub_08085708 is the only caller. */
int sub_08085638(int, int);
int sub_080856A0(int, int);
/* Already MATCHED (32 bytes, never promoted or declared): returns
 * `&gUnknown_0810E6E0[(gPlayers[i].unk1a - 1) * 0x20]`, the stride-0x20
 * palette row that table's note describes. `u16 *` rather than `u8 *` because
 * its only caller, sub_08085950, hands the result straight to
 * ApplyPaletteExt's `u16 *` first parameter with no arithmetic in between; the
 * index is used as a full word (`lsls r1,r0,#4; subs r1,r1,r0`) so `int`. */
/* Wave 33 orchestrator: `u8 *`, not `u16 *` -- src/decomp/c_080261C8.c is
 * PROMOTED and computes `u8 *base + (tbl[i].unk1a - 1) * 0x20` in bytes. Its one
 * caller passes the result straight to ApplyPaletteExt with no indexing, so the
 * element width is byte-neutral there and the definition is the only evidence. */
u8 *sub_080261C8(int);

/* ---- wave 33 (W33-C): the 0x0804C address-locality block ---- */

/* THREE parameters, all narrowed `lsls #0x10; lsrs #0x10` in its own prologue.
 * The third is s16 rather than u16: it is spilled whole and then re-widened
 * with `lsls #0x10; asrs #0x10` before being compared against -1, which is the
 * sign-extend-at-the-use half of a declared s16 (PROMOTE_MODE zero-extends both
 * at entry, so the prologue does not discriminate). Every one of its five
 * callers passes gUnknown_03001FBC there, which is s16 too. Void epilogue. */
void sub_0804CA98(u16, u16, s16);
/* Already DEFINED and matched in src/decomp/c_0804BDD8.c but never declared;
 * sub_0804CA44 is its first C caller. Published verbatim from that file. */
int sub_0804BECC(u16, u16, s16);

/* ---- wave 33 (W33-C): the 0x08050 address-locality block ---- */

/* The song-stop half of the gUnknown_0824238C / gUnknown_08242308 pair those
 * globals' note describes; sub_080504A8 calls it twice with bare `movs`
 * immediates (0x3b, 0x3c). u16 song index, matching the `lsls #0x10; lsrs #0xd`
 * stride-8 scaling that note records for the same table. */
void sub_08070544(u16);

/* ---- wave 33 (W33-F): blocks 0x08002 and 0x08057 ---- */

/* sub_08002964 hands it that function's fourth argument raw (`adds r0,r5,#0`)
 * and discards the result, so `int` is the weakest fit and nothing in this
 * wave can see the return type. */
/* Wave 33 orchestrator: returns `int`, not `void`. W33-A matched the body
 * byte-for-byte as `int sub_08001230(int)`; W33-F declared it `void` from its
 * call site in sub_08002964, which discards the result and so cannot see the
 * return type at all. The definition wins; the call site is byte-identical
 * either way. This was the wave's one flagged cross-agent collision. */
int sub_08001230(int);
/* The two OBJ-graphics lookups behind sub_08002964 and sub_080029F4: the
 * returned word becomes sub_08011E54's `void *` source with no arithmetic and
 * no narrowing in between, which is the only evidence either way. */
/* Wave 33 orchestrator: `const u8 *`, not `void *` -- src/decomp/c_0802A838.c
 * is PROMOTED and defines both this way. Weakest-model applies to types nobody
 * has named; here the definition names one and proto_check rejects the
 * disagreement. Callers take the result as `const u8 *` or cast. */
const u8 *sub_0802A85C(int);
const u8 *sub_0802A838(int);
/* sub_08002DEC calls it four times and feeds each result straight into
 * ApplyPaletteExt's `u16 *` first parameter, so that fixes the return type the
 * same way sub_08082660's does. Both arguments are bare `movs` immediates at
 * every site, so their widths are not visible -- `int` is the weakest fit. */
/* Wave 33 orchestrator: `const u8 *`, not `u16 *` -- src/decomp/c_0802A880.c is
 * PROMOTED and defines it that way. */
const u8 *sub_0802A8AC(int, int);
/* sub_08057048 fills a six-halfword stack record and passes its address; the
 * record's layout is described in that function's own file. */
void sub_080570C4(void *);
/* Wave 48, W48-G. PROMOTED in src/decomp/c_08057110.c with exactly this
 * signature: (chr, offset, pal, flip) -> gUnknown_08551A00[offset]. */
void sub_08057110(u16, u16, u16, u16);
/* Wave 48, W48-G. The gUnknown_08551A04 twin of sub_08057110, PROMOTED in
 * src/decomp/c_0805701C.c with exactly this signature. */
void sub_0805701C(u16, u16, u16, u16);
/* Wave 48, W48-G. The four sprite-row painters sub_080579B8 fans out to, all
 * called with the identical (u16 *dst, int idx, &pos) triple. sub_080576D4 and
 * sub_08057A24 are PROMOTED with exactly this signature; the third argument's
 * pointee is the two-halfword {u16 x; u16 y;} record those files spell locally,
 * so the tag is only forward-declared here and each .c completes it.
 * sub_080577E4/sub_08057860 are not matched yet -- their third argument is
 * assumed the same record because sub_080579B8 hands all four the same stack
 * slot, which is evidence about the CALL, not about their bodies. */
struct Unk8057Pos;
void sub_080576D4(u16 *, int, struct Unk8057Pos *);
void sub_0805772C(u16 *, int, struct Unk8057Pos *);
void sub_080577E4(u16 *, int, struct Unk8057Pos *);
void sub_08057860(u16 *, int, struct Unk8057Pos *);
void sub_08057A24(u16 *, int, struct Unk8057Pos *);
/* Wave 48, W48-G. The two per-side fan-outs over the four painters above, both
 * MATCHED with this signature. sub_08057AE8 hands each `gBG0TilemapBuffer`,
 * which is already a `u16 *`. */
void sub_080579B8(u16 *);
void sub_08057A80(u16 *);
/* Allocates a slot or fails: sub_0801D6E8 calls it when its own fifth argument
 * is above 0x1f, compares the result against -1 and RETURNS IT UNCHANGED on
 * that path (`adds r0,r3,#0`, not a rematerialised -1), which is what makes the
 * return an `int` rather than something narrower. */
int sub_0801E13C(void);


/* ---- wave 34 (W34-D): block 0x08027 ---- */
/* The two halves of one clamp pair on gUnknown_03003130.unk04, and they are
 * their own block's undeclared callees. Both take nothing and return nothing:
 * zero argument registers are read before being written and both end on the
 * `pop {r0}; bx r0` void epilogue. sub_080275B4 walks unk04 down toward 3 and
 * sub_08027608 walks it up toward 0xad -- the two bounds gUnknown_08090A98
 * holds as its [0] and [1]. */
void sub_080275B4(void);
void sub_08027608(void);
/* Both are `void (void)` proc/queue callbacks whose ADDRESS is taken by
 * sub_0802776C's switch and handed to sub_0801F024. */
void sub_08027658(void);
void sub_08027710(void);
/* sub_08027984's argument arrives `movs r1,#0; ldrsh r0,[r5,r1]` off the s16
 * gUnknown_03001FBC and its result is discarded. `int` and NOT `s16`: the
 * ldrsh is the global's own width, so the call site cannot see the parameter's
 * -- and src/decomp/c_08016944.c already promotes the body as `int`. Declared
 * s16 first this wave and proto_check caught it; sub_08027984 is byte-identical
 * either way, which is exactly why the definition has to be the tie-break. */
void sub_08016974(int);


/* ---- wave 34 (W34-D): block 0x08025 ---- */
/* THREE parameters, not one, and src/decomp/c_08026588.c was corrected to
 * match. Its promoted body uses only the first, which is why the one-parameter
 * form survived -- an unused register parameter costs no instructions, so the
 * definition is byte-identical either way and had no oracle. sub_080250E8 is
 * the differently-shaped caller that finally shows it: it sets r0, r1 AND r2
 * before each of its two `bl`s, and r0/r1 are each the result of a full
 * twenty-instruction pointer-difference-by-12 divide. Nobody computes that to
 * pass a dead argument. Both are army numbers -- `(p - gUnknown_08499594) / 64
 * + 1` truncated to u8 -- and r2 is a `ldrb` of the record's unk00. */
/* Takes the record and a displayed-HP value (`Div(hp - 1, 10) + 1`, or 0 when
 * the 7-bit unk04_0 field is empty). The second parameter is `int`: sub_08025D60
 * passes that expression with no narrowing at all, while sub_0802505C's u16
 * local is narrowed on its own account before the call. Result unused at both
 * sites. */
/* Promoted in src/decomp/c_08025D20.c; declared here so sub_08025D60 (its
 * caller, a different unit) can see them. */
void sub_08025D20(int);
void sub_08025D40(int);
void sub_08026588(u8, u8, u8);
/* Promoted in src/unit.c as `int sub_08042C68(int, int)`; declared
 * here so sub_080253B0 can see it. */
int sub_08042C68(int, int);
void sub_08025B24(struct Unit *, int);
/* One record pointer, result unused (sub_08025D60). */
void sub_0802A5C4(struct Unit *);
/* Wave 39, W39-F. The 0x0802A3FC unit-scan block.
 *
 * sub_0802A304 is a sub_0802A38C callback, the sibling of sub_0802A2E4, and
 * both are typed `void *` because the object -- a struct Unit record --
 * is reached through a file-local view: sub_0802A304 indexes unk07/unk08 as a
 * 2-element cargo array, which the shared struct cannot express and which is
 * not worth reshaping it for (see the note on struct Unit.unk07).
 *
 * sub_0802A38C and sub_0802A258 are DELIBERATELY NOT DECLARED HERE.
 * src/decomp/c_0802A38C.c defines sub_0802A38C with a file-local `struct
 * Unk2A38C *` parameter, so any declaration in this header is a conflicting
 * type for that unit; the callers declare it themselves as
 * `bool8 sub_0802A38C(void *, int (*)(void *))`, which is the weakest
 * spelling that agrees with both call sites. The work/ draft for sub_0802A258
 * is in the same position.
 *
 * ONE FINDING TO CARRY FORWARD: work/sub_0802A258 declares itself `bool8`,
 * and that is REFUTED by its only call site. sub_0802A3FC truth-tests the
 * result with `lsls r0, #0x10`, which is a 16-bit return; a bool8/u8 return
 * gives `lsls #0x18`. The callee's own body returns only the literals 0 and 1
 * and so cannot tell the two apart -- exactly the situation the note on
 * sub_0802A1E4 above describes for that function. u16 vs s16 is still open;
 * nothing narrows or sign-extends it. Whoever promotes sub_0802A258 should
 * make it 16-bit. */
int sub_0802A2E4(void *); /* promoted in src/decomp/c_0802A2E4.c */
int sub_0802A304(void *);
void sub_0802A3FC(void);
void sub_0802A6B0(void);
/* Promoted in src/decomp/c_08025B28.c; declared here so other units can call
 * it. */
void sub_08025B28(u16, u32);
/* The gUnknown_085D5ABC[type].unk14 permission-table predicates. sub_08025F74
 * takes the record directly and sub_08025EF0 the two unit ids -- same three
 * tests, and sub_08025EF0 additionally requires the two ids to share an army
 * (`(a & 0xc0) == (b & 0xc0)`). Both return a byte. */
bool8 sub_08025F74(struct Unit *, u8);
bool8 sub_08025EF0(int, int);
bool8 sub_08025FC0(struct Unit *, struct Unit *);
bool8 sub_080253B0(struct Unit *);

/* Wave 34, W34-K -- block 0x08069 / 0x0806B. Every signature below is read off
 * the CALL SITE, per wave 30; none of these had a declaration anywhere.
 *
 * sub_080679D8 takes TEN arguments: four in registers plus six stack slots,
 * which is what the `sub sp, #0x18` in sub_08069154 is reserving. All ten are
 * plain words at the one call site (1, -1, 0x170, 0x88, -0x3800, 0, 0xc0,
 * 0x100, 0xc, proc), and the second and fifth are negative, so `int`. */
void sub_080679D8(int, int, int, int, int, int, int, int, int, ProcPtr);
/* Wave 51, W51-H. The 0x08067xxx proc helpers the three 0x08069 timeline
 * functions drive. ARITY READ OFF EACH CALLEE'S OWN PROLOGUE, not off a call
 * site: sub_08067BD0 and sub_08067D04 each copy r0/r1/r2 into callee-saved
 * registers and hand r3 to Proc_Start as the parent, which is four; the four
 * `void` ones write no argument register before their first `bl`.
 *   sub_08067BD0's second parameter is SIGNED -- sub_080691BC passes -1 there
 * (`movs r1,#1; rsbs r1,r1,#0`) while its siblings pass 1 -- and the callee
 * feeds it to `lsls r0, r6, #7`. The first and third are `int` for symmetry,
 * not from evidence. */
void sub_080677E8(void);
void sub_0806780C(void);
void sub_08067A24(void);
void sub_08067BD0(int, int, int, ProcPtr);
void sub_08067C7C(u32); /* `u32`, not `int`: c_08067C7C.c already DEFINES it
                         * that way and a promoted definition wins over a
                         * declaration. Byte-identical at both call sites. */
void sub_08067D04(int, int, int, ProcPtr);
void sub_08067D4C(void);
void sub_08067DD4(ProcPtr);
/* sub_08067ED0 takes NINE. Its first parameter is `u8` and its second `u16`:
 * sub_08069DE8 narrows the first with `lsl #0x18; lsr #0x18` on the one path
 * where the value is not a constant zero, and loads the second with `ldrh`
 * from a member it reads with a full `ldr` at the very next call -- so the
 * narrowing is the parameter's, not the member's.
 *
 * Parameters 7 and 8 are `u8`, taken from the PROMOTED definition in
 * src/decomp/c_08067ED0.c rather than from the call site. sub_08069DE8 passes
 * small constants and a value it has just compared against zero, so `int` was
 * byte-identical there and the call site could not discriminate -- the
 * definition is the only evidence, and it wins. */
void sub_08067ED0(u8, u16, int, int, int, int, u8, u8, ProcPtr);
/* Five arguments, the fifth on the stack.
 *
 * Wave 36, W36-K: RETYPED from `(int, int, int, int, ProcPtr)`. The note that
 * used to sit here said "BOTH arrive as full words", read off sub_08069DE8's
 * call site -- but that call passes the CONSTANTS 0x280 and 3, which narrow at
 * compile time and so show nothing either way. The callee's own prologue is
 * decisive: sub_080686E8 opens `lsls r5,#0x10; lsrs r5,#0x10` on r2 and
 * `lsls r4,#0x18; lsrs r4,#0x18` on r3, i.e. PROMOTE_MODE zero-extension of a
 * u16 and a u8 parameter, before any use. Retyping is byte-neutral at
 * sub_08069DE8 (both arguments are literals there) and is what matches the
 * definition. Same shape as sub_08068810 below. */
void sub_080686E8(int, int, u16, u8, ProcPtr);
/* Wave 36, W36-K. The graphics loader both sub_080686E8 and sub_08068810 hand
 * a `gTextTable[]` blob to. First parameter is `u8 *`: the body walks it
 * with `ldrb [p]` and `ldrb [p + n]` as a 0xff-terminated id list. Second is
 * u16 (`lsls #0x10; lsrs #0x10` at entry, then `lsls #5` for a 0x20-byte VRAM
 * stride). Returns the loop counter held in ip -- an `int` count, not a u8;
 * both callers only `strb` it, which is byte-neutral either way, so `int` is
 * the weaker fit that costs no caller a re-narrowing. */
int sub_08068038(u8 *, u16);
/* Wave 36, W36-K. sub_080686E8's seven-argument sibling: same proc setup, same
 * gUnknown_085D3DD0 -> gTextTable -> sub_08068038 graphics chain, but it
 * fills four more proc bytes (+0x38, +0x39, +0x4e, +0x4f) instead of +0x4d.
 * Arguments 5 and 6 narrow at entry exactly as sub_080686E8's 3 and 4 do. */
void sub_08068810(int, int, int, int, u16, u8, ProcPtr);
/* Wave 36, W36-K. Both are already PROMOTED and byte-verified -- the signatures
 * below are copied verbatim from src/decomp/c_080673D0.c and
 * src/decomp/c_080678BC.c, which had no declaration anywhere, so sub_08068A00
 * could not see them. */
void sub_080673D0(u32, u32, ProcPtr);
void sub_080678D4(u32);
/* Wave 36, W36-K. Four more already-PROMOTED, previously undeclared functions
 * sub_080699E8 needs; signatures copied verbatim from src/decomp/c_0806978C.c,
 * src/decomp/c_080697BC.c and src/decomp/c_080697CC.c. */
void sub_0806978C(void);
void sub_080697A4(void);
void sub_080697BC(void);
void sub_08069924(u8);
/* Wave 36, W36-K. Still asm. One argument, materialised at the call site
 * (`adds r0, r4, #0` on sub_080699E8's own proc immediately before the `bl`),
 * result unused. */
void sub_0806974C(ProcPtr);
/* Wave 36, W36-K. Promoted in src/decomp/c_0806E740.c, previously undeclared;
 * signature copied verbatim. */
void sub_0806E7C0(int, int, ProcPtr);
/* Wave 36, W36-K. Both promoted (src/decomp/c_0806AEC4.c, c_0806E210.c) and
 * previously undeclared; signatures copied verbatim. */
void sub_0806AEC4(int);
void sub_0806E210(int, ProcPtr);
/* Wave 36, W36-K. Promoted in src/decomp/c_0806F0A0.c, previously undeclared.
 * Its definition names a struct tag that lives in that .c, so the tag is
 * FORWARD-DECLARED here rather than retyped to ProcPtr: retyping would have
 * made the promoted definition disagree with this prototype, and the definition
 * wins. Callers outside that file cast. */
struct Unk0806F0A0Proc;
void sub_0806F0A0(struct Unk0806F0A0Proc *);
/* Four arguments, all in registers, the last the caller's proc. */
void sub_08068014(int, int, int, ProcPtr);
/* Takes the caller's proc and nothing else; sub_0806B910 calls it with a bare
 * `adds r0, r4, #0` and discards whatever comes back. */
void sub_0806AD04(ProcPtr);
/* Already DEFINED and promoted in src/decomp/c_0806BD6C.c but never declared.
 * This line publishes the promoted signature so sub_0806BD84 can call it. Do
 * not weaken it -- the definition wins. */
void sub_0806BD6C(u16 *, u16);
/* Two more from this block, promoted alongside their callers. sub_080697CC is
 * the graphics loader sub_08069864 calls; sub_0806B87C is the fade starter
 * sub_0806B910 calls. */
void sub_080697CC(void);
void sub_0806B87C(ProcPtr);

/* ---- Wave 35 (W35-A) ---- */
/* PARKED in data/parked.json (95.5%), so declared from its call site in
 * sub_0804365C rather than promoted. It reads NO argument register --
 * parked.json records its body as
 * `if (gUnknown_085C77A0[gPlaySt.unk02].unk24) return it;
 *  if (gPlaySt.unk30) return it; return 0;` -- and sub_0804365C
 * uses the result (`adds r4,r0,#0; cmp r4,#0`), so it is not void. The
 * parked entry probed u16 and int and found them byte-identical, so `int`
 * is the weakest fit. */
int sub_08043630(void);
/* EIGHT arguments, read straight off sub_080438FC's two call sites: three
 * pointers to the caller's own stack slots in r0/r1/r2, a plain int in r3,
 * then four stack words at [sp+0], [sp+4], [sp+8] and [sp+0xc], the last of
 * which is a fourth pointer to a caller stack slot. Both call sites agree on
 * every position; only [sp+0], [sp+4] and [sp+8] differ in value between
 * them. Nothing reads r0 afterwards, so void. */
void sub_080439A8(int *, int *, int *, int, int, int, int, int *);

/* ---- wave 35 (W35-C): blocks 0x08021-0x08025 ---- */
/* Promoted from the MATCHED sub_08024DDC; declared here so sub_08024E60 (which
 * calls it twice, the second time with its arguments swapped) can see it. Both
 * parameters are the gUnknown_030013D0 record -- see struct BattleUnit in
 * unknown-globals.h for how the layout was measured. VOID, and that is settled
 * from the callee rather than guessed: sub_08024DDC ends `pop {r0}; bx r0`,
 * which overwrites any result, and sub_08024E60 reads nothing after either
 * `bl`. */
void sub_08024DDC(struct BattleUnit *, struct BattleUnit *);
/* TWO arguments, both materialised immediately before the `bl` as bare
 * `movs rN,#imm` (sub_080246B4 calls it as (1, 8) and (0, 8) from two arms of
 * one switch), and the result is discarded. `int` is the weakest type that
 * fits -- the two call sites only ever pass 0, 1 and 8, so nothing constrains
 * the width, and nothing narrows on return. */
void sub_0803F880(int, int);
/* THREE arguments, and the third is what proves the arity: sub_08024058
 * materialises `movs r2,#0xe0; ands r2,r3` -- the top-three-bits field of the
 * gUnknown_08499590 +0x1432 terrain byte it has just written back -- in the
 * instruction pair immediately before the `bl`, with r0 and r1 already holding
 * its own two sign-extended s16 parameters. Nobody computes a mask to pass a
 * dead argument. Result unused (r0 is not read before the next `bl`). */
/* RETYPED, wave 39 (W39-C), from `void sub_080240B4(int, int, int)`. The body
 * settles all four: it RETURNS `(s16)n` (`lsls r2,#0x10; asrs r2,#0x10;
 * adds r0,r2,#0` at the epilogue), and it narrows r0/r1 to 16 bits and r2 to 8
 * at entry. Per PROMOTE_MODE the entry pair is a ZERO-extend for `s16` just as
 * much as for `u16`; what makes these s16 rather than u16 is that every USE
 * re-extends with `asrs` (the second shift pair). The narrow types cost the
 * promoted caller sub_08024058 nothing -- it already holds two s16 parameters
 * and a `v & 0xe0` that fits u8 -- and c_08024058.c was re-run through
 * try_match after this change and still matches. The old prototype's `void`
 * was read off that one caller discarding the result, which is exactly the
 * "agreement between files, not correctness" failure the brief describes. */
s16 sub_080240B4(s16, s16, u8);
/* Wave 39, W39-C. sub_080240B4's near-twin, same three narrowed parameters and
 * the same `(s16)n` return, read off its own body -- its only caller
 * (sub_08028580) is still assembly, so nothing external constrains it. */
s16 sub_0802419C(s16, s16, u8);
/* Already DEFINED and promoted in src/decomp/c_080249EC.c but never declared.
 * This line publishes the promoted signature so sub_08024A2C (a different
 * unit) can call it. The definition wins -- do not weaken it. */
int sub_080249EC(int, s8, u8);
/* Two more undeclared callees of block 0x08024, both from sub_080246B4 and
 * both with the SAME first argument, the literal 0x48. sub_0803F8E0's second
 * argument is `Div(gGameClock, 20) % 4` and sub_0803FE50's is
 * gGameClock itself, each materialised in r1 immediately before its
 * `bl`; nothing reads r0 after either, so both are void. `int` throughout is
 * the weakest type that fits -- gGameClock is already s32 and neither
 * value is narrowed on the way in. (Neither was on the wave brief's
 * undeclared-callee list for this block, which listed only sub_080240B4,
 * sub_08024DDC and sub_0803F880 -- the list undercounts.) */
void sub_0803F8E0(int, int);
void sub_0803FE50(int, int);
/* The WRAM half of the BIOS decompressor, declared to match LZ77UnCompVram
 * above (same argument shape, same const-ness). sub_080247A4 passes a ROM blob
 * out of gUnknown_085C77A0[].unk2c as the source and the gUnknown_03003F68
 * buffer as the destination. */
void LZ77UnCompWram(const void *, void *);
/* Called with nothing set up in r0-r3 and its result discarded, twice, from
 * both arms of sub_080247A4 -- so `void (void)` is byte-identical to any other
 * shape here and is the weakest that fits. */
/* WAVE 35, W35-E: WAS `void sub_08037B84(void);`, WHICH IS WRONG. The promoted
 * definition in src/decomp/c_08037B84.c has taken `void *p` since it landed,
 * and sub_0803CD2C / sub_0803CDBC / sub_0803CE28 all materialise the argument
 * (`adds r0, r5, #0` where r5 already holds gUnknown_02000000 -- a nullary
 * callee needs no such copy). It has no other caller in src/, so nothing had
 * ever exercised the disagreement. */
void sub_08037B84(void *);

/* WAVE 35 (W35-D), block 0x0806D. Four of these were already PROMOTED with no
 * declaration anywhere (sub_0806DCB8 / sub_0806DD34 / sub_0806DDF4 in
 * src/decomp/c_0806DCB8.c and c_0806DDF4.c) -- a symbol missing from this
 * header is not evidence it is underived. Signatures copied from those
 * definitions; sub_0806DD34's parameter is a struct local to c_0806DCB8.c, so
 * the tag is forward-declared here to give it file scope rather than prototype
 * scope. Its object IS a struct Unk08580934_Obj (sub_0806DE38 passes
 * gUnknown_08580934->unk54[unk33] straight to it) -- the two models agree on
 * every offset; retyping c_0806DCB8.c is left for a wave that can re-verify
 * both matched functions in it. */
/* WAVE 35 (W35-D), the m4a block. All four are undeclared and unpromoted;
 * these signatures are read off the call sites in sub_08070350, sub_080707F4
 * and sub_08072B54.
 *   sub_0806F734 -- the MP2K umul3232H32 shape: two u32 in, one out,
 *     sub_08070350 nests one call inside the other.
 *   sub_0806FBD4 -- takes the ADDRESS of gUnknown_03005740 (`ldr r4,=sym;
 *     adds r0,r4,#0`), which sub_080707F4 then also parks in SoundInfo+0x34,
 *     so it fills a jump table rather than being called with a value.
 *   sub_080714FC -- third argument is narrowed `lsls #0x18; asrs #0x18` at the
 *     call, so the parameter is s8; the second is the 0xffff track mask.
 * sub_08073228 is likewise read off sub_08073304's call site (four arguments,
 * the fourth being the proc it has just started). */
/* Only their ADDRESSES are used -- sub_080707F4 parks them in the SoundInfo
 * hook block; nothing in C calls either yet, so `void (void)` is the weakest
 * shape that fits. */
void sub_080700C0(void);
void sub_080718E4(void);
int sub_08072B2C(int);
u32 sub_0806F734(u32, u32);
void sub_0806FBD4(void *);
void sub_080708EC(u32);
void sub_08070668(struct MusicPlayerInfo *);
void sub_080714FC(struct MusicPlayerInfo *, u16, s8);
/* Wave 38 (W38-A): m4a ClearModM, already DEFINED in src/decomp/c_08071564.c as
 * `void sub_08071564(struct MusicPlayerTrack *)` but never declared here. Its
 * two callers sub_08071584 and sub_080715F8 pass the loop's own track pointer
 * straight through in r0, which agrees. The definition wins, not this line. */
void sub_08071564(struct MusicPlayerTrack *);
/* Wave 38 (W38-A): m4a ModDepthSet / LFOSpeedSet -- the two MPlay*Control entry
 * points that call ClearModM. Third parameter is u8 in both: `lsls #0x18;
 * lsrs #0x18` at entry and a bare `strb` into `mod` (+0x17) / `lfoSpeed`
 * (+0x19). Both have fan-in 0, so nothing corroborates from a call site. */
void sub_08071584(struct MusicPlayerInfo *, u16, u8);
void sub_080715F8(struct MusicPlayerInfo *, u16, u8);
/* Wave 38 (W38-A): the wave-27 palette pair, already DEFINED in
 * src/decomp/c_08071C84.c as `void (int)` but never declared here. Declared now
 * because sub_08071CC4 / sub_08071CDC call them. The definitions win. */
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


/* Wave 35, W35-E. Derived from the call sites in 0x0803A/0x0803C/0x0803E;
 * none of these has a definition in src/decomp/ or an entry in
 * data/promoted.json, so the arity is read off the argument setup and the
 * void-ness off the discarded results.
 *
 * sub_08071948 takes 5 (r0-r3 plus one `str rN,[sp]`), sub_080376DC takes 6
 * (r0-r3 plus `[sp]` and `[sp,#4]`), sub_0801FAC4 takes 5. sub_0803AB3C and
 * sub_0803D6D0 get no argument set up at all. */
void sub_0803AB3C(void);
/* Wave 38 (W38-A): fifth parameter retyped `int` -> `u16`. It arrives on the
 * stack and the ROM normalises it `ldr r0,[sp,#0x18]; lsls r0,#0x10;
 * lsrs r6,r0,#0x10` -- that is PROMOTE_MODE's prologue zero-extension of a
 * sub-word parameter, which an `int` does not get, and with `int` the function
 * is two instructions short at entry. All three promoted callers
 * (c_0803A338.c, c_0803A4A8.c, c_08046D30.c) pass the literal 0x8360, so the
 * narrowing is a compile-time no-op there; all three re-verified byte-for-byte
 * after this change. It is a BG tilemap entry (palette 8 | tile 0x360), which
 * is 16-bit anyway. */
void sub_08071948(u16 *, int, int, const void *, u16);
/* W35-E: sub_0803D6FC is DEFINED in src/decomp/c_0803D6B8.c (not c_0803D6FC.c),
 * which is why a glob for its own file missed it. Signature copied verbatim. */
struct Unk3D6FC;
void sub_0803D6FC(struct Unk3D6FC *);
void sub_080376DC(void *, int, int, int, int, int);
/* Wave 55 (W55-E): declared to match the PROMOTED definition in
 * src/decomp/c_0803D990.c exactly (`int sub_0803D990(int, int, int, int, u8)`),
 * not inferred from call sites -- the definition is the stronger witness. It is
 * a clamp/step helper: (value, delta, min, max, wrap). sub_0803D9FC is its
 * first cross-unit caller, which is why nothing declared it until now. */
int sub_0803D990(int, int, int, int, u8);

/* Wave 55 (W55-E): third parameter retyped `int` -> `u8` from the DEFINITION.
 * sub_0803CFA4 opens `lsls r2,r2,#0x18; lsrs r2,r2,#0x18; str r2,[sp,#0x10]`
 * -- PROMOTE_MODE's entry zero-extension of a sub-word parameter, before the
 * first `bl`, which an `int` parameter cannot produce (the only use is
 * `cmp r0, #1`, so a cast at the use would sit after the call). Byte-neutral
 * at both callers: c_0803CDBC.c passes the constant 1 and c_0803CF54.c passes
 * its own `u8 a3`. */
void sub_0803CFA4(const void *, u8 *, u8);
void sub_0803D6D0(void);
void sub_08026040(int, int, int, int);
/* wave 49: corrected (int x5) -> (u16, u16, u16, u16, u8), read straight off
 * the definition's prologue -- r0..r3 are narrowed `lsls #0x10 / lsrs #0x10`
 * and the stacked fifth `lsls #0x18 / lsrs #0x18`, none of which agbcc emits
 * for an int parameter. Byte-neutral at the only call site
 * (src/decomp/c_0803E9F8.c already casts arguments 1 and 2 to u16 and passes
 * literals for 4); c_0803E9F8 re-verified by trymatch exit code after the
 * change. */
void sub_0801FAC4(u16, u16, u16, u16, u8);
void sub_0801FCE0(int, int, int);
void sub_0801FD30(int, int, int);
/* Wave 35, W35-E: matched definitions in this wave's batch. */
void sub_0803A338(void);
void sub_0803A4A8(void);
void sub_0803A5B8(void);
/* Wave 43, W43-I: FOURTH PARAMETER ADDED, and W35-E's arity above is what was
 * wrong -- not the types. sub_0803AB3C sets r3 up as well, `lsls r3,#0x18;
 * asrs r3,#0x18` immediately before the `bl`, which is an s8 conversion of a
 * value that is 0, 0xd or -1 and cannot be anything but a fourth argument.
 * c_0803AAC0.c never reads it; a trailing parameter that is never referenced
 * costs no instructions, and that file was re-verified byte-for-byte after the
 * change. The arity was read "off the argument setup" in wave 35 from the call
 * sites that existed THEN -- sub_0803AB3C was still unmatched, so its r3 was
 * never in evidence. */
void sub_0803AAC0(u8, u8, u8, s8);
void sub_0803CEB8(u8, const void *);
void sub_0803CF54(u8, const void *, u8);
int sub_0801AD70(u8);
/* Wave 50 (W50-K). Read off sub_0801B2FC's two call sites, which set up NO
 * arguments at all (`bl sub_0801ADC8` with r0-r3 dead) and use no result. That
 * is the whole evidence: arity is invisible in a pass-through wrapper, but this
 * is not one -- nothing is forwarded. NOT PROVEN against its own prologue; if a
 * later wave promotes sub_0801ADC8 itself, its definition wins over this. */
void sub_0801ADC8(void);
/* Wave 50 (W50-K). Copied from the PROMOTED definition in src/decomp/c_0801B018.c
 * rather than read off a call site -- a promoted definition wins. sub_0801AC58
 * calls it with an `int` loop index and the ROM narrows it (`lsls #0x10;
 * lsrs #0x10`) at the call, which is what the u16 parameter produces. */
int sub_0801B018(u16);
/* Wave 41 (W41-A): the slot allocator sub_0801AD70/sub_0801ADC8/sub_0801B2FC
 * all ask before touching gUnknown_0200CC38. RETURN TYPE READ OFF ITS OWN
 * BODY, not off a call site: the "none" path is `ldr r0, =0x0000FFFF`, a
 * POSITIVE pool constant -- an s16 function returning -1 emits
 * `movs r0,#1; rsbs r0,r0,#0` instead -- and the success path narrows r8 with
 * `lsls #0x10; lsrs #0x10`. So it returns u16 and its sentinel is 0xFFFF, not
 * -1. sub_0801AD70's `lsls r0,#0x10; cmp r0, #0xFFFF0000` is combine folding
 * the re-narrow into `(u16)result == 0xFFFF`; an (s16) reading of that test is
 * byte-identical at the call site and WRONG about the callee.
 * The parameter is narrow (`lsls #0x10; lsrs #0x10` at entry) and is only ever
 * tested for equality -- against 0xff and against a u8 array element -- so
 * nothing here discriminates its signedness; u16 agrees with the return. */
u16 sub_0801B120(u16);
u16 FindNewestCompleteSave(u16); /* sub_0801B120's readable name; see
                                  * src/decomp/c_0801B120.c. */
void sub_08022AF8(u8, u8, u8, u8); /* W35-E: signature copied from the
                                    * promoted definition in c_08022AF8.c. */

/* Wave 35, W35-F -- block 0x0804B. All read off call sites in sub_0804B744,
 * which materialises every argument. sub_0804B55C's third parameter is dead in
 * its own body (r2 is written by `lsls r2,r6,#1` before any read) but the
 * caller emits `movs r2,#0` in front of both calls, so the arity is 3.
 * sub_0804B850's last three arguments arrive as `str rN,[sp,#0/4/8]` against a
 * `sub sp,#0xc` frame, i.e. parameters 5-7. sub_0804B42C/sub_0804B4C4 are
 * PROMOTED in src/decomp/c_0804B42C.c and these signatures are copied from
 * there, not inferred. */
int sub_0804B42C(int, int);
int sub_0804B4C4(int, int);
u16 sub_0804B55C(u16, u8 *, int);
u16 sub_0804B644(u16, u16);
/* Wave 43, W43-K: parameter 1 was `int` and is NARROW. sub_0804B850's prologue
 * truncates BOTH r0 and r1 with `lsls #0x10; lsrs #0x10` back to back, which is
 * the PROMOTE_MODE entry pattern for a sub-word parameter -- an `int` gets no
 * such pair, and a cast at the single use would sit next to that use instead of
 * in the prologue. The only caller, c_0804B744.c, passes the literals 0 and 1,
 * so the change is byte-neutral there; re-verified after the edit. */
void sub_0804B850(u16, u16, void *, void *, void *, void *, void *);
/* Wave 43, W43-K -- sub_0804B850's three callees, read off its call sites.
 * sub_0804BB28 and sub_0804BB44 are PROMOTED in src/decomp/c_0804BB28.c and
 * these agree with the definitions there; sub_0804BB28's third parameter is
 * dead in its own body and was added to that definition in this wave (see the
 * comment on it). sub_0804BB74's third parameter is `u32` because its body
 * opens `lsrs r6, r6, #2` on the raw incoming value -- a bare logical shift
 * right, which an `int` would have made `asrs`; its first parameter is NOT
 * narrowed at entry, so it is `int` where sub_0804BB28's sibling index is. */
/* Wave 43, W43-K: both are PROMOTED -- src/decomp/c_0804A18C.c and
 * c_0804AE10.c -- and these are copied from those definitions, not inferred.
 * sub_0804A6D8 is the first C caller of either. */
void sub_0804A1E4(u8);
void sub_0804AE10(void);
/* Wave 55, W55-H. All three copied from their PROMOTED definitions, not
 * inferred: src/decomp/c_0804A64C.c, c_0804A68C.c, and -- note the filename --
 * c_0804A6A4.c for sub_0804A6D8, which is why grepping for a c_0804A6D8.c finds
 * nothing. sub_0804A6D8 returns `int`, not void; sub_0804A760 discards it.
 * sub_0804A760 is the first C caller of all three. */
void sub_0804A64C(void);
void sub_0804A68C(void);
int sub_0804A6D8(void);
/* Wave 55, W55-H: copied from the PROMOTED definition in
 * src/decomp/c_0804BA64.c, not inferred. sub_0804B8BC is its first C caller. */
void sub_0804BA64(u16, u16);
void sub_0804BB28(int, void *, int);
void sub_0804BB44(int, void *, int);
void sub_0804BB74(int, void *, u32, int);
void sub_0804B8BC(u16, u16);
/* Wave 35, W35-F. sub_0804D6C8/sub_0804D6FC are PROMOTED in
 * src/decomp/c_0804D6C8.c and c_0804D6FC.c with an unread second parameter;
 * copied verbatim rather than inferred. sub_080505A4 is read off sub_0804D0FC
 * and sub_0804D4E8, which pass a u16 group index and the u16 entry field
 * unk1e and discard nothing. */
void sub_0804D6C8(u16, int, u16);
void sub_0804D6FC(u16, int, u16);
void sub_080505A4(u16, u16);

/* Wave 35, W35-E: byte-verified definitions in src/decomp/ that had no
 * declaration anywhere (src/decomp/c_08037638.c, c_0803D3D8.c, c_080248F8.c).
 * Signatures copied verbatim from those definitions. */
void sub_08037638(int, int, int, int);
void sub_0803D3D8(int, u8 *);
u8 sub_080248F8(void);

/* Wave 35, W35-L: the 0x08047-0x08049 batch and the callees it reaches.
 *
 * sub_08014B0C is the structural twin of sub_080149C0 above -- six arguments,
 * four in registers and two on the stack -- and both sub_080487B4 and
 * sub_08047B98 call it. The FOURTH parameter is `int` and not `u8 *`: that is
 * where the two call sites disagree with sub_080149C0's shape, sub_08047B98
 * passing `p->unk1f + 1` and sub_080487B4 passing the word
 * gUnknown_0849EDB0[i].unk04. Everything else follows sub_080149C0.
 *
 * sub_080199D0 takes one argument (`movs r0,#1`) whose width is not
 * recoverable from the call site; `u8` follows the note on gUnknown_0300xxxx
 * in unknown-globals.h that records it setting a byte from its argument. Its
 * result is discarded, so `void`.
 *
 * sub_08047190 / sub_08047920 / sub_080488E0 are declared from their call
 * sites in sub_08047B98 and sub_08048F4C only; `void *` is the weakest type
 * for the record pointer sub_08047B98 forwards unchanged, and every result is
 * discarded. Widen these when the definitions are matched. */
/* Wave 40 (W40-D) corrects arguments 4..6 from the BODY, the same axis wave 21
 * corrected sub_08014668 on. Argument 4 is `u32`: it is the dividend of a
 * `% 10` / `/ 10` pair that compiles to __umodsi3/__udivsi3, which a signed
 * spelling cannot produce. Arguments 5 and 6 are `u16`, narrowed
 * `lsls #0x10; lsrs #0x10` in the PROLOGUE -- ahead of the destination
 * arithmetic and grouped with each other, i.e. PROMOTE_MODE on narrow
 * parameters. An `int` spelling cannot emit them at all: argument 5's only use
 * is an `orrs` feeding a `strh` and argument 6's is a multiply feeding one, so
 * force_to_mode folds a cast at either use away entirely. Byte-neutral at both
 * call sites -- c_08047B98.c passes `p->unk1f + 1`, 0x8000 and 0, and
 * c_080487B4.c passes a word, an already-`(u16)`-cast product and 0. */
void sub_08014B0C(int, int, u16 *, u32, u16, u16);
/* Wave 40 (W40-D), matched. sub_08014B0C's five-argument sibling and the same
 * digit renderer, but SIGNED: its `% 10` / `/ 10` pair compiles to
 * __modsi3/__divsi3 where sub_08014B0C's compiles to the unsigned pair, so
 * argument 4 is `int` there and `u32` above. It also drops sub_08014B0C's
 * tile-group argument, using the fixed tile pair 0x123/0x133 instead. Argument
 * 5 is `u16` on the same prologue evidence. No caller exists in src/ yet;
 * signature copied verbatim from the matched definition. */
void sub_08014B60(int, int, u16 *, int, u16);
void sub_080199D0(u8);
/* Wave 56, W56-G: the second parameter is WIDE, not `u8`. The ROM's prologue
 * spills r1 to the frame with no `lsls #24; lsrs #24` in front of it, and a `u8`
 * parameter re-narrows there; every use is a word `cmp` against 0..3. Both
 * callers pass a u8 member, which promotes with no change of bytes. */
void sub_08047190(void *, int);
void sub_08047920(void *);
/* Wave 48, W48-I: both MATCHED this wave, so these two are the definitions'
 * own types rather than call-site guesses. sub_0804769C returns a u16 counter
 * and its sole caller sub_080484CC truth-tests the result with a bare
 * `lsls #0x10`, which agrees. `struct Unk0804769C` is in unknown-globals.h
 * because caller and callee land in different translation units. */
u16 sub_0804769C(struct Unk0804769C *, u16);
void sub_080484CC(struct Unk0804769C *);
/* Declared from sub_080484CC's call site only: one argument, the same record
 * pointer forwarded unchanged, result discarded. `void *` is the weakest type
 * that fits; widen it when the definition is matched. */
/* Wave 55, W55-H: retyped from `void *`. The only caller, src/decomp/c_080484CC.c,
 * already passes a `struct Unk0804769C *`, so the change is byte-neutral there
 * and was re-verified by exit code. The body reads unk1f/unk20/unk21 off it.
 *
 * sub_08048158 takes the SAME object through c_08047B98.c's file-local
 * `struct Unk08047B98` (unknown-globals.h's note on Unk0804769C already records
 * that they are one object seen at different offsets). That tag is forward-
 * declared below rather than moved, so both promoted definitions still complete
 * it themselves and neither file needed editing; sub_080482D8 casts at the call.
 * sub_08047F70 is NOT promoted -- its parameter is read off this call site
 * alone (r0 = the same pointer, result unused), so treat it as unproved. */
struct Unk08047B98;
void sub_080482D8(struct Unk0804769C *);
void sub_08047F70(struct Unk08047B98 *);
void sub_08048158(struct Unk08047B98 *);
void sub_080488E0(void);
/* Wave 43, W43-L. Already DEFINED in src/decomp/c_08048F10.c; it had no
 * declaration here because nothing outside its own unit called it until
 * sub_08048FD8 (matched this wave) did. The type is the definition's: it
 * returns the NEW value of the s16 gUnknown_084C30F8->unk832 as u16, and
 * sub_08048FD8 truth-tests that result with a bare `lsls #0x10`, which agrees. */
u16 sub_08048F10(void);
/* Wave 43, W43-L. Already DEFINED in src/decomp/c_080485DC.c; it had no
 * declaration here because nothing outside its own unit called it until
 * sub_08048FD8 (matched this wave) did. The type is the definition's. */
void sub_080485DC(const u8 *);
/* Wave 43, W43-L, matched -- this declaration is taken from sub_08048850's OWN
 * definition rather than from a call site, which is why the widths are firm.
 * Both parameters narrow in the prologue (`lsls #0x10; lsrs #0x10`), so both
 * are 16-bit; UNSIGNED because a1 only ever feeds `i < a1 + 3` and a2 only
 * ever indexes the u8 array gUnknown_02028E1C, and PROMOTE_MODE would emit the
 * same prologue for s16 either way -- nothing re-narrows a use, so the sign is
 * unproved and u16 is the weakest fit. Void: the epilogue sets no r0 and both
 * callers (sub_08049178, sub_08049B80) discard. */
void sub_08048850(u16, u16);

/* The W35-L batch itself. sub_08048EC4 returns `u16`: sub_080490BC narrows its
 * result with `lsls #0x10; lsrs #0x10` and then reuses the same register as an
 * argument, so the value is kept, not just tested. sub_080485F8's epilogue is
 * `pop {r0}; bx r0`, the void spelling. */
u16 sub_08048EC4(void);
void sub_080485F8(void);

/* Wave 56, W56-K. Declared from the call site in sub_08049360, which is
 * `bl sub_08048F4C` with no argument register set up and the result never
 * read -- the next instruction reloads r0/r1 from gUnknown_084C30F8. Its own
 * prologue is `push {r4, lr}` followed immediately by
 * `ldr r0,=gUnknown_084C30F8`, so it takes no parameters either. NOT matched
 * and not attempted; the body is still `asm`. */
void sub_08048F4C(void);

/* Wave 56, W56-K. Four functions that already have PROMOTED, byte-exact
 * definitions but no declaration anywhere; sub_08049360 is the first caller
 * outside their own units. Types taken from the definitions, which are the
 * stronger witness: src/decomp/c_08017704.c, c_0803C864.c, c_08048644.c,
 * c_0804931C.c. */
u32 sub_08017704(u32);
void sub_0803C864(u8);
void sub_08048644(u16, u16);
void sub_0804931C(void);

/* ---- wave 36 (W36-E) ---- */

/* Promoted in src/decomp/c_080248E4.c since wave 30 but never declared -- the
 * fifth instance of the 407-function gap. Copied verbatim from that
 * definition; sub_0807B2F8 hands the result to sub_080149C0's `u8 *`. */
u8 *sub_080248E4(void);
/* Promoted in src/decomp/c_08074714.c and likewise never declared; copied
 * verbatim from that definition. sub_08076888 calls it with a literal 4. */
void sub_08074714(ProcPtr);

/* Proc handlers in the 0x08076 and 0x0807A-0x0807B blocks, all taking their own
 * proc and returning nothing (`pop {..}; pop {r0}; bx r0` with no value set). */
void sub_08076298(ProcPtr);
void sub_0807B2F8(ProcPtr);
void sub_0807BCF0(ProcPtr);
void sub_0807BED8(ProcPtr);
/* Wave 48 (W48-C), read entirely off its ONE call site, sub_0806AB24 -- the
 * body is still assembly, so every field below is a caller-side reading and
 * none of it is confirmed at the callee.
 *   FIVE parameters, the fifth on the stack (`str r4, [sp]` with sp lowered by
 * 8). A text layout/wrap routine: arg1 is whatever `u8 *sub_08024944(u16)`
 * returned, arg2 is `&<u16 local>` -- an OUT-parameter, read back with `ldrh`
 * straight after the call -- arg3 is a `u8 *` into the caller's own proc at
 * +0x2f which the caller then fills as a per-line width array, arg4 is a u16
 * proc field (a wrap width), arg5 is the caller's proc.
 *   The result is a LINE COUNT: the caller stores it into an `int` field and
 * uses it as a `< n` loop bound over arg3's array. Declared `u16` because the
 * call site narrows with `lsls #0x10; lsrs #0x10` -- but that is byte-neutral
 * against `int` plus an explicit `(u16)` cast at the one use (agbcc re-narrows
 * a narrow-returning callee at every call site), so the width is NOT proved.
 * Likewise arg4: a narrow and a wide parameter are identical at this call. */
void sub_0807B51C(int, int, int, int);
void sub_0807B738(ProcPtr);
u16 sub_0807B7BC(u8 *, u16 *, u8 *, int, void *);
/* Nullary: sub_0807A0C4 calls it with `bl` and no argument register set up,
 * and its own prologue reads none. It ends by writing 2 into +0x3a of the
 * struct Unk03001470 sub_08014740 returns, so the result is discarded too. */
void sub_0807A860(void);
/* sub_0807A860 hands it the u8 at +0x1d of a gPlayers record and
 * sub_08078E14's `int` result, and narrows the value it returns to u16 for
 * sub_08014740's fourth parameter. `int` both ways is the weakest model that
 * gives that single `lsls #0x10; lsrs #0x10` at the call site; a `u16` return
 * would be byte-identical here, so this is NOT proved. */
int sub_0807A3AC(int, int);
/* sub_0807BCF0 hands it its own proc once a frame counter passes a threshold;
 * the result is not used. */
void sub_0807BFB8(ProcPtr);

/* Wave 36 (W36-F). The per-frame update/vblank dispatchers at 0x08036884,
 * 0x080368E8, 0x08036944, 0x080369BC, 0x08036A50 and 0x08036AB8 call these in
 * long straight-line runs, and none of them was declared anywhere.
 *
 * The first block is copied VERBATIM from byte-verified definitions already in
 * src/decomp/ (see c_0801348C.c, c_08013B2C.c, c_0801F050.c, c_0801F0AC.c,
 * c_08054B7C.c) -- not re-derived from the call sites. */
void sub_08013510(void);
void sub_08013B2C(void);
void sub_0801F050(void);
void sub_0801F06C(void);
void sub_0801F0AC(void);
void sub_0801F0C8(void);
void sub_0801F0E0(void);
void sub_0801F0FC(void);
void sub_08054B7C(void);

/* The second block has no promoted definition. Each prologue in asm/ opens by
 * writing r0 (or by not touching an argument register at all) before any read,
 * so all are nullary; each is called for effect with the result discarded, so
 * `void` is the weakest type that fits every site. sub_0803B404 is a lone
 * `bx lr`. */
void sub_08011AD8(void);
void sub_08011B98(void);
void sub_08015954(void);
void sub_08019470(void);
void sub_0802FACC(void);
void sub_080345C8(void);
/* Wave 36 (W36-A) is matching sub_08023EEC itself and owns its final
 * signature. Declared here only because sub_080369BC could not compile without
 * it: its prologue opens `ldr r2, =gUnknown_08090A1C` with no argument register
 * read, so it is nullary, and sub_080369BC discards the result. If W36-A's
 * definition disagrees, W36-A's wins. */
void sub_08023EEC(void);
void sub_0803B404(void);
void sub_0803F990(void);

/* Handed to sub_08011B34's `void *` slot by sub_0803662C, via the
 * `(void *)fn` house convention. All nullary by the same prologue reading;
 * sub_0803550C and sub_08043590 open on a global, the rest write r0 first. */
void sub_08021DD8(void);
void sub_08022048(void);
void sub_08022A6C(void);
void sub_0803550C(void);
void sub_08043590(void);
void sub_0803678C(void);

/* Wave 36 (W36-F), block 0x08037. Read off each callee's own prologue in
 * asm/code-0801D390.s, not guessed from the call site:
 *   sub_08037170  four `lsls #16`/`lsrs #16` pairs on r0..r3, so four u16s;
 *                 `pop {r0}; bx r0`, so void.
 *   sub_080377C4  `mov sl, r0` with no narrowing -- one pointer argument.
 *   sub_080378A8  `str r0, [sp]` -- one pointer argument (a VRAM address).
 *   sub_08037B90  opens `movs r3, #0` and never reads an argument register.
 *                 Nullary for real: sub_08037CF8's `bl sub_080378A8` /
 *                 `bl sub_08037B90` pair with nothing between it is TWO
 *                 statements, not a nested call.
 *   sub_08013D00  selects one of gBG0TilemapBuffer/7C/80/84, derefs it and
 *                 returns `*p + y * 64 + x * 2` -- a u16 * into a tilemap.
 *                 r0 is compared against 1/2/3, r1 is the x term, r2 the y. */
u16 *sub_08013D00(int, int, int);
void sub_080377C4(void *);
void sub_080378A8(void *);
void sub_08037B90(void);
void sub_08037170(u16, u16, u16, u16);
void sub_080360D0(ProcPtr);

/* sub_080366D0 and sub_080366C4 take `void (*)(void)`, so these two are fixed
 * as nullary void by the parameter type, not by inference. */
void sub_08036944(void);
void sub_080369BC(void);

/* Wave 36 (W36-I), all read off sub_08004E88 / sub_0800487C / sub_08004970,
 * which are the only call sites in the tree that constrain them:
 *   sub_08002F1C  no argument register written before the `bl`, result
 *                 discarded -- nullary void.
 *   sub_0800C8D8  likewise -- but see below, this one was WRONG.
 *   sub_0800C874  nullary; its r0 is `strb`d straight into
 *                 gActiveMap->propertyCount, so the return is at least a byte
 *                 and `int` is the weakest fit (no re-narrowing appears).
 *   sub_0800CAA0  nullary; its r0 is handed on as sub_0800CB30's second
 *                 argument with no narrowing between the two `bl`s.
 *   sub_0800CB30  called as (1, sub_0800CAA0()).
 *   sub_0808B694  two pointers -- a struct Unk03001470 field address and
 *                 gActiveMap->designName -- and the result is a bare
 *                 `cmp r0,#0`, so a comparison predicate. */
void sub_08002F1C(void);
/* Wave 48 (W48-A) retypes sub_0800C8D8 from `void` to `int`.  W36-I inferred
 * void from its two call sites (sub_08003B8C and sub_08004E88) discarding the
 * result, which is exactly the blind spot the brief warns about: a discarded
 * result constrains nothing.  The body ends `adds r0, r5, #0` before the pop,
 * where r5 is a counter the second loop increments, so it returns that count --
 * the same "how many entries did I touch" value its neighbours sub_0800C874,
 * sub_0800C8A0 and sub_0800C6A8 all return.  Both callers still discard it and
 * both were re-verified byte-identical under the new type.
 *
 * sub_0800C958 had no declaration anywhere in the tree.  It is `int (int)`:
 * the argument reaches sub_0800C6E8's first parameter unchanged and is then
 * switched over the same 0x28/0x48/0x68/0x88 ids, and the result is summed
 * into sub_0800C9E8's counters with no narrowing at either of its uses. */
int sub_0800C8D8(void);
int sub_0800C958(int);
/* sub_08004E38's signature is copied verbatim from the byte-verified
 * definition in src/decomp/c_08004E38.c, which had no declaration anywhere.
 * sub_0800376C likewise (src/decomp/c_0800376C.c). sub_080036A4 and
 * sub_08003814 are wave-36 matches in work/, both plainly nullary void. */
void sub_08004E38(char *, const char *);
void sub_080036A4(void);
void sub_0800376C(void);
void sub_08003814(void);

/* Family F059. The byte-verified definitions in src/decomp/c_080055B8.c were
 * written `(void)`, but their only call site -- sub_08004D28 -- materialises
 * `movs r0,#0; movs r1,#0; movs r2,#0` in front of every one of the three
 * `bl`s, and only a call site can prove an argument. All three parameters are
 * unused in the bodies, which is why `(void)` was byte-identical there and is
 * NOT evidence against them; the definitions were widened to match and
 * re-verified. Wave 36, W36-I. */
void sub_080055B8(int, int, int);
void sub_08005634(int, int, int);
void sub_080056B0(int, int, int);
int sub_0800C874(void);
int sub_0800CAA0(void);
/*   sub_0800CB30  called as (0, 0) and then (1, <the first call's result>) in
 *                 sub_08004F9C, which is what proves the return value.
 *   sub_0800C9E8  nullary; its r0 is narrowed `lsls #0x18; lsrs #0x18` into
 *                 sub_0803CF54's declared u8 third parameter. */
int sub_0800CB30(int, int);
int sub_0800C9E8(void);
/* sub_080032EC: sub_080030BC's tail call, `(lane, x >> 4, 0x6a)`. */
void sub_080032EC(int, int, int);
int sub_0808B694(const void *, const void *);

/* ---- wave 36 (W36-J): the 0x08046-0x08049 address-locality block ---------- */

/* Both have byte-verified definitions in src/decomp/ and had no declaration in
 * any header -- the "407 promoted, 0 declared" case from the brief. Signatures
 * copied verbatim from c_080468D4.c and c_080470DC.c, not inferred. */
void sub_080468D4(int);
void sub_080470E8(void);

/* Derived from their call sites in sub_08046D30 and sub_08046E48, which is all
 * there is: both take gUnknown_02028DD5 and gUnknown_02028DD6 in r0/r1, each a
 * plain `ldrb` of a u8 global with no narrowing before the `bl`, so `int` is
 * the weakest model that fits. Neither result is used and neither caller
 * re-narrows, but with the value discarded that proves nothing about the
 * return, so both are declared void off the call sites alone. */
/* Wave 49, W49-J: RETYPED to (u8, u8) off the definitions' OWN prologues, which
 * outrank the call sites the paragraph above reasons from -- each opens with
 * `lsls #0x18; lsrs #0x18` on r0 and on r1 and spills nothing else, and an
 * `int` parameter is never narrowed at entry. Byte-neutral at every caller: the
 * arguments were already plain `ldrb`s of u8 globals, so nothing has to
 * re-narrow. Both really are void -- sub_08046778 tail-calls sub_08013AEC and
 * sub_08046914 does the same, with nothing in r0 after it. */
void sub_08046778(u8, u8);
void sub_08046914(u8, u8);

/* Wave 43, W43-L. RETYPED from all-`int` off sub_080499F8's call site to the
 * widths sub_08049944's OWN prologue proves -- the definition matched this
 * wave. All five arguments narrow at entry, in argument order, before any
 * other work: `lsls #0x10; lsrs #0x10` on a1/a2/a3/a5 and `lsls #0x18;
 * lsrs #0x18` on a4. An `int` parameter cast at a use puts those shifts at the
 * use, not in the prologue, so these are declared narrow.
 *
 * The SIGNS are settled at the uses, per PROMOTE_MODE (entry is zero-extending
 * for s16 and u16 alike): a2 and a3 are re-widened `lsls #0x10; asrs #0x10`
 * before sub_08014B0C's two `int` parameters, which only an s16 source can
 * produce; a1 is compared `blo`/`bhs` against the digit ramp, unsigned; a5
 * goes to sub_08014B0C's `u16` fifth parameter unnarrowed.
 *
 * Byte-neutral at the one existing caller and re-verified there: c_080499F8.c
 * passes a u16 member, two literals, a u8 member and 0x3000, every one of
 * which converts to the narrow parameter with the same instructions it already
 * used to convert to `int`. */
void sub_08049944(u16, s16, s16, u8, u16);

/* Wave 36 (W36-L): the four undeclared callees of the 0x08020/0x08021 blocks,
 * all read off their call sites in sub_08020354 / sub_080201E0 / sub_08020D50 /
 * sub_080210C8, which are byte-identical in shape.
 *
 * sub_0801F9C0, sub_080200EC and sub_08020B88 are one three-way twin set: each
 * is called twice with (x, y, value, flag). The FIRST two arguments separate
 * them -- sub_08020354 narrows its pair with `lsls #0x10; lsrs #0x10` before
 * passing them to sub_0801F9C0, while sub_080201E0 and sub_08020D50 narrow
 * theirs with `asrs` before passing them to sub_080200EC / sub_08020B88 -- so
 * u16 for the first and s16 for the other two.
 *
 * The THIRD argument is the decisive one and it is read off the SECOND call
 * site in each caller, where the value is `gUnknown_085D5ABC[t].unk0e - 1`, an
 * `ldrb` minus one. sub_0801F9C0's caller emits `lsls #0x10; lsrs #0x10` on it;
 * sub_080200EC's and sub_08020B88's emit NOTHING. That is not a difference in
 * the callers: num_sign_bit_copies proves `(u8)x - 1` already sign-extends from
 * HImode (range -1..254) so a SIGNED narrowing is elided, while the UNSIGNED
 * one is real (-1 becomes 0xffff). So the third parameter is u16 in the first
 * and s16 in the other two. The fourth is a literal 0 / 0xff / 1 / -1 and -1 is
 * materialised as `movs #1; rsbs`, never `movs #0xff`, so it is signed and
 * `int` is the weakest model.
 *
 * sub_08020EDC takes six: three s16 coordinates, a `u8 *` into the
 * gUnknown_08499590 map (`gUnknown_08499590 + 0x1E42 + sel * 1288`), then two
 * stack words. Arguments 5 and 6 are `int`: sub_080210C8 sign-extends its own
 * s8 fifth parameter (`lsls #0x18; asrs #0x18`) and zero-extends its own u8
 * sixth (`lsls #0x18; lsrs #0x18`) before storing each to the outgoing frame,
 * which is the widening a narrow argument into an `int` parameter costs -- a
 * narrow parameter here would have needed no instruction at all. */
/* Promoted in src/decomp/c_080223E0.c with no declaration anywhere; signature
 * copied verbatim from that definition. */
void sub_080223E0(u16, u16);
/* wave 49: 4th parameter corrected int -> u8, the same correction wave 41 made
 * to the twin sub_080200EC below and for the same reason. The definition's
 * prologue narrows r3 with `lsls #0x18 / lsrs #0x18`, which agbcc does not emit
 * for an int parameter, so the width is 8 bits; PROMOTE_MODE zero-extends
 * either signedness so the prologue alone does not settle the sign, but the
 * only call site (src/decomp/c_080201E0.c's twin aside, here c_08020354.c)
 * passes the literals 0 and 0xff, and 0xff is materialised as `movs #0xff`
 * rather than the `movs #1; rsbs` a negative would need -- so unsigned.
 * sub_08020354 re-verified by trymatch exit code after the change. */
void sub_0801F9C0(u16, u16, u16, u8);
/* wave 41: 4th parameter corrected int -> s16. The definition's prologue
 * narrows r3 with `lsls #0x10 / lsrs #0x10`, which agbcc does not emit for an
 * int parameter -- so the width is 16 bits and the old `int` was a guess made
 * before any definition existed. Byte-neutral at the only call site
 * (src/decomp/c_080201E0.c passes the literals 1 and -1); sub_080201E0
 * re-verified by trymatch exit code after the change. */
void sub_080200EC(s16, s16, s16, s16);
/* Wave 49, W49-I. 4th parameter CORRECTED int -> s16, the same correction wave
 * 41 made to the twin sub_080200EC above and for the same reason: the
 * definition's prologue narrows r3 with `lsls #0x10 / lsrs #0x10`, which agbcc
 * does not emit for an int parameter. The body then reads that 16-bit stack
 * slot back with `ldrb` for its `gUnknown_03003340[yy][xx] =` store. Signedness
 * is not settled by the prologue (PROMOTE_MODE zero-extends either), but the
 * only caller (src/decomp/c_08020D50.c) passes 0 and -1 and -1 is materialised
 * as `movs #1; rsbs` -- so signed. Byte-neutral there; c_08020D50 re-verified
 * by try_match exit code after the change. */
void sub_08020B88(s16, s16, s16, s16);
/* Wave 90, W90-C: the 6th parameter is `int`, as the prose above says, and
 * the definition proves it. Its prologue narrows the 6th only at the
 * `f = flags;` copy, AFTER the `d = delta;` group; a `u8` formal would be
 * narrowed first by PROMOTE_MODE, which put one `lsls` a slot early and was
 * the last residual of sub_08020EDC. The matched caller
 * src/decomp/c_080210C8.c keeps its `lsls #0x18; lsrs #0x18` with an explicit
 * `(u8)a6` at each call, which is the same tree the old prototype built, and
 * was re-verified by trymatch exit code. (Wave 73 read that narrowing as
 * proof of a `u8` formal; a cast at the call site gives it too.) */
void sub_08020EDC(s16, s16, s16, u8 *, int, int);

/* Wave 36, W36-M. Every signature below is COPIED VERBATIM from a byte-verified
 * definition in src/decomp/ -- none of them had a declaration in any header,
 * which blocked the 0x08028-0x0802D block. Do not re-derive them from argument
 * registers; the definition outranks that. */
u32 sub_08012E4C(void);                              /* c_08012E4C.c */
void sub_080251BC(int, int, struct Unk802C57C *);    /* c_080251BC.c */
void sub_08037200(u16, u16, u16, u16);               /* c_08037200.c */
u8 sub_0803EED4(int, int);                           /* c_0803EED4.c */
int sub_080249C8(int);                               /* c_080249C8.c */
const u8 *sub_0802A880(int, int);                    /* c_0802A880.c */
void sub_0802E250(void);                             /* c_0802E250.c */
void sub_0802A7C4(void);                             /* c_0802A7C4.c */
void sub_0802DBE4(void);                             /* c_0802DBE4.c */
void sub_0803A59C(void);                             /* c_0803A59C.c */
void sub_0803A8F0(struct Unit *);             /* c_0803A8F0.c */
void sub_080470F8(u16);                              /* c_080470F8.c */
struct Unit *sub_08025580(void);              /* c_08025580.c */
/* VARARGS, and that is the whole reason 0x08090AC4-0x08090B44 is full of
 * printf-style strings: the debug overlay in sub_080281F0 / sub_080283E4 is a
 * run of sub_08013428(x, y, "SNOW:%s", ...) calls. */
void sub_08013428(int, int, const char *, ...);      /* c_08013428.c */
/* RETYPED wave 36 (W36-M): c_0802D9B8.c defined it over a local
 * `struct Unk2D9B8Proc` whose three s16 members are exactly struct
 * Unk03001470's unk1e/unk20/unk22, and its ONLY caller sub_0802DA18 passes
 * `&gUnknown_03001470[i]`. The definition was retyped to agree; byte-identical,
 * re-verified with try_match. */
void sub_0802D9B8(struct Unk03001470 *);             /* c_0802D9B8.c */
/* Undeclared and unpromoted. Read off sub_0802DCB4's call site, which passes
 * gUnknown_030033E4's two halves read as s16 OBJECTS and re-narrows the result
 * with `lsls #0x18` -- the same call shape as its neighbour sub_0802E4B4. */
u8 sub_0802E2D0(s16, s16);

/* ---- wave 37 (W37-C) ----
 * The 0x0800A/0x0800B cell-update block. Every one takes the same (x, y) cell
 * key as a bare `adds rN, r0, #0` / `adds rN, r1, #0` pair with no narrowing,
 * so `int` on both.
 *   sub_08007F68 and sub_0800AA30 are already promoted (c_08007F68.c,
 * c_0800AA30.c) and were only missing a prototype; these agree with the
 * definitions.
 *   sub_08009918 returns 0/1 built with `movs r0,#0` / `movs r0,#1`; its one
 * caller sub_0800A95C shifts the result left by up to 8 with no narrowing, so
 * `int`.
 *   sub_0800A098 and sub_0800A588 are void: every call site drops the result
 * and both end `pop {r0}; bx r0` after a non-call.  sub_0800A95C's result is
 * an `ldrsh` out of a ROM table that sub_0800A588 tests with `cmp r2,#0` both
 * `bge` and `ble`, so it is a signed value -- `int`, on the same reading as
 * sub_0800A798. */
void sub_08007F68(int, int, int);
int sub_0800AA30(int, int, int);
int sub_08009918(int, int);
void sub_0800A098(int, int);
void sub_0800A588(int, int);
int sub_0800A95C(int, int);
/* sub_08009538 returns 0/1 (`movs r0,#0` / `movs r0,#1`), tested with a bare
 * `cmp r0, #0` by its one caller.  sub_0800A3D4 is promoted in
 * src/decomp/c_0800A3D4.c as void(int, int); this only adds the prototype.
 *   NOTE on the s16 sub_0800B61C declared above (wave 36 block): sub_0800BB2C
 * corroborates it -- that caller re-narrows with `lsls #0x10; asrs #0x10` too.
 * sub_0800A95C is the near-twin that does NOT get an s16 return despite the
 * identical body shape (`ldrsh` out of a ROM table, returned straight): its
 * caller sub_0800A588 uses the result with no narrowing at all, so an s16
 * return there would add a `lsls #16; asrs #16` the ROM does not have.  The
 * two together are the reminder that the return width lives at the CALLERS. */
int sub_08009538(int, int);
void sub_0800A3D4(int, int);
void sub_0800BB2C(int, int);
/* Same (x, y) key, same bare `adds rN, r0, #0` entry, no narrowing anywhere.
 * sub_0800168C's result is only ever `cmp r0, #0` (sub_0800AF74), so int.
 * sub_0800BEB8 is promoted void(int, int) in src/decomp/c_0800BEB8.c and was
 * missing only a prototype.  sub_0800A2EC, sub_0800AF24 and sub_0800BEE4 end
 * `pop {r0}; bx r0` after a non-call, so void; sub_0800A6AC and sub_0800A884
 * return an `ldrsh` table entry that is negative on the reject path, so int on
 * the same reading as sub_0800A95C. */
int sub_0800168C(int, int);
void sub_0800BEB8(int, int);
void sub_0800A2EC(int, int);
int sub_0800A6AC(int, int);
int sub_0800A884(int, int);
void sub_0800AF24(int, int);

/* Wave 37 (W37-E). The 0x0800xxxx map-cursor block. Every arity below was read
 * off the CALLEE's own prologue, not off a call site; every result is discarded
 * at every call site found in asm/, and each of these ends `pop {rN}; bx rN`,
 * so `void` is the return type unless noted.
 *
 * The seven nullary ones are sub_0800057C's jump-table arms (cases 0..7 of
 * gActiveMap->mode); none of them reads r0-r3 before writing it. */
void sub_080005FC(void);
void sub_08000650(void);
void sub_08000664(void);
void sub_08000694(void);
void sub_0800081C(void);
void sub_08004CA0(void);
void sub_08005F4C(void);

/* Nullary: its first act is `bl sub_08025E74`, so nothing in r0-r3 is live. */
void sub_08003B8C(void);

void sub_08000BF8(void);
void sub_08000C68(void);
void sub_08000CCC(int);
void sub_08000DF8(int);

/* `int` return: sub_08000CCC keeps the result in r0 and does signed modular
 * arithmetic on it (`subs #4; bge; adds #0x11`), with no re-narrowing after the
 * `bl`. One argument -- r1 onwards is dead on entry. */
int sub_08001D24(int);
void sub_080073F8(int, int);

/* sub_08002510's first parameter is DEAD in the body (r0 is never read before
 * being written) but real: sub_08001DAC passes it, exactly as sub_080030BC's
 * a2/a3 are declared-and-unread. The second is masked `& 0xff` and becomes
 * sub_0801F34C's third argument. */
void sub_08002298(int, int);
void sub_08002510(int, int);

/* Wave 50, W50-G. Both are nullary and void: sub_08002AB0 and sub_08002C38
 * `bl` each of them with no argument setup at all and discard r0, and neither
 * callee reads r0-r3 before writing them (sub_08007B54's first instruction
 * after the push loads its own pool word; sub_08007B74's loads
 * gActiveMap). */
void sub_08007B54(void);
void sub_08007B74(void);

/* Arities from sub_08001DAC's call sites, which put 5, 6 and 7 words in place;
 * sub_0800272C's is additionally fixed by its promoted definition in
 * src/decomp/c_0800272C.c and must agree with it. */
void sub_0800272C(int, int, int, int, int, int, int);
void sub_08002844(int, int, int, int, int, int, int);
void sub_08002964(int, int, int, int, int, int);
void sub_080029F4(int, int, int, int, int, int);
void sub_08003088(int, int);

/* Two out-parameters, both written with a full-word `str` in the callee body,
 * and an int return tested `cmp r0, #0` by both readers. */
int sub_0800C6E8(int, int *, int *);


/* ---- Wave 37 (W37-H): block 0x08035 predicates ----
 * sub_08035C90 and sub_08035CF4 are both narrowed by their one caller
 * sub_08035D0C with `lsls r0,#0x18; cmp r0,#0`, so both return u8 -- the bare
 * `cmp r0,#0` an int return would give is not what the ROM has.  That is why
 * src/decomp/c_08035CF4.c's `int sub_08035CF4(void)` was retyped to
 * `u8 sub_08035CF4(ProcPtr)` this wave: the caller also loads r0 with the proc
 * (`adds r0, r4, #0`) immediately before the `bl`, so the argument is real even
 * though the body ignores it.  Both re-verified byte-exact after the change.
 *   sub_08035D0C's result is re-narrowed `lsls #0x10; lsrs #0x10` by
 * sub_08035E90, so u16 and not u8: its returned constants reach 0x4b.
 *   sub_08035080 is nullary (r0 is written before it is read in its prologue)
 * and returns 0/1, narrowed `lsls #0x18` by sub_08035170.
 *   sub_080129F8 is the same story one block over: it is promoted in
 * src/decomp/c_080129F8.c and had no prototype at all; its only caller
 * sub_08035170 narrows with `lsls #0x18`, so the definition was retyped from
 * `int` to `u8` (byte-exact before and after -- it returns 0/1 constants). */
u8 sub_08035C90(ProcPtr);
u8 sub_08035CF4(ProcPtr);
u16 sub_08035D0C(ProcPtr);
u8 sub_08035080(void);
u8 sub_080129F8(u16);

/* ---- Wave 37 (W37-F): blocks 0x08009 and 0x0800F, map-tile predicates ----
 * All take the same (x, y) key as the rest of the gUnknown_08499590 map family;
 * every arity was read off the callee's own prologue (r0 and r1 saved, r2 never
 * read), not off a call site.
 *   sub_0800F2E0 is already promoted `int (int, int)` in src/decomp/c_0800F2E0.c
 * and was missing only a prototype.
 *   sub_0800F318 and sub_0800F368 return 0/1 built with `movs rN,#0` /
 * `movs rN,#1`; sub_0800F3B8 and sub_0800F418 shift those results left by up to
 * 7 and `orrs` them together with no narrowing, so int.
 *   sub_0800F3B8 returns that same 8-bit mask, unnarrowed, so int.
 *   sub_08009310 returns 0/1 the same way.
 *   sub_08009264 sets no return value on either exit path and ends
 * `pop {r4,r5,r6}; pop {r0}; bx r0` after a `bl sub_08007F9C` whose own result
 * is void, and its one caller sub_0800F4E0 (itself void) drops it: void.
 *   sub_08009CF8's result is only ever turned into a 0/1 by sub_08009918's
 * `rsbs; orrs; lsrs #0x1f` (a returned `!= 0`), so nothing narrows it: int. */
void sub_08009264(int, int);
int sub_08009310(int, int);
int sub_08009CF8(int, int);
int sub_0800F2E0(int, int);
int sub_0800F318(int, int);
int sub_0800F368(int, int);
int sub_0800F3B8(int, int);

/* ---- Wave 37 (W37-G): blocks 0x08010, 0x08016, 0x08018, 0x0801B ----
 * The (x, y) map-key family again -- every arity below was read off the
 * callee's own prologue in asm/code.s, not off a call site.
 *   sub_0800C574 takes THREE: it saves r0/r1/r2 into r5/r6/r7 before touching
 * anything, and ends `pop {r4,r5,r6,r7}; pop {r0}; bx r0`, which destroys r0,
 * so it cannot return a value.
 *   sub_0800F564 takes three (r2 is copied to r6 first and indexes the
 * gUnknown_0848894C / gUnknown_08488954 direction pair); its result is
 * compared against 2 by sub_08010B34 with no narrowing, so int.
 *   sub_0800F8D4 takes two and its result is only zero-tested: int.
 *   sub_0800FD44 takes three (`cmp r2,#0; bne` in the prologue) and its result
 * is forwarded straight into sub_08001158's int third parameter.
 *   sub_08018194 narrows its one argument `lsls #0x18; lsrs #0x18`, so u8, and
 * ends `pop {r0}; bx r0`: void.
 *   sub_080179AC reads no argument register and ends `pop {r0}; bx r0`: void.
 *   sub_0801B7C0 is the opcode dispatcher of the gUnknown_0808EF64 byte-stream
 * (see gUnknown_0808EF64's note): r0 is dereferenced `ldrb r0,[r7]` so it is a
 * pointer, r1 is opaque, and it returns -1 for "no slot".
 *   sub_0801BA1C narrows its second argument `lsls #0x10; lsrs #0x10` (u16),
 * loops on the third with `cmp r2,#0; ble` (signed int) and `strh`s through
 * the first, so that one is a pointer; `pop {r0}; bx r0`, so void.
 *   sub_08010604 / sub_08010DD4 / sub_08010B34 take the (x, y) pair as plain
 * ints (no prologue narrowing) and return an int tile id / 0/1 / -1.
 *   sub_0801659C narrows its one argument `lsls #0x18; lsrs #0x18` and always
 * returns 0, the same bool8 (u8) shape as its sub_080161B4 neighbours. */
void sub_0800C574(int, int, int);
int sub_0800F564(int, int, int);
/* Wave 56 (W56-J). NEW -- sub_0800F77C had no declaration anywhere in the tree.
 * Three plain ints: (x, y, dir). x and y arrive unnarrowed and are added to a
 * signed s16 delta before a signed `blt 0` range check; dir is scaled `lsls #1`
 * to index the gUnknown_0848895C / gUnknown_08488964 tables and is also handed
 * straight to sub_0800F564's int third parameter. The return is an int taking
 * 0, 2, or a count of 0..4, reached through three distinct `movs r0` sites, so
 * it is a value and not a bool. Matched byte-for-byte with this signature. */
int sub_0800F77C(int, int, int);
int sub_0800F8D4(int, int);
int sub_0800FD44(int, int, int);
int sub_08010604(int, int);
int sub_08010B34(int, int);
int sub_08010DD4(int, int);
bool8 sub_0801659C(u8);
void sub_080179AC(void);
void sub_08018194(u8);
int sub_0801B7C0(const char *, int);
void sub_0801BA1C(void *, u16, int);
/* Wave 40 (W40-G). Both are already DEFINED in src/decomp/ and the definition
 * wins: sub_0801B738 in src/decomp/c_0801B70C.c (an IWRAM-overlay trampoline,
 * four arguments, first u8) and sub_0801B964 in src/decomp/c_0801B964.c (the
 * paired-counter advance returning 1 on wrap). sub_0801B8D0 is the first
 * cross-file caller of either. It truth-tests sub_0801B964's result through
 * `lsls #0x18` with no `lsrs`, which is a u8 CAST at the call site and not a
 * narrow return type -- the definition returns int. */
int sub_0801B738(u8, int, int, int);
int sub_0801B964(int);
/* sub_0801B8D0(str, vram, kind) -- the glyph-run renderer, matched in wave 40
 * and called only by sub_0801B7C0. It walks a NUL-terminated string, hands each
 * byte to sub_0801B738 with a running x, and returns `Div(w + 7, 8) * 2`, a
 * width in half-tiles; sub_0801B7C0 both adds that to its halfword cursor and
 * stores `w >> 1` (an ARITHMETIC shift) into a u8, which is what makes the
 * return plain int rather than unsigned.
 *   The second parameter is `int` and not a pointer even though every caller
 * builds a VRAM address for it: it is forwarded to sub_0801B738's int second
 * parameter, whose promoted definition wins. The caller keeps the
 * `(u8 *)(charbase) + (tile * 32 + 0x06000000)` association documented in
 * src/decomp/c_0801B780.c and casts the whole sum back to int, which is free. */
int sub_0801B8D0(const u8 *, int, int);
/* sub_0801BAB8 is a lone `bx lr` -- the do-nothing default entry sub_0801BABC
 * writes into all 15 slots of gUnknown_03002FE0. Its address is only ever
 * TAKEN, never called here, so `void (void)` records nothing but the fact that
 * it is a function. IrqMain is crt0.s's ARM interrupt entry, which sub_0801BABC
 * CpuFastSets into IWRAM; declared so that pool word can be named. */
void sub_0801BAB8(void);
void IrqMain(void);
/* Already DEFINED in src/decomp/c_0804B0CC.c; signature copied from the
 * definition, which wins. sub_080048D4 is its first cross-file caller and
 * passes `(int)gActiveMap->designName` -- the int first parameter is the
 * definition's, so the pointer is cast at the call site. */
void sub_0804B10C(int, u8);
/* Already DEFINED in src/decomp/c_0801820C.c; sub_08018254 is its first
 * cross-file user and only takes its address, storing it into
 * gUnknown_0200C528[a].unk08 through a `void *` cast. Signature copied from
 * the definition, which wins. */
void sub_0801820C(struct Unk0200C528 *);

/* Wave 37 (W37-H). sub_08038848 pushes one step onto the gUnknown_0849D5F8
 * move stack. Both parameters are s8: the entry pair is `lsls #0x18; lsrs
 * #0x18` (PROMOTE_MODE, which says nothing) but every use inside is
 * `lsls #0x18; asrs #0x18`, and its one caller sub_08038C98 reads the first
 * with a bare `ldrsb` and sign-extends the second. */
void sub_08038848(s8, s8);

/* Wave 54, W54-H. Already DEFINED in src/decomp/c_08055F68.c with exactly this
 * signature -- copied from the definition rather than inferred. sub_08055D4C
 * is its first cross-unit caller. */
u16 sub_08055F68(u16);

/* Wave 54, W54-H. All four are already DEFINED in src/decomp/c_0805521C.c with
 * exactly these signatures -- copied from the definitions rather than inferred,
 * so the declarations cannot drift. sub_08054C5C is their first cross-unit
 * caller, and it chains all four: each result is re-narrowed
 * `lsls #0x10; lsrs #0x10` and handed to the next as the trailing `tile`
 * argument, which corroborates the u16 return the definitions already carry. */
u16 sub_0805521C(u16, u16, u16, u16);
u16 sub_08055288(u16, u16, u16, u16, u16);
u16 sub_0805530C(u16, u16, u16, u16);
u16 sub_08055374(u16, u16);
/* Wave 54, W54-H. Matched this wave; no promoted body yet, so this is read off
 * the definition in work/sub_08055058/. Parameters 1 and 2 are UNREAD by the
 * body -- they are positional, and the count comes from sub_08054C5C's call
 * site, which sets all four. Parameter 3 is the one tested `== 2` and
 * parameter 4 subscripts gUnknown_08552D80; the u16 return is the tile
 * cursor sub_08054C5C re-narrows and chains into sub_08055374. */
u16 sub_08055058(u16, u16, u16, u16);
/* Wave 54, W54-H. Matched this wave. Both parameters are 16 bits (`lsls #0x10;
 * lsrs #0x10` at entry) and it is void -- nothing is left in r0 and it ends
 * `pop {r0}; bx r0`. Parameter 1 is the side (it scales gUnknown_020298E0 by
 * 0x90 and indexes gUnknown_08552148 and gUnknown_03004580) and parameter 2 is
 * the slot, which subscripts gUnknown_02029A10[side].entries; both are
 * forwarded UNCHANGED to sub_08050F24 as the last statement, which is where
 * the pairing is confirmed rather than guessed. */
void sub_08054598(u16, u16);

/* Promoted in src/decomp/c_080386DC.c as void(int, int); this only adds the
 * prototype its caller sub_08038C98 needs (wave 37, W37-H). */
void sub_080386DC(int, int);

/* Already defined in src/decomp/c_08012F6C.c with exactly this signature;
 * nothing declared it, so its one caller sub_080729EC could not see it
 * (wave 37, W37-N). It is the CpuFastSet/CpuSet data-move front end. */
void sub_08012F6C(const void *src, void *dst, int size);

/* Wave 37 (W37-H) -- FINDING, NOT YET APPLIED: sub_08015438's FOURTH
 * parameter is declared `void *` and is not one. Every caller that exists in C
 * today passes 0 or NULL there (c_08027A50, c_080393CC, c_0803B264, and
 * c_08015410 which only forwards), so the wrong type has been invisible.
 * sub_08035BC4 -- still unmatched, parked in work/sub_08035BC4/ -- passes a
 * sign-extended s16 VALUE in r3 (`lsls r3,#0x10; asrs r3,#0x10` immediately
 * before the `bl`), which no pointer expression produces. The fix is `int` for
 * parameter 4 here and in src/decomp/c_08015438.c, plus `NULL` -> `0` in
 * src/decomp/c_08027A50.c; it is byte-neutral (same register, same width) but
 * it touches five promoted files, so it needs a wave with budget to re-run
 * try_match on all of them. */

/* Wave 37 (W37-I). sub_08034A7C is defined in src/decomp/c_08034A7C.c and was
 * never declared here; sub_08034AF8 is its first caller outside its own unit.
 * Both parameters are plain `int` -- the y arrives as a `movs #0x4e` immediate
 * and the second as a bare `ldrb` of gPlayers[i].unk1a. */
void sub_08034A7C(int, int);
/* Byte-returning predicate: every caller re-narrows the result with
 * `lsls #0x18` before testing it (sub_08034AF8, and the guard the
 * gPlayers.unk2d note describes). */
bool8 sub_08026D44(int);
void sub_08026F04(void);
void sub_080268F4(void);
void sub_08044178(int);
void sub_0802BFBC(void);
void sub_08034C8C(void);
/* sub_0803D788 calls it as (gBG3TilemapBuffer, 0, 0x800) with the length built
 * once in a callee-saved register and copied into r2 -- arity 3, no width
 * evidence beyond the register.
 *
 * WAVE 37: this was first declared `(void *, int, int)` from that caller-side
 * reading alone, which proto_check.py then flagged against the ALREADY-PROMOTED
 * definition in src/decomp/c_08013098.c: `void sub_080130C8(u16 *dst, int delta,
 * int size)`. The definition wins -- it is byte-verified, and the brief's rule is
 * that a promoted definition beats "the weakest type that fits". This is the same
 * error that broke wave 14's first SPLIT=1 build (`void *` declared as the weakest
 * model against a promoted named type).
 *
 * Agreeing costs nothing here and is strictly better: gBG3TilemapBuffer is itself
 * declared `u16 *`, so the sole call site now type-checks exactly rather than
 * decaying through void *. */
void sub_080130C8(u16 *, int, int);

/* Wave 37 (W37-J3), block 0x08056.
 *
 * sub_0805601C passes FIVE arguments to sub_080560A4 -- r0..r3 plus one stack
 * word -- but that callee's own prologue narrows only r0 and r1, so the last
 * three are dead inside it and their widths are the caller's. Every argument
 * at both call sites is one of sub_0805601C's own u16 parameters or a small
 * constant. sub_0805634C narrows all three of its arguments at entry. */
void sub_080560A4(u16, u16, u16, u16, u16);
void sub_0805634C(u16, u16, u16);
/* Wave 49, W49-M, block 0x08056. sub_0805634C computes
 * `t = sub_0805653C(c, b)` -- note the REVERSED argument order at that call
 * site, the third parameter first -- and passes t on to sub_080564B8 as its
 * third argument.
 *   The RETURN TYPE is 32-bit and UNSIGNED, and both halves of that are read
 * off the caller rather than the body: `mov r8, r0` after the `bl` with NO
 * re-narrowing rules out a u16 return (agbcc re-narrows a narrow-returning
 * callee's result at every call site), and the two loops that then use it as a
 * bound against a u16 counter compare `blo`/`bhs`, which an `int` would have
 * made `blt`/`bge`. The body itself only ever returns 0, 1 or its own u16
 * first parameter. */
u16 sub_0805653C(u16, u16);
/* Wave 49, W49-M. Arities off the two call sites in this block: sub_0805634C
 * calls sub_080564B8 with r0/r1/r2 and sub_08056638 with r0 alone, and both
 * callees narrow exactly those registers at entry. Widths from those
 * prologues. */
void sub_080564B8(u16, u16, u16);
void sub_08056638(u16);
void sub_0805601C(u16, u16, u16, u16);
void sub_08056D8C(u16, u16, u16);
/* sub_08056EEC fills a six-halfword stack record and passes its address -- the
 * same shape, and the same `void *` spelling, as sub_08057048/sub_080570C4.
 * The record's layout is described in sub_08056EEC's own file. */
void sub_08056F8C(void *);
/* The second argument is never read: sub_08056EEC narrows r0 and r2 and leaves
 * r1 alone. Arity 3 is from the r2 use, not from a caller -- nothing calls it
 * yet. */
void sub_08056EEC(u16, u16, u16);
/* Promoted in src/decomp/c_080573F0.c and never declared until now; the file
 * defined it with no prior prototype in scope. void/void off the definition. */
void sub_080573F0(void);

/* ---- wave 37 (W37-O1), the 0x08019000 block ---- */

/* Four argument-free, result-discarded callees of sub_080191B0. Read off each
 * callee's own prologue, not off the call site: sub_0803CB8C, sub_080198C4 and
 * sub_0801797C each write a literal-pool global before touching an argument
 * register, and sub_08017A0C's prologue loads its own pool words first. */
void sub_0803CB8C(void);
void sub_080198C4(void);
void sub_0801797C(void);
void sub_08017A0C(void);

/* Wave 40, W40-E. NO callers anywhere in the tree, so neither the return type
 * nor its existence is constrained by anything -- the body ends `movs r0,#0`
 * before the pop, and `int` is the natural-C reading of that, not a proof.
 * The parameter IS settled: it is sign-extended once for the loop's `!=` test
 * and the pre-shifted copy is re-`asr`ed for the gUnknown_0200C528 index. */
int sub_08017F0C(s16);

/* Its one caller (asm/code-0806CFC8.s, inside sub_08074384) does
 * `ldr r0, [r4, #4]; cmp r0, #0; beq ...; bl sub_08019348` and then ignores
 * r0, so: one argument, result discarded. The argument is the same `const u8 *`
 * script pointer sub_080193B0 takes -- sub_08019348 forwards it unchanged on
 * one path and stores it into gUnknown_0200C508 on the other. */
void sub_08019348(const u8 *);

/* A gUnknown_0200C528 list-script handler in the c_0801903C / c_080190EC
 * family: s16 slot index, `movs r0, #1` before the epilogue. Never reached by
 * a `bl` -- it is installed as a table word like the rest of that family. */
bool8 sub_0801906C(s16);


/* ---- wave 37 (W37-K1) ----
 * The 0x0805A / 0x0805D adjacent-cell probe family: the twin of the
 * sub_08058BB4 / sub_08058C54 / sub_08058CE8 / sub_08058E88 block already
 * matched in src/decomp/c_08058BB4.c, and typed off that block.
 *
 * sub_0805ACFC and sub_0805C128 are the per-cell testers. Both open
 * `adds r4,r0,#0 / adds r5,r1,#0 / adds r6,r2,#0` with no PROMOTE_MODE shift
 * pair, so both coordinates are `int`; both end `pop {r4,r5,r6}; pop {r0};
 * bx r0`, so both are void. The out-pointer is `u16 *` and not `s16 *`:
 * sub_0805ACA8 and sub_0805A854 read slot 0 back with a plain `ldrh` for the
 * 0x270F sentinel test, the same discriminator c_08058BB4.c records.
 *
 * sub_0805ACA8 and sub_0805A854 are the four-neighbour drivers. Each returns
 * 1/0 as `movs r0,#1` / `movs r0,#0` split across a `b`, which is the one
 * spelling `return <cmp>;` cannot produce, so `int` and an explicit if/else.
 * sub_0805A854 takes the cell as a `u16 *` pair and updates it in place.
 *
 * sub_0805A5E0 stores a whole word through its one argument
 * (`movs r0,#1; rsbs r0,r0,#0; str r0,[r7]`) and ends `pop {r0}; bx r0`, so
 * `void (int *)` -- an out-parameter, not a return value.
 *
 * sub_0805D648 takes FIVE arguments: r0-r3 plus one stack word, set up as
 * `movs r2,#0; str r2,[sp]` before r2 is reloaded with the third. In
 * sub_0805DA84 r0 and r1 arrive as `movs rK,#0 / ldrsh` off a `u16` pair, so
 * they are `s16` parameters -- declaring them `u16` emits `ldrh` at that call
 * site instead and does not match -- and r3 arrives as `lsls #0x18;
 * lsrs #0x18` off an `int`, so `u8`.
 *
 * sub_08058BB4 is already DEFINED in src/decomp/c_08058BB4.c and had no
 * declaration anywhere; this publishes that definition unchanged (re-verified
 * with try_match after the edit). Its `u16` first parameter is what makes
 * sub_0805DA84's `lsls #0x10; lsrs #0x10` appear in front of the `bl`. */
void sub_0805ACFC(int, int, u16 *);
/* Wave 48, W48-F: u8, not int -- same correction as sub_0805BA34/sub_0805BB8C/
 * sub_0805BC7C in this block. sub_0805A514 re-narrows the result with
 * `lsls #0x18; lsrs #0x18` before `cmp r0,#1`, which agbcc does not emit for an
 * int return. Byte-neutral in src/decomp/c_0805ACA8.c (only 0 and 1 are
 * produced) and re-verified there after the change. */
u8 sub_0805ACA8(int, int, u16 *);
void sub_0805C128(int, int, u16 *);
int sub_0805A854(u16 *);
void sub_0805A5E0(int *);
/* Wave 52, W52-D. sub_0805A268, sub_0805A388 and sub_0805A514 are DELIBERATELY
 * NOT DECLARED HERE, and the reason is worth recording because two waves have
 * now been tempted to add them. All three are already DEFINED, in
 * src/decomp/c_0805A268.c and c_0805A514.c, taking `struct Unk5A514Cell *` --
 * a FILE-LOCAL tag repeated verbatim in three promoted files
 * (c_0805A268.c, c_0805A514.c, c_0805A744.c) and therefore unnameable from a
 * shared header. Declaring them here with any other pointer type is a real
 * cross-unit disagreement that `try_match` cannot see; `tools/proto_check.py`
 * catches it and did. A caller repeats the tag body and casts at the call
 * site instead -- see work/sub_0805E160 and work/sub_0805E87C.
 *   The caller-side evidence, for whoever eventually merges the types: all
 * three take the dereferenced gUnknown_03003F20 in r0, which
 * include/unknown-globals.h declares `struct Unk03003338 *`. So
 * struct Unk03003338 and struct Unk5A514Cell describe the same object and the
 * cast is not a coincidence.
 *
 * Wave 52, W52-D. NULLARY on a BARE PROLOGUE: sub_0805E2AC pushes, saves the
 * high registers and adjusts sp without copying r0 anywhere, and sub_0805E160's
 * only call to it sets up no arguments at all. W21-B's rule says a bare prologue
 * is evidence for a WIDE parameter rather than for none, so this is the weaker
 * of the two readings and is UNPROVEN -- sub_0805E160 matched byte-for-byte
 * either way, because r0 happens to hold the cell's x at that call and a
 * forwarded argument costs no instruction. Settle it from sub_0805E2AC's own
 * body when that function is derived. */
void sub_0805E2AC(void);
/* Wave 49, W49-G. CORRECTED from (s16, s16, int, u8, int): parameters 3 and 5
 * are u8, not int. sub_0805D648's own prologue narrows all five --
 * `lsls #0x10 / lsrs #0x10` on the first two and `lsls #0x18 / lsrs #0x18` on
 * the last three -- and agbcc emits no PROMOTE_MODE pair at all for an `int`
 * parameter. The first two stay s16 because both are sign-extended at their
 * uses (`lsls #0x10; asrs #0x10`) before an int comparison against
 * gUnknown_030040D8->unk02/unk03. Every caller is still in asm/, so this
 * costs nothing in the tree. */
void sub_0805D648(s16, s16, u8, u8, u8);
/* Wave 55, W55-C. Copied VERBATIM from the promoted definition in
 * src/decomp/c_08058BB4.c (`int sub_08058DEC(int x, int y, u16 *out)`), which
 * had no declaration anywhere; sub_0805DCD4 calls it from another unit and
 * passes x and y with no narrowing at either call site, which agrees. */
int sub_08058DEC(int, int, u16 *);
/* Wave 49, W49-G. sub_0805D5EC is DEFINED in src/decomp/c_0805D5EC.c and had no
 * declaration; this publishes it unchanged so sub_0805D648 can call it. */
void sub_0805D5EC(void);
/* Wave 49, W49-G. sub_08071910 is the linker's THUMB->ARM veneer for
 * sub_08000554, which data/asm-resident.json records and which is LONGJMP:
 * `ldm r0!, {r4-fp, ip, sp, lr}` then `movs r0,r1; moveq r0,#1; moveq pc,lr`.
 * Declaring it NORETURN is what the ROM requires, not a convenience:
 * sub_0805D648 ends with `bl sub_08071910` and NO epilogue, and its first call
 * site is followed directly by a literal pool, so agbcc must know control does
 * not come back. Spelled against the veneer rather than sub_08000554 because
 * the veneer is the symbol the ROM's `bl` encodes; naming the ARM function
 * would make the linker synthesise a second one. */
void sub_08071910(u8 *, int) __attribute__((noreturn));
int sub_08058BB4(u16, u16 *);
/* Wave 49, W49-D. Already DEFINED in src/decomp/c_08058BB4.c but never declared;
 * added because sub_080587FC calls it. Signature copied from the definition. */
int sub_08058C54(int, int, u16 *);
/* Wave 49, W49-D. Declared from its only call site (sub_080587FC) plus its own
 * body: it parks r0 in sb and finishes `mov r1,sb; str r0,[r1]`, a WORD store,
 * and returns 0 or -1. */
int sub_08058A2C(int *);
/* Wave 51, W51-K.  sub_0805878C is DEFINED in src/decomp/c_0805878C.c and was
 * never declared; this publishes that definition unchanged so sub_0805E718 can
 * call it. */
struct Unk03003338 *sub_0805878C(void);
/* Wave 51, W51-K.  sub_080587FC TAKES AN ARGUMENT, which its only call site
 * (sub_0805E718) hides completely: the ROM there is `bl sub_08058744;
 * bl sub_080587FC` with no argument register written in between, because
 * sub_08058744's result is already in r0.  This is the wave-50/51 arity rule --
 * a callee whose argument is already live costs no instruction at the call.
 * sub_080587FC's own prologue is `str r0, [sp, #8]`, a WORD spill with no
 * PROMOTE_MODE narrowing, and it reloads that slot for two `cmp r0, #0` tests,
 * so `int`.  It returns the record count `(p - gUnknown_03003338) >> 3` and
 * sub_0805E718 tests it against 0. */
int sub_080587FC(int);

/* Wave 37, W37-K2: the undeclared callees reached from the 0x0805D / 0x08060 /
 * 0x08061 blocks. Arity is read off the argument registers written immediately
 * before each `bl`, and void-ness off whether r0 is live afterwards.
 *
 * sub_08061868, sub_08061B00, sub_080606D0, sub_0805D438 and sub_08061AC4 are
 * five of the six arms of sub_0806171C's jump table: each is reached with no
 * argument register written and its result is discarded by a `pop {r0}`
 * epilogue. sub_08061AC4's is copied verbatim from the promoted definition in
 * src/decomp/c_08061AC4.c, which wins over any guess here.
 *
 * sub_08061178 takes `(u8)(gUnknown_030046C0.unk06 - 1)` -- the caller's
 * `subs #1; lsls #0x18; lsrs #0x18` is PROMOTE_MODE's caller-side narrowing for
 * a u8 parameter -- and its result is stored with a bare `strb`, so u8 out.
 * sub_080611D8 fills a 4-byte stack object whose two halves are then read with
 * `ldrh` and handed to sub_08025E08, and its result is tested with
 * `lsls #0x18; cmp #0`, so it returns a byte.
 *
 * sub_08058144 returns a POINTER: sub_0805DFF4 tests the result against 0 and
 * then reads bytes +1 and +2 off it, which is struct Unit's unk01/unk02
 * pair.
 * WAVE 45, W45-E: the `struct Unit *` here is WRONG and I left it alone
 * rather than break a promoted file. The matched body returns
 * `&gUnknown_084995A0[v]` -- the ROM scales v by 8 (`lsls #3`) and reads
 * `unk03[a2]` at +3, which is struct PropertyListEntry (0x08), not Unit
 * (0x0c, and it is reached by a *3*4 chain everywhere else). The +1/+2 evidence
 * above does not discriminate: PropertyListEntry's filler_00[3] covers those bytes
 * too. Fixing this means retyping to `struct PropertyListEntry *`, naming
 * filler_00's three bytes, and editing src/decomp/c_0805DFF4.c (its local `p`)
 * -- byte-neutral in both, but it touches a promoted file, so it wants an
 * orchestrator's re-sweep rather than a mid-wave unilateral edit.
 * sub_08057F00's result is __divsi3's dividend, so `int`. sub_08059A0C's
 * is only tested against 0. sub_0805BFDC takes four registers and nothing on
 * the stack. sub_080591E4 and sub_0805C0AC both take the address of the same
 * 4-byte (u16, u16) stack object, so `void *` is the weakest model that fits
 * both. */
void sub_080606D0(void);
void sub_08061868(void);
void sub_08061AC4(void);
void sub_08061B00(void);
void sub_0805D438(void);
u8 sub_08061178(u8);
u8 sub_080611D8(void *);
/* Wave 49, W49-E. sub_08061668's signature is COPIED FROM the promoted
 * definition in src/decomp/c_08061668.c, which wins over any reading here.
 * RECORDED, NOT ACTED ON: its only C caller, sub_080611D8, narrows the result
 * to a byte before testing it (`lsls r0,#0x18; cmp r0,#0`), which is the
 * caller-side evidence for a `u8` return -- and per the brief a return type is
 * settled from the callers. Retyping it to `u8` is byte-neutral inside
 * c_08061668.c itself (its body only ever returns the constants 0 and 1), so
 * the change is safe, but it touches a promoted file and wants an
 * orchestrator's re-sweep rather than a mid-wave unilateral edit.
 * sub_080611D8 therefore reproduces the narrowing with a u8 local.
 *
 * sub_08061308 takes (unit-class byte, small mode selector, the same 4-byte
 * (u16, u16) object sub_08061668 fills) and its result is compared against 1
 * after `lsls #0x18; lsrs #0x18`, so it returns a byte. All five call sites are
 * in sub_080611D8 and all five pass the same three operand classes. */
int sub_08061668(u16 *);
u8 sub_08061308(u8, u8, u16 *);
void sub_080610D0(void);
void sub_0806056C(u8);
void sub_0805E440(void);
void sub_0805F4F8(void);
void sub_0805FB70(void);
/* Wave 51, W51-N. Callees of the 0x08059/0x0805F battle-cursor block that had
 * no declaration. All four take the `union Unk802C57CBuf` scratch cell as a
 * `void *` out-parameter, the same contract sub_080591E4 and sub_08059C00
 * already carry.
 *   sub_0805FC1C's first argument is the 3-bit field at gUnknown_030040D8+9
 * bits 3..5, arriving zero-extended (`lsls #0x1a; lsrs #0x1d`), so `int` is the
 * weakest fit.
 *   sub_0805C988 and sub_0805A8C0 both return a BYTE: sub_08059674 tests the
 * first with a bare `lsls #0x18` truth test and re-narrows the second with
 * `lsls #0x18; lsrs #0x18` before `cmp #1`. Their parameter widths are NOT
 * pinned -- sub_08059674 hands them cell coordinates that are already
 * sign-extended for its own map arithmetic, so CSE supplies the `asrs` either
 * way and `s16` would emit the same bytes. `int` is the weakest fit. */
void sub_0805FC1C(int, void *);
int sub_0805C988(int, int);
/* sub_0805A8C0 IS DELIBERATELY NOT DECLARED HERE. Wave 51, W51-N measured both
 * sides and they genuinely disagree, so the original cannot have had a
 * prototype in scope at its caller:
 *   - the DEFINITION needs `u16` (src/decomp/c_0805A8C0.c). Its ROM prologue
 *     zero-extends BOTH parameters itself (`lsls #16; lsrs #16` twice);
 *     retyping them to `int` deletes those four bytes and drops the function to
 *     10.9%.
 *   - the CALLER sub_08059674 passes values it has just SIGN-extended (`asrs
 *     #16`) with no conversion instruction between them and the `bl`. Adding
 *     `u16` parameters to a visible prototype makes agbcc insert `lsrs #16` at
 *     the call, which keeps the u16 copies live across the whole body and costs
 *     r8 -- +12 bytes, 16.5%.
 * Both are consistent with K&R default promotion: no prototype in scope, so the
 * caller passes ints and the callee narrows its own parameters at entry.
 * No declaration here can serve both. Measured, all four:
 *   `(u16, u16)` -> caller 16.5%;  `(int, int)` -> definition 10.9%;
 *   omitted entirely -> agbcc runs warnings-as-errors and rejects the implicit
 *   declaration;  `()` -> caller matches but the definition will not compile
 *   ("an argument type that has a default promotion can't match an empty
 *   parameter name list declaration").
 * So the caller carries its own K&R declaration in src/decomp/c_08059674.c and
 * the header stays silent. Both functions verified byte-for-byte that way. Do
 * not "fix" this by adding a prototype -- it will break one of the two. */
void sub_08059C60(void *);
void sub_08059464(void *);
/* CORRECTED in wave 45 (W45-H): `void sub_08058058(void)` was wrong on both
 * counts. The body opens `adds r7, r0, #0` -- r0 is READ before being written
 * and is the starting element index into the gUnknown_03003F20 list
 * (`lsls r0,r7,#2; adds r2,r2,r0`) -- and it RETURNS that index advanced by the
 * number of records appended (`adds r0,r7,#0`), with an early `movs r0,#0` on
 * the guard-fail path. The old prototype was invisible because its only caller,
 * sub_0805DFF4, calls it immediately after sub_0804151C, whose `int` result is
 * already sitting in r0 -- a nested call is byte-identical to two statements,
 * so `sub_08058058(sub_0804151C())` and `sub_0804151C(); sub_08058058();`
 * compile the same. src/decomp/c_0805DFF4.c is updated to the nested spelling
 * and re-verified. */
int sub_08058058(int);
int sub_08057F00(int);
struct Unit *sub_08058144(int, int);
void sub_080591E4(void *);
int sub_08059A0C(void *);
void sub_0805BFDC(int, int, int, int);
void sub_0805C0AC(void *);
/* Wave 51, W51-C. The four battle-animation entry points sub_08059760,
 * sub_08059824, sub_080598BC and sub_08059978 fill the same 4-byte (u16, u16)
 * stack object sub_080591E4 and sub_0805C0AC take, and hand its address to one
 * of these two before testing the low halfword against 0x270F -- so `void *`
 * on the last parameter for the same reason it is `void *` there.
 *   sub_0805A9AC's first argument is a 0/1 selector (`movs r0,#0` in
 * sub_08059760, `movs r0,#1` in sub_08059824) and nothing else is set up, so
 * the arity is two.
 *   sub_08059B4C takes FIVE: r0 is sub_08057F00(1)'s `int` result left in place,
 * r1 a gUnknown_085766E0->unk04[] byte, r2 the constant 0, r3 the
 * gUnknown_03003F20 list pointer, and the stack word is the (u16, u16) object.
 * `int` on r1 rather than `u8` because the value arrives already zero-extended
 * from an `ldrb` and neither width costs an instruction at the call.
 *   sub_0805EB58 and sub_0805F914 are called with no argument set-up and their
 * results are unused. */
void sub_0805A9AC(int, void *);
/* Wave 52, W52-B. No prototype existed; this one is read off the MATCHED body
 * (work/sub_0805A6DC). One argument: the output cursor for the same
 * {u8 x; u8 y; s16 v;} 4-byte record c_0805A514.c produces, spelled `u8 *`
 * because the plain scalar-pointer stores are what the byte match needs -- a
 * `struct Unk5A514Cell *` cursor is 4 bytes short. The result is the record
 * COUNT, formed as `(out - (u8 *)gUnknown_03003F20) >> 2`. Its one caller,
 * sub_0805F4F8, is still asm, so nothing else constrains this yet. */
int sub_0805A6DC(u8 *);
void sub_08059B4C(int, int, int, void *, void *);
void sub_0805EB58(void);
void sub_0805F914(void);
/* Wave 53, W53-E. Both signatures are COPIED FROM the promoted definitions in
 * src/decomp/c_08059674.c and src/decomp/c_0805F6D4.c, which win over any
 * reading here; neither had a declaration anywhere, so every caller in the
 * 0x08059/0x0805F battle-cursor block was carrying its own. sub_08059674's
 * `u8` return is why its callers all test the result with a bare
 * `lsls r0,#0x18` truth test rather than `cmp r0,#0`, and its `s16` parameters
 * are why callers holding `int` coordinates sign-extend (`lsls #0x10;
 * asrs #0x10`) in front of the `bl` while callers holding `s16` ones do not.
 * NOTE the c_08059674.c comment: that file also carries its own K&R
 * declaration of sub_0805A8C0, which must NOT be moved here. */
u8 sub_08059674(s16, s16);
void sub_0805F6D4(void);
/* Wave 51, W51-E. The 0x0805ED70/0x0805EE40/0x0805EF00/0x0805F074 quartet is
 * the same battle-animation entry-point shape W51-C documents just above, and
 * these declarations come out of matching all four.
 *   All four take nothing and compute nothing after their last call.
 * sub_0805EF00's one caller, sub_0805F074, calls it with no argument register
 * written and discards, which is the only caller evidence any of them has.
 *   sub_08059AEC is nullary: three of the four call it immediately after the
 * gUnknown_030013EC indirect call with no argument set-up at all, and no caller
 * reads r0 afterwards.
 *   sub_08059E3C, sub_08059F24 and sub_0805A008 each take ONE argument and it
 * is the same one in all three -- the dereferenced gUnknown_03003F20 list
 * pointer, arriving as a bare `adds r0,rN,#0` off the register that has held it
 * since the prologue. That is the operand sub_08059A0C already takes as
 * `void *`, and sub_08059F24/sub_0805A008 are the two arms of a single
 * if/else in sub_0805ED70, so they share a signature by construction. All
 * three results are discarded at every call site seen, hence `void`. */
void sub_08059AEC(void);
void sub_08059E3C(void *);
void sub_08059F24(void *);
void sub_0805A008(void *);
void sub_0805ED70(void);
void sub_0805EE40(void);
void sub_0805EF00(void);
void sub_0805F074(void);
/* Wave 51, W51-M. The same entry-point shape once more, declared from
 * sub_0805ECDC's body: it takes nothing, its result is never read, and it ends
 * on the shared `bl sub_0805F7B8` tail the quartet above ends on.
 *   sub_080590DC is sub_080591E4's twin: sub_0805ECDC picks between the two on
 * gUnknown_085D5ABC[unk00].unk1a == 0x20 and hands each the address of the SAME
 * four-byte stack object, so they share a signature by construction -- the same
 * `void *` sub_080591E4 already carries. Result discarded, hence void.
 *   sub_08058F90 is already DEFINED in src/decomp/c_08058BB4.c as
 * `int sub_08058F90(void *)`; this only re-declares it, because sub_0805ECDC is
 * its first matched caller and compares the result against -1 as a full word. */
void sub_0805ECDC(void);
void sub_080590DC(void *);
int sub_08058F90(void *);
/* Wave 51, W51-M. The two arms of sub_0805E9DC's local function-pointer pair
 * (the 8-byte .rodata template at 0x0816DA88, holding 0x0805E87D and
 * 0x0805E779). Reached through `bl _call_via_r0`, i.e. a NULLARY indirect call
 * -- gcc parks the pointer in the first free scratch register, so r0 means no
 * argument register is live -- and the result is discarded.
 *   sub_0805EA54 is nullary on the same evidence: sub_0805E9DC calls it with no
 * argument set-up and never reads r0. Bodies not read. */
void sub_0805E778(void);
void sub_0805E87C(void);
void sub_0805EA54(void);
/* Wave 45, W45-E. sub_08057EC0 takes NO arguments: its only caller
 * (sub_08058144) re-enters it at the top of a loop with r0/r1 holding whatever
 * the previous iteration left there, and it writes r0 before reading it. It
 * returns a POINTER -- an element of the 4-byte-strided array *gUnknown_03003F20
 * whose lowest-`unk02` live entry it selects and then stamps with 0x7FFF, or 0
 * when none is live. sub_08058144 reads bytes +0 and +1 off the result as a
 * (column, row) pair, so the record is {u8, u8, s16}. `void *` is the weakest
 * model that fits; the record has no struct tag yet, and gUnknown_03003F20's
 * declared `struct Unk03003338 *` (stride 0x08) is NOT it -- sub_08057EC0 walks
 * the array with `adds r2, #4`. */
void *sub_08057EC0(void);
/* Wave 45, W45-F. sub_0805BB8C adds four sub_0805BBF8 results together with no
 * `lsls #0x18; lsrs #0x18` between the `bl` and the add, so the return is NOT
 * re-narrowed and the function returns `int` rather than a byte -- even though
 * the body only ever returns 0 or 1. Both parameters are `int` for the same
 * reason in reverse: the body tests `x < 0` off the raw argument register with
 * no sign-extension pair, which a declared s16 would have forced. */
int sub_0805BBF8(int, int);
/* Wave 45, W45-F. The fourth argument is an OUT parameter, a pair of adjacent
 * halfwords: sub_0805BAFC writes `strh` at +0 and +2, and sub_0805BA34 seeds
 * +0 with 9999 before four calls and re-reads it afterwards with a plain
 * `ldrh` -- unsigned, so `u16 *` rather than `s16 *` even though the values
 * stored are map coordinates. Third argument is a unit-type id (`ldrb` off
 * struct Unit's unk00 at the call site), passed full-width. */
void sub_0805BAFC(int, int, int, u16 *);
/* Wave 45, W45-F. Three nullary queries -- sub_0805BE10, sub_0805BE54 and
 * sub_0805BEF0 call each with no argument register written beforehand, and
 * compare the results (and `+ 5` / `+ 2` of them) with a SIGNED `bge`, so the
 * return is `int`. */
int sub_08058318(void);
int sub_0805848C(void);
int sub_080585D4(void);
/* Wave 47, W47-G. The other two census counters of the same family, both
 * already DEFINED (src/decomp/c_080583DC.c, src/decomp/c_08058254.c) and never
 * declared; these publish the definitions unchanged. sub_0805BEA0 compares
 * `sub_08058254() < sub_080583DC() + 5` with a signed `bge`, the same tell. */
int sub_080583DC(void);
int sub_08058254(void);
/* Wave 49, W49-A. Same family again, already DEFINED (src/decomp/c_080586CC.c)
 * and never declared; this publishes the definition unchanged. Its one caller
 * found so far, sub_0805B2EC, only truth-tests the result (`cmp r0,#0`), so it
 * adds no width evidence either way -- the `int` is the definition's. */
int sub_080586CC(void);
/* Copied verbatim from the promoted definition in src/decomp/c_08058BB4.c,
 * which had no prototype; the definition wins. */
void sub_08058F30(u8 *);

/* Wave 37, W37-K3. Copied verbatim from the promoted definitions in
 * src/decomp/, which had no prototype anywhere; the definition wins. */
void sub_08061CDC(void);
void sub_08062028(void);
void sub_08062C7C(u8);

/* Wave 37, W37-K3. A whole-struct assignment `*dst = *src` of the 0x130-byte
 * struct Unk085771C4, compiled out of line: 0x10 bytes of unrolled ldrb/strb
 * then a 24-trip loop copying 0xc at a time, 0x130 in total. Both parameters
 * are that record type -- sub_08061788 passes a ROM row of gUnknown_085771C4
 * as the source and two different RAM records as the destination. Not yet
 * matched; the signature is read off the copy length, which is why the pointee
 * is the sized record rather than `void`. */
void sub_08061A40(struct Unk085771C4 *, const struct Unk085771C4 *);
/* Wave 50, W50-M, MATCHED. Builds one gUnknown_085771C4-shaped record into the
 * destination: the 0x10-byte header is copied from gUnknown_085771C4[a2] and
 * the 24 following 0xc-byte rows are that record summed byte-wise with
 * gUnknown_085771C4[gUnknown_0857690C[a3][gPlayers[a4].unk1d]]. The
 * pointee is the sized record for the same reason sub_08061A40's is -- the
 * 0x130 stride is what proves it. Parameter widths are read straight off the
 * entry narrowings: `lsls/lsrs #0x18` twice then `#0x10`. */
void sub_08061928(struct Unk085771C4 *, u8, u8, u16);

/* Wave 37, W37-K3. Nullary: each prologue overwrites r0 before reading it. */
void sub_080607E8(void);
void sub_08061E98(void);
void sub_0806279C(void);

/* Wave 44, W44-H. The three arms of sub_08061E98's on-stack handler table.
 * That function copies { sub_08061DCC, sub_08061E54, sub_08061E80 } from its
 * own unit's .rodata onto its stack with one `ldm`/`stm` pair and dispatches on
 * the low three bits of a unit record's unk09 (`bl _call_via_r1` -- ONE
 * argument, per the register-index rule). The three ROM words at 0x0816DB10
 * read 0x08061DCD / 0x08061E55 / 0x08061E81 out of baserom.gba, THUMB bit set,
 * and that is what fixes the ORDER; nothing in the dispatch does.
 *   Each takes one gUnknown_08499594 unit record -- sub_08061E98 passes
 * `&gUnknown_08499594[i]` with no arithmetic between the load and the call.
 * The tags below are the ones the promoted definitions in src/decomp/ already
 * use, and they stay INCOMPLETE here: each handler reads the record through its
 * own bitfield view because struct Unit declares +0x04 and +0x09 as
 * plain bytes, and splitting those would touch every other reader of that
 * shared struct (the same reasoning the sub_08061DCC note below records).
 * Declaring the tag incomplete lets every existing definition keep compiling
 * with no edit, and lets a caller name the functions. If a later wave gives the
 * unit record one shared bitfield type, these three and sub_08061E98's own
 * local view should collapse into it. */
struct Unk8061DCC;
struct Unk61E54;
struct Unk61E80;
void sub_08061DCC(struct Unk8061DCC *);
void sub_08061E54(struct Unk61E54 *);
void sub_08061E80(struct Unk61E80 *);

/* Wave 44, W44-H. Copied verbatim from the promoted definition in
 * src/decomp/c_0802700C.c, which had no prototype anywhere; the definition
 * wins. sub_08061F34 is the first caller in C -- it passes the u16
 * gUnknown_030033EC and two int cell coordinates and tests the result with a
 * bare `lsls #0x18`, all of which the definition's types already produce. */
bool8 sub_0802700C(int, int, int);

/* Wave 37, W37-K3, matched this wave. */
void sub_08061788(u16);
void sub_08061868(void);
void sub_08061B00(void);
void sub_08061CF8(void);
/* sub_08061DCC is deliberately NOT declared here. Its parameter is one unit
 * record -- the same layout as struct Unit (unk00 subscripts
 * gUnknown_085D5ABC[] with the `* 0x5c` stride, unk04_0 and unk06_0 come out
 * with that struct's documented `ldrb; lsls #25; lsrs #25`) -- except that it
 * WRITES the low three bits of unk09, and only a real bitfield reproduces the
 * ROM's SImode `movs #8; rsbs; ands` mask; the hand-written `(x & ~7) | K`
 * narrows to `movs #0xf8; ands` and is two instructions, not three. Splitting
 * Unit's plain `u8 unk09` would touch every other reader of that shared
 * struct, so the draft carries its own tag and the prototype stays out of the
 * header until a caller needs it. Both callers are still assembly. */

/* Wave 37, W37-Q3 -- the gUnknown_03001470 "wake every pending slot" pass.
 * sub_08015954 and sub_08015994 both set unk12 bit 0 across the 30 slots and
 * then run `do { sub_08015A9C(); } while (sub_08015B94());`. Neither call sets
 * up an argument register, so both are nullary; sub_08015B94's result is
 * narrowed `lsls r0,r0,#0x18` at both call sites, which is the u8/bool8
 * readout, and its body returns only 0 and 1.
 *
 * sub_080159E0 was promoted in an earlier wave (src/decomp/c_080159E0.c) and
 * was never declared ANYWHERE; the signature below is copied verbatim from
 * that definition. */
void sub_080159E0(u8);
void sub_08015994(void);
void sub_08015A9C(void);
bool8 sub_08015B94(void);
/* A gUnknown_0848A160 opcode handler, so `u8` slot index in and the
 * interpreter's keep-going flag out, like every other handler in that table.
 * The parameter width is hard: the prologue is `lsls r0,r0,#0x18; lsrs`. */
bool8 sub_08015D24(u8);

/* ---- wave 37 (W37-Q4) ---- */
/* Builds the unlocked-CO list in gUnknown_020288A0 and returns how many it
 * wrote, not counting the 0xff terminator. `u8`: sub_08043D00 re-narrows the
 * result with `lsls #0x18; lsrs #0x18` before doing anything with it. */
u8 sub_08043CA0(void);
/* Wave 50 (W50-H). sub_08043C98 is sub_08043CA0's neighbour and its only known
 * caller is sub_0803BFBC, which stores the result with a whole-word `str` into
 * struct Unk08580934's `u8 *unk18`. That member's type was settled in wave 36
 * from its own reader (`ldr [r0,#0x18]` then an unscaled index and `ldrb`), so
 * `u8 *` is the return type both ends already agree on and the assignment needs
 * no cast. Nothing narrower is provable -- this is the only call site. */
u8 *sub_08043C98(void);
/* Third argument is compared against 14 as a SIGNED int and then used two ways:
 * as sub_08043AA0's palette-slot index when it is small, and as CpuFastSet's
 * `void *` destination when it is not -- so `int`, with the cast at the
 * CpuFastSet call. Second argument is the `u16 *` tile buffer the nibble
 * rewrite walks with `strh`/`adds #2`. */
void sub_08043E8C(int, u16 *, int);

/* Wave 44, W44-D. PARKED (data/parked.json) -- declaration only. Arity and
 * void-ness read straight off sub_08087548's one call site: r0 arrives as
 * sub_08037D80's `int` result with no re-narrowing, r1 is that loop's `int`
 * counter and r2 is sub_08087548's own third parameter forwarded unchanged, so
 * three `int`s. sub_08087548 discards the result and is itself void
 * (`pop {r0}; bx r0`), so nothing constrains a return value -- `void` is the
 * weakest type that fits. */
/* Wave 77, W77-E: FIRST PARAMETER CORRECTED int -> u32, on the function's own
 * clamp. The body is `cmp r3, #1; bhi` -- an UNSIGNED compare, which an `int`
 * parameter cannot produce (it would be `bgt`). Wave 30 measured the same thing
 * from the other side. The only caller, promoted in src/decomp/c_08087548.c,
 * passes sub_08037D80's `int` result; int -> u32 is a byte-neutral implicit
 * conversion, so the call site does not discriminate and the own-body compare
 * is the hard fact. This conflict made the draft fail to COMPILE. */
void sub_08087514(u32, int, int);

/* Wave 44, W44-D. Already MATCHED and promoted as
 * `void sub_08043FA8(int a, void *b, int c)` in src/decomp/c_08043FA8.c but
 * never declared here; sub_08087B74 is the first caller in a different unit.
 * Copied verbatim from the definition -- the promoted file wins. */
void sub_08043FA8(int, void *, int);

/* ---- wave 45, W45-G: the 0x0800C region ---- */

/* sub_0800C7E8 masks its own parameter `& 0x1f` before switching on it, so
 * nothing at a call site constrains the width and `int` is the weakest type
 * that fits.  It RETURNS: three arms write r2 (0, 1 or 2) and the shared tail
 * is `adds r0, r2, #0`, and its caller sub_0800C840 forwards that result
 * straight out through `pop {r1}; bx r1`. */
int sub_0800C7E8(int);

/* sub_0800C2D0 has exactly ONE caller in the tree, sub_0800C22C, which drops
 * the result at all nine call sites -- so `void` is the weakest type that
 * fits and no other caller can disagree.  Three `int`s: the first two are
 * sub_0800C22C's own (x, y) parameters plus/minus 1 with no narrowing, the
 * third is a literal 0 or 1 flag. */
void sub_0800C2D0(int, int, int);

/* Wave 45, W45-G.  The (x, y) cell pair, unnarrowed, exactly as the
 * sub_08007D70 / sub_0800BEE4 neighbours take it. */
void sub_0800CEF8(int, int);

/* Wave 45, W45-G.  sub_0800C7A4 and sub_0800C75C are the clear and the set
 * half of one pair: both switch the same first parameter over
 * 0x28/0x48/0x68/0x88 to an index 0..3 and write gActiveMap's unk17
 * and unk1b at that index.  The first parameter is not narrowed at entry, so
 * `int`; sub_0800C75C's second and third are stored with bare `strb`s, which
 * makes their width a floor only, and `int` is the weakest that fits.  Both
 * are void -- sub_0800C7A4 falls out of the default arm to a bare `bx lr`
 * with r0 still holding the parameter. */
void sub_0800C7A4(int);
void sub_0800C75C(int, int, int);

/* Wave 45, W45-G.  THREE parameters, not the two the register scan reports:
 * r2 is passed straight through to sub_08001158's int third parameter without
 * ever being written, and its caller sub_0800EAF4 sets it to a literal 0x25
 * and 0x65 at the two call sites.  Void: `pop {r4, r5}; pop {r0}; bx r0`. */
void sub_0800EBFC(int, int, int);

/* Wave 46, W46-A.  Copied verbatim from the promoted definition in
 * src/decomp/c_080736F4.c, which is authoritative -- the function was matched
 * without ever being declared, so sub_08073714 was the first caller to need
 * it.  Row index is UNSIGNED (`bhi` guard) and x is signed (`< 0` clamp). */
void sub_080736F4(int x, u32 y, u16 *row);

/* Wave 46, W46-G.  Same story as sub_080736F4 directly above: copied verbatim
 * from the promoted definition in src/decomp/c_08073974.c, which is
 * authoritative.  sub_08073998 is the first caller to need it. */
void sub_08073974(int x, u32 y, int c, u16 *base);

/* Wave 46, W46-G.  Copied verbatim from the promoted definition in
 * src/decomp/c_0806E4BC.c, which is authoritative -- it too was matched before
 * anything calling it was, so sub_0806E510 is the first caller to need it.
 * Seven parameters; the fourth is the only narrow one and the seventh is
 * Proc_Start's parent.  It RETURNS the proc (`pop {r1}; bx r1` with r0 live). */
ProcPtr sub_0806E4BC(int a1, int a2, int a3, u16 a4, int a5, int a6, ProcPtr parent);

/* Wave 46, W46-F.  Both copied from their promoted definitions, which are
 * authoritative: src/decomp/c_08065200.c and src/decomp/c_08065818.c.  Neither
 * had ever been declared -- they were matched before anything that calls them,
 * and sub_08065238 / sub_0806530C / sub_0806540C (for the first) and
 * sub_0806574C (for the second) are the first callers to need a prototype.
 * sub_08065200's `int` return is corroborated at all three call sites: the
 * result goes straight into an `strh` with no re-narrowing in between. */
int sub_08065200(int);
void sub_08065818(void);

/* Wave 46, W46-F.  Copied verbatim from the promoted definition in
 * src/decomp/c_08064D44.c, which is authoritative; sub_0806530C is the first
 * caller to need it. */
void sub_08064D44(struct Unk08580934_Obj *, int, int, int);

/* Wave 46, W46-F.  ONE parameter (`adds r5, r0, #0` is the only register read
 * before anything is written) and void -- the two exit paths leave 0x80 and
 * sub_08021810's result in r0 respectively, so nothing consistent is returned,
 * and sub_0806574C discards it.  The argument is gUnknown_08580934 at that
 * call site and the body writes the whole +0x00..+0x20 header through it, but
 * `void *` is the weakest type that fits and costs the caller nothing --
 * leaving the eventual definition of sub_0803BFBC free to name its own type
 * rather than being pinned to struct Unk08580934 by a caller.
 *   Wave 46, W46-E: agreed with, not changed -- sub_0806D850 is the second
 * caller and passes the same global, so `void *` still costs nothing. Worth
 * recording that the body CORROBORATES struct Unk08580934 independently of
 * everything that typed it: it writes +0x04 as a `strh` (the s16 unk04),
 * +0x18 as a whole word (the u8 *unk18), and fills exactly the four parallel
 * byte tables at +0x09, +0x11, +0x1c and +0x20 four entries at a time -- the
 * extents unk09/unk0d/unk11/unk20 were only ever guessed at from "whatever
 * fits below the next member". */
void sub_0803BFBC(void *);

/* ---- wave 47 (W47-G) ---- the 0x0805B4A8 / 0x0805BC7C cursor-target block.
 *
 * sub_0808B6C4, sub_0805B4A8 and sub_0805BD40 are already DEFINED in
 * src/decomp/ and were never declared; these publish those definitions
 * unchanged (sub_0808B6C4 is the tree's memset -- dst, fill byte, length).
 *
 * sub_0805B4D8 returns a BOOLEAN BYTE: its body only ever produces the
 * literals 0 and 1, and its caller sub_0805BDE4 re-narrows the result with
 * `lsls #0x18; lsrs #0x18` before `cmp #1` -- the call-site narrowing agbcc
 * emits for a byte-returning callee. The second and third arguments are
 * out-parameters written with a whole-word `str` (`str r4,[r6]` and
 * `str r0,[r1]`), so they are `int *` and not halfword pointers.
 *
 * The four x/y/out builders below all take the cell key as two unnarrowed
 * `int`s (`adds rN, r0, #0` with no shift pair in any prologue) and write the
 * pair back as two `strh` through the third argument, the same shape as the
 * promoted sub_0805BE10 / sub_0805BEF0 family. sub_0805BC7C returns 0 or 1. */
void *sub_0808B6C4(void *, int, int);            /* c_0808B6C4.c */
int sub_0805B4A8(void);                          /* c_0805B4A8.c */
u8 sub_0805B4D8(int, int *, int *);
int sub_0805BD40(int, int, int, int, s16 *);     /* c_0805BD40.c */
/* Wave 48, W48-F: u8, not int. Its one caller sub_0805B814 re-narrows the result
 * with `lsls #0x18; lsrs #0x18` before `cmp r0,#1`, which agbcc does not emit for
 * an int return. Byte-neutral in the body (only 0 and 1 are produced), verified
 * against src/decomp/c_0805BC7C.c after the change. */
u8 sub_0805BC7C(int, int, u16 *);
void sub_0805BDE4(int, int, u16 *);
void sub_0805BEA0(int, int, u16 *);
void sub_0805BF3C(int, int, u16 *);

/* ---- wave 48 (W48-B) ---- the 0x0805B744 AI-turn driver block.
 *
 * sub_0805B980 is already DEFINED in src/decomp/c_0805B980.c and was never
 * declared; this publishes that definition unchanged (it takes nothing and
 * computes no value after its final store).
 *
 * sub_0805B744 sets up ONE 4-byte stack slot, passes its address to
 * sub_0805B8F4 and then to sub_0805B814, and nothing else in the frame is
 * addressed -- so both take a single pointer to that slot.  Its neighbours
 * sub_0805BC7C / sub_0805BDE4 write the same-sized slot as an x/y pair of
 * `strh`, hence `u16 *`.  sub_0805B8F4's result is re-narrowed at the call
 * site with `lsls #0x18; lsrs #0x18` before `cmp #1`, the narrowing agbcc
 * emits for a byte-returning callee, so it returns u8. */
/* sub_08059C00 takes the scratch cell list (gUnknown_03003F20, dereferenced
 * from its own pointer global) and the address of the same 4-byte x/y pair
 * sub_080591E4 and sub_0805BAFC use; sub_0805B778 seeds +0 with 9999 before
 * the call and re-reads it with `ldrh` afterwards, so `u16 *`.  Its result is
 * never read at either call site.  sub_0805F7B8 is nullary and its result is
 * likewise never read. */
/* Wave 51, W51-K.  RETYPED from `void`: sub_08059C00 ends
 * `asrs r0, r3, #0x10; pop {r4-r7}; pop {r1}; bx r1`, where r3 is the winning
 * record's value shifted left 16 -- a sign-extended s16 computed on both arms
 * and left in r0.  A `void` body cannot emit that (it is dead code and agbcc
 * deletes it).  Byte-neutral at both call sites, which is why it went
 * unnoticed: neither reads the result, so no re-narrowing appears anywhere. */
s16 sub_08059C00(void *, u16 *);
void sub_0805F7B8(void);
/* sub_080581A4's first argument is a plane inside the gUnknown_08499590 map
 * (`gUnknown_08499590 + 0x3C72`, the same plane c_0805B980.c reads), passed as
 * a raw `u8 *` the way sub_0801F92C takes `gUnknown_08499590 + 0x2852` beside
 * it; the second is a bare `movs r1,#0` at the only call site, unnarrowed. */
void sub_080581A4(u8 *, int);
/* sub_0805B5BC and sub_0805B6A0 are sub_0805B4D8's twins over the same
 * gUnknown_02029ED8 record (see that symbol's comment). They differ from it
 * only in taking the two cursor indices BY POINTER instead of by value --
 * both are `ldr rN,[rN]` at entry and the second is written back with `str`
 * when the 0xFE branch advances it -- and, for sub_0805B6A0, in dropping the
 * terrain predicate entirely. Third and fourth arguments are the same pair of
 * whole-word out-parameters sub_0805B4D8 has, so `int *`.
 *   Return type is UNSETTLED: both bodies only ever produce 0 and 1, and no
 * caller was inspected. `int` is the weakest type that fits and is what they
 * were matched under; if a caller turns up that re-narrows with
 * `lsls #0x18; lsrs #0x18`, they are u8 like sub_0805B4D8 and this must
 * change. Changing it is byte-neutral in their own bodies.
 *   WAVE 49, W49-A: the callers turned up, and they say u8 -- but they say it
 * with a BARE `lsls r0,r0,#0x18` and no `lsrs`, which the wave brief classes as
 * a truth test rather than a value-kept narrowing, so it is one notch weaker
 * than the tell recorded above. It is still positive evidence: under an `int`
 * return a plain `if (f(...))` needs only `cmp r0,#0`, and the shift is exactly
 * agbcc re-narrowing a byte-returning callee. Five independent call sites agree
 * -- sub_0805AE88, sub_0805AF90, sub_0805B1CC (sub_0805B5BC) and sub_0805B0AC,
 * sub_0805B2EC (sub_0805B6A0) -- and all five matched with the declaration left
 * at `int` and an explicit `(u8)` cast written at the call. NOT changed here:
 * these are shared declarations under promoted definitions, and the cast is
 * byte-identical to retyping. If a sixth site keeps the value, retype then.
 *   sub_0805B3F4 is the turn-start entry beside them: it takes nothing,
 * computes nothing after its final indirect call, and dispatches through
 * gUnknown_08576890. */
void sub_0805B3F4(void);
int sub_0805B5BC(int *, int *, int *, int *);
int sub_0805B6A0(int *, int *, int *, int *);
/* Wave 49, W49-A. sub_0805AF90 is the sub_0805B5BC-driven map-redraw loop in
 * this same family; it returns nothing (its only exit is a bare early return)
 * and its two callers, sub_0805AE88 and sub_0805B2EC, both discard. */
void sub_0805AF90(void);
void sub_0805B980(void);
void sub_0805B744(void);
void sub_0805B778(void);
void sub_0805B814(u16 *);
u8 sub_0805B8F4(u16 *);
/* Wave 48, W48-F. Both are DEFINED in src/decomp/c_0805B980.c and were matched
 * there under `int`, which W45-F flagged as unsettled because no caller had been
 * inspected. The callers exist and they settle it as u8: sub_0805B814 re-narrows
 * sub_0805BA34's result with `lsls r0,#0x18; cmp r0,#0` before its truth test,
 * and sub_0805B8F4 re-narrows sub_0805BB8C's with `lsls #0x18; lsrs #0x18` before
 * `cmp r0,#1`. agbcc emits neither for an `int` return, and both call sites are
 * the plain `if (f(...))` / `if (f(...) == 1)` spelling with no cast in sight.
 * The definitions in c_0805B980.c were changed to match and both re-verified
 * byte-identical, as predicted -- the bodies only ever produce 0 and 1, so
 * narrowing the return is free there. */
u8 sub_0805BA34(int, int, u16 *);
u8 sub_0805BB8C(int, int);

/* Wave 50, W50-D. sub_08032484 is already PROMOTED (src/decomp/c_08032484.c)
 * but was never declared here; sub_080324C4 is its first caller outside its
 * own unit, and passes `gBG0TilemapBuffer + 0x221`. The promoted definition
 * names the parameter type, so this must agree with it. */
void sub_08032484(u16 *);
/* Wave 50, W50-D. All four are void/void: sub_080324C4 and sub_08032BCC call
 * them with no argument register set up and never read r0 afterwards. */
void sub_08034290(void);
void sub_080328EC(void);
void sub_08032AFC(void);
/* Wave 50, W50-D. sub_08032BCC calls it as sub_08031B6C(&gUnknown_02027C2C[i
 * * 0x13], <the 0x34-byte stack buffer sub_0803CCB8(int, u8 *) just filled>),
 * result unused. sub_08032BCC itself is NOT declared here: it is a proc
 * callback reached only through a ProcCmd table, its parameter is a proc
 * struct local to its own unit, and a `void *` declaration here would
 * conflict with that definition. */
void sub_08031B6C(u8 *, u8 *);
/* Wave 50, W50-D. All four read off sub_08031638's call sites, which narrow
 * every result: sub_0802F408 is tested with a BARE `lsls #24` (a truth test on
 * a byte), sub_0802F4A0 with `lsls #24; lsrs #24; cmp #1` (a value kept, and
 * sub_080312AC tests the same function both ways), sub_0802F504 with
 * `lsls #24; asrs #24; cmp #1; ble` -- signed, i.e. the same s8 its neighbour
 * sub_0802F534 is already declared to return. sub_08030D84 is a bare tail call
 * with no argument set up and its result unread. */
bool8 sub_0802F408(void);
bool8 sub_0802F4A0(void);
s8 sub_0802F504(void);
void sub_08030D84(void);

/* Wave 51, W51-O. Two functions that are ALREADY DEFINED in src/decomp but had
 * no declaration in this header, so nothing outside their own file could call
 * them. Both signatures are copied from the promoted definition, which wins:
 * src/decomp/c_08038368.c and src/decomp/c_0805CA24.c. */
int sub_0803840C(void);
int sub_0805CA24(void);
/* sub_0805E778 hands it the dereferenced gUnknown_03003F20 list pointer, the
 * same value sub_08059E3C and sub_08059C00 take, and both of those are declared
 * `void *` here. Result unused at the only call site. */
void sub_0805A0EC(void *);

/* Wave 51, W51-P. Three callees of this batch that had no declaration here.
 * Each signature is copied from a definition that already exists and therefore
 * wins: work/sub_0808488C (matched, returns one of two u16 palette bases),
 * src/decomp/c_0807F8E4.c (`return Proc_Find(...) != 0;`, so `int` -- its
 * caller sub_0807C9EC re-narrows the result with `lsls #24; lsrs #24`, which is
 * the caller storing it in a u8, not a u8 return) and src/decomp/c_08087B20.c
 * (four ints; the fourth is a sprite-id base added to each decimal digit). */
u16 *sub_0808488C(int);
int sub_0807F8E4(void);
void sub_08087B20(int, int, int, int);

/* Wave 53, W53-A. Three callees of sub_0804A260 that had no declaration here.
 * sub_08013034 takes the NUL-terminated byte buffer sub_0804A260 has just
 * filled at gUnknown_030044E0 + 0x41 (one argument; the pointer is already in
 * r0 and nothing else is set up), and its result is discarded.
 * sub_0804A6A4 is nullary -- no argument register is written before the call
 * and its result is discarded.
 * sub_080741C4 takes THREE arguments: r0, r1 and r2 each get their own
 * `movs #0` at the only call site and r3 is left alone, which is the one arity
 * readout that is not a guess (an argument already in the right register would
 * have cost zero instructions, but a literal 0 never is). */
void sub_08013034(u8 *);
void sub_0804A6A4(void);
void sub_080741C4(int, int, int);

/* Wave 53, W53-C. Callees of the sub_0807C614 / sub_08081060 / sub_08085B30
 * screen-setup cluster that had no declaration here. Where a promoted
 * definition exists it wins and the signature is copied from it:
 * src/decomp/c_08078D40.c (sub_08078D80), src/decomp/c_08037750.c,
 * src/decomp/c_08084804.c, src/decomp/c_080878A8.c (sub_08087938).
 *
 * sub_080845C4 and sub_080845E8 are the exception, and they are the reason the
 * arity rule reads in BOTH directions. src/decomp/c_08084580.c defines them as
 * `(int i)` and `(void)`, but sub_08081060 -- their only caller in the ROM --
 * sets up r1 at all ten call sites. An argument the callee never reads costs
 * zero instructions in the callee, so a promoted definition cannot see it; the
 * caller is the only witness to the true arity. Both definitions were widened
 * to match and re-verified byte-identical. */
void sub_08078D80(ProcPtr);
void sub_08037750(int);
void sub_08084804(void);
void sub_08087938(void);
void sub_080845C4(int, int);
void sub_080845E8(int, int);
/* sub_08085B30 hands it `gUnknown_03005900 + gUnknown_03005930` (a u8 widened
 * into an int plus a u16) with the sum built in an int register and no
 * re-narrowing, and the result is unused. */
void sub_08086F3C(int);

#endif // UNKNOWN_FUNCS_H

/* Wave 56, W56-L. Declared from CALL-SITE evidence in sub_0800081C only --
 * neither is promoted and neither was matched here, so treat both as the
 * weakest model that fits rather than as measurements.
 *
 * sub_080085E0: `bl sub_080085E0` with NO argument setup at all in front of it
 * (the preceding insn is `beq`, and r0-r3 are dead across it), and the next
 * instruction is another argument-free `bl sub_08021D10`, so r0 is neither
 * supplied nor read -- void(void). This agrees with the two existing notes on
 * struct ActiveMap: unk2a is described as "the action code sub_080085E0
 * dispatches on" and unk6a as "a one-byte result code sub_080085E0 leaves
 * behind", i.e. it takes its input and returns its output through the
 * gActiveMap record, not through registers.
 *
 * sub_0800AEAC: called as `ldrsh r0,[r3,#8]; ldrsh r1,[r3,#0xa]; bl`, the same
 * (x, y) cell pair sub_0800B528, sub_0800BC5C, sub_08009310 and sub_08010DD4
 * take from the same two fields in the same dispatch chain -- all four of which
 * are already declared int(int, int). Its result is consumed by `cmp r0,#0;
 * bne`, a bare truth test, so the return is a value but its width is inherited
 * from the siblings rather than proved. */
void sub_080085E0(void);
int sub_0800AEAC(int, int);

/* Wave 56, W56-L. Three callees of sub_0800081C that had no declaration
 * anywhere despite ALREADY BEING PROMOTED. Each signature is COPIED FROM THE
 * PROMOTED DEFINITION, which is the stronger witness, not inferred from the
 * call site: src/decomp/c_0800105C.c `int sub_0800105C(void)`,
 * src/decomp/c_08004D10.c `void sub_08004D10(void)` and
 * src/decomp/c_0800BC5C.c `int sub_0800BC5C(int x, int y)`. Nothing changes
 * for the existing definitions; this only stops a fresh caller compiling them
 * as implicit-int. */
int sub_0800105C(void);
void sub_08004D10(void);
int sub_0800BC5C(int, int);
