# Automated Build with GitHub Actions

This document describes how to set up automated builds using GitHub Actions for this project, including automatic version calculation based on commit history using GitVersion.

## Workflow Overview

The automated build process is triggered when:
1. A push is made to the repository
2. The CHANGELOG.md file has been updated in that push

The build artifacts will include information from the CHANGELOG.md in their description, along with an automatically calculated version number.

## Automatic Version Calculation

We use GitVersion for Semantic Versioning (SemVer), calculated automatically based on commit history and tags:

1. Major version: Incremented for breaking changes
2. Minor version: Incremented for new features
3. Patch version: Incremented for bug fixes

The version is calculated using commit messages since the last Git tag. We use conventional commit messages to determine version changes:

- "BREAKING CHANGE:" or "!:" for major version increment
- "feat:" for minor version increment
- "fix:", "build:", "chore:", "ci:", "docs:", "perf:", "refactor:", "revert:", "style:", "test:" for patch version increment

## Setting up the Workflow

1. Create a `GitVersion.yml` file in the root of your repository with the following content:

```yaml
mode: ContinuousDelivery
branches: {}
ignore:
  sha: []
merge-message-formats: {}
major-version-bump-message: "^(build|chore|ci|docs|feat|fix|perf|refactor|revert|style|test)(\\([\\w\\s-]*\\))?(!:|:.*\\n\\n((.+\\n)+\\n)?BREAKING CHANGE:\\s.+)"
minor-version-bump-message: "^(feat)(\\([\\w\\s-]*\\))?:"
patch-version-bump-message: "^(build|chore|ci|docs|fix|perf|refactor|revert|style|test)(\\([\\w\\s-]*\\))?:"
```

2. Create a new file `.github/workflows/build.yml` in your repository with the following content:

```yaml
name: Automated Build

on:
  push:
    paths:
      - 'CHANGELOG.md'
      - '.github\workflows\build.yml'

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
      with:
        fetch-depth: 0  # Fetch all history for all tags and branches
    
    - name: Install GitVersion
      uses: gittools/actions/gitversion/setup@v0.10.2
      with:
        versionSpec: '5.x'
    
    - name: Determine Version
      id: gitversion
      uses: gittools/actions/gitversion/execute@v0.10.2
      with:
        useConfigFile: true
    
    - name: Set up PlatformIO
      uses: actions/setup-python@v4
      with:
        python-version: '3.x'
    - run: pip install platformio
    
    - name: Build project
      run: platformio run
    
    - name: Update CHANGELOG.md
      run: |
        sed -i "s/## \[Unreleased\]/## [${{ steps.gitversion.outputs.semVer }}] - $(date +'%Y-%m-%d')/" CHANGELOG.md
    
    - name: Get latest changelog entry
      id: changelog
      run: |
        LATEST_ENTRY=$(sed -n '/^## /,/^## /p' CHANGELOG.md | sed '$d' | tail -n +2)
        echo "latest_entry=$LATEST_ENTRY" >> $GITHUB_OUTPUT
    
    - name: Create Release
      id: create_release
      uses: actions/create-release@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        tag_name: v${{ steps.gitversion.outputs.semVer }}
        release_name: Release v${{ steps.gitversion.outputs.semVer }}
        body: ${{ steps.changelog.outputs.latest_entry }}
        draft: false
        prerelease: false
    
    - name: Upload Release Asset
      uses: softprops/action-gh-release@v1
      with:
        files: ./.pio/build/esp32doit-devkit-v1/firmware.bin
        tag_name: v${{ steps.gitversion.outputs.semVer }}
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}

    - name: Commit CHANGELOG.md changes
      run: |
        git config --local user.email "action@github.com"
        git config --local user.name "GitHub Action"
        git commit -am "Update CHANGELOG.md for version ${{ steps.gitversion.outputs.semVer }}"
        git push
```

3. Commit and push these files to your repository.

## How it Works

1. The workflow is triggered when changes are pushed to the CHANGELOG.md file or the build.yml file.
2. GitVersion calculates the new version number based on commit history and tags.
3. It updates the CHANGELOG.md file with the new version number.
4. It sets up the PlatformIO environment and builds the project.
5. It extracts the latest entry from the updated CHANGELOG.md file.
6. A new release is created with the calculated version number and changelog entry as the description.
7. The built firmware is uploaded as an asset to the release, including the version number in the filename.
8. The updated CHANGELOG.md is committed back to the repository.

## Notes

- Ensure that your CHANGELOG.md file is properly formatted with ## headers for each version, including an "## [Unreleased]" section at the top.
- Use conventional commit messages to ensure accurate version calculation.
- The workflow assumes that your PlatformIO environment is named 'esp32doit-devkit-v1'. Adjust this if necessary.
- You may need to adjust the paths and filenames according to your project structure.

Remember to keep your CHANGELOG.md updated with each significant change in the "## [Unreleased]" section to trigger the automated build and release process.
