/*******************************************************************************
 *        Module: paomiantv
 *          File: mp4v2helper.cpp
 * Functionality: paomiantv jni stub for mp4v2.
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
#include "mp4v2module.h"
#include "autolog.h"
#include "common.h"
#include "stdlib.h"
#include "mp4clip.h"

namespace paomiantv
{
void log_cb(MP4LogLevel loglevel, const char *fmt, va_list ap)
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

CMp4v2Module::CMp4v2Module()
{
    m_nCount = 1;
    m_pbyClazzName = "cn/paomiantv/mediasdk/Mp4Clip";
    m_pMethods = (JNINativeMethod *)malloc(sizeof(JNINativeMethod) * m_nCount);
    m_pMethods[0].name = "clipMp4";
    m_pMethods[0].signature = "(Ljava/lang/String;Ljava/lang/String;JJ)Z";
    m_pMethods[0].fnPtr = (void *)jni_clipMp4;
    MP4SetLogCallback(log_cb);
}

CMp4v2Module::~CMp4v2Module()
{
    free(m_pMethods);
}

jboolean CMp4v2Module::jni_clipMp4(JNIEnv *env, jobject jclip, jstring src, jstring dst, jlong start, jlong duration)
{
    s8 achSrcPath[512] = {0};
    s8 achDstPath[512] = {0};
    getStringBytes(env, src, achSrcPath, 512);
    getStringBytes(env, dst, achDstPath, 512);

    LOGV("clip: src,dst,start,duration = %s,%s,%lld,%lld,", achSrcPath, achDstPath, start, duration);
    TMp4CutterParam *param = (TMp4CutterParam *)malloc(sizeof(TMp4CutterParam));
    param->pchSrcPath = achSrcPath;
    param->pchDstPath = achDstPath;
    param->sllDuration = duration;
    param->sllStart = start;
    CMp4Clip *clip = new CMp4Clip(env, jclip, param);
    clip->clip();
    delete clip;
    free(param);
    return TRUE;
}

jboolean CMp4v2Module::jni_joinMp4(JNIEnv *env, jobject jjoin, jstring src, jstring dst)
{
    s8 achSrcPath[512] = {0};
    s8 achDstPath[512] = {0};
    getStringBytes(env, src, achSrcPath, 512);
    getStringBytes(env, dst, achDstPath, 512);

    LOGV("join: src,dst = %s,%s", achSrcPath, achDstPath, start, duration);
    TMp4CutterParam *param = (TMp4CutterParam *)malloc(sizeof(TMp4CutterParam));
    param->pchSrcPath = achSrcPath;
    param->pchDstPath = achDstPath;
    param->sllDuration = duration;
    param->sllStart = start;
    CMp4Join *join = new CMp4Join(env, jjoin, param);
    join->join();
    delete join;
    free(param);
    return TRUE;
}

}
