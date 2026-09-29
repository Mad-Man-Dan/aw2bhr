#ifndef GUARD_BATTLE_H
#define GUARD_BATTLE_H

/* The two IWRAM battle records and the layout they share. Moved out of
 * include/unknown-globals.h; every evidence comment below is the original
 * wave note, unchanged.
 *
 * Requires struct Unit from include/unit.h, which global.h includes
 * first. struct Unk802C57C still lives in unknown-globals.h, which is
 * included after this file, so the tag is forward-declared here rather than
 * inside a parameter list (agbcc warns, and warnings are errors).
 */
struct Unk802C57C;

/* An IWRAM record whose ADDRESS is what gets passed around -- eleven functions
 * name it and every one of them loads a clean `.4byte gUnknown_030013D0` and
 * either hands it to a routine or reads a fixed offset off it. Offsets seen:
 *
 *   +0x00  a pointer (`ldr r0,[r5]`, then `ldrb [r0,#4]` / `strh [r0,#4]`)
 *   +0x08  read BOTH as s16 (`movs r1,#8; ldrsh r0,[r5,r1]`, sub_0802505C and
 *          sub_080250E8) and as a byte masked with 0x7f (`ldrb r1,[r5,#8]`)
 *   +0x0a  u16   +0x14  u16   +0x18  s16
 *
 * so at least 0x1a bytes are in use. NOT a scalar and not const (it is RAM and
 * sub_0802505C writes through +0x00). Two independent hints at the extent, and
 * they disagree, which is why nothing stronger is declared here:
 * sub_080252EC hands gUnknown_030013D0 and gUnknown_030013B0 to the same stub
 * one after the other, a 0x20 delta suggesting a 0x20-byte record; but
 * gUnknown_030013EC is a separate symbol only 0x1c in. sub_080250E8 calls
 * sub_0802505C twice, on this and on `*gUnknown_08090A40`, so whatever the
 * record is, there is more than one of them and one is reached by pointer.
 * The ABI symbol remains `u8 []` for clean pool words; the typed C views
 * below share one BattleUnit layout. */
extern u8 gUnknown_030013D0[];
/* WAVE 35 (W35-C): the record's layout is now settled for +0x00..+0x14 and the
 * struct below is that model, derived from sub_08024DDC (MATCHED byte-for-byte
 * with it, first attempt) and corroborated by sub_08024E60. It is declared
 * here rather than locally in one .c because sub_08024DDC's prototype names it
 * and two units need to agree.
 *
 * +0x00 is a `struct Unit *` -- sub_08024DDC reaches unk04_0 through it
 * as `ldr r0,[r4]; ldrb r1,[r0,#4]`, the SAME 7-bit HP field the
 * sub_08025D20 note in unknown-functions.h describes, and sub_08058A2C was
 * already read the same way (see the gUnknown_0816D948 note above). That is two
 * independent readers agreeing on the member type, which is what the brief asks
 * for before believing a byte-neutral type.
 *
 * Every one of +0x08/+0x0c/+0x0e/+0x12/+0x14 is s16 and that is MEASURED, not
 * assumed: sub_08024DDC reads all five with `movs rI,#K; ldrsh rD,[rB,rI]` (the
 * s16-OBJECT tell -- ldrsh has no immediate-offset form, hence the scratch) and
 * writes them with `strh`. A u16 member feeding the same signed multiply would
 * have given `ldrh` plus an explicit shift pair. +0x08 agrees with the +0x08
 * s16 already recorded for this record via sub_08058A2C.
 *
 * Xenesis identifies +0x04 as the terrain ID. The code copies the unit
 * ammo bitfield into +0x0a, so that field is remaining ammunition. +0x18
 * records the attack choice used to index presentation data. Only +0x16
 * remains filler; the full extent is still unproved. */
struct BattleUnit /* head only; extent unproved */
{
    /* 0x00 */ struct Unit *unit;
    /* 0x04 */ u16 terrainId;
    /* 0x06 */ s16 terrainDefense;
    /* 0x08 */ s16 remainingHp;
    /* 0x0a */ u16 ammo;
    /* 0x0c */ s16 damage;
    /* 0x0e */ s16 totalDefense;
    /* 0x10 */ s16 baseDamage;
    /* 0x12 */ s16 hpLoss;
    /* 0x14 */ s16 displayDamage;
    /* 0x16 */ u8 filler_16[2];
    /* 0x18 */ s16 attackType;
};
/* The second record of that same type, and the one that pins them as two
 * instances rather than one object: sub_08041B98 reads +0x18 as `ldrsh` and
 * +0x04 as `ldrh` through BOTH symbols, four instructions apart, with the same
 * displacements. sub_080252EC (wave 12) hands each of them in turn to the
 * sub_080252E8 stub -- gUnknown_030013D0 first, then this one, which is source
 * order and not address order. Same `u8 []` model and the same reason. */
extern u8 gUnknown_030013B0[];

/* Typed C views; the original linker symbols remain for immutable asm. */
#define gBattleDefender ((struct BattleUnit *)gUnknown_030013B0)
#define gBattleAttacker ((struct BattleUnit *)gUnknown_030013D0)

/* Named battle entry points; each is a linker alias for the sub_XXXXXXXX
 * symbol its .c file .thumb_sets, so the declarations in
 * include/unknown-functions.h stay and existing callers compile unchanged. */
void SelectBattleWeapon(struct BattleUnit *a, struct BattleUnit *b, s16 c, u8 d);
void CalcBattleDamage(s16 a1, s16 a2, struct Unk802C57C *a3);

#endif /* GUARD_BATTLE_H */
