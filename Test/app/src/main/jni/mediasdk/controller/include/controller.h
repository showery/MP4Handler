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
    CController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE);
    virtual ~CController();

    BOOL32 m_bIsWithPreview;
    const CStoryboard *m_pStoryboard;
    ILock *m_pLock;

  public:
    virtual void init();

    virtual void uninit();

    virtual void start();

    virtual void stop();

    virtual void resume();

    virtual void pause();
};
}

#endif /* _PAOMIANTV_CONTROLLER_H_ */
