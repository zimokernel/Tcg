/*************************************************************************
	> File Name: es_common.h
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 17:27:03 2017
 ************************************************************************/

#ifndef _ES_COMMON_H
#define _ES_COMMON_H

#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif

#ifdef WIN32
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl
#else
#define ESUTIL_API
#define ESCALLBACK
#endif

// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

    
typedef struct
{
    GLfloat m[4][4];
}ESMatrix;
    
typedef struct ESContext ESContext;

struct ESContext
{
    void       *platformData;
    void       *userData;
    GLint       width;// Window width
    GLint       height;
#ifndef __APPLE__
    // Display handle
    EGLNativeDisplayType eglNativeDisplay;
    // Window handle
    EGLNativeWindowType  eglNativeWindow;
    // EGL display
    EGLDisplay  eglDisplay;
    // EGL context
    EGLContext  eglContext;
    // EGL surface
    EGLSurface  eglSurface;
#endif
    void(ESCALLBACK *drawfunc)(ESContext *);
    void(ESCALLBACK *shutdownfunc)(ESContext *);
    void(ESCALLBACK *keyfunc)(ESContext *, unsigned char, int, int);
    void(ESCALLBACK *updatefunc)(ESContext *, float deltaTime);
};

/**
 Create a window with the specified parameters

 @param ctx Application context
 @param title Name for title bar of window
 @param width Width in pixels of window to create
 @param height height in pixels of window to create
 @param flags Bitfield for the window creation flags
         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created

 @return GL_TRUE if window creation is succesful, GL_FALSE otherwise
 */
GLboolean ESUTIL_API es_create_window(ESContext *ctx, const char *title, GLint width, GLint height, GLuint flags);

/**
 Register a draw callback function to be used to render each frame

 @param ctx Application context
 @param drawfunc Draw callback function that will be used to render the scene
 */
void ESUTIL_API es_register_drawfunc(ESContext *ctx, void(ESCALLBACK *drawfunc)(ESContext *));

/**
 Register a callback function to be called on shutdown

 @param ctx Application context
 @param shutdownfunc Shutdown callback function
 */
void ESUTIL_API es_register_shutdownfunc(ESContext *ctx, void(ESCALLBACK *shutdownfunc)(ESContext *));

/**
 Register an update callback function to be used to update on each time step

 @param ctx Application context
 @param updatefunc Update callback function that will be used to render the scene
 */
void ESUTIL_API es_register_updatefunc(ESContext *ctx, void(ESCALLBACK *updatefunc)(ESContext *, float));

/**
 Register a keyboard input processing callback function

 @param ctx Application context
 @param keyfunc Key callback function for application processing of keyboard input
 */
void ESUTIL_API es_register_keyfunc(ESContext *ctx,
                                   void(ESCALLBACK *keyfunc)(ESContext *, unsigned char, int, int));

void ESUTIL_API es_log_message(const char *format, ...);
/**
 load a shader ,check for compile errors, print error message to output log

 @param type Type of shader(GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 @param src Shader source string
 @return A new shader object on success, 0 => failure
 */
GLuint ESUTIL_API es_load_shader(GLenum type, const char *src);
/**
 Load a vertex and fragment shader, create a progrom object,link program.
 Errors output to log.
 @param vert Vertex shader source code
 @param frag Fragment shader source code
 @return A new program object linked with the vertex/fragment shader pair,0=>failure
 */
GLuint ESUTIL_API es_load_program(const char *vert, const char *frag);
/**
 Generates geometry for a sphere. Allocates memory for the vertex data and stores
 the results in the arrays.
 Generate index list for a TRIANGLE_STRIP

 @param numslices The number of slices in the sphere
 @param radius radius
 @param vertices If not NULL, will contain array of float3 postions
 @param normals If not NULL, will contain array of float3 normals
 @param texcoords If not NULL, will contain array of float2 texcoords
 @param indices If not NULL, will contain the array of indices for the triangle strip
 @return The number of indices required for rendering the buffer
    (the number of indices stored in the indices array, if it is not NULL),as
     a GL_TRIANGLE_STRIP.
 */
int ESUTIL_API es_gen_sphere(int numslices, float radius, GLfloat **vertices, GLfloat **normals,
                            GLfloat **texcoords, GLushort **indices);

/**
 Generates geometry for a cube.  Allocates memory for the vertex data and stores
 the results in the arrays.
 Generate index list for a TRIANGLES

 @param scale The size of the cube, use 1.0 for a unit cube.
 @param vertices If not NULL, will contain array of float3 positions
 @param normals If not NULL, will contain array of float3 normals
 @param texcoords If not NULL, will contain array of float2 texCoords
 @param indices If not NULL, will contain the array of indices for the triangle strip
 @return The number of indices required for rendering the buffer
    (the number of indices stored in the indices array, if it is not NULL),as
     a GL_TRIANGLE_STRIP.
 */
int ESUTIL_API es_gen_cube(float scale, GLfloat **vertices, GLfloat **normals,
                          GLfloat **texcoords, GLushort **indices);


