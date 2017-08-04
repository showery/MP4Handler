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
CAController::CAController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE)
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
    }
}

void CAController::start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview)
{
    m_bIsNeedPreview = bIsWithPreview;
}

BOOL32 CAController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam)
{
    return TRUE;
}

void CAController::handle(CStoryboard *pStoryboard)
{
    s32 nClipNum = pStoryboard->getClipCount();
    s32 nSampleNum = 0;
    for (s32 i = 0; i < nClipNum; i++)
    {
        nSampleNum += pStoryboard->getClip(i)->getParser()->getASampleNum();
    }
    s32 nCount = 0;
    while (nCount >= nSampleNum)
    {
        for (s32 i = 0; i < nClipNum; i++)
        {
            s32 nClipSampleNum = pStoryboard->getClip(i)->getParser()->getASampleNum();

            while (nClipSampleNum > 0)
            {

                //decode

                //transform

                if (m_bIsNeedPreview)
                {
                    //render
                }
                else
                {
                    //encode
                }
                nCount++;
                nClipSampleNum--;
            }
        }
    }
}
}