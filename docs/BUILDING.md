# Building Bizzy-Fusion-OS

This guide covers building Bizzy-Fusion-OS on different platforms and configurations.

## Prerequisites

### Required
- **Git** - For cloning the repository
- **CMake** - Version 3.10 or higher
- **C/C++ Compiler** - GCC 7.0+ or Clang 5.0+
- **Make** - GNU Make or compatible
- **Python** - Version 3.8+ (for scripts and testing)

### Optional (for specific components)
- **NASM** - For assembly code (if building x86 bootloader)
- **QEMU** - For emulating the OS
- **GDB** - For debugging
- **Doxygen** - For documentation generation

## Linux Installation

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    git \
    nasm \
    qemu \
    gdb
```

### Fedora/RHEL

```bash
sudo dnf install -y \
    gcc \
    g++ \
    cmake \
    python3 \
    python3-pip \
    git \
    nasm \
    qemu \
    gdb
```

### Arch Linux

```bash
sudo pacman -S \
    base-devel \
    cmake \
    python \
    git \
    nasm \
    qemu \
    gdb
```

## macOS Installation

Using Homebrew:

```bash
brew install cmake python3 git nasm qemu gdb
```

Or using MacPorts:

```bash
sudo port install cmake python39 git nasm qemu gdb
```

## Windows Installation

### Using MinGW (Recommended)

1. Install [MinGW-w64](https://www.mingw-w64.org/)
2. Install [CMake](https://cmake.org/download/)
3. Install [Python 3](https://www.python.org/)
4. Install [Git](https://git-scm.com/)

Or use Windows Subsystem for Linux (WSL):

```bash
# In WSL Ubuntu terminal
sudo apt-get install build-essential cmake python3 python3-pip git nasm qemu gdb
```

## Building the Project

### 1. Clone the Repository

```bash
git clone https://github.com/liljaybae5-tech/bizzy-fusion-os.git
cd bizzy-fusion-os
```

### 2. Install Dependencies

```bash
./scripts/install-deps.sh
```

This script installs any Python dependencies and verifies all prerequisites.

### 3. Create Build Directory

```bash
mkdir build
cd build
```

### 4. Configure the Build

#### Basic Configuration (Release)

```bash
cmake ..
```

#### Debug Configuration (with debugging symbols)

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

#### Custom Configuration

```bash
cmake -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_TESTING=ON \
      -DENABLE_DOCUMENTATION=ON \
      -DTARGET_ARCH=x86_64 \
      ..
```

**Available CMake Options:**

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | Release | Build type: Debug, Release, RelWithDebInfo |
| `ENABLE_TESTING` | ON | Build test suite |
| `ENABLE_DOCUMENTATION` | OFF | Generate Doxygen docs |
| `ENABLE_STATIC_ANALYSIS` | OFF | Run code analysis tools |
| `TARGET_ARCH` | x86_64 | Target architecture: x86, x86_64, arm64 |
| `ENABLE_OPTIMIZATION` | ON | Compiler optimizations |

### 5. Build the Project

```bash
make
```

Or to build with verbose output:

```bash
make VERBOSE=1
```

Or to use multiple cores:

```bash
make -j4  # Use 4 cores
```

**Build targets:**

```bash
make                   # Build everything
make kernel            # Build kernel only
make drivers           # Build drivers only
make utilities         # Build utilities
make test              # Build and run tests
make docs              # Generate documentation
make clean             # Remove build artifacts
make distclean          # Remove all build files and reset
```

### 6. Run Tests

```bash
make test
```

Or run specific test suites:

```bash
ctest -V                    # Verbose output
ctest -R "test_memory"      # Run matching tests
ctest -N                    # Show available tests without running
```

## Running the OS

### Using QEMU

```bash
# If a disk image was built:
qemu-system-x86_64 -drive format=raw,file=bizzy-fusion-os.img

# With debugging enabled:
qemu-system-x86_64 -drive format=raw,file=bizzy-fusion-os.img -s -S

