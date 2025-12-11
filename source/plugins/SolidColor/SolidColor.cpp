#include "SolidColor.h"
using namespace ffglex;

// Plugin info - update these for your plugin
static PluginInstance p = {
    PluginFactory< SolidColor >,    // Factory method
    "SLDC",                          // Plugin unique ID (4 characters)
    "Solid Color",                   // Plugin name
    2,                               // API major version number
    1,                               // API minor version number
    1,                               // Plugin major version number
    0,                               // Plugin minor version number
    FF_SOURCE,                       // Plugin type (FF_SOURCE or FF_EFFECT)
    "Generates a solid color",       // Plugin description
    "by Your Name"                   // About
};

// Simple vertex shader - just passes through positions and UVs
static const char vertexShaderCode[] = R"(#version 410 core
layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;
out vec2 uv;
void main()
{
    gl_Position = vPosition;
    uv = vUV;
}
)";

// Fragment shader - outputs a solid color
static const char fragmentShaderCode[] = R"(#version 410 core
uniform vec4 Color;
out vec4 fragColor;
in vec2 uv;
void main()
{
    fragColor = Color;
}
)";

SolidColor::SolidColor() :
    colorLocation( -1 ),
    red( 1.0f ),
    green( 0.0f ),
    blue( 0.0f ),
    alpha( 1.0f )
{
    // Set minimum and maximum inputs (0 for a source plugin)
    SetMinInputs( 0 );
    SetMaxInputs( 0 );

    // Add parameters
    // Parameter type: FF_TYPE_STANDARD = 0.0-1.0 range slider
    AddParam( ffglqs::ParamRange::Create( "Red", 1.0f, { 0.0f, 1.0f } ) );
    AddParam( ffglqs::ParamRange::Create( "Green", 0.0f, { 0.0f, 1.0f } ) );
    AddParam( ffglqs::ParamRange::Create( "Blue", 0.0f, { 0.0f, 1.0f } ) );
    AddParam( ffglqs::ParamRange::Create( "Alpha", 1.0f, { 0.0f, 1.0f } ) );
}

FFResult SolidColor::InitGL( const FFGLViewportStruct* vp )
{
    // Compile and link shaders
    if( !shader.Compile( vertexShaderCode, fragmentShaderCode ) )
    {
        DeInitGL();
        return FF_FAIL;
    }

    // Get uniform locations
    colorLocation = shader.FindUniform( "Color" );
    if( colorLocation < 0 )
    {
        DeInitGL();
        return FF_FAIL;
    }

    // Call base class initialization
    return ffglqs::Source::InitGL( vp );
}

FFResult SolidColor::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
    // Get current parameter values
    red = GetParamValue( 0 );
    green = GetParamValue( 1 );
    blue = GetParamValue( 2 );
    alpha = GetParamValue( 3 );

    // Activate our shader
    ScopedShaderBinding shaderBinding( shader.GetGLID() );

    // Set the color uniform
    glUniform4f( colorLocation, red, green, blue, alpha );

    // Draw a full-screen quad with the solid color
    quad.Draw();

    return FF_SUCCESS;
}

FFResult SolidColor::DeInitGL()
{
    shader.FreeGLResources();
    return ffglqs::Source::DeInitGL();
}
