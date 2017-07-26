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
#include "autolog.h"
#include "common.h"
#include "stdlib.h"
#include "jnimoduleclip.h"

namespace paomiantv
{

CJNIModuleClip::CJNIModuleClip()
{
}

CJNIModuleClip::~CJNIModuleClip()
{
}

TJavaClazzParam *CJNIModuleClip::GetJavaClazzParam()
{
    TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
    JNINativeMethod arrMethods[] =
        {
            {"_init", "(Ljava/lang/String;JJ)Z", (void *)init},
            {"_uninit", "()Z", (void *)uninit},
            {"_getSrc", "()Ljava/lang/String;", (void *)getSrc},
            {"_setSrc", "(Ljava/lang/String;)V", (void *)setSrc},
            {"_getStart", "()J", (void *)getStart},
            {"_setStart", "(J)V", (void *)setStart},
            {"_getDuration", "()J", (void *)getDuration},
            {"_setDuration", "(J)V", (void *)setDuration},
            {"_getFilter", "(I)Ljava/lang/String;", (void *)getFilter},
            {"_addFilter", "(Ljava/lang/String;)Z", (void *)addFilter},
            {"_removeFilter", "(Ljava/lang/String;)Z", (void *)removeFilter},
            {"_getFilterCount", "()I", (void *)getFilterCount},
            {"_addTransition", "(Ljava/lang/String;)Z", (void *)addTransition},
            {"_getTransition", "(I)Ljava/lang/String;", (void *)getTransition},
            {"_removeTransition", "(Ljava/lang/String;)Z", (void *)removeTransition},
            {"_getTransitionCount", "(Ljava/lang/String;)Z", (void *)getTransitionCount}};
    ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
    sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMClip%s", "");
    ptJavaClazzParam->m_ptMethods = (JNINativeMethod *)malloc(sizeof(arrMethods));
    memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
    return ptJavaClazzParam;
}

jboolean CJNIModuleClip::jni_clipMp4(JNIEnv *env, jobject jclip, jstring src, jstring dst, jlong start, jlong duration)
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

jboolean CJNIModuleClip::jni_joinMp4(JNIEnv *env, jobject jjoin, jstring src, jstring dst)
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

jboolean CJNIModuleClip::init(JNIEnv *env, jobject jclip, jstring jsrc, jlong jstart, jlong jduration) {}

jboolean CJNIModuleClip::uninit(JNIEnv *env, jobject jclip) {}

jstring CJNIModuleClip::getSrc(JNIEnv *env, jobject jclip) {}

void CJNIModuleClip::setSrc(JNIEnv *env, jobject jclip, jstring jsrc) {}

jlong CJNIModuleClip::getStart(JNIEnv *env, jobject jclip) {}

void CJNIModuleClip::setStart(JNIEnv *env, jobject jclip, jlong jstart) {}

jlong CJNIModuleClip::getDuration(JNIEnv *env, jobject jclip) {}

void CJNIModuleClip::setDuration(JNIEnv *env, jobject jclip, jlong jduration) {}

jstring CJNIModuleClip::getFilter(JNIEnv *env, jobject jclip, jint jindex) {}

jboolean CJNIModuleClip::addFilter(JNIEnv *env, jobject jclip, jstring jfilter) {}

jboolean CJNIModuleClip::removeFilter(JNIEnv *env, jobject jclip, jstring jfilter) {}

jint CJNIModuleClip::getFilterCount(JNIEnv *env, jobject jclip) {}

jboolean CJNIModuleClip::addTransition(JNIEnv *env, jobject jclip, jstring jtransition) {}

jstring CJNIModuleClip::getTransition(JNIEnv *env, jobject jclip, jint jindex) {}

jboolean CJNIModuleClip::removeTransition(JNIEnv *env, jobject jclip, jstring) {}

jint CJNIModuleClip::getTransitionCount(JNIEnv *env, jobject jclip) {}
