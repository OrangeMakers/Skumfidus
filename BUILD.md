# Automated Build with GitHub Actions

This document describes how to set up automated builds using GitHub Actions for this project, including automatic version calculation based on commit history.

## Workflow Overview

The automated build process is triggered when:
1. A push is made to the repository
2. The CHANGELOG.md file has been updated in that push

The build artifacts will include information from the CHANGELOG.md in their description, along with an automatically calculated version number.

## Automatic Version Calculation

We use Semantic Versioning (SemVer) for version numbers, calculated automatically based on commit history:

1. Major version: Incremented for breaking changes
2. Minor version: Incremented for new features
3. Patch version: Incremented for bug fixes

The version is calculated using commit messages since the last Git tag. We use conventional commit messages to determine version changes:

- "BREAKING CHANGE:" or "!:" for major version increment
- "feat:" for minor version increment
- "fix:" for patch version increment

## Setting up the Workflow

1. Install the `git-semver` tool in your GitHub Actions environment:

```bash
npm install -g git-semver
```

2. Create a new file `.github/workflows/build.yml` in your repository with the following content:

```yaml
name: Automated Build

on:
  push:
    paths:
      - 'CHANGELOG.md'

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
      with:
        fetch-depth: 0  # Fetch all history for all tags and branches
    
    - name: Set up PlatformIO
      uses: actions/setup-python@v2
      with:
        python-version: '3.x'
    - run: pip install platformio
    
    - name: Install git-semver
      run: npm install -g git-semver
    
    - name: Calculate version
      id: semver
      run: echo "::set-output name=version::$(git-semver)"
    
    - name: Build project
      run: platformio run
    
    - name: Update CHANGELOG.md
      run: |
        sed -i "s/## \[Unreleased\]/## [${{ steps.semver.outputs.version }}] - $(date +'%Y-%m-%d')/" CHANGELOG.md
    
    - name: Get latest changelog entry
      id: changelog
      run: |
        LATEST_ENTRY=$(sed -n '/^## /,/^## /p' CHANGELOG.md | sed '$d' | tail -n +2)
        echo "::set-output name=latest_entry::$LATEST_ENTRY"
    
    - name: Create Release
      id: create_release
      uses: actions/create-release@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        tag_name: v${{ steps.semver.outputs.version }}
        release_name: Release v${{ steps.semver.outputs.version }}
        body: ${{ steps.changelog.outputs.latest_entry }}
        draft: false
        prerelease: false
    
    - name: Upload Release Asset
      uses: actions/upload-release-asset@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        upload_url: ${{ steps.create_release.outputs.upload_url }}
        asset_path: ./.pio/build/esp32doit-devkit-v1/firmware.bin
        asset_name: firmware-v${{ steps.semver.outputs.version }}.bin
        asset_content_type: application/octet-stream

    - name: Commit CHANGELOG.md changes
      run: |
        git config --local user.email "action@github.com"
        git config --local user.name "GitHub Action"
        git commit -am "Update CHANGELOG.md for version ${{ steps.semver.outputs.version }}"
        git push

3. Commit and push this file to your repository.

## How it Works

1. The workflow is triggered when changes are pushed to the CHANGELOG.md file.
2. It calculates the new version number based on commit history.
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
