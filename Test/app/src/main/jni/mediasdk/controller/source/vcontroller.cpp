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
#include "vprocessor.h"
namespace paomiantv {

    CVController::CVController(const CStoryboard *pStoryboard, BOOL32 bIsWithPreview = TRUE){
        USE_LOG;
        m_pProcessor = new CVProcessor;
    }

    CVController::~CVController() {
        USE_LOG;
        if(m_pProcessor!=NULL){
            delete m_pProcessor;
        }
    }

    void CVController::start(CStoryboard *pStoryboard, BOOL32 bIsWithPreview) {

    }

    BOOL32 CVController::transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam) {
        return TRUE;
    }

    void CVController::handle(CStoryboard *pStoryboard) {

    }
}