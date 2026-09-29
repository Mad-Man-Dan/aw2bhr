#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080784C8.
 * sub_080784C8 @ 0x080784C8
 */

#include "proc.h"

/* Family F032, and byte-identical to BlockingCoSpeech_Wait2 -- same nullary bool8
 * predicate, same break. See the note there on why IsCoSpeechScriptRunning takes no
 * argument despite the untouched r0. */

void BlockingCoSpeech_Wait(ProcPtr proc)
{
    if (IsCoSpeechScriptRunning() == 0)
        Proc_Break(proc);
}
asm(".global sub_080784C8\n.thumb_set sub_080784C8, BlockingCoSpeech_Wait\n");
