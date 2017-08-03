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

#include "controller.h"

namespace paomiantv {

    CController::CController()
            :m_bNewFrame(FALSE),
             m_bIsStarted(FALSE),
             m_bIsStop(FALSE),
             m_bIsNeedPreview(FALSE),
             m_pThread(NULL)
    {
        init();
    }

    CController::~CController() {
        uninit();
    }

    void CController::init() {
        m_pLock = new CLock;
    }

    void CController::uninit() {
        delete m_pLock;
        m_pLock =NULL;
    }

    void CController::start(CStoryboard *pStoryboard) {
        TThreadData *data = new TThreadData;
        data->holder = this;
        data->data = pStoryboard;
        m_pThread = new CThread(ThreadWrapper, data);
        m_pThread->start();
    }

//static
    void *CController::ThreadWrapper(void *pData) {
        CThread::SetName("CStoryboard");
        TThreadData *p = (TThreadData *) pData;
        ((CController *) (p->holder))->ThreadEntry((CStoryboard *)p->data);
        return (void *) pData;
    }

    void CController::ThreadEntry(CStoryboard *pStoryboard) {
        m_pLock->lock();
        LOGI("storyboard process thread is started");
        m_bIsStarted = TRUE;
        while (!m_bIsStop) {

            while (!m_bIsStop && !m_bNewFrame) {
                m_pLock->wait();
            }
            if (!m_bIsStop) {
                m_pLock->unlock();
                handle(pStoryboard);
                m_pLock->lock();
                m_bNewFrame = FALSE;
            }
        }

        m_bIsStarted = FALSE;
        LOGI("storyboard process thread is stopped");
        m_pLock->unlock();
    }

    void CController::cancel() {

        BEGIN_AUTOLOCK(m_pLock);
            LOGI("GMH264Decoder::stop");

            if (m_bIsStarted && !m_bIsStop) {
                m_bIsStop = TRUE;
                m_pLock->acttive();
            }

        END_AUTOLOCK;
        TThreadData *p = (TThreadData *) m_pThread->join();
        delete p;
        delete m_pThread;
    }

    void CController::startPreview(CStoryboard *pStoryboard) {

    }

    void CController::pausePreview() {

    }

    void CController::resumePreview() {

    }

    void CController::stopPreview() {

    }

    void CController::resume() {

    }

    void CController::pause() {

    }

}