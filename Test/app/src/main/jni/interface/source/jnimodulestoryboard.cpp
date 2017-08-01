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
#include <jnimodulemanager.h>
#include "jnimoduleclip.h"
#include "constant.h"
#include "jnimodulestoryboard.h"
#include "autolog.h"
#include "jnicommon.h"

namespace paomiantv {

    void JNIModuleStoryboard_OnFailed(void *delegate, s32 nErr, s8 *pchDescription) {
        if (delegate != NULL) {
            ((CJNIModuleStoryboard *) delegate)->failedCallback(nErr, pchDescription);
        }

    }

    void JNIModuleStoryboard_OnSuccess(void *delegate) {
        if (delegate != NULL) {
            ((CJNIModuleStoryboard *) delegate)->successCallback();
        }
    }

    void JNIModuleStoryboard_OnProgress(void *delegate, s32 nProgress) {
        if (delegate != NULL) {
            ((CJNIModuleStoryboard *) delegate)->progressCallback(nProgress);
        }
    }

    void JNIModuleStoryboard_OnAlways(void *delegate) {
        if (delegate != NULL) {
            ((CJNIModuleStoryboard *) delegate)->alwaysCallback();
        }
    }

    TJavaClazzParam *CJNIModuleStoryboard::GetJavaClazzParam() {
        TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
        JNINativeMethod arrMethods[] =
                {
                        {"_init",         "(Ljava/lang/String;)Z",              (void *) jni_init},
                        {"_uninit",       "()Z",                                (void *) jni_uninit},
                        {"_setBGM",       "(Ljava/lang/String;JJJJ)Z",          (void *) jni_setBGM},
                        {"_addClip",      "(Lcn/paomiantv/mediasdk/PMClip;)Z",  (void *) jni_addClip},
                        {"_insertClip",   "(ILcn/paomiantv/mediasdk/PMClip;)Z", (void *) jni_insertClip},
                        {"_removeClip",   "(I)Lcn/paomiantv/mediasdk/PMClip",   (void *) jni_removeClip},
                        {"_getClip",      "(I)Lcn/paomiantv/mediasdk/PMClip;",  (void *) jni_getClip},
                        {"_swapClip",     "(II)Z",                              (void *) jni_swapClip},
                        {"_getClipCount", "()I",                                (void *) jni_getClipCount},
                        {"_procsss",      "()Z",                                (void *) jni_procsss},
                        {"_cancel",       "()Z",                                (void *) jni_cancel},
                };
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/mediasdk/PMStoryboard%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }

    CJNIModuleStoryboard::CJNIModuleStoryboard(JNIEnv *env, jobject jStoryboard, jclass jcls,
                                               jfieldID jfld) {
        USE_LOG;
        m_sJNIClips.clear();
        if (env == NULL || jStoryboard == NULL || jcls == NULL || jfld == NULL) {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0) {
            LOGE("get java vm failed");
            return;
        }

        //get methods of the object jstoryboard
        TJavaMemeber atJAVAMemberMethod[] = {
                {STORYBOARD_METHOD_FAILED_NAME,   STORYBOARD_METHOD_FAILED_SIG},
                {STORYBOARD_METHOD_PROGRESS_NAME, STORYBOARD_METHOD_PROGRESS_SIG},
                {STORYBOARD_METHOD_SUCCESS_NAME,  STORYBOARD_METHOD_SUCCESS_SIG},
                {STORYBOARD_METHOD_ALWAYS_NAME,   STORYBOARD_METHOD_ALWAYS_SIG}};
        for (s32 i = 0; i < NELEM(atJAVAMemberMethod); i++) {
            m_ajmtd[i] = env->GetMethodID(jcls, atJAVAMemberMethod[i].m_pchName,
                                          atJAVAMemberMethod[i].m_pchSigniture);
            if (m_ajmtd[i] == NULL) {
                LOGE("java method 'void %s()' is not defined", atJAVAMemberMethod[i].m_pchName);
                return;
            }
        }

        m_jfldNativeAddr = jfld;

        m_jObject = env->NewGlobalRef(jStoryboard);
        if (m_jObject == NULL) {
            LOGE("new global reference failed ,jvm stack table is full or unknown reason");
        }
        m_pStoryboard = new CStoryboard;
        if (m_pStoryboard != NULL) {
            LOGD("Storyboard instance allocated: %u", sizeof(CStoryboard));
            // only register valid ones
            m_pStoryboard->bindEvent(JNIModuleStoryboard_OnFailed, JNIModuleStoryboard_OnSuccess,
                                     JNIModuleStoryboard_OnProgress,
                                     JNIModuleStoryboard_OnAlways, this);
            CJNIModuleManger::getInstance()->add(this);
        } else {
            LOGE("new storyboard failed ,memory is not enough!");
        }

    }

