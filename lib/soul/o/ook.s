(ook
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ook.")
      (rest "$mcname$ ook$s$.")
    )
    (arguments
      (arguments happily, like the librarian, manically, evilly, wickedly,
                 insanely, mischievously, devilishly, impishly, cheekily, 
                 idiotically, tentatively, wryly, skeptically, #)
      (self "You ook $arg$.")
      (rest "$mcname$ ook$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ook at $hcname$.")
      (target "$mcname$ ook$s$ at you.")
      (rest  "$mcname$ ook$s$ at $hcname$.")
    )
    (arguments
      (arguments happily, like the librarian, manically, evilly, wickedly,
                 insanely, mischievously, devilishly, impishly, cheekily, 
                 idiotically, tentatively, wryly, skeptically, #)
      (self   "You ook $arg$ at $hcname$.")
      (target "$mcname$ ook$s$ $arg$ at you.")
      (rest  "$mcname$ ook$s$ $arg$ at $hcname$.")
    )
  )
)
/* Hobbes, 961216 */
