(scratch
 *((prepositions [at])
   (adverbs      ,visciously,slightly,wombely,happily,joyfully,angrily)
   (undirected
    *((adverbs   secretly)
      (mymsg     [You scratch yourself $ADJ])
      (theirmsg  [$TP_NAME scratch $TP_FOO $ADJ])))
   (directed
    *((adverbs   penetratingly)
      (mymsg     [You scratch $OB_NAME $ADJ])
      (theirmsg  [$TP_NAME scratch $OB_NAME $ADJ])))))
$$
(scream
 *((prepositions [at])
   (adverbs      terror,fright,fear,pain,surprise,frustration,despair,anger)
   (noargs
    *((mymsg     [Arggghhhh!!!!])
      (theirmsg  [$TP_NAME screams])))
   (undirected
    *((mymsg     [You scream $ADJ])
      (theirmsg  [$TP_NAME scream $ADJ])))
   (directed
    *((mymsg     [You scream $ADJ at $OB_NAME])
      (theirmsg  [$TP_NAME scream $ADJ at $OB_NAME])))))
$$
(shake
 *((prepositions [with at])
   (undirected
    *((adverbs   ,sorrowfully,carefully,slowly,once,quickly,curtly,pinkly,
                 vigorously,completely,fishily,tentatively,in agreement,
                 in disbelief)
      (mymsg     [You shake your head $ADJ])
      (theirmsg  [$TP_NAME shakes $TP_POSS head $ADJ])))
   (directed
    *((adverbs   ,hands with,fins with,fingers with,legs with,
                 pinkys with,tentacles with,your fist at)
      (mymsg     [You shake $ADJ $OB_NAME])
      (theirmsg  [$TP_NAME shake $ADJ $OB_NAME])))))
