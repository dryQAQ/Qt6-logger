# Contributing to Qt6-Logger

We welcome contributions to the Qt6-Logger project! Here are some guidelines to help you get started.

## How to Contribute

### 1. Fork the Repository

Fork the repository to your GitHub account and clone it locally.

### 2. Create a Branch

Create a feature branch for your changes:
```bash
git checkout -b feature/your-feature-name
```

### 3. Make Changes

- Follow the existing code style
- Add tests for new functionality
- Update documentation if needed

### 4. Run Tests

Make sure all tests pass before submitting:
```bash
mkdir -p build && cd build
cmake -DBUILD_TESTS=ON ..
make
./tests/logger_tests
```

### 5. Submit a Pull Request

Push your changes to your fork and submit a pull request to the main branch.

## Code Style

- Use 4 spaces for indentation
- Follow Qt coding conventions
- Use `camelCase` for functions and variables
- Use `PascalCase` for classes and enums
- Add Doxygen-style comments for new functions

## Reporting Issues

If you find a bug or have a feature request, please create an issue on GitHub with:
- A clear description of the problem
- Steps to reproduce (for bugs)
- Expected behavior

## License

By contributing to this project, you agree that your contributions will be licensed under the MIT License.
