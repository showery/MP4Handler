/*******************************************************************************
 *        Module: mediasdk
 *          File: clipparse.cpp
 * Functionality: parse clip.
 *       Related: mp4v2
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/

#include "clipparser.h"

namespace paomiantv {


    CClipParser::CClipParser(CClip *pClip) {
        m_pClip = pClip;
        init();
    }

    CClipParser::~CClipParser() {
        uninit();
    }

    BOOL32 CClipParser::init() {
        BOOL32 re = FALSE;
        do {
            if (m_pClip == NULL || m_pClip->getSrc() == NULL || strlen(m_pClip->getSrc()) <= 0) {
                LOGE("clip parameter is invalid, parse failed!")
                break;
            }
            m_Handle = MP4Read(m_pClip->getSrc());
            if (m_Handle == NULL) {
                LOGE("read clip failed!")
                break;
            }
            m_uTimeScale = MP4GetTimeScale(m_Handle);
            m_uTrackNum = MP4GetNumberOfTracks(m_Handle);
            MP4TrackId trackId = MP4_INVALID_TRACK_ID;
            for (u32 i = 0; i < m_uTrackNum; i++) {
                trackId = MP4FindTrackId(m_Handle, i);
                const s8 *trackType = MP4GetTrackType(m_Handle, trackId);
                if (MP4_IS_VIDEO_TRACK_TYPE(trackType)) {
                    m_uVTrackId = trackId;
                    m_uVTimeScale = MP4GetTrackTimeScale(m_Handle, m_uVTrackId);
                    m_uVSampleMaxSize = MP4GetTrackMaxSampleSize(m_Handle, m_uVTrackId);
                    m_uVSampleNum = MP4GetTrackNumberOfSamples(m_Handle, m_uVTrackId);
                } else if (MP4_IS_AUDIO_TRACK_TYPE(trackType)) {
                    m_uATrackId = trackId;
                    m_uATimeScale = MP4GetTrackTimeScale(m_Handle, m_uATrackId);
                    m_uASampleMaxSize = MP4GetTrackMaxSampleSize(m_Handle, m_uATrackId);
                    m_uASampleNum = MP4GetTrackNumberOfSamples(m_Handle, m_uATrackId);
                }
            }
            re = TRUE;
        } while (0);

        return re;
    }

    void CClipParser::uninit() {
        if (m_Handle != NULL) {
            MP4Close(m_Handle);
        }
    }


    void CClipParser::copyTrack(
            MP4TrackId trackId,
            MP4FileHandle pDstHandle,
            s64 startTime,
            s64 duration) {
        if(!MP4_IS_VALID_TRACK_ID(trackId)){
            return;
        }
        MP4Timestamp trackStartTime = MP4ConvertToTrackTimestamp(
                m_Handle,
                trackId,
                startTime,
                MP4_MSECS_TIME_SCALE);

        MP4Duration trackDuration = MP4ConvertToTrackDuration(
                m_Handle,
                trackId,
                duration,
                MP4_MSECS_TIME_SCALE);

        MP4TrackId dstTrackId = MP4CloneTrack(m_Handle, trackId, pDstHandle);

        if (!MP4_IS_VALID_TRACK_ID(dstTrackId)) {
            return;
        }

        MP4Timestamp when = 0;
        const MP4SampleId numSamples = MP4GetTrackNumberOfSamples(m_Handle, trackId);
        MP4SampleId sampleId = MP4GetSampleIdFromTime(m_Handle, trackId, trackStartTime, true);

        while (true) {
            if (when >= trackDuration) {
                break;
            }
            if (sampleId > numSamples) {
                break;
            }
            MP4Duration sampleDuration = MP4GetSampleDuration(m_Handle, trackId, sampleId);
            bool rc = MP4CopySample(
                    m_Handle,
                    trackId,
                    sampleId,
                    pDstHandle,
                    dstTrackId,
                    sampleDuration);
            if (!rc) {
                MP4DeleteTrack(pDstHandle, dstTrackId);
                break;
            }
            sampleId++;
            when += sampleDuration;
        }
    }

    void CClipParser::save(s8 *pchFilePath) {

        MP4FileHandle dstHandle = MP4_INVALID_FILE_HANDLE;
        if (pchFilePath == NULL || strlen(pchFilePath) <= 0) {
            return;
        }

        dstHandle = MP4Create(pchFilePath, MP4_CREATE_64BIT_DATA | MP4_CREATE_64BIT_TIME);
        if (!MP4_IS_VALID_FILE_HANDLE(dstHandle)) {
            return;
        }

        MP4SetTimeScale(dstHandle, m_uTimeScale);
        for (u32 i = 0; i < m_uTrackNum; i++) {
            copyTrack(MP4FindTrackId(m_Handle, i), dstHandle, m_pClip->getStart(),
                      m_pClip->getDuration());
        }
        MP4Close(dstHandle);
    }

    void CClipParser::getVidoeSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime,
                                         u64 &duration, u64 &renderoffset, BOOL &isSync) {
        MP4ReadSample(m_Handle, m_uVTrackId, nId, &buff, &size, &starttime, &duration,
                      &renderoffset, &isSync);
    }

    void CClipParser::getAudioSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime,
                                         u64 &duration, u64 &renderoffset, BOOL &isSync) {
        MP4ReadSample(m_Handle, m_uATrackId, nId, &buff, &size, &starttime, &duration,
                      &renderoffset, &isSync);
    }

    void CClipParser::getAudioSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,
                                           u64 &duration, u64 &renderoffset, BOOL &isSync) {
        MP4ReadSampleFromTime(m_Handle, m_uATrackId, ullTimestamp, &buff, &size, &starttime,
                              &duration,
                              &renderoffset, &isSync);

    }

    void CClipParser::getVidoeSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,
                                           u64 &duration, u64 &renderoffset, BOOL &isSync) {
        MP4ReadSampleFromTime(m_Handle, m_uVTrackId, ullTimestamp, &buff, &size, &starttime,
                              &duration,
                              &renderoffset, &isSync);

    }

}
