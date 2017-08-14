#ifndef _PAOMIANTV_CONTSTANT_H_
#define _PAOMIANTV_CONTSTANT_H_

#define MAX_LEN_FILE_PATH 512
#define MAX_AUDIO_FRAME_BUFFER_SIZE 1024 * 2 * 2 //Stereo, aac, 16bit

#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080
#define MAX_VIDEO_FRAME_BUFFER_SIZE (MAX_WIDTH * MAX_HEIGHT * 3 / 2) //YUV420

#define MAX_SPS_SIZE 512
#define MAX_PPS_SIZE 512
//#define  MAX_H264_FRAME_SIZE            300000
//#define  MAX_H264_HEADER_SIZE           1000
#define BLOCK_QUEUE_SIZE 30

//decoder
#define VIDEO_MIME_TYPE "video/avc"
#define AUDIO_MIME_TYPE "audio/mp4a-latm"
#endif