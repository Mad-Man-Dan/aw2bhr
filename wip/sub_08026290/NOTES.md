
## wave 97 (W97-L)
Base: previous draft (28.8%, +8, `sub sp,#4` spill of i+1) kept as sub_08026290.w97L-start.c. New source
(sub_08026290.c, = vc.c): bind the address of the current army's CO byte before the retry loop
(`ci = &gPlaySt.co[i]; ... *ci = v;`), inner loop still on bare `gPlaySt.aiControlled[j]` / `gPlaySt.co[j]`.
Result: size-exact (176), no frame, push list identical, prologue and outer-loop shape now the ROM's, but the score
FALLS to 19.3% because the pool words differ (`&gPlaySt.co[i]` folds to a `gPlaySt+0x3d` literal, `ldr r0,=0x3d`, where
the ROM does `adds r6,r2,#0; adds r6,#0x3d; adds r7,r5,r6` from a bare word; and n lands in r7 not r8).
Negatives (measured with spellings.py): binding `ai` at the top of the outer body and using it in the inner loop
(-4, frame 8), binding `co = gPlaySt.co; ci = co + i` (size-exact, frame 8), both bound (-20), binding ai and co
inside the if AFTER the store (-8, frame 4), `ai`+`co`+`ci` all bound in the first spelling (-20).
Next: ROM's r6 = base+0x3d is a real pointer variable (`co`) alive across the inner loop and r7 = &co[i]; a
spelling that keeps `co` a pointer WITHOUT the frame is still needed (the frame comes from i+1 being spilled once
`co` and `ci` are both live).
Proposed summary tried: + "binding &co[i] before the retry loop removes the frame and the +8 but folds the address
into a gPlaySt+0x3d literal".
