/*******************************************************************************
 *        Module: interface
 *          File:
 * Functionality: define jni renderer module.
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

#ifndef _PAOMIANTV_JNIMODULERENDER_H_
#define _PAOMIANTV_JNIMODULERENDER_H_

#include "jnimodule.h"
#include "renderer.h"

//method name
#define RENDERER_METHOD_NEW_FRAME_NAME "fireOnNewFrame"
//method signiture
#define RENDERER_METHOD_NEW_FRAME_SIG "()V"

//field name
#define RENDERER_FIELD_NATIVE_ADDRESS_NAME "mNativeRendererAddress"
//field signiture
#define RENDERER_FIELD_NATIVE_ADDRESS_SIG "I"
namespace paomiantv {

    class CJNIModuleRenderer : public CJNIModule {

    private:
        CRenderer *m_pRenderer;

    public:
        static TJavaClazzParam *GetJavaClazzParam();

        static CJNIModuleRenderer *CreateJniRenderer(JNIEnv *env, jobject jRenderer);

        static CJNIModuleRenderer *GetJniRenderer(JNIEnv *env, jobject jRenderer);

        static void DestroyJniRenderer(CJNIModuleRenderer *&p);

        static bool IsValid(CJNIModuleRenderer *p);


    private:

        friend void JNIModuleRenderer_OnNewFrame(void *delegate);

        static void jni_Draw(JNIEnv *env, jobject thiz);

        static void jni_Init(JNIEnv *env, jobject thiz, jint nWidth, jint nHeight);

        static void jni_Create(JNIEnv *env, jobject thiz);

        static void jni_Destroy(JNIEnv *env, jobject thiz);

        static void jni_setDirection(JNIEnv *env, jobject thiz, jint nDirection);

        static jint jni_getDirection(JNIEnv *env, jobject thiz);

        static void jni_setRotate(JNIEnv *env, jobject thiz, jfloat fDegree);

        static jfloat jni_getRotate(JNIEnv *env, jobject thiz);

        static void jni_setTranslate(JNIEnv *env, jobject thiz, jfloat fTranslateX,
                                     jfloat fTranslateY, jfloat fTranslateZ);

        static jfloat jni_getTranslate(JNIEnv *env, jobject thiz, jint nTranslateEnum);

        static void jni_setScale(JNIEnv *env, jobject thiz, jfloat fScaleX, jfloat fScaleY,
                                 jfloat fScaleZ);

        static jfloat jni_getScale(JNIEnv *env, jobject thiz, jint nScaleEnum);

        static void jni_setFlip(JNIEnv *env, jobject thiz, jint nFlip);

        static jint jni_getFlip(JNIEnv *env, jobject thiz);

        static void jni_keepAspectRatio(JNIEnv *env, jobject thiz, jboolean bValue);

        static jint jni_getAspectState(JNIEnv *env, jobject thiz);

        static void jni_setAspectState(JNIEnv *env, jobject thiz, jint state);

        static void jni_refresh(JNIEnv *env, jobject thiz);

        static void jni_setFrame(JNIEnv *env, jobject thiz, jbyteArray jframeBuff,
                                 jint jframeWidth, jint jframeheight);

    public:
        void newFrameCallback();

        void Draw();

        void Init(int nWidth, int nHeight);

        inline void SetPause(BOOL32 isPause);

        inline bool GetPause();

        inline void SetRotate(float fDegree);

        inline float GetRotate();

        inline void SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ);

        inline float GetTranslate(int nTranslateEnum);

        inline void SetScale(float fScaleX, float fScaleY, float fScaleZ);

        inline float GetScale(int nScaleEnum);

        inline void SetDirection(int nDir);

        inline int GetDirection();

        inline void SetFlip(int nFlip);

        inline int GetFlip();

        inline bool IsEmptyFrame();

        inline CRenderer *getRenderer();

        void SetFrame(unsigned char *pbyData, int nWidth, int nHeight);

        void SetFrame(unsigned char *pbyData, int nWidth, int nHeight,
                      int nFrameDirection,      // recommendation for display orientation
                      bool bIsFrontCamera,      // whether is captured from front camera
                      unsigned long long uTimestamp); // the timestamp of capture this frame
        void SetChannel(int nChannel);

        /*!
         * \brief   set the aspect state which indicates how to keep aspect ratio.
         * \param   nState binary format:
         *           _0 - keep viewport dimension
         *           _1 - transpose viewport dimension
         *           0_ - keep frame dimension
         *           1_ - transpose frame dimension
         * \author  huangxuefeng
         * \date    2017-08-01
         */
        inline void SetAspectState(int nState);

        inline int GetAspectState();

        void Refresh();

        /*!
         * \brief   whether keep the aspect ratio or not.
         * \author  huang xuefeng
         * \date    2017-08-01
         */
        inline void KeepAspectRatio(EMAspectRatio mode);

        inline void QuerySettings(TRendererSettings *settings);

        inline void UpdateSettings(const TRendererSettings &settings);

    private:
        CJNIModuleRenderer(JNIEnv *env, jobject jRenderer,
                           jclass jcls, jfieldID jfld, jmethodID jmtd);

        ~CJNIModuleRenderer();

        JavaVM *m_jvm;
        jclass m_jcls;
        jfieldID m_jfld;
        jmethodID m_jmtd;
    };


