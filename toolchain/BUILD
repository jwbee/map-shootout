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
    toolchain_identifier = "clang12-toolchain",
    toolchain_config = ":clang12_toolchain_config",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
)

load(":cc_toolchain_config.bzl", "cc_toolchain_config")
cc_toolchain_config(name = "clang12_toolchain_config")

