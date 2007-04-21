(nodnod
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
        (self "You nodnod.")
        (rest "$mcname$ nodnod$s$.")
        )
      (arguments 
        (arguments solemnly,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,annoyingly,like only Allana can,noddingly,with 
                 prosthetic legs, #)
      (self "You nodnod $arg$.")
      (rest "$mcname$ nodnod$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You nodnod at $hcname$.")
      (target "$mcname$ nodnod$s$ at you.")
      (rest  "$mcname$ nodnod$s$ at $hcname$.")
       )
      (arguments 
        (arguments solemnly,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,annoyingly,like only Allana can,noddingly,with 
                 prosthetic legs, #)
      (self   "You nodnod $arg$ at $hcname$.")
      (target "$mcname$ nodnod$s$ $arg$ at you.")
      (rest   "$mcname$ nodnod$s$ $arg$ at $hcname$.")
    )
  )
)

