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
#include "jnicommon.h"
#include "stdlib.h"
#include "jnimoduleclip.h"

namespace paomiantv
{

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


CJNIModuleClip::CJNIModuleClip(JNIEnv *env, jobject jClip, jclass jcls, jfieldID jfld)
{
    USE_LOG;

    if (env == NULL || jClip == NULL || jcls == NULL)
    {
        LOGE("invalid parameters");
        return;
    }

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return;
    }

    m_jfldNativeAddr = jfld;

    m_jObject = env->NewGlobalRef(jClip);
    m_pClip = new CClip;
    if (m_pClip != NULL)
    {
        LOGD("Clip instance allocated: %u", sizeof(CClip));
    }
    // only register valid ones
    m_Registry.Add(this);
}

CJNIModuleClip::~CJNIModuleClip()
{
    USE_LOG;

    if (m_pClip != NULL)
    {
        delete m_pClip;
        m_pClip = NULL;
        LOGD("Clip instance freed: %u", sizeof(CClip));
    }
    JNIEnv *env = NULL;
    if (m_jvm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }

    if (m_jObject != NULL)
    {
        env->DeleteGlobalRef(m_jObject);
        m_jObject = NULL;
        m_jfldNativeAddr = NULL;
        memset(m_ajmtd, 0, sizeof(m_ajmtd));
    }

    // be sure unregister before killing
    g_registry.Remove(this);
}

CJNIModuleClip *CJNIModuleClip::CreateJniClip(JNIEnv *env, jobject jClip)
{
    USE_LOG;
    CJNIModuleClip *ret = NULL;
    env->PushLocalFrame(10);
    do
    {
        //get class of the object jClip
        jclass jcls = env->GetObjectClass(jClip);
        if (jcls == NULL)
        {
            LOGE("get java class failed");
            break;
        }
        //get field native_address of the object jClip
        jfieldID jfld = env->GetFieldID(jcls, CLIP_FIELD_NATIVE_ADDRESS_NAME, CLIP_FIELD_NATIVE_ADDRESS_SIG);
        if (jfld == NULL)
        {
            LOGE("java field '%s' is not defined", CLIP_FIELD_NATIVE_ADDRESS_NAME);
            break;
        }

        jint nValue = env->GetIntField(jClip, jfld);
        if (nValue != 0 && g_registry.Contains((void *)nValue))
        {
            LOGI("the CJNIModuleClip is already created");
            ret = (CJNIModuleClip *)nValue;
            break;
        }

        CJNIModuleClip *pNew = new CJNIModuleClip(env, jClip, jcls, jfld);
        if (!g_registry.Contains(pNew))
        {
            LOGE("create CJNIModuleClip failed");
            delete pNew;
            ret = NULL;
            break;
        }
        else
        {
            LOGD("CJNIModuleClip instance allocated: %u", sizeof(CJNIModuleClip));
        }

        env->SetIntField(jClip, jfld, (jint)pNew);
        ret = pNew;

        LOGI("create CJNIModuleClip ok");
    } while (0);
    env->PopLocalFrame(NULL);

    return ret;
}

void CJNIModuleClip::DestroyJniClip(CJNIModuleClip *&p)
{
    USE_LOG;

    if (p == NULL || !m_Registry.Contains(p))
    {
        LOGE("invalid parameters");
        return;
    }

    JNIEnv *env = NULL;
    if (p->m_jvm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }
    env->PushLocalFrame(10);
    do
    {
        jint nValue = env->GetIntField(p->m_jObject, p->m_jfldNativeAddr);
        if (nValue != (jint)p)
        {
            LOGE("java field is changed improperly!");
        }

        env->SetIntField(p->m_jObject, p->m_jfldNativeAddr, 0);
        delete p;

        LOGI("destroy CJNIModuleClip ok");
    } while (0);
    env->PopLocalFrame(NULL);
    p = NULL;
}

CJNIModuleClip *CJNIModuleClip::GetJniClip(JNIEnv *env, jobject jClip)
{
    CJNIModuleClip *ret = NULL;
    env->PushLocalFrame(10);
    do
    {
        if (env == NULL || jClip == NULL)
        {
            LOGE("invalid parameters");
            break;
        }

        jclass jcls = env->GetObjectClass(jClip);
        if (jcls == NULL)
        {
            LOGE("get java class failed");
            break;
        }

        jfieldID jfld = env->GetFieldID(jcls, CLIP_FIELD_NATIVE_ADDRESS_NAME, CLIP_FIELD_NATIVE_ADDRESS_SIG);
        if (jfld == NULL)
        {
            LOGE("java java field '%s' failed", CLIP_FIELD_NATIVE_ADDRESS_NAME);
            break;
        }

        jint nValue = env->GetIntField(jClip, jfld);
        if (nValue == 0 || !m_Registry.Contains((CJNIModuleClip *)nValue))
        {
            //LOGE("get jni Clip from java object failed");
            //return NULL;
            LOGI("try to get a new CJNIModuleClip");
            ret = CreateJniClip(env, jClip);
        }
        else
        {
            ret = (CJNIModuleClip *)nValue;
        }

    } while (0);

    env->PopLocalFrame(NULL);
    return ret;
}


CJNIModuleClip *CJNIModuleClip::ExistJniClip(JNIEnv *env, jobject jClip)
{
    CJNIModuleClip *ret = NULL;
    env->PushLocalFrame(10);
    do
    {
        if (env == NULL || jClip == NULL)
        {
            LOGE("invalid parameters");
            break;
        }

        jclass jcls = env->GetObjectClass(jClip);
        if (jcls == NULL)
        {
            LOGE("get java class failed");
            break;
        }

        jfieldID jfld = env->GetFieldID(jcls, CLIP_FIELD_NATIVE_ADDRESS_NAME, CLIP_FIELD_NATIVE_ADDRESS_SIG);
        if (jfld == NULL)
        {
            LOGE("java java field '%s' failed", CLIP_FIELD_NATIVE_ADDRESS_NAME);
            break;
        }

        jint nValue = env->GetIntField(jClip, jfld);
        if (nValue == 0 || !m_Registry.Contains((CJNIModuleClip *)nValue))
        {
            LOGE("get jni Clip from java object failed");
            break;
            // LOGI("try to get a new CJNIModuleClip");
            // ret = CreateJniClip(env, jClip);
        }
        else
        {
            ret = (CJNIModuleClip *)nValue;
        }

    } while (0);

    env->PopLocalFrame(NULL);
    return ret;
}

bool CJNIModuleClip::IsValid(CJNIModuleClip *p)
{
    // checks residency validity
    return m_Registry.Contains(p);
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
