//
// Created by John.Huang on 2017/8/1.
//

#include "constant.h"
#include "jnimodulemanager.h"
#include "jnicommon.h"
#include "jnimodulerenderer.h"

namespace paomiantv{

    void JNIModuleRenderer_OnNewFrame(void *delegate) {
        if (delegate != NULL) {
            ((CJNIModuleRenderer *) delegate)->newFrameCallback();
        }

    }

    TJavaClazzParam *CJNIModuleRenderer::GetJavaClazzParam() {
        TJavaClazzParam *ptJavaClazzParam = new TJavaClazzParam;
        JNINativeMethod arrMethods[] =
                {    {"keepAspectRatio", "(Z)V", (void*) jni_keepAspectRatio},
                     {"setDirection", "(I)V", (void*) jni_setDirection},
                     {"getDirection", "()I", (void*) jni_getDirection},
                     {"setFlip", "(I)V", (void*) jni_setFlip},
                     {"getFlip", "()I", (void*) jni_getFlip},
                     {"setScale", "(FFF)V", (void*) jni_setScale},
                     {"getScale", "(I)F", (void*) jni_getScale},
                     {"setTranslate", "(FFF)V", (void*) jni_setTranslate},
                     {"getTranslate", "(I)F", (void*) jni_getTranslate},
                     {"setRotate", "(F)V", (void*) jni_setRotate},
                     {"getRotate", "()F", (void*) jni_getRotate},
                     {"getAspectState", "()I", (void*)jni_getAspectState},
                     {"setAspectState", "(I)V", (void*)jni_setAspectState},
                     {"setFrame", "([BII)V", (void*)jni_setFrame},
                     {"refresh", "()V", (void*)jni_refresh},
                     {"_draw", "()V", (void*) jni_Draw},
                     {"_init", "(II)V", (void*) jni_Init},
                     {"_create", "()V", (void*) jni_Create},
                     {"_destroy", "()V", (void*) jni_Destroy},};
        ptJavaClazzParam->m_nMtdCount = NELEM(arrMethods);
        sprintf(ptJavaClazzParam->m_pchClazzName, "cn/paomiantv/render/PMRenderer%s", "");
        ptJavaClazzParam->m_ptMethods = (JNINativeMethod *) malloc(sizeof(arrMethods));
        memcpy(ptJavaClazzParam->m_ptMethods, arrMethods, sizeof(arrMethods));
        return ptJavaClazzParam;
    }


    CJNIModuleRenderer::CJNIModuleRenderer(JNIEnv *env, jobject jRenderer,
                               jclass jcls, jfieldID jfld, jmethodID jmtd)
            : m_jvm(NULL),
              m_pRenderer(NULL),
              m_jcls(NULL),
              m_jfld(jfld),
              m_jmtd(jmtd)
    {
        USE_LOG;

        if (env == NULL || jRenderer == NULL
            || jcls == NULL || jfld == NULL || jmtd == NULL)
        {
            LOGE("invalid parameters");
            return;
        }

        if (env->GetJavaVM(&m_jvm) != 0)
        {
            LOGE("get java vm failed");
            return;
        }

        m_jObject = env->NewGlobalRef(jRenderer);
        m_jcls = (jclass)env->NewGlobalRef(jcls);

        m_pRenderer = new CRenderer;
        if (m_pRenderer != NULL)
        {
            LOGD("renderer instance allocated: %u", sizeof(CRenderer));
            m_pRenderer->bindEvent(JNIModuleRenderer_OnNewFrame,this);
        }
        // only register valid ones
        CJNIModuleManger::getInstance()->add(this);
    }

// FIXME: when exit the application, it will sometimes prints this error log:
// E/libEGL(19049): call to OpenGL ES API with no current context (logged once per thread)
// -- noted by huangxuefeng, 2013-07-29
    CJNIModuleRenderer::~CJNIModuleRenderer()
    {
        USE_LOG;

        if (m_pRenderer != NULL)
        {
            m_pRenderer->Destroy();
            delete m_pRenderer;
            m_pRenderer = NULL;
            LOGD("renderer instance freed: %u", sizeof(CRenderer));
        }

        // be sure unregister before killing
        CJNIModuleManger::getInstance()->remove(this);
    }

