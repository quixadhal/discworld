(downgrape
  (pattern " <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string>  <indirect:any-living>")
  (pattern " <indirect:any-living> <string>")
  (single
    (no-arguments
      (self  "You downgrape yourself.")
      (rest  "$mcname$ downgrape$s$ $mobj$self.")
    )
    (arguments
      (arguments  everything, skills, guilds, commands, your guild, #)
      (self  "You downgrape $arg$.")
      (rest  "$mcname$ downgrape$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You downgrape $hcname$.")
      (target "$mcname$ downgrape$s$ you.")
      (rest   "$mcname$ downgrape$s$ $hcname$.")
    )
    (arguments
      (arguments  ability to use souls, commands, guild, weapons, armour, 
                  mail service, internet connection, womble, wossname, #)
      (self   "You downgrape $mhcname$ $arg$.")
      (target "$mcname$ downgrape$s$ your $arg$.")
      (rest   "$mcname$ downgrape$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  wardrobe)
      (self   "You downgrape $mhcname$ $arg$.  They feel naked.")
      (target "$mcname$ downgrape$s$ your $arg$.  You feel naked.")
      (rest   "$mcname$ downgrape$s$ $mhcname$ $arg$.  They feel naked.")
      
    )
    (arguments
      (arguments  code)
      (self   "You downgrape $mhcname$ ability to $arg$.  They crash the mud.")
      (target "$mcname$ downgrape$s$ your ability to $arg$.  You crash the mud.")
      (rest   "$mcname$ downgrape$s$ $mhcname$ ability to $arg$.  They crash the mud.")
    )
    (arguments
      (arguments  with excessive force, with subtlety and finesse, 
                  with finesse only, with loving kindness, 
                  with the best intentions)
      (self   "You downgrape $hcname$ $arg$.")
      (target "$mcname$ downgrape$s$ you $arg$.")
      (rest   "$mcname$ downgrape$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  for good reasons)
      (self   "You downgrape $hcname$ $arg$ (if only $hpronoun$ knew).")
      (target "$mcname$ downgrape$s$ you $arg$ (if only you knew).")
      (rest   "$mcname$ downgrape$s$ $hcname$ $arg$ (if only $hpronoun$ knew).")
    )
    (arguments
      (arguments  frogs, voices, creators, wombles, lords)
      (self   "You downgrape $hcname$ because the $arg$ told you to.")
      (target "$mcname$ downgrape$s$ you because the $arg$ told $mobj$ to.")
      (rest   "$mcname$ downgrape$s$ $hcname$ because the $arg$ told $mobj$ to.")
    )
    (arguments
      (arguments  Ray, Uncle Sam)
      (self   "You downgrape $hcname$ because $arg$ told you to.")
      (target "$mcname$ downgrape$s$ you because $arg$ told $mobj$ to.")
      (rest   "$mcname$ downgrape$s$ $hcname$ because $arg$ told $mobj$ to.")
    )
  )
)

