(bombard
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> with <string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
     (arguments for that pathetic joke, for making that comment, for that lousy 
                pun, for the fun of it, just because)
      (self "You bombard $hcname$ with rotten eggs and tomatoes $arg$.")
      (target "$mcname$ bombard$s$ you with rotten eggs and tomatoes $arg$.")
      (rest "$mcname$ bombard$s$ $hcname$ with rotten eggs and tomatoes $arg$.")
    )
   (arguments
    (arguments rotten eggs, rotten tomatoes, noise, demands, honey, 
               punctuation, grammar, #)
    (self "You bombard $hcname$ with $arg$.")
    (target "$mcname$ bombard$s$ you with $arg$.")
    (rest "$mcname$ bombard$s$ $hcname$ with $arg$.")
    )
  )
)
