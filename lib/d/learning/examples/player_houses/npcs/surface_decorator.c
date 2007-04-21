/**
 *  This is an example file for the Learning domain.
 *
 *  In this file you should learn everything you need to know to put together 
 *  an NPC that will decorate the surfaces of player housing. 
 *
 *  There are a lot of comments in this particular file, but the basic things 
 *  you will need to add don't amount to much.
 *
 *  @author Una
 *  @September 20, 2002
 *
 **/

// This is the inherit you will need to use
inherit "/std/npc/surface_decorator";

void setup() {

    set_name( "worker" );
    // The big burly sort that does cat calls
    set_short( "construction worker" );
    add_property( "determinate", "a " );
    add_adjective( "construction" );
    add_property( "unique", 1 );
    setup_nationality( "/std/nationality/agatean", "Bes Pelargic" );    
    basic_setup( "human", "warrior", 30 );
    set_gender( 1 );
    
    set_long("This is a construction worker.  You can use him to "
        "decorate surfaces in player housing to the exact specifications "
        "that you will set later in this file.  He is slightly grubby, "
        "is covered in dust and paint, and likes to do wolf whistles.\n");
    
    // Adding the place property ensures that the decorator will
    // charge the correct currency when the player needs to pay.
    // If you don't add a place property, the default currency 
    // will be provincial.
    
    add_property( "place", "Counterweight Continent" );
       
   
   
    // Add_allowed_surface() adds surfaces to the NPC that they are allowed to
    // decorate.  You can decorate floors, ceilings and walls.
    
    // Note that just adding "wall" will include all of the walls in the 
    // room (north, south, east and west).  
    
    add_allowed_surface( "floor" ); 
    add_allowed_surface( "wall" );  
        
    
    
    // The following bit is what adds items to the NPC's stock list.  
    
    // The first argument in add_fitted_surface() is the short description 
    // of the decoration that you want to add.  This could be 
    // "wood panelling", "rough wooden beams", "neon yellow carpet",
    // or any other type of decoration that would make sense with the 
    // surfaces you've described above in add_allowed_surface().
    
    // The second argument is the long description that will be installed in
    // the room itself.  For example, the room would look like: The floor is         
    // covered with plush yellow carpet and the other surfaces are blah blah 
    // blah.
    
    // The third argument is the cost of the decoration and its installation.  
    // The type of currency the player is charged in is determined by the 
    // "add_property( "place", <location> );" that you defined above.  In 
    // this example, the player will be charged in Rhinu since the 
    // place is the Counterweight Continent.
        
    add_fitted_surface( "plush green carpet", "covered with plush "
        "green carpet", 1000 );
    add_fitted_surface( "plush red carpet", "covered with plush "
        "red carpet", 1000 );
    add_fitted_surface( "plush yellow carpet", "covered with plush "
        "yellow carpet", 1000 );
    add_fitted_surface( "plush blue carpet", "covered with plush "
        "blue carpet", 1000 );
    
    
        
    // The following function defines what the surface will look like while 
    // it is being decorated.  Note that if the worker is killed in the middle
    // of his job, the surface will retain this message until the player hires
    // him again and he completes the work.  The default message is:
    // "covered in dust and dirt".
    
    set_under_construction_mess( "covered in dust, plaster and carpet bits" );


    
    // This adds a domain which the NPC will freely wander around.  
    
    // In this example, if the file name of the room is anywhere 
    // in /w/una or anywhere in /d/cwc/Bes_Pelargic, the NPC will happily 
    // follow a player around.  
    
    // However, if they follow a player out of this file structure into 
    // another area,  (say they are in Bes Pelargic and walk onto 
    // the T-shop, which is in /d/underworld), the NPC will be moved
    // to the rubbish room and will then attempt to move themselves 
    // to the "good room" they were in.  If they can't move themselves 
    // into the last "good room" within a few seconds, they will be 
    // dested in the rubbish room.
    
    add_allowed_domain( "/w/una" );
    add_allowed_domain( "/d/cwc/Bes_Pelargic" );
    
    
    
    // You need to set some customised responses for the NPC so they don't 
    // sound like they have a stick up their arse, too. :)  In this example,
    // we will continue with the carpet theme used above and add a spicy 
    // Morporkian spicy pirate flavour to his new custom responses!
    
    set_responses( ({
        
        // This is what he will say if the player requests a type of 
        // surface you haven't added in add_allowed_surface().
        "'I don't fit those kinds of surfaces matey!",
        
        // This is what he will say if the player requests a type of 
        // carpet short that you have not listed in add_fitted_surface().
        "'I've never heard of those sort of carpets, arrrrr.",
        
        // This is what he will say if the player is not the owner of 
        // (or allowed by the owner) the house they are asking the worker
        // to decorate.
        "'Arr, you don't own this place, mister!",
        
        // This is what he will say if the room he is in when the player
        // asks him to decorate is not a player-housing room with decoratable
        // surfaces.
        "'I can't be decoratin' these here surfaces, shogun!  You should "
        "take me to a house!",
        
        // This is what he will say if something goes bizarrely wrong and 
        // the code needs to bail before he decorates the room.
        "'Arr matey, something went horribly wrong!",
        
        // This is what he will say when he is done the job.
        "'Arrrr matey, all done!",
        
        // This is what he will say if the player doesn't have enough money 
        // to pay for the decoration they have requested.  Leave in $decor$
        // and $amount$ - they are already replaced in the inherit by 
        // what the player has chosen and its price.
        "'You don't have enough money to pay for the $decor$!  It "
        "will cost you $amount$, guv!",
        
        // This is what he will say when you ask him to list his goods.
        "'I can fit $allowed$ with any of the following:",
        
        // This is what he will say if he hasn't got any stock.
        "'Cor blimey mister, I haven't got any stock.",
        
        // This is what he will say when he starts the job.
        "'Okey doke, I'll have it done in a jiffy guv'nor!",
        
        // This is what he will say if he can't see the person that's speaking
        // to him.
        "'What?  Who sed dat?",
        
        // This is what he will say if he's already following someone and 
        // another person tries to hire him.  $employer$ is replaced by the 
        //person who has hired him.
        "'I've already got this great job with $employer$, mate!",
        
        // This is what he will say when you hire his services.
        "'Righty-o, you just take me to the room you want spiffied "
        "up and ask me to fit something!",
        
        // This is what he will say if you try to take him out of an area he 
        // is not allowed to go out of.  You set this area with         
        // add_allowed_domain(), as shown above.
        "'Sorry matey, bad karma over that way."
        
    }) );  

        // That's all.  Easy peasy.  Enjoy and have fun. :)

} /* setup() */
