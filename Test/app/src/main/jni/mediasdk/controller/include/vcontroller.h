/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: video controller
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

#ifndef _PAOMIANTV_VCONTROLLER_H_
#define _PAOMIANTV_VCONTROLLER_H_

#include "autolock.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"
#include "thread.h"
#include "vprocessor.h"
#include "storyboard.h"
#include "controller.h"

namespace paomiantv
{

class CVController:public CController
{

public:
    CVController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE);

     ~CVController();

    void start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE);

private:
    static void *ThreadWrapper(void *pData);

     void ThreadEntry(CStoryboard *pStoryboard);

    BOOL32 m_bIsStop;
    BOOL32 m_bIsStarted;
    BOOL32 m_bIsNeedPreview;
    CThread *m_pThread;
    ILock *m_pLock;

    CVProcessor *m_pProcessor;

private:
    void handle(CStoryboard *pStoryboard);
    s32 getSampleNum(CStoryboard *pStoryboard);
    //输入输出都是解码后的声音数据(YUV)
    BOOL32 transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam);
};
}

#endif /* _PAOMIANTV_VCONTROLLER_H_ */
