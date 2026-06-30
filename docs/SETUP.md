# Bizzy-Fusion-OS Setup Guide

## Prerequisites

- Git
- C/C++ compiler (GCC or Clang)
- CMake 3.10+
- Python 3.8+

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/liljaybae5-tech/bizzy-fusion-os.git
cd bizzy-fusion-os
```

### 2. Build the Kernel

```bash
mkdir build
cd build
cmake ..
make
```

### 3. Run Tests

```bash
make test
```

## Development Environment

### Setting Up Your Development Environment

1. Install required dependencies:
   ```bash
   ./scripts/install-deps.sh
   ```

2. Configure your build:
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. Build with debugging symbols:
   ```bash
   make debug
   ```

## Project Structure Overview

- **kernel/**: Core operating system kernel
- **drivers/**: Hardware abstraction layer and device drivers
- **utilities/**: System utilities and tools
- **ui/**: User interface framework
- **docs/**: Documentation and guides
- **tests/**: Unit and integration tests

## Troubleshooting

For common issues and solutions, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
