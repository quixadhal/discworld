(tempt
  (pattern "<indirect:any-living>")
  (pattern "[to] <string> <indirect:any-living>")
  (pattern "<indirect:any-living> [to] <string>")
  (single
    (no-arguments
      (self "You tempt fate.")
      (rest "$mcname$ tempts fate.")
    )
  )
  (targeted
    (no-arguments
      (self   "You tempt $hcname$.")
      (target "$mcname$ tempts you.")
      (rest   "$mcname$ tempts $hcname$.")
    )
    (arguments
      (arguments  love, promises, timtams, dreams, loot, booty, money, treasure, 
                  rabbit ears, lies, stories of fantastic adventures, 
                  the past, a present, the future, empty promises, 
                  promises which are either half empty or half full)
      (self   "You tempt $hcname$ with $arg$.")
      (target "$mcname$ tempts you with $arg$.")
      (rest   "$mcname$ tempts $hcname$ with $arg$.")
    )
    (arguments
      (arguments  hurriedly, happily, deviously, sinisterly, dextrously, slyly, 
                  lovingly, cutely, cunningly, #)
      (self   "You tempt $hcname$ $arg$.")
      (target "$mcname$ tempts you $arg$.")
      (rest   "$mcname$ tempts $hcname$ $arg$.")
    )
    (arguments
      (arguments  the Dark Side, mow the lawn, do the dishes, quack like a duck, 
                  share the last timtam)
      (self   "You tempt $hcname$ to $arg$.")
      (target "$mcname$ tempts you to $arg$.")
      (rest   "$mcname$ tempts $hcname$ to $arg$.")
    )
    (arguments
      (arguments  death, undoing, marriage)
      (self   "You tempt $hcname$ to $hposs$ $arg$.")
      (target "$mcname$ tempts you to your $arg$.")
      (rest   "$mcname$ tempts $hcname$ to $hposs$ $arg$.")
    )
    (arguments
      (arguments  heart, spleen, marbles, cutlery, cabbages)
      (self   "You tempt $hcname$ with all your $arg$.")
      (target "$mcname$ tempts you with all $mposs$ $arg$.")
      (rest   "$mcname$ tempts $hcname$ with all $mposs$ $arg$.")
    )
    (arguments
      (arguments  fate)
      (self   "You tempt $arg$ with $hcname$.")
      (target "$mcname$ tempts fate with you.")
      (rest   "$mcname$ tempts fate with $hcname$.")
    )
  )
)

