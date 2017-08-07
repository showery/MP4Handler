/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: define jni engine module
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

#ifndef _PAOMIANTV_JNIMODULEENGINE_H_
#define _PAOMIANTV_JNIMODULEENGINE_H_

#include "jnimodule.h"
#include "engine.h"

//field name
#define ENGINE_FIELD_NATIVE_ADDRESS_NAME "mNativeEngineAddress"
//field signiture
#define ENGINE_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv {

    class CJNIModuleEngine : public CJNIModule {
    private:
        CEngine *m_pEngine;
    public:

        static TJavaClazzParam *GetJavaClazzParam();

        static CJNIModuleEngine *CreateJniEngine(JNIEnv *env, jobject jengine);

        static CJNIModuleEngine *GetJniEngine(JNIEnv *env, jobject jengine);

        static void DestroyJniEngine(CJNIModuleEngine *&p);

        static bool IsValid(CJNIModuleEngine *p);

    private:

        CJNIModuleEngine(JNIEnv *env, jobject jEngine, jfieldID jfld);

        virtual ~CJNIModuleEngine();

        static jboolean jni_init(JNIEnv *env, jobject jengine);

        static void jni_uninit(JNIEnv *env, jobject jengine);

        static void jni_setDataSource(JNIEnv *env, jobject jengine, jobject jstoryboard);

        static void jni_start(JNIEnv *env, jobject jengine, jboolean jisSave);

        static void jni_seekTo(JNIEnv *env, jobject jengine, jint jclipIndex);

        static void jni_pause(JNIEnv *env, jobject jengine);

        static void jni_resume(JNIEnv *env, jobject jengine);

        static void jni_cancel(JNIEnv *env, jobject jengine);

    public:
        inline CEngine *getEngine();


    };

    inline CEngine *CJNIModuleEngine::getEngine() {
        return m_pEngine;
    }
}

#endif /* _PAOMIANTV_JNIMODULEENGINE_H_ */