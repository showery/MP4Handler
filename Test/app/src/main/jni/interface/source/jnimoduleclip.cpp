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

#include <jnimodulemanager.h>
#include "constant.h"
#include "autolog.h"
#include "jnicommon.h"
#include "stdlib.h"
#include "jnimoduleclip.h"

namespace paomiantv {

    TJavaClazzParam *CJNIModuleClip::GetJavaClazzParam() {
        TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
        JNINativeMethod arrMethods[] =
                {
                        {"_init",               "(Ljava/lang/String;JJ)Z",                 (void *) jni_init},
                        {"_uninit",             "()Z",                                     (void *) jni_uninit},
                        {"_getSrc",             "()Ljava/lang/String;",                    (void *) jni_getSrc},
                        {"_setSrc",             "(Ljava/lang/String;)V",                   (void *) jni_setSrc},
                        {"_getStart",           "()J",                                     (void *) jni_getStart},
                        {"_setStart",           "(J)V",                                    (void *) jni_setStart},
                        {"_getDuration",        "()J",                                     (void *) jni_getDuration},
                        {"_setDuration",        "(J)V",                                    (void *) jni_setDuration},
                        {"_getFilter",          "(I)Lcn/paomiantv/mediasdk/PMFilter;",     (void *) jni_getFilter},
                        {"_addFilter",          "(Lcn/paomiantv/mediasdk/PMFilter;)Z",     (void *) jni_addFilter},
                        {"_removeFilter",       "(I)Lcn/paomiantv/mediasdk/PMFilter;",     (void *) jni_removeFilter},
                        {"_getFilterCount",     "()I",                                     (void *) jni_getFilterCount},
                        {"_addTransition",      "(Lcn/paomiantv/mediasdk/PMTransition;)Z", (void *) jni_addTransition},
                        {"_getTransition",      "(I)Lcn/paomiantv/mediasdk/PMTransition;", (void *) jni_getTransition},
                        {"_removeTransition",   "(I)Lcn/paomiantv/mediasdk/PMTransition;", (void *) jni_removeTransition},
                        {"_getTransitionCount", "()I",                                     (void *) jni_getTransitionCount},
                };
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMClip%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }


    CJNIModuleClip::CJNIModuleClip(JNIEnv *env, jobject jClip, jfieldID jfld) {
        USE_LOG;

        if (env == NULL || jClip == NULL || jfld == NULL) {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0) {
            LOGE("get java vm failed");
            return;
        }

        m_jfldNativeAddr = jfld;

        m_jObject = env->NewGlobalRef(jClip);
        if (m_jObject == NULL) {
            LOGE("new global reference failed ,jvm stack table is full or unknown reason");
        }
        m_pClip = new CClip;
        if (m_pClip != NULL) {
            LOGD("Clip instance allocated: %u", sizeof(CClip));
            // only register valid ones
            CJNIModuleManger::getInstance()->add(this);
        } else {
            LOGE("new clip failed ,memory is not enough!");
        }

    }

    CJNIModuleClip::~CJNIModuleClip() {
        USE_LOG;

        if (m_pClip != NULL) {
            delete m_pClip;
            m_pClip = NULL;
            LOGD("Clip instance freed: %u", sizeof(CClip));
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
        }

        std::set<CJNIModuleFilter *>::iterator iterFilter = m_sJNIFilters.begin();

        while (iterFilter != m_sJNIFilters.end()) {
            if (*iterFilter != NULL) {
                CJNIModuleFilter *pJNIFilter = *iterFilter;
                CJNIModuleFilter::DestroyJniFilter(pJNIFilter);
            }
            ++iterFilter;
        }
        m_sJNIFilters.clear();

        std::set<CJNIModuleTransition *>::iterator iterTrans = m_sJNITransitions.begin();

        while (iterTrans != m_sJNITransitions.end()) {
            if (*iterTrans != NULL) {

                CJNIModuleTransition *pJNITrans = *iterTrans;
                CJNIModuleTransition::DestroyJniTransition(pJNITrans);
            }
            ++iterTrans;
        }
        m_sJNITransitions.clear();

        // be sure unregister before killing
        CJNIModuleManger::getInstance()->remove(this);
    }

