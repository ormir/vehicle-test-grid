#!/bin/bash
#
# gitwait - watch file and git commit all changes as they happen
#

git checkout auto-commit

while true; do
	git pull
<<<<<<< HEAD
	git commit -a -m 'autocommit on change'
	git push origin 'auto-commit'
	sleep 2
=======
	fswatch -o ./*
	git commit -a -m 'autocommit on change `date +%F-%T`'
	git push 'auto-commit'
	# sleep 2
>>>>>>> master
done