/*******************************************************************************
 *        Module: mediasdk
 *          File: acontroller.cpp
 * Functionality: handle audio data.
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

#include "acontroller.h"
#include "aprocessor.h"
#include "clipparser.h"
#include "constant.h"

namespace paomiantv {
    CAController::CAController(CStoryboard *pStoryboard, BOOL32 bIsSave)
            : CController(pStoryboard, bIsSave) {
        USE_LOG;
        m_pProcessor = new CAProcessor;
        m_pbyABuf = (u8 *) malloc(MAX_AUDIO_FRAME_BUFFER_SIZE);
    }

    CAController::~CAController() {
        USE_LOG;
        if (m_pProcessor != NULL) {
            delete m_pProcessor;
            m_pProcessor = NULL;
        }
        if (m_pbyABuf != NULL) {
            free(m_pbyABuf);
            m_pbyABuf = NULL;
        }
    }

    int CAController::run() {
        m_pLock->lock();
        LOGI("audio controller is started");
        m_bIsStarted = TRUE;
        while (!m_bIsStopped) {
            while (!m_bIsStopped && m_bIsPaused) {
                m_pLock->wait();
            }
            if (!m_bIsStopped) {
                m_pLock->unlock();
                BOOL32 bIsLastSample = FALSE;
                BOOL bIsSync = FALSE;
                u32 uSize = 0;
                u64 ullStartTm = 0;
                u64 ullDuration = 0;
                u64 ullRenderOffset = 0;
                m_pStoryboard->getNextASpample(bIsLastSample, m_pbyABuf, uSize, ullStartTm,
                                               ullDuration, ullRenderOffset, bIsSync);
                //decode

                //transform

                if (m_bIsSave) {
                    //encode
                } else {
                    //render
                }
                m_pLock->lock();
            }
        }
        m_bIsStarted = FALSE;
        LOGI("audio controller is stopped");
        m_pLock->unlock();
        return 0;
    }
/*
    void CAController::start(BOOL32 bIsSave) {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted) {
                return;
            }
            m_pThread->start();
            m_bIsSave = bIsSave;
            m_bIsStopped = FALSE;

        END_AUTOLOCK;
    }

    void CAController::stop() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped) {
                m_bIsStopped = TRUE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
        m_pThread->join();
    }

    void CAController::resume() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && m_bIsPaused) {
                m_bIsPaused = FALSE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
    }

    void CAController::pause() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && !m_bIsPaused) {
                m_bIsPaused = TRUE;
            }

        END_AUTOLOCK;
    }

    void CAController::seekTo(s32 nClipIndex) {
        m_pStoryboard->seekTo(nClipIndex);
    }
*/
    BOOL32 CAController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam) {
        return TRUE;
    }

    void CAController::handle(CStoryboard *pStoryboard) {
    }
}