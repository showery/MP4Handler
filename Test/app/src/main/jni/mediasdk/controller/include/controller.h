/*******************************************************************************
 *        Module: mediasdk
 *          File: 
 * Functionality: controller
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
#ifndef _PAOMIANTV_CONTROLLER_H_
#define _PAOMIANTV_CONTROLLER_H_

#include <typeinfo>
#include "typedef.h"
#include "autolock.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"
#include "storyboard.h"
#include "controller.h"

namespace paomiantv
{

class CController
{
  protected:
    CController(CStoryboard *pStoryboard, BOOL32 bIsSave = FALSE);
    virtual ~CController();

    virtual int run() = 0;

    BOOL32 m_bIsSave;
    CStoryboard *m_pStoryboard;

    ILock *m_pLock;
    CThread *m_pThread;

    BOOL32 m_bIsStoped;
    BOOL32 m_bIsPaused;

  private:
    static void *ThreadWrapper(void *pData);

  public:
    virtual void start(BOOL32 bIsSave) = 0;

    virtual void stop() = 0;

    virtual void resume() = 0;

    virtual void pause() = 0;

    virtual void seekTo(s64 sllPosition) = 0;
};
}

#endif /* _PAOMIANTV_CONTROLLER_H_ */
