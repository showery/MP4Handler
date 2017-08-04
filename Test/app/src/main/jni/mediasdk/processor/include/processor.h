/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: processor
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

#ifndef _PAOMIANTV_PROCESSOR_H
#define _PAOMIANTV_PROCESSOR_H

#include "typedef.h"
#include "autolock.h"

namespace paomiantv {

    class CProcessor {
    public:
        CProcessor() {
            m_pLock = new CLock;
        };

        virtual  ~CProcessor() {
            delete m_pLock;
            m_pLock = NULL;
        };

    protected:
        ILock *m_pLock;
        BOOL32 m_bIsFinished;
    public:
        inline BOOL32 isFinished();
    };

    inline BOOL32 CProcessor::isFinished() {
        return m_bIsFinished;
    }
}

#endif //_PAOMIANTV_PROCESSOR_H
