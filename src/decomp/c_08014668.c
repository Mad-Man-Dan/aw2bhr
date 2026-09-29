#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014668.
 * sub_08014668 @ 0x08014668, sub_080146D4 @ 0x080146D4
 */

/* Rebuilds the gUnknown_0200C020 record and starts script gUnknown_08489548.
 * Byte-identical twin of sub_080146D4, which names gUnknown_08489568 instead
 * -- the only difference between the two, and the only reason they are two
 * functions.
 *
 * The address of gUnknown_0200C020 is bound to a LOCAL before the
 * gUnknown_03002514 store, not written out at each of its two uses: the ROM
 * materialises that pool word first and reloads r0 from r8 at both calls,
 * where naming the global twice materialises gUnknown_03002514's word first
 * and lets r0 survive, two instructions short.
 *
 * Arguments 4..6 are u16 (narrowed in the prologue, ahead of the store);
 * 1 and 2 are int and are cast to InitTextWriter's s16 parameters at the call.
 * It returns sub_080152EC's proc pointer -- `pop {r1}; bx r1`.
 */
struct Unk03001470 *StartTextBoxViaRecord(int a, int b, u16 *c, u16 d, u16 e, u16 f)
{
    struct Unk08014074 *s;

    s = &gUnknown_0200C020;
    gUnknown_03002514 = 0;
    InitTextWriter(s, a, b, c, d, e, f);
    TextWriterDisableDelay(s);

    return sub_080152EC(gUnknown_08489548, 0);
}
asm(".global sub_08014668\n.thumb_set sub_08014668, StartTextBoxViaRecord\n");

/* Byte-identical twin of StartTextBoxViaRecord -- SAME SOURCE apart from the script
 * blob it starts (gUnknown_08489568 here, gUnknown_08489548 there). See
 * StartTextBoxViaRecord for why the record's address is bound to a local and why
 * arguments 4..6 are u16. */
struct Unk03001470 *sub_080146D4(int a, int b, u16 *c, u16 d, u16 e, u16 f)
{
    struct Unk08014074 *s;

    s = &gUnknown_0200C020;
    gUnknown_03002514 = 0;
    InitTextWriter(s, a, b, c, d, e, f);
    TextWriterDisableDelay(s);

    return sub_080152EC(gUnknown_08489568, 0);
}
