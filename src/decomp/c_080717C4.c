#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080717C4.
 * sub_080717C4 @ 0x080717C4, sub_080717E4 @ 0x080717E4
 */

/* `ply_xcmd` in the Fire Emblem decomps -- AW2 and FE share the Intelligent
 * Systems m4a sound driver, so the FE C is the starting point and this one
 * confirms byte-for-byte.
 *
 * Fetch one byte from the track's command cursor, post-incrementing it in
 * place, and dispatch through gUnknown_081BA00C. TWO arguments reach the
 * handler: `bl _call_via_r2` is the readout, since gcc puts an indirect
 * call's pointer in the first free scratch register and r0/r1 are taken.
 * Neither argument is typed by anything here -- r0 is never touched and the
 * track only ever has its +0x40 cursor read. */
struct Unk80717C4Track
{
    /* 0x00 */ u8 filler_00[0x40];
    /* 0x40 */ u8 *cmd;
};

void ply_xcmd(void *mplay, struct Unk80717C4Track *track)
{
    u32 n = *track->cmd++;

    gUnknown_081BA00C[n](mplay, track);
}
asm(".global sub_080717C4\n.thumb_set sub_080717C4, ply_xcmd\n");

/* One indirect call through a RAM function pointer, with both of this
 * wrapper's own arguments passed straight through. `ldr r2,=g; ldr r2,[r2]`
 * is a single load of the pointer VALUE with no index, so gUnknown_03005740
 * is one pointer and not a table; `bl _call_via_r2` counts the arguments at
 * two. Nothing here types either argument. */
void ply_xxx(void *a, void *b)
{
    gUnknown_03005740(a, b);
}
asm(".global sub_080717E4\n.thumb_set sub_080717E4, ply_xxx\n");