    CJNIModuleClip *CJNIModuleClip::CreateJniClip(JNIEnv *env, jobject jClip) {
        USE_LOG;
        CJNIModuleClip *ret = NULL;
        env->PushLocalFrame(10);
        do {
            //get class of the object jClip
            jclass jcls = env->GetObjectClass(jClip);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }
            //get field native_address of the object jClip
            jfieldID jfld = env->GetFieldID(jcls, CLIP_FIELD_NATIVE_ADDRESS_NAME,
                                            CLIP_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java field '%s' is not defined", CLIP_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jClip, jfld);
            if (nValue != 0 && CJNIModuleManger::getInstance()->contains((CJNIModule *) nValue)) {
                LOGI("the CJNIModuleClip is already created");
                ret = (CJNIModuleClip *) nValue;
                break;
            }

            CJNIModuleClip *pNew = new CJNIModuleClip(env, jClip, jfld);
            if (!CJNIModuleManger::getInstance()->contains(pNew)) {
                LOGE("create CJNIModuleClip failed");
                delete pNew;
                ret = NULL;
                break;
            } else {
                LOGD("CJNIModuleClip instance allocated: %u", sizeof(CJNIModuleClip));
            }

            env->SetIntField(jClip, jfld, (jint) pNew);
            ret = pNew;

            LOGI("create CJNIModuleClip ok");
        } while (0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleClip::DestroyJniClip(CJNIModuleClip *&p) {
        USE_LOG;

        if (p == NULL || !CJNIModuleManger::getInstance()->contains(p)) {
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

            LOGI("destroy CJNIModuleClip ok");
        } while (0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleClip *CJNIModuleClip::GetJniClip(JNIEnv *env, jobject jClip) {
        CJNIModuleClip *ret = NULL;
        env->PushLocalFrame(10);
        do {
            if (env == NULL || jClip == NULL) {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jClip);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, CLIP_FIELD_NATIVE_ADDRESS_NAME,
                                            CLIP_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java java field '%s' failed", CLIP_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jClip, jfld);
            if (nValue == 0 || !CJNIModuleManger::getInstance()->contains((CJNIModuleClip *) nValue)) {
                //LOGE("get jni Clip from java object failed");
                //return NULL;
                LOGI("try to get a new CJNIModuleClip");
                ret = CreateJniClip(env, jClip);
            } else {
                ret = (CJNIModuleClip *) nValue;
            }

        } while (0);

        env->PopLocalFrame(NULL);
        return ret;
    }

    bool CJNIModuleClip::IsValid(CJNIModuleClip *p) {
        // checks residency validity
        return CJNIModuleManger::getInstance()->contains(p);
    }

    jboolean
    CJNIModuleClip::jni_init(JNIEnv *env, jobject jclip, jstring jsrc, jlong jstart,
                             jlong jduration) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::CreateJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if ((jsrc == NULL) ||
            (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) <= 0)) {
            LOGE("file path is null or char array is not enough!");
            return FALSE;
        }
        pJNIClip->getCClip()->init(achSrcPath, jstart, jduration);
        return TRUE;
    }

    jboolean CJNIModuleClip::jni_uninit(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        } else {
            CJNIModuleClip::DestroyJniClip(pJNIClip);
        }
        return TRUE;
    }

    jstring CJNIModuleClip::jni_getSrc(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return NULL;
        }

        return stringToJString(env, pJNIClip->getCClip()->getSrc());
    }