    bool CJNIModuleRenderer::IsValid(CJNIModuleRenderer *p)
    {
        // checks residency validity
        return CJNIModuleManger::getInstance()->contains(p);
    }

    CJNIModuleRenderer * CJNIModuleRenderer::CreateJniRenderer(JNIEnv *env, jobject jRenderer)
    {
        USE_LOG;
        CJNIModuleRenderer *ret = NULL;
        env->PushLocalFrame(10);
        do
        {
            jclass jcls = env->GetObjectClass(jRenderer);
            if (jcls == NULL)
            {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, RENDERER_FIELD_NATIVE_ADDRESS_NAME, RENDERER_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL)
            {
                LOGE("java field '%s' is not defined", RENDERER_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jmethodID jmtd = env->GetMethodID(jcls, RENDERER_METHOD_NEW_FRAME_NAME, RENDERER_METHOD_NEW_FRAME_SIG);
            if (jmtd == NULL)
            {
                LOGE("java method 'void %s()' is not defined", RENDERER_METHOD_NEW_FRAME_NAME);
                break;
            }

            jint nValue = env->GetIntField(jRenderer, jfld);
            if (nValue != 0 && CJNIModuleManger::getInstance()->contains((void*)nValue))
            {
                LOGI("the CJNIModuleRenderer is already created");
                ret = (CJNIModuleRenderer*)nValue;
                break;
            }

            CJNIModuleRenderer * pNew = new CJNIModuleRenderer(env, jRenderer, jcls, jfld, jmtd);
            if (!CJNIModuleManger::getInstance()->contains(pNew))
            {
                LOGE("create CJNIModuleRenderer failed");
                delete pNew;
                ret = NULL;
                break;
            }
            else
            {
                LOGD("CJNIModuleRenderer instance allocated: %u", sizeof(CJNIModuleRenderer));
            }

            env->SetIntField(jRenderer, jfld, (jint)pNew);
            ret = pNew;

            LOGI("create CJNIModuleRenderer success");
        }while(0);
        env->PopLocalFrame(NULL);

        return ret;
    }

    void CJNIModuleRenderer::DestroyJniRenderer(CJNIModuleRenderer* &p)
    {
        USE_LOG;

        if (p == NULL || !CJNIModuleManger::getInstance()->contains(p))
        {
            LOGE("invalid parameters");
            return;
        }

        JNIEnv *env = NULL;
        if (p->m_jvm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        {
            LOGE("get JNIEnv failed");
            return;
        }
        env->PushLocalFrame(10);
        do
        {
            jint nValue = env->GetIntField(p->m_jObject, p->m_jfld);
            if (nValue != (jint)p)
            {
                LOGE("java field is changed improperly!");
            }

            env->SetIntField(p->m_jObject, p->m_jfld, 0);
            delete p;
            LOGD("CJNIModuleRenderer instance freed: %u", sizeof(CJNIModuleRenderer));

            LOGI("destroy CJNIModuleRenderer ok");
        }while(0);
        env->PopLocalFrame(NULL);
        p = NULL;
    }

    CJNIModuleRenderer * CJNIModuleRenderer::GetJniRenderer(JNIEnv *env, jobject jRenderer)
    {
        CJNIModuleRenderer* ret = NULL;
        env->PushLocalFrame(10);
        do
        {
            if (env == NULL || jRenderer == NULL)
            {
                LOGE("invalid parameters");
                break;
            }

            jclass jcls = env->GetObjectClass(jRenderer);
            if (jcls == NULL)
            {
                LOGE("get java class failed");
                break;
            }

            jfieldID jfld = env->GetFieldID(jcls, RENDERER_FIELD_NATIVE_ADDRESS_NAME, RENDERER_FIELD_NATIVE_ADDRESS_SIG);
            if (jfld == NULL)
            {
                LOGE("java java field '%s' failed", RENDERER_FIELD_NATIVE_ADDRESS_NAME);
                break;
            }

            jint nValue = env->GetIntField(jRenderer, jfld);
            if (nValue == 0
                || !CJNIModuleManger::getInstance()->contains((CJNIModuleRenderer*)nValue))
            {
                LOGE("get jni renderer from java object failed");
                break;
//                LOGI("try to get a new jni renderer");
//                ret = CreateJniRenderer(env, jRenderer);
            }
            else
            {
                ret = (CJNIModuleRenderer*)nValue;
            }

        }while(0);

        env->PopLocalFrame(NULL);
        return ret;
    }


    void CJNIModuleRenderer::SetFrame(unsigned char *pbyData, int nWidth, int nHeight)
    {
        if(pbyData == NULL || nWidth <=0 || nHeight <= 0)
        {
            LOGE("SetFrame Frame is invalid.");
            return;
        }
        if(m_pRenderer->GetPause())
        {
            LOGE("Paused,do not Render.");
            return;
        }


        m_pRenderer->SetFrame(pbyData,nWidth,nHeight);

//    // save one remote frame for test
//    {
//        static int nFrameIndex = 0;
//        if (nFrameIndex % 10 == 0)
//        {
//            char strPath[128];
//            sprintf(strPath, "/sdcard/DCIM/r_%dx%d.yuv", nWidth, nHeight);
//            CAutoFile fileYuv(strPath, "wa");
//            fileYuv.write(pbyData, nWidth * nHeight * 1.5);
//            LOGD("saved file %s", strPath);
//        }
//        ++ nFrameIndex;
//    }
    }

    void CJNIModuleRenderer::SetFrame(unsigned char *pbyData, int nWidth, int nHeight,
                                int nFrameDirection, bool bIsFrontCamera,
                                unsigned long long uTimestamp)
    {
        if(pbyData == NULL || nWidth <=0 || nHeight <= 0)
        {
            LOGE("SetFrame Frame is invalid.");
            return;
        }
        if(m_pRenderer->GetPause())
        {
            LOGE("Paused,do not Render.");
            return;
        }

        m_pRenderer->SetFrame(pbyData,nWidth,nHeight,uTimestamp, nFrameDirection, bIsFrontCamera);

    }

    void CJNIModuleRenderer::Draw()
    {
        m_pRenderer->Draw();
    }

    void CJNIModuleRenderer::Init(int nWidth, int nHeight)
    {
        m_pRenderer->Init(nWidth, nHeight);
    }

    void CJNIModuleRenderer::SetChannel(int nChannel)
    {
        USE_LOG;
        LOGD("channel#%d", nChannel);
/*
    CVidGlobalData *pVidShared = CVidGlobalData::Instance();
    CJniRenderer *pChangedOtherRenderer = NULL;
    BEGIN_AUTOLOCK(&pVidShared->m_DecPlylock);
    if (pVidShared->m_pRenderer[nChannel] == this)
    {
        return;
    }

    for (int i=0; i<kRendererMax; ++i)
    {
        if (i == nChannel)
        {
            // change channel
            CJniRenderer *pRenderer = (CJniRenderer*)pVidShared->m_pRenderer[i];
            if (pRenderer != this)
            {
                // before apply change, swap frame at first.
                if (CJniRenderer::IsValid(pRenderer))
                {
                    LOGD("swap channel frames.");
                    m_pRenderer->SwapFrame(*pRenderer->m_pRenderer);
                    pChangedOtherRenderer = pRenderer;
                }
                pVidShared->m_pRenderer[i] = this;
            }
        }
        else if (this == pVidShared->m_pRenderer[i])
        {
            pVidShared->m_pRenderer[i] = NULL;
        }
    }
    m_pRenderer->SetChannel(nChannel);

    // configuration
    // TODO (huangxuefeng 2013-12-16) configure channel-buffers better
    TRendererSettings tSettings;
    m_pRenderer->QuerySettings(&tSettings);
    TVidFrame tFrame = {0};
    switch (nChannel)
    {
    case kRendererAux:
        tSettings.kAspectRatio = kAspRatPadding;
        tSettings.nFlip = 0;
        tFrame.pbyData = pVidShared->m_tVidDecSecond.picture_bufsrc;
        tFrame.nWidth  = pVidShared->m_tVidDecSecond.sourceWidth;
        tFrame.nHeight = pVidShared->m_tVidDecSecond.sourceHeight;
        m_pRenderer->SetFrame(tFrame);
        m_bNewFrame = TRUE;
        break;
    case kRendererMain:
        tSettings.kAspectRatio = kAspRatPadding;
        tSettings.nFlip = 0;
        tFrame.pbyData = pVidShared->m_tVidDecFirst.picture_bufsrc;
        tFrame.nWidth  = pVidShared->m_tVidDecFirst.sourceWidth;
        tFrame.nHeight = pVidShared->m_tVidDecFirst.sourceHeight;
        m_pRenderer->SetFrame(tFrame);
        m_bNewFrame = TRUE;
        break;
    case kRendererPreview:
        tSettings.kAspectRatio = kAspRatTrim;
        tSettings.nFlip = 1;
        break;
    }
    LOGV("frame-data[0]:%d", (tFrame.pbyData?tFrame.pbyData[0]:1));
    m_pRenderer->UpdateSettings(tSettings);

    END_AUTOLOCK;

    Refresh();
    if (pChangedOtherRenderer)
    {
        pChangedOtherRenderer->Refresh();
    }
    */
    }

    void CJNIModuleRenderer::Refresh()
    {
        m_pRenderer->refresh();
    }

    void CJNIModuleRenderer::jni_Draw(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer* pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return;
        }
        pJNIRenderer->Draw();
    }

    void CJNIModuleRenderer::jni_Init(JNIEnv *env, jobject thiz, jint nWidth, jint nHeight)
    {
        USE_LOG;
        LOGV("%d x %d", nWidth, nHeight);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return;
        }
        pJNIRenderer->Init(nWidth, nHeight);
    }

    void CJNIModuleRenderer::jni_Create(JNIEnv *env, jobject thiz)
    {
        USE_LOG;
        CJNIModuleRenderer::CreateJniRenderer(env, thiz);
    }

    void CJNIModuleRenderer::jni_Destroy(JNIEnv *env, jobject thiz)
    {
        USE_LOG;
        CJNIModuleRenderer * pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            LOGE("jni renderer is null");
            return;
        }
        CJNIModuleRenderer::DestroyJniRenderer(pJNIRenderer);
    }

