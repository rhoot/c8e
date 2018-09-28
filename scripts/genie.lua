--
-- Copyright (c) 2018 Johan Sköld
-- License: https://opensource.org/licenses/ISC
--

local SFML_DIR = "../3rdparty/SFML-2.5.0"

solution "c8e"
    location  "../.build/prj"
    objdir    "../.build/obj"
    targetdir "../.build/out"

    language "C++"
    configurations { "Debug", "Release" }
    defines {"SFML_STATIC=1"}

    includedirs {path.join(SFML_DIR, "include")}
    libdirs {"../.build/lib"}
    windowstargetplatformversion "10.0.17134.0"

    flags {
        "Cpp11",
        "NoExceptions",
        "NoPCH",
        "NoRTTI",
        "StaticRuntime",
        "Symbols",
    }

    configuration {"Release"}
        flags {"OptimizeSpeed"}

    configuration {"vs*"}
        buildoptions {
            "/wd4530", -- warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
        }
        defines {
            "_SCL_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS",
        }

    configuration {"windows"}
        platforms {"x64"}

    configuration {"not windows"}
        platforms {"Native"}

    project "c8e"
        kind "ConsoleApp"
        files {"../src/**"}
        links {"sfml"}

        flags {
            "ExtraWarnings",
            "FatalWarnings",
        }

        configuration {"macosx"}
            linkoptions {
                "-framework Carbon",
                "-framework Cocoa",
                "-framework IOKit",
                "-framework OpenGL",
                "-ObjC",
            }

        configuration {"vs*"}
            buildoptions {
                "/wd4201", -- warning C4201: nonstandard extension used: nameless struct/union
            }

        configuration {"windows"}
            links {
                "opengl32",
                "winmm",
            }

        configuration {"linux"}
            links {
                "GL",
                "X11",
                "Xrandr",
                "pthread",
                "udev",
            }

    project "sfml"
        kind "StaticLib"
        targetdir "../.build/lib"
        includedirs {path.join(SFML_DIR, "src")}

        includedirs {
            path.join(SFML_DIR, "extlibs/headers/stb_image"),
        }

        files {
            path.join(SFML_DIR, "src/SFML/Graphics/BlendMode.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Color.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/GLCheck.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/GLExtensions.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/GLLoader.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Glsl.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Image.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/ImageLoader.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/RenderStates.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/RenderTarget.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/RenderTextureImpl.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/RenderTextureImplFBO.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/RenderWindow.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Shader.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Sprite.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Texture.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/TextureSaver.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Transform.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Transformable.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/Vertex.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/VertexBuffer.cpp"),
            path.join(SFML_DIR, "src/SFML/Graphics/View.cpp"),

            path.join(SFML_DIR, "src/SFML/System/Clock.cpp"),
            path.join(SFML_DIR, "src/SFML/System/Err.cpp"),
            path.join(SFML_DIR, "src/SFML/System/Lock.cpp"),
            path.join(SFML_DIR, "src/SFML/System/Mutex.cpp"),
            path.join(SFML_DIR, "src/SFML/System/Sleep.cpp"),
            path.join(SFML_DIR, "src/SFML/System/String.cpp"),
            path.join(SFML_DIR, "src/SFML/System/ThreadLocal.cpp"),
            path.join(SFML_DIR, "src/SFML/System/Time.cpp"),

            path.join(SFML_DIR, "src/SFML/Window/Context.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/Cursor.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/GlContext.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/GlResource.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/Joystick.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/JoystickManager.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/SensorManager.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/VideoMode.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/Window.cpp"),
            path.join(SFML_DIR, "src/SFML/Window/WindowImpl.cpp"),
        }

        configuration {"macosx"}
            buildoptions {
                "-Wno-deprecated-declarations",
            }

            files {
                path.join(SFML_DIR, "src/SFML/System/Unix/ClockImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/MutexImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/SleepImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/ThreadLocalImpl.cpp"),

                path.join(SFML_DIR, "src/SFML/Window/OSX/AutoreleasePoolWrapper.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/cg_sf_conversion.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/cpp_objc_conversion.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/CursorImpl.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/HIDInputManager.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/HIDJoystickManager.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/JoystickImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SensorImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFApplication.m"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFApplicationDelegate.m"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFContext.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFKeyboardModifiersHelper.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFOpenGLView.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFOpenGLView+keyboard.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFOpenGLView+mouse.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFSilentResponder.m"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFViewController.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFWindowController.mm"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/SFWindow.m"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/VideoModeImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/OSX/WindowImplCocoa.mm"),
            }

        configuration {"vs*"}
            buildoptions {
                "/wd4267", -- warning C4267: 'argument': conversion from 'size_t' to 'GLint', possible loss of data
                "/wd4996", -- warning C4996: 'GetVersion': was declared deprecated
            }

        configuration {"windows"}
            flags {
                "Unicode",
            }
            files {
                path.join(SFML_DIR, "src/SFML/System/Win32/ClockImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Win32/MutexImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Win32/SleepImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Win32/ThreadLocalImpl.cpp"),

                path.join(SFML_DIR, "src/SFML/Window/Win32/CursorImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/JoystickImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/SensorImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/VideoModeImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/WglContext.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/WglExtensions.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Win32/WindowImplWin32.cpp"),
            }

        configuration {"linux"}
            files {
                path.join(SFML_DIR, "src/SFML/System/Unix/ClockImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/MutexImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/SleepImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/System/Unix/ThreadLocalImpl.cpp"),

                path.join(SFML_DIR, "src/SFML/Window/Unix/CursorImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/Display.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/GlxContext.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/GlxExtensions.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/JoystickImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/SensorImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/VideoModeImpl.cpp"),
                path.join(SFML_DIR, "src/SFML/Window/Unix/WindowImplX11.cpp"),

            }