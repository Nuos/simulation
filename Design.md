Questions:
----------
* How to properly handle different scales?
  * framebuffer (distance buffer) issues w/ FP precision
* How to do collision detection with dynamically generated meshes (i.e. terrain)
* How to stream high-res texture/data-maps for *parts* of objects
  * like how outerra only streams the data for the part you are at in earth
* Handle frustum culling *AND* distance scales:
  * Binary space partitioning:
    * each objects world position : [0,1] --> relative to space partition position
      * useful for fixing depth buffer issue?
    * each space partition has an integer position pointing to one of its corners
    * Each partition has list of objects inside
      * may have subpartitions?
    * Use the partition cubes for frustum culling 

* For Global Rendering:
  * Books:
    * GPU PRO 3 CH 1:2
    * GPU PRO 4 CH 1:1
    * GPU PRO 4 CH 1:3
    * GPU PRO 5 CH 6:3 (quadtrees on GPU)
  * Docs: 
    * ROAM
    * Under the hood of virtual globes
    * Rendering massive terrains
    * High performance terrain rendering
    * globe
    * cdlod
    * Adaptive terrain tessellation
    * spherical worlds
    * adaptive terrain rendering 
  * Binary Space Partitioning
    * Octree/Quadtree
  * Subdivided Meshes
    * Bounding Volume Heirarchy
    * Ellipsoid class
      * mesh generated for each cell in the root BVH
      * Subdivided meshes for each of those cells' children and so on.
      * Choose which mesh to show based on Distance to viewer from cell
      * Subdivide at initialization and just stream? or only subdivide when needed?
    * Coordinate transforms to get
      * world positions (x,y,z)
      * local positions ( latitude, longitude, altitude )

* Frustum culling:
  * test mesh points for inclusion into the rendering
  * XNA math functions all throughout
* Cube subdivision into ellipsoid
  * when frustum tests each vertex against the frustum, the included meshes are subdivided based on distance
  * Need to ensure backfaces are removed prior to subdivision ( early point*normal rejection )

Goals:
------
* Rendering Engine:
  * multiple scales: close to objects<->astronomical scales
    * Solar Systems
    * Planets
    * Landscapes
    * Rocks/Trees/Caves/lakes/rivers
    * Particles
  * tunable rendering options:
    * different shader types
    * different effects (particles, atmo, etc.)
    * tessellation
* Terrain Generation:
  * possible evolution/deformation?
    * mountains -> climate -> lakes/rivers -> erosion?
  * Handles many parameters:
    * biome
    * planetary climate
    * tessellation level
  * ability to save output of generation
* Physics Engine:
  * Modular for each area of physics:
    * fluids
    * gravity
    * collisions
    * aero
  * each module can be swapped with a different module?
  * Integration with standards for:
    * physx
    * jsbsim
    * beamNG

Next Steps:
-----------
* Improvements based on Sample application (VS2013 sample dx Universal app)
  * Asynchronous (multithreaded) appraoch to loading and generating resources
  * Better adaptation to screen size (window size dependent resources)
  * Better adaptation to screen orientation
  * 2d/text rendering 
  * Better handling of callbacks (device lost, etc.)
* Finish Transition to DX11.2
  * Uses only MSVS 2013, Windows 8.1 SDK (no need for 2010 & june 2010 sdk)
* Modularize engine:
  * Base
    * Math
    * Data Structures
    * Objects
    * Memory
    * Time
  * Input
  * Physics
  * Renderer
    * Camera
    * Shaders
  * Engine
  * App
* Change earth representation to actual mesh
  * each set of triangles on the mesh should correspsond to sub textures
    * this allows us to only load textures into memory which may actually be displayed
    * solves:
      * large texture size issue
      * texture streaming issue
      * wasted resource issue (1/2 of texture is not visible currently) 
* Multiple objects:
  * Earth
  * Moon
  * Sun
  * Create object management system which contains info for each object:
    * space partition it's in?
    * local partition position
    * bounding object (sphere,box,etc.)
    * Sub objects/meshes:
      * mesh data
      * texture file
        * each sub mesh can then have more accurate texture files
        * distance can determine mesh to use?
        * this is how we save the dynamically generated meshes?
      * relevant effects
      * configuration functions? (i.e. what data must be passed to effect)
    * Physical data?
      * mass
      * speed
      * local partition position
* Binary Space Partitioning:
  * Configure:
    * partition depth
    * partition size (at each depth layer)
    * total space size?
      * or do we just create space as we need based on object creation?
  * Information contained:
    * position
    * list of objects (objects may just be sub-partitions)
    * bounding object (cube)
  * Load objects into 
* View Frustum Culling
  * Use binary space partitioning
* Stream data (like outerra) instead of loading all at once?
  * really means split data for sending to GPU based on need:
    * don't need to send full earth texture/height-maps since can't see all of it at once
* HDR (High dynamic range) Rendering:
  * render to FP texture
  * Get max value
  * Scale so all values are [0,1] based on max value
  * render quad to screen with texture from FP texture
* Water Layer for earth:
  * Books:
    * GPU PRO 2 CH 1:1
    * GPU PRO 2 CH 5:3
    * GPU PRO 3 CH 2:3
    * GPU PRO 5 CH 1:4
  * sphere mesh at sea level
  * with some sort of reflection effect?
  * Effects:
    * Wave tessellation
    * above surface: reflection and refraction
    * below surface: color, etc.?
