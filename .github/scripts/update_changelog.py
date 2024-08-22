import re
import re
import sys
import os
from datetime import date
import shutil

def validate_and_update_release(version):
    with open('RELEASE.md', 'r') as f:
        content = f.read()

    validation_errors = []

    # Check if the first section is "## Description of next release"
    if not content.startswith("## Description of next release"):
        validation_errors.append("The first section in RELEASE.md should be '## Description of next release'")

    # Check if there's a description and it's not the placeholder
    description_match = re.search(r'## Description of next release\s*\n\n(.*?)\n\n##', content, re.MULTILINE | re.DOTALL)
    if not description_match or description_match.group(1).strip() == "[Insert release description here]":
        validation_errors.append("Release description is missing or has not been updated in RELEASE.md")

    sections = ["Added", "Changed", "Deprecated", "Removed", "Fixed", "Security"]
    valid_sections = []
    section_positions = {}

    for section in sections:
        section_pattern = rf'### {section}\s*(.*?)(?=\n###|\Z)'
        section_match = re.search(section_pattern, content, re.DOTALL)
        
        if not section_match:
            validation_errors.append(f"### {section} section is missing in RELEASE.md")
            continue

        section_positions[section] = section_match.start()
        section_content = section_match.group(1).strip()
        
        if section_content:
            if section_content.strip() != "- No changes":
                valid_sections.append(section)
                content = re.sub(section_pattern, f"### {section}\n{section_content.strip()}\n", content, flags=re.DOTALL)
            else:
                content = re.sub(section_pattern, f"### {section}\n- No changes\n", content, flags=re.DOTALL)
        else:
            content = re.sub(section_pattern, f"### {section}\n- No changes\n", content, flags=re.DOTALL)

    # Check if sections are in the correct order
    sorted_positions = sorted(section_positions.items(), key=lambda x: x[1])
    correct_order = [section for section, _ in sorted_positions]
    
    if correct_order != sections:
        validation_errors.append(f"Sections are not in the correct order. Expected: {', '.join(sections)}. Found: {', '.join(correct_order)}")

    if not valid_sections:
        validation_errors.append("At least one section must have changes in RELEASE.md")

    if validation_errors:
        print("Validation errors:")
        for error in validation_errors:
            print(f"- {error}")
        sys.exit(1)

    # Update the header with the new version
    today = date.today().isoformat()
    new_version_header = f'## [{version}] - {today}'
    content = content.replace('## Description of next release', new_version_header, 1)

    # Strip blank lines at the beginning and end of the content
    content = content.strip()

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
        print("Error: Unable to find the position to insert the new release in CHANGELOG.md")
        sys.exit(1)

    with open('CHANGELOG.md', 'w') as f:
        f.write(updated_changelog)

    # Delete RELEASE.md and create RELEASE_NEXT.md
    os.remove('RELEASE.md')
    shutil.copy('assets/RELEASE_TEMPLATE.md', 'RELEASE_NEXT.md')

    # Remove the version header from the content for the latest_entry output
    latest_entry = re.sub(r'^## \[.*?\] - .*?\n', '', content, 1).strip()

    # Replace all "###" with "##" in the latest_entry
    latest_entry = re.sub(r'###', '##', latest_entry)

    return latest_entry

if __name__ == "__main__":
    version = sys.argv[1]
    latest_entry = validate_and_update_release(version)
    print(f"latest_entry<<EOF\n{latest_entry}\nEOF")
