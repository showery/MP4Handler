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
        m_pvFilterLock = new CLock;
        m_pvTransLock = new CLock;
        m_pchSrcPath = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_ptClipParam = new TClipParam;
        m_pcVTrackParam = new CTrackParamVideo;
        m_pcATrackParam = new CTrackParamAudio;
        m_pParser = new CClipParser(this);
    }

    CClip::~CClip() {
        USE_LOG;
        uninit();
        delete m_pParser;
        free(m_pchSrcPath);
        delete m_ptClipParam;
        delete m_pcVTrackParam;
        delete m_pcATrackParam;
        delete m_pvTransLock;
        delete m_pvFilterLock;
        delete m_pLock;
    }

    void CClip::init(s8 *pchSrc, s64 sllStart, s64 sllDuration) {
        BEGIN_AUTOLOCK(m_pvFilterLock);
            m_vFilters.clear();
        END_AUTOLOCK;
        BEGIN_AUTOLOCK(m_pvTransLock);
            m_vTransitions.clear();
        END_AUTOLOCK;
        BEGIN_AUTOLOCK(m_pLock);
            if (pchSrc != NULL && strlen(pchSrc)) {
                strncpy(m_pchSrcPath, pchSrc, MAX_LEN_FILE_PATH);
            }
            m_ptClipParam->m_sllStart = sllStart;
            m_ptClipParam->m_sllDuration = sllDuration;
            m_pParser->parse();
        END_AUTOLOCK;
    }

    void CClip::uninit() {
        BEGIN_AUTOLOCK(m_pvFilterLock);
            m_vFilters.clear();
        END_AUTOLOCK;
        BEGIN_AUTOLOCK(m_pvTransLock);
            m_vTransitions.clear();
        END_AUTOLOCK;
        BEGIN_AUTOLOCK(m_pLock);
            memset(m_pchSrcPath, 0, MAX_LEN_FILE_PATH);
            memset(m_ptClipParam, 0, sizeof(TClipParam));
            m_pParser->parse();
        END_AUTOLOCK;
    }

    void CClip::setSrc(s8 *pchSrc) {
        BEGIN_AUTOLOCK(m_pLock);
            if (pchSrc != NULL && strlen(pchSrc)) {
                strncpy(m_pchSrcPath, pchSrc, MAX_LEN_FILE_PATH);
            } else {
                memset(m_pchSrcPath, 0, MAX_LEN_FILE_PATH);
            }
            m_pParser->parse();
        END_AUTOLOCK;
    }

    void CClip::setStart(s64 sllStart) {
        BEGIN_AUTOLOCK(m_pLock);
            m_ptClipParam->m_sllStart = sllStart;
            m_pParser->parse();
        END_AUTOLOCK;
    }

    void CClip::setDuration(s64 sllDuration) {
        BEGIN_AUTOLOCK(m_pLock);
            m_ptClipParam->m_sllDuration = sllDuration;
            m_pParser->parse();
        END_AUTOLOCK;
    }


    CFilter *CClip::getFilter(s32 nIndex) {
        CFilter *pFilter = NULL;
        BEGIN_AUTOLOCK(m_pvFilterLock);
            pFilter = m_vFilters.at(nIndex);
        END_AUTOLOCK;
        return pFilter;
    }

    BOOL32 CClip::addFilter(CFilter *pFilter) {
        BEGIN_AUTOLOCK(m_pvFilterLock);
            m_vFilters.push_back(pFilter);
        END_AUTOLOCK;
        return TRUE;
    }

    CFilter *CClip::removeFilter(s32 nIndex) {
        CFilter *pFilter = NULL;
        BEGIN_AUTOLOCK(m_pvFilterLock);
            pFilter = m_vFilters.at(nIndex);
            m_vFilters.erase(m_vFilters.begin() + nIndex);
        END_AUTOLOCK;
        return pFilter;
    }

    BOOL32 CClip::addTransition(CTransition *pTransition) {
        BEGIN_AUTOLOCK(m_pvTransLock);
            m_vTransitions.push_back(pTransition);
        END_AUTOLOCK;
        return TRUE;
    }

    CTransition *CClip::getTransition(s32 nIndex) {
        CTransition *pTransition = NULL;
        BEGIN_AUTOLOCK(m_pvTransLock);
            pTransition = m_vTransitions.at(nIndex);
        END_AUTOLOCK;
        return pTransition;
    }

    CTransition *CClip::removeTransition(s32 nIndex) {
        CTransition *pTransition = NULL;
        BEGIN_AUTOLOCK(m_pvTransLock);
            pTransition = m_vTransitions.at(nIndex);
            m_vTransitions.erase(m_vTransitions.begin() + nIndex);
        END_AUTOLOCK;
        return pTransition;
    }

    BOOL CClip::getVidoeSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                                   u64 &renderoffset, BOOL &isSync) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVidoeSampleById(nId, buff, size, starttime, duration, renderoffset,
                                               isSync);
        END_AUTOLOCK;
        return re;
    }

    BOOL CClip::getVideoSPS(u8 *&sps, u32 &size) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVideoSPS(sps, size);
        END_AUTOLOCK;
        return re;
    }

    BOOL CClip::getVideoPPS(u8 *&pps, u32 &size) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVideoPPS(pps, size);
        END_AUTOLOCK;
        return re;
    }

    BOOL CClip::getVidoeSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,

                                     u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVidoeSampleByTime(ullTimestamp, buff, size, starttime, duration,
                                                 renderoffset, isSync);
        END_AUTOLOCK;
        return re;
    }

    BOOL CClip::getAudioSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                                   u64 &renderoffset, BOOL &isSync) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getAudioSampleById(nId, buff, size, starttime, duration, renderoffset,
                                               isSync);
        END_AUTOLOCK;
        return re;
    }

    BOOL CClip::getAudioSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,
                                     u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = false;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getAudioSampleByTime(ullTimestamp, buff, size, starttime, duration,
                                                 renderoffset, isSync);
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getVSampleStartId() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVSampleStartId();
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getVSampleEndId() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVSampleEndId();
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getVSampleNum() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getVSampleNum();
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getASampleStartId() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getASampleStartId();
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getASampleEndId() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getASampleEndId();
        END_AUTOLOCK;
        return re;
    }

    u32 CClip::getASampleNum() {
        u32 re = 0;
        BEGIN_AUTOLOCK(m_pLock);
            re = m_pParser->getASampleNum();
        END_AUTOLOCK;
        return re;
    }

} // namespace paomiantv
