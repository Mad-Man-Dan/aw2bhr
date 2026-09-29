#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08037610.
 * sub_08037610 @ 0x08037610
 */

/* Family F035, exemplar src/decomp/c_0803B6E8.c. Same shape -- save the
 * argument, start something, store the argument into the result -- but the
 * starter is sub_080152EC rather than Proc_Start, the second argument is a
 * literal 0 rather than PROC_TREE_3, and the store lands at +0x18.
 *
 * The exemplar declares its own `struct UnkB6E8Proc` because Proc_Start returns
 * a bare ProcPtr and nothing else describes that object. Here the opposite
 * holds: sub_080152EC is prototyped `struct Unk03001470 *sub_080152EC(const
 * void *, u8)`, that struct IS declared in include/unknown-globals.h, and +0x18
 * fell inside its filler_15 -- so the right move is to narrow the filler in the
 * header (the "adding a field is normal" case), not to invent a local type that
 * would implicitly deny sub_080152EC's declared return type. */

void StartMapPreviewPictureScript(int a)
{
    struct Unk03001470 *slot = sub_080152EC(gUnknown_0849D41C, 0);

    slot->unk18 = a;
}
asm(".global sub_08037610\n.thumb_set sub_08037610, StartMapPreviewPictureScript\n");
