// class world
    // must have "bioms" which are size of nosie algorthems, these are squares
    // bioms will have an origin that will demonstrate the 0,0 of the biom in the world
    // this origin will be kept track of in the world space, the origin of a biom, will
    // default to being 0,0
        // bioms will be made up of planes
            // planes are an object which contain vertices that will draw the world
            // planes have a set width and there detail can also be set
            // this is done through using a triangle length and plane length
            // planes height will have to be effected by the "bioms"
        // noise this will be how the heights are effected
            // something like value noise is rendered this array will be the size
            // of the biom, it will be a square
        // biom generating logic
            // according to the origin, and where the player is, we will calculate
            // what planes need to be draw and with what detail
            // there will be some other values here like    
                // render distance
                // plane size
            // this rendering logic should be able to abjust to the position of the player
            // so if the player gets into a new plane new planes must render
    // world logic, the player must have some sort of global scope 
    // data structures, the world class is one object, which will contain lots, of information
    // on the world the random should come from outside this class, so it can be saved


// object class
    // this class has the obective of initilizing objects
    // it gives a layout of what objects will defaultly have

    // it needs to create a buffer for that object

    // even the plane, or world can use this as to be drawn they need buffers

    // this should be where the textures are alligned with a specific object

// render class
    // I think this is important should be able to take objects to render
    // rendering does need information on the world

    // handles render logic, should establish a render pipeline,
    // what gets rendered first to last
        // this render pipeline should probably be set in stages, so values
        // for what comes first to last, so a user can put that in

    // textures are a valueble asset and should be linked to the renderer

    // camera transformations will be linked obviously to the rendered, you 
    // need things like view and projection


// gamelogic class/function
    // there should be a class/function to handle the beinging of game logic
    // this can be things like loading enemies, and stuff like that
    // keeps the main function more clean
    // keep main as calling the window

    // this will hold when to call all other logic

    // will handle input callbacks for now

    // camera will get information here to be effected 

    // this is where early collision logic will be held