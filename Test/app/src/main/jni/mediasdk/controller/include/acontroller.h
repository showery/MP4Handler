/*******************************************************************************
 *        Module: mediasdk
 *          File: 
 * Functionality: audio controller
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-24  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_ACONTROLLER_H_
#define _PAOMIANTV_ACONTROLLER_H_

#include "autolock.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"
#include "thread.h"
#include "aprocessor.h"
#include "storyboard.h"
#include "controller.h"

namespace paomiantv
{

class CAController:public CController
{
  public:
    CAController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE);

     ~CAController();

    void start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE);

  private:
    static void *ThreadWrapper(void *pData);

    void ThreadEntry(CStoryboard *pStoryboard);

    BOOL32 m_bIsStop;
    BOOL32 m_bIsStarted;
    BOOL32 m_bIsNeedPreview;
    CThread *m_pThread;
    ILock *m_pLock;

    CAProcessor *m_pProcessor;

  private:
    void handle(CStoryboard *pStoryboard);
    s32 getSampleNum(CStoryboard *pStoryboard);
    //输入输出都是解码后的声音数据(PCM)
    BOOL32 transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam);
};
}

#endif /* _PAOMIANTV_ACONTROLLER_H_ */
