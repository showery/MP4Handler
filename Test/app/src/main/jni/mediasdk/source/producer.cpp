/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: producer
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-03  v1.0        huangxuefeng  created
 ******************************************************************************/

#include "typedef.h"
#include "producer.h"

namespace paomiantv {

    CProducer::CProducer(CStoryboard *pStoryboard, BOOL32 bIsSave)
            : m_pStoryboard(pStoryboard),
              m_bIsStarted(FALSE),
              m_bIsPaused(FALSE),
              m_bIsSave(bIsSave) {
        m_pLock = new CLock;
        m_pVController = new CVController(pStoryboard, bIsSave);
        m_pAController = new CAController(pStoryboard, bIsSave);
    }

    CProducer::~CProducer() {
        if (m_pVController != NULL) {
            delete m_pVController;
            m_pVController = NULL;
        }
        if (m_pAController != NULL) {
            delete m_pAController;
            m_pAController = NULL;
        }
        if (m_pLock != NULL) {
            delete m_pLock;
            m_pLock = NULL;
        }
    }

    void CProducer::start(BOOL32 bIsSave) {
        BEGIN_AUTOLOCK(m_pLock);
            if (!m_bIsStarted) {
                m_pAController->start(bIsSave);
                m_pVController->start(bIsSave);
                m_bIsStarted = TRUE;
                m_bIsPaused = FALSE;
            }
        END_AUTOLOCK;
    }

    void CProducer::stop() {
        BEGIN_AUTOLOCK(m_pLock);
            if (!m_bIsStarted) {
                m_pAController->stop();
                m_pVController->stop();
                m_bIsStarted = FALSE;
                m_bIsPaused = FALSE;
            }
        END_AUTOLOCK;
    }

    void CProducer::resume() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && m_bIsPaused) {

                m_bIsPaused = FALSE;
                m_pAController->resume();
                m_pVController->resume();
            }
        END_AUTOLOCK;
    }

    void CProducer::pause() {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsStarted && !m_bIsPaused) {
                m_bIsPaused = TRUE;
                m_pAController->pause();
                m_pVController->pause();
            }
        END_AUTOLOCK;
    }

    void CProducer::seekTo(s64 sllPosition) {
        BEGIN_AUTOLOCK(m_pLock);
            if (m_bIsSave) {
                m_pAController->seekTo(sllPosition);
                m_pVController->seekTo(sllPosition);
            }
        END_AUTOLOCK;
    }
}