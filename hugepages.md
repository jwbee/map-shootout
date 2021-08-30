# 15 minutes with tcmalloc could save you 15% or more
## TL;DR

[TCMalloc](https://github.com/google/tcmalloc) used together with 
Linux [transparent hugepages](https://www.kernel.org/doc/html/latest/admin-guide/mm/transhuge.html) 
can speed up a C++ hash table lookup workload between 10% and 40%. The effect is more pronounced with very large working sets.

## Background

Intel-compatible x86 processors offer multiple memory page sizes. The traditional default page is 4KiB. They also support "huge pages" which are 2MiB or 4MiB,
and sometimes 1GiB, but that mode is rarely used. Linux supports the 2MiB page size, and that is the size most people are referring to when they mention huge pages.

The point of a huge page is simply to cover more memory with a single translation from logical to physical address. A program addressing a working set of 1GiB
needs 262144 4KiB pages, but only 512 2MiB pages. Page table entries take up space, and walking them takes time, and translation caches are a limited
microarchitectural resource. Recent CPUs (Haswell and later on the Intel side, Zen and later from AMD) have between 1024 and 2048 second-level TLB entries, 
which means with small pages the TLB can cover only 4-8 MiB, but with huge pages the TLB covers 2-4 GiB.

Linux supports huge pages, either explicitly or transparently. Explicit huge pages allow the operator to allocate a specific number of pages and assign them
to processes. This is a pain and nobody uses it, except for people operating high-performance databases who can afford the operational complexity. Transparent
huge pages (THP) allows the Linux kernel to opportunistically use a huge page if an allocation or collection of them happen to be aligned and sized conveniently.
Most systems disable THP, because of a combination of inertia, ignorance, and disinformation. They can be easily enabled and disabled while the system runs, unlike
explicit huge pages.

TCMalloc is a memory allocator from Google. Many people are familiar with the old TCMalloc from the [gperftools](https://github.com/gperftools/gperftools)
collection. That older library is available in Linux distribution packages like Debian's 
[libtcmalloc-minimal4](https://packages.debian.org/stretch/libtcmalloc-minimal4). However, that well-known library is obsolete. This article is about the 
[*new* TCMalloc](https://github.com/google/tcmalloc) which is a separate project. This TCMalloc is a 
[huge-page-aware allocator](https://github.com/google/tcmalloc/blob/master/docs/temeraire.md), or HPAA. It allocates memory in a way designed to make THP
function optimally. It allocates huge-aligned and huge-sized regions, and it avoids freeing small areas within huge regions, which would 
force Linux to decompose a huge page into numerous small pages.

## Workload

I initially wrote this benchmark because I wanted to find out why the [Robin Hood `unordered_flat_set`](https://github.com/martinus/robin-hood-hashing) is faster
than the [Abseil `flat_hash_set`](https://abseil.io/docs/cpp/guides/container#abslflat_hash_map-and-abslflat_hash_set). My particular workload involves a large 
(~1GiB) hash set full of short (<20B) strings, where the speed of point lookups is paramount, and the keys arrive randomly. The benchmark reads random strings on
stdin, stores them in a vector, loads them all into an unordered set, then shuffles the vector and access the keys in the set. 10 million lines of input results in
a working set of about 1GB. 30 million lines increases this to 3GB, and 60 million lines needs a working set of about 5GB. These are useful points of inquiry
since 1GB, 3GB, and 5GB are well-covered, barely-covered, and not-totally-covered, respectively, by the 1536-entry huge page TLB in my CPU.

## Results

Playing around with `perf stat` during the benchark runs I noticed that the rate of page faults was quite high. I confirmed further with the PMU counter
`mem_inst_retired.stlb_miss_loads` that the program was taking a TLB miss every 400 CPU cycles. Figuring that huge pages could be helpful, I enabled THP

```# echo always> /sys/kernel/mm/transparent_hugepage/enabled```

THP helped slightly, reducing the STLB miss rate to once per 500 cycles, and reduced the page fault rate by a factor of 10, but that was not the factor of 500x
I was expecting. I figured the huge-page-oblivious allocator that comes with the GNU toolchain might be antagonistic to THP, so I plugged in tcmalloc by adding
this to my bazel `cc_binary` target:

```malloc = "@com_google_tcmalloc//tcmalloc",```

That had the desired effect, cutting STLB misses to 1 per 7000 cycles, and reducing the page fault rate by a factor of *500000x*.

The results given here are all for the Robin Hood `flat_unordered_set` using the xxHash hash function. The allocator and THP are varied. The cycles, STLB misses,
and total runtime are for the whole benchmark run, which includes reading and shuffling the inputs. The latency given is the mean point lookup. The platform is an
Intel 8th-generation "Coffee Lake" Core i5-8259U with 32GB of memory.

### 10 million keys
Allocator|THP|Cycles|STLB misses|Latency
-|-|-|-|-
GNU|disabled|55.5B|244M|105ns
GNU|enabled|47.0B|133M|85ns
TCMalloc|enabled|39.8B|5.59K|81ns

The improvement over baseline with TCMalloc+THP is 28% in terms of CPU time and 23% in terms of point lookup latency.

### 30 million keys
Allocator|THP|Cycles|STLB misses|Latency
-|-|-|-|-
GNU|disabled|146B|471M|128ns
GNU|enabled|122B|284M|93ns
TCMalloc|enabled|98.6B|27.1K|85ns

The gap widens with a larger working set, to 32% CPU time and 33% latency

### 60 million key
Allocator|THP|Cycles|STLB misses|Latency
-|-|-|-|-
GNU|disabled|304B|849M|139ns
GNU|enabled|240B|521M|99ns
TCMalloc|enabled|187B|27.5M|88ns

TCMalloc's lead continues to increase, even when the working set is too large for the huge page STLB to cover. With this working set, TCMalloc is 38% faster than
baseline, with 36% lower latency. TCMalloc is still 22% faster with 11% lower latency than the stock GNU allocator with THP enabled.

## Conclusion

Huge-page-aware allocation is a pretty good idea, and it is easy to try, especially if your C++ program is already built with bazel. Don't take advice
on THP from old blog posts written by people who didn't really know what they were talking about. It is worth the trouble to gather your own data.
