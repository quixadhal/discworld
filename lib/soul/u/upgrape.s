(upgrape
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string>  <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You upgrape yourself.")
      (rest  "$mcname$ upgrape$s$ $mobj$self.")
    )
    (arguments
      (arguments  everything, skills, guilds, commands, your guild, #)
      (self  "You upgrape $arg$.")
      (rest  "$mcname$ upgrape$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You upgrape $hcname$.")
      (target "$mcname$ upgrape$s$ you.")
      (rest   "$mcname$ upgrape$s$ $hcname$.")
    )
    (arguments
      (arguments  ability to use souls, commands, guild, weapons, armour, 
                  mail service, internet connection, womble, wossname, #)
      (self   "You upgrape $mhcname$ $arg$.")
      (target "$mcname$ upgrape$s$ your $arg$.")
      (rest   "$mcname$ upgrape$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  wardrobe)
      (self   "You upgrape $mhcname$ $arg$.")
      (target "$mcname$ upgrape$s$ your $arg$.")
      (rest   "$mcname$ upgrape$s$ $mhcname$ $arg$.")
     
    )
    (arguments
      (arguments  with excessive force, with sublety and finesse, 
                  with finesse only, with loving kindness, 
                  with the best intentions)
      (self   "You upgrape $hcname$ $arg$.")
      (target "$mcname$ upgrape$s$ you $arg$.")
      (rest   "$mcname$ upgrape$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  for poor reasons)
      (self   "You upgrape $hcname$ $arg$ (if only $hpronoun$ knew).")
      (target "$mcname$ upgrape$s$ you $arg$ (if only you knew).")
      (rest   "$mcname$ upgrape$s$ $hcname$ $arg$ (if only $hpronoun$ knew).")
    )
    (arguments
      (arguments  players, voices)
      (self   "You upgrape $hcname$ because the $arg$ told you to.")
      (target "$mcname$ upgrape$s$ you because the $arg$ told $mobj$ to.")
      (rest   "$mcname$ upgrape$s$ $hcname$ because the $arg$ told $mobj$ to.")
    )
  )
)

