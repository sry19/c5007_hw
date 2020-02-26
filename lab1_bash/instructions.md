### TODO Please edit the following information in your assignment

- Name(s): Ruoyun Sun
- How many hours did it take you to complete this lab? 3 hours
- Did you collaborate with any other students/TAs/Professors? no
- Did you use any external resources? (Cite them below)
  - https://infojinx.com/2017/11/du-command-check-disk-space-utilization-linux/
  - http://www.panix.com/~elflord/unix/bash-tute.html
- (Optional) What was your favorite part of the assignment? practice writing scripts and learn useful commands
- (Optional) How would you improve the assignment?

(See the writeup on Canvas)

# Automate (some of) your life

Getting familiar with the terminal is a fundamental skill to have. Many of you have various levels of experience using the terminal.

For this assignment, you will be warming up by writing a 1-2 page tutorial on bash scripting. Your tutorial will be embededded in a shell script, explaining how to do something on the terminal that is non-trivial but that can be automated. You will run your script by typing `sh myScript.sh` on the command line as an executable shell script(sh is the GNU Bourne-Again SHell -- type 'man sh' for more information).  You can look at example 'sh' scripts for inspiration on what you can achieve on the web and cite them appropriately. 

The tutorial will be done purely in text and and within a file called 'myscript.sh'. You are required to type your tutorial using a terminal based text editor like 'vim', 'emacs', or 'nano' to write your tutorial (I personally use the vim text editor).

Examples of a non-trivial scripts

* Sort a particular column of data from a .csv file (e.g. stock prices). Then print out the top ten values, and the average of that column of data.
* Write a shell script that allows you to query information about your computer system from a menu of options. Users might like to know information like hard drive space, if a graphics card is available, how fast the cpu is, etc.
* Write a shell script that downloads a webpage (using wget, perhaps to grab some json data), then parses some of the data, and prints it to the terminal. 
* You can use your imagination to create other examples.

# Part 1 - myScript.sh

<img width="400px" src="https://cdn-images-1.medium.com/max/1600/1*MaSfT-bQOxfWnKjFPxMg6g.gif">

## Very Brief Example Tutorial (Template)
(Note this example is fairly trivial, probably worth a C-. You can use it as a template)

```
# Problem Statement: "Often I have to search for different files and directories within my computer system. 
#                     In order to do this I use the 'ls' command to help orient myself.
# Name of command: ls
# Example of its usage: 
# -bash-4.2$ ls
# Lab1  README.md

# Here is the myScript.sh script that can list the directories and how many total directories are in the current folder.
# usage: sh myScript.sh

ls
ls | wc -l
```

Some ideas for improving the script above script to earn closer to an A are the following:

- Print out a 'tree' visualization of the directory you are searching.
- Output the amount of file space each directory is taking up
- Output some graphical patterns after running the command.
- List the history of the last 100 commands that have been run.
  - Then sort how frequently they are used.

* Some inspiration for developing shell scripting skills can be found here: https://www.hackerrank.com/domains/shell


# Resources to help

- A pretty good introduction to shell scripting is here: https://www.howtogeek.com/67469/the-beginners-guide-to-shell-scripting-the-basics/ .


# Feedback Loop

(An optional task that will reinforce your learning throughout the semester)

Read this article and implement the little 'whatis' command in your .bashrc. https://lifehacker.com/how-can-i-quickly-learn-terminal-commands-1494082178

```
echo "Did you know that:"; whatis $(ls /bin | shuf -n 1)
```

This is a nice way to learn a new command everytime you log on!
