#ifndef _PAOMIANTV_MP4CUTTER_H_
#define _PAOMIANTV_MP4CUTTER_H_

#include <jni.h>
#include "typedef.h"
#include "../include/mp4v2/mp4v2.h"

namespace paomiantv
{

/*!
 * \brief    used in Cutter.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagClipParam
{
  //! start time, zero or positive is acceptable.
  s64 sllStart;
  //! duration, zero or positive is acceptable.
  s64 sllDuration;
  //! source mp4 file path
  s8 *pchSrcPath;
  //! constructure
  tagClipParam(){
    memset(this,0,sizeof(tagClipParam));
  }
} TClipParam;

class CClip
{
public:
  CClip();
  ~CClip();

  void clip();

private:
  TClipParam *m_ptClipParam;
  MP4TrackId clipTrack(MP4FileHandle srcFile, MP4TrackId trackId, MP4FileHandle dstFile, s64 startTime, s64 duration);
};

} // namespace paomiantv

#endif // _PAOMIANTV_MP4CUTTER_H_