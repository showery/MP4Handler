/*******************************************************************************
 *        Module: mediasdk
 *          File: clip.cpp
 * Functionality: clip entity.
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
#include "constant.h"
#include "clip.h"
namespace paomiantv {

    CClip::CClip() {
        USE_LOG;
        m_pLock = new CLock;
        m_vFilters.clear();
        m_vTransitions.clear();
        m_pchSrcPath = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_ptClipParam = new TClipParam;
        m_pcVTrackParam = new CTrackParamVideo;
        m_pcATrackParam = new CTrackParamAudio;

    }

    CClip::~CClip() {
        USE_LOG;
        free(m_pchSrcPath);
        delete m_ptClipParam;
        delete m_pcVTrackParam;
        delete m_pcATrackParam;
        delete m_pLock;
    }

    void CClip::init(s8 *pchSrc, s64 sllStart, s64 sllDuration) {
        strncpy(m_pchSrcPath, pchSrc, MAX_LEN_FILE_PATH);
        m_ptClipParam->m_sllStart = sllStart;
        m_ptClipParam->m_sllDuration = sllDuration;
    }

    void CClip::uninit() {
    }

    s8 *CClip::getSrc() {
        return m_pchSrcPath;
    }

    void CClip::setSrc(s8 *pchSrc) {
        strncpy(m_pchSrcPath,pchSrc,MAX_LEN_FILE_PATH);
    }

    s64 CClip::getStart() {
        return m_ptClipParam->m_sllStart;
    }

    void CClip::setStart(s64 sllStart) {
        m_ptClipParam->m_sllStart = sllStart;
    }

    s64 CClip::getDuration() {
        return m_ptClipParam->m_sllDuration;
    }

    void CClip::setDuration(s64 sllDuration) {
        m_ptClipParam->m_sllDuration = sllDuration;
    }

    CFilter *CClip::getFilter(s32 nIndex) {
        return m_vFilters.at(nIndex);
    }

    BOOL32 CClip::addFilter(CFilter *pFilter) {
        m_vFilters.push_back(pFilter);
        return TRUE;
    }

    CFilter * CClip::removeFilter(s32 nIndex) {
        CFilter* pFilter = m_vFilters.at(nIndex);
        m_vFilters.erase(m_vFilters.begin() + nIndex);
        return pFilter;
    }

    s32 CClip::getFilterCount() {
        return m_vFilters.size();
    }

    BOOL32 CClip::addTransition(CTransition *pTransition) {
        m_vTransitions.push_back(pTransition);
        return TRUE;
    }

    CTransition* CClip::getTransition(s32 nIndex) {
        return m_vTransitions.at(nIndex);
    }

    CTransition* CClip::removeTransition(s32 nIndex) {

       CTransition* pTransition = m_vTransitions.at(nIndex);
           m_vTransitions.erase(m_vTransitions.begin() + nIndex);
        return pTransition;
    }

    s32 CClip::getTransitionCount() {
        return m_vTransitions.size();
    }

} // namespace paomiantv
