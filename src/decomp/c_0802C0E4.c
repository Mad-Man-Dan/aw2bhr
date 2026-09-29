#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C0E4.
 * sub_0802C0E4 @ 0x0802C0E4, sub_0802C0E8 @ 0x0802C0E8, sub_0802C118 @ 0x0802C118
 */

/* An empty function. `bx lr` with no `push` at all is agbcc's leaf epilogue for
 * a body that does nothing, and the two bytes after it are the `.align 2, 0`
 * ahead of the 4-aligned StartMainMenuForGameMode -- here the splitter counted that pad as
 * part of this function rather than giving it its own symbol, which is the
 * opposite of what it did to OptionsMenu_DeleteUsability's pad at 0x0802C62A.
 */

void sub_0802C0E4(void)
{
}

/* A three-way dispatch on the mode selector gPlaySt.gameMode -- the
 * same 1/2/3 the three sub_0803Bxxx starters write into it. The
 * `cmp #2; beq / cmp #2; bgt / cmp #1; beq` tree with the literal pool sitting
 * INSIDE it is agbcc's balanced switch over three case values, not an if-chain,
 * and cases 1 and 2 share a block: case 1 falls through into case 2's
 * StartMainMenu(). Case 3 is a separate block calling the same function, so the
 * source lists it separately rather than folding it into case 2.
 *
 * The parameter is never read -- the body opens by loading gPlaySt
 * straight over r0 -- so its width is settled entirely at the only call site,
 * QuitToMainMenu, which hands it the u16 global gUnknown_030033EC with a bare
 * `ldrb`. A byte load out of a halfword global is what a u8 parameter costs;
 * an `int` parameter would have emitted `ldrh`. `pop {r0}; bx r0`, so void.
 */

void StartMainMenuForGameMode(u8 a)
{
    switch (gPlaySt.gameMode)
    {
    case 1:
        ReloadProgressFromProfile();
        /* fallthrough */
    case 2:
        StartMainMenu();
        break;

    case 3:
        StartMainMenu();
        break;
    }
}
asm(".global sub_0802C0E8\n.thumb_set sub_0802C0E8, StartMainMenuForGameMode\n");

/* Two bare statements. StartMainMenuForGameMode discards its argument, so the `ldrb` of
 * the u16 gUnknown_030033EC is the only evidence for that parameter's width --
 * see the note on StartMainMenuForGameMode in include/unknown-functions.h.
 * `pop {r0}; bx r0`, so void.
 */

void QuitToMainMenu(void)
{
    sub_080366A4();
    StartMainMenuForGameMode(gUnknown_030033EC);
}
asm(".global sub_0802C118\n.thumb_set sub_0802C118, QuitToMainMenu\n");
