(ogle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ogle at yourself.")
      (rest "$mcname$ ogle$s$ at $mobj$self.  How odd.")
    )
    (arguments
      (arguments  openly, surreptitiously, all over, lasciviously, lewdly, 
                  wickedly, smarmily, seductively, like a dirty old man, 
                  like a dirty old woman, like a one-eyed man, 
                  with bugged-out eyes, like a cross-eyed Elvis impersonator, 
                  fondly, sideways, with sheep's eyes, oogily, googily-oogly, #)
      (self "You ogle $arg$ around the room.")
      (rest "$mcname$ ogle$s$ $arg$ around the room.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ogle at $hcname$.")
      (target "$mcname$ ogle$s$ at you.")
      (rest   "$mcname$ ogle$s$ at $hcname$.")
    )
    (arguments
      (arguments  openly, surreptitiously, all over, lasciviously, lewdly, 
                  wickedly, smarmily, seductively, like a dirty old man, 
                  like a dirty old woman, like a one-eyed man, 
                  with bugged-out eyes, like a cross-eyed Elvis impersonator, 
                  fondly, sideways, with sheep's eyes, oogily, googily-oogly, #)
      (self   "You ogle $arg$ at $hcname$.")
      (target "$mcname$ ogle$s$ $arg$ at you.")
      (rest   "$mcname$ ogle$s$ $arg$ at $hcname$.")
    )
  )
)
/* Saffra 2-01-02, idea rep */

