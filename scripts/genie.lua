solution "c8e"
    location "../.build/prj"
    targetdir "../.build/out"

    language "C++"

    platforms { "Native" }
    configurations { "Debug", "Release" }

    flags {
        "Cpp11",
        "ExtraWarnings",
        "FatalWarnings",
        "NoExceptions",
        "NoPCH",
        "NoRTTI",
        "StaticRuntime",
        "Symbols",
    }

    configuration {"Release"}
        flags {
            "OptimizeSpeed",
        }

    project "c8e"
        kind "ConsoleApp"

        files {
            "../src/**",
        }
