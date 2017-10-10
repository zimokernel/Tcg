/*************************************************************************
	> File Name: es_transform.c
	> Author:  TTc
	> Mail: liutianshxkernel@gmail.com
	> Created Time: Mon Oct  9 21:17:48 2017
 ************************************************************************/

#include <string.h>
#include <math.h>
#include "es_common.h"

#define PI 3.1415926535897932384626433832795f

void
es_scale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result->m[0][0] *= sx;
    result->m[0][1] *= sx;
    result->m[0][2] *= sx;
    result->m[0][3] *= sx;
    
    result->m[1][0] *= sy;
    result->m[1][1] *= sy;
    result->m[1][2] *= sy;
    result->m[1][3] *= sy;
    
    result->m[2][0] *= sz;
    result->m[2][1] *= sz;
    result->m[2][2] *= sz;
    result->m[2][3] *= sz;
}

void
es_translate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
    result->m[3][0] +=(result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] +=(result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] +=(result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] +=(result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void
es_rotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat sinAngle, cosAngle;
    GLfloat mag = sqrtf(x * x + y * y + z * z);
    
    sinAngle = sinf(angle * PI / 180.0f);
    cosAngle = cosf(angle * PI / 180.0f);

    if(mag > 0.0f) {
        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLfloat oneMinusCos;
        ESMatrix rotMat;
        
        x /= mag;
        y /= mag;
        z /= mag;
        
        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;
        
        rotMat.m[0][0] =(oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] =(oneMinusCos * xy) - zs;
        rotMat.m[0][2] =(oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0F;
        
        rotMat.m[1][0] =(oneMinusCos * xy) + zs;
        rotMat.m[1][1] =(oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] =(oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0F;
        
        rotMat.m[2][0] =(oneMinusCos * zx) - ys;
        rotMat.m[2][1] =(oneMinusCos * yz) + xs;
        rotMat.m[2][2] =(oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0F;
        
        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;
        
        es_matrix_multiply(result, &rotMat, result);
    }
}

void
es_frustum(ESMatrix *result, float left, float right, float bottom, float top, float nearz, float farz)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farz - nearz;
    ESMatrix    frust;
    
    if((nearz <= 0.0f) ||(farz <= 0.0f) ||
       (deltaX <= 0.0f) ||(deltaY <= 0.0f) ||(deltaZ <= 0.0f)) {
        return;
    }
    frust.m[0][0] = 2.0f * nearz / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;
    
    frust.m[1][1] = 2.0f * nearz / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;
    
    frust.m[2][0] =(right + left) / deltaX;
    frust.m[2][1] =(top + bottom) / deltaY;
    frust.m[2][2] = -(nearz + farz) / deltaZ;
    frust.m[2][3] = -1.0f;
    
    frust.m[3][2] = -2.0f * nearz * farz / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;
    
    es_matrix_multiply(result, &frust, result);
}

void
es_perspective(ESMatrix *result, float fovy, float aspect, float nearz, float farz)
{
    GLfloat frustumW, frustumH;
    
    frustumH = tanf(fovy / 360.0f * PI) * nearz;
    frustumW = frustumH * aspect;
    
    es_frustum(result, -frustumW, frustumW, -frustumH, frustumH, nearz, farz);
}

void
es_ortho(ESMatrix *result, float left, float right, float bottom, float top, float nearz, float farz)
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farz - nearz;
    ESMatrix    ortho;
    
    if((deltaX == 0.0f) ||(deltaY == 0.0f) ||(deltaZ == 0.0f)) {
        return;
    }
    es_matrix_load_identity(&ortho);
    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearz + farz) / deltaZ;
    
    es_matrix_multiply(result, &ortho, result);
}

