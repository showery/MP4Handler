/// 视频帧处理模块头文件
/// 本模块实现视频帧的多层合成以及像素映射表方式的滤镜
/// 作者：杨方华
/// 创建日期：2017-8-8
/// 注意：特意使用中文注释，请勿修改

#ifndef __PAOMIANTV_FRAME_PROCESSOR_H__
#define __PAOMIANTV_FRAME_PROCESSOR_H__

#include <functional>
#include <vector>

namespace paomiantv
{

/// 图像像素格式
typedef enum {
    PIXEL_FORMAT_RGB = 0,
    PIXEL_FORMAT_RGBA,
    PIXEL_FORMAT_NV12, //只能作为输入格式，预留格式。
    PIXEL_FORMAT_YV12, //只能作为输入格式
    PIXEL_FORMAT_YUV444_INTERLEAVED, //只能作为输出格式
    PIXEL_FORMAT_COUNT,
} PixelFormat;

/// 图像数据类，
/// 建议使用对象池机制管理ImageData对象
class ImageData
{
public:
    ImageData()
    :pixelBuffer(nullptr)
    ,bufferSize(0)
    ,pixelFormat(0)
    ,width(0)
    ,height(0)
    ,bufferDestructor(std::bind(ImageData::deleteBuffer, std::placeholders::_1))
    {
        
    }
    
    ~ImageData()
    {
        reset();
    }
    
    static inline void deleteBuffer(unsigned char* buf)
    {
        if (buf) {
            delete []buf;
        }
    }
    
    void reset()
    {
        if (nullptr != bufferDestructor) {
            bufferDestructor(pixelBuffer);
        }
        bufferDestructor = std::bind(ImageData::deleteBuffer, std::placeholders::_1);
        pixelBuffer = nullptr;
        bufferSize = 0;
        pixelFormat = 0;
        width = 0;
        height = 0;
    }

public: ///对象池机制
    /// 从对象池中创建ImageData对象指针
    static ImageData* create();
    
    /// 将ImageData对象释放回对象池
    static void release(ImageData* imgData);
    
    /// 清空ImageData对象池
    static void clear();
    
public:
    unsigned char *pixelBuffer;
    unsigned int bufferSize;
    unsigned int pixelFormat;
    unsigned int width;
    unsigned int height;
    /// pixelBuffer的释放回调函数，
    ///默认为ImageData::deleteBuffer，若无需ImageData自动处理可赋值为nullptr
    std::function<void(unsigned char*)> bufferDestructor;
};

/// 视频帧分层数据类，
/// 建议使用对象池机制管理LayerData对象
class LayerData
{
public:
    LayerData()
    :layerImage(nullptr)
    ,mapImage(nullptr)
    ,imageDestructor(std::bind(LayerData::deleteImage, std::placeholders::_1))
    {
        for (int i=0; i<8; ++i) {
            positionData[i] = 0.0f;
        }
    }
    
    ~LayerData()
    {
        reset();
    }
    
    static inline void deleteImage(ImageData *img)
    {
        if (img) {
            delete img;
        }
    }
    
    void reset()
    {
        if (nullptr != imageDestructor) {
            imageDestructor(layerImage);
            imageDestructor(mapImage);
        }
        imageDestructor = std::bind(LayerData::deleteImage, std::placeholders::_1);
        layerImage = nullptr;
        mapImage = nullptr;
        for (int i=0; i<8; ++i) {
            positionData[i] = 0.0f;
        }
    }
    
public: ///对象池机制
    /// 从对象池中创建LayerData对象指针
    static LayerData* create();
    
    /// 将LayerData对象释放回对象池
    static void release(LayerData* layerData);
    
    /// 清空LayerData对象池
    static void clear();
    
public:
    ImageData *layerImage;
    ImageData *mapImage;
    float positionData[8];
    /// ImageData对象的释放回调函数
    /// 默认为LayerData::deleteImage, 若无需LayerData内部自动处理可赋为nullptr
    std::function<void(ImageData*)> imageDestructor;
};

/// 视频帧处理模块
/// 该模块API与线程绑定，只能在调用初始化(initialize)接口的线程中调用处理(processFrame)或绘制(drawFrame)接口
class FrameProcessor
{
public:
    /// 初始化本模块
    /// 在某线程执行处理过程之前需要调用此接口以初始化处理模块
    static bool initialize(int width, int height);
    
    /// 反初始化本模块
    /// 程序退出前建议调用此接口以销毁模块相关数据
    static void uninitialize();

    static bool isInitialized();
    
    /// 处理视频帧
    /// @param srcLayers 输入视频帧分层，层级排序从下到上(从底层到顶层)
    /// @param destImage 处理结果的输出图像，需要输入已分配好的内存块
    /// @see srcLayers中图像的可选像素格式为：
    ///     PIXEL_FORMAT_RGB
    ///     PIXEL_FORMAT_RGBA
    ///     PIXEL_FORMAT_YV12
    ///  destImage的可选像素格式为:
    ///     PIXEL_FORMAT_RGB
    ///     PIXEL_FORMAT_RGB
    ///     PIXEL_FORMAT_YUV444_INTERLEAVED
    static void processFrame(const std::vector<LayerData*>& srcLayers, ImageData *destImage);
    
    /// 绘制视频帧
    /// @param srcLayers 输入视频帧分层，层级排序从下到上(从底层到顶层)
    /// @see 调用此接口之前需要手动绑定FBO
    /// srcLayers中图像的可选像素格式为：
    ///     PIXEL_FORMAT_RGB
    ///     PIXEL_FORMAT_RGBA
    ///     PIXEL_FORMAT_YV12
    static void drawFrame(const std::vector<LayerData*>& srcLayers);

public:
    /// info级别日志的输出回调函数，默认为系统标准输出函数
    static std::function<void(const char*)> logInfoCallback;
    /// error级别日志的输出回调函数，默认为系统标准输出函数
    static std::function<void(const char*)> logErrorCallback;
};

} //namespace paomiantv
#endif