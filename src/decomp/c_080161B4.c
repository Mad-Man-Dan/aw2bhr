#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080161B4.
 * sub_080161B4 @ 0x080161B4, sub_080162A4 @ 0x080162A4, sub_08016370 @ 0x08016370, sub_0801642C @ 0x0801642C, sub_080164E0 @ 0x080164E0
 */


/* How the script commands below see a gUnknown_03001470 slot: from 0x3c on the
 * bytes are floats -- position, velocity, acceleration -- and a frame count.
 * struct Unk03001470 in include/unknown-globals.h describes the same bytes as
 * a function pointer and halfword flags, which is another subsystem's view of
 * the same scratch area; the original was most likely a union.
 *
 * Unk1470Wrap only exists so the slots can be written `gPath[a].member`. They
 * are reached that way, and not as `p[i].member` on a plain pointer, because
 * the original adds the member offset to the base address and then indexes; a
 * plain pointer folds the offset into the load instruction instead and the
 * output no longer matches. */
struct Unk1470Path
{
    /* 0x00 */ u8 filler_00[0x04];
    /* 0x04 */ const void *unk04;
    /* 0x08 */ u8 filler_08[0x30];
    /* 0x38 */ s16 unk38;
    /* 0x3a */ u8 filler_3a[0x02];
    /* 0x3c */ float unk3c;
    /* 0x40 */ float unk40;
    /* 0x44 */ u8 filler_44[0x08];
    /* 0x4c */ float unk4c;
    /* 0x50 */ float unk50;
    /* 0x54 */ float unk54;
    /* 0x58 */ float unk58;
    /* 0x5c */ int unk5c;
};
struct Unk1470Wrap /* not a real object: see above */
{
    /* 0x00 */ struct Unk1470Path unk00[30];
};
#define gPath (((struct Unk1470Wrap *)gUnknown_03001470)->unk00)


/*
 * SlotOp_MoveAlongOffsets -- script command: step along a list of per-frame offsets.
 *
 * The command word points at a list of s16 x/y pairs in ROM, one pair per
 * frame. The slot's .unk38 is both the cursor into that list and the frame
 * count: on the first frame the slot's current position is read out with
 * GetSlotSpritePosition and kept as the float origin, and every frame after that
 * SetSlotSpritePosition moves the sprite to the origin plus this frame's pair. An x of
 * -1 ends the list -- the tick is cleared and the script cursor steps 8 bytes
 * on to the next command. Returns FALSE, as all the command handlers in this
 * file do; what the caller reads into that is not visible here.
 *
 * Why the C looks odd: `r` is set from the command word and advanced in a
 * second statement. As one expression the compiler reads the command word
 * last, and the output no longer matches.
 */
bool8 SlotOp_MoveAlongOffsets(u8 a)
{
    s16 x, y;
    const s16 *r;

    if (gPath[a].unk38 == 0) {
        GetSlotSpritePosition(a, &x, &y);
        gPath[a].unk3c = x;
        gPath[a].unk40 = y;
    }
    r = *(const s16 **)gPath[a].unk04;
    r += gPath[a].unk38 * 2;
    if (r[0] == -1) {
        gPath[a].unk04 = (const u8 *)gPath[a].unk04 + 8;
        gPath[a].unk38 = 0;
    } else {
        SetSlotSpritePosition(a, gPath[a].unk3c + r[0], gPath[a].unk40 + r[1]);
        gPath[a].unk38++;
    }
    return FALSE;
}
asm(".global sub_080161B4\n.thumb_set sub_080161B4, SlotOp_MoveAlongOffsets\n");


/*
 * StepSlotSpriteMotion -- one frame of the smooth move that the three commands below
 * set up.
 *
 * Adds velocity to position and acceleration to velocity, pushes the position
 * out through SetSlotSpritePosition, and counts the frame off .unk5c. When that counter
 * was already 0 the command is done: the tick is cleared and the script cursor
 * steps 8 bytes on to the next command.
 *
 * Why the C looks odd: the counter is read and decremented in one go, as
 * `n = p->unk5c--`. Split into two statements the decrement becomes a
 * three-operand subtract instead of the original's copy-then-decrement; with
 * no local at all, `p->unk5c-- == 0` compares the decremented value against -1
 * instead.
 */
