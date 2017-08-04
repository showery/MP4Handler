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
#include <controller/include/ctrlmanager.h>
#include "typedef.h"
#include "producer.h"
#include "ctrlmanager.h"

namespace paomiantv
{
// CProducer::Garbo CProducer::garbo; // 一定要初始化，不然程序结束时不会析构garbo

// CProducer *CProducer::m_pInstance = NULL;

// CProducer *CProducer::getInstance() {
//     if (m_pInstance == NULL)
//         m_pInstance = new CProducer();
//     return m_pInstance;
// }

CProducer::CProducer(CStoryboard *pStoryboard)
    : m_pStoryboard(pStoryboard),
      m_bIsStarted(FALSE),
      m_bIsPaused(FALSE)
{
    m_pLock = new CLock;
}

CProducer::~CProducer()
{
    delete m_pLock;
    m_pLock = NULL;
}

void CProducer::start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview)
{
    BEGIN_AUTOLOCK(m_pLock);
    if (!m_bIsStarted)
    {
        CCtrlManager::getInstance()->start(pStoryboard, bIsWithPreview);
        m_bIsStarted = TRUE;
        m_bIsPaused = FALSE;
    }
    END_AUTOLOCK;
}

void CProducer::stop()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (!m_bIsStarted)
    {
        CCtrlManager::getInstance()->stop();
        m_bIsStarted = FALSE;
        m_bIsPaused = FALSE;
    }
    END_AUTOLOCK;
}

void CProducer::resume()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted && m_bIsPaused)
    {

        m_bIsPaused = FALSE;
        CCtrlManager::getInstance()->resume();
    }
    END_AUTOLOCK;
}

void CProducer::pause()
{
    BEGIN_AUTOLOCK(m_pLock);
    if (m_bIsStarted && !m_bIsPaused)
    {
        m_bIsPaused = TRUE;
        CCtrlManager::getInstance()->pause();
    }
    END_AUTOLOCK;
}
}