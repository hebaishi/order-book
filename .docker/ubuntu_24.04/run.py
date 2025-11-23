#!/usr/bin/python3
import subprocess
import os


def run(cmd, interactive=False):
    args = cmd.split(' ')
    if not interactive:
        return subprocess.run(args, capture_output=True, text=True).stdout.strip()
    else:
        proc = subprocess.Popen(args)
        proc.wait()
        if proc.returncode != 0:
            print(f'Process failed with exit code: {proc.returncode}')
            exit(1)


git_root = run('git rev-parse --show-toplevel')
current_branch = run('git rev-parse --abbrev-ref HEAD')
git_root_basename = os.path.basename(git_root)
project_root = git_root
if git_root_basename == current_branch:
    project_root = os.path.normpath(os.path.join(git_root, '..'))

project_name = os.path.basename(project_root)
mount_dir = project_root
destination_dir = '/work'

image_tag = f"{project_name}_ubuntu_24.04_gcc14:latest"
print(f"Building Docker image with tag: ${image_tag}")
run(f'docker build -t {image_tag} -f Dockerfile {git_root}', interactive=True)

print("Running Docker container...")
print(f"  Image: ${image_tag}")
print(f"  Mounting: ${mount_dir} -> {destination_dir}")

run(f'docker run -it --rm -v {mount_dir}:{destination_dir} -w {destination_dir} {image_tag} /bin/bash', interactive=True)
