/*******************************************************************************
 *        Module: paomiantv
 *          File:
 * Functionality: video process
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

#ifndef _PAOMIANTV_VPROCESSOR_H_
#define _PAOMIANTV_VPROCESSOR_H_

#include "processor.h"

namespace paomiantv {
    class CVProcessor : public CProcessor {
    public:
        CVProcessor();

        ~CVProcessor();
    };
}

#endif //_PAOMIANTV_VPROCESSOR_H_
