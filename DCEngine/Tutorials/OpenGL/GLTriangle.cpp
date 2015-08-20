#include "GLTriangle.h"

#define WIREFRAME 0
#define DRAW_RECTANGLE 0 // When set to 0, will draw a triangle instead

namespace DCEngine {
  namespace Tutorial {

    /**************************************************************************/
    /**************************************************************************/
    /*                            Drawing a Triangle
    /**************************************************************************/
    /*    Initialize()
    /**********************/
    void GLTriangle::Initialize() {
      trace << "GLTriangle::Initialize \n";

      //////////////////
      // 1.1 VERTEX INPUT
      //////////////////

      /* 1. To start drawing something we first have to give OpenGL some input vertex
         data. Because we want to render a single triangle, we want to specify a total
         of 3 vertices with each vertex having a 3D position. We define them
         in a normalized device coordinates (the visible region of OpenGL) in a
         GLFloat array. */

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
#if(!DRAW_RECTANGLE)
      GLfloat vertices[]{
        -0.5f, -0.5f, 0.0f, // Because OpenGL works in 3D space, we render a 2D triangle
        0.5f,  -0.5f, 0.0f, // with each vertex having a z coordinate of 0.0
        0.0f,  0.5f,  0.0f  // This way the depth of the triangle remains the same.
      };

      /* 2. With the vertex data defined, we'd like to send it as input to the first
      process of the graphics pipeline: the vertex shader. This is done by:
      - Creating memory on the GPU where we store the vertex data
      - Configure how OpenGL should interpret the memory
      - Specify how to send the data to the graphics card
      The vertex shader then processes as much vertices as well it to from its memory. */
#endif 

      /* Vertex Buffer Objects (VBO), An OpenGL object which has an unique ID corrresponding
      to the buffer. It stores a large number of vertices in the GPU's memory. The
      advantage of using these buffer objects is that a large batch of data can be sent
      all at once to the graphics card. */
      //GLuint VBO;
      glGenBuffers(1, &VBO); // Generates the buffer ID

      /* Element Buffer Objects (EBO). An EBO is a buffer, just like A VBO, that stores
         indices that OpenGL uses to decide what vertices to draw. This is called 'indexed
         drawing' and allows us to store only the unique vertices of a shape then specify
         the order at which we want to draw these vertices in. */

         // First, we have to specify the unique (vertices),
#if(DRAW_RECTANGLE)
      GLfloat vertices[]{
        0.5f,  0.5f, 0.0f, // Top Right
        0.5f, -0.5f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f  // Top Left
      };


      // and the indices to draw them as a rectangle.
      GLuint indices[]{ // (!) Note that we start from element [0] in the verticesEBO array
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
      };

      glGenBuffers(1, &EBO);
#endif

      /* (!) OpenGL has many types of buffer objects and the buffer type of a vertex buffer
         object is GL_ARRAY_BUFFER. OpenGL allows us to bind several buffers at once as
         long as they have a different buffer type. */
#if(!DRAW_RECTANGLE)
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
            - GL_STREAM_DRAW: The data will change everytime it is drawn. */
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#endif

#if(DRAW_RECTANGLE)
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#endif


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

           The first thing needed is to write the vertex shader in the shader language GLSL, then
           compile the shader so it can be used in the application. */

           //////////////////////////////////////////////////////////////////////////
           // The vertex shader has been written in the file "VertexShader.glsl"
      std::string vertexSource = GetGLSLCode("VertexShader.glsl");
      const GLchar* vertexShaderSource = (GLchar*)vertexSource.c_str();

      /* This vertex shader is currently the most simple vertex shader we can imagine
         as it did no processing on input data and simply forwarded it to the shader's output.
         In real applications the input data is not arleady in NDC, so we first have to
         transform the input data to coordinates within OpenGL's visible region.
      */

      //////////////////////////////////
      // 1.3 COMPILING THE VERTEX SHADER
      //////////////////////////////////

      /* We wrote the source code for the vertex shader, but in order for OpenGL to use
         the shader it has to dynamically compile it at run-time from its source code.
         The first thing we need to do is create a shader object, again referenced by an ID.
      */

      // We store the vertex shader in a GLuint
      //GLuint vertexShader;
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
      AssertShaderCompilation(vertexShader, "Vertex shader");

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
      std::string fragmentSource = GetGLSLCode("FragmentShader.glsl");
      const GLchar* fragmentShaderSource = (GLchar*)fragmentSource.c_str();

      ///////////////////////////////////
      // 2.2 COMPILING THE FRAGMENT SHADER
      ///////////////////////////////////

      /* The process for compiling a fragment shader is similar to that of a vertex
         shader. */
         //GLuint fragmentShader;
      fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragmentShader);

