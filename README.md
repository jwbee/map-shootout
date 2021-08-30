# map-shootout

A quick benchmark I wrote to check which of these hash table implementations seems fastest. You'll need bazel, a C++ toolchain, and python3 with numpy.

## Prep

Build the benchmark program:

`bazel build -c opt --cxxopt=--std=c++17 :all`

Generate the inputs (which are large):

`python3 ./make_gibberish.py > gibberish.txt`

Run the benchmarks:

`./bazel-bin/benchmark < gibberish.txt`