/**
 Generates a square grid consisting of triangles. Allocates memory for the vertex data and stores
 the results in the arrays.  Generate index list as TRIANGLES.
 @param size size create a grid of size by size(number of triangles =(size-1)*(size-1)*2)
 @param vertices If not NULL, will contain array of float3 positions
 @param indices If not NULL, will contain the array of indices for the triangle strip
 @return The number of indices required for rendering the buffers(the number of indices stored in the indices array if it is not NULL) as a GL_TRIANGLES.
 */
int ESUTIL_API es_gen_squaregrid(int size, GLfloat **vertices, GLuint **indices);

/**
 Loads a 8-bit, 24-bit or 32-bit TGA image from a file

 @param io Context related to IO facility on the platform
 @param filename Name of the file on disk
 @param width Width of loaded image in pixels
 @param height Height of loaded image in pixels
 @return Pointer to loaded image.  NULL on failure
 */
char *ESUTIL_API es_load_tga(void *io, const char *filename, int *width, int *height);


/**
 Load a png image from a file

 @param filename Base file of the name on disk(without png extension)
 @param width Width of loaded image in pixels
 @param height Height of loaded image in pixels
 @return Pointer to loaded image.  NULL on failure.
 */
char * ESUTIL_API es_load_png(char *filename, int *width, int *height);
/**
 Multiply matrix specified by result with a scaling matrix and return new matrix in result

 @param result Specifies the input matrix.  Scaled matrix is returned in result.
 @param sx sy, sz Scale factors along the x, y and z axes respectively
 */
void ESUTIL_API es_scale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);
/**
 Multiply matrix specified by result with a translation matrix and return new matrix in result

 @param result Specifies the input matrix.  Translated matrix is returned in result.
 @param tx ty, tz Scale factors along the x, y and z axes respectively
 */
void ESUTIL_API es_translate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);
/**
 Multiply matrix specified by result with a rotation matrix and return new matrix in result

 @param result Specifies the input matrix.  Rotated matrix is returned in result.
 @param angle Specifies the angle of rotation, in degrees.
 @param x y, z Specify the x, y and z coordinates of a vector, respectively
 */
void ESUTIL_API es_rotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
/**
 用一个透视图矩阵来指定乘法矩阵，并返回新的矩阵
 Multiply matrix specified by result with a perspective matrix and return new matrix in result

 @param result Specifies the input matrix.  new matrix is returned in result.
 @param left right Coordinates for the left and right vertical clipping planes(左右垂直剪切面)
 @param bottom top Coordinates for the bottom and top horizontal clipping planes(上下水平剪切平面)
 @param nearz farz Distances to the near and far depth clipping planes.  Both distances must be positive.（深度剪切面，正数）
 */
void ESUTIL_API es_frustum(ESMatrix *result, float left, float right, float bottom, float top, float nearz, float farz);
/**
 用一个透视图矩阵来指定乘法矩阵，并返回新的矩阵
 Multiply matrix specified by result with a perspective matrix and return new matrix in result

 @param result Specifies the input matrix.  new matrix is returned in result.
 @param fovy Field of view y angle in degrees 视窗的角度
 @param aspect Aspect ratio of screen 屏幕的长宽比
 @param nearz Near plane distance 近平面的距离
 @param farz Far plane distance 远平面的距离
 */
void ESUTIL_API es_perspective(ESMatrix *result, float fovy, float aspect, float nearz, float farz);

/**
 用一个透视图矩阵来指定乘法矩阵，并返回新的矩阵
 Multiply matrix specified by result with a perspective matrix and return new matrix in result

 @param result result Specifies the input matrix.  new matrix is returned in result
 @param left right Coordinates for the left and right vertical clipping planes
 @param bottom top Coordinates for the bottom and top horizontal clipping planes
 @param nearz farz Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
 */
void ESUTIL_API es_ortho(ESMatrix *result, float left, float right, float bottom, float top, float nearz, float farz);

/**
 执行下列操作-结果矩阵= srcA矩阵* srcB矩阵
 perform the following operation - result matrix = srcA matrix * srcB matrix

 @param result Returns multiplied matrix
 @param srcA srcB Input matrices to be multiplied
 */
void ESUTIL_API es_matrix_multiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);
/**
 return an indentity matrix

 @param result returns identity matrix
 */
void ESUTIL_API es_matrix_load_identity(ESMatrix *result);

/**
 从眼睛位置生成一个变换矩阵，观察和向上的向量
 Generate a transformation matrix from eye position, look at and up vectors

 @param result Returns transformation matrix
 @param posX posY, posZ           eye position
 @param lookAtX lookAtY, lookAtZ  look at vector
 @param upX upY, upZ              up vector
 */
void ESUTIL_API
esMatrixLookAt(ESMatrix *result,
                float posX,    float posY,    float posZ,
                float lookAtX, float lookAtY, float lookAtZ,
                float upX,     float upY,     float upZ);


//#ifdef __cplusplus
//}
//#endif
#endif
