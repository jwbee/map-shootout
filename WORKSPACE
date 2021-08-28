workspace(name = "countwords")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

local_repository(
    name = "com_google_absl",
    path = "/home/jwb/abseil-cpp",
)

http_archive(
    name = "rules_cc",
    sha256 = "9a446e9dd9c1bb180c86977a8dc1e9e659550ae732ae58bd2e8fd51e15b2c91d",
    strip_prefix = "rules_cc-262ebec3c2296296526740db4aefce68c80de7fa",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/262ebec3c2296296526740db4aefce68c80de7fa.zip"],
)

http_archive(
    name = "com_github_google_benchmark",
    strip_prefix = "benchmark-1.5.6",
    urls = ["https://github.com/google/benchmark/archive/refs/tags/v1.5.6.zip"],
)

local_repository(
  name = "com_google_tcmalloc",
  path = "/home/jwb/tcmalloc",
)
