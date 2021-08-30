package(default_visibility = ["//visibility:public"])

cc_toolchain_suite(
    name = "clang12_suite",
    toolchains = {
        "local|clang": ":clang12_toolchain",
    },
)

filegroup(name = "empty")

cc_toolchain(
    name = "clang12_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang12_toolchain_config",
    toolchain_identifier = "clang12-toolchain",
)

load(":cc_toolchain_config.bzl", "cc_toolchain_config")

cc_toolchain_config(name = "clang12_toolchain_config")
