#!/usr/bin/env bash

set -e

to_snapshot=(include tools src tests CMakeLists.txt conanfile.txt CMake .clang-format .gitignore examples)
project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../" >/dev/null 2>&1 && pwd)"
snapshot_filename="kerfuffle-snapshot-$(date +%s).tar"
snapshot_path="snapshots/${snapshot_filename}"

echo "$project_root"/"$snapshot_path"

cd "$project_root"

ls "${to_snapshot[@]}" >.snapshot_list

mkdir -p snapshots

tar --exclude '__MACOSX' --exclude ".DS_Store" -cf "${snapshot_path}" "${to_snapshot[@]}" .snapshot_list

md5 "${snapshot_path}"

rm .snapshot_list

echo "${snapshot_filename}" >snapshots/.last_snapshot

#tar --list --file="$snapshot_path" | python3 tools/treeify.py
