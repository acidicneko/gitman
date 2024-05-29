#!/bin/bash

PKG_LOC="$GITMAN_ROOT/packages/$1"

CURRENT_HASH=$(curl -s -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/$2/commits/$3 \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')

source "$PKG_LOC"/last_commit

if [ "$CURRENT_HASH" = "$LAST_HASH" ] ; then
    echo "[$1] Already the latest version."
    exit 0
else
    #echo "LAST_HASH=$CURRENT_HASH" > "$PKG_LOC"/last_commit
    echo "[$2] Update available."
    exit 1
fi