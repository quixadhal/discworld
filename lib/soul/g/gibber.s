(gibber
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You gibber madly.")
      (rest "$mcname$ gibber$s$ madly.")
    )
    (arguments
       (arguments anxiously, madly, wildly, crazily, psychotically, unhappily,
                  dangerously, #)
      (self "You gibber $arg$.")
      (rest "$mcname$ gibber$s$ $arg$.")
    )
  )
  (targeted
     (no-arguments
       (self "You gibber madly at $hcname$.")
       (target "$mcname$ gibber$s$ maddly at you.")
       (rest "$mcname$ gibber$s$ madly at $hcname$.")
     )
     (arguments
       (arguments anxiously, madly, wildly, crazily, psychotically, unhappily,
                  dangerously, #)
       (self "You gibber $arg$ at $hcname$")
       (target "$mcname$ gibber$s$ $arg$ at you.")
       (rest "$mcname$ gibber$s$ $arg$ at $mcname$.")
     )
  )         
)
