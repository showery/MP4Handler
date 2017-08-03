/*******************************************************************************
 *        Module: paomiantv
 *          File:
 * Functionality: audio process
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

#ifndef _PAOMIANTV_APROCESSOR_H_
#define _PAOMIANTV_APROCESSOR_H_


#include <typedef.h>

class CAprocessor {
public:
    CAprocessor();
    ~CAprocessor();
private:
    BOOL32  m_bIsFinished;

};


#endif //_PAOMIANTV_APROCESSOR_H_