void
es_matrix_multiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB)
{
    ESMatrix tmp;
    int i;
    for(i=0; i<4; i++) {
        tmp.m[i][0] =  (srcA->m[i][0] * srcB->m[0][0]) +
                       (srcA->m[i][1] * srcB->m[1][0]) +
                       (srcA->m[i][2] * srcB->m[2][0]) +
                       (srcA->m[i][3] * srcB->m[3][0]) ;
        
        tmp.m[i][1] =  (srcA->m[i][0] * srcB->m[0][1]) +
                       (srcA->m[i][1] * srcB->m[1][1]) +
                       (srcA->m[i][2] * srcB->m[2][1]) +
                       (srcA->m[i][3] * srcB->m[3][1]) ;
        
        tmp.m[i][2] =  (srcA->m[i][0] * srcB->m[0][2]) +
                       (srcA->m[i][1] * srcB->m[1][2]) +
                       (srcA->m[i][2] * srcB->m[2][2]) +
                       (srcA->m[i][3] * srcB->m[3][2]) ;
        
        tmp.m[i][3] =  (srcA->m[i][0] * srcB->m[0][3]) +
                       (srcA->m[i][1] * srcB->m[1][3]) +
                       (srcA->m[i][2] * srcB->m[2][3]) +
                       (srcA->m[i][3] * srcB->m[3][3]) ;
    }
    memcpy(result, &tmp, sizeof(ESMatrix));
}

void ESUTIL_API
es_matrix_load_identity(ESMatrix *result)
{
    memset(result, 0x0, sizeof(ESMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void ESUTIL_API
esMatrixLookAt(ESMatrix *result,
                float posX,    float posY,    float posZ,
                float lookAtX, float lookAtY, float lookAtZ,
                float upX,     float upY,     float upZ)
{
    float axisX[3], axisY[3], axisZ[3];
    float length;
    // axisZ = lookAt - pos
    axisZ[0] = lookAtX - posX;
    axisZ[1] = lookAtY - posY;
    axisZ[2] = lookAtZ - posZ;
    // normalize axisZ
    length = sqrtf(axisZ[0] * axisZ[0] + axisZ[1] * axisZ[1] + axisZ[2] * axisZ[2]);
    if(length != 0.0f) {
        axisZ[0] /= length;
        axisZ[1] /= length;
        axisZ[2] /= length;
    }
    // axisX = up X axisZ
    axisX[0] = upY * axisZ[2] - upZ * axisZ[1];
    axisX[1] = upZ * axisZ[0] - upX * axisZ[2];
    axisX[2] = upX * axisZ[1] - upY * axisZ[0];
    // normalize axisX
    length = sqrtf(axisX[0] * axisX[0] + axisX[1] * axisX[1] + axisX[2] * axisX[2]);
    
    if(length != 0.0f) {
        axisX[0] /= length;
        axisX[1] /= length;
        axisX[2] /= length;
    }
    // axisY = axisZ x axisX
    axisY[0] = axisZ[1] * axisX[2] - axisZ[2] * axisX[1];
    axisY[1] = axisZ[2] * axisX[0] - axisZ[0] * axisX[2];
    axisY[2] = axisZ[0] * axisX[1] - axisZ[1] * axisX[0];
    // normalize axisY
    length = sqrtf(axisY[0] * axisY[0] + axisY[1] * axisY[1] + axisY[2] * axisY[2]);
    if(length != 0.0f) {
        axisY[0] /= length;
        axisY[1] /= length;
        axisY[2] /= length;
    }
    memset(result, 0x0, sizeof(ESMatrix));
    
    result->m[0][0] = -axisX[0];
    result->m[0][1] =  axisY[0];
    result->m[0][2] = -axisZ[0];
    
    result->m[1][0] = -axisX[1];
    result->m[1][1] =  axisY[1];
    result->m[1][2] = -axisZ[1];
    
    result->m[2][0] = -axisX[2];
    result->m[2][1] =  axisY[2];
    result->m[2][2] = -axisZ[2];
    
    // translate(-posX, -posY, -posZ)
    result->m[3][0] =  axisX[0] * posX + axisX[1] * posY + axisX[2] * posZ;
    result->m[3][1] = -axisY[0] * posX - axisY[1] * posY - axisY[2] * posZ;
    result->m[3][2] =  axisZ[0] * posX + axisZ[1] * posY + axisZ[2] * posZ;
    result->m[3][3] = 1.0f;
}
