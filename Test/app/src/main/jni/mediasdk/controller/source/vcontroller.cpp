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
namespace paomiantv
{

CVController::CVController(CStoryboard *pStoryboard, BOOL32 bIsSave)
    : CController(pStoryboard, bIsSave)
{
    USE_LOG;
    m_pProcessor = new CVProcessor;
}

CVController::~CVController()
{
    USE_LOG;
    if (m_pProcessor != NULL)
    {
        delete m_pProcessor;
    }
}

int CVController::run()
{
    LOGI("video controller is started");
    s32 nClipNum = m_pStoryboard->getClipCount();
    s32 nSampleNum = 0;
    for (s32 i = 0; i < nClipNum; i++)
    {
        nSampleNum += m_pStoryboard->getClip(i)->getParser()->getVSampleNum();
    }
    s32 nCount = 0;
    while (nCount >= nSampleNum && !m_bIsStoped)
    {
        for (s32 i = 0; i < nClipNum; i++)
        {
            s32 nClipSampleNum = m_pStoryboard->getClip(i)->getParser()->getVSampleNum();

            while (nClipSampleNum > 0 && !m_bIsStoped)
            {

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
    LOGI("video controller is stopped");
    return 0;
}

void CVController::start(BOOL32 bIsSave)
{
}

void CVController::stop()
{
}

void CVController::resume()
{
}

void CVController::pause()
{
}

void CVController::seekTo(s64 sllPosition){

}

BOOL32 CVController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam)
{
    return TRUE;
}

void CVController::handle(CStoryboard *pStoryboard)
{
}
}