#!/bin/bash

DEFAULT_ROOT_LOCATION="$HOME"/.local/share/gitman
PKG_LOC="$DEFAULT_ROOT_LOCATION"/packages/"$1"

HASH=$(curl -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/acidicneko/$1/commits/main \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')

echo "LAST_HASH=$HASH" > "$PKG_LOC"/last_commit