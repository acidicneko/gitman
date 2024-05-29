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
GitMan looks for software sources in the file `~/.config/gitman/packages.json`.

A sample `packages.json` looks like this:
```json
[config]
r{
  "packages": [
    {
      "name": "recscreen",
      "repo": "AvishekPD/recscreen",
      "branch": "master",
      "type": "commit" 
    },
    {
      "name":"zeta",
      "repo": "acidicneko/zeta",
      "branch": "main",
      "type": "commit"
    },
    {
      "name":"repo-name",
      "repo": "username/repo-name",
      "branch": "branch-name",
      "type": "commit"
    }
  ] 
}
```
User can add a custom repository which is GitMan compatible to `packages.json` and install(update) the respective software via GitMan.

To sync(install) all packages:
```
gitman -s

To update all packages:
```
gitman -u package-name
```
