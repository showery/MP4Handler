/*******************************************************************************
 *        Module: paomiantv
 *          File: jnimodulefilter.cpp
 * Functionality: filter jni.
 *       Related: mediasdk
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-30  v1.0        huangxuefeng  created
 ******************************************************************************/


#include "jnimodulemanager.h"
#include "constant.h"
#include "jnicommon.h"
#include "jnimodulefilter.h"

namespace paomiantv {
    CLock CJNIModuleFilter::m_SingleInstanceLock;
    TJavaClazzParam *CJNIModuleFilter::GetJavaClazzParam() {
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
                        {"_setDuration", "(J)V",                    (void *) jni_setDuration}};
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMFilter%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }


    CJNIModuleFilter::CJNIModuleFilter(JNIEnv *env, jobject jFilter, jfieldID jfld) {
        USE_LOG;

        if (env == NULL || jFilter == NULL || jfld == NULL) {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0) {
            LOGE("get java vm failed");
            return;
        }

        m_jfldNativeAddr = jfld;

        m_jObject = env->NewGlobalRef(jFilter);
        if (m_jObject == NULL) {
            LOGE("new global reference failed ,jvm stack table is full or unknown reason");
        }
        m_pFilter = new CFilter;
        if (m_pFilter != NULL) {
            LOGD("Filter instance allocated: %u", sizeof(CFilter));
            // only register valid ones
            CJNIModuleManager::getInstance()->add(this);
        } else {
            LOGE("new filter failed ,memory is not enough!");
        }

    }

    CJNIModuleFilter::~CJNIModuleFilter() {
        USE_LOG;

        if (m_pFilter != NULL) {
            delete m_pFilter;
            m_pFilter = NULL;
            LOGD("Filter instance freed: %u", sizeof(CFilter));
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

    CJNIModuleFilter *CJNIModuleFilter::CreateJniFilter(JNIEnv *env, jobject jFilter) {
        USE_LOG;
        CJNIModuleFilter *ret = NULL;
        env->PushLocalFrame(10);
        do {
            //get class of the object jFilter
            jclass jcls = env->GetObjectClass(jFilter);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }
            //get field native_address of the object jFilter
            jfieldID jfld = env->GetFieldID(jcls, FILTER_FIELD_NATIVE_ADDRESS_NAME,
                                            FILTER_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java field '%s' is not defined", FILTER_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jFilter, jfld);
            if (nValue != 0 && CJNIModuleManager::getInstance()->contains((CJNIModule *) nValue)) {
                LOGI("the CJNIModuleFilter is already created");
                ret = (CJNIModuleFilter *) nValue;
                break;
            }

            CJNIModuleFilter *pNew = new CJNIModuleFilter(env, jFilter, jfld);
            if (!CJNIModuleManager::getInstance()->contains(pNew)) {
                LOGE("create CJNIModuleFilter failed");
                delete pNew;
                ret = NULL;
                break;
            } else {
                LOGD("CJNIModuleFilter instance allocated: %u", sizeof(CJNIModuleFilter));
            }

            env->SetIntField(jFilter, jfld, (jint) pNew);
            ret = pNew;

            LOGI("create CJNIModuleFilter ok");
        } while (0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleFilter::DestroyJniFilter(CJNIModuleFilter *&p) {
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

            LOGI("destroy CJNIModuleFilter ok");
        } while (0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleFilter *CJNIModuleFilter::GetJniFilter(JNIEnv *env, jobject jFilter) {
        CJNIModuleFilter *ret = NULL;
        env->PushLocalFrame(10);
        do {
            if (env == NULL || jFilter == NULL) {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jFilter);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, FILTER_FIELD_NATIVE_ADDRESS_NAME,
                                            FILTER_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java java field '%s' failed", FILTER_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jFilter, jfld);
            if (nValue == 0 ||
                !CJNIModuleManager::getInstance()->contains((CJNIModuleFilter *) nValue)) {
                //LOGE("get jni Filter from java object failed");
                //return NULL;
                m_SingleInstanceLock.lock();
                if(!env->GetIntField(jFilter, jfld) ||
                   !CJNIModuleManager::getInstance()->contains((CJNIModuleFilter *) nValue)){
                    LOGI("try to get a new CJNIModuleFilter");
                    ret = CreateJniFilter(env, jFilter);
                }
                m_SingleInstanceLock.unlock();

            } else {
                ret = (CJNIModuleFilter *) nValue;
            }

        } while (0);

        env->PopLocalFrame(NULL);
        return ret;
    }

    bool CJNIModuleFilter::IsValid(CJNIModuleFilter *p) {
        // checks residency validity
        return CJNIModuleManager::getInstance()->contains(p);
    }

    jboolean CJNIModuleFilter::jni_init(JNIEnv *env, jobject jFilter, jstring jsrc, jlong jstart,
                                        jlong jduration) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return FALSE;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if ((jsrc == NULL) ||
            (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) <= 0)) {
            LOGE("file path is null or char array is not enough!");
            return FALSE;
        }
        pJNIFilter->getFilter()->init(achSrcPath, jstart, jduration);
        return TRUE;
    }

    jboolean CJNIModuleFilter::jni_uninit(JNIEnv *env, jobject jFilter) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL) {
            return FALSE;
        } else {
            CJNIModuleFilter::DestroyJniFilter(pJNIFilter);
        }
        return TRUE;
    }

    jstring CJNIModuleFilter::jni_getSrc(JNIEnv *env, jobject jFilter) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return NULL;
        }

        return stringToJString(env, pJNIFilter->getFilter()->getSrc());
    }

    void CJNIModuleFilter::jni_setSrc(JNIEnv *env, jobject jFilter, jstring jsrc) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) >= 0) {
            pJNIFilter->getFilter()->setSrc(achSrcPath);
        }

    }

    jlong CJNIModuleFilter::jni_getStart(JNIEnv *env, jobject jFilter) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return -1;
        }

        return pJNIFilter->getFilter()->getStart();
    }

    void CJNIModuleFilter::jni_setStart(JNIEnv *env, jobject jFilter, jlong jstart) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return;
        }
        pJNIFilter->getFilter()->setStart(jstart);
    }

    jlong CJNIModuleFilter::jni_getDuration(JNIEnv *env, jobject jFilter) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return -1;
        }
        return pJNIFilter->getFilter()->getDuration();
    }

    void CJNIModuleFilter::jni_setDuration(JNIEnv *env, jobject jFilter, jlong jduration) {
        USE_LOG;
        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jFilter);
        if (pJNIFilter == NULL || pJNIFilter->getFilter() == NULL) {
            return;
        }
        pJNIFilter->getFilter()->setDuration(jduration);
    }
}