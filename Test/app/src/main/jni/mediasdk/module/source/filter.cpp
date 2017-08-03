/*******************************************************************************
 *        Module: mediasdk
 *          File: filter.cpp
 * Functionality: filter entity.
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
#include <stdlib.h>
#include "autolog.h"
#include "autofile.h"
#include "autolock.h"
#include "constant.h"
#include "filter.h"

namespace paomiantv
{

CFilter::CFilter()
{
    USE_LOG;
    m_pchSrcPath = (s8 *)malloc(MAX_LEN_FILE_PATH);
}

CFilter::~CFilter()
{
    USE_LOG;
    free(m_pchSrcPath);
}
void CFilter::init(s8* pchSrc, s64 sllStart, s64 sllDurantion)
{
    strncpy(m_pchSrcPath,pchSrc,MAX_LEN_FILE_PATH);
    m_sllStart = sllStart;
    m_sllDuration = sllDurantion;
}

void CFilter::uninit()
{
}

void CFilter::setSrc(s8 *pchSrc)
{
    strncpy(m_pchSrcPath, pchSrc, MAX_LEN_FILE_PATH);
}

void CFilter::setStart(s64 sllStart)
{
    m_sllStart = sllStart;
}

void CFilter::setDuration(s64 sllDuration)
{
    m_sllDuration = sllDuration;
}
void CFilter::parse()
{
}

} // namespace paomiantv
