import os
from conan import ConanFile
from conan.tools.files import get, copy, download
from conan.errors import ConanInvalidConfiguration
from conan.tools.scm import Version


class ArmToolchainPackage(ConanFile):
    name = "arm-linux-toolchain"
    version = "14.2"

    license = "GPL-3.0-only"
    homepage = "https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads"
    description = "Conan package for the ARM toolchain, targeting different Linux ARM architectures."
    settings = "os", "arch"
    package_type = "application"

    def _archs32(self):
        return ["armv6", "armv7", "armv7hf"]
    
    def _archs64(self):
        return ["armv8", "armv8.3"]

    def _get_toolchain(self, target_arch):
        if target_arch in self._archs32():
            return "arm-none-linux-gnueabihf"
        else:
            return "aarch64-none-linux-gnu"
        
    def _get_download(self, toolchain):
        downloads = {
            ("Linux", "aarch64-none-linux-gnu"): ("https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-aarch64-none-linux-gnu.tar.xz", "47aeefc02b0ee39f6d4d1812110952975542d365872a7474b5306924bca4faa1", True),
            ("Windows", "aarch64-none-linux-gnu"): ("https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-mingw-w64-x86_64-aarch64-none-linux-gnu.zip", "9428b2039923916418ec7037025be8cec3202926d65db9452519977834ad2a89", False),

        }
        return downloads[(str(self.settings.os), toolchain)]

    def validate(self):
        if self.settings.arch != "x86_64":
            raise ConanInvalidConfiguration(f"This toolchain is not compatible with {self.settings.arch}. "
                                            "It can only run on x86_64.")

        valid_archs = self._archs32() + self._archs64()
        if self.settings_target.os != "Linux" or self.settings_target.arch not in valid_archs:
            raise ConanInvalidConfiguration(f"This toolchain only supports building for Linux-{', '.join(valid_archs)}. "
                                           f"{self.settings_target.os}-{self.settings_target.arch} is not supported.")

        if self.settings_target.compiler != "gcc":
            raise ConanInvalidConfiguration(f"The compiler is set to '{self.settings_target.compiler}', but this "
                                            "toolchain only supports building with gcc.")

        if Version(self.settings_target.compiler.version) >= Version("15") or Version(self.settings_target.compiler.version) < Version("14"):
            raise ConanInvalidConfiguration(f"Invalid gcc version '{self.settings_target.compiler.version}'. "
                                            "Only 14.X versions are supported for the compiler.")

    def source(self):
        download(self, "https://developer.arm.com/GetEula?Id=37988a7c-c40e-4b78-9fd1-62c20b507aa8", "LICENSE", verify=False)

    def build(self):
        toolchain = self._get_toolchain(self.settings_target.arch)
        download, sha, strip_root = self._get_download(toolchain)
        get(self, download,
            sha256=sha, strip_root=strip_root)

    def package(self):
        toolchain = self._get_toolchain(self.settings_target.arch)
        dirs_to_copy = [toolchain, "bin", "include", "lib", "libexec"]
        for dir_name in dirs_to_copy:
            copy(self, pattern=f"{dir_name}/*", src=self.build_folder, dst=self.package_folder, keep_path=True)
        copy(self, "LICENSE", src=self.build_folder, dst=os.path.join(self.package_folder, "licenses"), keep_path=False)

    def package_id(self):
        self.info.settings_target = self.settings_target
        # We only want the ``arch`` setting
        self.info.settings_target.rm_safe("os")
        self.info.settings_target.rm_safe("compiler")
        self.info.settings_target.rm_safe("build_type")

    def package_info(self):
        toolchain = self._get_toolchain(self.settings_target.arch)
        self.cpp_info.bindirs.append(os.path.join(self.package_folder, toolchain, "bin"))

        self.conf_info.define("tools.build:compiler_executables", {
            "c":   f"{toolchain}-gcc",
            "cpp": f"{toolchain}-g++",
            "asm": f"{toolchain}-as"
        })
