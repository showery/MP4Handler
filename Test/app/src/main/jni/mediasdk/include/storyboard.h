#ifndef _PAOMIANTV_MP4CUTTER_H_
#define _PAOMIANTV_MP4CUTTER_H_

#include <jni.h>
#include <pthread.h>
#include "typedef.h"
#include "autolock.h"
#include "clip.h"

namespace paomiantv
{
/*!
 * \brief    used in Storyboard.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagBGMParam
{
  //! , zero or positive is acceptable.
  s64 m_sllStart;
  //! duration, zero or positive is acceptable.
  s64 sllDuration;
  //! source mp4 file path
  s8 *pchSrcPath;
  //! constructure
  tagBGMParam()
  {
    memset(this, 0, sizeof(tagBGMParam));
  }
} TBGMParam;

class CStoryboard
{
public:
  CStoryboard();
  ~CStoryboard();

  BOOL32 init(s8 *pchDstPath);

  BOOL32 setBGM(s8 *pchSrcpath, s64 sllStartCutTm, s64 sllDurationCutTm, s64 sllStartTm, s64 sllEndTm);

  BOOL32 addClip(CClip *pClip);

  CClip *removeClip(s32 index);

  CClip *getClip(s32 index);

  BOOL32 swapClip(s32 indexA, s32 indexB);

  BOOL32 process();

  BOOL32 cancel();

private:
  void start();
  void stop();
  static void *ThreadWrapper(void *pThis);
  int ThreadEntry();

  //BGM
  TClipParam *m_ptBGMClipParam;
  s8 *m_pchSrcBGM;
  s64 m_sslBGMStartTm;
  s64 m_sslBGMEndTm;
  //clips
  s8 *m_pchDstPath;
  std::vector<CClip *> m_vClips;

  BOOL32 m_bIsStarted;
  BOOL32 m_bIsStop;
  BOOL32 m_bIsProcessing;
  
  CThread *m_pThread;
  ILock *m_pLock;
};

} // namespace paomiantv

#endif // _PAOMIANTV_MP4CUTTER_H_