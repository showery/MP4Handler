/*******************************************************************************
 *        Module: paomiantv
 *          File: vcontroller.cpp
 * Functionality: handle video data.
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
#include "vcontroller.h"
#include "vprocessor.h"
#include "constant.h"
#include "frame.h"

namespace paomiantv {

    CVController::CVController(CStoryboard *pStoryboard, BOOL32 bIsSave)
            : CController(pStoryboard, bIsSave) {
        USE_LOG;
        m_pH264Dec = new CH264Dec(pStoryboard,bIsSave);
    }

    CVController::~CVController() {
        USE_LOG;
        stop();
        if (m_pH264Dec != NULL) {
            delete m_pH264Dec;
            m_pH264Dec = NULL;
        }
    }

    int CVController::run() {
        m_pLock->lock();
        LOGI("video controller is started");
        m_bIsStarted = TRUE;
        while (!m_bIsStopped) {
            while (!m_bIsStopped && m_bIsPaused) {
                m_pLock->wait();
            }
            if (!m_bIsStopped) {
                m_pLock->unlock();
                TFrame* ptFrame = new TFrame;
                m_pStoryboard->getNextVSpample(ptFrame->isLast, ptFrame->data, ptFrame->size, ptFrame->startTm,
                                               ptFrame->duration, ptFrame->renderOffset, ptFrame->isSync,ptFrame->isSPS,ptFrame->isPPS);
                //decode
                m_pH264Dec->addToQueue(ptFrame);
                m_pLock->lock();
            }
        }
        m_bIsStarted = FALSE;
        LOGI("video controller is stopped");
        m_pLock->unlock();
        return 0;
    }
/*
    void CVController::start(BOOL32 bIsSave) {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted) {
                return;
            }
            m_pThread->start();
            m_bIsSave = bIsSave;
            m_bIsStopped = FALSE;

        END_AUTOLOCK;
    }

    void CVController::stop() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped) {
                m_bIsStopped = TRUE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
        m_pThread->join();
    }

    void CVController::resume() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && m_bIsPaused) {
                m_bIsPaused = FALSE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
    }

    void CVController::pause() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && !m_bIsPaused) {
                m_bIsPaused = TRUE;
            }

        END_AUTOLOCK;
    }

    void CVController::seekTo(s32 nClipIndex) {
        m_pStoryboard->seekTo(nClipIndex);
    }
*/
    BOOL32 CVController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam) {
        return TRUE;
    }

    void CVController::handle(CStoryboard *pStoryboard) {
    }
}