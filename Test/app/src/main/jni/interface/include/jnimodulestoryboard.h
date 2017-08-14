/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: define jni storyboard module
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomaintv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_JNIMODULESTORYBOARD_H_
#define _PAOMIANTV_JNIMODULESTORYBOARD_H_

#include "jnimoduleclip.h"
#include "storyboard.h"

#define ERROR_SOURCE 10000
#define ERROR_SOURCE_DIST "Soure file is null or not existed!"

//method name
#define STORYBOARD_METHOD_FAILED_NAME "fireOnFailed"
#define STORYBOARD_METHOD_PROGRESS_NAME "fireOnProgress"
#define STORYBOARD_METHOD_SUCCESS_NAME "fireOnSuccess"
#define STORYBOARD_METHOD_ALWAYS_NAME "fireOnAlways"
//method signiture
#define STORYBOARD_METHOD_FAILED_SIG "(ILjava/lang/String;)V"
#define STORYBOARD_METHOD_PROGRESS_SIG "(I)V"
#define STORYBOARD_METHOD_SUCCESS_SIG "(Ljava/lang/String;)V"
#define STORYBOARD_METHOD_ALWAYS_SIG "()V"

//field name
#define STORYBOARD_FIELD_NATIVE_ADDRESS_NAME "mNativeStoryboardAddress"
//field signiture
#define STORYBOARD_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv {

    class CJNIModuleStoryboard : public CJNIModule {
    private:
        CStoryboard *m_pStoryboard;
        jmethodID m_ajmtd[4];
        std::vector<CJNIModuleClip *> m_vJNIClips;
        jobject jrenderer;
        static CLock m_SingleInstanceLock;

    public:
        static CJNIModuleStoryboard *CreateJniStoryboard(JNIEnv *env, jobject jStoryboard);

        static CJNIModuleStoryboard *GetJniStoryboard(JNIEnv *env, jobject jStoryboard);

        static void DestroyJniStoryboard(CJNIModuleStoryboard *&p);

        static bool IsValid(CJNIModuleStoryboard *p);

    private:
        CJNIModuleStoryboard(JNIEnv *env, jobject jStoryboard, jclass jcls, jfieldID jfld);

        virtual ~CJNIModuleStoryboard();

        friend void JNIModuleStoryboard_OnFailed(void *delegate, s32 nErr, s8 *pchDescription);

        friend void JNIModuleStoryboard_OnSuccess(void *delegate);

        friend void JNIModuleStoryboard_OnProgress(void *delegate, s32 nProgress);

        friend void JNIModuleStoryboard_OnAlways(void *delegate);


        static jboolean jni_init(JNIEnv *env, jobject jstoryboard, jstring jdstPath);

        static jboolean jni_uninit(JNIEnv *env, jobject jstoryboard);

        static jboolean
        jni_setBGM(JNIEnv *env, jobject jstoryboard, jstring jsrcpath, jlong jstartCutTm,
                   jlong jdurationCutTm, jlong jstartTm, jlong jendTm);

        static jboolean jni_addClip(JNIEnv *env, jobject jstoryboard, jobject jclip);


        static jboolean
        jni_replaceClip(JNIEnv *env, jobject jstoryboard, jint jindex, jobject jclip);

        static jboolean
        jni_insertClip(JNIEnv *env, jobject jstoryboard, jint jindex, jobject jclip);

        static jobject jni_removeClip(JNIEnv *env, jobject jstoryboard, jint jindex);

        static jobject jni_getClip(JNIEnv *env, jobject jstoryboard, jint jindex);

        static jboolean jni_swapClip(JNIEnv *env, jobject jstoryboard, jint jindexA, jint jindexB);

        static jint jni_getClipCount(JNIEnv *env, jobject jstoryboard);

        static jboolean jni_attachRenderer(JNIEnv *env, jobject jstoryboard, jobject jrenderer);

        static void jni_detachRenderer(JNIEnv *env, jobject jstoryboard);

        BOOL32 addClip(CJNIModuleClip *clip);

        BOOL32 replaceClip(s32 nIndex, CJNIModuleClip *clip);

        BOOL32 insertClip(s32 nIndex, CJNIModuleClip *clip);

        jobject removeClip(s32 nIndex);

        CJNIModuleClip *getClip(s32 jindex);

    public:


        static TJavaClazzParam *GetJavaClazzParam();

        void successCallback();

        void progressCallback(s32 nProgress);

        void failedCallback(s32 nErr, s8 *pchDescription);

        void alwaysCallback();




        inline CStoryboard *getCStoryboard();
    };

    inline CStoryboard *CJNIModuleStoryboard::getCStoryboard() {
        return m_pStoryboard;
    }
}

#endif /* _PAOMIANTV_JNIMODULESTORYBOARD_H_ */