# AegisAlloc-M

> **Hardened Deterministic $O(1)$ Memory Allocator & Hardware Profiler for ARM Cortex-M**

![Language](https://img.shields.io/badge/Language-C99%20%7C%20ARM%20Assembly-blue.svg)
![Target](https://img.shields.io/badge/Target-ARM%20Cortex--M%20%7C%20x86-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## Overview

**AegisAlloc-M** is a high-performance, deterministic $O(1)$ fixed-block memory pool allocator engineered for mission-critical, real-time embedded systems (bare-metal & RTOS). Designed specifically to overcome the non-deterministic latency and fragmentation risks of standard dynamic memory allocation (`malloc`/`free`), it incorporates anti-forensic defense mechanisms and cycle-accurate hardware profiling.

---

## Technical Highlights

* **Deterministic $O(1)$ Latency:** Constant-time block allocation and deallocation using native bit-manipulation hardware instructions (`CLZ` / `__builtin_clz`) over a 32-bit status bitmap.
* **Anti-Forensic Memory Sanitization:** Forced memory zeroization during deallocation via `volatile` pointers to prevent compiler optimizations (*Dead Store Elimination* / DSE) from stripping security cleanup code.
* **Heap Exploit Mitigation:** Bitmap bit-rotation logic (inspired by ASLR) to obfuscate block allocation order and counter predictable heap overflow exploits.
* **Cycle-Accurate Profiling (HAL):** Direct register access to ARM Cortex-M `DWT_CYCCNT` with an x86 `__rdtsc` fallback. Features atomic IRQ masking (`cpsid i` / `msr`) for jitter-free Worst-Case Execution Time (WCET) measurement.
* **Zero External Fragmentation:** Static 2 KB RAM pool layout partitioned into 32 blocks of 64 bytes with strict 8-byte alignment to prevent hardware bus faults.

---

## Architectural Specifications

| Parameter | Specification | Engineering Purpose |
| :--- | :--- | :--- |
| **Pool Capacity** | 2048 Bytes (32 blocks × 64 Bytes) | Low SRAM footprint suited for resource-constrained MCUs |
| **Block Alignment** | 8-Byte Strict Alignment | ARM AAPCS compliance & optimal bus access |
| **Allocation Latency** | $O(1)$ (~12–15 CPU cycles on Cortex-M) | Deterministic real-time WCET guarantees |
| **Sanitisation Scheme** | `0x00` zeroization via `volatile` loop | Eliminates sensitive data exposure post-release |

---
## Directory Structure

```text
.
├── include/
│   ├── pool_allocator.h   # Memory pool data structures and public API
│   └── profiler.h         # Hardware Abstraction Layer (HAL) for profiling
├── src/
│   ├── pool_allocator.c   # Bitmap logic, CLZ search, and volatile sanitization
│   └── profiler.c         # ARM DWT register drivers & inline assembly
├── test/
│   └── test_allocator.c   # Unit test suite and edge-case verification
├── main.c                 # Benchmark harness and validation entry point
└── README.md              # Project documentation
Building and Execution
Host Simulation (x86 / GCC)

Compile and execute the test harness locally on Windows or Linux:
  # Compile sources with C99 standard and include directories
  gcc -O2 -Wall -Iinclude src/pool_allocator.c src/profiler.c main.c -o alloc_test.exe

  # Execute benchmark
  .\alloc_test.exe

This project is open-source and distributed under the MIT License.
