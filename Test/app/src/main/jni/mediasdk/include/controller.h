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
#include "thread.h"
#include "storyboard.h"

namespace paomiantv {

    class CController {
    protected:
        CController();

        virtual ~CController();

        static void *ThreadWrapper(void *pData);

        virtual void ThreadEntry(CStoryboard *pStoryboard);

        virtual void handle(CStoryboard *pStoryboard)=0;

        //输入输出都是解码后的数据
        virtual BOOL32 transform(u8 *pbyIn, u8 *pbyOut, void *ptTransParam) =0;

        BOOL32 m_bIsStop;
        BOOL32 m_bIsStarted;
        BOOL32 m_bIsNeedPreview;
        BOOL32 m_bNewFrame;
        CThread *m_pThread;
        ILock *m_pLock;

    public:


        void start(CStoryboard *pStoryboard);

        void cancel();

        void init();

        void uninit();

        void startPreview(CStoryboard *pStoryboard);

        void pausePreview();

        void resumePreview();

        void stopPreview();

        void resume();

        void pause();
    };
}

#endif /* _PAOMIANTV_CONTROLLER_H_ */
