#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08018DF8.
 * sub_08018DF8 @ 0x08018DF8
 */

/* An installed slot callback that draws the node's coordinates relative to the
 * gUnknown_08499590 viewport. The slot's +0x14 word -- declared `u32`, the only
 * other access being StartCoSpeechScript's store -- is dereferenced as a node here, so
 * it is cast rather than retyped.
 *
 * The node's unk08/unk0a are declared `u16` and read `ldrsh`, so they are cast,
 * the same treatment they get in c_08017C4C.c. The viewport's +4/+6 are the
 * camera column/row pair documented on gUnknown_08499590.
 *
 * gUnknown_08499590 is named INLINE at both uses and deliberately not bound to a
 * local: binding it moves its `ldr` ahead of the node's own unk08 read, which is
 * 7 bytes wrong. The binding local is punctuation, and here there was no
 * statement to punctuate -- CSE hoists the pointer to its first real use, which
 * is inside the subtraction. */
void EventCursorScript_Draw(struct Unk0200C528 *slot)
{
    struct Unk0200C528Node *p;

    p = (struct Unk0200C528Node *)slot->unk14;
    DrawMapCursorSprite((s16)p->unk08 - gMap->scrollX,
                 (s16)p->unk0a - gMap->scrollY,
                 p->unk0c);
}
asm(".global sub_08018DF8\n.thumb_set sub_08018DF8, EventCursorScript_Draw\n");
