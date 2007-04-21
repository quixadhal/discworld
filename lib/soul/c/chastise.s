(chastise
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You chastise $hcname$.")
      (target "$mcname$ chastise$s$ you.")
      (rest  "$mcname$ chastise$s$ $hcname$.")
    )
    (arguments
      (arguments  loudly,insistently,argumentatively,unmercifully,unexpectedly,enthusiastically,calmly,rationally,insanely,madly,lovingly,politely,suggestively,eagerly,reluctantly,firmly,excitedly,understandably,irrationally,imaginatively,destructively,cunningly,thoughtlessly,carefully,offhandedly,absolutely)
      (self   "You chastise $hcname$ $arg$.")
      (target "$mcname$ chastise$s$ you $arg$.")
      (rest  "$mcname$ chastise$s$ $hcname$ $arg$.")
    )
  )
)

