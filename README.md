# GitMan
GitMan is a GitHub based source package manager, which installs and manages packages directly from upstream GitHub Repositories.
GitMan can manage all your software installed independently from GitHub, including installing and updating.
It supports custom GitHub reposiories which are GitMan compatible.

## Dependencies
GitMan requires the following software to function properly.
- git
- curl
- bash

For package dependency check respective package pages.

## Installation
GitMan is still in its early stages of its development and there will be some quircks here and there.
One line installation

```sh
git clone https://github.com/acidicneko/gitman.git && cd gitman && ./install.sh
```

## How does it work?
GitMan looks for software sources in a file `~/.local/share/gitman/db.ini`.

A sample `db.ini` looks like this:
```ini
[config]
repos=https://github.com/acidicneko https://github.com/AvishekPD

# main repository
[https://github.com/acidicneko]
zeta=A C++ helper tool
rci=A noob rice installer
gitman=A git based package manager
nyaafetch=A simple fetch tool.

[https://github.com/AvishekPD]
recscreen = A command line screen recorder

[github-user-link]
name-of-repository = a-simple-description
```
User can add a custom repository which is GitMan compatible to `db.ini` and install(update) the respective software via GitMan.

To install a software:
```
gitman -i package-name
```
To search a software:
```
gitman -s package-name
```
To uninstall a software:
```
gitman -r package-name
```
To update a software:
```
gitman -u package-name
```
