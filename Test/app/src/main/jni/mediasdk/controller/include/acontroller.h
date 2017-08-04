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
#include "../../module/include/storyboard.h"
#include "../../processor/include/aprocessor.h"

namespace paomiantv
{

class CAController : public CController
{
  public:
    CAController(CStoryboard *pStoryboard, BOOL32 bIsSave = FALSE);

    ~CAController();

    void start(BOOL32 bIsSave);

    void stop();

    void resume();

    void pause();
    
    void seekTo(s64 sllPosition);

  private:
    CAProcessor *m_pProcessor;

  private:
    int run();
    void handle(CStoryboard *pStoryboard);
    s32 getSampleNum(CStoryboard *pStoryboard);
    //输入输出都是解码后的声音数据(PCM)
    BOOL32 transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam);
};
}

#endif /* _PAOMIANTV_ACONTROLLER_H_ */
