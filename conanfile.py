from importlib.metadata import requires
from logging import exception
from conans import ConanFile, CMake, tools
from conan.tools.cmake import CMakeToolchain


class BoostEchoServer(ConanFile):
    name = "boost_echo_server"
    version = "0.1.0"

    requires = 'boost/1.78.0'

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        pass

    def package_info(self):
        pass
