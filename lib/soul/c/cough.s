(cough
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You cough.")
      (rest "$mcname$ coughs.")
    )
    (arguments
      (arguments up someting nasty,up a frog,politely,sarcastically,
       manically,evilly,wickedly,insanely,mischievously,devilishly,
       impishly,cheekily,idiotically,tentatively,wryly,sceptically,#)
      (self "You cough $arg$.")
      (rest "$mcname$ coughs $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You cough at $hcname$.")
      (target "$mcname$ coughs at you.")
      (rest  "$mcname$ coughs at $hcname$.")
    )
    (arguments
      (arguments up someting nasty,up a frog,politely,sarcastically,
       manically,evilly,wickedly,insanely,mischievously,devilishly,
       impishly,cheekily,idiotically,tentatively,wryly,sceptically,
       some germs,#)
      (self   "You cough $arg$ at $hcname$.")
      (target "$mcname$ coughs $arg$ at you.")
      (rest  "$mcname$ coughs $arg$ at $hcname$.")
    )
  )
)

