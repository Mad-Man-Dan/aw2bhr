#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071F88.
 * sub_08071F88 @ 0x08071F88, sub_08071F94 @ 0x08071F94, sub_08071FA0 @ 0x08071FA0, sub_08071FAC @ 0x08071FAC, sub_08071FB8 @ 0x08071FB8, sub_08071FC4 @ 0x08071FC4, sub_08071FD0 @ 0x08071FD0, sub_08071FE0 @ 0x08071FE0, sub_08071FF0 @ 0x08071FF0, sub_08072000 @ 0x08072000, sub_08072010 @ 0x08072010, sub_08072020 @ 0x08072020, sub_08072030 @ 0x08072030, sub_08072040 @ 0x08072040, sub_08072050 @ 0x08072050, sub_08072068 @ 0x08072068, sub_08072080 @ 0x08072080, sub_08072098 @ 0x08072098, sub_080720B0 @ 0x080720B0, sub_080720C8 @ 0x080720C8, sub_080720DC @ 0x080720DC, sub_080720F0 @ 0x080720F0, sub_08072104 @ 0x08072104, sub_08072118 @ 0x08072118, sub_08072130 @ 0x08072130, sub_08072148 @ 0x08072148, sub_08072160 @ 0x08072160, sub_08072178 @ 0x08072178, sub_08072190 @ 0x08072190, sub_080721A4 @ 0x080721A4, sub_080721B8 @ 0x080721B8, sub_080721D0 @ 0x080721D0, sub_080721E4 @ 0x080721E4, sub_080721F8 @ 0x080721F8, sub_0807220C @ 0x0807220C, sub_08072220 @ 0x08072220, sub_08072234 @ 0x08072234, sub_08072248 @ 0x08072248, sub_0807225C @ 0x0807225C, sub_08072270 @ 0x08072270, sub_08072288 @ 0x08072288
 */

#include "proc.h"

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071F88(void)
{
    sub_08011550(0x10);
}

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071F94(void)
{
    sub_08011550(4);
}

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071FA0(void)
{
    sub_08011550(0x40);
}

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071FAC(void)
{
    sub_0801156C(0x10);
}

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071FB8(void)
{
    sub_0801156C(4);
}

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on sub_08011550 in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void sub_08071FC4(void)
{
    sub_0801156C(0x40);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08071FD0(ProcPtr parent)
{
    sub_080115E0(0x10, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08071FE0(ProcPtr parent)
{
    sub_080115E0(4, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08071FF0(ProcPtr parent)
{
    sub_080115E0(0x40, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08072000(ProcPtr parent)
{
    sub_080115F8(0x10, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08072010(ProcPtr parent)
{
    sub_080115F8(4, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08072020(ProcPtr parent)
{
    sub_080115F8(0x40, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08072030(ProcPtr parent)
{
    sub_08011610(4, parent);
}

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that sub_080115E0/F8/sub_08011610/sub_0801163C forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void sub_08072040(ProcPtr parent)
{
    sub_0801163C(4, parent);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072050(ProcPtr parent)
{
    sub_080722B8(1, 4, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072068(ProcPtr parent)
{
    sub_080722B8(1, 8, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072080(ProcPtr parent)
{
    sub_080722B8(1, 0x10, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072098(ProcPtr parent)
{
    sub_080722B8(1, 0x20, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_080720B0(ProcPtr parent)
{
    sub_080722B8(1, 0x40, parent, sub_080723DC);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080720C8(ProcPtr parent)
{
    sub_080722B8(0, 8, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080720DC(ProcPtr parent)
{
    sub_080722B8(0, 0x10, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080720F0(ProcPtr parent)
{
    sub_080722B8(0, 0x20, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_08072104(ProcPtr parent)
{
    sub_080722B8(0, 0x40, parent, NULL);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072118(ProcPtr parent)
{
    sub_080722B8(3, 4, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072130(ProcPtr parent)
{
    sub_080722B8(3, 8, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072148(ProcPtr parent)
{
    sub_080722B8(3, 0x10, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072160(ProcPtr parent)
{
    sub_080722B8(3, 0x20, parent, sub_080723DC);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072178(ProcPtr parent)
{
    sub_080722B8(3, 0x40, parent, sub_080723DC);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_08072190(ProcPtr parent)
{
    sub_080722B8(2, 4, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080721A4(ProcPtr parent)
{
    sub_080722B8(2, 8, parent, NULL);
}

/* The only two-statement member of the family: it is the 24-byte outlier
 * sitting among the 20-byte NULL-callback wrappers, and the extra 4 bytes
 * are a second `bl`, not a pool word. Same call as sub_080721A4 (2, 8),
 * followed by sub_08072394. */
void sub_080721B8(ProcPtr parent)
{
    sub_080722B8(2, 8, parent, NULL);
    sub_08072394();
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080721D0(ProcPtr parent)
{
    sub_080722B8(2, 0x10, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080721E4(ProcPtr parent)
{
    sub_080722B8(2, 0x20, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_080721F8(ProcPtr parent)
{
    sub_080722B8(2, 0x40, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_0807220C(ProcPtr parent)
{
    sub_080722B8(6, 0x10, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_08072220(ProcPtr parent)
{
    sub_080722B8(7, 0x10, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_08072234(ProcPtr parent)
{
    sub_080722B8(6, 8, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_08072248(ProcPtr parent)
{
    sub_080722B8(4, 4, parent, NULL);
}

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void sub_0807225C(ProcPtr parent)
{
    sub_080722B8(4, 8, parent, NULL);
}

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void sub_08072270(ProcPtr parent)
{
    sub_080722B8(7, 8, parent, sub_08072454);
}

/* The tail of the family and a different shape entirely: not a forwarder.
 * `lsls r0, r0, #0x18` between the `bl` and the `cmp` is the narrowing of a
 * bool8/u8 return -- it is what fixes sub_080116A0 as 8 bits wide rather
 * than `int`, which would compare with a bare `cmp r0, #0`. So: while any
 * fade proc is alive, keep blocking; the frame the last one dies, break. */
void sub_08072288(ProcPtr proc)
{
    if (sub_080116A0() == 0)
        Proc_Break(proc);
}
