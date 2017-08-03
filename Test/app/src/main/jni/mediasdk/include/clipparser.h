/*******************************************************************************
 *        Module: mediasdk
 *          File: 
 * Functionality: clip parser
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_CLIPPARSE_H_
#define _PAOMIANTV_CLIPPARSE_H_

#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"
#include "stdlib.h"
#include "clip.h"

namespace paomiantv {

    class CClipParser {
    public:
        CClipParser(CClip *pClip);

        virtual ~CClipParser();

        void save(s8 *pchFilePath);

        void
        getVidoeSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                           u64 &renderoffset,
                           BOOL &isSync);

        void
        getVidoeSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                             u64 &renderoffset,
                             BOOL &isSync);

        void
        getAudioSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                           u64 &renderoffset,
                           BOOL &isSync);

        void
        getAudioSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                             u64 &renderoffset, BOOL &isSync);

    private:
        CClip *m_pClip;


        MP4FileHandle m_Handle;
        u32 m_uTimeScale;
        u32 m_uTrackNum;

        u32 m_uVTimeScale;
        MP4TrackId m_uVTrackId;
        u32 m_uVSampleNum;
        u32 m_uVSampleMaxSize;
        u64 m_ullDuration;

        u32 m_uATimeScale;
        MP4TrackId m_uATrackId;
        u32 m_uASampleNum;
        u32 m_uASampleMaxSize;
        u64 m_ullADuration;


    private:
        BOOL32 init();

        void uninit();

        void copyTrack(MP4TrackId trackId, MP4FileHandle pDstHandle, s64 startTime, s64 duration);


    };
}

#endif /* _PAOMIANTV_CLIPPARSE_H_ */