void StepSlotSpriteMotion(u8 a)
{
    int n;

    gPath[a].unk3c += gPath[a].unk4c;
    gPath[a].unk40 += gPath[a].unk50;
    gPath[a].unk4c += gPath[a].unk54;
    gPath[a].unk50 += gPath[a].unk58;
    SetSlotSpritePosition(a, gPath[a].unk3c, gPath[a].unk40);
    n = gPath[a].unk5c--;
    if (n == 0) {
        gPath[a].unk38 = 0;
        gPath[a].unk04 = (const u8 *)gPath[a].unk04 + 8;
    }
}
asm(".global sub_080162A4\n.thumb_set sub_080162A4, StepSlotSpriteMotion\n");


/*
 * SlotOp_MoveAccelerated -- script command: start a move under constant acceleration.
 *
 * The command word points at five floats: velocity x and y, acceleration x and
 * y, and a frame count. On the first frame only (the slot's tick is still 0)
 * the current position is read out with GetSlotSpritePosition as the float origin and
 * the five values are copied into the slot. Every frame then runs one step of
 * StepSlotSpriteMotion, which is what retires the command. Returns FALSE.
 */
bool8 SlotOp_MoveAccelerated(u8 a)
{
    s16 x, y;
    const float *q;

    q = *(const float **)gPath[a].unk04;
    if (gPath[a].unk38 == 0) {
        GetSlotSpritePosition(a, &x, &y);
        gPath[a].unk3c = x;
        gPath[a].unk40 = y;
        gPath[a].unk4c = q[0];
        gPath[a].unk50 = q[1];
        gPath[a].unk54 = q[2];
        gPath[a].unk58 = q[3];
        gPath[a].unk5c = q[4];
        gPath[a].unk38++;
    }
    StepSlotSpriteMotion(a);
    return FALSE;
}
asm(".global sub_08016370\n.thumb_set sub_08016370, SlotOp_MoveAccelerated\n");


/*
 * SlotOp_MoveX -- script command: move horizontally at a constant speed.
 *
 * SlotOp_MoveAccelerated's setup with both accelerations forced to 0, and with the
 * operands stored inline in the 8-byte command instead of behind its first
 * word: the signed word at +0 is the x velocity and the unsigned halfword at
 * +4 is the frame count. SlotOp_MoveY below is the vertical version. Returns
 * FALSE.
 */
bool8 SlotOp_MoveX(u8 a)
{
    s16 x, y;

    if (gPath[a].unk38 == 0) {
        GetSlotSpritePosition(a, &x, &y);
        gPath[a].unk3c = x;
        gPath[a].unk40 = y;
        gPath[a].unk4c = *(const int *)gPath[a].unk04;
        gPath[a].unk50 = 0;
        gPath[a].unk54 = 0;
        gPath[a].unk58 = 0;
        gPath[a].unk5c = ((const u16 *)gPath[a].unk04)[2];
        gPath[a].unk38++;
    }
    StepSlotSpriteMotion(a);
    return FALSE;
}
asm(".global sub_0801642C\n.thumb_set sub_0801642C, SlotOp_MoveX\n");


/*
 * SlotOp_MoveY -- script command: move vertically at a constant speed.
 * SlotOp_MoveX with the inline word driving y instead of x, and the other
 * three float members zeroed. Returns FALSE.
 */
bool8 SlotOp_MoveY(u8 a)
{
    s16 x, y;

    if (gPath[a].unk38 == 0) {
        GetSlotSpritePosition(a, &x, &y);
        gPath[a].unk3c = x;
        gPath[a].unk40 = y;
        gPath[a].unk4c = 0;
        gPath[a].unk50 = *(const int *)gPath[a].unk04;
        gPath[a].unk54 = 0;
        gPath[a].unk58 = 0;
        gPath[a].unk5c = ((const u16 *)gPath[a].unk04)[2];
        gPath[a].unk38++;
    }
    StepSlotSpriteMotion(a);
    return FALSE;
}
asm(".global sub_080164E0\n.thumb_set sub_080164E0, SlotOp_MoveY\n");