$$
(shiver
 *((adverbs      in fear,with cold,slightly,#)
   (undirected
    *((mymsg     [You shiver $ADJ])
      (theirmsg  [$TP_NAME shiver $ADJ])))))
$$
(shrug
 *((adverbs      ,helplessly,slightly,evanly,rhythmicly,around in circles,
                 with your legs)
   (undirected
    *((theirmsg  [$TP_NAME shrugs $ADJ])
      (mymsg     [You shrug $ADJ])))))
$$
(shudder
 *((adverbs      ,happily,joyfully,angrily)
   (undirected
    *((adverbs   fear,revulsion,disapointment,ecstasy)
      (mymsg     [You shudder in $ADJ])
      (theirmsg  [$TP_NAME shudders in $ADJ])))))
$$
(sigh
 *((adverbs      deeply,silently,desperately,tiredly,sadly,slowly,deamily,
                 happily,heavily,suggestively,sarcasticly,in relief,with relief)
   (undirected
    *((mymsg     [You sigh $ADJ])
      (theirmsg  [$TP_NAME sighs $ADJ])))))
$$
(sing
 *((prepositions [to])
   (adverbs      #)
   (undirected
    *((mymsg     [You sing $ADJ])
      (theirmsg  [$TP_NAME sing $ADJ])))
   (directed
    *((mymsg     [You sing $ADJ to $OB_NAME])
      (theirmsg  [$TP_NAME sing $ADJ to $OB_NAME])))))
$$
(slap
 *((noargs
    *((mymsg     [You slap your forehead])
      (theirmsg  [$TP_NAME slaps $TP_POSS forehead])))
   (directed
    *((mymsg     [You slap $OB_NAME])
      (theirmsg  [$TP_NAME slaps $OB_NAME])))))
$$
(sleep
 *((noargs
    *((mymsg     [You go to sleep)
      (theirmsg  [$TP_NAME sing $ADJ])))
   (directed
    *((mymsg     [You go to sleep in $OB_NAME arms])
      (theirmsg  [$TP_NAME goes to sleep in $OB_NPOSS arms])))))
$$
(smirk
 *((noargs
    *((mymsg     [You smirk])
      (theirmsg  [$TP_NAME smirks])))))
$$
(smile
 *((prepositions [at])
   (adverbs      ,happily,sadly,nervously,wryly,broadly,crookedly,stupidly,
                 idiotically,condesendingly,ironically,patiently,brightly,
                 slightly,nastily,excitedly,strangely,with forced patience,
                 plasticly,carefully,sadisticly,lopsidedly,fishily,catishly,
                 faintly,like a train,angily,paranoicaly,innocently,frogily,
                 slyly,weakly,humbly,paradoxicaly,sarcasticly,vicously,
                 garishly,sweetly,innocently,lovingly,dreamily,radiantly,
                 enthusiasticly,insolently,quirkily,completely,unconsciously,
                 drunkenly,insanely,cutely,maliciously,absentmindedly,
                 curtly,disturbingly,unbeliveably,quietly,loudly,differently,
                 winningly,in triumph,seductively,tightly,
                 gosh there are lots of aruments,softly,lividly,demonicaly,
                 manically,warily,sardonically,lazily,serenely,disappointedly,
                 secretly,penetratingly)
   (undirected
    *((mymsg     [You smile $ADJ])
      (theirmsg  [$TP_NAME smiles $ADJ])))
   (directed
    *((mymsg     [You smile $ADJ at $OB_NAME])
      (theirmsg  [$TP_NAME smiles $ADJ at $OB_NAME])))))
$$
(snap
 *((prepositions [at])
   (adverbs      fingers,leg,arm in a quick salute)
   (undirected
    *((mymsg     [You snap your $ADJ])
      (theirmsg  [$TP_NAME snaps $TP_POSS $ADJ])))
   (directed
    *((mymsg     [You snap your $ADJ at $OB_NAME])
      (theirmsg  [$TP_NAME snaps $TP_POSS $ADJ at $OB_NAME])))))
$$
(snarl
 *((prepositions [at])
   (adverbs      ,visiously,nastily,demonicaly,happily,sadly,
                 unexpectedly,suddenly)
   (undirected
    *((mymsg     [You snarl $ADJ])
      (theirmsg  [$TP_NAME snarls $ADJ])))
   (directed
    *((mymsg     [You snarl $ADJ at $OB_NAME])
      (theirmsg  [$TP_NAME snarls $ADJ at $OB_NAME])))))
$$
(sneer
 *((prepositions [at])
   (undirected
    *((mymsg     [You sneer])
      (theirmsg  [$TP_NAME sneers])))
   (directed
    *((mymsg     [You sneer at $OB_NAME])
      (theirmsg  [$TP_NAME sneers at $OB_NAME])))))
$$
(sneeze
 *((prepositions [on])
   (adverbs      ,violently,loudly,silently,quietly,suddenly,unexpectedly)
   (undirected
    *((mymsg     [You sneeze $ADJ])
      (theirmsg  [$TP_NAME sneezes $ADJ])))
   (directed
    *((mymsg     [You sneeze $ADJ on $OB_NAME])
      (theirmsg  [$TP_NAME sneeze $ADJ on $OB_NAME])))))
$$
(snicker
 *((adverbs      ,mischeviously,nastily,a bit,like skippy)
   (undirected
    *((mymsg     [You snicker $ADJ])
      (theirmsg  [$TP_NAME snickers $ADJ])))))
$$
(sniff
 *((adverbs      ,dejectedly,noisily,silently,loudly,in disdain)
   (undirected
    *((mymsg     [You sniff $ADJ])
      (theirmsg  [$TP_NAME sniffs $ADJ])))))
$$
(snigger
 *((noargs
    *((mymsg     [You snigger])
      (theirmsg  [$TP_NAME sniggers])))))
$$
(snore
 *((adverbs      ,loudly,quitely,sypatheticly,strangely,carefully,backwards,
                 like bastian,like a train,over a keyboard with bastian)
   (undirected
    *((mymsg     [You snore $ADJ])
      (theirmsg  [$TP_NAME snores $ADJ])))))
$$
(snort
 *((adverbs      ,in ignitation,carefully,rudely)
   (undirected
    *((mymsg     [You snort $ADJ])
      (theirmsg  [$TP_NAME snort $ADJ])))))
$$
(snuggle
 *((directed
    *((theirmsg  [$TP_NAME snuggles up to $OB_NAME affectionately])
      (mymsg     [You snuggle up to $OB_NAME affectionately])))))
$$
(sob
 *((advebrs      ,uncontrollablely,happily,carefully,slightly,
                 quietly,loudly,helplessly,completely,unexpectedly,
                 silently,forlornly,cheekily)
   (undirected
    *((theirmsg  [$TP_NAME sobs $ADJ])
      (mymsg     [You sob $ADJ])))
   (directed
    *((theirmsg  [$TP_NAME sobs $ADJ on $OB_NPOSS shoulder])
      (mymsg     [You sob $ADJ on $OB_NPOSS shoulder])))))
$$
(sorry
 *((adverbs      ,profusely,reservedly,profunctily,carefully,insultingly,
                 insolently)
   (undirected
    *((mymsg     [You appologise $ADJ])
      (theirmsg  [$TP_NAME appologise $ADJ])))
   (undirected
    *((mymsg     [You appologise $ADJ to $OB_NAME])
      (theirmsg  [$TP_NAME appologise $ADJ to $OB_NAME])))))
$$
(spank
 *((adverbs      ,seductively,carefully,gently,uncontrollably,politely,
                 sensuously,selectively,unexpectedly,with an evil smirk,
                 painfully,sneakily,unprovoked,professionaly,vigorously,
                 reflexively,with a cat,like a train)
   (undirected
    *((mymsg     [You spank $OB_NAME $ADJ])
      (theirmsg  [$TP_NAME spanks $OB_NAME $ADJ])))))
$$
(spit
 *((prepositions [on])
   (noargs
    *((mymsg     [You spit])
      (theirmsg  [$TP_NAME spit])))
   (directed
    *((mymsg     [You spit in $OB_NPOSS face])
      (theirmsg  [$TP_NAME spit in $OB_NPOSS face])))))
$$
(spock
 *((noargs
    *((mymsg     [You raise one eyebrow])
      (theirmsg  [$TP_NAME raises one eyebrow])))))
$$
(squeeze
 *((adverbs      fondly,gently,tightly)
   (directed
    *((mymsg     [You squeeze $OB_NAME $ADJ])
      (theirmsg  [$TP_NAME squeeze $OB_NAME $ADJ])))))
$$
(stagger
 *((adverbs      #)
   (noargs
    *((mymsg     [You stagger around])
      (theirmsg  [$TP_NAME staggers around])))
   (undirected
    *((mymsg     [You stagger $ADJ])
      (theirmsg  [$TP_NAME staggers $ADJ])))))
$$
(stamp
 *((adverbs      ,angrily,impatiently,warningly,fighteningly)
   (undirected
    *((mymsg     [You stamp your foot $ADJ])
      (theirmsg  [$TP_NAME stamps $TP_POSS foot $ADJ])))))
$$
(stand
 *((adverbs      in uffish thought,about,patiently)
   (undirected
    *((mymsg     [You stand $ADJ])
      (theirmsg  [$TP_NAME stands $ADJ])))))
$$
(stare
 *((prepositions [at])
   (adverbs      ,happily,sadly,woriedly,lazily,rudely,dazedly,hungrily,
                 macabrely,absent-mindedly,fanaticaly,lovingly,respectfully,
                 womblely,frogily,desperately,sternly,tentatively,politely,
                 listlessly,dreamily,intently,mindlessly,expectantly,longingly)
   (undirected
    *((mymsg     [You stare $ADJ into space])
      (theirmsg  [$TP_NAME stares $ADJ into space])))
   (directed
    *((adverbs   romantically,continuously,annoyingly)
      (mymsg     [You stare $ADJ at $OB_NAME])
      (theirmsg  [$TP_NAME stare $ADJ at $OB_NAME])))))
$$
(stifle
 *((noargs
    *((mymsg     [You stifle a giggle])
      (theirmsg  [$TP_NAME stifle a giggle])))))
$$
(strangle
 *((noargs
    *((mymsg     [You strangle yourself])
      (theirmsg  [$TP_NAME strangles $TP_FOO])))
   (directed
    *((adverbs   #)
      (mymsg     [You strangle $OB_NAME $ADJ])
      (theirmsg  [$TP_NAME strangle $OB_NAME $ADJ])))))
$$
(stroke
 *((directed
    *((mymsg     [You stroke $OB_NAME])
      (theirmsg  [$TP_NAME stroke $OB_NAME])))))
$$
(strut
 *((noargs
    *((mymsg     [You strut you stuff])
      (theirmsg  [$TP_NAME struts proudly around the room])))))
$$
(stumble
 *((adverbs      #)
   (noargs
    *((mymsg     [You stumble around the room])
      (theirmsg  [$TP_NAME stumbles around the room])))
   (undirected
    *((mymsg     [You stumble $ADJ])
      (theirmsg  [$TP_NAME stumbles $ADJ])))
   (noargs
    *((mymsg     [You stumble over $OB_NAME $ADJ])
      (theirmsg  [$TP_NAME stumbles over $OB_NAME $ADJ])))))
$$
(sulk
 *((noargs
    *((mymsg     [You sulk in the corner])
      (theirmsg  [$TP_NAME sulks in the corner])))))
$$
(swim
 *((noargs
    *((mymsg     [You give a good imitation of swimming])
      (theirmsg  [$TP_NAME swims around])))))
$$
