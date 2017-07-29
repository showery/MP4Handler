#ifndef _PAOMIANTV_MP4CUTTER_H_
#define _PAOMIANTV_MP4CUTTER_H_

#include <jni.h>
#include "typedef.h"
#include "../include/mp4v2/mp4v2.h"

namespace paomiantv
{

class CMp4Extract
{
  public:
    CMp4Extract(s8* pchSrcPath);
    ~CMp4Extract();

    s32 getFrame(s32 VTrackId, s32 totalFrame);

  private:
    MP4FileHandle m_pMp4File;
    s32 init(s8 *pchSrcPath);
    void uninit();
};

} // namespace paomiantv

#endif // _PAOMIANTV_MP4CUTTER_H_