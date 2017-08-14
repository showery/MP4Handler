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

#include "constant.h"
#include "clipparser.h"
#include "common.h"

namespace paomiantv {

    CClipParser::CClipParser(CClip *pClip)
            : m_pClip(pClip) {
        m_Handle = MP4_INVALID_FILE_HANDLE;
        m_pchLastSrc = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_pbySPS = (u8 *) malloc(MAX_SPS_SIZE);
        m_pbyPPS = (u8 *) malloc(MAX_PPS_SIZE);
        m_uSPSLen = 0;
        m_uPPSLen = 0;
        memset(m_pchLastSrc, 0, MAX_LEN_FILE_PATH);
        memset(m_pbySPS, 0, MAX_SPS_SIZE);
        memset(m_pbyPPS, 0, MAX_PPS_SIZE);
        parse();
    }

    CClipParser::~CClipParser() {
        if (m_pchLastSrc != NULL) {
            free(m_pchLastSrc);
            m_pchLastSrc = NULL;
        }

        if (m_pbySPS != NULL) {
            free(m_pbySPS);
            m_pbySPS = NULL;
        }
        if (m_pbyPPS != NULL) {
            free(m_pbyPPS);
            m_pbyPPS = NULL;
        }

        if (MP4_IS_VALID_FILE_HANDLE(m_Handle)) {
            MP4Close(m_Handle);
            m_Handle = MP4_INVALID_FILE_HANDLE;
        }
    }

    void CClipParser::reset() {
        memset(m_pchLastSrc, 0, MAX_LEN_FILE_PATH);
        m_Handle = MP4_INVALID_FILE_HANDLE;

        m_uTimeScale = 0;
        m_uTrackNum = 0;

        m_uVTimeScale = 0;
        m_uVTrackId = 0;
        m_uVSampleNum = 0;
        m_uVSampleMaxSize = 0;
        m_ullDuration = 0;

        m_uATimeScale = 0;
        m_uATrackId = 0;
        m_uASampleNum = 0;
        m_uASampleMaxSize = 0;
        m_ullADuration = 0;

        m_uVStartId = 0;
        m_uVEndId = 0;

        m_uAStartId = 0;
        m_uAEndId = 0;
    }

    void CClipParser::initSpsPps() {

        const u8 nalHeader[] = {0x00, 0x00, 0x00, 0x01};
        // read sps/pps
        uint8_t **seqheader;
        uint8_t **pictheader;
        uint32_t *pictheadersize;
        uint32_t *seqheadersize;
        uint32_t ix;
        MP4GetTrackH264SeqPictHeaders(m_Handle, m_uVTrackId, &seqheader, &seqheadersize,
                                      &pictheader,
                                      &pictheadersize);

        //sps
        u32 count = 0;
        BOOL32 flag = TRUE;
        memcpy(m_pbySPS + count, nalHeader, 4);
        count += 4;
        u8 *temp = NULL;
        for (ix = 0; seqheadersize[ix] != 0; ix++) {

            if (!flag) {
                goto bailSPS;
            }

            if (count + seqheadersize[ix] > MAX_SPS_SIZE) {
                temp = (u8 *) realloc(m_pbySPS, count + seqheadersize[ix]);
                if (temp == NULL) {
                    flag = FALSE;
                    count = 0;
                    goto bailSPS;
                }
                m_pbySPS = temp;
            }
            memcpy(m_pbySPS + count, seqheader[ix], seqheadersize[ix]);
            count += seqheadersize[ix];
            bailSPS:
            free(seqheader[ix]);
        }
        m_uSPSLen = count;
        free(seqheader);
        free(seqheadersize);


        //pps
        count = 0;
        flag = TRUE;
        temp = NULL;
        memcpy(m_pbyPPS + count, nalHeader, 4);
        count += 4;
        for (ix = 0; pictheadersize[ix] != 0; ix++) {

            if (!flag) {
                goto bailPPS;
            }
            if (count + pictheadersize[ix] > MAX_PPS_SIZE) {
                temp = (u8 *) realloc(m_pbyPPS, count + pictheadersize[ix]);
                if (temp == NULL) {
                    flag = FALSE;
                    count = 0;
                    goto bailPPS;
                }
                m_pbyPPS = temp;
            }
            memcpy(m_pbyPPS + count, pictheader[ix], pictheadersize[ix]);
            count += pictheadersize[ix];
            bailPPS:
            free(pictheader[ix]);
        }
        m_uPPSLen = count;
        free(pictheader);
        free(pictheadersize);
    }

