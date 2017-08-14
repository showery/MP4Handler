/// 视频帧处理模块头文件
/// 本模块实现视频帧的多层合成以及像素映射表方式的滤镜
/// 作者：杨方华
/// 创建日期：2017-8-8
/// 注意：特意使用中文注释，请勿修改

#ifndef __PAOMIANTV_FRAME_PROCESSOR_H__
#define __PAOMIANTV_FRAME_PROCESSOR_H__

#include <memory>
#include <vector>

namespace paomiantv {

typedef enum {
    PIXEL_FORMAT_RGB = 0,
    PIXEL_FORMAT_RGBA,
    PIXEL_FORMAT_NV12, //Only for Input, Reserved.
    PIXEL_FORMAT_YV12, //Only for Input.
    PIXEL_FORMAT_YUV444_INTERLEAVED, //Only for Output
    PIXEL_FORMAT_COUNT,
} PixelFormat;

/// 图像数据类
class ImageData
{
public:
    ImageData();

    virtual ~ImageData();

    /// 创建被智能指针管理的ImageData对象
    /// 内部已实现对象池机制，ImageData对象被LayerData管理并回收
    static std::shared_ptr<ImageData> create();

    /// 清空图像数据
    static void clear();

public:
    /// 被智能指针管理的内存块，可参考sharedbuffer.h
    std::shared_ptr<unsigned char> _pixelBuffer;
    unsigned int _bufferSize;
    unsigned int _pixelFormat;
    unsigned int _width;
    unsigned int _height;
};

/// 视频帧分层数据（原视频帧图像、字幕分层等）
class LayerData
{
public:
    LayerData();
    virtual ~LayerData();

    /// 创建被智能指针管理的LayerData对象
    /// 内部已实现对象池机制，需要手动调用releaseToPool回收对象
    static std::shared_ptr<LayerData> create();

    /// 回收LayerData对象
    static void releaseToPool(std::shared_ptr<LayerData> val);

    /// 清空分层数据
    void clear();

public:
    /// 当前分层图像数据对象，使用者无需手动释放该对象
    std::shared_ptr<ImageData> _layerImage;
    //PixelFormat of toneMap must be PIXEL_FORMAT_RGB
    /// 当前分层像素映射表数据对象
    /// 像素格式必须为PIXEL_FORMAT_RGB
    std::shared_ptr<ImageData> _toneMapImage;
    /// 当前图像分层四边形顶点数据，包含4个xy数据
    /// 顶点数据与ViewPort在同一坐标系（即窗口或渲染目标坐标系），
    /// 以窗口/渲染目标空间中的像素为单位
    float _positionData[8];
};

class FrameProcessor
{
public:
    static bool initialize();

    static void uninitialize();

    static void setViewPort(float x, float y, float width, float height);

    static bool processFrame(const std::vector<std::shared_ptr<LayerData> > &srcLayers, std::shared_ptr<ImageData> destImage);

    static void drawFrame(const std::vector<std::shared_ptr<LayerData> > &srcLayers);

private:
    FrameProcessor();
    ~FrameProcessor();
};

}

#endif