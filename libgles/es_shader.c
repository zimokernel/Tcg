/*************************************************************************
	> File Name: es_shader.c
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 17:26:40 2017
 ************************************************************************/

#include <stdlib.h>
#include "es_common.h"

GLuint ESUTIL_API
es_load_shader(GLenum type, const char *src)
{
    GLuint shader;
    GLint compiled;
    // Create the shader object
    shader = glCreateShader(type);
    if(shader == 0) {
        return 0;
    }
    //Load the shader source
    glShaderSource(shader, 1, &src, NULL);
    //Compile the shader
    glCompileShader(shader);
    //Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint info_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        if(info_len > 1) {
            char *info_log = malloc(sizeof(char)*info_len);
            glGetShaderInfoLog(shader, info_len, &info_len, info_log);
            es_log_message("Error compiling shader:\n%s\n",info_log);
            free(info_log);
        }
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

GLuint ESUTIL_API
es_load_program(const char *vert, const char *frag)
{
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program_obj;
    GLint linked;
    // Load the vertex/fragment shaders
    vertex_shader = es_load_shader(GL_VERTEX_SHADER, vert);
    if(vertex_shader == 0) {
        return 0;
    }
    fragment_shader = es_load_shader(GL_FRAGMENT_SHADER, frag);
    if(fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return 0;
    }
    // Create the program object
    program_obj = glCreateProgram();
    if(program_obj == 0) {
        return 0;
    }
    glAttachShader(program_obj, vertex_shader);
    glAttachShader(program_obj, fragment_shader);
    // Link the program
    glLinkProgram(program_obj);
    // Check the link status
    glGetProgramiv(program_obj, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char *infoLog = malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program_obj, infoLen, NULL, infoLog);
            es_log_message("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(program_obj);
        return 0;
    }
    // Free up no longer needed shader resources
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program_obj;
}