* Collision Detection & interaction
* Terrain Generation 2:
  * generate based on biome data instead of heightmap (similar terrain as found in GPU gems)
* Atmosphere 2:
  * Books:
    * GPU PRO 3 CH 2:2
    * GPU PRO 5 CH 2:2
  * More realistic scattering, better effects (sky not exactly right color yet)
* Sun Rendering 1:
  * Render light source (sun) as object viewable outside atmosphere
* Clouds 2:
  * moves
  * First implementation of volumetric clouds based on cloud texture as seed data
* Planets 2:
  * Simulation: motion (planetary bodies)
  * gravity
  * collision
* Trees 1:
* Grass 1:
  * Books:
    * GPU PRO 5 CH 3:1
* Physics:
  * Books:
    * GPU PRO 5 CH 6:4
  * Gravity (n-body)
  * Aero (JSBSim)
  * fluid mechanics (water, air?)
  * modular engine

Done:
-----
* Camera that can fly around freely: better movement than the current controls
* Terrain tessellation 1: 
  * currently, project just creates a huge sphere statically depending on the predefined size
  * Instead, do the poly generation (tessellation) in the geometry shader
    * based on distance to eye and position of eye vs sphere
    * know: radius of sphere, position of sphere, position of eye, direction of eye
* Terrain Generation 1:
  * after tesselation, we have control over poly placement in shader
  * do real terrain generation based on heightmap
* Atmosphere for earth 1:
  * sphere mesh(s) at atmo level
  * effects:
    * above atmo: blur and shadows and scattering
    * in atmo: blur and color and scattering
* Clouds 1:
  * Cloud texture over earth
  * works w/ atmosphere
  * Rendered from above and below
* Planet Rotation 
  * To work with atmospheric rendering
NEW:
* Add Camera control 
  * Flesh out input class & interface
  * Add control to engine for camera?


  Batman:
  ------

MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MM.:  .:'   `:::  .:`MMMMMMMMMMM|`MMM'|MMMMMMMMMMM':  .:'   `:::  .:'.MM
MMMM.     :          `MMMMMMMMMM  :*'  MMMMMMMMMM'        :        .MMMM
MMMMM.    ::    .     `MMMMMMMM'  ::   `MMMMMMMM'   .     ::   .  .MMMMM
MMMMMM. :   :: ::'  :   :: ::'  :   :: ::'      :: ::'  :   :: ::.MMMMMM
MMMMMMM    ;::         ;::         ;::         ;::         ;::   MMMMMMM
MMMMMMM .:'   `:::  .:'   `:::  .:'   `:::  .:'   `:::  .:'   `::MMMMMMM
MMMMMM'     :           :           :           :           :    `MMMMMM
MMMMM'______::____      ::    .     ::    .     ::     ___._::____`MMMMM
MMMMMMMMMMMMMMMMMMM`---._ :: ::'  :   :: ::'  _.--::MMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMM::.         ::  .--MMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM-.     ;::-MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM. .:' .M:F_P:MMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.   .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\ /MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMVMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


                     Tb.          Tb.                                
                     :$$b.        $$$b.                              
                     :$$$$b.      :$$$$b.                            
                     :$$$$$$b     :$$$$$$b                           
                      $$$$$$$b     $$$$$$$b                          
                      $$$$$$$$b    :$$$$$$$b                         
                      :$$$$$$$$b---^$$$$$$$$b                        
                      :$$$$$$$$$b        ""^Tb                       
                       $$$$$$$$$$b    __...__`.                      
                       $$$$$$$$$$$b.g$$$$$$$$$pb                     
                       $$$$$$$$$$$$$$$$$$$$$$$$$b                    
                       $$$$$$$$$$$$$$$$$$$$$$$$$$b                   
                       :$$$$$$$$$$$$$$$$$$$$$$$$$$;                  
                       :$$$$$$$$$$$$$^T$$$$$$$$$$P;                  
                       :$$$$$$$$$$$$$b  "^T$$$$P' :                  
                       :$$$$$$$$$$$$$$b._.g$$$$$p.db                 
                       :$$$$$$$$$$$$$$$$$$$$$$$$$$$$;                
                       :$$$$$$$$"""^^T$$$$$$$$$$$$P^;                
                       :$$$$$$$$       ""^^T$$$P^'  ;                
                       :$$$$$$$$    .'       `"     ;                
                       $$$$$$$$;   /                :                
                       $$$$$$$$;           .----,   :                
                       $$$$$$$$;         ,"          ;               
                       $$$$$$$$$p.                   |               
                      :$$$$$$$$$$$$p.                :               
                      :$$$$$$$$$$$$$$$p.            .'               
                      :$$$$$$$$$$$$$$$$$$p...___..-"                 
                      $$$$$$$$$$$$$$$$$$$$$$$$$;                     
   .db.          bug  $$$$$$$$$$$$$$$$$$$$$$$$$$                     
  d$$$$bp.            $$$$$$$$$$$$$$$$$$$$$$$$$$;                    
 d$$$$$$$$$$pp..__..gg$$$$$$$$$$$$$$$$$$$$$$$$$$$                    
d$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$p._            .gp. 
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$p._.ggp._.d$$$$b
