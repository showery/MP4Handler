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
#include "producer.h"
#include "../module/include/storyboard.h"

namespace paomiantv {

    class CEngine {
    public:
        CEngine();

        ~CEngine();

    private:
        CStoryboard *m_pStoryboard;
        CProducer *m_pProducer;

        static void log_cb(MP4LogLevel loglevel, const s8 *fmt, va_list ap);

    public:
        BOOL32 init();

        void uninit();

        void start(BOOL32 bIsSave = TRUE);

        void pause();

        void resume();

        void cancel();

        void setDataSource(CStoryboard *pStoryboard);

        void seekTo(s64 sllPosition);



        CProducer *getProducer();
    };

    inline CProducer *CEngine::getProducer(){
        return m_pProducer;
    }

} // namespace paomiantv

#endif //_PAOMIANTV_ENGINE_H
