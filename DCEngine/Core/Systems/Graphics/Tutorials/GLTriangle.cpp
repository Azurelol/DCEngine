#include "GLTriangle.h"

namespace Tutorial {

  /**************************************************************************/
  /**************************************************************************/
  /*                            Drawing a Triangle
  /**************************************************************************/
  /*    Initialize()
  /**********************/
  void GLTriangle::Initialize() {

    //////////////////
    // 1.1 VERTEX INPUT
    //////////////////

    /* 1. To start drawing something we first have to give OpenGL some input vertex
       data. Because we want to render a single triangle, we want to specify a total
       of 3 vertices with each vertex having a 3D position. We define them
       in a normalized device coorddinates (the visible region of OpenGL) in a
       GLFloat array. */
    GLfloat vertices[]{
      -0.5f, -0.5f, 0.0f, // Because OpenGL works in 3D space, we render a 2D triangle
      0.5f,  -0.5f, 0.0f, // with each vertex having a z coordinate of 0.0
      0.0f,  0.5f,  0.0f  // This way the depth of the triangle remains the same.
    };

    /* (!) Normalized Device Coordinates (NDC)
       Once vertex coordinates have been processed in the vertex shader, they should
       be in NDC coordinates which is a small space where the x,y,z values range from
       -1.0 to 1.0. Any coordinates outside this range will be discarded/clipped and
       won't be visible on screen.
       
       Unlike usual screen coordinates, the positive y-axis points in the up-direction,
       and the (0, 0) coordinates are at the center of the graph. Eventually you want
       all transformed coordinates to end up in this coordinate space, otherwise
       they won't be visible.
       
       NDC coordinates will be transformed to screen-space coordinates via the
       viewport transofmr using the data provided with glViewport. The resulting
       screen space coordinates are then transformed to fragments as inputs to
       the fragment shader. */


    /* 2. With the vertex data defined, we'd like to send it as input to the first
       process of the graphics pipeline: the vertex shader. This is done by:
       - Creating memory on the GPU where we store the vertex data
       - Configure how OpenGL should interpret the memory
       - Specify how to send the data to the graphics card
       The vertex shader then processes as much vertices as well it to from its memory. */

    /* Vertex Buffer Objects (VBO), An OpenGL object which has an unique ID corrresponding
       to the buffer. It stores a large number of vertices in the GPU's memory. The
       advantage of using these buffer objects is that a large batch of data can be sent 
       all at once to the graphics card. */
    GLuint VBO;
    glGenBuffers(1, &VBO); // Generates the buffer ID

    /* (!) OpenGL has many types of buffer objects and the buffer type of a vertex buffer
       object is GL_ARRAY_BUFFER. OpenGL allows us to bind several buffers at once as
       long as they have a different buffer type. */
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // We bind the newly created buffer to the GL_
                                        // ARRAY_BUFFER target with glBindBUffer function.

    /* (!) From this point any buffer calls made (on the GL_ARRAY_BUFFER target) will be
       used to configure the currently bound buffer, VBO. */

    /* A function specifically targeted to copy user-defined data into the currently
     bound buffer. 
     + The first argument is the type of buffer we want to copy data into: the vertex
       buffer object currently bound to the GL_ARRAY_BUFFER target.
     + The second argument specifies the size of the data (in bytes) we want to pass
       to the buffer; a simple sizeof of the vertex data suffices.
     + The third argument is the actual data we want to send.
     + The fourth argument specifies how we want the graphics card to manage the given
       data. This can take 3 forms:
       - GL_STATIC_DRAW: The data will most likely not change at all.
       - GL_DYNAMIC_DRAW: The data is likely to change a lot.
       - GL_STREAM_DRAW: The data will change everytime it is drawn.
       
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* (?) Since the position data of the triangle won't be changing and will stay
       the same for every render call, its usage type should be GL_STATIC_DRAW.
       If one had a buffer with data changing frequently, the other 2 draw types would
       ensure the graphics card places the data in memory that allows for faster writes. 
    */

    /* As of now, we have stored the vertex data within memory on the graphics card as 
       managed by a vertex buffer object (VBO). */

     ////////////////////////////////
     // 1.2 CREATING THE VERTEX SHADER
     ////////////////////////////////

    /* We now want to create a vertex and fragment shader that process this data. 
       The vertex shader is one of the shaders programmable by us. Modern OpenGL requires
       that we at least set up a vertex and fragment shader if we want to do some rendering. 
    */

    /* The first thing needed is to write the vertex shader in the shader language GLSL, then
       compile the shader so it can be used in the application. */

    //////////////////////////////////////////////////////////////////////////
    // The vertex shader has been written in the file "VertexShader.glsl"

    /* This vertex shader is currently the most simple vertex shader we can imagine
       as it did no processing on input data and simply forwarded it to the shader's output.
       In real applications the input data is not arleady in NDC, so we first have to
       transform the input data to coordinates within OpenGL's visible region.
    */

    //////////////////////////////////
    // 1.3 COMPILING THE VERTEX SHADER
    //////////////////////////////////

    std::string vertexSource = GetGLSLCode("VertexShader.glsl");      
    const GLchar* vertexShaderSource = (GLchar*)vertexSource.c_str();

    /* We wrote the source code for the vertex shader, but in order for OpenGL to use
       the shader it has to dynamically compile it at run-time from its source code. 
       The first thing we need to do is create a shader object, again referenced by an ID.
    */

    // We store the vertex shader in a GLuint
    GLuint vertexShader;
    // Create the shader, providing the type of shader we want to create as an argument
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    /* The 'glShaderSource' function will attach the shader source code. It takes: 
       + A shader object
       + How many strings we're passing as source code (1)
       + The actual source code of the vertex shader
       + Can leave as NULL
    */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the shader
    glCompileShader(vertexShader);
    
    // Check if the shader was compiled successfully
    AssertShaderCompilation(vertexShader);

    ////////////////////////////////
    // 2.1 CREATING FRAGMENT SHADER
    ////////////////////////////////

    /* The fragment shader calculates the color output of the pixels. For this tutorial,
       to keep things simple we will always outpout an orange-ish color*/

    /* (!) Colors in CG are represented in 4 values: red, green, blue and alpha (opacity),
        commonly known as RGBA. When defining a color in OpenGL or GLSL, we set the strength
        of each component 0.0 - 1.0. For example, to generate the color yellow we would set
        red to 1.0f, green to 1.0f. 
    */

    //////////////////////////////////////////////////////////////////////////
    // The fragment shader has been written in the file "FragmentShader.glsl"

    ///////////////////////////////////
    // 2.1 COMPILING THE FRAGMENT SHADER
    ///////////////////////////////////

    /* The process for compiling a fragment shader is similar to that of a vertex 
       shader. */
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    
    
    //glShaderSource()
   


  }

  /************************/
  /*    Update()
  /************************/
  void GLTriangle::Update() {

  }

}