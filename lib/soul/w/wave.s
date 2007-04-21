(wave
  (pattern "[at|to|at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [to|at] <indirect:any-living>")
  (pattern "[at|to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You wave.")
      (rest "$mcname$ waves.")
    )
    (arguments
      (arguments tiredly,sleepily,lazily,sadly,happily,redundantly,
                 forlornly,excitedly,longingly,pointlessly,madly,
                insanely,with complete devotion,cheerfully,merrily)
      (self "You wave $arg$.")
      (rest "$mcname$ wave$s$ $arg$.")
    )
    (arguments
      (arguments hand,tentacles,leg,frog,handkerchief)
      (self "You wave your $arg$.")
      (rest "$mcname$ wave$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wave to $hcname$.")
      (target "$mcname$ wave$s$ to you.")
      (rest  "$mcname$ wave$s$ to $hcname$.")
    )
    (arguments
      (arguments sleepily,tiredly,perfunctorily,lazily,sadly,happily,
                 forlornly,excitedly,longingly,redundantly,pointlessly,
                 passionately,greenly,your tentacles,madly,insanely,
                 with complete devotion,cheerfully,merrily,#)
      (self   "You wave $arg$ to $hcname$.")
      (target "$mcname$ wave$s$ $arg$ to you.")
      (rest   "$mcname$ wave$s$ $arg$ to $hcname$.")
    )
  )
)

