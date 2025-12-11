#pragma once
#include <ffgl/FFGLPluginSDK.h>

class SolidColor : public ffglqs::Source
{
public:
    SolidColor();

    // FFGL Plugin Methods
    FFResult InitGL(const FFGLViewportStruct* vp) override;
    FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
    FFResult DeInitGL() override;

private:
    // Shader related
    ffglex::FFGLShader shader;
    GLint colorLocation;
    
    // Parameter values
    float red;
    float green;
    float blue;
    float alpha;
};
