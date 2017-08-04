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
#include "ctrlmanager.h"

namespace paomiantv
{

CController::CController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview)
    : m_bIsWithPreview(bIsWithPreview),
      m_pStoryboard(pStoryboard)
{
    USE_LOG;
    init();
}

CController::~CController()
{
    USE_LOG;
    uninit();
}

void CController::init()
{
    m_pLock = new CLock;
}

void CController::uninit()
{
    if (m_pLock != NULL)
    {
        m_pLock = new CLock;
    }
}
void CController::start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview)
{
    m_pVCtrl->start(pStoryboard, bIsWithPreview);
    m_pACtrl->start(pStoryboard, bIsWithPreview);
    m_bIsWithPreview = bIsWithPreview;
}

void CController::stop()
{
    m_pVCtrl->stop();
    m_pACtrl->stop();
}

void CController::resume()
{
    m_pVCtrl->resume();
    m_pACtrl->resume();
}

void CController::pause()
{
    m_pVCtrl->pause();
    m_pACtrl->pause();
}
}