    CJNIModuleStoryboard::~CJNIModuleStoryboard() {
        USE_LOG;

        if (m_pStoryboard != NULL) {
            delete m_pStoryboard;
            m_pStoryboard = NULL;
            LOGD("Storyboard instance freed: %u", sizeof(CStoryboard));
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
            memset(m_ajmtd, 0, sizeof(m_ajmtd));
        }
        std::set<CJNIModuleClip *>::iterator iter = m_sJNIClips.begin();

        while (iter != m_sJNIClips.end()) {
            if (*iter != NULL) {
                CJNIModuleClip *pJNIClip = *iter;
                CJNIModuleClip::DestroyJniClip(pJNIClip);
            }
            ++iter;
        }
        m_sJNIClips.clear();
        // be sure unregister before killing
        CJNIModuleManger::getInstance()->remove(this);
    }

    CJNIModuleStoryboard *
    CJNIModuleStoryboard::CreateJniStoryboard(JNIEnv *env, jobject jStoryboard) {
        USE_LOG;
        CJNIModuleStoryboard *ret = NULL;
        env->PushLocalFrame(10);
        do {
            //get class of the object jstoryboard
            jclass jcls = env->GetObjectClass(jStoryboard);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }
            //get field native_address of the object jstoryboard
            jfieldID jfld = env->GetFieldID(jcls, STORYBOARD_FIELD_NATIVE_ADDRESS_NAME,
                                            STORYBOARD_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java field '%s' is not defined", STORYBOARD_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jStoryboard, jfld);
            if (nValue != 0 && CJNIModuleManger::getInstance()->contains((CJNIModule *) nValue)) {
                LOGI("the CJNIModuleStoryboard is already created");
                ret = (CJNIModuleStoryboard *) nValue;
                break;
            }

            CJNIModuleStoryboard *pNew = new CJNIModuleStoryboard(env, jStoryboard, jcls, jfld);
            if (!CJNIModuleManger::getInstance()->contains(pNew)) {
                LOGE("create CJNIModuleStoryboard failed");
                delete pNew;
                ret = NULL;
                break;
            } else {
                LOGD("CJNIModuleStoryboard instance allocated: %u", sizeof(CJNIModuleStoryboard));
            }

            env->SetIntField(jStoryboard, jfld, (jint) pNew);
            ret = pNew;

            LOGI("create CJNIModuleStoryboard ok");
        } while (0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleStoryboard::DestroyJniStoryboard(CJNIModuleStoryboard *&p) {
        USE_LOG;

        if (p == NULL) {
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

            LOGI("destroy CJNIModuleStoryboard ok");
        } while (0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleStoryboard *CJNIModuleStoryboard::GetJniStoryboard(JNIEnv *env, jobject jStoryboard) {
        CJNIModuleStoryboard *ret = NULL;
        env->PushLocalFrame(10);
        do {
            if (env == NULL || jStoryboard == NULL) {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jStoryboard);
            if (jcls == NULL) {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, STORYBOARD_FIELD_NATIVE_ADDRESS_NAME,
                                            STORYBOARD_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL) {
                LOGE("java java field '%s' failed", STORYBOARD_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jStoryboard, jfld);
            if (nValue == 0 || !CJNIModuleManger::getInstance()->contains((CJNIModuleStoryboard *) nValue)) {
                LOGE("get jni storyboard from java object failed");
                break;
//            LOGI("try to get a new CJNIModuleStoryboard");
//            ret = CreateJniStoryboard(env, jStoryboard);
            } else {
                ret = (CJNIModuleStoryboard *) nValue;
            }

        } while (0);

        env->PopLocalFrame(NULL);
        return ret;
    }

    bool CJNIModuleStoryboard::IsValid(CJNIModuleStoryboard *p) {
        // checks residency validity
        return CJNIModuleManger::getInstance()->contains(p);
    }

    jboolean CJNIModuleStoryboard::jni_init(JNIEnv *env, jobject jstoryboard, jstring jdstPath) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::CreateJniStoryboard(env,
                                                                                         jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        s8 achDstPath[MAX_LEN_FILE_PATH] = {0};
        if ((jdstPath == NULL) ||
            (getStringBytes(env, jdstPath, achDstPath, sizeof(achDstPath)) <= 0)) {
            LOGE("file path is null or char array is not enough!");
            return FALSE;
        }
        return pJNIStoryboard->getCStoryboard()->init(achDstPath);
    }

    jboolean CJNIModuleStoryboard::jni_uninit(JNIEnv *env, jobject jstoryboard) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        } else {
            CJNIModuleStoryboard::DestroyJniStoryboard(pJNIStoryboard);
        }
        return TRUE;
    }

    jboolean CJNIModuleStoryboard::jni_setBGM(JNIEnv *env, jobject jstoryboard, jstring jBGMPath,
                                              jlong jstartCutTm, jlong jdurationCutTm,
                                              jlong jstartTm,
                                              jlong jendTm) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        s8 achBGMPath[MAX_LEN_FILE_PATH] = {0};
        if ((jBGMPath == NULL) ||
            (getStringBytes(env, jBGMPath, achBGMPath, sizeof(achBGMPath)) <= 0)) {
            LOGE("file path is null or char array is not enough!");
            return FALSE;
        }
        return pJNIStoryboard->getCStoryboard()->setBGM(achBGMPath, jstartCutTm, jdurationCutTm,
                                                        jstartTm, jendTm);
    }

    jboolean CJNIModuleStoryboard::jni_addClip(JNIEnv *env, jobject jstoryboard, jobject jclip) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        return pJNIStoryboard->addClip(pJNIClip);
    }

    jboolean
    CJNIModuleStoryboard::jni_insertClip(JNIEnv *env, jobject jstoryboard, jint jindex,
                                         jobject jclip) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        CJNIModuleClip *pJNIClip = CJNIModuleClip::GetJniClip(env, jclip);
        if (pJNIClip == NULL) {
            return FALSE;
        }
        return pJNIStoryboard->insertClip(jindex, pJNIClip);
    }

