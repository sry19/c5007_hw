# Linker Lab

See Canvas for details.

# Submission

Modify this readme with the following information. Copy this info to the Canvas submission. Commit and push to Github.

* Commit your hack.c file.
* Put the following in your README.md file: 
   * Name(s): Ruoyun Sun
   * How many hours did it take you to complete this lab? 2 hours
   * Did you collaborate with any other students/TAs/Professors? No
   * Did you use any external resources? (Cite them below)
      * https://medium.com/@dkwok94/the-linking-process-exposed-static-vs-dynamic-libraries-977e92139b5f
   * How does breaking code into libraries like this save compile time?
   1. You do not need to recompile them if their source code does not change.
   2. Compiling a small file is faster than compiling a large file.
   * How does the linker find the code needed for our executable? Does all of the code live together in one executable?
   1. linker can search libraries in some directoies when it knows which libraries need to be used. One of the directoies that it looks at is set by LD_LIBRARY_PATH.
   2. They do not live together. Only address in the memory of the library function is added in the final executable. The physical code itself is located at that address.(https://medium.com/@dkwok94/the-linking-process-exposed-static-vs-dynamic-libraries-977e92139b5f)
   * (Optional) What was your favorite part of the assignment?
   * (Optional) How would you improve the assignment?
