//
// Created by John.Huang on 2017/7/31.
//

#ifndef _PAOMIANTV_JNIMODULETRANSITION_H
#define _PAOMIANTV_JNIMODULETRANSITION_H

#include "jnimodule.h"
#include "transition.h"

//field name
#define TRANSITION_FIELD_NATIVE_ADDRESS_NAME "mNativeTransitionAddress"
//field signiture
#define TRANSITION_FIELD_NATIVE_ADDRESS_SIG "I"

namespace paomiantv {
    class CJNIModuleTransition : public CJNIModule {

    private:
        CTransition *m_pTransition;

    public:

        static TJavaClazzParam *GetJavaClazzParam();

        static CJNIModuleTransition *CreateJniTransition(JNIEnv *env, jobject jtransition);

        static CJNIModuleTransition *GetJniTransition(JNIEnv *env, jobject jtransition);

        static void DestroyJniTransition(CJNIModuleTransition *&p);

        inline CTransition *getTransition();

    private:
        CJNIModuleTransition(JNIEnv *env, jobject jtransition, jfieldID jfld);

        virtual ~CJNIModuleTransition();



        static bool IsValid(CJNIModuleTransition *p);

        static jboolean
        jni_init(JNIEnv *env, jobject jtransition, jstring jsrc, jlong jstart, jlong jduration);

        static jboolean jni_uninit(JNIEnv *env, jobject jtransition);

        static jstring jni_getSrc(JNIEnv *env, jobject jtransition);

        static void jni_setSrc(JNIEnv *env, jobject jtransition, jstring jsrc);

        static jlong jni_getStart(JNIEnv *env, jobject jtransition);

        static void jni_setStart(JNIEnv *env, jobject jtransition, jlong jstart);

        static jlong jni_getDuration(JNIEnv *env, jobject jtransition);

        static void jni_setDuration(JNIEnv *env, jobject jtransition, jlong jduration);
    };

    inline CTransition *CJNIModuleTransition::getTransition() {
        return m_pTransition;
    }
}

#endif //_PAOMIANTV_JNIMODULETRANSITION_H