    jobject CJNIModuleStoryboard::jni_removeClip(JNIEnv *env, jobject jstoryboard, jint jindex) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return NULL;
        }
        CJNIModuleClip *pJNIClip = pJNIStoryboard->removeClip(jindex);
        if (pJNIClip==NULL){
            return NULL;
        }
        return pJNIClip->getObject();
    }

    jobject CJNIModuleStoryboard::jni_getClip(JNIEnv *env, jobject jstoryboard, jint jindex) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return NULL;
        }
        CJNIModuleClip *pJNIClip = pJNIStoryboard->getClip(jindex);
        if (pJNIClip == NULL) {
            return NULL;
        }
        return pJNIClip->getObject();
    }

    jboolean
    CJNIModuleStoryboard::jni_swapClip(JNIEnv *env, jobject jstoryboard, jint jindexA,
                                       jint jindexB) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        return pJNIStoryboard->getCStoryboard()->swapClip(jindexA, jindexB);
    }

    jint CJNIModuleStoryboard::jni_getClipCount(JNIEnv *env, jobject jstoryboard) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return 0;
        }
        return pJNIStoryboard->getCStoryboard()->getClipCount();
    }

    jboolean CJNIModuleStoryboard::jni_procsss(JNIEnv *env, jobject jstoryboard) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        pJNIStoryboard->getCStoryboard()->process();
        return TRUE;
    }

    jboolean CJNIModuleStoryboard::jni_cancel(JNIEnv *env, jobject jstoryboard) {
        USE_LOG;
        CJNIModuleStoryboard *pJNIStoryboard = CJNIModuleStoryboard::GetJniStoryboard(env,
                                                                                      jstoryboard);
        if (pJNIStoryboard == NULL) {
            return FALSE;
        }
        pJNIStoryboard->getCStoryboard()->cancel();
        return TRUE;
    }

    BOOL32 CJNIModuleStoryboard::addClip(CJNIModuleClip *clip) {
        m_sJNIClips.insert(clip);
        return m_pStoryboard->addClip(clip->getCClip());
    }

    BOOL32 CJNIModuleStoryboard::insertClip(s32 nIndex, CJNIModuleClip *clip) {
        m_sJNIClips.insert(clip);
        return m_pStoryboard->insertClip(nIndex, clip->getCClip());
    }

    CJNIModuleClip* CJNIModuleStoryboard::removeClip(s32 nIndex) {
        CClip *pClip = m_pStoryboard->removeClip(nIndex);
        if (pClip == NULL) {
            return NULL;
        }
        std::set<CJNIModuleClip *>::iterator iter;
        for (iter = m_sJNIClips.begin(); iter != m_sJNIClips.end();) {
            if (*iter != NULL && (*iter)->getCClip() == pClip) {
                CJNIModuleClip *pJNIClip = *iter;
                m_sJNIClips.erase(iter);
                return pJNIClip;
            }
            ++iter;
        }
        return NULL;
    }

    CJNIModuleClip *CJNIModuleStoryboard::getClip(s32 nIndex) {
        CClip *pClip = m_pStoryboard->getClip(nIndex);
        if (pClip == NULL) {
            return NULL;
        }
        std::set<CJNIModuleClip *>::iterator iter;
        for (iter = m_sJNIClips.begin(); iter != m_sJNIClips.end();) {
            if (*iter != NULL && (*iter)->getCClip() == pClip) {
                return *iter;
            }
            ++iter;
        }
        return NULL;
    }

    void CJNIModuleStoryboard::failedCallback(s32 nErr, s8 *pchDescription) {
        if (!m_jvm || !m_jObject || !m_ajmtd[0])
            return;
        JNIEnv *env = NULL;
        bool isAttacked = false;
        int status = (m_jvm)->GetEnv((void **) &env, JNI_VERSION_1_4);
        if (status != JNI_OK) {

            status = m_jvm->AttachCurrentThread(&env, NULL);
            if (status != JNI_OK) {
                LOGE("failed to attach current thread");
                return;
            }
            isAttacked = true;
        }

        env->PushLocalFrame(10);
        jstring jnistr = stringToJString(env, pchDescription);
        env->CallVoidMethod(m_jObject, m_ajmtd[0], nErr, jnistr);
        env->DeleteLocalRef(jnistr);
        env->PopLocalFrame(NULL);

        if (isAttacked) {
            m_jvm->DetachCurrentThread();
        }
    }

    void CJNIModuleStoryboard::progressCallback(s32 nProgress) {

        if (!m_jvm || !m_jObject || !m_ajmtd[1])
            return;
        JNIEnv *env = NULL;
        bool isAttacked = false;
        int status = (m_jvm)->GetEnv((void **) &env, JNI_VERSION_1_4);
        if (status != JNI_OK) {

            status = m_jvm->AttachCurrentThread(&env, NULL);
            if (status != JNI_OK) {
                LOGE("failed to attach current thread");
                return;
            }
            isAttacked = true;
        }

        env->PushLocalFrame(10);
        env->CallVoidMethod(m_jObject, m_ajmtd[1], nProgress);
        env->PopLocalFrame(NULL);

        if (isAttacked) {
            m_jvm->DetachCurrentThread();
        }
    }

    void CJNIModuleStoryboard::successCallback() {
        if (!m_jvm || !m_jObject || !m_ajmtd[2])
            return;
        JNIEnv *env = NULL;
        bool isAttacked = false;
        int status = (m_jvm)->GetEnv((void **) &env, JNI_VERSION_1_4);
        if (status != JNI_OK) {

            status = m_jvm->AttachCurrentThread(&env, NULL);
            if (status != JNI_OK) {
                LOGE("failed to attach current thread");
                return;
            }
            isAttacked = true;
        }

        env->PushLocalFrame(10);
        jstring jnistr = stringToJString(env, m_pStoryboard->getDstPath());
        env->CallVoidMethod(m_jObject, m_ajmtd[2], jnistr);
        env->DeleteLocalRef(jnistr);
        env->PopLocalFrame(NULL);

        if (isAttacked) {
            m_jvm->DetachCurrentThread();
        }
    }

    void CJNIModuleStoryboard::alwaysCallback() {
        if (!m_jvm || !m_jObject || !m_ajmtd[2])
            return;
        JNIEnv *env = NULL;
        bool isAttacked = false;
        int status = (m_jvm)->GetEnv((void **) &env, JNI_VERSION_1_4);
        if (status != JNI_OK) {

            status = m_jvm->AttachCurrentThread(&env, NULL);
            if (status != JNI_OK) {
                LOGE("failed to attach current thread");
                return;
            }
            isAttacked = true;
        }

        env->PushLocalFrame(10);
        jstring jnistr = stringToJString(env, m_pStoryboard->getDstPath());
        env->CallVoidMethod(m_jObject, m_ajmtd[3], jnistr);
        env->DeleteLocalRef(jnistr);
        env->PopLocalFrame(NULL);

        if (isAttacked) {
            m_jvm->DetachCurrentThread();
        }
    }

}
