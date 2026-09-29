#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08071F88.
 * sub_08071F88 @ 0x08071F88, sub_08071F94 @ 0x08071F94, sub_08071FA0 @ 0x08071FA0, sub_08071FAC @ 0x08071FAC, sub_08071FB8 @ 0x08071FB8, sub_08071FC4 @ 0x08071FC4, sub_08071FD0 @ 0x08071FD0, sub_08071FE0 @ 0x08071FE0, sub_08071FF0 @ 0x08071FF0, sub_08072000 @ 0x08072000, sub_08072010 @ 0x08072010, sub_08072020 @ 0x08072020, sub_08072030 @ 0x08072030, sub_08072040 @ 0x08072040, sub_08072050 @ 0x08072050, sub_08072068 @ 0x08072068, sub_08072080 @ 0x08072080, sub_08072098 @ 0x08072098, sub_080720B0 @ 0x080720B0, sub_080720C8 @ 0x080720C8, sub_080720DC @ 0x080720DC, sub_080720F0 @ 0x080720F0, sub_08072104 @ 0x08072104, sub_08072118 @ 0x08072118, sub_08072130 @ 0x08072130, sub_08072148 @ 0x08072148, sub_08072160 @ 0x08072160, sub_08072178 @ 0x08072178, sub_08072190 @ 0x08072190, sub_080721A4 @ 0x080721A4, sub_080721B8 @ 0x080721B8, sub_080721D0 @ 0x080721D0, sub_080721E4 @ 0x080721E4, sub_080721F8 @ 0x080721F8, sub_0807220C @ 0x0807220C, sub_08072220 @ 0x08072220, sub_08072234 @ 0x08072234, sub_08072248 @ 0x08072248, sub_0807225C @ 0x0807225C, sub_08072270 @ 0x08072270, sub_08072288 @ 0x08072288
 */

#include "proc.h"

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartMidFadeToBlack(void)
{
    StartFadeToBlack(0x10);
}
asm(".global sub_08071F88\n.thumb_set sub_08071F88, StartMidFadeToBlack\n");

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartSlowFadeToBlack(void)
{
    StartFadeToBlack(4);
}
asm(".global sub_08071F94\n.thumb_set sub_08071F94, StartSlowFadeToBlack\n");

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartFastFadeToBlack(void)
{
    StartFadeToBlack(0x40);
}
asm(".global sub_08071FA0\n.thumb_set sub_08071FA0, StartFastFadeToBlack\n");

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartMidFadeFromBlack(void)
{
    StartFadeFromBlack(0x10);
}
asm(".global sub_08071FAC\n.thumb_set sub_08071FAC, StartMidFadeFromBlack\n");

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartSlowFadeFromBlack(void)
{
    StartFadeFromBlack(4);
}
asm(".global sub_08071FB8\n.thumb_set sub_08071FB8, StartSlowFadeFromBlack\n");

/* One of the 41 fade wrappers at 0x08071F88-0x08072288. Argument is a fade
 * SPEED, not a mask -- see the comment on StartFadeToBlack in
 * include/unknown-functions.h. `pop {r0}` in the epilogue makes this void, and
 * the callee is void too, so nothing is forwarded. */
