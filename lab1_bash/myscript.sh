#!/bin/bash
#Problem·Statement:·Often·I·have·to·know the basic information about the sys
#em when I start my ubuntu

#in order to this, I use the 'uname -v' command to help orient myself.
#Name of command: ls
#·Example·of·its·usage:
#·-bash-4.2$uname -v
#
#·Here·is·the·myscript.sh·script·that·can·know the linux kernel versiont\
#·usage:·sh·myScript.sh
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
#Name·of·command:·ls

#Example·of·its·usage:
#-bash-4.2$uname·-v
#Here·is·the·myscript.sh·script·that·can·know·the·linux·kernel·version
#usage:·sh·myScript.sh$
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
    du -sh -- *
    return
}
print_date () {
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

