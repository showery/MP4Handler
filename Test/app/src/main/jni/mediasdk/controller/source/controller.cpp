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

#include <typedef.h>
#include "controller.h"

namespace paomiantv {

    CController::CController(CStoryboard *pStoryboard, BOOL32 bIsSave)
            : m_bIsSave(bIsSave),
              m_pStoryboard(pStoryboard),
              m_bIsStopped(FALSE),
              m_bIsPaused(FALSE),
              m_nPreviewFromClipIndex(0),
              m_nPreviewToClipIndex(-1) {
        USE_LOG;
        m_pLock = new CLock;
        m_pThread = new CThread(ThreadWrapper, this);
    }

    CController::~CController() {
        USE_LOG;
        if (m_pLock != NULL) {
            m_pLock = new CLock;
            m_pLock = NULL;
        }
        if (m_pThread != NULL) {
            delete m_pThread;
            m_pThread = NULL;
        }

    }

//static
    void *CController::ThreadWrapper(void *pThis) {
        CThread::SetName(typeid(*pThis).name());
        CController *p = (CController *) pThis;
        int nErr = p->run();
        return (void *) nErr;
    }


    void CController::start(BOOL32 bIsSave) {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted) {
                return;
            }
            m_pThread->start();
            m_bIsSave = bIsSave;
            m_bIsStopped = FALSE;

        END_AUTOLOCK;
    }

    void CController::stop() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped) {
                m_bIsStopped = TRUE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
        m_pThread->join();
    }

    void CController::resume() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && m_bIsPaused) {
                m_bIsPaused = FALSE;
                m_pLock->acttive();
            }
        END_AUTOLOCK;
    }

    void CController::pause() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsStopped && !m_bIsPaused) {
                m_bIsPaused = TRUE;
            }

        END_AUTOLOCK;
    }

    void CController::seekTo(s32 nClipIndex) {
        m_pStoryboard->seekTo(nClipIndex);
    }

}