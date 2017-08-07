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

namespace paomiantv {

    CEngine::CEngine() {
        USE_LOG;
    }

    CEngine::~CEngine() {
        USE_LOG;
    }

    BOOL32 CEngine::init() {
        MP4SetLogCallback(log_cb);
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
        if (m_pStoryboard != NULL || m_pProducer != NULL) {
            m_pProducer->start(bIsSave);
        }

    }

    void CEngine::resume() {
        if (m_pStoryboard != NULL || m_pProducer != NULL) {
            m_pProducer->resume();
        }
    }

    void CEngine::pause() {
        if (m_pStoryboard != NULL || m_pProducer != NULL) {
            m_pProducer->pause();
        }
    }

    void CEngine::cancel() {
        if (m_pStoryboard != NULL || m_pProducer != NULL) {
            m_pProducer->stop();
            delete m_pProducer;
            m_pProducer = NULL;
        }
    }

    void CEngine::seekTo(s32 nClipIndex) {
        if (m_pStoryboard != NULL || m_pProducer != NULL) {
            m_pProducer->seekTo(nClipIndex);
        }
    }

    void CEngine::setDataSource(CStoryboard *pStoryboard) {
        if (pStoryboard != NULL) {
            m_pStoryboard = pStoryboard;
            m_pProducer = new CProducer(pStoryboard);
        }

    }
}