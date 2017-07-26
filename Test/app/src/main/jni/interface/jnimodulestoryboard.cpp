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
#include "common.h"
#include "stdlib.h"

namespace paomiantv
{
CJNIModuleStoryboard::CJNIModuleStoryboard(JNIEnv *env, jobject jStoryboard)
{
    USE_LOG;

    if (env == NULL || jStoryboard == NULL)
    {
        LOGE("invalid parameters");
        return;
    }

    if (env->GetJavaVM(&m_jvm) != 0)
    {
        LOGE("get java vm failed");
        return;
    }
    m_jObject = env->NewGlobalRef(jStoryboard);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);

    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&m_mutex,&attr);
    pthread_mutexattr_destroy(&attr);
    pthread_cond_init(&m_cond, NULL);

    start(env, jStoryboard);

    // only register valid ones
    m_Registry.Add(this);

    env->PushLocalFrame(10);

    jclass jcls = env->GetObjectClass(jStoryboard);
    if (jcls == NULL)
    {
        LOGE("get java class failed");
        return;
    }
    m_jObject = env->NewGlobalRef(jStoryboard);
    m_jmtdFailed = env->GetMethodID(jcls, CUTTER_METHOD_FAILED_NAME, CUTTER_METHOD_FAILED_SIG);
    if (m_jmtdFailed == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_FAILED_NAME);
        return;
    }
    m_jmtdPrgress = env->GetMethodID(jcls, CUTTER_METHOD_PROGRESS_NAME, CUTTER_METHOD_PROGRESS_SIG);
    if (m_jmtdPrgress == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_PROGRESS_NAME);
        return;
    }
    m_jmtdSuccess = env->GetMethodID(jcls, CUTTER_METHOD_SUCCESS_NAME, CUTTER_METHOD_SUCCESS_SIG);
    if (m_jmtdSuccess == NULL)
    {
        LOGE("java method 'void %s()' is not defined", CUTTER_METHOD_SUCCESS_NAME);
        return;
    }
    env->PopLocalFrame(NULL);
}

CJNIModuleStoryboard::~CJNIModuleStoryboard()
{
        USE_LOG;

    stop();

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);

    if (m_pRenderer != NULL)
    {
        m_pRenderer->Destroy();
        delete m_pRenderer;
        m_pRenderer = NULL;
        LOGD("renderer instance freed: %u", sizeof(CRenderer));
    }

    // be sure unregister before killing
    g_registry.Remove(this);


	if(m_tVidFrame.pbyData != NULL)
	{
    	free(m_tVidFrame.pbyData);
    	m_tVidFrame.pbyData = NULL;
	}
}

