#ifndef _PAOMIANTV_STORYBOARD_H_
#define _PAOMIANTV_STORYBOARD_H_

class CClip;

#include <jni.h>
#include "thread.h"
#include "typedef.h"
#include "autolock.h"
#include "clip.h"

namespace paomiantv {

    typedef void ( *FailedCB    )(void *, s32, char *);

    typedef void ( *SuccessCB    )(void *);

    typedef void ( *ProgressCB    )(void *, s32);

    typedef void ( *AlwaysCB    )(void *);

    class CStoryboard {
    public:
        CStoryboard();

        ~CStoryboard();

        BOOL32 init(s8 *pchDstPath);

        inline s8 *getDstPath();

        BOOL32 uninit();

        BOOL32 setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm,
                      s64 sllEndTm);

        BOOL32 addClip(CClip *pClip);

        BOOL32 insertClip(s32 index, CClip *pClip);

        CClip *removeClip(s32 index);

        CClip *getClip(s32 index);

        BOOL32 swapClip(s32 indexA, s32 indexB);

        s32 getClipCount();

        void bindEvent(FailedCB cbOnFailed, SuccessCB cbOnSuccess, ProgressCB cbOnProgress,
                       AlwaysCB cbOnAlways, void *cdDelegate);

        void resume();

        void pause();

        void process();

        void cancel();

    private:
        void prepare();

        void stop();

        void handle();

        static void *ThreadWrapper(void *pThis);

        void ThreadEntry();

        //BGM
        TClipParam *m_ptBGMClipParam;
        s8 *m_pchSrcBGM;
        s64 m_sllBGMStartTm;
        s64 m_sllBGMEndTm;
        //clips
        s8 *m_pchDstPath;
        std::vector<CClip *> m_vClips;

        BOOL32 m_bIsStarted;
        BOOL32 m_bIsStop;
        BOOL32 m_bNewTask;
        BOOL32 m_bIsPaused;

        CThread *m_pThread;
        ILock *m_pLock;

        FailedCB m_cbOnFailed;
        SuccessCB m_cbOnSuccess;
        ProgressCB m_cbOnProgress;
        AlwaysCB m_cbOnAlways;
        void *m_cbDelegate;
    };

    inline s8 *CStoryboard::getDstPath() {
        return m_pchDstPath;
    }

} // namespace paomiantv

#endif // _PAOMIANTV_STORYBOARD_H_