    void CJNIModuleRenderer::jni_setDirection(JNIEnv *env, jobject thiz, jint nDirection)
    {
        LOGV("direction = %d", nDirection);
        if (nDirection >= 0 && nDirection < 4)
        {
            CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
            if (pJNIRenderer == NULL)
            {
                return ;
            }
            return pJNIRenderer->SetDirection(nDirection);
        }
    }

    jint CJNIModuleRenderer::jni_getDirection(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return 0;
        }
        return pJNIRenderer->GetDirection();
    }

    void CJNIModuleRenderer::jni_setRotate(JNIEnv *env, jobject thiz, jfloat fDegree)
    {
        LOGV("rotate: degree = %f", fDegree);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return ;
        }
        return pJNIRenderer->SetRotate(fDegree);
    }

    jfloat CJNIModuleRenderer::jni_getRotate(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);

        if (pJNIRenderer == NULL)
        {
            return 0;
        }
        return pJNIRenderer->GetRotate();

    }

    void CJNIModuleRenderer::jni_setTranslate(JNIEnv *env, jobject thiz, jfloat fTranslateX, jfloat fTranslateY, jfloat fTranslateZ)
    {
        LOGV("translate: x,y,z = %f,%f,%f,", fTranslateX, fTranslateY, fTranslateZ);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return ;
        }
        return pJNIRenderer->SetTranslate(fTranslateX, fTranslateY, fTranslateZ);
    }

    jfloat CJNIModuleRenderer::jni_getTranslate(JNIEnv *env, jobject thiz, jint nTranslateEnum)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if(nTranslateEnum >= kDirectX && nTranslateEnum <= kDirectZ)
        {
            if (pJNIRenderer == NULL)
            {
                return 0;
            }
            return pJNIRenderer->GetTranslate(nTranslateEnum);
        }
        return 0;

    }

    void CJNIModuleRenderer::jni_setScale(JNIEnv *env, jobject thiz, jfloat fScaleX, jfloat fScaleY, jfloat fScaleZ)
    {
        LOGV("scale: x,y,z = %f,%f,%f,", fScaleX, fScaleY, fScaleZ);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            LOGE("player is null!");
            return ;
        }
        return pJNIRenderer->SetScale(fScaleX, fScaleY, fScaleZ);

    }

    jfloat CJNIModuleRenderer::jni_getScale(JNIEnv *env, jobject thiz, jint nScaleEnum)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if(nScaleEnum >= kDirectX && nScaleEnum <= kDirectZ)
        {
            if (pJNIRenderer == NULL)
            {
                return 0;
            }
            return pJNIRenderer->GetScale(nScaleEnum);
        }
        return 0;
    }

    void CJNIModuleRenderer::jni_setFlip(JNIEnv *env, jobject thiz, jint nFlip)
    {
        LOGV("flip = %d", nFlip);
        if (nFlip >= 0 && nFlip < 4)
        {
            CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
            if (pJNIRenderer == NULL)
            {
                return ;
            }
            return pJNIRenderer->SetFlip(nFlip);
        }
    }

    jint CJNIModuleRenderer::jni_getFlip(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return 0;
        }
        return pJNIRenderer->GetFlip();
    }

    void CJNIModuleRenderer::jni_keepAspectRatio(JNIEnv *env, jobject thiz, jboolean bValue)
    {
        LOGV("keep aspect: %d", bValue);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return ;
        }
        if (bValue)
        {
            return pJNIRenderer->KeepAspectRatio(kAspRatTrim);
        }
        else
        {
            return pJNIRenderer->KeepAspectRatio(kAspRatStretch);
        }
    }

    jint CJNIModuleRenderer::jni_getAspectState(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return 0;
        }
        return pJNIRenderer->GetAspectState();
    }

    void CJNIModuleRenderer::jni_setAspectState(JNIEnv *env, jobject thiz, jint state)
    {
        LOGV("aspect state: %d", state);
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return;
        }
        return pJNIRenderer->SetAspectState(state);
    }

    void CJNIModuleRenderer::jni_refresh(JNIEnv *env, jobject thiz)
    {
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL)
        {
            return;
        }
        return pJNIRenderer->Refresh();
    }

    void CJNIModuleRenderer::jni_setFrame(JNIEnv *env, jobject thiz, jbyteArray jframeBuff, jint jframeWidth, jint jframeheight)
    {
        USE_LOG;
        CJNIModuleRenderer *pJNIRenderer = CJNIModuleRenderer::GetJniRenderer(env, thiz);
        if (pJNIRenderer == NULL || !CJNIModuleRenderer::IsValid(pJNIRenderer))
        {
            return;
        }
        jbyte* frameBuff = env->GetByteArrayElements(jframeBuff,0);
        if(frameBuff == NULL || jframeheight == 0 || jframeWidth ==0)
        {
            return;
        }
        pJNIRenderer->SetFrame((unsigned char *) frameBuff,jframeWidth,jframeheight);
        env->ReleaseByteArrayElements(jframeBuff,frameBuff,0);
        return;
    }

    void CJNIModuleRenderer::newFrameCallback() {
        if (!m_jvm || !m_jObject || !m_jmtd)
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
        env->CallVoidMethod(m_jObject, m_jmtd);
        env->PopLocalFrame(NULL);

        if (isAttacked) {
            m_jvm->DetachCurrentThread();
        }
    }
}