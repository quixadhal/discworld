(nod
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
        (self "You nod.")
        (rest "$mcname$ nod$s$.")
        )
      (arguments 
        (arguments solemnly,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,annoyingly,approvingly, #)
      (self "You nod $arg$.")
      (rest "$mcname$ nod$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You nod at $hcname$.")
      (target "$mcname$ nod$s$ at you.")
      (rest  "$mcname$ nod$s$ at $hcname$.")
       )
      (arguments 
        (arguments solemnly,happily,sadly,brightly,curtly,patiently,
                 slowly,eagerly,expectantly,firmly,sagely,emphatically,
                 knowingly,vigorously,in disagreement,tentatively,angrily,
                 kindly,annoyingly,approvingly, #)
      (self   "You nod $arg$ at $hcname$.")
      (target "$mcname$ nod$s$ $arg$ at you.")
      (rest   "$mcname$ nod$s$ $arg$ at $hcname$.")
    )
  )
)

