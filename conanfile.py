from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class CrestConan(ConanFile):
    name = "crest"
    version = "1.0"
    license = "MIT"
    author = "roteKlaue"
    url = "https://github.com/roteklaue/c-webserver"
    description = "A lightweight webserver framework in C."
    topics = ("webserver", "framework", "C", "networking")
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "src/*", "CMakeLists.txt"

    def layout(self):
        cmake_layout(self)

    def generate(self):

        pass

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "*.h", src="src", dst=self.package_folder + "/include")
        copy(self, "*.a", src=self.build_folder, dst=self.package_folder + "/lib", keep_path=False)
        copy(self, "*.so", src=self.build_folder, dst=self.package_folder + "/lib", keep_path=False)
        copy(self, "*.dll", src=self.build_folder, dst=self.package_folder + "/lib", keep_path=False)
        copy(self, "*.dylib", src=self.build_folder, dst=self.package_folder + "/lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["crest"]

