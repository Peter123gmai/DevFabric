#!/bin/bash

# tao ham dung cac lenh voi dau vao la $? - return code
function process_code( )
{ 
    local exit_code=$1
    local task_a=$2
    if [[ $exit_code -ne 0 ]]; then 
    clear;
    echo something error, please check log_commit.txt ;
    echo task: "$task_a";
    echo task "$2" have error with exit code: "$1" 
    echo status of "$2": "$1" >> log_commit.txt
    exit_file
    exit 0
    else 
    echo status of "$2": "$1" >> log_commit.txt
    fi
}

# function exit log file
function exit_file()
{
    echo log end at "$(date +'%Y-%m-%d %H:%M:%S')" >> log_commit.txt
    echo ----------------------------------------------------------------------------- >> log_commit.txt
}

# file bat dau ghi log
git status

if [[ -z $(git status --porcelain) ]]; then
    echo -e "\n\033[0;33m[Notice] clean work, nothing to commit!\033[0m"
    exit 0 # Dừng script tại đây luôn, không chạy các lệnh dưới nữa
fi

echo started commit at "$(date +'%Y-%m-%d %H:%M:%S')" >> log_commit.txt 
git status
# hoi nhap noi dung commit va cos muon dọn màn hình sau khi kết thúc code ko

read -rp "type commit content: " content
read -rp "do you want to clear screen after push? enter y/n: " clear_a

echo commit content: "$content" >> log_commit.txt
echo clear screen after push: "$clear_a" >> log_commit.txt

# khai bao bien moi chua $?
abc=20;

# git add/commit/push deu duoc ghi ma return code vao log_commit.txt
# goi ham process code sau moi lan chay
git add . 
abc=$?
#echo status of git add: "$abc" >> log_commit.txt  # ma loi cua git add duoc ghi vào file log_commit.txt
process_code "$abc" "git add"

git commit -m "at $(date +'%Y-%m-%d %H:%M:%S') with content $content"
abc=$?
#echo status of git commit: "$abc" >> log_commit.txt
process_code "$abc" "git commit"

git push
abc=$?
#echo status of git push: "$abc" >> log_commit.txt
process_code "$abc" "git push"

# ket thuc log
exit_file

# cap nhat file log
git add log_commit.txt
git commit -m "update file log, running near the end of the file"
git push

if [[ "$clear_a" == "y" || "$clear_a" == "Y" ]]; then clear; echo cleared screen, for more information, please visit log_commit.txt; fi


exit 0
