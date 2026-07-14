# Contributing to Bizzy-Fusion-OS

Thank you for your interest in contributing to Bizzy-Fusion-OS! This document provides guidelines and instructions for contributing.
Thank you for your interest in contributing to Bizzy-Fusion-OS! We welcome contributions from developers of all skill levels. This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)
- [Getting Help](#getting-help)

## Code of Conduct

Please be respectful and constructive in all interactions.
We are committed to providing a welcoming and inclusive environment. Please be respectful and constructive in all interactions with other contributors. We do not tolerate harassment, discrimination, or unprofessional behavior of any kind.

## Getting Started

### Prerequisites

- Git
- C/C++ compiler (GCC 7.0+ or Clang 5.0+)
- CMake 3.10+
- Python 3.8+
- Basic familiarity with OS concepts (helpful but not required)

### First-Time Setup

1. **Fork the repository** on GitHub (click "Fork" button)

2. **Clone your fork:**
   ```bash
   git clone https://github.com/YOUR-USERNAME/bizzy-fusion-os.git
   cd bizzy-fusion-os
   ```

3. **Add upstream remote:**
   ```bash
   git remote add upstream https://github.com/liljaybae5-tech/bizzy-fusion-os.git
   ```

4. **Install dependencies:**
   ```bash
   ./scripts/install-deps.sh
   ```

5. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   make
   ```

6. **Run tests:**
   ```bash
   make test
   ```

Great! You're ready to start contributing.

## How to Contribute

### 1. Fork and Branch
### Finding Tasks

We have several ways to find work that matches your skills:

#### 1. Good First Issues
Look for issues labeled `good-first-issue`:
- These are beginner-friendly tasks
- Well-scoped and documented
- Perfect for learning the codebase

[View good-first-issue tasks](../../issues?q=label%3A%22good-first-issue%22)

#### 2. Help Wanted Issues
Issues labeled `help-wanted`:
- Intermediate complexity
- Need additional hands
- Usually have context and requirements

[View help-wanted tasks](../../issues?q=label%3A%22help-wanted%22)

#### 3. Roadmap Items
Check our [ROADMAP.md](ROADMAP.md):
- Overview of planned features
- Organized by phase and priority
- Contains implementation notes

#### 4. Your Own Ideas
Have a feature idea or improvement?
- Open an issue first to discuss (no duplicates!)
- Label it as `feature-request` or `enhancement`
- Wait for feedback before starting work

### Contribution Types

We welcome all types of contributions:

- **Code** - New features, bug fixes, refactoring
- **Tests** - Unit tests, integration tests, test improvements
- **Documentation** - README improvements, guides, API docs
- **Examples** - Sample code, tutorials, use cases
- **Feedback** - Issues, code reviews, design suggestions
- **Tools** - Build scripts, development tools, utilities

## Development Workflow

### 1. Create a Feature Branch

```bash
git checkout -b feature/your-feature-name
# or for bug fixes:
git checkout -b fix/issue-description
# or for documentation:
git checkout -b docs/what-you-documented
```

**Branch naming convention:**
- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation
- `refactor/` - Code refactoring
- `test/` - Test additions/improvements
- `perf/` - Performance improvements

### 2. Make Your Changes

Follow the [Coding Standards](docs/CODING_STANDARDS.md):

```c
/**
 * Document your functions with doc comments
 *
 * @param param Parameter description
 * @return Return value description
 */
void my_function(int param)
{
    // Use 4-space indentation
    // Follow Google C++ Style Guide
    // Keep functions focused and small
    
    if (param < 0) {
        return ERROR_INVALID_PARAM;
    }
    
    // Implementation
}
```

### 2. Make Changes
### 3. Write Tests

- Follow the coding standards in [CODING_STANDARDS.md](docs/CODING_STANDARDS.md)
- Write clear, descriptive commit messages
- Add tests for new features
Add tests for any new code or bug fixes:

### 3. Submit a Pull Request
```bash
# Tests go in tests/ directory
# Create test file: tests/unit/test_my_feature.c

- Provide a clear description of changes
- Reference any related issues
- Ensure all tests pass
#include "../test_framework.h"
#include "../../src/my_module.h"

## Coding Standards
TEST(test_my_function_basic)
{
    int result = my_function(5);
    ASSERT_EQUAL(result, EXPECTED_VALUE);
}

- Use 4-space indentation
- Follow Google C++ Style Guide
- Document public APIs
- Write unit tests for new code
TEST(test_my_function_error)
{
    int result = my_function(-1);
    ASSERT_EQUAL(result, ERROR_INVALID_PARAM);
}
```

Run tests:
```bash
make test
```

### 4. Update Documentation

Update relevant documentation:
- If adding a feature, update `ARCHITECTURE.md`
- If changing build process, update `BUILDING.md`
- Add/update code comments
- Update README if appropriate

### 5. Commit Your Changes

Write clear, descriptive commit messages:

```bash
git add file1.c file2.h
git commit -m "feat(memory): Implement buddy allocator for page allocation

Added buddy allocator algorithm for efficient physical memory management.
Reduces fragmentation and improves allocation speed.

- Implemented buddy allocation and deallocation
- Added unit tests with 90% coverage
- Updated ARCHITECTURE.md with new design

Closes #42"
```

**Commit message format:**
```
<type>(<scope>): <subject>

<body - detailed explanation>

<footer - issue references>
```

**Types:**
- `feat` - New feature
- `fix` - Bug fix
- `docs` - Documentation
- `style` - Code formatting/style
- `refactor` - Code restructuring
- `perf` - Performance improvement
- `test` - Test changes
- `chore` - Build/dependency changes

### 6. Keep Your Branch Updated

```bash
git fetch upstream
git rebase upstream/main
```

### 7. Push Your Changes

```bash
git push origin feature/your-feature-name
```

## Testing

### Running All Tests

```bash
cd build
make test
```

### Running Specific Tests

```bash
ctest -V                    # Verbose output
ctest -R "test_memory"      # Run matching tests
ctest --output-on-failure   # Show output only for failures
```

### Test Coverage

Aim for 80%+ code coverage on new code:

```bash
# If coverage tools are installed
make coverage
```

### Types of Tests Needed

- **Unit tests** - Test individual functions/modules
- **Integration tests** - Test module interactions
- **Edge cases** - Boundary conditions, NULL inputs, etc.
- **Error paths** - Test error handling, not just happy path

## Pull Request Process

1. Update documentation as needed
2. Add/update tests
3. Ensure CI passes
4. Request review from maintainers
5. Address feedback and iterate
### Before Creating a PR

- [ ] Branch is up-to-date: `git rebase upstream/main`
- [ ] Code follows standards: Check [CODING_STANDARDS.md](docs/CODING_STANDARDS.md)
- [ ] Tests pass: `make test`
- [ ] No compiler warnings: `make` (clean build)
- [ ] Documentation updated
- [ ] Commits are clear and focused

### Creating a Pull Request

1. **Go to GitHub** and click "New Pull Request"
2. **Select your branch** for comparison with `main`
3. **Fill in the PR template:**

```markdown
## Description
Brief explanation of changes.

## Type of Change
- [ ] New feature
- [ ] Bug fix
- [ ] Documentation update
- [ ] Other

## Related Issues
Closes #<issue_number>

## Changes Made
- First change
- Second change
- Third change

## Testing Done
- Tested with X
- Verified Y works
- All tests passing

## Checklist
- [ ] Code follows style guidelines
- [ ] Tests added/updated
- [ ] Documentation updated
- [ ] No new warnings
- [ ] Changes reviewed locally
```

4. **Wait for review**

### During Code Review

- Respond to feedback promptly
- Make requested changes in new commits (don't squash yet)
- Re-request review after making changes
- Feel free to ask clarifying questions

### After Approval

- Your PR will be merged by a maintainer
- Be proud - you've contributed! 🎉

## Reporting Issues

### Bug Reports

Include the following information:

```markdown
## Description
Clear description of the bug.

## Steps to Reproduce
1. Do this
2. Then this
3. Then that

## Expected Behavior
What should happen?

## Actual Behavior
What actually happened?

## Environment
- OS: [e.g., Ubuntu 20.04, macOS 11]
- Compiler: [e.g., GCC 10.2]
- Build: [Release/Debug]

## Logs/Screenshots
[Attach any relevant error messages or screenshots]

## Additional Context
[Any other helpful information]
```

### Feature Requests

```markdown
## Description
What feature would you like to see?

## Use Case
Why do you need this? What problem does it solve?

## Proposed Solution
How do you think it should work?

## Alternatives Considered
Other approaches?

## Related Issues
[Link to related issues if any]
```

## Getting Help

### Need Assistance?

1. **Check documentation:**
   - [SETUP.md](docs/SETUP.md) - Setup instructions
   - [BUILDING.md](docs/BUILDING.md) - Build guide
   - [ARCHITECTURE.md](docs/ARCHITECTURE.md) - System design
   - [CODING_STANDARDS.md](docs/CODING_STANDARDS.md) - Code style

2. **Ask in an issue:**
   - Comment on related issues
   - Create a new issue with "question" label
   - Be specific about what you're stuck on

3. **Review existing code:**
   - Look at similar implementations
   - Check git history: `git log -p --grep="keyword"`
   - Review closed PRs for patterns

4. **Reach out to maintainers:**
   - Mention @liljaybae5-tech in issues
   - Ask in PR comments
   - Be respectful of their time

### Common Questions

**Q: How do I build the project?**
A: See [BUILDING.md](docs/BUILDING.md)

**Q: What coding style should I follow?**
A: See [CODING_STANDARDS.md](docs/CODING_STANDARDS.md)

**Q: How do I run tests?**
A: `cd build && make test`

**Q: What's a good first issue?**
A: Look for `good-first-issue` label or check [ROADMAP.md](ROADMAP.md)

**Q: Can I work on X feature?**
A: Open an issue first to discuss and check for duplicates!

## Resources

- [Linux Kernel Coding Style](https://www.kernel.org/doc/html/latest/process/coding-style.html)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Git Basics](https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository)
- [GitHub Flow](https://guides.github.com/introduction/flow/)

## Recognition

Contributors are acknowledged in:
- Pull request comments (thank you!)
- Project README (for significant contributions)
- Release notes (for feature additions)

## Questions?

## Issues
Don't hesitate to ask! We're here to help new contributors succeed.

When reporting issues:
- Provide a clear title and description
- Include reproduction steps
- Specify your environment
- Attach relevant logs or screenshots
Happy contributing! 🚀
