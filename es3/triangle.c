/*************************************************************************
	> File Name: triangle.c
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 22:17:20 2017
 ************************************************************************/

/*
 This is a simple example that draws a single triangle with
 a minimal vertex/fragment shader.  The purpose of this
 example is to demonstrate the basic concepts of
 OpenGL ES 3.0 rendering.
 */
#include "es_common.h"

typedef struct
{
    GLuint programObject;
} UserData;

GLuint
triangle_load_shader(GLenum type, const char *src)
{
   return es_load_shader(type, src);
}

int
triangle_init(ESContext *ctx)
{
    UserData *userData = ctx->userData;
    char vShaderStr[] =
    "#version 300 es                          \n"
    "layout(location = 0) in vec4 vPosition;  \n"
    "void main()                              \n"
    "{                                        \n"
    "   gl_Position = vPosition;              \n"
    "}                                        \n";
    
    char fShaderStr[] =
    "#version 300 es                              \n"
    "precision mediump float;                     \n"
    "out vec4 fragColor;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    "   fragColor = vec4(1.0, 0.0, 0.0, 1.0);  \n"
    "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;
    
    // Load the vertex/fragment shaders
    vertexShader = triangle_load_shader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = triangle_load_shader(GL_FRAGMENT_SHADER, fShaderStr);
    
    // Create the program object
    programObject = glCreateProgram();
    if(programObject == 0) {
        return 0;
    }
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    // Link the program
    glLinkProgram(programObject);
    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            es_log_message("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return GL_FALSE;
    }
    
    // Store the program object
    userData->programObject = programObject;
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return GL_TRUE;
}

void
triangle_draw(ESContext *ctx)
{
    UserData *userData = ctx->userData;
    GLfloat vVertices[] = {
        0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    // Set the viewport
    glViewport(0, 0, ctx->width, ctx->height);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // Use the program object
    glUseProgram(userData->programObject);
    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void
triangle_shutdown(ESContext *ctx)
{
    UserData *userData = ctx->userData;
    glDeleteProgram(userData->programObject);
}

int
triangle_main(ESContext *ctx)
{
    ctx->userData = malloc(sizeof(UserData));
    
    es_create_window(ctx, "Hello Triangle", 320, 240, ES_WINDOW_RGB);
    
    if(!triangle_init(ctx))
    {
        return GL_FALSE;
    }
    
    es_register_shutdownfunc(ctx, triangle_shutdown);
    es_register_drawfunc(ctx, triangle_draw);
    
    return GL_TRUE;

}

