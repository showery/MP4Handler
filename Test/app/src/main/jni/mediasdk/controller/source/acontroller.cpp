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

namespace paomiantv
{
CAController::CAController(CStoryboard *pStoryboard, BOOL32 bIsSave)
    : CController(pStoryboard, bIsSave)
{
    USE_LOG;
    m_pProcessor = new CAProcessor;
}

CAController::~CAController()
{
    USE_LOG;
    if (m_pProcessor != NULL)
    {
        delete m_pProcessor;
        m_pProcessor = NULL;
    }
}

int CAController::run()
{
    BEGIN_AUTOLOCK(m_pLock);
    LOGI("audio controller is started");
    m_bIsStarted = TRUE;
    s32 nClipNum = m_pStoryboard->getClipCount();
    s32 nSampleNum = 0;
    for (s32 i = 0; i < nClipNum; i++)
    {
        nSampleNum += m_pStoryboard->getClip(i)->getParser()->getASampleNum();
    }
    s32 nCount = 0;

    while (nCount < nSampleNum && !m_bIsStoped)
    {
        s32 nClipIndex = 0;
        while (nClipIndex < nClipNum && !m_bIsStoped)
        {
            s32 nClipSampleNum = m_pStoryboard->getClip(nClipIndex)->getParser()->getASampleNum();
            s32 nClipSamlpeIndex = 1;
            while (nClipSamlpeIndex <= nClipSampleNum && !m_bIsStoped)
            {
                if (m_bIsPaused)
                {
                    m_pLock->wait();
                }

                //decode

                //transform

                if (m_bIsSave)
                {
                    //encode
                }
                else
                {
                    //render
                }
                nCount++;
                nClipSamlpeIndex++;
                m_pStoryboard->handleProgress(0);
            }
            nClipIndex++;
        }
    }
    m_bIsStarted = FALSE;
    if (m_bIsSave)
    {
        m_pStoryboard->handleAlways();
    }
    LOGI("audio controller is stopped");
    END_AUTOLOCK;
    return 0;
}

void CAController::start(BOOL32 bIsSave)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted)
    {
        return;
    }

    m_pThread->start();

    m_bIsStoped = FALSE;

    END_AUTOLOCK;
}

void CAController::stop()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted && !m_bIsStoped)
    {
        m_bIsStoped = TRUE;
        m_pLock->acttive();
    }
    END_AUTOLOCK;
    m_pThread->join();
}

void CAController::resume()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted && !m_bIsStoped && m_bIsPaused)
    {
        m_bIsPaused = FALSE;
        m_pLock->acttive();
    }
    END_AUTOLOCK;
}

void CAController::pause()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted && !m_bIsStoped && !m_bIsPaused)
    {
        m_bIsPaused = TRUE;
    }

    END_AUTOLOCK;
}

void CAController::seekTo(s64 sllPosition)
{
}

BOOL32 CAController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam)
{
    return TRUE;
}

void CAController::handle(CStoryboard *pStoryboard)
{
}
}