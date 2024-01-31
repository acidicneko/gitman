#!/bin/bash

PKG_LOC="$GITMAN_ROOT"/packages/"$1"

HASH=$(curl -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/acidicneko/$1/commits/main \
        | grep sha \
        | awk 'NR==1{print $2}' \
        | sed 's/[",]//g')

if [ $? != 0 ] ; then
        exit 1
fi

echo "LAST_HASH=$HASH" > "$PKG_LOC"/last_commit

if [ $? != 0 ] ; then
        exit 1
fi