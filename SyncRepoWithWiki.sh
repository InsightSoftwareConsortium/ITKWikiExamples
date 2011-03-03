#!/bin/bash
#
# SyncRepoWithWIki - synchronize the wikiexamples with the
#                    itk.org/Wiki/ITK?examples wiki
#
# Make sure the wiki is up
echo "Synchronizing the wikiexamples repository with the wiki."
HOST=www.itk.org
echo "0) Can we access the wiki?"
ping -c 1 $HOST &> /dev/null
if test "${?}" = 1
  then
  echo "$HOST is not accessible. Try again later"
  exit
fi

echo "1) Pull updates from master repository"
git pull origin master

echo "2) Remove all .cxx, .h, .py, .tcl, files from local repository"
find . -name mwclient -prune -type f -o "(" -name \*.cxx -o -name \*.h -o -name \*.tcl -o -name \*.java -o -name \*.py ")" -exec rm -f {} \;

echo "3) Scrape the wiki"
./Admin/ScrapeWiki

echo "4) Process modified files"
git commit -m "SYNC: Files modified on wiki." `git status | grep modified: | cut -d":" -f2,2`

echo "5) Process new files"
find . "(" -name \*.cxx -o -name \*.h -o -name \*.tcl -o -name \*.py -o -name \*.java ")" -exec git add {} \;
git commit -m "SYNC: Files added to wiki."

echo "6) Process deleted files"
git rm `git status | grep deleted: | cut -d":" -f2,2`
git commit -m "SYNC: Files deleted (or moved) from wiki."

echo "7) Push the changes"
git push git@gitorious.org:itkwikiexamples/itkwikiexamples.git
