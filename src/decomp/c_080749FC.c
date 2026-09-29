#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080749FC.
 * sub_080749FC @ 0x080749FC
 */

#include "proc.h"
/* The proc struct and the `const u8 *` at +0x2c are c_08074AAC.c's, which is
 * what starts this proc; the byte it points at is a SIGNED id (`ldrb` then an
 * explicit `lsls #0x18; asrs #0x18`) indexing the 0x30-byte gUnknown_08615194
 * records for an (x, y) pair.
 *
 * `p` has to be its OWN statement.  The `ldr r3, =gUnknown_08615194` is the
 * table's address constant and agbcc emits it at the head of the statement
 * that first references the table: fold the `proc->unk_2c` load into that
 * statement and the pool `ldr` moves ahead of it, leave the subscript inline
 * in the call arguments and the pool `ldr` sinks past the whole (s8) cast.
 * The ROM has it between the two, which is one statement boundary and 6 bytes.
 * Same lever as LoadCoPaletteVariant in the 0x08043000 block.
 */
struct Unk8074AAC
{
    /* 0x00 */ u8 filler_00[0x2c];
    /* 0x2c */ const u8 *unk_2c;
};

void MarkerReveal_PanToMarker(struct Unk8074AAC *proc)
{
    const u8 *p = proc->unk_2c;
    const struct Unk08615194 *r = &gUnknown_08615194[(s8)*p];

    StartWorldMapCameraPan(proc, r->flagX, r->flagY, 1);
}
asm(".global sub_080749FC\n.thumb_set sub_080749FC, MarkerReveal_PanToMarker\n");
