/*******************************************************************************
 *        Module: mediasdk
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

#include "autofile.h"
#include "stdlib.h"
#include "h264dec.h"
#include "constant.h"

namespace paomiantv {
    s64 systemnanotime() {
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000LL + now.tv_nsec;
    }

    CH264Dec::CH264Dec(CStoryboard *pStoryboard, BOOL32 bIsSave) : m_pStoryboard(pStoryboard),
                                                                   m_bIsSave(bIsSave) {
        USE_LOG;
        init();
    }

    CH264Dec::~CH264Dec() {
        USE_LOG;
        destory();
    }


    void CH264Dec::init() {
        m_pQueue = new CReceiveH264Queue();

        m_pVCodec = AMediaCodec_createDecoderByType(VIDEO_MIME_TYPE);
        m_nLastFrameNo = 0;
        m_bStarted = false;
        m_bStop = false;
        m_nWidth = 0;
        m_nHeight = 0;
        m_pLock = new CLock;
        m_pThread = new CThread(ThreadWrapper, this);
        renderstart = -1;

        AMediaFormat *pVInFormat = AMediaFormat_new();
        AMediaFormat_setString(pVInFormat, AMEDIAFORMAT_KEY_MIME, VIDEO_MIME_TYPE);
        AMediaFormat_setInt32(pVInFormat, AMEDIAFORMAT_KEY_WIDTH, MAX_WIDTH);
        AMediaFormat_setInt32(pVInFormat, AMEDIAFORMAT_KEY_HEIGHT, MAX_HEIGHT);
        AMediaFormat_setInt32(pVInFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT,
                              OMX_COLOR_FormatYUV420Planar);
        LOGE("input format: %s", AMediaFormat_toString(pVInFormat));
        AMediaCodec_configure(m_pVCodec, pVInFormat, NULL, NULL, 0);
        AMediaCodec_start(m_pVCodec);
        AMediaFormat_delete(pVInFormat);

        start();
    }

    void CH264Dec::destory() {
        stop();
        AMediaCodec_stop(m_pVCodec);
        AMediaCodec_delete(m_pVCodec);
        delete m_pLock;
        delete m_pQueue;
        m_pQueue = NULL;
    }

    void CH264Dec::start() {
        LOGI("CH264Dec::start");
        CAutoLock cAutoLock(m_pLock);
        if (m_bStarted) {
            return;
        }
        m_pThread->start();
        m_bStop = false;

    }

    void CH264Dec::stop() {
        LOGI("CH264Dec::stop");

        m_pLock->lock();
        if (m_bStarted && !m_bStop) {
            m_bStop = true;
        }
        m_pLock->unlock();

        m_pThread->join();
    }


//static
    void *CH264Dec::ThreadWrapper(void *pThis) {
        prctl(PR_SET_NAME, (unsigned long) "CH264Dec", 0, 0, 0);
        CH264Dec *p = (CH264Dec *) pThis;
        int nErr = p->ThreadEntry();
        return (void *) nErr;
    }

    int CH264Dec::ThreadEntry() {
        int nErr = 0;


        m_pLock->lock();
        LOGI("CH264Dec is started");
        m_bStarted = true;
        while (!m_bStop) {
            m_pLock->unlock();
            decodeFrame();
            m_pLock->lock();
        }
        m_bStarted = false;
        LOGI("CH264Dec is stopped");
        m_pLock->unlock();

        return 0;
    }

    bool CH264Dec::addToQueue(TFrame *&pframe) {

        if (pframe == NULL || pframe->data == NULL || pframe->size <= 0) {
            LOGE("frame not invalid! ");
            return false;
        }
        m_pQueue->push(pframe);
        return true;
    }

    void CH264Dec::decodeFrame() {
        //USE_LOG;
        do {
            // push the raw h264 to decoder
            ssize_t bufidx = AMediaCodec_dequeueInputBuffer(m_pVCodec, 2000);
            //LOGV("input buffer %zd", bufidx);
            if (bufidx >= 0) {
                TFrame *pframe = NULL;
                m_pQueue->pop(pframe);
                if (pframe == NULL || pframe->data == NULL || pframe->size <= 0) {
                    LOGE("frame is invalid! ");
                    if (pframe != NULL) {
                        LOGE("frame is invalid! ");
                        delete pframe;
                    }
                    break;
                }
                size_t bufsize;
                auto buf = AMediaCodec_getInputBuffer(m_pVCodec, bufidx, &bufsize);
                if (bufsize < pframe->size || buf == NULL) {
                    LOGE("codec buffer is smaller than raw frame data,or codec buffer is null!");
                    break;
                }

                memcpy(buf, pframe->data, pframe->size);
                AMediaCodec_queueInputBuffer(m_pVCodec, bufidx, 0, pframe->size,
                                             (pframe->startTm + pframe->duration +
                                              pframe->renderOffset) / 1000,
                                             (pframe->isPPS || pframe->isSPS)
                                             ? AMEDIACODEC_FLAG_CODEC_CONFIG : 0);
                if (pframe->data != NULL) {
                    free(pframe->data);
                    pframe->data = NULL;
                }
                delete pframe;
            }

            // pop the YUV420p data decoded;
            AMediaCodecBufferInfo info;
            auto status = AMediaCodec_dequeueOutputBuffer(m_pVCodec, &info, 0);
            if (status >= 0) {
                if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
                    LOGV("output EOS");
                }
                auto format = AMediaCodec_getOutputFormat(m_pVCodec);
//                LOGV("output format: %s", AMediaFormat_toString(format));
                s32 width = 0;
                s32 height = 0;
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_HEIGHT, &height);
                AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_WIDTH, &width);


                int64_t presentationNano = info.presentationTimeUs * 1000;
                if (renderstart < 0) {
                    renderstart = systemnanotime() - presentationNano;
                }
                int64_t delay = (renderstart + presentationNano) - systemnanotime();
                if (delay > 0) {
                    usleep(delay / 1000);
                }

                size_t bufsize;
                auto buf = AMediaCodec_getOutputBuffer(m_pVCodec, status, &bufsize);
                if (buf != NULL && bufsize > 0) {
                    //transform
                    if (m_bIsSave) {
                        //encode
                    } else {
                        //render
// save one remote frame for test
                        static bool issaved = false;
                        if (!issaved) {
                            char strPath[128];
                            sprintf(strPath, "/sdcard/DCIM/r_%dx%d.yuv", width, height);
                            CAutoFile fileYuv(strPath, "wa");
                            fileYuv.write(buf, width * height * 3 / 2);
                            LOGD("saved file %s", strPath);

                            issaved = true;
                        }
                        m_pStoryboard->drawVFrame(buf, width, height);
                        m_nWidth = width;
                        m_nHeight = height;


                    }
                }
                AMediaCodec_releaseOutputBuffer(m_pVCodec, status, false);
            } else if (status == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
                LOGV("output buffers changed");
            } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
                auto format = AMediaCodec_getOutputFormat(m_pVCodec);
                LOGV("format changed to: %s", AMediaFormat_toString(format));
                AMediaFormat_delete(format);
            } else if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
//                LOGV("no output buffer right now");
            } else {
                LOGV("unexpected info code: %zd", status);
            }


        } while (0);
        return;
    }
}

