#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08034F6C.
 * GetMapLock @ 0x08034F6C, IncrementMapLock @ 0x08034F7C
 *
 * Not a Xenesis-documented name. gGameLock.map is a lock DEPTH rather than a
 * flag -- read here, bumped here, decremented with a floor at zero by
 * DecrementMapLock and cleared outright by RemoveMapLock -- so this trio plus
 * RemoveMapLock is the map half of the lock set in include/lock.h. The old
 * sub_XXXXXXXX symbols are kept as linker aliases below so every other unit
 * keeps resolving them unchanged.
 */

/* `int` and not `s8`: byte-identical here either way (the `lsls #0x18;
 * asrs #0x18` is the s8 member read), but RunMapStateMachine tests the result with a
 * bare `cmp r0, #0` and an s8 return would have narrowed it there first.
 * Retyped in wave 13 (A2); re-verified byte-for-byte. */
int GetMapLock(void)
{
    return gGameLock.map;
}

void IncrementMapLock(void)
{
    gGameLock.map++;
}

asm(".global sub_08034F6C\n.thumb_set sub_08034F6C, GetMapLock\n"
    ".global sub_08034F7C\n.thumb_set sub_08034F7C, IncrementMapLock\n");
