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

namespace paomiantv
{

CController::CController(CStoryboard *pStoryboard, BOOL32 bIsSave)
    : m_bIsSave(bIsSave),
      m_pStoryboard(pStoryboard),
      m_bIsStoped(FALSE),
      m_bIsPaused(FALSE)
{
    USE_LOG;
    m_pLock = new CLock;
    m_pThread = new CThread(ThreadWrapper,this);
}

CController::~CController()
{
    USE_LOG;
    if (m_pLock != NULL)
    {
        m_pLock = new CLock;
        m_pLock = NULL;
    }
    if(m_pThread!=NULL){
        delete m_pThread;
        m_pThread=NULL;
    }
    
}

//static
void *CController::ThreadWrapper(void *pThis)
{
    CThread::SetName(typeid(*pThis).name());
    CController *p = (CController *)pThis;
    int nErr = p->run();
    return (void *)nErr;
}
}