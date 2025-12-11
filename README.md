# SolidColor FFGL Plugin

A simple FFGL (FreeFrame GL) plugin for Resolume Arena that generates a solid color output. This serves as a minimal template for creating cross-platform FFGL plugins with GitHub Actions CI/CD.

[![Build FFGL Plugin](https://github.com/YOUR_USERNAME/ffgl-solid-plugin/actions/workflows/build.yml/badge.svg)](https://github.com/YOUR_USERNAME/ffgl-solid-plugin/actions/workflows/build.yml)

## Features

- ✅ **Cross-platform**: Builds on Windows and macOS
- ✅ **Universal Binary**: macOS builds support both Intel (x86_64) and Apple Silicon (arm64)
- ✅ **Automated CI/CD**: GitHub Actions builds and releases automatically
- ✅ **Simple RGBA controls**: Four sliders for Red, Green, Blue, and Alpha

## Downloads

Download the latest release from the [Releases page](https://github.com/YOUR_USERNAME/ffgl-solid-plugin/releases).

- **Windows**: `SolidColor-Windows.zip` - Contains `SolidColor.dll`
- **macOS**: `SolidColor-macOS-Universal.zip` - Contains `SolidColor.bundle` (works on Intel and Apple Silicon)

## Installation

### Windows
1. Download `SolidColor-Windows.zip`
2. Extract the contents
3. Copy `SolidColor.dll` to one of these locations:
   - `Documents\Resolume Arena\Extra Effects\`
   - Or add a custom folder in Resolume: `Preferences → Video → FFGL Plugin Folders`
4. Restart Resolume Arena

### macOS
1. Download `SolidColor-macOS-Universal.zip`
2. Extract the contents
3. Copy `SolidColor.bundle` to:
   - `~/Documents/Resolume Arena/Extra Effects/`
4. Restart Resolume Arena

## Building from Source

### Prerequisites

- **CMake** 3.16 or higher
- **Git**
- **Windows**: Visual Studio 2019/2022 with C++ workload
- **macOS**: Xcode Command Line Tools (`xcode-select --install`)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/ffgl-solid-plugin.git
cd ffgl-solid-plugin

# Create build directory
mkdir build && cd build

# Configure (Windows with Visual Studio)
cmake .. -G "Visual Studio 17 2022" -A x64

# Configure (macOS - Universal Binary)
cmake .. -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"

# Build
cmake --build . --config Release
```

### Output Locations

- **Windows**: `build/bin/Release/SolidColor.dll`
- **macOS**: `build/bin/SolidColor.bundle`

## Project Structure

```
ffgl-solid-plugin/
├── .github/
│   └── workflows/
│       └── build.yml          # GitHub Actions CI/CD
├── source/
│   └── plugins/
│       └── SolidColor/
│           ├── SolidColor.cpp # Plugin implementation
│           └── SolidColor.h   # Plugin header
├── CMakeLists.txt             # Build configuration
└── README.md
```

## Creating Your Own Plugin

This template uses the official [Resolume FFGL SDK](https://github.com/resolume/ffgl) which is automatically downloaded during the CMake configuration step.

### To create a new plugin based on this template:

1. **Fork/Clone this repository**

2. **Rename the plugin files**:
   - Rename `SolidColor.cpp` and `SolidColor.h` to your plugin name
   - Update `CMakeLists.txt` with your new file names

3. **Update plugin info** in your `.cpp` file:
   ```cpp
   static PluginInstance p = {
       PluginFactory< YourPlugin >,
       "XXXX",                    // Unique 4-character ID
       "Your Plugin Name",        // Display name
       2, 1,                      // API version (2.1)
       1, 0,                      // Your plugin version
       FF_SOURCE,                 // or FF_EFFECT for effects
       "Description",
       "About text"
   };
   ```

4. **Choose plugin type**:
   - `FF_SOURCE` - Generates visuals (like this Solid Color plugin)
   - `FF_EFFECT` - Processes input textures

5. **Set inputs** in constructor:
   ```cpp
   SetMinInputs(0); SetMaxInputs(0);  // Source plugin
   SetMinInputs(1); SetMaxInputs(1);  // Single-input effect
   SetMinInputs(2); SetMaxInputs(2);  // Two-input mixer
   ```

### Parameter Types

```cpp
// Slider (0.0 - 1.0)
AddParam(ffglqs::ParamRange::Create("Name", defaultValue, {min, max}));

// Toggle/Boolean
AddParam(ffglqs::ParamBool::Create("Name", false));

// Option dropdown
auto options = ffglqs::ParamOption::Create("Name");
options->elements.push_back({"Option 1", 0.0f});
options->elements.push_back({"Option 2", 1.0f});
AddParam(options);

// Text input
AddParam(ffglqs::ParamText::Create("Name", "default"));

// Trigger button
AddParam(ffglqs::ParamTrigger::Create("Name"));
```

### Basic Shader Template

```cpp
static const char fragmentShaderCode[] = R"(#version 410 core
uniform sampler2D InputTexture;  // For effects
uniform vec4 MyColor;
uniform float MyFloat;

in vec2 uv;
out vec4 fragColor;

void main()
{
    // For effects: sample input
    vec4 inputColor = texture(InputTexture, uv);
    
    // Your shader logic here
    fragColor = inputColor * MyColor;
}
)";
```

## Compatibility

- **Resolume Arena**: 7.0.3 and later
- **Resolume Avenue**: 7.0.3 and later
- **FFGL SDK**: 2.2

### macOS Requirements
- macOS 10.15 (Catalina) or later
- Resolume 7.11.0+ for native Apple Silicon support

## Troubleshooting

### Plugin doesn't appear in Resolume
1. Check the file is in a folder that Resolume scans
2. Verify the file extension (.dll for Windows, .bundle for macOS)
3. Check Resolume's log for loading errors
4. On macOS, ensure the bundle is not quarantined: `xattr -dr com.apple.quarantine SolidColor.bundle`

### Build fails
1. Ensure all prerequisites are installed
2. Try deleting the `build` folder and reconfiguring
3. Check that Git can access https://github.com/resolume/ffgl

### macOS code signing issues
For distribution, you may need to sign and notarize the bundle:
```bash
codesign --force --sign "Developer ID Application: Your Name" SolidColor.bundle
```

## Resources

- [FFGL SDK Documentation](https://github.com/resolume/ffgl/wiki)
- [Resolume Forum - FFGL Development](https://resolume.com/forum/viewforum.php?f=22)
- [OpenGL 4.1 Reference](https://www.khronos.org/registry/OpenGL-Refpages/gl4/)

## License

This template is released under the BSD-3-Clause license (same as the FFGL SDK).

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
