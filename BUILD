cc_binary(
    name = "benchmark",
    srcs = ["benchmark.cpp","robin_hood.h", "xxhash.h"],
    deps = [
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/hash",
	"@com_github_google_benchmark//:benchmark",
    ],
malloc = "@com_google_tcmalloc//tcmalloc",
)
