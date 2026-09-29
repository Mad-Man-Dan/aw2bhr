
## wave 95

Base: existing draft (684/680, +4, 25.3%, carries the `new_var` from an earlier permuter run).
- ROM slot [sp,#4] holds the spilled `q` (= gUnknown_08554A00[b*5+t], stored before q[200] and reloaded for q[201]); `b * 20` lives in r8 across both loops as a shared base (`lsls r0,i,#2; add r0,r8` for gUnknown_0855218C[b][i][0] and for the 08554A00 lookup).
- Hypothesis tried: hoist `b20 = b * 20` and spell both lookups as byte-offset reads `(u8 *)tbl + b20 + t * 4`: 692 (+12), 17.8%. Worse. The extra source local costs pressure but the cast spelling also loses the array's scaled-index form.
- Not permuter-run.
