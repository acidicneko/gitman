#!/bin/bash

PKG_LOC="$GITMAN_ROOT/packages/$1"

source "$PKG_LOC"/last_commit

if [ $4 != "null" ] ; then
    if [ $4 == "$LAST_HASH" ] ; then
        exit 0
    else
        echo "[$2] Basing package on commit [$4]"
        exit 1
    fi
fi

CURRENT_HASH=$(curl -s -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/$2/commits/$3 \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')


if [ "$CURRENT_HASH" = "$LAST_HASH" ] ; then
    echo "[$1] Already the latest version."
    exit 0
else
    #echo "LAST_HASH=$CURRENT_HASH" > "$PKG_LOC"/last_commit
    echo "[$2] Update available."
    exit 1
fi