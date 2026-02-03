# Contributing to PDF4QT

Thank you for your interest in contributing to PDF4QT! This document provides guidelines and instructions for contributing to the project.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Documentation](#documentation)

## 🤝 Code of Conduct

This project adheres to a Code of Conduct. By participating, you are expected to uphold this code. Please be respectful, inclusive, and professional in all interactions.

## 🎯 How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- A clear, descriptive title
- Detailed steps to reproduce the issue
- Expected vs. actual behavior
- Your environment (OS, PDF4QT version, etc.)
- Screenshots or error logs if applicable

Use the [Bug Report template](.github/ISSUE_TEMPLATE/bug_report.yml) when creating bug reports.

### Suggesting Features

Feature suggestions are welcome! Use the [Feature Request template](.github/ISSUE_TEMPLATE/feature_request.yml) and include:

- The problem you're trying to solve
- Your proposed solution
- Alternative approaches you've considered
- Use cases and examples

### Code Contributions

We welcome code contributions! Areas where you can help:

- Fixing bugs
- Implementing new features
- Improving performance
- Enhancing documentation
- Writing tests
- UI/UX improvements

## 🚀 Getting Started

### Prerequisites

- C++20 compatible compiler (GCC 11+, MSVC 2022, or Clang 13+)
- Qt 6.6 or higher
- CMake 3.16 or higher
- vcpkg (for dependency management)

### Building from Source

1. **Clone the repository**
   ```bash
   git clone https://github.com/boterocamilo/PDF4QT.git
   cd PDF4QT
   ```

2. **Install vcpkg**
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh  # or bootstrap-vcpkg.bat on Windows
   export VCPKG_ROOT=$(pwd)
   ```

3. **Configure and build**
   ```bash
   cmake -B build -S . \
     -DPDF4QT_INSTALL_QT_DEPENDENCIES=0 \
     -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
     -DCMAKE_BUILD_TYPE=Release
   
   cmake --build build --parallel
   ```

4. **Run the application**
   ```bash
   ./build/Pdf4QtViewer  # or Pdf4QtEditor
   ```

## 🔄 Development Workflow

1. **Fork the repository** on GitHub
2. **Create a feature branch** from `master`
   ```bash
   git checkout -b feature/amazing-feature
   ```
3. **Make your changes** following our coding standards
4. **Test your changes** thoroughly
5. **Commit your changes** with clear, descriptive messages
6. **Push to your fork**
   ```bash
   git push origin feature/amazing-feature
   ```
7. **Open a Pull Request** against the `master` branch

## 💻 Coding Standards

### C++ Style

- Follow modern C++20 practices
- Use meaningful variable and function names
- Keep functions focused and concise
- Add comments for complex logic
- Use const correctness
- Prefer RAII and smart pointers

### Qt Conventions

- Follow Qt naming conventions (camelCase for methods, PascalCase for classes)
- Use Qt containers (QVector, QList, etc.) where appropriate
- Leverage Qt's signal/slot mechanism
- Use Qt's memory management (parent-child relationships)

### Code Formatting

- Indent with 4 spaces (no tabs)
- Opening braces on same line for functions
- Line length limit: 120 characters
- Include guards: `#ifndef FILENAME_H` format

### Example

```cpp
void PDFGlassmorphismToolbar::setAutoHideEnabled(bool enabled)
{
    if (m_autoHideEnabled != enabled)
    {
        m_autoHideEnabled = enabled;
        
        if (enabled)
        {
            resetAutoHideTimer();
        }
        else
        {
            m_autoHideTimer->stop();
        }
    }
}
```

## 📝 Commit Guidelines

### Commit Message Format

```
<type>: <subject>

<body>

<footer>
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

### Example

```
feat: Add glassmorphism floating toolbar

Implement modern translucent toolbar with blur effects.
Features include drag-to-move, auto-hide, and theme adaptation.

Closes #123
```

## 🔍 Pull Request Process

1. **Update documentation** if you've changed APIs or features
2. **Add tests** for new functionality
3. **Ensure all tests pass** locally
4. **Update the README** if needed
5. **Follow the PR template** when creating your pull request
6. **Request review** from maintainers
7. **Address feedback** promptly and professionally

### PR Checklist

- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No new warnings introduced
- [ ] Tests added/updated
- [ ] All tests passing
- [ ] Commit messages are clear

## 🧪 Testing

### Running Tests

```bash
cd build
ctest --output-on-failure
```

### Writing Tests

- Add unit tests for new functionality
- Update existing tests when changing behavior
- Ensure tests are deterministic and isolated
- Use descriptive test names

## 📚 Documentation

### Code Documentation

- Document public APIs with comments
- Use Doxygen-style comments for functions
- Explain complex algorithms
- Document assumptions and constraints

### User Documentation

- Update README.md for user-facing changes
- Add usage examples for new features
- Update screenshots if UI changes
- Keep changelog current

## 🏗️ Project Structure

```
PDF4QT/
├── Pdf4QtLibCore/       # Core PDF functionality
├── Pdf4QtLibWidgets/    # UI widgets (including glassmorphism)
├── Pdf4QtLibGui/        # Main window implementations
├── Pdf4QtViewer/        # PDF viewer application
├── Pdf4QtEditor/        # PDF editor application
├── Pdf4QtDiff/          # PDF comparison tool
├── Pdf4QtPageMaster/    # Page manipulation tool
└── UnitTests/           # Test suite
```

## 💡 Tips for Contributors

- **Start small**: Begin with documentation or simple bug fixes
- **Ask questions**: Don't hesitate to ask for clarification
- **Be patient**: Reviews may take time
- **Stay updated**: Regularly sync your fork with upstream
- **Test thoroughly**: Test on multiple platforms if possible
- **Have fun**: Enjoy the process of contributing!

## 🌟 Recognition

Contributors are recognized in:
- GitHub contributors list
- Release notes for significant contributions
- Project documentation

## 📞 Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For questions and general discussion
- **Email**: Mgr.Jakub.Melka@gmail.com for private inquiries

## 📜 License

By contributing to PDF4QT, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to PDF4QT! Your efforts help make this project better for everyone. 🎉