/*
 * inline member functions implementation
 */
    inline CRenderer *CJNIModuleRenderer::getRenderer() {
        return m_pRenderer;
    }

    inline void CJNIModuleRenderer::SetPause(BOOL32 isPause) {
        m_pRenderer->SetPause(isPause);
    }

    inline bool CJNIModuleRenderer::GetPause() {
        return m_pRenderer->GetPause();
    }

    inline void CJNIModuleRenderer::SetRotate(float fDegree) {
        m_pRenderer->SetRotate(fDegree);
    }

    inline float CJNIModuleRenderer::GetRotate() {
        return m_pRenderer->GetRotate();
    }

    inline void
    CJNIModuleRenderer::SetTranslate(float fTranslateX, float fTranslateY, float fTranslateZ) {
        m_pRenderer->SetTranslate(fTranslateX, fTranslateY, fTranslateZ);
    }

    inline float CJNIModuleRenderer::GetTranslate(int nTranslateEnum) {
        return m_pRenderer->GetTranslate((EMDirect) nTranslateEnum);
    }

    inline void CJNIModuleRenderer::SetScale(float fScaleX, float fScaleY, float fScaleZ) {
        m_pRenderer->SetScale(fScaleX, fScaleY, fScaleZ);
    }

    inline float CJNIModuleRenderer::GetScale(int nScaleEnum) {
        return m_pRenderer->GetScale((EMDirect) nScaleEnum);
    }

    inline void CJNIModuleRenderer::SetDirection(int nDir) {
        m_pRenderer->SetDirection(nDir);
    }

    inline int CJNIModuleRenderer::GetDirection() {
        return m_pRenderer->GetDirection();
    }

    inline void CJNIModuleRenderer::SetFlip(int nFlip) {
        m_pRenderer->SetFlip(nFlip);
    }

    inline int CJNIModuleRenderer::GetFlip() {
        return m_pRenderer->GetFlip();
    }

// huangxuefeng 2017-08-01
    inline void CJNIModuleRenderer::KeepAspectRatio(EMAspectRatio mode) {
        m_pRenderer->KeepAspectRatio(mode);
    }

// huangxuefeng 2017-08-01
    inline void CJNIModuleRenderer::SetAspectState(int nState) {
        m_pRenderer->SetAspectState(nState);
    }

// huangxuefeng 2017-08-01
    inline int CJNIModuleRenderer::GetAspectState() {
        return m_pRenderer->GetAspectState();
    }

// huangxuefeng 2017-08-01
    inline void CJNIModuleRenderer::QuerySettings(TRendererSettings *settings) {
        m_pRenderer->QuerySettings(settings);
    }

// huangxuefeng 2017-08-01
    inline void CJNIModuleRenderer::UpdateSettings(const TRendererSettings &settings) {
        m_pRenderer->UpdateSettings(settings);
    }

// huangxuefeng 2017-08-01
    inline bool CJNIModuleRenderer::IsEmptyFrame() {
        return m_pRenderer->IsEmptyFrame();
    }
}

#endif //_PAOMIANTV_JNIMODULERENDER_H_
