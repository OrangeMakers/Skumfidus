import re
import sys
from datetime import date

def check_and_update_changelog(version):
    with open('CHANGELOG.md', 'r') as f:
        content = f.read()

    # Check if [Unreleased] section exists and has a non-default description
    unreleased_match = re.search(r'^## \[Unreleased\]\s*\n(.*?)\n^###', content, re.MULTILINE | re.DOTALL)
    if not unreleased_match or unreleased_match.group(1).strip() == "[Insert release description here]":
        print("Error: [Unreleased] section is missing or has not been updated in CHANGELOG.md")
        sys.exit(1)

    sections = ["Added", "Changed", "Deprecated", "Removed", "Fixed", "Security"]
    for section in sections:
        if f"### {section}" not in content:
            print(f"Error: ### {section} section is missing in CHANGELOG.md")
            sys.exit(1)

        section_pattern = rf'### {section}\s*(.*?)(?=\n###|\Z)'
        section_match = re.search(section_pattern, content, re.DOTALL)
        if section_match:
            section_content = section_match.group(1).strip()
            if not section_content or section_content == f"- [Update {section}]":
                content = re.sub(section_pattern, f"### {section}\n- No changes\n", content, flags=re.DOTALL)
            else:
                content = re.sub(rf'- \[Update {section}\]\s*\n', '', content)

    # Extract the latest entry
    latest_entry = re.search(r'^## \[Unreleased\](.*?)(?=^##|\Z)', content, re.MULTILINE | re.DOTALL).group(1).strip()

    # Update the [Unreleased] header with the new version
    today = date.today().isoformat()
    content = re.sub(r'## \[Unreleased\]', f'## [{version}] - {today}', content, count=1)

    with open('CHANGELOG.md', 'w') as f:
        f.write(content)

    return latest_entry

if __name__ == "__main__":
    version = sys.argv[1]
    latest_entry = check_and_update_changelog(version)
    print(f"latest_entry<<EOF\n{latest_entry}\nEOF")
