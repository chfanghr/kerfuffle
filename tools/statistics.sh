#!/usr/bin/env bash

set -e

command -v jq >/dev/null 2>&1 || (echo "jq is required but not found" && exit)
command -v cloc >/dev/null 2>&1 || (echo "cloc is required but not found" && exit)

project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/../" >/dev/null 2>&1 && pwd)"
tools_dir="$project_root/tools"

cd "$project_root"

if [[ -f snapshots/.last_snapshot ]]; then
  last_snapshot="snapshots/$(cat snapshots/.last_snapshot)"
fi

#echo $last_snapshot

snapshot_output=()
while IFS= read -r line; do
  snapshot_output+=("$line")
done < <("$tools_dir"/snapshot.sh)

echo "${snapshot_output[0]}"
echo "${snapshot_output[1]}"

POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"
  case $key in
  -v | --verbose)
    VERBOSE=true
    shift
    ;;
  --show-diff)
    SHOW_DIFF=true
    shift
    ;;
  *) # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift              # past argument
    ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [[ $VERBOSE ]]; then
  echo "statistics:"
  cloc "${snapshot_output[0]}"
  if [ $SHOW_DIFF ] && [ "${last_snapshot}" ]; then
    echo
    echo "diff:"
    cloc --diff "${snapshot_output[0]}" "${last_snapshot}"
  fi
else
  cloc_output="$(cloc --json "${snapshot_output[0]}")"
  code="$(echo "$cloc_output" | jq ".SUM[\"code\"]")"
  comment="$(echo "$cloc_output" | jq ".SUM[\"comment\"]")"
  blank="$(echo "$cloc_output" | jq ".SUM[\"blank\"]")"
  echo $((code + comment + blank))
  echo $(echo "$cloc_output" | jq ".SUM[\"nFiles\"]")
fi
