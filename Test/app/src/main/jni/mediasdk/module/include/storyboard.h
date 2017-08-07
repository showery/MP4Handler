/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: storyboard entity.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_STORYBOARD_H_
#define _PAOMIANTV_STORYBOARD_H_

class CClip;

#include "thread.h"
#include "typedef.h"
#include "autolock.h"
#include "clip.h"
#include "renderer.h"

namespace paomiantv {

    typedef void (*FailedCB)(void *, s32, char *);

    typedef void (*SuccessCB)(void *);

    typedef void (*ProgressCB)(void *, s32);

    typedef void (*AlwaysCB)(void *);

    class CStoryboard {
    public:
        CStoryboard();

        ~CStoryboard();

        BOOL32 init(s8 *pchDstPath);

        BOOL32 uninit();

        BOOL32 setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm,
                      s64 sllEndTm);

        BOOL32 addClip(CClip *pClip);

        BOOL32 replaceClip(s32 index, CClip *pClip);

        BOOL32 insertClip(s32 index, CClip *pClip);

        CClip *removeClip(s32 index);

        CClip *getClip(s32 index);

        BOOL32 swapClip(s32 indexA, s32 indexB);

        s32 getClipCount();

        void attachRenderer(CRenderer *pRenderer);

        void detachRenderer();


        void bindEvent(FailedCB cbOnFailed, SuccessCB cbOnSuccess, ProgressCB cbOnProgress,
                       AlwaysCB cbOnAlways, void *cdDelegate);

        void handleFailed(s32 nErr, s8 *pchDescription);

        void handleSuccess();

        void handleProgress(s32 nPorgress);

        void handleAlways();


        BOOL32 getNextVSpample(BOOL32 &bIsLastSample, u8 *&buff, u32 &size, u64 &starttime,
                               u64 &duration,
                               u64 &renderoffset, BOOL &isSync);

        BOOL32
        getNextASpample(BOOL32 &bIsLastSample, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                        u64 &renderoffset, BOOL &isSync);

        void seekTo(s32 nIndex);


        inline CRenderer *getRenderer();

        inline s8 *getDstPath();

    private:
        //BGM
        TClipParam *m_ptBGMClipParam;
        s8 *m_pchSrcBGM;
        s64 m_sllBGMStartTm;
        s64 m_sllBGMEndTm;
        //clips
        s8 *m_pchDstPath;
        std::vector<CClip *> m_vClips;
        //renderer
        CRenderer *m_pRenderer;

        MP4FileHandle m_Handle;
        ILock *m_pLock;

        FailedCB m_cbOnFailed;
        SuccessCB m_cbOnSuccess;
        ProgressCB m_cbOnProgress;
        AlwaysCB m_cbOnAlways;
        void *m_cbDelegate;

        s32 m_nStartClipIndex;
        s32 m_nCurrAClipIndex;
        MP4SampleId m_uCurrASampleId;
        s32 m_nCurrVClipIndex;
        MP4SampleId m_uCurrVSampleId;
    };

    inline CRenderer *CStoryboard::getRenderer() {
        return m_pRenderer;
    }

    inline s8 *CStoryboard::getDstPath() {
        return m_pchDstPath;
    }

} // namespace paomiantv

#endif // _PAOMIANTV_STORYBOARD_H_