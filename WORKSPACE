workspace(name = "mapshootout")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "com_google_absl",
    sha256 = "1a7edda1ff56967e33bc938a4f0a68bb9efc6ba73d62bb4a5f5662463698056c",
    strip_prefix = "abseil-cpp-20210324.2",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20210324.2.zip"],
)

http_archive(
    name = "rules_cc",
    sha256 = "1e19e9a3bc3d4ee91d7fcad00653485ee6c798efbbf9588d40b34cbfbded143d",
    strip_prefix = "rules_cc-68cb652a71e7e7e2858c50593e5a9e3b94e5b9a9",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/68cb652a71e7e7e2858c50593e5a9e3b94e5b9a9.zip"],
)

# This has to be loaded here or tcmalloc git_repository fails to initialize.
# I don't know why; if I use local_repository I don't need to do this.
http_archive(
    name = "rules_fuzzing",
    sha256 = "a5734cb42b1b69395c57e0bbd32ade394d5c3d6afbfe782b24816a96da24660d",
    strip_prefix = "rules_fuzzing-0.1.1",
    urls = ["https://github.com/bazelbuild/rules_fuzzing/archive/v0.1.1.zip"],
)

load("@rules_fuzzing//fuzzing:repositories.bzl", "rules_fuzzing_dependencies")

rules_fuzzing_dependencies()

load("@rules_fuzzing//fuzzing:init.bzl", "rules_fuzzing_init")

rules_fuzzing_init()

http_archive(
    name = "com_github_google_benchmark",
    strip_prefix = "benchmark-1.5.6",
    urls = ["https://github.com/google/benchmark/archive/refs/tags/v1.5.6.zip"],
)

git_repository(
    name = "com_google_tcmalloc",
    branch = "master",
    recursive_init_submodules = True,
    remote = "https://github.com/google/tcmalloc.git",
)

http_archive(
    name = "cyan4973_xxhash",
    build_file = "@//:BUILD.xxhash",
    sha256 = "064333c754f166837bbefefa497642a60b3f8035e54bae52eb304d3cb3ceb655",
    strip_prefix = "xxHash-0.8.0",
    urls = ["https://github.com/Cyan4973/xxHash/archive/refs/tags/v0.8.0.zip"],
)

http_archive(
    name = "martinus_robin_hood_hashing",
    build_file = "@//:BUILD.robinhood",
    sha256 = "68a40a5fb6a582a0900318068b1c9bb82eb11bbd2cd04ba8a9e7b56c24d146eb",
    strip_prefix = "robin-hood-hashing-3.11.3/src/include",
    urls = ["https://github.com/martinus/robin-hood-hashing/archive/refs/tags/3.11.3.zip"],
)
