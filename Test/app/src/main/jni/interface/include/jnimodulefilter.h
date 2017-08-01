/*******************************************************************************
 *        Module: interface
 *          File:
 * Functionality: define jni filter modules.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/

#ifndef _PAOMIANTV_JNIMODULEFILTER_H
#define _PAOMIANTV_JNIMODULEFILTER_H

#include "jnimodule.h"
#include "filter.h"

//field name
#define FILTER_FIELD_NATIVE_ADDRESS_NAME "mNativeFilterAddress"
//field signiture
#define FILTER_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv {
    class CJNIModuleFilter : public CJNIModule {

    private:
        CFilter *m_pFilter;

    public:
        static TJavaClazzParam *GetJavaClazzParam();

        static CJNIModuleFilter *CreateJniFilter(JNIEnv *env, jobject jfilter);

        static CJNIModuleFilter *GetJniFilter(JNIEnv *env, jobject jfilter);

        static void DestroyJniFilter(CJNIModuleFilter *&p);


    private:
        CJNIModuleFilter(JNIEnv *env, jobject jfilter, jfieldID jfld);

        virtual ~CJNIModuleFilter();

        static bool IsValid(CJNIModuleFilter *p);


        static jboolean jni_init(JNIEnv *env, jobject jfilter, jstring jsrc, jlong jstart,
                                 jlong jduration);

        static jboolean jni_uninit(JNIEnv *env, jobject jfilter);

        static jstring jni_getSrc(JNIEnv *env, jobject jfilter);

        static void jni_setSrc(JNIEnv *env, jobject jfilter, jstring jsrc);

        static jlong jni_getStart(JNIEnv *env, jobject jfilter);

        static void jni_setStart(JNIEnv *env, jobject jfilter, jlong jstart);

        static jlong jni_getDuration(JNIEnv *env, jobject jfilter);

        static void jni_setDuration(JNIEnv *env, jobject jfilter, jlong jduration);

    public:
        inline CFilter *getFilter();
    };

    inline CFilter *CJNIModuleFilter::getFilter() {
        return m_pFilter;
    }

}

#endif //_PAOMIANTV_JNIMODULEFILTER_H
