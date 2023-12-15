-- set xmake minimum version
set_xmakever("2.8.2")
add_rules("mode.debug", "mode.release")

cppLanguage = "cxx20"
projectName = "AdventOfCode"
resourceFolder = "resources"

-- project
set_project(projectName)
-- set project version
set_version("2023.1.1", {build = "%Y%m%d", soname = true})
-- set warning all as error
set_warnings("all", "error")

add_requires("argparse")
add_requires("spdlog")
add_requires("fmt")
add_requires("boost")
add_requires("eigen")


target("advent_day")
    set_encodings("utf-8")
    set_kind("binary")
    set_languages(cppLanguage)
    add_files("src/**.cpp")
    add_headerfiles("src/**.hpp")

    add_extrafiles("$(resourceFolder)/*.txt")

    -- set the auto-generated config.h
    set_configdir("$(buildir)/config")
    -- set project name in the config.h
    add_configfiles("configured_files/config.hpp.in", {variables = {PROJECT_NAME = projectName, PROJECT_RESOURCES = resourceFolder}})
    add_includedirs("$(buildir)")
    add_headerfiles("$(buildir)/config/**.hpp")
    add_extrafiles("configured_files/**")

    add_packages("argparse")
    add_packages("spdlog")
    add_packages("fmt")
    add_packages("boost")
    add_packages("eigen")

    after_build(function (target)
        -- Copy resources to the build directory
        print("== Copying resources to the build directory")
        os.cp("resources", target:targetdir())
    end)
--    on_install(function (target)
        -- Copy resources to the install directory
--        print("== Copying resources to the install directory")
--        os.cp("resources", target:installdir())
--    end)
