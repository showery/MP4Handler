/*******************************************************************************
 *        Module: shader
 *          File: shader.cpp
 * Functionality: tools for opengl shader programs.
 *       Related: GLES2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 fotopalce, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/
#include <stdlib.h>

#include "autolog.h"
#include "shader.h"
#include "version.h"

namespace paomiantv {


GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    if (shader == 0)
    {
        LOGE("create shader failed");
        return 0;
    }

    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOGE("%s", infoLog);
            delete [] infoLog;
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


}
