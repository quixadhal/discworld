(scare
   (pattern "<indirect:any-living>")
   (pattern "<string>")
   (pattern "<string> <indirect:any-living>")
   (pattern "<indirect:any-living> <string>")
   (single
     (no-arguments
       (self "You scare easily.")
       (rest "$mcname$ scare$s$ easily.")
     )
     (arguments
     (arguments wombles, bunnies, newbies, liaisons, #)
       (self "You scare $arg$.")
       (rest "$mcname$ scare$s$ $arg$.")
     )
   )
   (targeted
     (no-arguments
       (self "You scare $hcname$.")
       (target "$mcname$ scare$s$ you.")
       (rest "$mcname$ scare$s$ $hcname$.")
       
     )
     (arguments
     (arguments suddenly, frighteningly, fatefully, stupidly, slightly,
                unexpectedly, like a scary Sasquatch, like a horror click,
                in the dark, like a ghostie, like a ghoulie, like a long-
                legged beastie, like an annoyed librarian, like a creature
                from the Dungeon Dimensions, like an upset High Lord, with     
                snakes, with spiders, with a coffee table, with MacChirton 
                code, like a basilisk in the face, with bad spelling, #)
       (self "You scare $hcname$ $arg$.")
       (target "$mcname$ scare$s$ you $arg$.")
       (rest "$mcname$ scare$s$ $hcname$ $arg$.")
       
     )
   ) 
 )
/* Saffra 02-07-01 on bequest of Sasquatch */
