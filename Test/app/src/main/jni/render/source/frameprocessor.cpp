#include "frameprocessor.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#if defined(__ANDROID__)
#include <android/log.h>
#include<egl/egl.h>
#else //iOS
//TODO:Reserved for iOS
#endif

namespace paomiantv
{

/////////////////////////////////////////////////////////////////
/// class ImageData

    static std::vector<ImageData*> s_imagePool;

    ImageData* ImageData::create()
    {
        if (!s_imagePool.empty()) {
            ImageData *imgData = s_imagePool.back();
            s_imagePool.pop_back();
            return imgData;
        } else {
            return new ImageData();
        }
    }
        
    void ImageData::release(ImageData *imgData)
    {
        if (imgData) {
            s_imagePool.push_back(imgData);
        }
    }
        
    void ImageData::clear()
    {
        for (size_t i=0; i<s_imagePool.size(); ++i) {
            if (s_imagePool[i]) {
                delete s_imagePool[i];
            }
        }
        s_imagePool.clear();
    }

/////////////////////////////////////////////////////////////////
/// class LayerData

    static std::vector<LayerData*> s_layerPool;
        
    LayerData *LayerData::create()
    {
        if (!s_layerPool.empty()) {
            LayerData *layerData = s_layerPool.back();
            s_layerPool.pop_back();
            return layerData;
        } else {
            return new LayerData();
        }
    }
        
    void LayerData::release(LayerData *layerData)
    {
        if (layerData) {
            s_layerPool.push_back(layerData);
        }
    }

    void LayerData::clear()
    {
        for (size_t i=0; i<s_layerPool.size(); ++i) {
            if (s_layerPool[i]) {
                delete s_layerPool[i];
            }
        }
        s_layerPool.clear();
    }

/////////////////////////////////////////////////////////////////
/// Shader

#ifndef TOSTRING
#define TOSTRING(v) #v
#endif

    typedef enum ShaderType
    {
        SHADER_RGB2RGB = 0,
        SHADER_RGB2YUV,
        SHADER_YUV2RGB,
        SHADER_YUV2YUV
    };
    static std::unordered_map<int, const char*> s_defaultFragments;
    static std::unordered_map<int, const char*> s_toneMapFragments;

    //Vertex
    static const char *s_vertexShader = TOSTRING(
        attribute vec4 a_position;
        attribute vec2 a_texCoord;
        varying mediump vec2 v_texCoord;
        
        void main()
        {
            gl_Position = a_position;
            v_texCoord = a_texCoord;
        }
    );

