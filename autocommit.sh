#!/bin/bash
#
# gitwait - watch file and git commit all changes as they happen
#

git checkout auto-commit

while true; do
	git pull
	git commit -a -m 'autocommit on change'
	git push origin 'auto-commit'
	sleep 2
done