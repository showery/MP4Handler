/*******************************************************************************
 *        Module: codec
 *          File:
 * Functionality: h264 decoder
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
#ifndef _PAOMIANTV_H264DEC_H_
#define _PAOMIANTV_H264DEC_H_

#include <deque>
#include <unistd.h>
#include "autolock.h"
#include "thread.h"
#include "vcodec.h"
#include "frame.h"
#include "vprocessor.h"
#include "storyboard.h"

namespace paomiantv {

    class CReceiveH264Queue {
    public:
        CReceiveH264Queue(s32 max_size = 5) {
            m_pLock = new CLock;
            m_pSemDataAvailable = new CSemaphore();
            m_pSemSizeLimit = new CSemaphore(max_size);
        }

        ~CReceiveH264Queue() {
            clear();
            delete m_pSemSizeLimit;
            delete m_pSemDataAvailable;
            delete m_pLock;
        }

        void push(TFrame *node) {
            m_pSemSizeLimit->wait();
            m_pLock->lock();
            m_vBuffers.push_back(node);
            m_pLock->unlock();
            m_pSemDataAvailable->post();
        }

        void pop(TFrame *&node) {
            m_pSemDataAvailable->wait();
            m_pLock->lock();
            node = m_vBuffers.front();
            m_vBuffers.pop_front();
            m_pLock->unlock();
            m_pSemSizeLimit->post();
        }

        bool empty() {
            CAutoLock cAutoLock(m_pLock);
            return m_vBuffers.empty();
        }

        u64 size() {
            CAutoLock cAutoLock(m_pLock);
            return m_vBuffers.size();
        }

    private:
        void clear() {
            TFrame *tmp = NULL;
            m_pLock->lock();
            while (true) {
                if(m_vBuffers.empty()){
                    break;
                }
                tmp = m_vBuffers.front();
                m_vBuffers.pop_front();
                m_pLock->unlock();
                if (tmp!=NULL){
                    if (tmp->data != NULL) {
                        free(tmp->data);
                    }
                    delete tmp;
                }
                m_pLock->lock();
            }
            m_pLock->unlock();
            return;
        }


    private:
        ILock *m_pLock;
        ISemaphore *m_pSemDataAvailable;
        ISemaphore *m_pSemSizeLimit;
        std::deque<PTFrame> m_vBuffers;

    };

    class CH264Dec {
    public:
        CH264Dec(CStoryboard *pStoryboard,BOOL32 bIsSave = FALSE);

        virtual ~CH264Dec();

        bool addToQueue(TFrame *&pframe);

        void changeFormat(u8* sps,u32 spslen,u8* pps,u32 ppslen);

    private:
        void init();

        void destory();

        void start();

        void stop();

        void decodeFrame();

        static void *ThreadWrapper(void *pThis);

        int ThreadEntry();

    private:
        BOOL32 m_bIsSave;
        CStoryboard *m_pStoryboard;

        AMediaCodec *m_pVCodec;

        CVProcessor *m_pProcessor;

        ILock *m_pLock;
        CThread *m_pThread;
        CReceiveH264Queue *m_pQueue;
        BOOL32 m_bStarted;
        BOOL32 m_bStop;
        s32 m_nLastFrameNo;
        s32 m_nFramesNumber;
        s32 m_nWidth;
        s32 m_nHeight;
        s64 renderstart;
    };
}


#endif //_PAOMIANTV_H264DEC_H_