    BOOL32 CClipParser::parse() {
        BOOL32 re = FALSE;
        do {
            if (m_pClip == NULL || m_pClip->getSrc() == NULL || strlen(m_pClip->getSrc()) <= 0) {
                LOGE("clip parameter is invalid, parse failed!");
                break;
            }
            if (MP4_IS_VALID_FILE_HANDLE(m_Handle)) {
                if (strcmp(m_pClip->getSrc(), m_pchLastSrc) != 0) {
                    MP4Close(m_Handle);
                    m_Handle = MP4Read(m_pClip->getSrc());
                }
            } else {
                m_Handle = MP4Read(m_pClip->getSrc());
            }

            if (!MP4_IS_VALID_FILE_HANDLE(m_Handle)) {
                LOGE("read clip failed!");
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
                    initTrackId(m_uVTrackId, m_uVSampleNum, m_uVStartId, m_uVEndId);
                    initSpsPps();
                } else if (MP4_IS_AUDIO_TRACK_TYPE(trackType)) {
                    m_uATrackId = trackId;
                    m_uATimeScale = MP4GetTrackTimeScale(m_Handle, m_uATrackId);
                    m_uASampleMaxSize = MP4GetTrackMaxSampleSize(m_Handle, m_uATrackId);
                    m_uASampleNum = MP4GetTrackNumberOfSamples(m_Handle, m_uATrackId);
                    initTrackId(m_uATrackId, m_uASampleNum, m_uAStartId, m_uAEndId);
                }
            }
            if (m_uSPSLen == 0 || m_uPPSLen == 0) {
                break;
            }
            re = TRUE;
            strncpy(m_pchLastSrc, m_pClip->getSrc(), MAX_LEN_FILE_PATH);
        } while (0);

        if (!re) {
            reset();
        }
        return re;
    }

    void
    CClipParser::initTrackId(const MP4TrackId trackId, const u32 uSamplesNum, MP4SampleId &uStartId,
                             MP4SampleId &uEndId) {
        MP4Timestamp trackStartTime = MP4ConvertToTrackTimestamp(
                m_Handle,
                trackId,
                m_pClip->getStart(),
                MP4_MSECS_TIME_SCALE);

        MP4Duration trackDuration = MP4ConvertToTrackDuration(
                m_Handle,
                trackId,
                m_pClip->getDuration(),
                MP4_MSECS_TIME_SCALE);

        MP4Timestamp when = 0;
        MP4SampleId sampleId = MP4GetSampleIdFromTime(m_Handle, trackId, trackStartTime, true);
        uStartId = sampleId;
        while (true) {
            if (when >= trackDuration) {
                break;
            }
            if (sampleId > uSamplesNum) {
                break;
            }
            sampleId++;
            when += MP4GetSampleDuration(m_Handle, trackId, sampleId);
        }
        uEndId = --sampleId;
    }

    void CClipParser::copyTrack(
            MP4TrackId trackId,
            MP4FileHandle pDstHandle,
            s64 startTime,
            s64 duration) {
        if (!MP4_IS_VALID_TRACK_ID(trackId)) {
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

    BOOL CClipParser::getVidoeSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime,
                                         u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = MP4ReadSample(m_Handle, m_uVTrackId, nId, &buff, &size, &starttime, &duration,
                                &renderoffset, &isSync);
        if (re) {
            starttime = starttime * 1000000000 / m_uVTimeScale;
            duration = duration * 1000000000 / m_uVTimeScale;
            renderoffset = duration * 1000000000 / m_uVTimeScale;
            if (buff && size > 4) {
                //标准的264帧，前面几个字节就是frame的长度.
                //需要替换为标准的264 nal 头.
                buff[0] = 0x00;
                buff[1] = 0x00;
                buff[2] = 0x00;
                buff[3] = 0x01;
            }
        }
        return re;
    }

    BOOL CClipParser::getAudioSampleById(u32 nId, u8 *&buff, u32 &size, u64 &starttime,
                                         u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = MP4ReadSample(m_Handle, m_uATrackId, nId, &buff, &size, &starttime, &duration,
                                &renderoffset, &isSync);
        if (re) {
            starttime = starttime * 1000000000 / m_uVTimeScale;
            duration = duration * 1000000000 / m_uVTimeScale;
            renderoffset = duration * 1000000000 / m_uVTimeScale;
        }
        return re;
    }

    BOOL CClipParser::getAudioSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,
                                           u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = MP4ReadSampleFromTime(m_Handle, m_uATrackId, ullTimestamp, &buff, &size,
                                        &starttime,
                                        &duration,
                                        &renderoffset, &isSync);
        if (re) {
            starttime = starttime * 1000000000 / m_uVTimeScale;
            duration = duration * 1000000000 / m_uVTimeScale;
            renderoffset = duration * 1000000000 / m_uVTimeScale;
        }
        return re;
    }

    BOOL CClipParser::getVidoeSampleByTime(u64 ullTimestamp, u8 *&buff, u32 &size, u64 &starttime,
                                           u64 &duration, u64 &renderoffset, BOOL &isSync) {
        BOOL re = MP4ReadSampleFromTime(m_Handle, m_uVTrackId, ullTimestamp, &buff, &size,
                                        &starttime,
                                        &duration,
                                        &renderoffset, &isSync);
        if (re) {
            starttime = starttime * 1000000000 / m_uVTimeScale;
            duration = duration * 1000000000 / m_uVTimeScale;
            renderoffset = duration * 1000000000 / m_uVTimeScale;
        }
        return re;
    }
}
