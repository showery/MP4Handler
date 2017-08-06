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

namespace paomiantv
{
CStoryboard::CStoryboard()
    : m_ptBGMClipParam(NULL),
      m_pchSrcBGM(NULL),
      m_sllBGMStartTm(0),
      m_sllBGMEndTm(-1),
      m_pRenderer(NULL)
{
    USE_LOG;
    m_vClips.clear();
    m_pLock = new CLock;
    m_ptBGMClipParam = new TClipParam;
    m_pchDstPath = (s8 *)malloc(MAX_LEN_FILE_PATH);
    m_pchSrcBGM = (s8 *)malloc(MAX_LEN_FILE_PATH);
    memset(m_pchDstPath, 0, MAX_LEN_FILE_PATH);
    memset(m_pchSrcBGM, 0, MAX_LEN_FILE_PATH);
}

CStoryboard::~CStoryboard()
{
    USE_LOG;
    free(m_pchDstPath);
    free(m_pchSrcBGM);
    delete m_pLock;
    delete m_ptBGMClipParam;
    for (int i = 0; i < m_vClips.size(); i++)
    {
        if (m_vClips[i] != NULL)
        {
            delete m_vClips[i];
        }
    }
    m_vClips.clear();
}

BOOL32 CStoryboard::init(s8 *pchDstPath)
{
    BOOL32 re = FALSE;
    BEGIN_AUTOLOCK(m_pLock);
    do
    {
        if (pchDstPath == NULL)
        {
            break;
        }
        else
        {
            strncpy(m_pchDstPath, pchDstPath, MAX_LEN_FILE_PATH);
            m_Handle = MP4Create(pchDstPath, 0);
            if (MP4_IS_VALID_FILE_HANDLE(m_Handle))
            {
                re = TRUE;
            }
        }
    } while (0);
    END_AUTOLOCK;
    return re;
}

BOOL32 CStoryboard::uninit()
{
    BEGIN_AUTOLOCK(m_pLock);
    END_AUTOLOCK;
    return TRUE;
}

BOOL32
CStoryboard::setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm,
                    s64 sllEndTm)
{
    BEGIN_AUTOLOCK(m_pLock);
    strncpy(m_pchSrcBGM, pchSrcpath, MAX_LEN_FILE_PATH);
    m_ptBGMClipParam->m_sllDuration = sllDurationCutTm;
    m_ptBGMClipParam->m_sllStart = sllStartCutTm;
    m_sllBGMStartTm = sllStartTm;
    m_sllBGMEndTm = sllEndTm;
    END_AUTOLOCK;
    return TRUE;
}

BOOL32 CStoryboard::addClip(CClip *pClip)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_vClips.push_back(pClip);
    END_AUTOLOCK;
    return TRUE;
}

BOOL32 CStoryboard::insertClip(s32 index, CClip *pClip)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_vClips.insert(m_vClips.begin() + index, pClip);
    END_AUTOLOCK;
    return TRUE;
}

CClip *CStoryboard::removeClip(s32 index)
{
    CClip *pClip = NULL;
    BEGIN_AUTOLOCK(m_pLock);
    pClip = m_vClips.at(index);
    m_vClips.erase(m_vClips.begin() + index);
    END_AUTOLOCK;
    return pClip;
}

CClip *CStoryboard::getClip(s32 index)
{
    CClip *pClip = NULL;
    BEGIN_AUTOLOCK(m_pLock);
    pClip = m_vClips.at(index);
    END_AUTOLOCK;
    return pClip;
}

BOOL32 CStoryboard::swapClip(s32 indexA, s32 indexB)
{
    BOOL32 re = FALSE;
    BEGIN_AUTOLOCK(m_pLock);
    do
    {

        CClip *clipA = m_vClips.at(indexA);
        if (clipA == NULL)
        {
            break;
        }
        m_vClips[indexA] = m_vClips.at(indexB);
        if (m_vClips[indexA] == NULL)
        {
            break;
        }
        m_vClips[indexB] = clipA;
        re = TRUE;

    } while (0);
    END_AUTOLOCK;
    return re;
}

void CStoryboard::bindEvent(FailedCB cbOnFailed, SuccessCB cbOnSuccess, ProgressCB cbOnProgress,
                            AlwaysCB cbOnAlways, void *cbDelegate)
{
    m_cbDelegate = cbDelegate;
    m_cbOnAlways = cbOnAlways;
    m_cbOnFailed = cbOnFailed;
    m_cbOnProgress = cbOnProgress;
    m_cbOnSuccess = cbOnSuccess;
}

s32 CStoryboard::getClipCount()
{
    BEGIN_AUTOLOCK(m_pLock);
    return m_vClips.size();
    END_AUTOLOCK;
}

void CStoryboard::detachRenderer()
{
    BEGIN_AUTOLOCK(m_pLock);
    m_pRenderer = NULL;
    END_AUTOLOCK;
}

void CStoryboard::attachRenderer(CRenderer *pRenderer)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_pRenderer = pRenderer;
    END_AUTOLOCK;
}

void CStoryboard::handleFailed(s32 nErr, s8 *pchDescription) {
    m_cbOnFailed(m_cbDelegate,nErr,pchDescription);
}

void CStoryboard::handleSuccess() {
    m_cbOnSuccess(m_cbDelegate);
}

void CStoryboard::handleProgress(s32 nProgress) {
    m_cbOnProgress(m_cbDelegate,nProgress);
}

void CStoryboard::handleAlways() {
    m_cbOnAlways(m_cbDelegate);
}
} // namespace paomiantv
