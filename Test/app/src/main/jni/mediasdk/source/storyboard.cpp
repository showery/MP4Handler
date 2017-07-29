#include <stdlib.h>
#include "storyboard.h"
#include "autolog.h"

namespace paomiantv
{
CStoryboard::CStoryboard(s8 *pchDstPath)
    : m_ptBGMClipParam(NULL),
      m_pchSrcBGM(NULL),
      m_sslBGMStartTm(0),
      m_sslBGMEndTm(-1),
      m_bIsStarted(FALSE),
      m_bIsStop(FALSE),
      m_bIsProcessing(FALSE)
{
    m_pLock = new CLock;
    m_pThread = new CThread(ThreadWrapper,this);
    m_pchDstPath = (s8 *)malloc(MAX_LEN_FILE_PATH);
    memset(m_pchDstPath, 0, MAX_LEN_FILE_PATH);
}

CStoryboard::~CStoryboard()
{
    stop();
    free(m_pchDstPath);
    delete m_pLock;
    for (int i = 0; i < m_vClips.size(); i++)
    {
        if (m_vClips[i] != null)
        {
            delete m_vClips[i];
        }
    }
}

//static
void *CStoryboard::ThreadWrapper(void *pThis)
{
#ifndef _ANDROID_
    prctl(PR_SET_NAME, "CStoryboard");
#else
    prctl(PR_SET_NAME, (unsigned long)"CStoryboard", 0, 0, 0);
#endif
    CStoryboard *p = (CStoryboard *)pThis;
    p->ThreadEntry();
    return (void *)p;
}

void CStoryboard::ThreadEntry()
{
    m_pLock->lock();
    LOGI("storyboard process thread is started");
    m_bStarted = TRUE;
    while (!m_bStop)
    {
        while (!m_bStop && !m_bNewFrame)
        {
            pthread_cond_wait(&m_cond, &m_mutex);
        }
        if (!m_bStop)
        {
            pthread_mutex_unlock(&m_mutex);
            handle();
            pthread_mutex_lock(&m_mutex);

            m_bNewFrame = FALSE;
        }
    }
    m_bStarted = FALSE;
    LOGI("storyboard process thread is stopped");
    pthread_mutex_unlock(&m_mutex);
}

BOOL32 CStoryboard::init(s8 *pchDstPath)
{
    BEGIN_AUTOLOCK(m_pLock);
    strncpy(m_pchDstPath, pchDstPath, MAX_LEN_FILE_PATH);
    END_AUTOLOCK;
    return TRUE;
}
BOOL32 CStoryboard::setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm, s64 sllEndTm)
{
    BEGIN_AUTOLOCK(m_pLock);
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

CClip *CStoryboard::removeClip(s32 index)
{
    CClip *pClip = NULL;
    BEGIN_AUTOLOCK(m_pLock);
    pClip = m_vClips.pop_back();
    END_AUTOLOCK;
    return pClip;
}

CClip *CStoryboard::getClip(s32 index)
{
    CClip *pClip = NULL;
    BEGIN_AUTOLOCK(m_pLock);
    END_AUTOLOCK;
    return pClip;
}

BOOL32 CStoryboard::swapClip(s32 indexA, s32 indexB)
{
    BEGIN_AUTOLOCK(m_pLock);
    END_AUTOLOCK;
    return TRUE;
}

BOOL32 CStoryboard::process()
{
    BEGIN_AUTOLOCK(m_pLock);
    m_bIsProcessing = TRUE;
    m_pLock->acttive();
    END_AUTOLOCK;
    return TRUE;
}

BOOL32 CStoryboard::cancel()
{
    BEGIN_AUTOLOCK(m_pLock);
    END_AUTOLOCK;
    return TRUE;
}

} // namespace paomiantv
