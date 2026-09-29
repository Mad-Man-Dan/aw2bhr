
## wave 95

No probe run. Reading the pre-registered chapters (two-statement split, member-array giv order) against the record: the wave-70 park already ran the two-statement `p = base; p += K;` split, before the loop (spills, kills the inner CSE) and in the body (const-propagates back). The chapters' successful splits use a base that is a LOAD (`e4 = *pE4`) or a two-element row read; here the base is a bare SYMBOL_REF, so cse folds base+K back. Nothing in the chapters gives a non-constant base for gUnknown_020298E0 / gUnknown_08551D22. Left at the wave-70 draft (360/364).
(draft left at the wave-70 draft.)
