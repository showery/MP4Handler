/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: transform audio/video parameters.
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

#ifndef _PAOMIANTV_TRANS_PARAM_H_
#define _PAOMIANTV_TRANS_PARAM_H_

#include "typedef.h"

namespace paomiantv
{
typedef struct tagVSecdPicParam
{
    //! the width of secondary picture.
    s32 m_nWidth;
    //! the height of secondary picture.
    s32 m_nHeight;
    //! the position X in base picture for persentation(%)
    float *m_fX;
    //! the position Y in base picture for persentation(%)
    float *m_fY;
    //! the degree for rotation(0.0-360.0)
    float *m_fDegree;
    //! the scale
    float *m_fScale;
    //! the image for picture synthesis
    u8 *m_pbySecondaryPic;
} TVSecdPicParam;

typedef struct tagVTextParam
{
    //! the position X in base picture for persentation(%)
    float *m_fX;
    //! the position Y in base picture for persentation(%)
    float *m_fY;
    //! the degree for rotation(0.0-360.0)
    float *m_fDegree;
    //! the scale
    float *m_fScale;
    //! the text for picture;
    s8 *m_pchText;
} TVTextParam;

typedef struct tagVTransParam
{
    //! the output width of picture.
    s32 m_nWidth;
    //! the out height of picture.
    s32 m_nHeight;
    //! the transfrom matix for filter and transition
    u8 *m_pchMatrix;
    //! the image for picture synthesis
    TVSecdPicParam *m_ptVSecdPicParam;
    //! the text for picture;
    TVTextParam *m_ptVTextParam;
} TVTransParam;

typedef struct tagASecdToneParam
{
    //! the out channels of audio track.
    s32 m_nChannel;
    //! the out frequence re of audio track. 8k/22k/44.1k etc
    s32 m_nFrequency;
    //! the out precision of audio track. 8bit/16bit/32bit etc
    s32 m_nPrecision;
    //! the text for picture;
    u8 *m_pbySecondaryTone;
} TASecdToneParam;

typedef struct tagATransParam
{
  public:
    //! the out channels of audio track.
    s32 m_nChannel;
    //! the out frequence re of audio track. 8k/22k/44.1k etc
    s32 m_nFrequency;
    //! the out precision of audio track. 8bit/16bit/32bit etc
    s32 m_nPrecision;
    //! the out precision of audio track. 8bit/16bit/32bit etc
    TASecdToneParam *m_ptASecdToneParam;
} TATransParam;
} // namespace paomiantv

#endif // _PAOMIANTV_TRANS_PARAM_H_