(meep
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You meep.")
      (rest "$mcname$ meep$s$.")
    )
    (arguments 
      (arguments happily, sadly, brightly, curtly, patiently, slowly, eagerly,
                 expectantly, firmly, sagely, emphatically, knowingly, 
                 vigorously, in disagreement, tentatively, angrily, kindly, 
                 like an alarm clock, like a roadrunner, meekly, quietly, 
                 shyly, tiredly, in fear, in slight surprise, coweringly, 
                 weakly, mild-mannereredly, #)
      (self "You meep $arg$.")
      (rest "$mcname$ meep$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You meep at $hcname$.")
      (target "$mcname$ meep$s$ at you.")
      (rest   "$mcname$ meep$s$ at $hcname$.")
    )
    (arguments 
    (arguments happily, sadly, brightly, curtly, patiently, slowly, eagerly,
               expectantly, firmly, sagely, emphatically, knowingly, 
               vigorously, in disagreement, tentatively, angrily, kindly, 
               like an alarm clock, like a roadrunner, meekly, quietly, 
               shyly, tiredly, in fear, in slight surprise, coweringly, 
               weakly, mild-mannereredly, #)
    (self   "You meep $arg$ at $hcname$.")
    (target "$mcname$ meep$s$ $arg$ at you.")
    (rest   "$mcname$ meep$s$ $arg$ at $hcname$.")
    )
  )
)
/* Saffra 20 Apr 2001 */
