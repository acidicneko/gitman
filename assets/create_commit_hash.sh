#!/bin/bash

PKG_LOC="$GITMAN_ROOT"/packages/"$1"

if [ $4 != "null" ] ; then
        echo "LAST_HASH=$4" > "$PKG_LOC"/last_commit
        exit
fi

HASH=$(curl -s -H "Accept: application/vnd.github+json" \
        https://api.github.com/repos/$2/commits/$3 \
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