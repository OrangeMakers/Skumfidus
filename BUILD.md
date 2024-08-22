# Automated Build with GitHub Actions

This document describes how to set up automated builds using GitHub Actions for this project.

## Workflow Overview

The automated build process is triggered when:
1. A push is made to the repository
2. The CHANGELOG.md file has been updated in that push

The build artifacts will include information from the CHANGELOG.md in their description.

## Setting up the Workflow

1. Create a new file `.github/workflows/build.yml` in your repository with the following content:

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
    
    - name: Set up PlatformIO
      uses: actions/setup-python@v2
      with:
        python-version: '3.x'
    - run: pip install platformio
    
    - name: Build project
      run: platformio run
    
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
        tag_name: ${{ github.ref }}
        release_name: Release ${{ github.ref }}
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
        asset_name: firmware.bin
        asset_content_type: application/octet-stream
```

2. Commit and push this file to your repository.

## How it Works

1. The workflow is triggered when changes are pushed to the CHANGELOG.md file.
2. It sets up the PlatformIO environment and builds the project.
3. It extracts the latest entry from the CHANGELOG.md file.
4. A new release is created with the changelog entry as the description.
5. The built firmware is uploaded as an asset to the release.

## Notes

- Ensure that your CHANGELOG.md file is properly formatted with ## headers for each version.
- The workflow assumes that your PlatformIO environment is named 'esp32doit-devkit-v1'. Adjust this if necessary.
- You may need to adjust the paths and filenames according to your project structure.

Remember to keep your CHANGELOG.md updated with each significant change to trigger the automated build and release process.
