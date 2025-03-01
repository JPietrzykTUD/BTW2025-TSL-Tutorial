# SIMD for Everyone - A tutorial to TSL

This repository contains supplementary material for the tutorial "SIMD for Everyone - A tutorial to TSL", presented at Workshop on Novel Data Management Ideas on Heterogeneous Hardware Architectures (NoDMC) @ 21st Conference on Database Systems for Business, Technology and Web (BTW 2025) in Bamberg, Germany. 

To bootstrap the tutorial, the repository contains a [devcontainer](./.devcontainer/devcontainer.json) and an associated [dockerfile](./.devcontainer/Dockerfile). 
The image contain:

- Development tools (for building the examples)
  - clang, gcc (generate x86 binaries)
  - aarch64-linux-gnu-gcc, aarch64-linux-gnu-binutils (generate aarch64 binaries)
  - cmake, make, ninja
  - python (execute the TSL generator)
- Execution/Emulation environment
  - qemu (run/emulate aarch64 code on x86)
  - intel-sde (emulate latest x86 hardware on older x86 platforms)

For the course of the tutorial, we will provide an introduction in how to use and extend the **T**emplate **S**IMD **L**ibrary ([available on *Github*](https://github.com/db-tu-dresden/TSL)) for exploiting hardware provided SIMD capabilities in a hardware-agnostic way. 

<!--  -->

An overview of the supported instructions can be found [**here**](https://db-tu-dresden.github.io/TSL/).*