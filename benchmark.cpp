#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#define XXH_INLINE_ALL
#include "xxhash.h"

#include "absl/container/flat_hash_set.h"
#include "absl/hash/hash.h"
#include "benchmark/benchmark.h"
#include "robin_hood.h"

namespace {
std::vector<std::string> words{};
struct xxhasher {
  std::size_t operator()(const std::string_view &s) const {
    return XXH3_64bits(s.data(), s.size());
  }
};
std::random_device r{};
std::default_random_engine rng{r()};
} // namespace

// Fill an instance of SetType with the contents of words.
template <typename SetType> void BM_SetCreate(benchmark::State &state) {
  SetType m{};
  m.reserve(16'000'000);
  for (auto _ : state) {
    m.clear();
    for (const auto &w : words) {
      m.emplace(w);
    }
  }
}

// Fill instance of SetType (not part of the timing), then access it randomly.
template <typename SetType> void BM_SetFind(benchmark::State &state) {
  SetType m{};
  m.reserve(16'000'000);
  for (const auto &w : words) {
    m.emplace(w);
  }
  std::shuffle(std::begin(words), std::end(words), rng);
  typename SetType::size_type sz{0};
  typename std::vector<std::string>::size_type w{0};
  for (auto _ : state) {
    sz += m.count(words[w]);
    ++w;
  }
  state.SetItemsProcessed(sz);
}

BENCHMARK_TEMPLATE(BM_SetCreate,
                   robin_hood::unordered_flat_set<std::string_view>);
BENCHMARK_TEMPLATE(
    BM_SetCreate, robin_hood::unordered_flat_set<std::string_view,
                                                 absl::Hash<std::string_view>>);
BENCHMARK_TEMPLATE(BM_SetCreate,
                   robin_hood::unordered_flat_set<std::string_view, xxhasher>);
BENCHMARK_TEMPLATE(BM_SetCreate, absl::flat_hash_set<std::string_view>);
BENCHMARK_TEMPLATE(BM_SetCreate,
                   absl::flat_hash_set<std::string_view, xxhasher>);
BENCHMARK_TEMPLATE(BM_SetCreate, std::unordered_set<std::string_view>);
BENCHMARK_TEMPLATE(
    BM_SetCreate,
    std::unordered_set<std::string_view, absl::Hash<std::string_view>>);
BENCHMARK_TEMPLATE(BM_SetCreate,
                   std::unordered_set<std::string_view, xxhasher>);

// Iteration counts are hard-wired to the same size as the words vector.
BENCHMARK_TEMPLATE(BM_SetFind, absl::flat_hash_set<std::string_view>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind, absl::flat_hash_set<std::string_view, xxhasher>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind, robin_hood::unordered_flat_set<std::string_view>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind,
                   robin_hood::unordered_flat_set<std::string_view,
                                                  absl::Hash<std::string_view>>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind,
                   robin_hood::unordered_flat_set<std::string_view, xxhasher>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind, std::unordered_set<std::string_view>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(
    BM_SetFind,
    std::unordered_set<std::string_view, absl::Hash<std::string_view>>)
    ->Iterations(10'000'000);
BENCHMARK_TEMPLATE(BM_SetFind, std::unordered_set<std::string_view, xxhasher>)
    ->Iterations(10'000'000);

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);

  words.reserve(10'000'000);
  std::ios::sync_with_stdio(false);
  for (std::string line; std::getline(std::cin, line);) {
    words.emplace_back(line);
  }
  if (std::cin.bad()) {
    std::cerr << "error reading stdin\n";
    return 1;
  }

  if (words.size() != words.capacity()) {
      std::cerr << "Expected exactly ten million lines on stdin" << std::endl;
      std::abort();
   }

  std::shuffle(std::begin(words), std::end(words), rng);

  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}
