/*******************************************************************************
 *        Module: paomiantv
 *          File: vcontroller.cpp
 * Functionality: handle video data.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#include "vcontroller.h"

namespace paomiantv {

    CVController::Garbo CVController::garbo; // 一定要初始化，不然程序结束时不会析构garbo

    CVController *CVController::m_pInstance = NULL;

    CVController *CVController::getInstance() {
        if (m_pInstance == NULL)
            m_pInstance = new CVController();
        return m_pInstance;
    }

    CVController::CVController(){

    }

    CVController::~CVController() {
    }

    BOOL32 CVController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam) {
        return TRUE;
    }

    void CVController::handle(CStoryboard *pStoryboard) {

    }
}