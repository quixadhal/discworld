(beg
  (pattern "at <indirect:any-living> like a <string>")
  (pattern "like a <string> at <indirect:any-living>")
  (pattern "like a <string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments dog,cat,frog,chicken,frisbee,cow,large bear,poodle,
                 chihuahua,train,beggar,noble man,Labrador,
                 German shepherd,cannibal,chimpanzee,orang-utang,librarian,
                 fish,dolphin,tokzic,banshee,bloodhound,St. Bernard,
                 Virgin Mary,husky,wolf,dingo,Alsatian,newbie,#)
      (self "You beg like $article$ $arg$.")
      (rest "$mcname$ beg$s$ like $article$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments dog,cat,frog,chicken,frisbee,cow,large bear,poodle,
                 chihuahua,train,beggar,noble man,Labrador,
                 German shepherd,cannibal,chimpanzee,orang-utang,librarian,
                 fish,dolphin,tokzic,banshee,bloodhound,St. Bernard,
                 Virgin Mary,husky,wolf,dingo,Alsatian,newbie,#)
      (self   "You beg like $article$ $arg$ at $hcname$.")
      (target "$mcname$ beg$s$ like $article$ $arg$ at you.")
      (rest  "$mcname$ beg$s$ like $article$ $arg$ at $hcname$.")
    )
  )
)