# With more verbosity:
qemu-system-x86_64 -drive format=raw,file=bizzy-fusion-os.img -d cpu,int
```

### Using GDB for Debugging

**Terminal 1 - QEMU:**
```bash
qemu-system-x86_64 -drive format=raw,file=bizzy-fusion-os.img -s -S
```

**Terminal 2 - GDB:**
```bash
gdb -x gdb-commands.txt
```

Where `gdb-commands.txt` might contain:
```
target remote localhost:1234
break main
continue
```

## Build Configurations

### Release Build (Optimized)

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_OPTIMIZATION=ON ..
make -j4
```

Produces: Optimized binary, minimal debugging info, smaller size

### Debug Build (Development)

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

Produces: Debugging symbols, no optimization, larger binary

### Development Build (Recommended)

```bash
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DENABLE_TESTING=ON \
      -DENABLE_STATIC_ANALYSIS=ON \
      ..
make -j4
```

Produces: Optimized with debug symbols, testing enabled, static analysis

## Incremental Building

After the initial build, subsequent builds are fast:

```bash
cd build
make -j4  # Only rebuilds changed files
```

## Clean Build

To start fresh:

```bash
cd build
make clean
cmake ..
make -j4
```

Or from repository root:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j4
```

## Troubleshooting Build Issues

### CMake Configuration Fails

**Problem:** `CMake Error: Could not find CMakeLists.txt`

**Solution:**
```bash
cd build
cmake ..  # Make sure you're in build directory
```

### Compiler Errors

**Problem:** `error: gcc not found` or compiler version mismatch

**Solution:**
```bash
# Check installed compiler
gcc --version

# Specify compiler explicitly
cmake -DCMAKE_C_COMPILER=gcc-10 -DCMAKE_CXX_COMPILER=g++-10 ..
```

### Python Dependency Errors

**Problem:** `ModuleNotFoundError: No module named 'xyz'`

**Solution:**
```bash
pip install -r requirements.txt
# or
python3 -m pip install -r requirements.txt
```

### Missing Build Tools

**Problem:** `make: command not found`

**Solution:**
```bash
# Install build tools for your OS
# See Prerequisites section above
```

### Out of Memory During Build

**Problem:** Build fails with memory errors

**Solution:**
```bash
# Use fewer parallel jobs
make -j1  # Single core only
```

### Cache Issues

**Problem:** Changes not taking effect

**Solution:**
```bash
# Clear CMake cache
rm -rf build
mkdir build
cd build
cmake ..
make
```

## Advanced Building

### Generating Documentation

```bash
cmake -DENABLE_DOCUMENTATION=ON ..
make docs
# HTML docs in: build/docs/html/index.html
```

### Running Static Analysis

```bash
cmake -DENABLE_STATIC_ANALYSIS=ON ..
make analyze
```

### Cross-Compilation

For building for different architecture:

```bash
cmake -DTARGET_ARCH=arm64 \
      -DCMAKE_TOOLCHAIN_FILE=toolchain-arm64.cmake \
      ..
make
```

### Installing After Build

```bash
make install
# Installs to CMAKE_INSTALL_PREFIX (default: /usr/local)
```

## Build Output

After successful build:

```
build/
├── bin/                 # Executable binaries
├��─ lib/                 # Compiled libraries
├── tests/               # Test executables
├── docs/                # Generated documentation
├── CMakeFiles/          # CMake internals
└── CMakeCache.txt       # CMake configuration cache
```

## CI/CD Integration

For automated builds:

```bash
#!/bin/bash
cd bizzy-fusion-os
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_TESTING=ON \
      -DENABLE_STATIC_ANALYSIS=ON \
      ..
make -j4
make test
```

## Performance Tips

1. **Use multiple cores:** `make -j$(nproc)`
2. **Use ccache:** `cmake -DCMAKE_C_COMPILER_LAUNCHER=ccache ..`
3. **Incremental builds:** Only rebuild what's necessary
4. **Separate debug/release:** Use different build directories

## Getting Help

- Check [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for common issues
- Review [SETUP.md](SETUP.md) for basic setup
- Open an issue on GitHub
- Check build logs in `build/CMakeFiles/CMakeOutput.log`

## Next Steps

After building successfully:

1. Run tests: `make test`
2. Check documentation: `make docs`
3. Review the [ARCHITECTURE.md](ARCHITECTURE.md) guide
4. See [CONTRIBUTING.md](../CONTRIBUTING.md) to contribute!
