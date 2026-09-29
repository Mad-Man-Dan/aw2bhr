#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019F2C.
 * sub_08019F2C @ 0x08019F2C, sub_08019F50 @ 0x08019F50
 */

/*
 * sub_08019F2C -- open an option list: a plain forwarder to CreateMenu.
 *
 * Passes its five parameters through and returns what CreateMenu returns,
 * which is the list object as an int. The four u16 parameters are narrowed on
 * entry and the fifth arrives on the stack.
 *
 * The `return` matters: the original's epilogue is the returns-a-value one, so
 * a void wrapper would not match.
 */
int sub_08019F2C(const void *a, u16 b, u16 c, u16 d, u16 e)
{
    return CreateMenu(a, b, c, d, e);
}

/* CreateRootMenu -- sub_08019F2C above with a sub_0801A604() call in front of it.
 * Same five parameters, same forwarded return value. */
int CreateRootMenu(const void *a, u16 b, u16 c, u16 d, u16 e)
{
    sub_0801A604();
    return CreateMenu(a, b, c, d, e);
}
asm(".global sub_08019F50\n.thumb_set sub_08019F50, CreateRootMenu\n");
