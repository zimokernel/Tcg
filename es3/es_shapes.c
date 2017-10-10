/*************************************************************************
	> File Name: es_shapes.c
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 17:26:48 2017
 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "es_common.h"

#define ES_PI (3.14159265f)

int ESUTIL_API
es_gen_sphere(int numslices, float radius, GLfloat **vertices, GLfloat **normals,
                  GLfloat **texcoords, GLushort **indices)
{
    int i,j;
    int num_parallels = numslices / 2;
    int num_vertices =(num_parallels+1) *(num_parallels+1);
    int num_indices = num_parallels * numslices * 6;
    float angle_step =(2.0f * ES_PI) /((float)numslices);
    // Allocate memory for buffers
    if(vertices != NULL) {
        *vertices = malloc(sizeof(GLfloat) * 3 * num_vertices);
    }
    if(normals != NULL) {
        *normals = malloc(sizeof(GLfloat) * 3 * num_vertices);
    }
    if(texcoords != NULL) {
        *texcoords = malloc(sizeof(GLfloat) * 2 * num_vertices);
    }
    if(indices != NULL) {
        *indices = malloc(sizeof(GLushort) * num_indices);
    }
    
    for(i=0;i<num_parallels+1; i++) {
        for(j=0; j<numslices+1; j++) {
            int vertex =(i *(numslices + 1) + j) * 3;
            if(vertices) {
               (*vertices)[vertex + 0] = radius * sinf(angle_step *(float)i) *
                sinf(angle_step *(float)j);
               (*vertices)[vertex + 1] = radius * cosf(angle_step *(float)i);
               (*vertices)[vertex + 2] = radius * sinf(angle_step *(float)i) *
                cosf(angle_step *(float)j);
            }
            if(normals) {
               (*normals)[vertex + 0] =(*vertices)[vertex + 0] / radius;
               (*normals)[vertex + 1] =(*vertices)[vertex + 1] / radius;
               (*normals)[vertex + 2] =(*vertices)[vertex + 2] / radius;
            }
            if(texcoords) {
                int tex_index =(i *(numslices + 1) + j) * 2;
               (*texcoords)[tex_index + 0] =(float) j /(float) numslices;
               (*texcoords)[tex_index + 1] =(1.0f -(float) i) /(float)(num_parallels - 1);
            }
        }
    }
    // Generate the indices
    if(indices != NULL) {
        GLushort *index_buf =(*indices);
        for(i=0; i<num_parallels; i++) {
            for(j=0; j<numslices; j++) {
                *index_buf++  = i *(numslices + 1) + j;
                *index_buf++ =(i + 1) *(numslices + 1) + j;
                *index_buf++ =(i + 1) *(numslices + 1) +(j + 1);
                
                *index_buf++ = i *(numslices + 1) + j;
                *index_buf++ =(i + 1) *(numslices + 1) +(j + 1);
                *index_buf++ = i *(numslices + 1) +(j + 1);
            }
        }
    }
    
    return num_indices;
}

int ESUTIL_API
es_gen_cube(float scale, GLfloat **vertices, GLfloat **normals,
                GLfloat **texcoords, GLushort **indices)
{
    int i;
    int num_vertices = 24;
    int num_indices = 36;
    
    GLfloat cube_verts[] =
    {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
    };
    
    GLfloat cube_normals[] =
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };
    
    GLfloat cube_tex[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    // Allocate memory for buffers
    if(vertices != NULL) {
        *vertices = malloc(sizeof(GLfloat) * 3 * num_vertices);
        memcpy(*vertices, cube_verts, sizeof(cube_verts));
        for(i=0; i<num_vertices * 3; i++) {
           (*vertices)[i] *= scale;
        }
    }
    
    if(normals != NULL) {
        *normals = malloc(sizeof(GLfloat) * 3 * num_vertices);
        memcpy(*normals, cube_normals, sizeof(cube_normals));
    }
    
    if(texcoords != NULL) {
        *texcoords = malloc(sizeof(GLfloat) * 2 * num_vertices);
        memcpy(*texcoords, cube_tex, sizeof(cube_tex)) ;
    }
    
    // Generate the indices
    if(indices != NULL) {
        GLushort cube_indices[] =
        {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 15, 14,
            12, 14, 13,
            16, 17, 18,
            16, 18, 19,
            20, 23, 22,
            20, 22, 21
        };
        
        *indices = malloc(sizeof(GLushort) * num_indices);
        memcpy(*indices, cube_indices, sizeof(cube_indices));
    }
    
    return num_indices;
}

int ESUTIL_API
es_gen_squaregrid(int size, GLfloat **vertices, GLuint **indices)
{
    int i, j;
    int num_indices =(size - 1) *(size - 1) * 2 * 3;
    // Allocate memory for buffers
    if(vertices != NULL) {
        int num_vertices = size * size;
        float stepSize =(float) size - 1;
        *vertices = malloc(sizeof(GLfloat) * 3 * num_vertices);
        
        for(i = 0; i < size; ++i) { // row
            for(j = 0; j < size; ++j) { // column
               (*vertices) [ 3 *(j + i * size)     ] = i / stepSize;
               (*vertices) [ 3 *(j + i * size) + 1 ] = j / stepSize;
               (*vertices) [ 3 *(j + i * size) + 2 ] = 0.0f;
            }
        }
    }
    
    // Generate the indices
    if(indices != NULL) {
        *indices = malloc(sizeof(GLuint) * num_indices);
        for(i = 0; i < size - 1; ++i) {
            for(j = 0; j < size - 1; ++j) {
                // two triangles per quad
               (*indices) [ 6 *(j + i *(size - 1))     ] = j +(i)   *(size)    ;
               (*indices) [ 6 *(j + i *(size - 1)) + 1 ] = j +(i)   *(size) + 1;
               (*indices) [ 6 *(j + i *(size - 1)) + 2 ] = j +(i + 1) *(size) + 1;
                
               (*indices) [ 6 *(j + i *(size - 1)) + 3 ] = j +(i)   *(size)    ;
               (*indices) [ 6 *(j + i *(size - 1)) + 4 ] = j +(i + 1) *(size) + 1;
               (*indices) [ 6 *(j + i *(size - 1)) + 5 ] = j +(i + 1) *(size)    ;
            }
        }
    }
    
    return num_indices;
}
