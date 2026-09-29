#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08075E68.
 * sub_08075E68 @ 0x08075E68
 */

#include "proc.h"
/* Starts gUnknown_08614410 blocking under a2 and seeds it from RemoveWorldMapMarker's
 * 12-byte out-record, converting that record's absolute (x, y) into camera
 * space by subtracting gUnknown_0202FDFC's origin pair.
 * a1 is an INDEX, not a pointer: its other use is `&gUnknown_0202FDFC.unk12[a1]`
 * -- the ROM adds it to the pool word plus 0x12 rather than dereferencing it.
 * The `& 0xFE | 2` on that byte is a plain scalar pair: a bitfield write would
 * have arrived at 0xFE by negation the way c_080755F0.c's 0xDF does. */
/* Wave 38: this file used to define its own `struct Unk8074834Out` for
 * RemoveWorldMapMarker's out-parameter -- s16 at +0/+2/+4/+6 and a pointer at +8. W38-B
 * matched RemoveWorldMapMarker itself and identified that record as `struct
 * Unk0202FE38`, the 12-byte list entry it looks up, with byte-identical layout.
 * The local tag was a second name for one object, so it is gone and the header
 * type is used directly; the only member difference is that unk08 is typed
 * `struct Unk0801C210 *` there rather than `void *`, which assigns to this
 * file's `void *unk54` implicitly. Byte-for-byte unchanged. */
struct Unk8075E68
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ int unk2c;
    /* 0x30 */ int unk30;
    /* 0x34 */ u8 filler_34[0x20];
    /* 0x54 */ void *unk54;
    /* 0x58 */ s32 unk58;
};

void StartWorldMapMissionClear(s32 a1, ProcPtr a2)
{
    struct Unk8075E68 *proc;
    struct Unk0202FE38 v;

    proc = Proc_StartBlocking(gUnknown_08614410, a2);
    proc->unk58 = a1;

    RemoveWorldMapMarker(a1, &v);

    proc->unk2c = v.unk02 - gUnknown_0202FDFC.unk00;
    proc->unk30 = v.unk04 - gUnknown_0202FDFC.unk02;
    proc->unk54 = v.unk08;

    gUnknown_0202FDFC.unk12[a1] = (gUnknown_0202FDFC.unk12[a1] & 0xFE) | 2;
}
asm(".global sub_08075E68\n.thumb_set sub_08075E68, StartWorldMapMissionClear\n");
