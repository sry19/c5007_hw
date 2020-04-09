## one core
# single-threaded DirectoryParser:
* Took 570.146464 seconds to execute.
* 639209 entries in the index
* Crawled 53 files

# 5 threads:
* 676.927030 seconds to execute.
* 639209 entries in the index
* Crawled 53 files

# 10 threads:
* 1545.831295 seconds to execute.
* 639209 entries in the index.
* Crawled 53 files

## experiment 1
# one core

# DirectoryParser_MT with one thread:
* Took 538.780041 seconds to execute.
* 639209 entries in the index
* Crawled 53 files

# Single-threaded DirectoryParser:
* Took 549.682389 seconds to execute.
* 639209 entries in the index
* Crawled 53 files

Explanation: if the machine has only one core, there won't be much benefit when using multiple threads. Additionally, creating more threads may slow this program.

## experiment 2
## 3 cores
## one·core$
# single-threaded·DirectoryParser:
* Took 547.973791 seconds to execute.
* 639209 entries in the index
* Crawled·53·files$

# 5 threads:
* Took 829.315983 seconds to execute.
* 639209 entries in the index
* Crawled 53 files

# 10 threads:
* Took 1324.569971 seconds to execute.
* 639209 entries in the index

Explanation: The purpose of this experiment is to show how multiple cores will benefit the multi-threaded approaches. However, I think because there are a small number of rows in each file, the benefit is not very obvious.