      // Check if the shader was compiled successfully
      AssertShaderCompilation(fragmentShader, "Fragment shader");

      ///////////////////////////////////
      // 3.1 SHADER PROGRAM
      ///////////////////////////////////

      /* A shader program is the final linked version of multiple shaders combined. To use
         recently compiled shaders we have to link them to a shader program object, then
         activate this shader program when rendering objects. The activated shader program's
         shaders will be the ones used when we issue render calls.

         When linking shaders into a program it links the outputs of each shader to the
         inputs of the next shader. (This is why you get linking errors if outputs and
         inputs do not match.)

      */

      // The 'glCreateProgram' function creates a program and returns the ID reference
      // to the newly created program object.
      //GLuint shaderProgram;
      shaderProgram = glCreateProgram();

      // Attaches the previously compiled shaders to the program object
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      // Links them
      glLinkProgram(shaderProgram);
      // Check if the shader program linked successfully
      AssertShaderProgramLinking(shaderProgram, "Shader Program");

      // We now have a program object that we can activate 
      //glUseProgram(shaderProgram);

      /* (!) Every shader and rendering call after glUseProgram will now use
          this program object (and thus the shaders). */

          // Now that we have linked the shader objects with the program object, 
          // we no longer need them. Delete them!
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      /* We have sent the input vertex data to the GPU and instructed how it
         should process the vertex data within a vertex and fragment shader.

         We still have to tell OpenGL how it should it interpret the vertex data
         in memory and how it should connect the vertex data to the vertex shader's
         attributes. */

         ///////////////////////////////////
         // 4.1 LINKING VERTEX ATTRIBUTES
         ///////////////////////////////////

         /* The vertex shader allows us to specify any input we want in the form of vertex
            attributes. While this allows for great flexibility, it means we have to manually
            specify what part of our input data goes to which vertex attribute in the vertex
            shader.

            We have to specify how OpenGL should interpret the vertex data before rendering.

            Vertex buffer data is formatted as follows:

            [  VERTEX 1 ][  VERTEX 2 ][  VERTEX 3 ]
            [ X | Y | Z ][ X | Y | Z ][ X | Y | Z ]
            BYTE:     0   4   8   12   16  20  24   28  32  36
            POSITION: --STRIDE:12->
            -OFFSET: 0

            + The position data is stored as 32-bit (4 byte) floating point values.
            + Each position is compsoed of 3 of those values.
            + There is no space (or other values) between each set of 3 values. They are
              tightly packed in the array.
            + The first value in the data is the beginning of the buffer.

         */

         /* We can tell OpenGL how it should interpret the vertex data (per vertex attribute)
            using 'glVertexAttribPointer':

            + The first argument specifies which vertex attribute we want to configure.
              (?) Remember that we specified the location of the 'position' vertex attribute
              in the vertex shader with 'layout (location = 0)'.
            + The second argument specifies the size of the vertex attribute.
              (?) The vertex attribute is a vec3 so it's composed of 3 values.
            + The third argument specifies the type of the data which is GL_FLOAT.
              (?) A vec* in GLSL consists of floating point values.
            + The fourth argument specifies if we want the data to be normalized. When set to
              true all data that doesn't have a value between 0 (or -1 for signed) and 1 will be
              mapped to those values.
              (?) We leave this at false.
            + The fifth argument is known as the stride and tells us the space between consecutive
              vertex attribute sets. Since the next set of position data is located exactly 3
              times the size of a GLfloat away, we use that value as the stride.
              We could have specified the the stride as 0 to let OpenGL determine the stride.
              (Only works when the values are tightly packed.)
            + The six and last argument is of type GLvoid* and requires a weirdd cast. This is the
              offset where the position data begins in the buffer.
              (?) Since the position data is at the start of the data array the value is just 0. */

              //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

              // Enables the vertex attribute, giving the vertex attribute location as its argument.
              // Vertex attributes are disabledby default.
              // glEnableVertexAttribArray(0);

              /* (!) Each vertex attribute takes its data from memory managed by a VBO, and which VBO
                 takes it data from (one could have multiple VBOs) is determined by the VBO currently
                 bound to GL_ARRAY_BUFFER when calling 'glVertexAttribPointer'.
                 Since the previously defined VBO was bound before calling 'glVertexAttribPointer',
                 vertex attribute '0' is now associated with its vertex data. */

                 /* We have everything set up: We have initialized the vertex data in a buffer using a
                    VBO, set up a vertex and fragment shader and told OpenGL how to link the vertex data to
                    to the vertex shader's attributes. We now have to draw the object. */

