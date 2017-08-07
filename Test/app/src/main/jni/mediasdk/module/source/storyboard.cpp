/*******************************************************************************
 *        Module: mediasdk
 *          File: storyboard.cpp
 * Functionality: storyboard entity.
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
#include "storyboard.h"
#include "autolog.h"

namespace paomiantv {
    CStoryboard::CStoryboard()
            : m_ptBGMClipParam(NULL),
              m_pchSrcBGM(NULL),
              m_sllBGMStartTm(0),
              m_sllBGMEndTm(-1),
              m_nStartClipIndex(0),
              m_nCurrAClipIndex(0),
              m_uCurrASampleId(0),
              m_nCurrVClipIndex(0),
              m_uCurrVSampleId(0),
              m_pRenderer(NULL),
              m_Handle(MP4_INVALID_FILE_HANDLE) {
        USE_LOG;
        m_pLock = new CLock;
        m_ptBGMClipParam = new TClipParam;
        m_pchDstPath = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_pchSrcBGM = (s8 *) malloc(MAX_LEN_FILE_PATH);
    }

    CStoryboard::~CStoryboard() {
        USE_LOG;
        uninit();
        free(m_pchDstPath);
        free(m_pchSrcBGM);
        delete m_ptBGMClipParam;
        delete m_pLock;
    }

    BOOL32 CStoryboard::init(s8 *pchDstPath) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (pchDstPath == NULL) {
                    break;
                } else {
                    m_vClips.clear();
                    memset(m_pchDstPath, 0, MAX_LEN_FILE_PATH);
                    memset(m_pchSrcBGM, 0, MAX_LEN_FILE_PATH);
                    strncpy(m_pchDstPath, pchDstPath, MAX_LEN_FILE_PATH);
                    m_Handle = MP4Create(pchDstPath, 0);
                    if (MP4_IS_VALID_FILE_HANDLE(m_Handle)) {
                        re = TRUE;
                    }
                }
            } while (0);
        END_AUTOLOCK;
        return re;
    }

    BOOL32 CStoryboard::uninit() {
        BEGIN_AUTOLOCK(m_pLock);
            //            for (int i = 0; i < m_vClips.size(); i++) {
            //                if (m_vClips[i] != NULL) {
            //                    delete m_vClips[i];
            //                }
            //            }
            if (MP4_IS_VALID_FILE_HANDLE(m_Handle)) {
                MP4Close(m_Handle);
                m_Handle = MP4_INVALID_FILE_HANDLE;
            }
            m_vClips.clear();
        END_AUTOLOCK;
        return TRUE;
    }

    BOOL32
    CStoryboard::setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm,
                        s64 sllEndTm) {
        BEGIN_AUTOLOCK(m_pLock);
            strncpy(m_pchSrcBGM, pchSrcpath, MAX_LEN_FILE_PATH);
            m_ptBGMClipParam->m_sllDuration = sllDurationCutTm;
            m_ptBGMClipParam->m_sllStart = sllStartCutTm;
            m_sllBGMStartTm = sllStartTm;
            m_sllBGMEndTm = sllEndTm;
        END_AUTOLOCK;
        return TRUE;
    }

    BOOL32 CStoryboard::addClip(CClip *pClip) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (pClip == NULL) {
                    break;
                }
                m_vClips.push_back(pClip);
                re = TRUE;
            } while (0);

        END_AUTOLOCK;
        return re;
    }

    BOOL32 CStoryboard::replaceClip(s32 index, CClip *pClip) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (index >= m_vClips.size() || pClip == NULL) {
                    break;
                }
                m_vClips[index] = pClip;
                re = TRUE;
            } while (0);

        END_AUTOLOCK;
        return re;
    }

    BOOL32 CStoryboard::insertClip(s32 index, CClip *pClip) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (index >= m_vClips.size() || pClip == NULL) {
                    break;
                }
                m_vClips.insert(m_vClips.begin() + index, pClip);
                re = TRUE;
            } while (0);

        END_AUTOLOCK;
        return re;
    }

    CClip *CStoryboard::removeClip(s32 index) {
        CClip *pClip = NULL;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (index >= m_vClips.size()) {
                    break;
                }
                pClip = m_vClips.at(index);
                m_vClips.erase(m_vClips.begin() + index);
            } while (0);
        END_AUTOLOCK;
        return pClip;
    }

    CClip *CStoryboard::getClip(s32 index) {
        CClip *pClip = NULL;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (index >= m_vClips.size()) {
                    break;
                }
                pClip = m_vClips.at(index);
            } while (0);
        END_AUTOLOCK;
        return pClip;
    }

    BOOL32 CStoryboard::swapClip(s32 indexA, s32 indexB) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            do {
                if (indexA >= m_vClips.size() || indexB >= m_vClips.size()) {
                    break;
                }

                CClip *clipA = m_vClips.at(indexA);
                if (clipA == NULL) {
                    break;
                }
                CClip *clipB = m_vClips.at(indexB);
                if (clipB == NULL) {
                    break;
                }
                m_vClips[indexA] = clipB;
                m_vClips[indexB] = clipA;
                re = TRUE;

            } while (0);
        END_AUTOLOCK;
        return re;
    }


    void CStoryboard::bindEvent(FailedCB cbOnFailed, SuccessCB cbOnSuccess, ProgressCB cbOnProgress,
                                AlwaysCB cbOnAlways, void *cbDelegate) {
        m_cbDelegate = cbDelegate;
        m_cbOnAlways = cbOnAlways;
        m_cbOnFailed = cbOnFailed;
        m_cbOnProgress = cbOnProgress;
        m_cbOnSuccess = cbOnSuccess;
    }

    s32 CStoryboard::getClipCount() {
        s32 size = 0;
        BEGIN_AUTOLOCK(m_pLock);
            size = m_vClips.size();
        END_AUTOLOCK;
        return size;
    }

    void CStoryboard::detachRenderer() {
        m_pRenderer = NULL;
    }

    void CStoryboard::attachRenderer(CRenderer *pRenderer) {
        m_pRenderer = pRenderer;
    }

    void CStoryboard::handleFailed(s32 nErr, s8 *pchDescription) {
        m_cbOnFailed(m_cbDelegate, nErr, pchDescription);
    }

    void CStoryboard::handleSuccess() {
        m_cbOnSuccess(m_cbDelegate);
    }

    void CStoryboard::handleProgress(s32 nProgress) {
        m_cbOnProgress(m_cbDelegate, nProgress);
    }

    void CStoryboard::handleAlways() {
        m_cbOnAlways(m_cbDelegate);
    }

    void CStoryboard::seekTo(s32 nIndex) {
        BEGIN_AUTOLOCK(m_pLock);
            m_nStartClipIndex = nIndex;
        END_AUTOLOCK;
    }

    BOOL32 CStoryboard::getNextVSpample(BOOL32 &bIsLastSample, u8 *&buff, u32 &size, u64 &starttime,
                                        u64 &duration,
                                        u64 &renderoffset, BOOL &isSync) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            s32 nVSampleNum = 0;
            s32 nClipCount = m_vClips.size();
            for (s32 i = 0; i < nClipCount; i++) {
                nVSampleNum += m_vClips[i]->getVSampleNum();
            }
            if (m_nCurrVClipIndex == nClipCount - 1 &&
                m_uCurrVSampleId == m_vClips[m_nCurrVClipIndex]->getVSampleEndId()) {
                bIsLastSample = TRUE;
            }
            if (m_nCurrVClipIndex >= nClipCount || m_nCurrVClipIndex < m_nStartClipIndex) {
                m_nCurrVClipIndex = m_nStartClipIndex;
            }
            if (m_uCurrVSampleId == 0) {
                m_uCurrVSampleId = m_vClips[m_nCurrVClipIndex]->getVSampleStartId();
            }

            re = m_vClips[m_nCurrVClipIndex]->getVidoeSampleById(m_uCurrVSampleId, buff, size,
                                                                starttime, duration, renderoffset,
                                                                isSync);
            m_uCurrVSampleId++;
            if (m_uCurrVSampleId > m_vClips[m_nCurrVClipIndex]->getVSampleEndId()) {
                m_nCurrVClipIndex++;
                if (m_nCurrVClipIndex >= nClipCount) {
                    m_nCurrVClipIndex = m_nStartClipIndex;
                }
                m_uCurrVSampleId = m_vClips[m_nCurrVClipIndex]->getVSampleStartId();
            }

        END_AUTOLOCK;

        return re;
    }

    BOOL32 CStoryboard::getNextASpample(BOOL32 &bIsLastSample, u8 *&buff, u32 &size, u64 &starttime,
                                        u64 &duration,
                                        u64 &renderoffset, BOOL &isSync) {
        BOOL32 re = FALSE;
        BEGIN_AUTOLOCK(m_pLock);
            s32 nASampleNum = 0;
            s32 nClipCount = m_vClips.size();
            for (s32 i = 0; i < nClipCount; i++) {
                nASampleNum += m_vClips[i]->getASampleNum();
            }
            if (m_nCurrAClipIndex == nClipCount - 1 &&
                m_uCurrASampleId == m_vClips[m_nCurrAClipIndex]->getASampleEndId()) {
                bIsLastSample = TRUE;
            }
            if (m_nCurrAClipIndex >= nClipCount || m_nCurrAClipIndex < m_nStartClipIndex) {
                m_nCurrAClipIndex = m_nStartClipIndex;
            }
            if (m_uCurrASampleId == 0) {
                m_uCurrASampleId = m_vClips[m_nCurrAClipIndex]->getASampleStartId();
            }

            re = m_vClips[m_nCurrAClipIndex]->getAudioSampleById(m_uCurrASampleId, buff, size,
                                                                starttime, duration, renderoffset,
                                                                isSync);

            m_uCurrASampleId++;
            if (m_uCurrASampleId > m_vClips[m_nCurrAClipIndex]->getASampleEndId()) {
                m_nCurrAClipIndex++;
                if (m_nCurrAClipIndex >= nClipCount) {
                    m_nCurrAClipIndex = m_nStartClipIndex;
                }
                m_uCurrASampleId = m_vClips[m_nCurrAClipIndex]->getASampleStartId();
            }

            s32 nCount = 0;
            for (s32 i = 0; i < m_nCurrAClipIndex; i++) {
                nCount += m_vClips[i]->getASampleNum();
            }
            nCount += m_uCurrASampleId - m_vClips[m_nCurrAClipIndex]->getASampleStartId() + 1;
            m_cbOnProgress(m_cbDelegate, nCount / nASampleNum);

        END_AUTOLOCK;
        return re;
    }
} // namespace paomiantv
