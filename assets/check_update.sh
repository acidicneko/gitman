#!/bin/bash

PKG_LOC="$GITMAN_ROOT/packages/$1"

CURRENT_HASH=$(curl -s -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/acidicneko/$1/commits/main \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')

source "$PKG_LOC"/last_commit

if [ "$CURRENT_HASH" = "$LAST_HASH" ] ; then
    echo "Already the latest version."
    exit 0
else
    #echo "LAST_HASH=$CURRENT_HASH" > "$PKG_LOC"/last_commit
    echo "[$1] Update available."
    exit 1
fi