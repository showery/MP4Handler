/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: define engine entity.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-02  v1.0        huangxuefeng  created
 ******************************************************************************/


#ifndef _PAOMIANTV_ENGINE_H
#define _PAOMIANTV_ENGINE_H

#include "typedef.h"
#include "mp4v2/mp4v2.h"
#include "storyboard.h"

namespace paomiantv{

    class CEngine
    {
    public:
        CEngine();
        ~CEngine();

    private:
        static void log_cb(MP4LogLevel loglevel, const s8 *fmt, va_list ap);

    public:
        BOOL32 init();
        void uninit();

        void process(CStoryboard *pStoryboard);
        void pause();
        void cancel();

        void startPreview(CStoryboard *pStoryboard);
        void pausePreview();
        void stopPreview();

        void resume();

        void resumePreview();
    };

} // namespace paomiantv

#endif //_PAOMIANTV_ENGINE_H
