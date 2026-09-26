#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016824.
 * sub_08016824 @ 0x08016824, sub_080168BC @ 0x080168BC
 */


/*
 * sub_08016824 -- switch a sprite into rotate/scale mode.
 *
 * `a` is a gUnknown_03001470 slot, and its .unk26 names the OBJ in
 * gUnknown_0200E438. sub_0801DAB0 hands out a free affine matrix; the matrix
 * index is recorded on the OBJ's .unk3a, and then the OBJ's OAM attributes are
 * fetched with sub_0801566C, patched and handed back with sub_08015608:
 * affine mode on, and the matrix index split across three OAM fields -- its
 * low 3 bits are matrixNum, bit 3 is hFlip and bit 4 is vFlip. sub_080168BC
 * below undoes all of this.
 *
 * Why the C looks odd: sub_0801DAB0 is called with no argument at all, because
 * the original passes on whatever the argument register already held, which
 * here is this function's own parameter. Writing `sub_0801DAB0(a)` adds a
 * register copy the original does not have; see the note on the prototype in
 * include/unknown-functions.h. `v` is a real s8 local rather than an int with
 * casts, which is what puts one sign-extension after the store and another at
 * its one signed use.
 */
void sub_08016824(s16 a)
{
    struct OamData o;
    int m;
    s8 v;

    m = sub_0801DAB0();
    gUnknown_0200E438[gUnknown_03001470[a].unk26].unk3a = m;
    v = m;
    sub_0801566C(a, (struct UnkVec *)&o);
    o.affineEnable = 1;
    o.matrixNum = v & 7;
    o.hFlip = (v & 8) >> 3;
    o.vFlip = (v & 0x10) >> 4;
    sub_08015608(a, *(struct UnkVec *)&o);
}


/*
 * sub_080168BC -- take a sprite back out of rotate/scale mode.
 *
 * Undoes sub_08016824 above: sub_0801DAE8 releases the affine matrix the OBJ
 * was holding, the OBJ's .unk3a goes to 0xFFFF to mark it as holding none, and
 * the OAM fields sub_08016824 set are cleared through the same fetch, patch
 * and hand-back pair. doubleSize is cleared here although sub_08016824 never
 * sets it.
 *
 * Why the C looks odd: `a` is an int with `(s16)` casts rather than an s16
 * parameter -- the casts sign-extend it, where a declared s16 parameter would
 * arrive zero-extended -- and the "no matrix" value is written 0xFFFF and not
 * -1, because the original loads it as a stored constant instead of building
 * it out of 1.
 */
void sub_080168BC(int a)
{
    struct OamData o;

    sub_0801DAE8(gUnknown_0200E438[gUnknown_03001470[(s16)a].unk26].unk3a);
    gUnknown_0200E438[gUnknown_03001470[(s16)a].unk26].unk3a = 0xFFFF;
    sub_0801566C(a, (struct UnkVec *)&o);
    o.affineEnable = 0;
    o.doubleSize = 0;
    o.matrixNum = 0;
    o.hFlip = 0;
    o.vFlip = 0;
    sub_08015608(a, *(struct UnkVec *)&o);
}
