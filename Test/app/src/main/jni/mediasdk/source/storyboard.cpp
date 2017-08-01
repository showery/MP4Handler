/*******************************************************************************
 *        Module: mediasdk
 *          File: storyboard.cpp
 * Functionality: storyboard entity.
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
#include <stdlib.h>
#include "constant.h"
#include "storyboard.h"
#include "autolog.h"

namespace paomiantv {
    CStoryboard::CStoryboard()
            : m_ptBGMClipParam(NULL),
              m_pchSrcBGM(NULL),
              m_sllBGMStartTm(0),
              m_sllBGMEndTm(-1),
              m_bIsStarted(FALSE),
              m_bIsStop(FALSE),
              m_bIsPaused(FALSE),
              m_bNewTask(FALSE) {
        USE_LOG;
        m_vClips.clear();
        m_pLock = new CLock;
        m_pThread = new CThread(ThreadWrapper, this);
        m_ptBGMClipParam = new TClipParam;
        m_pchDstPath = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_pchSrcBGM = (s8 *) malloc(MAX_LEN_FILE_PATH);
        memset(m_pchDstPath, 0, MAX_LEN_FILE_PATH);
        memset(m_pchSrcBGM, 0, MAX_LEN_FILE_PATH);
        prepare();
    }

    CStoryboard::~CStoryboard() {
        USE_LOG;
        stop();
        free(m_pchDstPath);
        free(m_pchSrcBGM);
        delete m_pLock;
        delete m_pThread;
        delete m_ptBGMClipParam;
        for (int i = 0; i < m_vClips.size(); i++) {
            if (m_vClips[i] != NULL) {
                delete m_vClips[i];
            }
        }
        m_vClips.clear();
    }

//static
    void *CStoryboard::ThreadWrapper(void *pThis) {
        CThread::SetName("CStoryboard");
        CStoryboard *p = (CStoryboard *) pThis;
        p->ThreadEntry();
        return (void *) p;
    }

    void CStoryboard::ThreadEntry() {
        m_pLock->lock();
        LOGI("storyboard process thread is started");
        m_bIsStarted = TRUE;
        while (!m_bIsStop) {

            while (!m_bIsStop && !m_bNewTask && m_bIsPaused) {
                m_pLock->wait();
            }
            if (!m_bIsStop) {
                m_pLock->unlock();
                handle();
                m_pLock->lock();
                m_bNewTask = FALSE;
            }
        }

        m_bIsStarted = FALSE;
        LOGI("storyboard process thread is stopped");
        m_pLock->unlock();
    }

    void CStoryboard::prepare() {
        LOGI("GMH264Decoder::start");
        int nErr;
        BEGIN_AUTOLOCK(m_pLock);

            if (m_bIsStarted) {
                return;
            }
            m_pThread->start();
            m_bIsStop = FALSE;
        END_AUTOLOCK;
    }

    void CStoryboard::stop() {
        BEGIN_AUTOLOCK(m_pLock);
            LOGI("GMH264Decoder::stop");

            if (m_bIsStarted && !m_bIsStop) {
                m_bIsStop = TRUE;
                m_pLock->acttive();
            }

        END_AUTOLOCK;
        m_pThread->join();
        m_bIsPaused = FALSE;
        m_bNewTask = FALSE;

    }

    BOOL32 CStoryboard::init(s8 *pchDstPath) {
        BEGIN_AUTOLOCK(m_pLock);
            strncpy(m_pchDstPath, pchDstPath, MAX_LEN_FILE_PATH);
        END_AUTOLOCK;
        return TRUE;
    }

    BOOL32 CStoryboard::uninit() {
        BEGIN_AUTOLOCK(m_pLock);
        END_AUTOLOCK;
        return TRUE;
    }


    BOOL32
    CStoryboard::setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm,
                        s64 sllEndTm) {
        BEGIN_AUTOLOCK(m_pLock);
            strncpy(m_pchSrcBGM, pchSrcpath, MAX_LEN_FILE_PATH);
            m_ptBGMClipParam->m_sllDuration = sllDurationCutTm;
            m_ptBGMClipParam->m_sllStart = sllStartCutTm;
            m_sllBGMStartTm = sllStartTm;
            m_sllBGMEndTm = sllEndTm;
        END_AUTOLOCK;
        return TRUE;
    }

    BOOL32 CStoryboard::addClip(CClip *pClip) {
        BEGIN_AUTOLOCK(m_pLock);
            m_vClips.push_back(pClip);
        END_AUTOLOCK;
        return TRUE;
    }

    BOOL32 CStoryboard::insertClip(s32 index, CClip *pClip) {
        BEGIN_AUTOLOCK(m_pLock);
            m_vClips.insert(m_vClips.begin() + index, pClip);
        END_AUTOLOCK;
        return TRUE;
    }

    CClip *CStoryboard::removeClip(s32 index) {
        CClip *pClip = NULL;
        BEGIN_AUTOLOCK(m_pLock);
            pClip = m_vClips.at(index);
            m_vClips.erase(m_vClips.begin() + index);
        END_AUTOLOCK;
        return pClip;
    }

    CClip *CStoryboard::getClip(s32 index) {
        CClip *pClip = NULL;
        BEGIN_AUTOLOCK(m_pLock);
            pClip = m_vClips.at(index);
        END_AUTOLOCK;
        return pClip;
    }

    BOOL32 CStoryboard::swapClip(s32 indexA, s32 indexB) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {

                CClip *clipA = m_vClips.at(indexA);
                if (clipA == NULL) {
                    break;
                }
                m_vClips[indexA] = m_vClips.at(indexB);
                if (m_vClips[indexA] == NULL) {
                    break;
                }
                m_vClips[indexB] = clipA;
                re = TRUE;

            } while (0);
        END_AUTOLOCK;
        return re;
    }

    void CStoryboard::resume() {
        BEGIN_AUTOLOCK(m_pLock);
            m_bIsPaused = FALSE;
            m_pLock->acttive();
        END_AUTOLOCK;
    }

    void CStoryboard::pause() {
        BEGIN_AUTOLOCK(m_pLock);
            m_bIsPaused = TRUE;
        END_AUTOLOCK;
    }


    void CStoryboard::handle() {

    }

    void CStoryboard::process() {

        BEGIN_AUTOLOCK(m_pLock);
            m_bNewTask = TRUE;
            m_bIsPaused = FALSE;
            m_pLock->acttive();
        END_AUTOLOCK;
        return;
    }

    void CStoryboard::cancel() {
        stop();
        return;
    }

    void CStoryboard::bindEvent(FailedCB cbOnFailed, SuccessCB cbOnSuccess, ProgressCB cbOnProgress,
                                AlwaysCB cbOnAlways, void *cbDelegate) {
        m_cbDelegate = cbDelegate;
        m_cbOnAlways = cbOnAlways;
        m_cbOnFailed = cbOnFailed;
        m_cbOnProgress = cbOnProgress;
        m_cbOnSuccess = cbOnSuccess;
    }

    s32 CStoryboard::getClipCount() {
        BEGIN_AUTOLOCK(m_pLock);
            return m_vClips.size();
        END_AUTOLOCK;
    }

} // namespace paomiantv