    ///DefaultFragments
    s_defaultFragments[SHADER_RGB2RGB] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_texture;
        void main()
        {
            gl_FragColor = texture2D(u_texture, v_texCoord);
        }
    );

    s_defaultFragments[SHADER_RGB2YUV] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_texture;
        void main()
        {
            vec4 rgbColor = texture2D(u_texture, v_texCoord);
            vec4 yuvColor = vec4(0, 0, 0, 1);
            yuvColor.r = 0.299 * rgbColor.r + 0.587 * rgbColor.g + 0.114 * rgbColor.b;
            yuvColor.g = -0.169 * rgbColor.r - 0.331 * rgbColor.g + 0.5 * rgbColor.b + 0.5;
            yuvColor.b = 0.5 * rgbColor.r - 0.419 * rgbColor.g - 0.081 * rgbColor.b + 0.5;
            gl_FragColor = yuvColor;
        }
    );

    s_defaultFragments[SHADER_YUV2RGB] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_yTexture;
        uniform sampler2D u_uTexture;
        uniform sampler2D u_vTexture;
        void main()
        {
            float y = texture2D(u_yTexture, v_texCoord).x;
            float u = texture2D(u_uTexture, v_texCoord).x;
            float v = texture2D(u_vTexture, v_texCoord).x;

            vec4 rgbColor = vec4(0, 0, 0, 1);
            rgbColor.r = y + 1.13983 * (v - 0.5);
            rgbColor.g = y - 0.39465 * (u - 0.5) - 0.58060 * (v - 0.5);
            rgbColor.b = y + 2.03211 * (u - 0.5);

            gl_FragColor = rgbColor;
        }
    );

    s_defaultFragments[SHADER_YUV2YUV] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_yTexture;
        uniform sampler2D u_uTexture;
        uniform sampler2D u_vTexture;
        void main()
        {
            vec4 yuvColor = (0, 0, 0, 1);

            yuvColor.r = texture2D(u_yTexture, v_texCoord).x;
            yuvColor.g = texture2D(u_uTexture, v_texCoord).x;
            yuvColor.b = texture2D(u_vTexture, v_texCoord).x;

            gl_FragColor = yuvColor;
        }
    );

    ///Tone Map Fragments
    s_toneMapFragments[SHADER_RGB2RGB] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_texture;
        uniform sampler2D u_toneMap;

        void mapColor(inout vec4 pixelColor)
        {
            pixelColor = clamp(pixelColor, 0.0, 1.0);
            
            highp float blueColor = pixelColor.b * 63.0;

            highp vec2 coord1;
            coord1.y = floor(floor(blueColor) / 8.0);
            coord1.x = floor(blueColor) - (quad1.y * 8.0);

            highp vec2 coord2;
            coord2.y = floor(ceil(blueColor) / 8.0);
            coord2.x = ceil(blueColor) - (coord2.y * 8.0);

            highp vec2 texPos1;
            texPos1.x = clamp((quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos1.y = clamp((quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec2 texPos2;
            texPos2.x = clamp((quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos2.y = clamp((quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec4 newColor1 = texture2D(u_toneMap, texPos1);
            highp vec4 newColor2 = texture2D(u_toneMap, texPos2);

            pixelColor = mix(newColor1, newColor2, fract(blueColor));
        }

        void main()
        {
            highp vec4 oriColor = texture2D(u_texture, v_texCoord);
            mapColor(oriColor);
            gl_FragColor = oriColor;
        }
    );

    s_toneMapFragments[SHADER_RGB2YUV] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_texture;
        uniform sampler2D u_toneMap;

        void mapColor(inout vec4 pixelColor)
        {
            pixelColor = clamp(pixelColor, 0.0, 1.0);
            
            highp float blueColor = pixelColor.b * 63.0;

            highp vec2 coord1;
            coord1.y = floor(floor(blueColor) / 8.0);
            coord1.x = floor(blueColor) - (quad1.y * 8.0);

            highp vec2 coord2;
            coord2.y = floor(ceil(blueColor) / 8.0);
            coord2.x = ceil(blueColor) - (coord2.y * 8.0);

            highp vec2 texPos1;
            texPos1.x = clamp((quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos1.y = clamp((quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec2 texPos2;
            texPos2.x = clamp((quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos2.y = clamp((quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec4 newColor1 = texture2D(u_toneMap, texPos1);
            highp vec4 newColor2 = texture2D(u_toneMap, texPos2);

            pixelColor = mix(newColor1, newColor2, fract(blueColor));
        }

        void main()
        {
            highp vec4 rgbColor = texture2D(u_texture, v_texCoord);
            mapColor(rgbColor);

            vec4 yuvColor = vec4(0, 0, 0, 1);
            yuvColor.r = 0.299 * rgbColor.r + 0.587 * rgbColor.g + 0.114 * rgbColor.b;
            yuvColor.g = -0.169 * rgbColor.r - 0.331 * rgbColor.g + 0.5 * rgbColor.b + 0.5;
            yuvColor.b = 0.5 * rgbColor.r - 0.419 * rgbColor.g - 0.081 * rgbColor.b + 0.5;
            
            gl_FragColor = yuvColor;
        }
    );

    s_toneMapFragments[SHADER_YUV2RGB] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_yTexture;
        uniform sampler2D u_uTexture;
        uniform sampler2D u_vTexture;
        uniform sampler2D u_toneMap;

        void mapColor(inout vec4 pixelColor)
        {
            pixelColor = clamp(pixelColor, 0.0, 1.0);
            
            highp float blueColor = pixelColor.b * 63.0;

            highp vec2 coord1;
            coord1.y = floor(floor(blueColor) / 8.0);
            coord1.x = floor(blueColor) - (quad1.y * 8.0);

            highp vec2 coord2;
            coord2.y = floor(ceil(blueColor) / 8.0);
            coord2.x = ceil(blueColor) - (coord2.y * 8.0);

            highp vec2 texPos1;
            texPos1.x = clamp((quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos1.y = clamp((quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec2 texPos2;
            texPos2.x = clamp((quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos2.y = clamp((quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec4 newColor1 = texture2D(u_toneMap, texPos1);
            highp vec4 newColor2 = texture2D(u_toneMap, texPos2);

            pixelColor = mix(newColor1, newColor2, fract(blueColor));
        }

        void main()
        {
            highp float y = texture2D(u_yTexture, v_texCoord).x;
            highp float u = texture2D(u_uTexture, v_texCoord).x;
            highp float v = texture2D(u_vTexture, v_texCoord).x;

            highp vec4 rgbColor = vec4(0, 0, 0, 1);
            rgbColor.r = y + 1.13983 * (v - 0.5);
            rgbColor.g = y - 0.39465 * (u - 0.5) - 0.58060 * (v - 0.5);
            rgbColor.b = y + 2.03211 * (u - 0.5);

            mapColor(rgbColor);
            gl_FragColor = rgbColor;
        }
    );

    s_toneMapFragments[SHADER_YUV2YUV] = TOSTRING(
        varying mediump vec2 v_texCoord;
        uniform sampler2D u_yTexture;
        uniform sampler2D u_uTexture;
        uniform sampler2D u_vTexture;
        uniform sampler2D u_toneMap;

        void mapColor(inout vec4 pixelColor)
        {
            pixelColor = clamp(pixelColor, 0.0, 1.0);
            
            highp float blueColor = pixelColor.b * 63.0;

            highp vec2 coord1;
            coord1.y = floor(floor(blueColor) / 8.0);
            coord1.x = floor(blueColor) - (quad1.y * 8.0);

            highp vec2 coord2;
            coord2.y = floor(ceil(blueColor) / 8.0);
            coord2.x = ceil(blueColor) - (coord2.y * 8.0);

            highp vec2 texPos1;
            texPos1.x = clamp((quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos1.y = clamp((quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec2 texPos2;
            texPos2.x = clamp((quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.r), 0.0, 1.0);
            texPos2.y = clamp((quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * pixelColor.g), 0.0, 1.0);

            highp vec4 newColor1 = texture2D(u_toneMap, texPos1);
            highp vec4 newColor2 = texture2D(u_toneMap, texPos2);

            pixelColor = mix(newColor1, newColor2, fract(blueColor));
        }

        void main()
        {
            highp float y = texture2D(u_yTexture, v_texCoord).x;
            highp float u = texture2D(u_uTexture, v_texCoord).x;
            highp float v = texture2D(u_vTexture, v_texCoord).x;

            highp vec4 rgbColor = vec4(0, 0, 0, 1);
            rgbColor.r = y + 1.13983 * (v - 0.5);
            rgbColor.g = y - 0.39465 * (u - 0.5) - 0.58060 * (v - 0.5);
            rgbColor.b = y + 2.03211 * (u - 0.5);

            mapColor(rgbColor);

            vec4 yuvColor = vec4(0, 0, 0, 1);
            yuvColor.r = 0.299 * rgbColor.r + 0.587 * rgbColor.g + 0.114 * rgbColor.b;
            yuvColor.g = -0.169 * rgbColor.r - 0.331 * rgbColor.g + 0.5 * rgbColor.b + 0.5;
            yuvColor.b = 0.5 * rgbColor.r - 0.419 * rgbColor.g - 0.081 * rgbColor.b + 0.5;

            gl_FragColor = yuvColor;
        }
    );

/////////////////////////////////////////////////////////////////
/// class ProgramData

class ProgramData
    {
    protected:
        ProgramData()
            :_program(0)
        {

        }

    public:
        ~ProgramData()
        {
            if (0 != _program) {
                glDeleteProgram(_program);
                _program = 0;
            }
        }

        static std::shared_ptr<ProgramData> create(const char *vertexCode, const char *fragmentCode);

        void use();
    
    protected:
        bool _init(const char *vertexCode, const char *fragmentCode);

        GLuint _compileShader(GLenum type, const char *shaderCode);

        bool _parseLocations();

        bool _parseUnifroms();

        GLuint _createVBO(GLint size, GLenum type);

    public:
        GLuint _program;
        std::unordered_map<std::string, GLuint> _attrMap;
        std::unordered_map<std::string, GLuint> _vboMap;
        std::unordered_map<std::string, GLuint> _uniformMap;
    }

    std::shared_ptr<ProgramData> ProgramData::create(const char *vertexCode, const char *fragmentCode)
    {
        std::shared_ptr<ProgramData> sharedThis(new ProgramData());
        if (sharedThis->_init(vertexCode, fragmentCode)) {
            return sharedThis;
        }
        return nullptr;
    }

    bool ProgramData::_init(const char *vertexCode, const char *fragmentCode)
    {
        GLuint vertexShader = 0;
        vertexShader = LoadShader(GL_VERTEX_SHADER, vertexCode);
        if (0 == vertexShader) {
            return 0;
        }
        fragmentShader = LoadShader(GL_VERTEX_SHADER, fragmentCode);
        if (0 == fragmentShader) {
            glDeleteShader(vertexShader);
            return 0;
        }

        GLuint curProgram = glCreateProgram();
        glAttachShader(curProgram, vertexShader);
        glAttachShader(curProgram, fragmentShader);
        glLinkProgram(curProgram);

        glDeleteShader(vertexShader);
        vertexShader = 0;
        glDeleteShader(fragmentShader);
        fragmentShader = 0;

        GLint status = 0;
        glGetProgramiv(curProgram, GL_LINK_STATUS, &status);
        if (GL_FALSE == status) {
            glDeleteProgram(curProgram);
            curProgram = 0;
            return false;
        }

        _program = curProgram;

        _parseLocations();
        _parseUnifroms();

        return true;
    }

    GLuint ProgramData::_compileShader(GLenum type, const char *shaderCode)
    {
        GLuint shader;
        GLint compiled;

        shader = glCreateShader(type);
        if (shader == 0) {
            FrameProcessor::logErrorCallback("create shader failed!");
            return 0;
        }

        glShaderSource(shader, 1, &shaderSrc, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1)
            {
                char *infoLog = new char[infoLen];
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                FrameProcessor::logErrorCallback(infoLog);
                delete [] infoLog;
            }

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    bool ProgramData::_parseLocations()
    {
        GLint attrCount = 0;
        glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &attrCount);
        if (0 < attrCount) {
            GLint len = 0;
            glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &len);
            if (0 < len) {
                GLchar attrName[64] = {0};
                GLint attrSize = 0;
                GLenum attrType = 0;

                for (GLint i=0; i<attrCount; ++i) {
                    glGetActiveAttrib(_program, i, len, nullptr, &attrSize, &attrType, attrName);
                    attrName[len] = '\0';
                    _attrMap[attrName] = glGetAttribLocation(_program, attrName);
                }
            }
        } else {
            return false;
        }
    }

    bool ProgramData::_parseUnifroms()
    {
        GLint uniformCount = 0;
        glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &uniformCount);
        if (0 < uniformCount) {
            GLint len = 0;
            glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &len);
            if (0 < len) {
                GLchar uniformName[64] = {0};
                GLint uniformSize;
                GLenum uniformType;

                for (GLint i=0; i<uniformCount; ++i) {
                    glGetActiveUniform(_program, i, len, nullptr, &uniformSize, &uniformType, uniformName);
                    uniformName[len] = {0};
                    _uniformMap[uniformName] = glGetUniformLocation(_program, uniformName);
                }
            }
        } else {
            return false;
        }

        return true;
    }

    void ProgramData::use()
    {
        glUseProgram(_program);
    }

/////////////////////////////////////////////////////////////////
/// class QuadRenderer

static const MIN_CACHE_TEXTURE_SIZE = 256;

    class QuadRenderer
    {
    public:
        QuadRenderer();
        virtual ~QuadRenderer();

        static bool initialize();

        static void destroy()

        static void setViewPort(float x, float y, float width, float height);

        static void clearColor(float r, float g, float b, float a);

        static void setRenderTarget(std::shared_ptr<ImageData> imgData);

        static void readRenderTarget(std::shared_ptr<ImageData> destImage);

        static void restoreRenderTarget();

        static void drawLayer(std::shared_ptr<LayerData> curLayer, bool isYUVTarget);

        static void resetUsingTextures();

    protected:
        static bool _initContext();
        static bool _initPrograms();
        static bool _initVertex();
        static void _destroyContext();

        static uint64_t _makeTextureKey(GLint internalFormat, GLint width, GLint height);
        static void _parseTextureKey(uint64_t key, GLint *internalFormat, GLint *width, GLint *height);
        static GLuint _genTexture(GLint internalFormat, GLint width, GLint height);
        static GLuint _popTexture(GLint internalFormat, GLint width, GLint height);
        static void _pushTexture(uint64_t key, GLuint tex);
        static void _useTexture(GLuint key, GLuint tex);
        static GLuint _uploadToTexture(unsigned char *srcBuffer, GLint internalFormat, GLint width, GLint height);
        static std::vector<Gluint> _getTextureFromData(std::shared_ptr<ImageData> imgData);

        static void _uploadPositionData(float *srcData, int srcStride);
        static void _adjustPositionData(float *srcData, int srcStride, float *destData, int destStride);

    protected:
        static EGLDisplay _display;
        static EGLContext _context;
        static EGLSurface _surface;

    protected:
        static std::unordered_map<int, std::shared_ptr<ProgramData> > _defaultPrograms;
        static std::unordered_map<int, std::shared_ptr<ProgramData> > _toneMapPrograms;
        static std::unordered_map<uint64_t, std::vector<GLuint> > _texturePool;
        static std::unordered_map<uint64_t, std::vector<GLuint> > _usingTextures;
        static float _viewPort[4];
        static float _positionData[4 * 2];
        static float _uvData[4 * 2];
        static GLuint _posVBO;
        static GLuint _coordVBO;
        static GLuint _fbo;
        static GLuint _prevFbo;
        static bool _initialized;
    };

    ///Static members
    EGLDisplay QuadRenderer::_display = EGL_NO_DISPLAY;
    EGLContext QuadRenderer::_context = EGL_NO_CONTEXT;
    EGLSurface QuadRenderer::_surface = EGL_NO_SURFACE;

    std::unordered_map<int, std::shared_ptr<ProgramData> > QuadRenderer::_defaultPrograms;
    std::unordered_map<int, std::shared_ptr<ProgramData> > QuadRenderer::_toneMapPrograms;
    std::unordered_map<uint64_t, std::vector<GLuint> > QuadRenderer::_texurePool;
    std::unordered_map<uint64_t, std::vector<GLuint> > QuadRenderer::_usingTextures;
    std::vector<GLuint> _tempTextures;
    float QuadRenderer::_viewPort[4];
    float QuadRenderer::_positionData[4 * 2];
    float QuadRenderer::_uvData[4 * 2];
    GLuint QuadRenderer::_posVBO = 0;
    GLuint QuadRenderer::_coordVBO = 0;
    GLuint QuadRenderer::_fbo = 0;
    GLuint QuadRenderer::_prevFbo = 0;
    GLuint QuadRenderer::_initialized = false;

    bool QuadRenderer::initialize()
    {
        if (!_initPrograms()) {
            return false;
        }
        if (!_initVertex()) {
            return false;
        }

        glGenFramebuffers(1, &_fbo);

        _initialized = true;
        return true;
    }

    void QuadRenderer::destroy()
    {
        if (!_initialized) {
            return;
        }

        _defaultPrograms.clear();
        _toneMapPrograms.clear();
        for (auto pool : _texturePool) {
            for (size_t i=0; i<pool->second.size(); ++i) {
                GLuint tex = pool->second[i];
                glDeleteTextures(1, &tex);
            }
        }
        _texturePool.clear();
        
        if (0 != _posVBO) {
            glDeleteBuffers(1, &_posVBO);
            _posVBO = 0;
        }

        if (0 != _uvVBO) {
            glDeleteBuffers(1, &_uvVBO);
            _uvVBO = 0;
        }

        if (0 != _fbo) {
            glDeleteFramebuffers(1, &_fbo);
            _fbo = 0;
        }

        _destroyContext();

        _initialized = false;
    }

    bool QuadRenderer::_initPrograms()
    {
        _defaultPrograms[SHADER_RGB2RGB] = ProgramData::create(s_vertexShader, s_defaultFragments[SHADER_RGB2RGB]);
        _defaultPrograms[SHADER_RGB2YUV] = ProgramData::create(s_vertexShader, s_defaultFragments[SHADER_RGB2YUV]);
        _defaultPrograms[SHADER_YUV2RGB] = ProgramData::create(s_vertexShader, s_defaultFragments[SHADER_YUV2RGB]);
        _defaultPrograms[SHADER_YUV2YUV] = ProgramData::create(s_vertexShader, s_defaultFragments[SHADER_YUV2YUV]);

        _toneMapPrograms[SHADER_RGB2RGB] = ProgramData::create(s_vertexShader, s_toneMapFragments[SHADER_RGB2RGB]);
        _toneMapPrograms[SHADER_RGB2YUV] = ProgramData::create(s_vertexShader, s_toneMapFragments[SHADER_RGB2YUV]);
        _toneMapPrograms[SHADER_YUV2RGB] = ProgramData::create(s_vertexShader, s_toneMapFragments[SHADER_YUV2RGB]);
        _toneMapPrograms[SHADER_YUV2YUV] = ProgramData::create(s_vertexShader, s_toneMapFragments[SHADER_YUV2YUV]);

        //TODO:有待验证创建Program的错误
        return true;
    }

    bool QuadRenderer::_initVertex()
    {
        _positionData[0] = -1.0f; _positionData[1] =  1.0f; //左上
        _positionData[2] = -1.0f; _positionData[3] = -1.0f; //左下
        _positionData[4] =  1.0f; _positionData[5] = -1.0f; //右下
        _positionData[6] =  1.0f; _positionData[7] =  1.0f; //右上

        glGenBuffers(1, &_posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), _positionData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        _uvData[0] = 0.0f; _uvData[1] = 1.0f;
        _uvData[2] = 0.0f; _uvData[3] = 0.0f;
        _uvData[4] = 1.0f; _uvData[5] = 0.0f;
        _uvData[6] = 1.0f; _uvData[7] = 1.0f;

        glGenBuffers(1, &_uvVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), _uvData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    uint64_t QuadRenderer::_makeTextureKey(GLint internalFormat, GLint width, GLint height)
    {
        uint64_t key = 0;
        key = (uint64_t)internalFormat;
        key <<= 32;
        key |= (width << 16) | (height & 0xFFFF);
        return key;
    }

    void QuadRenderer::_parseTextureKey(uint64_t key, GLint *internalFormat, GLint *width, GLint *height)
    {
        if (internalFormat) {
            *internalFormat = (GLint)(key >> 32);
        }
        if (width) {
            *width = (GLint)((key & 0xFFFFFFFF) >> 16);
        }
        if (height) {
            *height = (GLint)(key & 0xFFFF);
        }
    }

    GLuint QuadRenderer::_genTexture(GLint internalFormat, GLint width, GLint height)
    {
        GLuint tex;
        glGenTexture(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        return tex;
    }

    GLuint QuadRenderer::_popTexture(GLint internalFormat, GLint width, GLint height)
    {
        GLuint tex = 0;

        uint64_t key = _makeTextureKey(internalFormat, width, height);
        auto poolItt = _texturePool.find(key);
        if (_texturePool.end() != poolItt && !poolItt->second.empty()) {
            tex = poolItt->second.back();
        } else {
            tex = _genTexture(internalFormat, width, height));
        }
        
        if (width >= MIN_CACHE_TEXTURE_SIZE && height >= MIN_CACHE_TEXTURE_SIZE) {
            _useTexture(key, tex);
        } else {
            _tempTextures.push_back(tex);
        }

        return tex;
    }

    void QuadRenderer::_pushTexture(uint64_t key, GLuint tex)
    {
        if (0 == tex) {
            return;
        }
        if (_texturePool.end() == _texturePool.find(key)) {
            _texturePool[key] = std::vector<GLuint>();
        }
        _texturePool[key].push_back(tex);
    }

    void QuadRenderer::_useTexture(GLuint key, GLuint tex)
    {
        if (0 == tex) {
            return;
        }
        auto poolItt = _usingTextures.find(key);
        if (_usingTextures.end() != poolItt) {
            poolItt->second.push_back(tex);
        } else {
            _usingTextures[key] = std::vector<GLuint>();
            _usingTextures[key].push_back(tex);
        }
    }

    void QuadRenderer::resetUsingTextures()
    {
        for (auto pool : _usingTextures) {
            for (size_t i=0; i<pool->second.size(); ++i) {
                _pushTexture(pool->first, pool->second[i]);
            }
        }
        _usingTextures.clear();

        for (size_t i=0; i<_tempTextures.size(); ++i) {
            glDeleteTextures(1, &_tempTextures[i]);
        }
        _tempTextures.clear();
    }

    GLuint QuadRenderer::_uploadToTexture(unsigned char *srcBuffer, GLint internalFormat, GLint width, GLint height)
    {
        GLuint destTexture = _popTexture(internalFormat, width, height);
        glBindTexture(GL_TEXTURE_2D, destTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, srcBuffer);
        glBindTexture(GL_TEXTURE_2D, destTexture);
    }

    std::vector<GLuint> QuadRenderer::_getTextureFromData(std::shared_ptr<ImageData> imgData)
    {
        std::vector<GLuint> texVec;
        do {
            if (!imgData) {
                break;
            }
            
            GLint internalFormat = 0;
            switch(imgData->_pixelFormat) {
            case PIXEL_FORMAT_RGB:
                internalFormat = GL_RGB;
                break;
            case PIXEL_FORMAT_RGBA:
                internalFromat = GL_RGBA;
                break;
            case PIXEL_FORMAT_YV12:
            //case PIXEL_FORMAT_NV12:
                internalFormat = GL_LUMINANCE;
                break;
            default:
                break;
            }
            if (0 == internalFormat) {
                break;
            }

            GLint width = (GLint)imgData->_width;
            GLint height = (GLint)imgData->_height;
            unsigned char *dataBuffer = imgData->_pixelBuffer.get();
            texVec.push_back(_uploadToTexture(dataBuffer, internalFormat, width, height));
            if (GL_LUMINANCE != internalFormat) {
                break;
            }
            //UV
            dataBuffer += width * height;
            width >>= 1;
            height >>= 1;
            texVec.push_back(_uploadToTexture(dataBuffer, internalFormat, width, height));
            dataBuffer += width * height;
            texVec.push_back(_uploadToTexture(dataBuffer, internalFormat, width, height));
        } while(false);

        return texVec;
    }

    void QuadRenderer::_uploadPositionData(float *srcData, int srcStride)
    {
        _adjustPositionData(srcData, srcStride, _positionData, 2);
        glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), _positionData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void QuadRenderer::_adjustPositionData(float *srcData, int srcStride, float *destData, int destStride)
    {
        float midX = (_viewPort[0] + _viewPort[2]) / 2.0f;
        float midY = (_viewPort[1] + _viewPort[3]) / 2.0f;
        destData[0 * destStride + 0] = (srcData[0 * srcStride + 0] - midX) * 2.0f / _viewPort[2];
        destData[0 * destStride + 1] = (srcData[0 * srcStride + 1] - midY) * 2.0f / _viewPort[3];
        destData[1 * destStride + 0] = (srcData[1 * srcStride + 0] - midX) * 2.0f / _viewPort[2];
        destData[1 * destStride + 1] = (srcData[1 * srcStride + 1] - midY) * 2.0f / _viewPort[3];
        destData[2 * destStride + 0] = (srcData[2 * srcStride + 0] - midX) * 2.0f / _viewPort[2];
        destData[2 * destStride + 1] = (srcData[2 * srcStride + 1] - midY) * 2.0f / _viewPort[3];
        destData[3 * destStride + 0] = (srcData[3 * srcStride + 0] - midX) * 2.0f / _viewPort[2];
        destData[3 * destStride + 1] = (srcData[3 * srcStride + 1] - midY) * 2.0f / _viewPort[3];
    }

    void QuadRenderer::clearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void QuadRenderer::setRenderTarget(std::shared_ptr<ImageData> imgData)
    {
        if (!imgData.get()) {
            return;
        }

        GLint internalFormat = GL_RGB;
        GLint width = (GLint)imgData->_width;
        GLint height = (GLint)imgData->_height;
        switch(imgData->_pixelFormat) {
        case PIXEL_FORMAT_RGB:
        case PIXEL_FORMAT_YUV444_INTERLEAVED:
            internalFormat = GL_RGB;
            break;
        case PIXEL_FORMAT_RGBA:
            internalFormat = GL_RGBA;
            break;
        default:
            return;
        }
        GLuint tex = _popTexture(_makeTextureKey(internalFormat, width, height));

        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
        if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void QuadRenderer::readRenderTarget(std::shared_ptr<ImageData> imgData)
    {
        if (!imgData.get()) {
            return;
        }
        
        GLenum format = GL_RGB;
        switch(imgData->_pixelFormat) {
        case PIXEL_FORMAT_RGB:
        case PIXEL_FORMAT_YUV444_INTERLEAVED:
            format = GL_RGB;
            break;
        case PIXEL_FORMAT_RGBA:
            format = GL_RGBA;
            break;
        default:
            return;
        }

        glReadPixels(0, 0, imgData->_width, imgData->_height, format, GL_UNSIGNED_BYTE, imgData->_pixelbuffer.get());
    }

    void QuadRenderer::restoreRenderTarget()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, _prevFbo);
    }

    void QuadRenderer::drawLayer(std::shared_ptr<LayerData> curLayer, bool isYUVTarget = false)
    {
        if (!curLayer.get()) {
            return;
        }

        std::vector<GLuint> texs = _getTextureFromData(curLayer->_layerImage);
        if (texs.empty()) {
            return;
        }

        int programType = SHADER_RGB2RGB;
        bool isYUVSource = false;
        if (isYUVTarget) {
            switch(curLayer->_pixelFormat) {
            case PIXEL_FORMAT_RGB:
            case PIXEL_FORMAT_RGBA:
                programType = SHADER_RGB2YUV;
                break;
            case PIXEL_FORMAT_YV12:
                programType = SHADER_YUV2YUV;
                isYUVSource = true;
                break;
            default:
                break;
            }
        } else {
            switch(curLayer->_pixelFormat) {
            case PIXEL_FORMAT_RGB:
            case PIXEL_FORMAT_RGBA:
                programType = SHADER_RGB2RGB;
                break;
            case PIXEL_FORMAT_YV12:
                programType = SHADER_YUV2RGB;
                isYUVSource = true;
                break;
            default:
                break;
            }
        }

        std::vector<GLuint> mapTexs = _getTextureFromData(curLayer->_toneMapImage);
        std::shared_ptr<ProgramData> curProgram;
        if (mapTexs.empty()) {
            curProgram = _defaultPrograms[programType];
            curProgram->use();
        } else {
            curProgram = _toneMapPrograms[programType];
            curProgram->use();
            glUniform1i(curProgram->_unifromMap["u_toneMap"], mapTexs[0]);
        }

        if (isYUVSource) {
            glUniform1i(curProgram->_uniformMap["u_yTexture"], texs[0]]);
            glUniform1i(curProgram->_uniformMap["u_uTexture"], texs[1]]);
            glUniform1i(curProgram->_uniformMap["u_vTexture"], texs[2]]);
        } else {
            glUniform1i(curProgram->_uniformMap["u_vTexture"], texs[0]]);
        }
        
        _uploadPositionData(curLayer->_positionData, 2);

        GLuint curLocation = curProgram->_attrMap["a_position"];
        glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
        glEnableVertexAttribArray(curLocation);
        glVertexAttribPointer(curLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

        curLocation = curProgram->_attrMap["a_texCoord"];
        glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
        glEnableVertexAttribArray(curLocation);
        glVertexAttribPointer(curLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }

/////////////////////////////////////////////////////////////////
/// class FrameProcessor

static thread_local bool s_isInitialized = false;
static thread_local EGLDisplay s_eglDisplay = EGL_NO_DISPLAY;
static thread_local EGLContext s_eglContext = EGL_NO_CONTEXT;
static thread_local EGLSurface s_eglSurface = EGL_NO_SURFACE;

#if defined(__ANDROID__)
    static void _defaultLogInfo(const char* log)
    {
        android_printLog(ANDROID_LOG_INFO, "%s", log);
    }

    static void _defaultLogError(const char* log)
    {
        android_printLog(ANDROID_LOG_ERROR, "%s", log);
    }

    static bool _initContext(int width, int height)
    {
        s_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (!eglInitialize(s_eglDisplay, nullptr, nullptr)) {
            return false;
        }
        //Choose Config
        const EGLinit attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_NONE
        };
        EGLint numConfigs;
        EGLConfig config;
        if (!eglChooseConfig(s_eglDisplay, attribs, &config, 1, &numConfigs)) {
            return false;
        }
        //Create Context
        const EGLint contextAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };
        s_eglContext = eglCreateContext(s_eglDisplay, config, EGL_NO_CONTEXT, contextAttr);
        if (EGL_NO_CONTEXT == s_eglContext) {
            return false;
        }
        //Create Surface
        width = width > 0 ? width : 1280;
        height = height > 0 ? height : 720;
        const EGLint surfaceAttr[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
        };
        s_eglSurface = eglCreatePbufferSurface(s_eglDisplay, config, surfaceAttr);
        if (EGL_NO_SURFACE == s_eglSurface) {
            return false;
        }

        eglMakeCurrent(s_eglDisplay, s_eglSurface, s_eglSurface, s_eglContext);
        return true;
    }

    static void _destroyContext()
    {
        eglMake(s_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(s_eglDisplay, s_eglContext);
        eglDestroySurface(s_eglDisplay, s_eglSurface);
        eglTerminate(s_eglDisplay);

        s_eglDisplay = EGL_NO_DISPLAY;
        s_eglSurface = EGL_NO_SURFACE;
        s_eglContext = EGL_NO_CONTEXT;
    }

    bool FrameProcessor::initialize(int width, int height)
    {
        if (s_isInitialized) {
            return true;
        }

        if (!_initContext(width, height) || !QuadRenderer::initialize()) {
            _destroyContext();
            return false;
        }

        if (nullptr == FrameProcessor::logInfoCallback) {
            FrameProcessor::logInfoCallback = std::bind(_defaultLogInfo, std::placeholders::_1);
        }
        if (nullptr == FrameProcessor::logErrorCallback) {
            FrameProcessor::logErrorCallback = std::bind(_defaultLogError, std::placeholders::_1);
        }
        
        s_isInitialized = true;
        return true;
    }

    void FrameProcessor::uninitialize()
    {
        if (!s_isInitialized) {
            return;
        }

        QuadRenderer::destroy();
        _destroyContext();

        s_isInitialized = false;
    }

    bool FrameProcessor::isInitialized()
    {
        return s_isInitialized;
    }
#else //iOS
    static void defaultLogInfo(const char* log)
    {
        //TODO:Reserved for iOS
    }

    static void defaultLogError(const char* log)
    {
        //TODO:Reserved for iOS
    }

    bool FrameProcessor::initialize(int width, int height)
    {
        if (nullptr == FrameProcessor::logInfoCallback) {
            FrameProcessor::logInfoCallback = std::bind(_defaultLogInfo, std::placeholders::_1);
        }
        if (nullptr == FrameProcessor::logErrorCallback) {
            FrameProcessor::logErrorCallback = std::bind(_defaultLogError, std::placeholders::_1);
        }
        //TODO:Reserved for iOS
        return false;
    }

    void FrameProcessor::uninitialize()
    {
        //TODO:Reserved for iOS
    }

    bool FrameProcessor::isInitialized()
    {
        //TODO:Reserved for iOS
        return false;
    }
#endif

    static bool _checkOutputFormat(unsigned int pixelFormat)
    {
        switch(pixelForamt) {
        case PIXEL_FORMAT_RGB:
        case PIXEL_FORMAT_RGBA:
        case PIXEL_FORMAT_YUV444_INTERLEAVED:
            return true;
        default:
            return false;
        }
    }

    void FrameProcessor::processFrame(const std::vector<LayerData*> &srcLayers, ImageData* destImage)
    {
        if (!s_isInitialized) {
            FrameProcessor::logErrorCallback("FrameProcessor.processFrame:FrameProcessor is not initialized!");
            return;
        }
        if (srcLayers.empty() || !destImage.get() || !_checkOutputFormat(destImage->_pixelFormat)) {
            return false;
        }
        
        QuadRenderer::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
        QuadRenderer::setRenderTarget(destImage);

        bool isYUVTarget = false;
        if (PIXEL_FORMAT_YUV444_INTERLEAVED == destImage->_pixelFormat) {
            isYUVTarget = true;
        }
        for(size_t i=0; i<srcLayers.size(); ++i) {
            QuadRenderer::drawLayer(srcLayers[i], isYUVTarget);
        }

        QuadRenderer::readRenderTarget(destImage);
        QuadRenderer::restoreRenderTarget();
        QuadRenderer::resetUsingTexture();

        return true;
    }
        
    void FrameProcessor::drawFrame(const std::vector<LayerData*> &srcLayers)
    {
        if (!s_isInitialized) {
            FrameProcessor::logErrorCallback("FrameProcessor.drawFrame:FrameProcessor is not initialized!");
            return;
        }
        for(size_t i=0; i<srcLayers.size(); ++i) {
            QuadRenderer::drawLayer(srcLayers[i], false);
        }
    }
    
} //namespace paomiantv