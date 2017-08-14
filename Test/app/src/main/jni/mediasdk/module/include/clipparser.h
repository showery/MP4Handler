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

    class CClip;

    class CClipParser {
    public:
        CClipParser(CClip *pClip);

        virtual ~CClipParser();

        BOOL32 parse();

        void save(s8 *pchFilePath);

        BOOL
        getVidoeSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                           u64 &renderoffset,
                           BOOL &isSync);

        BOOL
        getVidoeSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                             u64 &renderoffset,
                             BOOL &isSync);

        BOOL
        getAudioSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                           u64 &renderoffset,
                           BOOL &isSync);

        BOOL
        getAudioSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime, u64 &duration,
                             u64 &renderoffset, BOOL &isSync);

        inline u32 getVSampleStartId();

        inline u32 getVSampleEndId();

        inline u32 getVSampleNum();

        inline u32 getASampleStartId();

        inline u32 getASampleEndId();

        inline u32 getASampleNum();

        inline BOOL getVideoSPS(u8 *&sps, u32 &size);

        inline BOOL getVideoPPS(u8 *&sps, u32 &size);

    private:
        CClip *m_pClip;
        s8 *m_pchLastSrc;

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

        MP4SampleId m_uVStartId;
        MP4SampleId m_uVEndId;

        MP4SampleId m_uAStartId;
        MP4SampleId m_uAEndId;

        u8 *m_pbySPS;
        u32 m_uSPSLen;
        u8 *m_pbyPPS;
        u32 m_uPPSLen;

    private:
        void initTrackId(const MP4TrackId uVTrackId, const u32 uSamplesNum, MP4SampleId &uVStartId,
                         MP4SampleId &uVEndId);

        void copyTrack(MP4TrackId trackId, MP4FileHandle pDstHandle, s64 startTime, s64 duration);

        void reset();

        void initSpsPps();

    };

    inline u32 CClipParser::getVSampleStartId() {
        return m_uVStartId;
    }

    inline u32 CClipParser::getVSampleEndId() {
        return m_uVEndId;
    }

    inline u32 CClipParser::getVSampleNum() {
        return (m_uVEndId - m_uVStartId + 1);
    }

    inline u32 CClipParser::getASampleStartId() {
        return m_uAStartId;
    }

    inline u32 CClipParser::getASampleEndId() {
        return m_uAEndId;
    }

    inline u32 CClipParser::getASampleNum() {
        return (m_uAEndId - m_uAStartId + 1);
    }

    inline BOOL CClipParser::getVideoSPS(u8 *&sps, u32 &size) {

        if(m_uSPSLen==0){
            return false;
        }else{
            sps = (u8*)malloc(m_uSPSLen);
            memcpy(sps,m_pbySPS,m_uSPSLen);
            size=m_uSPSLen;
            return true;
        }
    }

    inline BOOL CClipParser::getVideoPPS(u8 *&pps, u32 &size) {
        if(m_uPPSLen==0){
            return false;
        }else{
            pps = (u8*)malloc(m_uPPSLen);
            memcpy(pps,m_pbyPPS,m_uPPSLen);
            size=m_uPPSLen;
            return true;
        }
    }
}

#endif /* _PAOMIANTV_CLIPPARSE_H_ */
