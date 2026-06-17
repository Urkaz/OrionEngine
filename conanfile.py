from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("spdlog/1.16.0")
        self.requires("catch2/3.11.0")
        self.requires("glfw/3.4")
        self.requires("glm/1.0.2")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.user_presets_path = False
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()
