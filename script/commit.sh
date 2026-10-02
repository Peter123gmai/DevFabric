#!/bin/bash
commit_content=""
read -p -r "type commit content: " commit_content

git status
git add .
git commit -m "$commit_content"
git push
