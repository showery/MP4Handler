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
#ifndef _PAOMIANTV_PRODUCER_H_
#define _PAOMIANTV_PRODUCER_H_

#include "typedef.h"
#include "autolock.h"
#include "vcontroller.h"
#include "acontroller.h"

namespace paomiantv
{
class CProducer
{
  public:
    CProducer(CStoryboard *pStoryboard, BOOL32 bIsSave = FALSE);

    virtual ~CProducer();

  private:
    BOOL32 m_bIsStarted;
    BOOL32 m_bIsPaused;
    BOOL32 m_bIsSave;
    CStoryboard *m_pStoryboard;

    CController* m_pVController;
    CController* m_pAController;
    ILock *m_pLock;

  public:
    // static CProducer *getInstance();

    void start(BOOL32 bIsSave);

    void stop();

    void resume();

    void pause();

    inline BOOL32 isPaused();
    inline BOOL32 isStarted();
    inline CStoryboard *getStoryboard();

    void seekTo(s32 nClipIndex);
};

inline BOOL32 CProducer::isPaused()
{
    return m_bIsPaused;
}
inline BOOL32 CProducer::isStarted()
{
    return m_bIsStarted;
}

inline CStoryboard *CProducer::getStoryboard()
{
    return m_pStoryboard;
}
}

#endif //_PAOMIANTV_PRODUCER_H_
