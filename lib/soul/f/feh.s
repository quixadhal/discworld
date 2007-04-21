(feh
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You go feh!")
      (rest "$mcname$ go$s$ feh!")
    )
    (arguments
      (arguments like Dogbolter, like Taffyd on a bad code day, 
                 like Laurana on a bad hair day, anxiously, angrily, #)
      (self "You go feh! $arg$.")
      (rest "$mcname$ go$s$ feh! $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You go feh! at $hcname$.")
      (target "$mcname$ go$s$ feh! at you.")
      (rest  "$mcname$ go$s$ feh! at $hcname$.")
    )
    (arguments
      (arguments like Dogbolter, like Taffyd on a bad code day, 
                 like Laurana on a bad hair day, anxiously, angrily, #)
      (self   "You go feh! $arg$ at $hcname$.")
      (target "$mcname$ go$s$ feh! $arg$ at you.")
      (rest  "$mcname$ go$s$ feh! $arg$ at $hcname$.")
    )
  )
)

