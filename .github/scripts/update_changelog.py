import re
import re
import sys
import os
from datetime import date
import shutil

def validate_and_update_release(version):
    with open('RELEASE.md', 'r') as f:
        content = f.read()

    # Check if there's a description
    description_match = re.search(r'^\[.*?\]\s*\n\n(.*?)\n\n###', content, re.MULTILINE | re.DOTALL)
    if not description_match or description_match.group(1).strip() == "[Insert release description here]":
        print("Error: Release description is missing or has not been updated in RELEASE.md")
        sys.exit(1)

    sections = ["Added", "Changed", "Deprecated", "Removed", "Fixed", "Security"]
    valid_sections = []
    for section in sections:
        if f"### {section}" not in content:
            print(f"Error: ### {section} section is missing in RELEASE.md")
            sys.exit(1)

        section_pattern = rf'### {section}\s*(.*?)(?=\n###|\Z)'
        section_match = re.search(section_pattern, content, re.DOTALL)
        if section_match:
            section_content = section_match.group(1).strip()
            if section_content and section_content != "- No changes":
                valid_sections.append(section)
                if "- No changes" in section_content:
                    content = re.sub(section_pattern, f"### {section}\n{section_content.replace('- No changes\n', '')}", content, flags=re.DOTALL)
            else:
                content = re.sub(section_pattern, f"### {section}\n- No changes\n", content, flags=re.DOTALL)

    if not valid_sections:
        print("Error: At least one section must have changes in RELEASE.md")
        sys.exit(1)

    # Update the [Unreleased] header with the new version
    today = date.today().isoformat()
    new_version_header = f'## [{version}] - {today}'
    content = content.replace('## [Unreleased]', new_version_header, 1)

    # Update CHANGELOG.md
    with open('CHANGELOG.md', 'r') as f:
        changelog_content = f.read()

    # Find the position of the first release header
    release_pattern = r'## \[\d+\.\d+\.\d+(?:-\d+)?\] - \d{4}-\d{2}-\d{2}'
    match = re.search(release_pattern, changelog_content)
    if match:
        insert_position = match.start()
        updated_changelog = changelog_content[:insert_position] + content + "\n\n" + changelog_content[insert_position:]
    else:
        # If no existing release is found, append to the end
        updated_changelog = changelog_content + "\n\n" + content

    with open('CHANGELOG.md', 'w') as f:
        f.write(updated_changelog)

    # Delete RELEASE.md and create RELEASE_NEXT.md
    os.remove('RELEASE.md')
    shutil.copy('assets/RELEASE_TEMPLATE.md', 'RELEASE_NEXT.md')

    return content

if __name__ == "__main__":
    version = sys.argv[1]
    latest_entry = validate_and_update_release(version)
    print(f"latest_entry<<EOF\n{latest_entry}\nEOF")
