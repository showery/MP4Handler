#ifndef _PAOMIANTV_MP4CUTTER_H_
#define _PAOMIANTV_MP4CUTTER_H_

#include <jni.h>
#include "typedef.h"
#include "../include/mp4v2/mp4v2.h"

namespace paomiantv
{

#define ERROR_SOURCE 10000
#define ERROR_SOURCE_DIST "Soure file is null or not existed!"

/*!
 * \brief    used in Cutter.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagMp4CutterParam
{
  //! start time, zero or positive is acceptable.
  s64 sllStart;
  //! duration, zero or positive is acceptable.
  s64 sllDuration;
  //! source mp4 file path
  s8 *pchSrcPath;
  //! output mp4file path
  s8 *pchDstPath;
} TMp4CutterParam;

class CMp4Clip
{
public:
  CMp4Clip(JNIEnv *env, jobject jcutter,
             TMp4CutterParam *pCutterParam);
  ~CMp4Clip();

  void clip();

private:
  JavaVM *m_jvm;
  jobject m_jCutter;
  jmethodID m_jmtdFailed;
  jmethodID m_jmtdPrgress;
  jmethodID m_jmtdSuccess;
  TMp4CutterParam *m_ptMp4CutterParam;
  BOOL32 m_bIsCancel;
 
  MP4TrackId clipTrack(MP4FileHandle srcFile, MP4TrackId trackId, MP4FileHandle dstFile, s64 startTime, s64 duration);
  void failedCallback(s32 nErr, s8 *pchDescription);
  void progressCallback(s32 nProgress);
  void successCallback();
};

} // namespace paomiantv

#endif // _PAOMIANTV_MP4CUTTER_H_