#!/bin/sh

# prunes and deletes bit branches that no longer exist on remote
git fetch -p && git branch -vv | grep ': gone' | sed -e 's/^.*origin\///g' | sed -e 's/:.*//g' | xargs git branch -D