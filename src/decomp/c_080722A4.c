#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080722A4.
 * sub_080722A4 @ 0x080722A4
 */

#include "proc.h"

/* A 42nd member of the 0x08071F88-0x08072288 fade-wrapper family, sitting just
 * past its end. Unlike the others it forwards BOTH of its parameters: the
 * parent in r0 and the completion callback in r1 move to r2/r3 untouched, and
 * only the kind and speed are constants. StartFadeCore's fourth parameter is
 * already declared `void (*)(void)`, which is what makes r1 a function pointer
 * rather than another word. */
void FadeInBlackWithCustomCallBack(ProcPtr parent, void (*onDone)(void))
{
    StartFadeCore(3, 0x40, parent, onDone);
}
asm(".global sub_080722A4\n.thumb_set sub_080722A4, FadeInBlackWithCustomCallBack\n");
