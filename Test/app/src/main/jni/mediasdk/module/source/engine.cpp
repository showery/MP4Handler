/*******************************************************************************
 *        Module: mediasdk
 *          File: engine.cpp
 * Functionality: engine entity.
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


#include "storyboard.h"
#include "engine.h"
#include "autolog.h"
#include "acontroller.h"
#include "vcontroller.h"

namespace paomiantv {

    CEngine::CEngine() {
        USE_LOG;
    }

    CEngine::~CEngine() {
        USE_LOG;
    }

    BOOL32 CEngine::init() {
        MP4SetLogCallback(log_cb);
        return TRUE;
    }

    void CEngine::uninit() {
        MP4SetLogCallback(NULL);
    }

    void CEngine::log_cb(MP4LogLevel loglevel, const s8 *fmt, va_list ap) {
        switch (loglevel) {
            case MP4_LOG_NONE:
                break;
            case MP4_LOG_ERROR:
                VLOGE(fmt, ap);
                break;
            case MP4_LOG_WARNING:
                VLOGW(fmt, ap);
                break;
            case MP4_LOG_INFO:
                VLOGI(fmt, ap);
                break;
            default:
                VLOGI(fmt, ap);
                break;
        }
    }

    void CEngine::startPreview(CStoryboard *pStoryboard) {
        CVController::getInstance()->startPreview(pStoryboard);
        CAController::getInstance()->startPreview(pStoryboard);
    }

    void CEngine::pausePreview() {
        CVController::getInstance()->pausePreview();
        CAController::getInstance()->pausePreview();
    }

    void CEngine::resumePreview() {
        CVController::getInstance()->resumePreview();
        CAController::getInstance()->resumePreview();
    }

    void CEngine::stopPreview() {
        CVController::getInstance()->stopPreview();
        CAController::getInstance()->stopPreview();
    }

    void CEngine::process(CStoryboard *pStoryboard) {

        CVController::getInstance()->start(pStoryboard);

        CAController::getInstance()->start(pStoryboard);
    }

    void CEngine::resume() {
        CVController::getInstance()->resume();

        CAController::getInstance()->resume();
    }

    void CEngine::pause() {
        CVController::getInstance()->pause();

        CAController::getInstance()->pause();
    }

    void CEngine::cancel() {
        CVController::getInstance()->cancel();

        CAController::getInstance()->cancel();
    }


}