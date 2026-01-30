A C++ gravity simulation implementing the Barnes-Hut algorithm I wrote in June 2025, the latest entry in my
lifelong tradition of writing gravity simulations. I was quite proud of it at the time; it could handle ~88k
bodies at a "respectable" 20 FPS. Looking back at it now, though, I see how far my C++ coding has come.

The code isn't laid out all that great, it's not really const-correct, I hadn't yet learned a lot of C++ best
practices, there isn't any parallelism except where C++ did it all for me, and a big speedup could probably be
gained by using Struct of Arrays instead of Array of Structs as well as following other data-oriented design
principles.

It's definitely good for the time, but I can do a lot better now, I think. I plan to make another gravity 
simulation, this time using OpenGL, leveraging compute shaders to do the updating in proper parallel, and
allowing even finer control over optimization.
