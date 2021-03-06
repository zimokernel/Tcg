/*************************************************************************
	> File Name: es_common.c
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 17:27:05 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "es_common.h"

#ifdef ANDROID
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
typedef AAsset esFile;
#else
typedef FILE esFile;
#endif

#ifdef __APPLE__
#include "FileWrapper.h"
#endif

#define INVERTED_BIT           (1 << 5)

#ifndef __APPLE__
#pragma pack(push,x1)                            // Byte alignment(8-bit)
#pragma pack(1)
#endif

typedef struct
#ifdef __APPLE__
__attribute__ ((packed))
#endif
{
    unsigned char  IdSize,
    MapType,
    ImageType;
    unsigned short PaletteStart,
    PaletteSize;
    unsigned char  PaletteEntryDepth;
    unsigned short X,
    Y,
    Width,
    Height;
    unsigned char  ColorDepth,
    Descriptor;
    
} TGA_HEADER;

#ifndef __APPLE__
#pragma pack(pop,x1)
#endif

#ifndef __APPLE__
EGLint
GetContextRenderableType(EGLDisplay eglDisplay)
{
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);
    // check whether EGL_KHR_create_context is in the extension string
    if(extensions != NULL && strstr(extensions, "EGL_KHR_create_context")) {
        // extension is supported
        return EGL_OPENGL_ES3_BIT_KHR;
    }
#endif
    // extension is not supported
    return EGL_OPENGL_ES2_BIT;
}
#endif


void ESUTIL_API
es_log_message(const char *format, ...)
{
    va_list ap;
    char buf[BUFSIZ];
    
    va_start(ap, format);
    vsprintf(buf, format, ap);
    
    printf("%s",buf);
    
    va_end(ap);
}
// Wrapper for platform specific File open
static esFile *
esFileOpen(void *ioContext, const char *fileName)
{
    esFile *pFile = NULL;
#ifdef ANDROID
    if(ioContext != NULL) {
        AAssetManager *assetManager =(AAssetManager *) ioContext;
        pFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    }
#else
#ifdef __APPLE__
    // iOS: Remap the filename to a path that can be opened from the bundle.
    fileName = GetBundleFileName(fileName);
#endif
    pFile = fopen(fileName, "rb");
#endif
    return pFile;
}
// Wrapper for platform specific File close
static void
esFileClose(esFile *pFile)
{
    if(pFile != NULL) {
#ifdef ANDROID
        AAsset_close(pFile);
#else
        fclose(pFile);
        pFile = NULL;
#endif
    }
}
// Wrapper for platform specific File read
static int
esFileRead(esFile *pFile, int bytesToRead, void *buffer)
{
    int bytesRead = 0;
    if(pFile == NULL) {
        return bytesRead;
    }
#ifdef ANDROID
    bytesRead = AAsset_read(pFile, buffer, bytesToRead);
#else
    bytesRead =(int)fread(buffer, bytesToRead, 1, pFile);
#endif
    return bytesRead;
}


GLboolean ESUTIL_API
es_create_window(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags)
{
#ifndef __APPLE__
    EGLConfig config;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    if(esContext == NULL) {
        return GL_FALSE;
    }
#ifdef ANDROID
    // For Android, get the width/height from the window rather than what the
    // application requested.
    esContext->width = ANativeWindow_getWidth(esContext->eglNativeWindow);
    esContext->height = ANativeWindow_getHeight(esContext->eglNativeWindow);
#else
    esContext->width = width;
    esContext->height = height;
#endif
    if(!WinCreate(esContext, title)) {
        return GL_FALSE;
    }
    esContext->eglDisplay = eglGetDisplay(esContext->eglNativeDisplay);
    if(esContext->eglDisplay == EGL_NO_DISPLAY) {
        return GL_FALSE;
    }
    // Initialize EGL
    if(!eglInitialize(esContext->eglDisplay, &majorVersion, &minorVersion)) {
        return GL_FALSE;
    }
    
    {
        EGLint numConfigs = 0;
        EGLint attribList[] = {
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
            EGL_ALPHA_SIZE,    (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
            EGL_DEPTH_SIZE,    (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
            EGL_STENCIL_SIZE,  (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
            EGL_SAMPLE_BUFFERS,(flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
            // if EGL_KHR_create_context extension is supported, then we will use
            // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
            EGL_RENDERABLE_TYPE, GetContextRenderableType(esContext->eglDisplay),
            EGL_NONE
        };
        
        // Choose config
        if(!eglChooseConfig(esContext->eglDisplay, attribList, &config, 1, &numConfigs)) {
            return GL_FALSE;
        }
        if(numConfigs < 1) {
            return GL_FALSE;
        }
    }
#ifdef ANDROID
    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib(esContext->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(esContext->eglNativeWindow, 0, 0, format);
    }
#endif // ANDROID
    // Create a surface
    esContext->eglSurface = eglCreateWindowSurface(esContext->eglDisplay, config,
                                                    esContext->eglNativeWindow, NULL);
    if(esContext->eglSurface == EGL_NO_SURFACE) {
        return GL_FALSE;
    }
    // Create a GL context
    esContext->eglContext = eglCreateContext(esContext->eglDisplay, config,
                                              EGL_NO_CONTEXT, contextAttribs);
    if(esContext->eglContext == EGL_NO_CONTEXT) {
        return GL_FALSE;
    }
    // Make the context current
    if(!eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
                          esContext->eglSurface, esContext->eglContext)) {
        return GL_FALSE;
    }
#endif // #ifndef __APPLE__
    return GL_TRUE;
}

void ESUTIL_API
es_register_drawfunc(ESContext *ctx, void(ESCALLBACK *drawfunc)(ESContext *))
{
    ctx->drawfunc = drawfunc;
}

void ESUTIL_API
es_register_shutdownfunc(ESContext *ctx, void(ESCALLBACK *shutdownfunc)(ESContext *))
{
    ctx->shutdownfunc = shutdownfunc;
}

void ESUTIL_API
es_register_updatefunc(ESContext *ctx, void(ESCALLBACK *updatefunc)(ESContext *, float))
{
    ctx->updatefunc = updatefunc;
}

void ESUTIL_API
es_register_keyfunc(ESContext *ctx,
                                     void(ESCALLBACK *keyfunc)(ESContext *, unsigned char, int, int))
{
    ctx->keyfunc = keyfunc;
}



char *ESUTIL_API
es_load_tga(void *io, const char *filename, int *width, int *height)
{
    char        *buffer;
    esFile      *fp;
    TGA_HEADER   Header;
    int          bytesRead;
    // Open the file for reading
    fp = esFileOpen(io, filename);
    if(fp == NULL) {
        // Log error as 'error in opening the input file from apk'
        es_log_message("esLoadTGA FAILED to load : { %s }\n", filename);
        return NULL;
    }
    
    bytesRead = esFileRead(fp, sizeof(TGA_HEADER), &Header);
    *width = Header.Width;
    *height = Header.Height;
    
    if(Header.ColorDepth == 8 ||
        Header.ColorDepth == 24 || Header.ColorDepth == 32) {
        int bytesToRead = sizeof(char) *(*width) *(*height) * Header.ColorDepth / 8;
        // Allocate the image data buffer
        buffer =(char *) malloc(bytesToRead);
        if(buffer) {
            bytesRead = esFileRead(fp, bytesToRead, buffer);
            esFileClose(fp);
            
            return(buffer);
        }
    }
    
    return NULL;
}
