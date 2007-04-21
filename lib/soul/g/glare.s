(glare
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments stonily, sternly, hotly, intently, solemnly, idiotically,
                 evenly, evilly, furiously, angrily, #)
      (self "You glare $arg$ at the world in general.")
      (rest "$mcname$ glare$s$ $arg$ around $mobj$self.")
    )
  )
  (targeted
    (arguments
      (arguments stonily, sternly, hotly, intently, solemnly, idiotically,
                 evenly, evilly, furiously, angrily, #)
      (self   "You glare $arg$ at $hcname$.")
      (target "$mcname$ glare$s$ $arg$ at you.")
      (rest  "$mcname$ glare$s$ $arg$ at $hcname$.")
    )
  )
)

