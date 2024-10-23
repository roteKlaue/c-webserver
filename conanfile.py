from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class CrestConan(ConanFile):
    name = "crest"
    version = "0.0.1"
    license = "MIT"
    author = "roteKlaue janoolhd@gmail.com"
    url = "https://github.com/roteklaue/c-webserver"
    description = "A lightweight webserver framework in C."
    topics = ("webserver", "framework", "C", "networking")
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "src/*", "CMakeLists.txt"

    def config_options(self):
        pass

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["crest"]
        self.cpp_info.includedirs = ["include"]
