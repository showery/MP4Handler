/*******************************************************************************
 *        Module: mediasdk
 *          File: engine.cpp
 * Functionality: engine entity.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-02  v1.0        huangxuefeng  created
 ******************************************************************************/

#include "engine.h"
#include "autolog.h"
#include "producer.h"
#include "vcodec.h"

extern int g_sdkVersion;

namespace paomiantv {

    CEngine::CEngine() : m_pProducer(NULL), m_pStoryboard(NULL) {
        USE_LOG;

    }

    CEngine::~CEngine() {
        USE_LOG;
        cancel();
    }

    BOOL32 CEngine::init(s32 nVersion) {
        MP4SetLogCallback(log_cb);
        g_sdkVersion = nVersion;
        return TRUE;
    }

    void CEngine::uninit() {
        MP4SetLogCallback(NULL);
    }

    void CEngine::log_cb(MP4LogLevel loglevel, const s8 *fmt, va_list ap) {
        switch (loglevel) {
            case MP4_LOG_NONE:
                break;
            case MP4_LOG_ERROR:
                VLOGE(fmt, ap);
                break;
            case MP4_LOG_WARNING:
                VLOGW(fmt, ap);
                break;
            case MP4_LOG_INFO:
                VLOGI(fmt, ap);
                break;
            default:
                VLOGI(fmt, ap);
                break;
        }
    }

    void CEngine::start(BOOL32 bIsSave) {
        if (m_pProducer != NULL) {
            m_pProducer->start(bIsSave);
        } else {
            LOGE("data source is null");
        }

    }

    void CEngine::resume() {
        if (m_pProducer != NULL) {
            m_pProducer->resume();
        }
    }

    void CEngine::pause() {
        if (m_pProducer != NULL) {
            m_pProducer->pause();
        }
    }

    void CEngine::cancel() {
        if (m_pProducer != NULL) {
            m_pProducer->stop();
            delete m_pProducer;
            m_pProducer = NULL;
        }
    }

    void CEngine::seekTo(s32 nClipIndex) {
        if (m_pProducer != NULL) {
            m_pProducer->seekTo(nClipIndex);
        }
    }

    void CEngine::setDataSource(CStoryboard *pStoryboard) {
        if (m_pProducer != NULL) {
            LOGW("the engine is started");
            m_pProducer->stop();
            delete m_pProducer;
            m_pProducer = NULL;
        }
        if (pStoryboard != NULL) {
            m_pStoryboard = pStoryboard;
            m_pProducer = new CProducer(pStoryboard);
        }
    }
}