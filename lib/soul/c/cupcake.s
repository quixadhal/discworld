(cupcake
  (pattern "[with] <string>")
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> [with] <string>")
  (single
    (no-arguments
      (self "You cupcake.")
      (rest "$mcname$ cupcakes.")
    )
    (arguments
      (arguments finesse,desire,lust,karma,intelligence,icing,flour,
                 water,eggs,milk,sugar,baking soda,an electric beater,
                 skill,dexterity,hands,fingers,legs,fins,chocolate,typos,
                 explosions of light,pools of limpets,cat hair,love,
                 shells of stuff,a mixing bowl and wooden spoons)
      (self "You cupcake with $arg$.")
      (rest "$mcname$ cupcakes with $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You cupcake $hcname$.")
      (target "$mcname$ cupcakes you.")
      (rest "$mcname$ cupcakes $hcname$.")
    )
    (arguments
      (arguments finesse,desire,lust,karma,intelligence,icing,flour,
                 water,eggs,milk,sugar,baking soda,an electric beater,
                 skill,dexterity,hands,fingers,legs,fins,chocolate,typos,
                 explosions of light,pools of limpets,cat hair,love,
                 shells of stuff,a mixing bowl and wooden spoons)
      (self "You cupcake $hcname$ with $arg$.")
      (target "$mcname$ cupcakes you with $arg$.")
      (rest "$mcname$ cupcakes $hcname$ with $arg$.")
    )
  )
)

