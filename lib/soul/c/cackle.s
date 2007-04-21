(cackle
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments     
      (self "You cackle with glee.")
      (rest "$mcname$ throw$s$ back $mposs$ head and cackle$s$ with glee.")
    )
    (arguments
      (arguments  with glee, happily, sadly, brightly, curtly, patiently, 
                  slowly, eagerly, expectantly, firmly, sagely, emphatically,
                  knowingly, vigorously, in disagreement, tentatively, 
                  angrily, manically, mischievously, nastily, despairingly, 
                  insanely, inanely, womblely, backwards, #)
        (self "You cackle $arg$.")
        (rest "$mcname$ cackle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You cackle at $hcname$.")
      (target "$mcname$ cackle$s$ at $hcname$.")
      (rest   "$mcname$ cackle$s$ at $hcname$.")
	   )
    (arguments
      (arguments  with glee, happily, sadly, brightly, curtly, patiently, 
                  slowly, eagerly, expectantly, firmly, sagely, emphatically,
                  knowingly, vigorously, in disagreement, tentatively, 
                  angrily, manically, mischievously, nastily, despairingly, 
                  insanely, inanely, womblely, backwards, #)
        (self   "You cackle $arg$ at $hcname$.")
        (target "$mcname$ cackle$s$ $arg$ at you.")
        (rest   "$mcname$ cackle$s$ $arg$ at $hcname$.")
    )
  )
)

