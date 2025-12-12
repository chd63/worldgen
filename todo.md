
// seperate a lot of the classes into headers and implementation
// will save a shit ton of time compiling because we wont have to recompile things

// im pretty sure there is a mega memory leak when using Biome's biomeData
// would probably be good to prevent that

// figure out solution for heightmap'

// the solution is to use the biomes out of the biomes map
// we are going to have to use the method of taking the world positions
// then get the map based off of that and push it into the movment and 
// camera code

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