                    ////////////////////////////
                    // 5.1 VERTEX ARRAY OBJECT
                    ////////////////////////////

                 /* A vertex array object (VAO) can be bound just like a vertex buffer object and any
                    subsequent vertex attribute calls from that point on will be stored inside the VAO.
                    This has the advantage that when configuring vertex attribute pointers you only have to
                    make those calls once and whenever we want to draw the object, we can just bind the
                    corresponding VAO.
                    This makes switching between different vertex data and attribute configurations as easy
                    as binding a different AO. All the state we just set is stored inside the VAO.
                    Core OpenGL requires that we use a VAO so that it knows what to do with our vertex
                    inputs. If we fail to bind a VAO, OpenGL will refuse to draw.

                    A vertex array object stores:
                    + Calls to 'glEnableVertexAttribArray' or 'glDisableVertexAttribArray'
                    + Vertex attribute configurations via 'glVertexAttribPointer'
                    + Vertex buffer objects associated with vertex atrtibutes by calls to 'glVertexAttribPointer'
                 */

                 // THe process to generate a VAO looks similar to that of a VBO
                 //GLuint VAO;
      glGenVertexArrays(1, &VAO);

      /* To use a VAO all you have to do is bind the VAO using 'glBindVertexArray'. From then on,
         we bind/configure the corresponding VBO(s) and attribute pointer(s), then unbind the VAO
         for later use.
         As soon as we want to draw an object, we simply bind the VAO with the preferred settings
         before drawing the object. */

         /*
           (!) Initialization code for the object we want to draw
         */

         // 1. Bind the Vertex Array Object    
      glBindVertexArray(VAO);

      // 2. Copy the vertices array in a buffer for OpenGL to use
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#if(DRAW_RECTANGLE)
      // 3.1 Copy our index array in an element buffer for OpenGL to use
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif

      // 3.2 Then set the vertex attribute pointers
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);

      // 4. Unbind the VAO
      glBindVertexArray(0);

      /* (!) It is common practice to unbind OpenGL objects when we're done configuring them,
         as to not mistakenly misconfigure them elsewhere.

         Usually when you have multiple objects that you want to draw, you first generate/configure
         all the VAOs (and thus, the requried VBO and attrib pointers) and store them for later use.

         The moment you want to draw one of our objects, we take the corresponding VAO, bind it,
         draw the object, then unbind the VAO again. */

#if (WIREFRAME)
         /* You can configure how OpenGL draws its primitives via 'glPolygonMode'.
            The first argument says we want to apply it to front and back all of triangles,
            and the secondline tells us to draw them as lines. Any subsequent drawing calls
            will render the triangles in wireframe mode until we set it back to its default. */
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // To set back to default:
      // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    }

    /************************/
    /*    Update()
    /************************/
    void GLTriangle::Update() {

      /*(!) Drawing code for the object */
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);

#if(!DRAW_RECTANGLE)
      /* OpenGL function that draws the object:
      + The first argument is the OpenGL primitive type we want to draw.
      + The second argument specifies the starting index of the vertex array, left at 0.
      + The third argument specifies how many vertices to draw.
      (?) Since we are only rendering 1 triangle from our data, it is exactly 3 vertices long. */
      glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

#if(DRAW_RECTANGLE)
      /* OpenGL function that draws the object:
      + The first argument specifies the mode we want to draw in.
      + The second argument is the count or number elements we want to draw.
      (?) We specified 6 indices so we want to draw 6 vertices in total.
      + The third argument is the type of indices, which is of type GL_UNSIGNED_INT.
      + The last argument allows us to specify an offset in the EBO (or pass in an index array),
      but that is when you're not using element buffer objects.

      The 'glDrawElements' function takes its indices from the EBO currently bound
      to the GL_ELEMENT_ARRAY_BUFFER. We have to bind the corresponding EBO each time
      we render an object with indices. A VAO also keeps track of element buffer object
      bindings. The EBO currently bound while a VAO is bound, is stored as the VAO's
      EBO.

      Thus, binding to a VAO also automatically binds its EBO.

      A VAO stores its glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
      This means it stores its unbind calls, so make sure you don't unbind the EBO
      before unbinding your VAO. Otherwise it won't have an EBO configured. */
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif    

      glBindVertexArray(0);
      trace << "GLTriangle::Update - Drawing triangle \n";
    }

    void GLTriangle::Terminate() {

      // Properly de-allocate all resources once they've outlived their purpose
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1, &VBO);
      glDeleteBuffers(1, &EBO);

    }

  }
}