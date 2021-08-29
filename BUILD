cc_binary(
    name = "benchmark",
    srcs = [
        "benchmark.cpp",
    ],
    malloc = "@com_google_tcmalloc//tcmalloc",
    deps = [
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/hash",
        "@com_google_absl//absl/strings",
        "@cyan4973_xxhash//:xxhash",
        "@martinus_robin_hood_hashing//:robin_hood_hashing",
    ],
)