void StartFastFadeFromBlack(void)
{
    StartFadeFromBlack(0x40);
}
asm(".global sub_08071FC4\n.thumb_set sub_08071FC4, StartFastFadeFromBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartMidLockingFadeToBlack(ProcPtr parent)
{
    StartLockingFadeToBlack(0x10, parent);
}
asm(".global sub_08071FD0\n.thumb_set sub_08071FD0, StartMidLockingFadeToBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartSlowLockingFadeToBlack(ProcPtr parent)
{
    StartLockingFadeToBlack(4, parent);
}
asm(".global sub_08071FE0\n.thumb_set sub_08071FE0, StartSlowLockingFadeToBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartFastLockingFadeToBlack(ProcPtr parent)
{
    StartLockingFadeToBlack(0x40, parent);
}
asm(".global sub_08071FF0\n.thumb_set sub_08071FF0, StartFastLockingFadeToBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartMidLockingFadeFromBlack(ProcPtr parent)
{
    StartLockingFadeFromBlack(0x10, parent);
}
asm(".global sub_08072000\n.thumb_set sub_08072000, StartMidLockingFadeFromBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartSlowLockingFadeFromBlack(ProcPtr parent)
{
    StartLockingFadeFromBlack(4, parent);
}
asm(".global sub_08072010\n.thumb_set sub_08072010, StartSlowLockingFadeFromBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartFastLockingFadeFromBlack(ProcPtr parent)
{
    StartLockingFadeFromBlack(0x40, parent);
}
asm(".global sub_08072020\n.thumb_set sub_08072020, StartFastLockingFadeFromBlack\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartSlowLockingFadeToWhite(ProcPtr parent)
{
    StartLockingFadeToWhite(4, parent);
}
asm(".global sub_08072030\n.thumb_set sub_08072030, StartSlowLockingFadeToWhite\n");

/* Fade wrapper. The incoming parameter is untouched except for
 * `adds r1, r0, #0`, so it becomes the callee's SECOND argument -- the parent
 * proc that StartLockingFadeToBlack/F8/StartLockingFadeToWhite/StartLockingFadeFromWhite forward to
 * Proc_StartBlocking. The constant is the fade speed. */
void StartSlowLockingFadeFromWhite(ProcPtr parent)
{
    StartLockingFadeFromWhite(4, parent);
}
asm(".global sub_08072040\n.thumb_set sub_08072040, StartSlowLockingFadeFromWhite\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToBlackWithCallBack_Speed04(ProcPtr parent)
{
    StartFadeCore(1, 4, parent, Fade_CommonCallBack);
}
asm(".global sub_08072050\n.thumb_set sub_08072050, FadeCoreToBlackWithCallBack_Speed04\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToBlackWithCallBack_Speed08(ProcPtr parent)
{
    StartFadeCore(1, 8, parent, Fade_CommonCallBack);
}
asm(".global sub_08072068\n.thumb_set sub_08072068, FadeCoreToBlackWithCallBack_Speed08\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToBlackWithCallBack_Speed10(ProcPtr parent)
{
    StartFadeCore(1, 0x10, parent, Fade_CommonCallBack);
}
asm(".global sub_08072080\n.thumb_set sub_08072080, FadeCoreToBlackWithCallBack_Speed10\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToBlackWithCallBack_Speed20(ProcPtr parent)
{
    StartFadeCore(1, 0x20, parent, Fade_CommonCallBack);
}
asm(".global sub_08072098\n.thumb_set sub_08072098, FadeCoreToBlackWithCallBack_Speed20\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToBlackWithCallBack_Speed40(ProcPtr parent)
{
    StartFadeCore(1, 0x40, parent, Fade_CommonCallBack);
}
asm(".global sub_080720B0\n.thumb_set sub_080720B0, FadeCoreToBlackWithCallBack_Speed40\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromBlack_Speed08(ProcPtr parent)
{
    StartFadeCore(0, 8, parent, NULL);
}
asm(".global sub_080720C8\n.thumb_set sub_080720C8, FadeCoreFromBlack_Speed08\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromBlack_Speed10(ProcPtr parent)
{
    StartFadeCore(0, 0x10, parent, NULL);
}
asm(".global sub_080720DC\n.thumb_set sub_080720DC, FadeCoreFromBlack_Speed10\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromBlack_Speed20(ProcPtr parent)
{
    StartFadeCore(0, 0x20, parent, NULL);
}
asm(".global sub_080720F0\n.thumb_set sub_080720F0, FadeCoreFromBlack_Speed20\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromBlack_Speed40(ProcPtr parent)
{
    StartFadeCore(0, 0x40, parent, NULL);
}
asm(".global sub_08072104\n.thumb_set sub_08072104, FadeCoreFromBlack_Speed40\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeInBlackWithCallBack_Speed04(ProcPtr parent)
{
    StartFadeCore(3, 4, parent, Fade_CommonCallBack);
}
asm(".global sub_08072118\n.thumb_set sub_08072118, FadeInBlackWithCallBack_Speed04\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeInBlackWithCallBack_Speed08(ProcPtr parent)
{
    StartFadeCore(3, 8, parent, Fade_CommonCallBack);
}
asm(".global sub_08072130\n.thumb_set sub_08072130, FadeInBlackWithCallBack_Speed08\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeInBlackWithCallBack_Speed10(ProcPtr parent)
{
    StartFadeCore(3, 0x10, parent, Fade_CommonCallBack);
}
asm(".global sub_08072148\n.thumb_set sub_08072148, FadeInBlackWithCallBack_Speed10\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeInBlackWithCallBack_Speed20(ProcPtr parent)
{
    StartFadeCore(3, 0x20, parent, Fade_CommonCallBack);
}
asm(".global sub_08072160\n.thumb_set sub_08072160, FadeInBlackWithCallBack_Speed20\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeInBlackWithCallBack_Speed40(ProcPtr parent)
{
    StartFadeCore(3, 0x40, parent, Fade_CommonCallBack);
}
asm(".global sub_08072178\n.thumb_set sub_08072178, FadeInBlackWithCallBack_Speed40\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeInBlackSpeed04(ProcPtr parent)
{
    StartFadeCore(2, 4, parent, NULL);
}
asm(".global sub_08072190\n.thumb_set sub_08072190, FadeInBlackSpeed04\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeInBlackSpeed08(ProcPtr parent)
{
    StartFadeCore(2, 8, parent, NULL);
}
asm(".global sub_080721A4\n.thumb_set sub_080721A4, FadeInBlackSpeed08\n");

/* The only two-statement member of the family: it is the 24-byte outlier
 * sitting among the 20-byte NULL-callback wrappers, and the extra 4 bytes
 * are a second `bl`, not a pool word. Same call as FadeInBlackSpeed08 (2, 8),
 * followed by ExcludeObjPalettesFromFadeCore. */
void FadeInBlackSpeed08Unk(ProcPtr parent)
{
    StartFadeCore(2, 8, parent, NULL);
    ExcludeObjPalettesFromFadeCore();
}
asm(".global sub_080721B8\n.thumb_set sub_080721B8, FadeInBlackSpeed08Unk\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeInBlackSpeed10(ProcPtr parent)
{
    StartFadeCore(2, 0x10, parent, NULL);
}
asm(".global sub_080721D0\n.thumb_set sub_080721D0, FadeInBlackSpeed10\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeInBlackSpeed20(ProcPtr parent)
{
    StartFadeCore(2, 0x20, parent, NULL);
}
asm(".global sub_080721E4\n.thumb_set sub_080721E4, FadeInBlackSpeed20\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeInBlackSpeed40(ProcPtr parent)
{
    StartFadeCore(2, 0x40, parent, NULL);
}
asm(".global sub_080721F8\n.thumb_set sub_080721F8, FadeInBlackSpeed40\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromWhiteLocking_Speed10(ProcPtr parent)
{
    StartFadeCore(6, 0x10, parent, NULL);
}
asm(".global sub_0807220C\n.thumb_set sub_0807220C, FadeCoreFromWhiteLocking_Speed10\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreToWhiteLocking_Speed10(ProcPtr parent)
{
    StartFadeCore(7, 0x10, parent, NULL);
}
asm(".global sub_08072220\n.thumb_set sub_08072220, FadeCoreToWhiteLocking_Speed10\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromWhiteLocking_Speed08(ProcPtr parent)
{
    StartFadeCore(6, 8, parent, NULL);
}
asm(".global sub_08072234\n.thumb_set sub_08072234, FadeCoreFromWhiteLocking_Speed08\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromWhite_Speed04(ProcPtr parent)
{
    StartFadeCore(4, 4, parent, NULL);
}
asm(".global sub_08072248\n.thumb_set sub_08072248, FadeCoreFromWhite_Speed04\n");

/* Fade wrapper with no completion callback: the NULL fourth argument is a
 * `movs r3, #0`, which is also why r3 is set LAST here and second in the
 * pool-loaded variants -- agbcc expands the memory operand first. */
void FadeCoreFromWhite_Speed08(ProcPtr parent)
{
    StartFadeCore(4, 8, parent, NULL);
}
asm(".global sub_0807225C\n.thumb_set sub_0807225C, FadeCoreFromWhite_Speed08\n");

/* Fade wrapper. The fourth argument comes out of the literal pool because it
 * is a FUNCTION POINTER; passing it by name is what makes the pool word carry a
 * relocation instead of a bare constant. Argument 0 selects the
 * gUnknown_081CBF68 record, argument 1 is the fade speed. */
void FadeCoreToWhiteLockingWithCallBack_Speed08(ProcPtr parent)
{
    StartFadeCore(7, 8, parent, Fade_WhiteCallBack);
}
asm(".global sub_08072270\n.thumb_set sub_08072270, FadeCoreToWhiteLockingWithCallBack_Speed08\n");

/* The tail of the family and a different shape entirely: not a forwarder.
 * `lsls r0, r0, #0x18` between the `bl` and the `cmp` is the narrowing of a
 * bool8/u8 return -- it is what fixes FadeExists as 8 bits wide rather
 * than `int`, which would compare with a bare `cmp r0, #0`. So: while any
 * fade proc is alive, keep blocking; the frame the last one dies, break. */
void WaitForFade(ProcPtr proc)
{
    if (FadeExists() == 0)
        Proc_Break(proc);
}
asm(".global sub_08072288\n.thumb_set sub_08072288, WaitForFade\n");
