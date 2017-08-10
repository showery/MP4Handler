#include "sharedbuffer.h"
#include "frameprocessor.h"

static std::shared_ptr<ImageData> _getVideoImage0(unsigned int width, unsigned int height)
{
    std::shared_ptr<ImageData> videoImage = ImageData::create();
    videoImage->_bufferSize = width * height + width * height / 4;
    videoImage->_pixelBuffer = allocSharedArray<unsigned char>(videoImage->_bufferSize);
    videoImage->_pixelFormat = PIXEL_FORMAT_YV12;
    videoImage->_width = width;
    videoImage->_height = height;
    /*填充视频帧数据块*/

    return videoImage;
}

static std::shared_ptr<ImageData> _getMapImage0()
{
    unsigned int width = 512;
    unsigned int height = 512;
    std::shared_ptr<ImageData> mapImage = ImageData::create();
    mapImage->_bufferSize = width * height * 3;
    mapImage->_pixelBuffer = allocSharedArray<unsigned char>(mapImage->_bufferSize);
    mapImage->_pixelFormat = PIXEL_FORMAT_RGB;
    mapImage->_width = width;
    mapImage->_height = height;
    /*填充像素映射表数据块*/

    return mapImage;
}

static std::shared_ptr<ImageData> _getLabelImage0(unsigned int width, unsigned int height)
{
    std::shared_ptr<ImageData> textImage = ImageData::create();
    textImage->_bufferSize = width * height * 3;
    textImage->_pixelBuffer = allocSharedArray<unsigned char>(textImage->_bufferSize);
    textImage->_pixelFormat = PIXEL_FORMAT_RGB;
    textImage->_width = width;
    textImage->_height = height;
    /*填充字幕图像数据块*/

    return textImage;
}

static void test0()
{
    //Video Layer
    unsigned int videoWidth = 1280;
    unsigned int videoHeight = 720;
    std::shared_ptr<ImageData> videoImage = _getVideoImage0(videoWidth, videoHeight);
    std::shared_ptr<ImageData> mapImage = _getMapImage0();
    
    std::shared_ptr<LayerData> videoLayer = LayerData::create();
    videoLayer->_layerImage = videoImage;
    videoLayer->_toneMapImage = mapImage;
    videoLayer->_positionData[0] = 0.0f;
    videoLayer->_positionData[1] = (float)videoHeight;
    videoLayer->_positionData[2] = 0.0f;
    videoLayer->_positionData[3] = 0.0f;
    videoLayer->_positionData[4] = (float)videoWidth;
    videoLayer->_positionData[5] = (float)0;
    videoLayer->_positionData[6] = (float)videoWidth;
    videoLayer->_positionData[7] = (float)videoHeight;

    //Label layer
    unsigned int labelWidth = 128;
    unsigned int labelHeight = 32;
    std::shared_ptr<ImageData> labelImage0 = _getLabelImage0(128, 32);
    
    std::shared_ptr<LayerData> labelLayer = LayerData::create();
    labelLayer->_layerImage = labelImage0;
    labelLayer->_mapImage = nullptr;
    labelLayer->_positionData[0] = (float)(videoWidth - labelWidth) / 2.0f;
    labelLayer->_positionData[1] = labelHeight;
    labelLayer->_positionData[2] = (float)(videoWidth - labelWidth) / 2.0f;
    labelLayer->_positionData[3] = 0.0f;
    labelLayer->_positionData[4] = (float)(videoWidth + labelWidth) / 2.0f;
    labelLayer->_positionData[5] = 0.0f;
    labelLayer->_positionData[7] = (float)(videoWidth + labelWidth) / 2.0f;
    labelLayer->_positionData[8] = labelHeight;

    //Dest buffer
    unsigned int destWidth = 1280;
    unsigned int destHeight = 720;
    std::shared_ptr<ImageData> destImage = ImageData::create();
    destImage->_bufferSize = destWidth * destHeight * 3;
    destImage->_pixelBuffer = allocSharedArray<unsigned char>(destImage->_bufferSize);
    destImage->_pixelFormat = PIXEL_FORMAT_YUV444_INTERLEAVED;
    destImage->_width = destWidth;
    destImage->_height = destHeight;

    FrameProcessor::setViewPort(0, 0, videoWidth, videoHeight);
    std::vector<std::shared_ptr<LayerData> > layers;
    layers.push_back(videoLayer);
    layers.push_back(labelLayer);
    FrameProcessor::processFrame(layers, destImage);

    LayerData::releaseToPool(videoLayer);
    LayerData::releaseToPool(labelLayer);
}
