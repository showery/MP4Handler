#include <stdlib.h>
#include "mp4extract.h"
#include "autolog.h"
#include "autofile.h"
#include "autolock.h"
#include "common.h"

namespace paomiantv
{

CMp4Extract::CMp4Extract(s8 *src)
{
    USE_LOG;
    init(src);
}

// FIXME: when exit the application, it will sometimes prints this error log:
// E/libEGL(19049): call to OpenGL ES API with no current context (logged once per thread)
// -- noted by huangxuefeng, 2013-07-29
CMp4Extract::~CMp4Extract()
{
    USE_LOG;
    uninit();
}

s32 CMp4Extract::getFrame(s32 VTrackId, s32 index)
{
    if (!m_pMp4File)
        return -1;
    // s8 NAL[5] = {0x00, 0x00, 0x00, 0x01};
    u8 *pData = NULL;
    u32 nSize = 0;
    MP4Timestamp pStartTime;
    MP4Duration pDuration;
    MP4Duration pRenderingOffset;
    BOOL pIsSyncSample = FALSE;

    MP4ReadSample(m_pMp4File, VTrackId, index, &pData, &nSize, &pStartTime, &pDuration, &pRenderingOffset, &pIsSyncSample);

    // //IDR֡ 帧，写入sps pps先
    // if (pIsSyncSample)
    // {
    //     fwrite(NAL, 4, 1, pFile);
    //     fwrite(sps, spslen, 1, pFile);

    //     fwrite(NAL, 4, 1, pFile);
    //     fwrite(pps, ppslen, 1, pFile);
    // }
    // //264frame
    // if (pData && nSize > 4)
    // {
    //     //标准的264帧，前面几个字节就是frame的长度.
    //     //需要替换为标准的264 nal 头.
    //     pData[0] = 0x00;
    //     pData[1] = 0x00;
    //     pData[2] = 0x00;
    //     pData[3] = 0x01;
    //     fwrite(pData, nSize, 1, pFile);
    // }

    //如果传入MP4ReadSample的视频pData是null
    // 它内部就会new 一个内存
    //如果传入的是已知的内存区域，
    //则需要保证空间bigger then max frames size.
    free(pData);
    pData = NULL;

    return 0;
}

s32 CMp4Extract::init(s8 *pchSrcPath)
{
    m_pMp4File = MP4Read(pchSrcPath);
    s32 videoindex = -1, audioindex = -1;
    if (!m_pMp4File)
    {
        LOGE("Read error....%s\r\n", pchSrcPath);
        return -1;
    }

    // MP4TrackId trackId = MP4_INVALID_TRACK_ID;
    // u32 numTracks = MP4GetNumberOfTracks(m_Mp4File;, NULL, 0);
    // LOGV("numTracks:%d\n", numTracks);

    // for (s32 i = 0; i < numTracks; i++)
    // {
    //     trackId = MP4FindTrackId(m_Mp4File, i, NULL, 0);
    //     const char *trackType = MP4GetTrackType(m_Mp4File;, trackId);
    //     if (MP4_IS_VIDEO_TRACK_TYPE(trackType))
    //     {
    //         videoindex = trackId;
    //         LOGV("audioindex:%d\n", videoindex);
    //     }
    //     else if (MP4_IS_AUDIO_TRACK_TYPE(trackType))
    //     {
    //         audioindex = trackId;
    //         LOGV("audioindex:%d\n", audioindex);
    //     }
    // }

    return 0;
}

void CMp4Extract::uninit()
{
    //需要mp4close 否则在嵌入式设备打开mp4上多了会内存泄露挂掉.
    MP4Close(m_pMp4File, 0);
}

} // namespace paomiantv
