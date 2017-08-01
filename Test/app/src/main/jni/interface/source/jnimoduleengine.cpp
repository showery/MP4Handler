/*******************************************************************************
 *        Module: paomiantv
 *          File: mp4v2helper.cpp
 * Functionality: paomiantv jni stub for PMEngine.
 *       Related: mp4v2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2013 360ANTS, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#include "jnimoduleengine.h"
#include "autolog.h"
#include "stdlib.h"

namespace paomiantv
{

CJNIModuleEngine::CJNIModuleEngine()
{
}

CJNIModuleEngine::~CJNIModuleEngine()
{
}

TJavaClazzParam *CJNIModuleEngine::GetJavaClazzParam()
{
    TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
    JNINativeMethod arrMethods[] =
        {
            {"_init", "()V", (void *)init},
            {"_uninit", "()V", (void *)uninit}};
    ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
    sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMEngine%s", "");
    ptJavaClazzParam->m_ptMethods = (JNINativeMethod *)malloc(sizeof(arrMethods));
    memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
    return ptJavaClazzParam;
}

void CJNIModuleEngine::init(JNIEnv *env, jobject jengine)
{
    MP4SetLogCallback(log_cb);
}

void CJNIModuleEngine::uninit(JNIEnv *env, jobject jengine)
{
    MP4SetLogCallback(NULL);
}

void CJNIModuleEngine::log_cb(MP4LogLevel loglevel, const s8 *fmt, va_list ap)
{
    switch (loglevel)
    {
    case MP4_LOG_NONE:
        break;
    case MP4_LOG_ERROR:
        VLOGE(fmt, ap);
        break;
    case MP4_LOG_WARNING:
        VLOGW(fmt, ap);
        break;
    case MP4_LOG_INFO:
        VLOGI(fmt, ap);
        break;
    default:
        VLOGI(fmt, ap);
        break;
    }
}
}
