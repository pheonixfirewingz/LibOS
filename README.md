# LibOS - V0.5.0

## setup before building

   0. git submodule init
    1. git submodule update
    2. cd vcpkg
    3. /bootstrap-vcpkg.(bat|sh)
    4. ./vcpkg integrate install
    5. ./vcpkg install

## dependencies
    1. c++ compiler
    2. cmake
    3. Ninja-Build
    4. vulkan sdk

## Contribution rules

- keep formatting the same as the rest of the project's
- focus on speed and optimization
- try to be memory safe (not required as a temp fix)

## Formatting

Nobody is perfect, and sometimes we mess things up. That said, Here are some good dos & don't to try and stick to:

**Do:**

- Write in idiomatic C++20
- Use `clang-format` (version 11 or later) to automatically format C++ files ot project style.
- Choose expressive variable, function and class names. Make it as obvious as possible what the code is doing.
- Split your changes into separate, atomic commits.
- Make sure your commits are rebased on the master branch.
- Wrap your commit messages at 72 characters.
- Write the commit message subject line in the imperative mood ("Foo: Change the way dates work", not "Foo: Changed the way dates work").
- Write your commit messages in proper English, with care and punctuation.
- Squash your commits when making revisions after a patch review.
- Add your personal copyright line to files when making substantive changes. (Optional but encouraged!)
- Check the spelling of your code, comments and commit messages.

**Don't:**

- Submit code that's incompatible with the project licence (GNU LESSER GENERAL PUBLIC LICENSE Version 3, February 1999)
- Touch anything outside the stated scope of the PR.
- Iterate excessively on your design across multiple commits.
- Use weasel-words like "refactor" or "fix" to avoid explaining what's being changed.
- End commit message subject lines with a period.
- Include commented-out code.
- Write in C. (Instead, take advantage of C++'s amenities, and don't limit yourself to the standard C library.)
- Attempt large architectural changes until you are familiar with the system and have worked on it for a while.

## legacy Platforms Supported

- Windows 10 (until 2025)

## Platforms being worked on

- Windows 11

- Manjaro latest

## Planned Platforms

- Windows 11
- Linux Arch base
- Linux Debian base
- Mac OS Big Sur & up

## LICENSE

the license is found in the LICENSE.md
