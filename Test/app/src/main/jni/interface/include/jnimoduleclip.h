/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: define jni clip module
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_JNIMODULECLIP_H
#define _PAOMIANTV_JNIMODULECLIP_H

#include <set>
#include "clip.h"
#include "jnimodulefilter.h"
#include "jnimoduletransition.h"
//field name
#define CLIP_FIELD_NATIVE_ADDRESS_NAME "mNativeClipAddress"
//field signiture
#define CLIP_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv {

    class CJNIModuleClip : public CJNIModule {
    private:
        CClip *m_pClip;
        std::vector<CJNIModuleFilter *> m_vJNIFilters;
        std::vector<CJNIModuleTransition *> m_vJNITransitions;

    public:
        static TJavaClazzParam *GetJavaClazzParam();

        static CJNIModuleClip *CreateJniClip(JNIEnv *env, jobject jClip);

        static CJNIModuleClip *GetJniClip(JNIEnv *env, jobject jClip);

        static void DestroyJniClip(CJNIModuleClip *&p);

    private:
        CJNIModuleClip(JNIEnv *env, jobject jClip, jfieldID jfld);

        virtual ~CJNIModuleClip();


        static bool IsValid(CJNIModuleClip *p);


        static jboolean
        jni_init(JNIEnv *env, jobject jclip, jstring jsrc, jlong jstart, jlong jduration);

        static jboolean jni_uninit(JNIEnv *env, jobject jclip);

        static jstring jni_getSrc(JNIEnv *env, jobject jclip);

        static void jni_setSrc(JNIEnv *env, jobject jclip, jstring jsrc);

        static jlong jni_getStart(JNIEnv *env, jobject jclip);

        static void jni_setStart(JNIEnv *env, jobject jclip, jlong jstart);

        static jlong jni_getDuration(JNIEnv *env, jobject jclip);

        static void jni_setDuration(JNIEnv *env, jobject jclip, jlong jduration);

        static jobject jni_getFilter(JNIEnv *env, jobject jclip, jint jindex);

        static jboolean jni_addFilter(JNIEnv *env, jobject jclip, jobject jfilter);

        static jobject jni_removeFilter(JNIEnv *env, jobject jclip, jint jposition);

        static jint jni_getFilterCount(JNIEnv *env, jobject jclip);

        static jboolean jni_addTransition(JNIEnv *env, jobject jclip, jobject jposition);

        static jobject jni_getTransition(JNIEnv *env, jobject jclip, jint jindex);

        static jobject jni_removeTransition(JNIEnv *env, jobject jclip, jint jposition);

        static jint jni_getTransitionCount(JNIEnv *env, jobject jclip);

    public:
        BOOL32 addFilter(CJNIModuleFilter *filter);

        jobject removeFilter(s32 nIndex);

        CJNIModuleFilter *getFilter(s32 position);

        BOOL32 addTransition(CJNIModuleTransition *transition);

        jobject removeTransition(s32 nIndex);

        CJNIModuleTransition *getTransition(s32 position);


        inline CClip *getCClip();
    };

    inline CClip *CJNIModuleClip::getCClip() {
        return m_pClip;
    }
}

#endif /* _PAOMIANTV_JNIMODULECLIP_H */