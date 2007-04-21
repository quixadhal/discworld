(lapdance
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You lapdance like a sultry princess from Klatch.")
      (rest "$mcname$ lapdance$s$ like a sultry princess from Klatch.")
    )
    (arguments
    (arguments across the room, loudly, in witches knickers, slowly, 
               all about the room, madly, for a Rhinu, # )
      (self "You lapdance for $arg$.")
      (rest "$mcname$ lapdance$s$ for $arg$.")
      )
    )
  (targeted
  (no-arguments
      (self   "You lapdance for $hcname$ like a sultry princess from Klatch.")
      (target "$mcname$ lapdance$s$ for you like a sultry princess from Klatch.")
      (rest  "$mcname$ lapdance$s$ for $hcname$ like a sultry princess from Klatch.")
  )
  (arguments
    (arguments across the room, loudly, in witches knickers, slowly, 
               all about the room, madly, for a Rhinu, # )
      (self   "You lapdance for $hcname$ $arg$.")
      (target "$mcname$ lapdance$s$ for you $arg$.")
      (rest  "$mcname$ lapdance$s$ for $hcname$ $arg$.")
    )
  )
)
