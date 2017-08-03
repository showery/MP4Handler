/*******************************************************************************
 *        Module: paomiantv
 *          File: jnimoduletransition.cpp
 * Functionality: transition jni.
 *       Related: mediasdk
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-31  v1.0        huangxuefeng  created
 ******************************************************************************/


#include "constant.h"
#include "jnicommon.h"
#include "jnimodulemanager.h"
#include "jnimoduletransition.h"

namespace paomiantv {

    TJavaClazzParam *CJNIModuleTransition::GetJavaClazzParam() {
        TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
        JNINativeMethod arrMethods[] =
                {
                        {"_init",        "(Ljava/lang/String;JJ)Z", (void *) jni_init},
                        {"_uninit",      "()Z",                     (void *) jni_uninit},
                        {"_getSrc",      "()Ljava/lang/String;",    (void *) jni_getSrc},
                        {"_setSrc",      "(Ljava/lang/String;)V",   (void *) jni_setSrc},
                        {"_getStart",    "()J",                     (void *) jni_getStart},
                        {"_setStart",    "(J)V",                    (void *) jni_setStart},
                        {"_getDuration", "()J",                     (void *) jni_getDuration},
                        {"_setDuration", "(J)V",                    (void *) jni_setDuration}
                };
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMTransition%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }


    CJNIModuleTransition::CJNIModuleTransition(JNIEnv *env, jobject jTransition, jfieldID jfld) {
        USE_LOG;

        if (env == NULL || jTransition == NULL || jfld == NULL) {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0) {
            LOGE("get java vm failed");
            return;
        }

        m_jfldNativeAddr = jfld;

        m_jObject = env->NewGlobalRef(jTransition);
        if (m_jObject == NULL) {
            LOGE("new global reference failed ,jvm stack table is full or unknown reason");
        }
        m_pTransition = new CTransition;
        if (m_pTransition != NULL) {
            LOGD("Transition instance allocated: %u", sizeof(CTransition));
            // only register valid ones
            CJNIModuleManager::getInstance()->add(this);
        } else {
            LOGE("new Transition failed ,memory is not enough!");
        }

    }

    CJNIModuleTransition::~CJNIModuleTransition() {
        USE_LOG;

        if (m_pTransition != NULL) {
            delete m_pTransition;
            m_pTransition = NULL;
            LOGD("Transition instance freed: %u", sizeof(CTransition));
        }
        JNIEnv *env = NULL;
        if (m_jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
            LOGE("get JNIEnv failed");
            return;
        }

        if (m_jObject != NULL) {
            env->DeleteGlobalRef(m_jObject);
            m_jObject = NULL;

        }
        m_jfldNativeAddr = NULL;
        m_jvm=NULL;

        // be sure unregister before killing
        CJNIModuleManager::getInstance()->remove(this);
    }

    CJNIModuleTransition *CJNIModuleTransition::CreateJniTransition(JNIEnv *env, jobject jTransition) {
        USE_LOG;
        CJNIModuleTransition *ret = NULL;
        env->PushLocalFrame(10);
        do {
            //get class of the object jTransition
            jclass jcls = env->GetObjectClass(jTransition);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }
            //get field native_address of the object jTransition
            jfieldID jfld = env->GetFieldID(jcls, TRANSITION_FIELD_NATIVE_ADDRESS_NAME,
                                            TRANSITION_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java field '%s' is not defined", TRANSITION_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jTransition, jfld);
            if (nValue != 0 && CJNIModuleManager::getInstance()->contains((CJNIModule *) nValue)) {
                LOGI("the CJNIModuleTransition is already created");
                ret = (CJNIModuleTransition *) nValue;
                break;
            }

            CJNIModuleTransition *pNew = new CJNIModuleTransition(env, jTransition, jfld);
            if (!CJNIModuleManager::getInstance()->contains(pNew)) {
                LOGE("create CJNIModuleTransition failed");
                delete pNew;
                ret = NULL;
                break;
            } else {
                LOGD("CJNIModuleTransition instance allocated: %u", sizeof(CJNIModuleTransition));
            }

            env->SetIntField(jTransition, jfld, (jint) pNew);
            ret = pNew;

            LOGI("create CJNIModuleTransition ok");
        } while (0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleTransition::DestroyJniTransition(CJNIModuleTransition *&p) {
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

            LOGI("destroy CJNIModuleTransition ok");
        } while (0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleTransition *CJNIModuleTransition::GetJniTransition(JNIEnv *env, jobject jTransition) {
        CJNIModuleTransition *ret = NULL;
        env->PushLocalFrame(10);
        do {
            if (env == NULL || jTransition == NULL) {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jTransition);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, TRANSITION_FIELD_NATIVE_ADDRESS_NAME,
                                            TRANSITION_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java java field '%s' failed", TRANSITION_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jTransition, jfld);
            if (nValue == 0 || !CJNIModuleManager::getInstance()->contains((CJNIModuleTransition *) nValue)) {
                //LOGE("get jni Transition from java object failed");
                //return NULL;
                LOGI("try to get a new CJNIModuleTransition");
                ret = CreateJniTransition(env, jTransition);
            } else {
                ret = (CJNIModuleTransition *) nValue;
            }

        } while (0);

        env->PopLocalFrame(NULL);
        return ret;
    }

    bool CJNIModuleTransition::IsValid(CJNIModuleTransition *p) {
        // checks residency validity
        return CJNIModuleManager::getInstance()->contains(p);
    }

    jboolean
    CJNIModuleTransition::jni_init(JNIEnv *env, jobject jTransition, jstring jsrc, jlong jstart, jlong jduration) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::CreateJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return FALSE;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if ((jsrc == NULL) ||
            (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) <= 0)) {
            LOGE("file path is null or char array is not enough!");
            return FALSE;
        }
        pJNITransition->getTransition()->init(achSrcPath, jstart, jduration);
        return TRUE;
    }

    jboolean CJNIModuleTransition::jni_uninit(JNIEnv *env, jobject jTransition) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return FALSE;
        } else {
            CJNIModuleTransition::DestroyJniTransition(pJNITransition);
        }
        return TRUE;
    }

    jstring CJNIModuleTransition::jni_getSrc(JNIEnv *env, jobject jTransition) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return NULL;
        }

        return stringToJString(env, pJNITransition->getTransition()->getSrc());
    }

    void CJNIModuleTransition::jni_setSrc(JNIEnv *env, jobject jTransition, jstring jsrc) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) >= 0) {
            pJNITransition->getTransition()->setSrc(achSrcPath);
        }

    }

    jlong CJNIModuleTransition::jni_getStart(JNIEnv *env, jobject jTransition) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return -1;
        }

        return pJNITransition->getTransition()->getStart();
    }

    void CJNIModuleTransition::jni_setStart(JNIEnv *env, jobject jTransition, jlong jstart) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return;
        }
        pJNITransition->getTransition()->setStart(jstart);
    }

    jlong CJNIModuleTransition::jni_getDuration(JNIEnv *env, jobject jTransition) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return -1;
        }
        return pJNITransition->getTransition()->getDuration();
    }

    void CJNIModuleTransition::jni_setDuration(JNIEnv *env, jobject jTransition, jlong jduration) {
        USE_LOG;
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env, jTransition);
        if (pJNITransition == NULL) {
            return;
        }
        pJNITransition->getTransition()->setDuration(jduration);
    }
}