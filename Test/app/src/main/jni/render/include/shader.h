/*******************************************************************************
 *        Module: render
 *          File: shader.h
 * Functionality: tools for opengl shader programs.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2014-01-19  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_SHADER_H_
#define _PAOMIANTV_SHADER_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace paomiantv {

GLuint LoadShader(GLenum type, const char *shaderSrc);


} // namespace paomiantv

#endif // _PAOMIANTV_SHADER_H_