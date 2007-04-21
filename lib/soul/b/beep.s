(beep
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
      (arguments 
        (arguments happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,like an alarm clock,like a roadrunner,#)
      (self "You beep $arg$.")
      (rest "$mcname$ beep$s$ $arg$.")
    )
  )
  (targeted
      (arguments 
        (arguments happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,like an alarm clock,like a roadrunner,#)
      (self   "You beep $arg$ at $hcname$.")
      (target "$mcname$ beep$s$ $arg$ at you.")
      (rest   "$mcname$ beep$s$ $arg$ at $hcname$.")
    )
  )
)
/*Hobbes 970317*/
