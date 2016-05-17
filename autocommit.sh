#!/bin/bash
#
# gitwait - watch file and git commit all changes as they happen
#

git checkout auto-commit

while true; do
	git pull
	fswatch -o ./*
	git commit -a -m 'autocommit on change `date +%F-%T`'
	git push 'auto-commit'
	# sleep 2
done