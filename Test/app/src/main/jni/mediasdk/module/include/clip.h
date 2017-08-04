/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: define clip entity.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-25  v1.0        huangxuefeng  created
 ******************************************************************************/

#ifndef _PAOMIANTV_CLIP_H_
#define _PAOMIANTV_CLIP_H_

#include <jni.h>
#include <vector>
#include "autolock.h"
#include "typedef.h"
#include "mp4v2/mp4v2.h"
#include "filter.h"
#include "transition.h"
#include "clipparser.h"

namespace paomiantv
{
class CClipParser;
/*!
 * \brief    used in Clip.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagClipParam
{
    //! start time, zero or positive is acceptable.
    s64 m_sllStart;
    //! duration, zero or positive is acceptable.
    s64 m_sllDuration;

    //! constructure
    tagClipParam()
    {
        m_sllStart = 0;
        m_sllDuration = -1;
    }
} TClipParam;

/*!
 * \brief    used in Clip.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
class CTrackParam
{
  public:
    //! the time scale of video track.
    u64 m_ullTimeScale;
    //! the number of samples video track.
    u32 m_uNumberSamples;

    //! constructure
    CTrackParam()
    {
        m_ullTimeScale = 0;
        m_uNumberSamples = 0;
    }

    virtual ~CTrackParam() {}
};

class CTrackParamVideo : public CTrackParam
{
  public:
    //! the width of video track.
    s32 m_nWidth;
    //! the height of video track.
    s32 m_nHeight;

    //! constructure
    CTrackParamVideo()
    {
        m_nWidth = 0;
        m_nHeight = 0;
    }

    virtual ~CTrackParamVideo() {}
};

class CTrackParamAudio : public CTrackParam
{
  public:
    //! the channels of audio track.
    s32 m_nChannel;
    //! the frequence re of audio track. 8k/22k/44.1k etc
    s32 m_nFrequency;
    //! the precision of audio track. 8bit/16bit/32bit etc
    s32 m_nPrecision;

    //! constructure
    CTrackParamAudio()
    {
        m_nChannel = 0;
        m_nFrequency = 0;
        m_nPrecision = 0;
    }

    virtual ~CTrackParamAudio() {}
};

/*!
 * \brief    used in Clip.
 * \author  huangxuefeng
 * \date    2017-07-21
 */
typedef struct tagSampleParam
{
    //! the start sample ID of track.
    u64 m_ullPresentationTime;
    //! the sample ID in track.
    u32 m_uId;
    //! the size of sample.
    s32 m_nSize;
    //! the video sample type: I or P.
    BOOL32 m_bIsIDR;
    //! the sample is audio or video.
    BOOL32 m_bIsVideo;

    //! constructure
    tagSampleParam()
    {
        memset(this, 0, sizeof(tagSampleParam));
    }
} TSampleParam;

class CClip
{
  public:
    CClip();

    ~CClip();

    void clip();

  private:
    s8 *m_pchSrcPath;
    TClipParam *m_ptClipParam;
    CTrackParam *m_pcVTrackParam;
    CTrackParam *m_pcATrackParam;
    std::vector<CFilter *> m_vFilters;
    std::vector<CTransition *> m_vTransitions;
    CClipParser *m_pParser;
    ILock *m_pLock;

  public:
    void init(s8 *pchSrc, s64 sllStart, s64 sllDuration);

    void uninit();

    void setSrc(s8 *pchSrc);

    void setStart(s64 ullStart);

    void setDuration(s64 ullDuration);

    CFilter *getFilter(s32 nIndex);

    BOOL32 addFilter(CFilter *pFilter);

    CFilter *removeFilter(s32 nIndex);

    BOOL32 addTransition(CTransition *pTransition);

    CTransition *getTransition(s32 nIndex);

    CTransition *removeTransition(s32 nIndex);

    inline s8 *getSrc();
    inline s64 getStart();
    inline s64 getDuration();
    inline s32 getFilterCount();
    inline s32 getTransitionCount();
    inline CClipParser *getParser();
};

inline s8 *CClip::getSrc()
{
    return m_pchSrcPath;
}

inline s64 CClip::getStart()
{
    return m_ptClipParam->m_sllStart;
}
inline s64 CClip::getDuration()
{
    return m_ptClipParam->m_sllDuration;
}
inline s32 CClip::getFilterCount()
{
    return m_vFilters.size();
}

inline s32 CClip::getTransitionCount()
{
    return m_vTransitions.size();
}

inline CClipParser *CClip::getParser()
{
    return m_pParser;
}

} // namespace paomiantv

#endif // _PAOMIANTV_CLIP_H_