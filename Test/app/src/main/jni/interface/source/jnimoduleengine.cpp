/*******************************************************************************
 *        Module: interface
 *          File: jnimoduleengine.cpp
 * Functionality: engine jni.
 *       Related: mediasdk
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
#include "jnimodulemanager.h"
#include "jnimoduleengine.h"
#include "autolog.h"
#include "stdlib.h"

namespace paomiantv {
    CLock CJNIModuleEngine::m_SingleInstanceLock;

    TJavaClazzParam *CJNIModuleEngine::GetJavaClazzParam() {
        TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
        JNINativeMethod arrMethods[] =
                {
                        {"_init",          "(I)Z",                                    (void *) jni_init},
                        {"_uninit",        "()V",                                     (void *) jni_uninit},
                        {"_setDataSource", "(Lcn/paomiantv/mediasdk/PMStoryboard;)V", (void *) jni_setDataSource},
                        {"_start",         "(Z)V",                                    (void *) jni_start},
                        {"_seekTo",        "(I)V",                                    (void *) jni_seekTo},
                        {"_pause",         "()V",                                     (void *) jni_pause},
                        {"_resume",        "()V",                                     (void *) jni_resume},
                        {"_cancel",        "()V",                                     (void *) jni_cancel}};
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMEngine%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }

    CJNIModuleEngine::CJNIModuleEngine(JNIEnv *env, jobject jEngine, jfieldID jfld) {
        USE_LOG;

        if (env == NULL || jEngine == NULL || jfld == NULL) {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0) {
            LOGE("get java vm failed");
            return;
        }

        m_jfldNativeAddr = jfld;

        m_jObject = env->NewGlobalRef(jEngine);
        if (m_jObject == NULL) {
            LOGE("new global reference failed ,jvm stack table is full or unknown reason");
        }
        m_pEngine = new CEngine;
        if (m_pEngine != NULL) {
            LOGD("Engine instance allocated: %u", sizeof(CEngine));
            // only register valid ones
            CJNIModuleManager::getInstance()->add(this);
        } else {
            LOGE("new filter failed ,memory is not enough!");
        }
    }

    CJNIModuleEngine::~CJNIModuleEngine() {
        USE_LOG;

        if (m_pEngine != NULL) {
            delete m_pEngine;
            m_pEngine = NULL;
            LOGD("Engine instance freed: %u", sizeof(CEngine));
        }
        JNIEnv *env = NULL;
        if (m_jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("get JNIEnv failed");
            return;
        }

        if (m_jObject != NULL) {
            env->DeleteGlobalRef(m_jObject);
            m_jObject = NULL;
            m_jfldNativeAddr = NULL;
            m_jvm = NULL;
        }

        // be sure unregister before killing
        CJNIModuleManager::getInstance()->remove(this);
    }

    CJNIModuleEngine *CJNIModuleEngine::CreateJniEngine(JNIEnv *env, jobject jEngine) {
        USE_LOG;
        CJNIModuleEngine *ret = NULL;
        env->PushLocalFrame(10);
        do {
            //get class of the object jEngine
            jclass jcls = env->GetObjectClass(jEngine);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }
            //get field native_address of the object jEngine
            jfieldID jfld = env->GetFieldID(jcls, ENGINE_FIELD_NATIVE_ADDRESS_NAME,
                                            ENGINE_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java field '%s' is not defined", ENGINE_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jEngine, jfld);
            if (nValue != 0 && CJNIModuleManager::getInstance()->contains((CJNIModule *) nValue)) {
                LOGI("the CJNIModuleEngine is already created");
                ret = (CJNIModuleEngine *) nValue;
                break;
            }

            CJNIModuleEngine *pNew = new CJNIModuleEngine(env, jEngine, jfld);
            if (!CJNIModuleManager::getInstance()->contains(pNew)) {
                LOGE("create CJNIModuleEngine failed");
                delete pNew;
                ret = NULL;
                break;
            } else {
                LOGD("CJNIModuleEngine instance allocated: %u", sizeof(CJNIModuleEngine));
            }

            env->SetIntField(jEngine, jfld, (jint) pNew);
            ret = pNew;

            LOGI("create CJNIModuleEngine ok");
        } while (0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleEngine::DestroyJniEngine(CJNIModuleEngine *&p) {
        USE_LOG;

        if (p == NULL || !CJNIModuleManager::getInstance()->contains(p)) {
            LOGE("invalid parameters");
            return;
        }

        JNIEnv *env = NULL;
        if (p->m_jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("get JNIEnv failed");
            return;
        }
        env->PushLocalFrame(10);
        do {
            jint nValue = env->GetIntField(p->m_jObject, p->m_jfldNativeAddr);
            if (nValue != (jint) p) {
                LOGE("java field is changed improperly!");
            }

            env->SetIntField(p->m_jObject, p->m_jfldNativeAddr, 0);
            delete p;

            LOGI("destroy CJNIModuleEngine ok");
        } while (0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleEngine *CJNIModuleEngine::GetJniEngine(JNIEnv *env, jobject jEngine) {
        CJNIModuleEngine *ret = NULL;
        env->PushLocalFrame(10);
        do {
            if (env == NULL || jEngine == NULL) {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jEngine);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, ENGINE_FIELD_NATIVE_ADDRESS_NAME,
                                            ENGINE_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java java field '%s' failed", ENGINE_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jEngine, jfld);
            if (nValue == 0 ||
                !CJNIModuleManager::getInstance()->contains((CJNIModuleEngine *) nValue)) {
                //LOGE("get jni Engine from java object failed");
                //return NULL;
                m_SingleInstanceLock.lock();
                if (!env->GetIntField(jEngine, jfld) ||
                    !CJNIModuleManager::getInstance()->contains((CJNIModuleEngine *) nValue)) {
                    LOGI("try to get a new CJNIModuleEngine");
                    ret = CreateJniEngine(env, jEngine);
                }
                m_SingleInstanceLock.unlock();
            } else {
                ret = (CJNIModuleEngine *) nValue;
            }

        } while (0);

        env->PopLocalFrame(NULL);
        return ret;
    }

    bool CJNIModuleEngine::IsValid(CJNIModuleEngine *p) {
        // checks residency validity
        return CJNIModuleManager::getInstance()->contains(p);
    }

    jboolean CJNIModuleEngine::jni_init(JNIEnv *env, jobject jengine, jint jversion) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return FALSE;
        }

        return pJNIEngine->getEngine()->init(jversion);
    }

    void CJNIModuleEngine::jni_uninit(JNIEnv *env, jobject jengine) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return;
        }
        pJNIEngine->getEngine()->uninit();
        CJNIModuleEngine::DestroyJniEngine(pJNIEngine);
    }

    void CJNIModuleEngine::jni_setDataSource(JNIEnv *env, jobject jengine, jobject jstoryboard) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return;
        }

        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL || pJNIStoryboard->getCStoryboard() == NULL) {
            return;
        }

        pJNIEngine->getEngine()->setDataSource(pJNIStoryboard->getCStoryboard());
    }

    void CJNIModuleEngine::jni_seekTo(JNIEnv *env, jobject jengine, jint jclipIndex) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            LOGE("start failed, jniengine or engine is NULL");
            return;
        }

        pJNIEngine->getEngine()->seekTo(jclipIndex);
    }

    void CJNIModuleEngine::jni_start(JNIEnv *env, jobject jengine, jboolean jisSave) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            LOGE("start failed, jniengine or engine is NULL");
            return;
        }

        pJNIEngine->getEngine()->start(jisSave);
    }

    void CJNIModuleEngine::jni_pause(JNIEnv *env, jobject jengine) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return;
        }
        pJNIEngine->getEngine()->pause();
    }

    void CJNIModuleEngine::jni_resume(JNIEnv *env, jobject jengine) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return;
        }

        pJNIEngine->getEngine()->resume();
    }

    void CJNIModuleEngine::jni_cancel(JNIEnv *env, jobject jengine) {
        USE_LOG;
        CJNIModuleEngine *pJNIEngine = CJNIModuleEngine::GetJniEngine(env, jengine);
        if (pJNIEngine == NULL || pJNIEngine->getEngine() == NULL) {
            return;
        }
        pJNIEngine->getEngine()->cancel();
    }
}
