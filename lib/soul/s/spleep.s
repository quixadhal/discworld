(spleep
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
      (arguments 
        (arguments sleepily,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,like an alarm clock,like a roadrunner,#)
      (self "You spleep $arg$.")
      (rest "$mcname$ spleep$s$ $arg$.")
    )
  )
  (targeted
      (arguments 
        (arguments sleepily,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,like an alarm clock,like a roadrunner,#)
      (self   "You spleep $arg$ at $hcname$.")
      (target "$mcname$ spleep$s$ $arg$ at you.")
      (rest   "$mcname$ spleep$s$ $arg$ at $hcname$.")
    )
  )
)
/*Hobbes 970317*/