    void CJNIModuleClip::jni_setSrc(JNIEnv *env, jobject jclip, jstring jsrc) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return;
        }
        s8 achSrcPath[MAX_LEN_FILE_PATH] = {0};
        if (getStringBytes(env, jsrc, achSrcPath, MAX_LEN_FILE_PATH) >= 0) {
            pJNIClip->getCClip()->setSrc(achSrcPath);
        }

    }

    jlong CJNIModuleClip::jni_getStart(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return -1;
        }

        return pJNIClip->getCClip()->getStart();
    }

    void CJNIModuleClip::jni_setStart(JNIEnv *env, jobject jclip, jlong jstart) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return;
        }
        pJNIClip->getCClip()->setStart(jstart);
    }

    jlong CJNIModuleClip::jni_getDuration(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return -1;
        }
        return pJNIClip->getCClip()->getDuration();
    }

    void CJNIModuleClip::jni_setDuration(JNIEnv *env, jobject jclip, jlong jduration) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return;
        }
        pJNIClip->getCClip()->setDuration(jduration);
    }

    jobject CJNIModuleClip::jni_getFilter(JNIEnv *env, jobject jclip, jint jindex) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return NULL;
        }
        CJNIModuleFilter *pJNIFilter = pJNIClip->getFilter(jindex);
        if (pJNIFilter == NULL) {
            return NULL;
        }
        return pJNIFilter->getObject();
    }

    jboolean CJNIModuleClip::jni_addFilter(JNIEnv *env, jobject jclip, jobject jfilter) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }

        CJNIModuleFilter *pJNIFilter = CJNIModuleFilter::GetJniFilter(env, jclip);
        if (pJNIFilter == NULL) {
            return FALSE;
        }
        return pJNIClip->addFilter(pJNIFilter);
    }

    jobject CJNIModuleClip::jni_removeFilter(JNIEnv *env, jobject jclip, jint jposition) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        CJNIModuleFilter *pJNIFilter = pJNIClip->removeFilter(jposition);
        if (pJNIFilter == NULL) {
            return NULL;
        }
        return pJNIFilter->getObject();
    }

    jint CJNIModuleClip::jni_getFilterCount(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return 0;
        }
        return pJNIClip->getCClip()->getFilterCount();
    }

    jboolean CJNIModuleClip::jni_addTransition(JNIEnv *env, jobject jclip, jobject jtransition) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        CJNIModuleTransition *pJNITransition = CJNIModuleTransition::GetJniTransition(env,
                                                                                      jtransition);
        if (pJNITransition == NULL) {
            return FALSE;
        }
        return pJNIClip->addTransition(pJNITransition);
    }

    jobject CJNIModuleClip::jni_getTransition(JNIEnv *env, jobject jclip, jint jposition) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return NULL;
        }
        CJNIModuleTransition *pJNITransition = pJNIClip->getTransition(jposition);
        if (pJNITransition == NULL) {
            return NULL;
        }
        return pJNITransition->getObject();
    }

    jobject CJNIModuleClip::jni_removeTransition(JNIEnv *env, jobject jclip, jint jposition) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        CJNIModuleTransition *pJNITransition = pJNIClip->removeTransition(jposition);
        if (pJNITransition == NULL) {
            return NULL;
        }
        return pJNITransition->getObject();
    }

    jint CJNIModuleClip::jni_getTransitionCount(JNIEnv *env, jobject jclip) {
        USE_LOG;
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return 0;
        }
        return pJNIClip->getCClip()->getTransitionCount();
    }

    CJNIModuleFilter *CJNIModuleClip::getFilter(s32 position) {
        CFilter *pFilter = m_pClip->getFilter(position);
        if (pFilter == NULL) {
            return NULL;
        }
        std::set<CJNIModuleFilter *>::iterator iter;
        for (iter = m_sJNIFilters.begin(); iter != m_sJNIFilters.end();) {
            if (*iter != NULL && (*iter)->getFilter() == pFilter) {
                return *iter;
            }
            ++iter;
        }
        return NULL;
    }

    CJNIModuleTransition *CJNIModuleClip::getTransition(s32 position) {
        CTransition *pTransition = m_pClip->getTransition(position);
        if (pTransition == NULL) {
            return NULL;
        }
        std::set<CJNIModuleTransition *>::iterator iter;
        for (iter = m_sJNITransitions.begin(); iter != m_sJNITransitions.end();) {
            if (*iter != NULL && (*iter)->getTransition() == pTransition) {
                return *iter;
            }
            ++iter;
        }
        return NULL;
    }

    BOOL32 CJNIModuleClip::addFilter(CJNIModuleFilter *filter) {
        m_sJNIFilters.insert(filter);

        m_pClip->addFilter(filter->getFilter());
        return TRUE;
    }

    BOOL32 CJNIModuleClip::addTransition(CJNIModuleTransition *transition) {
        m_sJNITransitions.insert(transition);

        m_pClip->addTransition(transition->getTransition());
        return TRUE;
    }

    CJNIModuleFilter *CJNIModuleClip::removeFilter(s32 nIndex) {
        CFilter *pFilter = m_pClip->removeFilter(nIndex);
        if (pFilter == NULL) {
            return NULL;
        }
        std::set<CJNIModuleFilter *>::iterator iter;
        for (iter = m_sJNIFilters.begin(); iter != m_sJNIFilters.end();) {
            if (*iter != NULL && (*iter)->getFilter() == pFilter) {
                CJNIModuleFilter *pJNIFilter = *iter;

                m_sJNIFilters.erase(iter);
                return pJNIFilter;


            }
            ++iter;
        }
        return NULL;
    }

    CJNIModuleTransition *CJNIModuleClip::removeTransition(s32 nIndex) {
        CTransition *pTransition = m_pClip->removeTransition(nIndex);
        if (pTransition == NULL) {
            return NULL;
        }
        std::set<CJNIModuleTransition *>::iterator iter;
        for (iter = m_sJNITransitions.begin(); iter != m_sJNITransitions.end();) {
            if (*iter != NULL && (*iter)->getTransition() == pTransition) {
                CJNIModuleTransition *pJNITransition = *iter;

                m_sJNITransitions.erase(iter);

                return pJNITransition;

            }
            ++iter;
        }
        return NULL;
    }
}