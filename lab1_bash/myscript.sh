#!/bin/bash


echo "
     \$0 = $0
     \$1 = $1
     "
#problem statement: Print out a 'tree' visualization of the directory you
#are searching.
#in order to do this, I use "sudo·apt·install·tree" to install a package
#and use "tree" to draw a tree"
#Name of command: sudo apt·install·tree / tree

#Example·of·its·usage:$tree
#.
#├── instructions.md
#├── myScript.sh
#├── myscript.sh
#├── myscript.sh~
#└── readme.md

#0 directories, 5 files
file_tree () {
    sudo apt install tree
    tree
    return
}

#Problem·Statement:·Often·I·have·to·know·the·basic·information·about·the·sys
#tem·when·I·start·my·ubuntu
#in·order·to·this,·I·use·the·'uname·-v'·command·to·help·orient·myself.
#Name·of·command:·uname -v

#Example·of·its·usage:
#-bash-4.2$uname·-v
##30~18.04.1-Ubuntu SMP Fri Jan 17 06:14:09 UTC 2020
u_version () {
    uname -v
    return
}
#Problem statement: estimate file space usage
#in order to do this, I use the 'du -sh -- *' to help orient myself
#Name of command: du -sh -- *

#Example of its usage:
#$du -sh -- *
#4.0K	instructions.md
#4.0K	myScript.sh
#4.0K	myscript.sh
#4.0K	myscript.sh~
#8.0K	readme.md
space_usage () {
    echo "this is the file space usage"
    du -sh -- *
    return
}
#problem statement: print the date
#in order to do this, I use 'date' to help orient myself
#Name of command: date

#Example of its usage:
#$date
#Tue Feb 25 17:27:46 PST 2020
print_date () {
    echo "this is the date"
    date
    return
}

case $1 in
    file_tree)
	file_tree
	;;
    u_version)
	u_version
	;;
    space_usage)
	space_usage
	;;
    print_date)
	print_date
	;;
esac

