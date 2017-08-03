/*******************************************************************************
 *        Module: mediasdk
 *          File: acontroller.cpp
 * Functionality: handle audio data.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/

#include "acontroller.h"

namespace paomiantv {

    CAController::Garbo CAController::garbo; // 一定要初始化，不然程序结束时不会析构garbo

    CAController *CAController::m_pInstance = NULL;

    CAController *CAController::getInstance() {
        if (m_pInstance == NULL)
            m_pInstance = new CAController();
        return m_pInstance;
    }

    CAController::CAController()
    {
    }

    CAController::~CAController() {
    }

    BOOL32 CAController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam) {
        return TRUE;
    }

    void CAController::handle(CStoryboard *pStoryboard) {
    }

}