TJavaClazzParam *CJNIModuleStoryboard::GetJavaClazzParam()
{
    TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
    JNINativeMethod arrMethods[] =
        {
            {"_init", "(Ljava/lang/String;)Z", (void *)init},
            {"_uninit", "()Z", (void *)uninit},
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


bool CJNIModuleStoryboard::IsValid(CJniRenderer *p)
{
    // checks residency validity
    return m_Registry.Contains(p);
}

CJniRenderer * CJniRenderer::CreateJniRenderer(JNIEnv *env, jobject jRenderer)
{
    USE_LOG;
	CJniRenderer *ret = NULL;
	env->PushLocalFrame(10);
	do
	{
	    jclass jcls = env->GetObjectClass(jRenderer);
	    if (jcls == NULL)
	    {
	        LOGE("get java class failed");
	        break;
	    }

	    jfieldID jfld = env->GetFieldID(jcls, FIELD_NAME, "I");
	    if (jfld == NULL)
	    {
	        LOGE("java field '%s' is not defined", FIELD_NAME);
	        break;
	    }

	    jmethodID jmtd = env->GetMethodID(jcls, METHOD_NAME, METHOD_SIG);
	    if (jmtd == NULL)
	    {
	        LOGE("java method 'void %s()' is not defined", METHOD_NAME);
	        break;
	    }

	    jint nValue = env->GetIntField(jRenderer, jfld);
	    if (nValue != 0 && m_Registry.Contains((void*)nValue))
	    {
	        LOGI("the jni renderer is already created");
	        ret = (CJniRenderer*)nValue;
			break;
	    }

	    CJniRenderer * pNew = new CJniRenderer(env, jRenderer, jcls, jfld, jmtd);
	    if (!m_Registry.Contains(pNew))
	    {
	        LOGE("create jni renderer failed");
	        delete pNew;
	        ret = NULL;
			break;
	    }
	    else
	    {
	        LOGD("jnirenderer instance allocated: %u", sizeof(CJniRenderer));
	    }

	    env->SetIntField(jRenderer, jfld, (jint)pNew);
		ret = pNew;

	    LOGI("create jni renderer ok");
	}while(0);
	env->PopLocalFrame(NULL);

    return ret;
}

void CJniRenderer::DestroyJniRenderer(CJniRenderer* &p)
{
    USE_LOG;

    if (p == NULL || !m_Registry.Contains(p))
    {
        LOGE("invalid parameters");
        return;
    }

    JNIEnv *env = NULL;
    if (p->m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        LOGE("get JNIEnv failed");
        return;
    }
	env->PushLocalFrame(10);
	do
	{
	    jint nValue = env->GetIntField(p->m_jRenderer, p->m_jfld);
	    if (nValue != (jint)p)
	    {
	        LOGE("java field is changed improperly!");
	    }

	    p->m_pRenderer->Destroy();

	    env->SetIntField(p->m_jRenderer, p->m_jfld, 0);
	    delete p;
	    LOGD("jnirenderer instance freed: %u", sizeof(CJniRenderer));

	    LOGI("destroy jni renderer ok");
	}while(0);
	env->PopLocalFrame(NULL);
    p = NULL;
}

CJniRenderer * CJniRenderer::GetJniRenderer(JNIEnv *env, jobject jRenderer)
{
	CJniRenderer* ret = NULL;
	env->PushLocalFrame(10);
	do
	{
	    if (env == NULL || jRenderer == NULL)
	    {
	        LOGE("invalid parameters");
			break;
	    }

	    jclass jcls = env->GetObjectClass(jRenderer);
	    if (jcls == NULL)
	    {
	        LOGE("get java class failed");
	        break;
	    }

	    jfieldID jfld = env->GetFieldID(jcls, FIELD_NAME, "I");
	    if (jfld == NULL)
	    {
	        LOGE("java java field '%s' failed", FIELD_NAME);
	        break;
	    }

	    jint nValue = env->GetIntField(jRenderer, jfld);
	    if (nValue == 0
	            || !m_Registry.Contains((CJniRenderer*)nValue))
	    {
	        //LOGE("get jni renderer from java object failed");
	        //return NULL;
	        LOGI("try to get a new jni renderer");
	        ret = CreateJniRenderer(env, jRenderer);
	    }
		else
		{
			ret = (CJniRenderer*)nValue;
		}

	}while(0);

	env->PopLocalFrame(NULL);
    return ret;
}



jboolean CJNIModuleStoryboard::init(JNIEnv *env, jobject jstoryboard, jstring jdstPath)
{
    s8 achDstPath[512] = {0};
    getStringBytes(env, dst, achDstPath, 512);
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

jboolean CJNIModuleStoryboard::uninit(JNIEnv *, jobject) {}

jboolean CJNIModuleStoryboard::addClip(JNIEnv *, jobject, jobject) {}

jobject CJNIModuleEngine::removeClip(JNIEnv *, jobject, jint) {}

jobject CJNIModuleStoryboard::getClip(JNIEnv *, jobject, jint) {}

jboolean CJNIModuleStoryboard::swapClip(JNIEnv *, jobject, jint, jint) {}

jboolean CJNIModuleStoryboard::procsss(JNIEnv *, jobject) {}

jboolean CJNIModuleStoryboard::cancel(JNIEnv *, jobject) {}
}

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
