from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd


class LibJSONConan(ConanFile):
    name = "libjson"
    version = "1.0.0"
    license = "Unlicense"
    url = "https://github.com/IgnacioPomar/libJSON"
    description = "Lightweight, multiplatform, dependency-free JSON library for C++17"
    topics = ("json", "parser")

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "test/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 17)

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = not self.conf.get("tools.build:skip_test", default=False)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.conf.get("tools.build:skip_test", default=False) is not True:
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["libJSON"]
        self.cpp_info.set_property("cmake_file_name", "libJSON")
        self.cpp_info.set_property("cmake_target_name", "libJSON::libJSON")
        if self.options.shared:
            self.cpp_info.defines = ["LIBJSON_DLL"]
