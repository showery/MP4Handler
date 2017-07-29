/*******************************************************************************
 *        Module: paomiantv
 *          File: mp4v2helper.cpp
 * Functionality: paomiantv jni stub for PMStoryboard.
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
#include "jnimodulestoryboard.h"
#include "autolog.h"
#include "jnicommon.h"
#include "stdlib.h"

namespace paomiantv
{

TJavaClazzParam *CJNIModuleStoryboard::GetJavaClazzParam()
{
    TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
    JNINativeMethod arrMethods[] =
        {
            {"_init", "(Ljava/lang/String;)Z", (void *)init},
            {"_uninit", "()Z", (void *)uninit},
            {"_setBGM", "(Ljava/lang/String;JJJJ)Z", (void *)setBGM},
            {"_addClip", "(Lcn/paomiantv/mediasdk/PMClip;)Z", (void *)addClip},
            {"_removeClip", "(I)Lcn/paomiantv/mediasdk/PMClip", (void *)removeClip},
            {"_getClip", "(I)Lcn/paomiantv/mediasdk/PMClip;", (void *)getClip},
            {"_swapClip", "(II)Z", (void *)swapClip},
            {"_procsss", "()Z", (void *)procsss},
            {"_cancel", "()Z", (void *)cancel},
        };
    ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
    sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMStoryboard%s", "");
    ptJavaClazzParam->m_ptMethods = (JNINativeMethod *)malloc(sizeof(arrMethods));
    memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
    return ptJavaClazzParam;
}

CJNIModuleStoryboard::CJNIModuleStoryboard(JNIEnv *env, jobject jStoryboard, jclass jcls, jfieldID jfld)
{
    USE_LOG;

    if (env == NULL || jStoryboard == NULL || jcls == NULL)
    {
        LOGE("invalid parameters");
        return;
    }

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return;
    }

    //get methods of the object jstoryboard
    TJavaMemeber atJAVAMemberMethod[] = {
        {STORYBOARD_METHOD_FAILED_NAME, STORYBOARD_METHOD_FAILED_SIG},
        {STORYBOARD_METHOD_PROGRESS_NAME, STORYBOARD_METHOD_PROGRESS_SIG},
        {STORYBOARD_METHOD_SUCCESS_NAME, STORYBOARD_METHOD_SUCCESS_SIG}};
    for (s32 i = 0; i < NELEM(atJAVAMemberMethod); i++)
    {
        m_ajmtd[i] = env->GetMethodID(jcls, atJAVAMemberMethod[i].m_pchName, atJAVAMemberMethod[i].m_pchSigniture);
        if (m_ajmtd[i] == NULL)
        {
            LOGE("java method 'void %s()' is not defined", atJAVAMemberMethod[i].m_pchName);
            break;
        }
    }

    m_jfldNativeAddr = jfld;

    m_jObject = env->NewGlobalRef(jStoryboard);
    m_pStoryboard = new CStoryboard;
    if (m_pStoryboard != NULL)
    {
        LOGD("Storyboard instance allocated: %u", sizeof(CStoryboard));
    }
    // only register valid ones
    m_Registry.Add(this);
}

CJNIModuleStoryboard::~CJNIModuleStoryboard()
{
    USE_LOG;

    if (m_pStoryboard != NULL)
    {
        delete m_pStoryboard;
        m_pStoryboard = NULL;
        LOGD("Storyboard instance freed: %u", sizeof(CStoryboard));
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

CJNIModuleStoryboard *CJNIModuleStoryboard::CreateJniStoryboard(JNIEnv *env, jobject jStoryboard)
{
    USE_LOG;
    CJNIModuleStoryboard *ret = NULL;
    env->PushLocalFrame(10);
    do
    {
        //get class of the object jstoryboard
        jclass jcls = env->GetObjectClass(jStoryboard);
        if (jcls == NULL)
        {
            LOGE("get java class failed");
            break;
        }
        //get field native_address of the object jstoryboard
        jfieldID jfld = env->GetFieldID(jcls, STORYBOARD_FIELD_NATIVE_ADDRESS_NAME, STORYBOARD_FIELD_NATIVE_ADDRESS_SIG);
        if (jfld == NULL)
        {
            LOGE("java field '%s' is not defined", STORYBOARD_FIELD_NATIVE_ADDRESS_NAME);
            break;
        }

        jint nValue = env->GetIntField(jStoryboard, jfld);
        if (nValue != 0 && g_registry.Contains((void *)nValue))
        {
            LOGI("the CJNIModuleStoryboard is already created");
            ret = (CJNIModuleStoryboard *)nValue;
            break;
        }

        CJNIModuleStoryboard *pNew = new CJNIModuleStoryboard(env, jStoryboard, jcls, jfld);
        if (!g_registry.Contains(pNew))
        {
            LOGE("create CJNIModuleStoryboard failed");
            delete pNew;
            ret = NULL;
            break;
        }
        else
        {
            LOGD("CJNIModuleStoryboard instance allocated: %u", sizeof(CJNIModuleStoryboard));
        }

        env->SetIntField(jStoryboard, jfld, (jint)pNew);
        ret = pNew;

        LOGI("create CJNIModuleStoryboard ok");
    } while (0);
    env->PopLocalFrame(NULL);

    return ret;
}

void CJNIModuleStoryboard::DestroyJniStoryboard(CJNIModuleStoryboard *&p)
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

        LOGI("destroy CJNIModuleStoryboard ok");
    } while (0);
    env->PopLocalFrame(NULL);
    p = NULL;
}

CJNIModuleStoryboard *CJNIModuleStoryboard::GetJniStoryboard(JNIEnv *env, jobject jStoryboard)
{
    CJNIModuleStoryboard *ret = NULL;
    env->PushLocalFrame(10);
    do
    {
        if (env == NULL || jStoryboard == NULL)
        {
            LOGE("invalid parameters");
            break;
        }

        jclass jcls = env->GetObjectClass(jStoryboard);
        if (jcls == NULL)
        {
            LOGE("get java class failed");
            break;
        }

        jfieldID jfld = env->GetFieldID(jcls, STORYBOARD_FIELD_NATIVE_ADDRESS_NAME, STORYBOARD_FIELD_NATIVE_ADDRESS_SIG);
        if (jfld == NULL)
        {
            LOGE("java java field '%s' failed", STORYBOARD_FIELD_NATIVE_ADDRESS_NAME);
            break;
        }

        jint nValue = env->GetIntField(jStoryboard, jfld);
        if (nValue == 0 || !m_Registry.Contains((CJNIModuleStoryboard *)nValue))
        {
            //LOGE("get jni storyboard from java object failed");
            //return NULL;
            LOGI("try to get a new CJNIModuleStoryboard");
            ret = CreateJniStoryboard(env, jStoryboard);
        }
        else
        {
            ret = (CJNIModuleStoryboard *)nValue;
        }

    } while (0);

    env->PopLocalFrame(NULL);
    return ret;
}

bool CJNIModuleStoryboard::IsValid(CJNIModuleStoryboard *p)
{
    // checks residency validity
    return m_Registry.Contains(p);
}

jboolean CJNIModuleStoryboard::init(JNIEnv *env, jobject jstoryboard, jstring jdstPath)
{
    USE_LOG;
    CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env, jstoryboard);
    if (pJNIStoryboard == NULL)
    {
        return FALSE;
    }
    s8 achDstPath[MAX_LEN_FILE_PATH] = {0};
    if ((jdstPath == NULL) || (getStringBytes(env, jdstPath, achDstPath, sizeof(achDstPath)) <= 0))
    {
        LOGE("file path is null or char array is not enough!");
        return FALSE;
    }
    return pJNIStoryboard->getCStoryboard()->init(achDstPath);
}

jboolean CJNIModuleStoryboard::uninit(JNIEnv *env, jobject jstoryboard)
{
    USE_LOG;
    CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env, jstoryboard);
    if (pJNIStoryboard == NULL)
    {
        return FALSE;
    }
    return  pJNIStoryboard->getCStoryboard()->uninit();
}

jboolean CJNIModuleStoryboard::setBGM(JNIEnv *env, jobject jstoryboard, jstring jBGMPath, jlong jstartCutTm, jlong jdurationCutTm, jlong jstartTm, jlongjendTm)
{
    USE_LOG;
    CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env, jstoryboard);
    if (pJNIStoryboard == NULL)
    {
        return FALSE;
    }
    s8 achBGMPath[MAX_LEN_FILE_PATH] = {0};
    if ((jBGMPath == NULL) || (getStringBytes(env, jBGMPath, achBGMPath, sizeof(achBGMPath)) <= 0))
    {
        LOGE("file path is null or char array is not enough!");
        return FALSE;
    }
    return pJNIStoryboard->getCStoryboard()->setBGM(achBGMPath, jstartCutTm, jdurationCutTm, jstartTm, jendTm);
}

jboolean CJNIModuleStoryboard::addClip(JNIEnv *env, jobject jstoryboard, jobject jclip)
{
    USE_LOG;
    CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env, jstoryboard);
    if (pJNIStoryboard == NULL)
    {
        return FALSE;
    }
    CJNIModuleClip *pJNIClip = CJNIModuleClip::ExistJniClip(env, jclip);
    if (pJNIClip == NULL)
    {
        return FALSE;
    }
    return pJNIStoryboard->getCStoryboard()->addClip(pJNIClip->getCClip());
}

jobject CJNIModuleEngine::removeClip(JNIEnv *env, jobject jstoryboard, jint) 
{
        USE_LOG;
    CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env, jstoryboard);
    if (pJNIStoryboard == NULL)
    {
        return FALSE;
    }
    CClip clip = pJNIStoryboard->getCStoryboard()->removeClip(index)
    return ;
}

jobject CJNIModuleStoryboard::getClip(JNIEnv *env, jobject jstoryboard, jint) {}

jboolean CJNIModuleStoryboard::swapClip(JNIEnv *env, jobject jstoryboard, jint, jint) {}

jboolean CJNIModuleStoryboard::procsss(JNIEnv *env, jobject jstoryboard) {}

jboolean CJNIModuleStoryboard::cancel(JNIEnv *env, jobject jstoryboard) {}

void CJNIModuleStoryboard::failedCallback(s32 nErr, s8 *pchDescription)
{
    if (!pchDescription || !m_jvm || !m_jCutter)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    jstring jnistr = stringToJString(env, pchDescription);
    env->CallVoidMethod(m_jCutter, m_jmtdFailed, nErr, jnistr);
    env->DeleteLocalRef(jnistr);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}

void CJNIModuleStoryboard::progressCallback(s32 nProgress)
{

    if (!m_jvm || !m_jCutter || !m_jmtdPrgress)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    env->CallVoidMethod(m_jCutter, m_jmtdPrgress, nProgress);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}

void CJNIModuleStoryboard::successCallback()
{
    if (!m_jvm || !m_jCutter || !m_jmtdSuccess)
        return;
    JNIEnv *env = NULL;
    bool isAttacked = false;
    int status = (m_jvm)->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (status != JNI_OK)
    {

        status = m_jvm->AttachCurrentThread(&env, NULL);
        if (status != JNI_OK)
        {
            LOGE("failed to attach current thread");
            return;
        }
        isAttacked = true;
    }

    env->PushLocalFrame(10);
    jstring jnistr = stringToJString(env, m_ptMp4CutterParam->pchDstPath);
    env->CallVoidMethod(m_jCutter, m_jmtdSuccess, jnistr);
    env->DeleteLocalRef(jnistr);
    env->PopLocalFrame(NULL);

    if (isAttacked)
    {
        m_jvm->DetachCurrentThread();
    }
}
}
