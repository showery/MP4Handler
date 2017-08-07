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

namespace paomiantv {

    class CController {
    public:
        CController(CStoryboard *pStoryboard, BOOL32 bIsSave = FALSE);

        virtual ~CController();

    protected:
        virtual int run() = 0;

        BOOL32 m_bIsSave;
        CStoryboard *m_pStoryboard;

        ILock *m_pLock;
        CThread *m_pThread;

        BOOL32 m_bIsStarted;
        BOOL32 m_bIsStopped;
        BOOL32 m_bIsPaused;

        //when preview mode, the clip position to preview started.default is 0
        s32 m_nPreviewFromClipIndex;
        //when preview mode, the clip position to preview ended. default is -1(meaning last index, reserved)
        s32 m_nPreviewToClipIndex;

    private:
        static void *ThreadWrapper(void *pData);

    public:
        virtual void start(BOOL32 bIsSave);

        virtual void stop();

        virtual void resume();

        virtual void pause();

        virtual void seekTo(s32 nClipIndex);
    };
}

#endif /* _PAOMIANTV_CONTROLLER_H_ */
