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

    LOGI("audio controller is started");
    s32 nClipNum = m_pStoryboard->getClipCount();
    s32 nSampleNum = 0;
    for (s32 i = 0; i < nClipNum; i++)
    {
        nSampleNum += m_pStoryboard->getClip(i)->getParser()->getASampleNum();
    }
    s32 nCount = 0;
    BEGIN_AUTOLOCK(m_pLock);
    while (nCount >= nSampleNum && !m_bIsStoped)
    {
        for (s32 i = 0; i < nClipNum; i++)
        {
            s32 nClipSampleNum = m_pStoryboard->getClip(i)->getParser()->getASampleNum();

            while (nClipSampleNum > 0 && !m_bIsStoped)
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
                nClipSampleNum--;
            }
        }
    }
    END_AUTOLOCK;
    LOGI("audio controller is stopped");
    return 0;
}

void CAController::start(BOOL32 bIsSave)
{
    
}

void CAController::stop()
{

}

void CAController::resume()
{

}

void CAController::pause()
{

}

void CAController::seekTo(s64 sllPosition){

}

BOOL32 CAController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam)
{
    return TRUE;
}

void CAController::handle(CStoryboard *pStoryboard)
{
}
}