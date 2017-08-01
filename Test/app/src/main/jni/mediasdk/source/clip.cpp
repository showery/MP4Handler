/*******************************************************************************
 *        Module: mediasdk
 *          File: clip.cpp
 * Functionality: clip entity.
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


#include <stdlib.h>
#include "constant.h"
#include "clip.h"
namespace paomiantv {

    CClip::CClip() {
        USE_LOG;
        m_pLock = new CLock;
        m_vFilters.clear();
        m_vTransitions.clear();
        m_pchSrcPath = (s8 *) malloc(MAX_LEN_FILE_PATH);
        m_ptClipParam = new TClipParam;
        m_pcVTrackParam = new CTrackParamVideo;
        m_pcATrackParam = new CTrackParamAudio;

    }

    CClip::~CClip() {
        USE_LOG;
        free(m_pchSrcPath);
        delete m_ptClipParam;
        delete m_pcVTrackParam;
        delete m_pcATrackParam;
        delete m_pLock;
    }

    void CClip::init(s8 *pchSrc, s64 sllStart, s64 sllDuration) {
        strncpy(m_pchSrcPath, pchSrc, MAX_LEN_FILE_PATH);
        m_ptClipParam->m_sllStart = sllStart;
        m_ptClipParam->m_sllDuration = sllDuration;
    }

    void CClip::uninit() {
    }

    s8 *CClip::getSrc() {
        return m_pchSrcPath;
    }

    void CClip::setSrc(s8 *pchSrc) {
        strncpy(m_pchSrcPath,pchSrc,MAX_LEN_FILE_PATH);
    }

    s64 CClip::getStart() {
        return m_ptClipParam->m_sllStart;
    }

    void CClip::setStart(s64 sllStart) {
        m_ptClipParam->m_sllStart = sllStart;
    }

    s64 CClip::getDuration() {
        return m_ptClipParam->m_sllDuration;
    }

    void CClip::setDuration(s64 sllDuration) {
        m_ptClipParam->m_sllDuration = sllDuration;
    }

    CFilter *CClip::getFilter(s32 nIndex) {
        return m_vFilters.at(nIndex);
    }

    BOOL32 CClip::addFilter(CFilter *pFilter) {
        m_vFilters.push_back(pFilter);
        return TRUE;
    }

    CFilter * CClip::removeFilter(s32 nIndex) {
        CFilter* pFilter = m_vFilters.at(nIndex);
        m_vFilters.erase(m_vFilters.begin() + nIndex);
        return pFilter;
    }

    s32 CClip::getFilterCount() {
        return m_vFilters.size();
    }

    BOOL32 CClip::addTransition(CTransition *pTransition) {
        m_vTransitions.push_back(pTransition);
        return TRUE;
    }

    CTransition* CClip::getTransition(s32 nIndex) {
        return m_vTransitions.at(nIndex);
    }

    CTransition* CClip::removeTransition(s32 nIndex) {

       CTransition* pTransition = m_vTransitions.at(nIndex);
           m_vTransitions.erase(m_vTransitions.begin() + nIndex);
        return pTransition;
    }

    s32 CClip::getTransitionCount() {
        return m_vTransitions.size();
    }



// MP4TrackId CClip::cropTrack(
//     MP4FileHandle srcFile,
//     MP4TrackId trackId,
//     MP4FileHandle dstFile,
//     s64 startTime,
//     s64 duration)
// {
//     MP4Timestamp trackStartTime = MP4ConvertToTrackTimestamp(
//         srcFile,
//         trackId,
//         startTime,
//         MP4_MSECS_TIME_SCALE);

//     MP4Duration trackDuration = MP4ConvertToTrackDuration(
//         srcFile,
//         trackId,
//         duration,
//         MP4_MSECS_TIME_SCALE);

//     MP4TrackId dstTrackId = MP4CloneTrack(srcFile, trackId, dstFile);

//     if (dstTrackId == MP4_INVALID_TRACK_ID)
//     {
//         return dstTrackId;
//     }

//     MP4Timestamp when = 0;
//     const MP4SampleId numSamples = MP4GetTrackNumberOfSamples(srcFile, trackId);
//     MP4SampleId sampleId = MP4GetSampleIdFromTime(srcFile, trackId, trackStartTime, true);

//     while (true)
//     {
//         if (when >= trackDuration)
//         {
//             break;
//         }
//         if (sampleId > numSamples)
//         {
//             break;
//         }
//         MP4Duration sampleDuration = MP4GetSampleDuration(srcFile, trackId, sampleId);
//         bool rc = MP4CopySample(
//             srcFile,
//             trackId,
//             sampleId,
//             dstFile,
//             dstTrackId,
//             sampleDuration);
//         if (!rc)
//         {
//             MP4DeleteTrack(dstFile, dstTrackId);
//             return MP4_INVALID_TRACK_ID;
//         }
//         sampleId++;
//         when += sampleDuration;
//     }

//     return dstTrackId;
// }
// void CClip::crop()
// {
//     MP4TrackId trackId = MP4_INVALID_TRACK_ID;
//     MP4FileHandle srcFile = MP4Modify(m_ptMp4CutterParam->pchSrcPath, 0);
//     if (!srcFile)
//     {
//         return;
//     }

//     MP4FileHandle dstFile = MP4_INVALID_FILE_HANDLE;

//     if (m_ptMp4CutterParam->pchDstPath)
//     {
//         dstFile = MP4Create(m_ptMp4CutterParam->pchDstPath, MP4_CREATE_64BIT_DATA | MP4_CREATE_64BIT_TIME);
//         MP4SetTimeScale(dstFile, MP4GetTimeScale(srcFile));
//     }

//     u32 numTracks = MP4GetNumberOfTracks(srcFile);
//     for (u32 i = 0; i < numTracks; i++)
//     {
//         trackId = MP4FindTrackId(srcFile, i);
//         clipTrack(srcFile, trackId, dstFile, m_ptMp4CutterParam->sllStart, m_ptMp4CutterParam->sllDuration);
//     }
//     MP4Close(srcFile);
//     if (dstFile != MP4_INVALID_FILE_HANDLE)
//     {
//         MP4Close(dstFile);
//     }
// }

} // namespace paomiantv
