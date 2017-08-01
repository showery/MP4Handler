/*******************************************************************************
 *        Module: renderbackground
 *          File: renderbackground.h
 * Functionality: draw a black background onto opengl context.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2014-01-19  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_RENDERERBACKGROUND_H_
#define _PAOMIANTV_RENDERERBACKGROUND_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "typedef.h"

namespace paomiantv {


class CRenderBackground
{
public:
    CRenderBackground();
    ~CRenderBackground();

    bool init();
    void destroy();
    void draw();

private:
    GLuint m_glProgram;
    GLuint m_glVertexShader;
    GLuint m_glFragmentShader;
    GLint  m_glAttributes;
    GLuint m_glVertexBuffer;
    GLuint m_glIndexBuffer;
};


} // namespace paomiantv


#endif // _PAOMIANTV_RENDERERBACKGROUND_H_