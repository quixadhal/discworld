(caper
  (pattern "[around] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [around] <indirect:any-living>")
  (pattern "[around] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You caper around.")
      (rest "$mcname$ caper$s$ around.")
    )
    (arguments
      (arguments  madly, merrily, happily, sadly, #)
      (self "You caper around $arg$.")
      (rest "$mcname$ caper$s$ around $arg$.")
    )
    (arguments
      (arguments  womble with its pants on fire, chicken, jester, bimbo, 
                  blonde, sulam, seductress, eager person, vivacious person, 
                  madman, madwoman, madperson, frog on the run, happy chap, 
                  polite gentle person, woman, no we aren't sexist, fool)
      (self "You caper around like $article$ $arg$.")
      (rest "$mcname$ caper$s$ around like $article$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You caper around $hcname$.")
      (target "$mcname$ caper$s$ around you.")
      (rest   "$mcname$ caper$s$ around $hcname$.")
    )
    (arguments
      (arguments  madly, merrily, happily, sadly, #)
      (self   "You caper around $hcname$ $arg$.")
      (target "$mcname$ caper$s$ around you $arg$.")
      (rest   "$mcname$ caper$s$ around $hcname$ $arg$.")
    )
    (arguments
      (arguments  womble with its pants on fire, chicken, jester, bimbo, 
                  blonde, sulam, seductress, eager person, vivacious person, 
                  madman, madwoman, madperson, frog on the run, happy chap, 
                  polite gentle person, woman, no we aren't sexist, fool)
      (self   "You caper around $hcname$ like $article$ $arg$.")
      (target "$mcname$ caper$s$ around you like $article$ $arg$.")
      (rest   "$mcname$ caper$s$ around $hcname$ like $article$ $arg$.")
    )
  )
)
/* Recoded by Hobbes, 970931 */
/* Put $article$ in, Saffra, 10-15-00 */
