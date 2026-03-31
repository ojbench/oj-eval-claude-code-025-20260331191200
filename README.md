# Problem 025 - 抄袭与查重

**ACMOJ Problem ID**: 1939

## Table of Contents

- [Problem 025 - 抄袭与查重](#problem-025-抄袭与查重)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [OJ Git Repository Compilation Process](#oj-git-repository-compilation-process)
    - [Git Configuration Requirements](#git-configuration-requirements)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Build a program plagiarism detection system AND a code transformer. Involves parsing a custom language with arrays, functions, closures.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **1939. 抄袭与查重** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/025/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

题目平台已经下线，平台源代码见 <https://github.com/ACMClassCourse-2022/ppca-5b-collab/>。

***

13:56 Update: 函数传递的参数中，数组是传引用的，不是传值。例如

```
(function (add a)
  (array.set a 0 1))

(function (main)
  (block
    (set a (array.create 10))
	(array.set a 0 0)
	(add a)
	(print (array.get a 0))))
```

会输出 1 而不是 0.

***

在过去的两个学期里，助教们对同学们的作业进行了多次查重。不过，一般的查重都是由程序完成的，因为助教没有时间一一对每两位同学的代码进行手动对比。既然助教不太会看查重系统检测不到的代码，想要在学期里彻底摆烂的你决定，用魔法对抗魔法：直接写一个抄袭程序，以 Frqohvv 或者 GdunVkdusqhvv 提交的作业代码作为输入，输出一个查重系统检测不到的代码，作为自己的作业交上去。

在今天的机考里，你需要……

1. 写一个上文所述的抄袭程序；
2. 写一个程序对其他同学抄袭程序的输出进行查重。

C++ 的处理对一次机考来说有点太复杂了。所以我们操作的语言是一种极简但仍然可用的编程语言。其细节详见 [题目 repo][repo] 中的 [lang.md][langmd]。Repo 中还给出了 [语言的解释器][interp]、[抄袭程序][cheat] 和 [查重程序][anticheat] 的模板。你可以在本次机考中随意使用、修改下发 repo 中包含的代码。

[repo]: https://github.com/ACMClassCourse-2022/ppca-5b
[langmd]: https://github.com/ACMClassCourse-2022/ppca-5b/blob/master/lang.md
[interp]: https://github.com/ACMClassCourse-2022/ppca-5b/blob/master/eval.cpp
[cheat]: https://github.com/ACMClassCourse-2022/ppca-5b/blob/master/cheat.cpp
[anticheat]: https://github.com/ACMClassCourse-2022/ppca-5b/blob/master/anticheat.cpp

## 评分方法

对于抄袭程序，我们会将八份 `.p` 的代码输入你的程序，然后检查你给出的程序和原有程序的功能是否一致 (通过跑测试点)。对于与原有功能一致的程序，我们会将你输出的 `.p` 代码和原有的 `.p` 代码一起交给其他同学提交的查重程序和若干 baseline 查重程序。你的得分是 (1 - 重复度) 的平均数。我们保证所有输入代码在人类看来都是明显不同的；例如，如果要解决的问题是最短路，那么输入代码可能是一份 Dijkstra 和一份 Bellman-Ford。

对于查重程序，我们会将其他同学和若干 baseline 生成的重复代码 (记作 `2.p`) 与原有代码 (`1.p`) 放在一起，输入你的程序，题目保证两份输入程序的功能相同。`2.p` 有可能是从 `1.p` 派生过来的 (就是说，`2.p` 是一份抄袭程序以 `1.p` 作为输入生成的)，也有可能不是 (即 `2.p` 是以另外一份代码作为输入生成的)。你需要输出 `1.p` 和 `2.p` 的重复度 $s$.

- 如果 $s=0.5$ 则你的程序将会获得 0 分。
- 如果 $s<0.5$, 则表示很可能代码是不重复的。数字越小代表你对结论越有信心。此时：
  - 如果 `1.p` 与 `2.p` 确实不同，则你将获得 $2\times(0.5-s)$ 的分数。
  - 否则，你将被扣去 $2\times(0.5-s)$ 的分数。
- 如果 $s>0.5$, 则表示很可能代码是重复的。数字越大代表你对结论越有信心。此时：
  - 如果 `1.p` 与 `2.p` 确实同源，则你将获得 $2\times(s-0.5)$ 的分数。
  - 否则，你将被扣去 $2\times(s-0.5)$ 的分数。

在计算两道题目总分的时候不会计入查重程序可能造成的负分。

出于明显的原因，**测试点不下发，禁止获取或试图获取测试点的全部或部分内容，否则本次机考以 0 分计**。

## 输入输出格式

### 抄袭程序

从标准输入读入一个程序，将修改后的程序输出到标准输出。你可以用 `scanProgram(std::cin)` 来读入；详见样例代码。

如果你的程序产生了运行错误、输出了非法的程序，或者输出了功能错误的程序，则你在此测试点上不能获得分数。

### 查重程序

标准输入流会依次输入：

1. 一个程序，以 `endprogram` 结束；
2. 另一个程序，以 `endprogram` 结束。`scanProgram()` 知道 `endprogram` 是结束符，所以直接调用两次即可，详见样例代码；
3. 这道题目的参考输入。(你可以用这些输入试着执行所给出的两个程序，来辅助你的判断。详见样例代码。)

你需要输出一个 0 到 1 之间的数 (闭区间)。超出范围的数会被压到 0 或 1。程序运行错误则视为输出 0。

## 资源限制

一次运行的时限是 500 ms，内存限制 512 MiB。所有 `.p` 程序需要在 $1\times10^6$ 个时刻内完成执行，否则记为超时；题目保证原始代码能在 6000 个时刻内完成执行。(所以正常来说不会产生超时。)

## 如何提交

现在的 ACMOJ 不太支持如此奇怪的评测方式，所以我们在一个临时平台上进行评测，地址为 <del><https://acm.sjtu.edu.cn/OnlineJudge/ppca-5b/></del><ins>平台已下线</ins>。你需要以真实学号注册帐号，否则助教将无法统计分数。你需要以真实学号注册帐号，否则助教将无法统计分数。

由于评测任务量较大，在五分钟内你只能对一道题目 (抄袭/查重) 提交一次代码，但如果你提交的代码本身产生了编译错误，则可以立即重新提交 (会重置 CD)。

在 repo 中运行 `make -j` 即可编译出 `./cheat` 和 `./anticheat`。**请提交 `make` 所产生的 `cheat-submit.cpp` 和 `anticheat-submit.cpp`，而不是 `cheat.cpp` 或 `anticheat.cpp`。**

### Input Format

See the problem description above.

### Output Format

See the problem description above.

### Samples

No sample data provided for this problem.

### Data Constraints

See the problem description for constraints.

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 1000 ms
- **Memory Limit (per test case)**: 512 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/025/data_test/`.

## Submission Requirements

### OJ Git Repository Compilation Process

For Git compilation, we will first clone the repository using a command similar to:
```bash
git clone <repo_url> . --depth 1 --recurse-submodules --shallow-submodules --no-local
```

Then we check if there is a `CMakeLists.txt` file. If it exists, we run (if not, a warning message will be displayed):
```bash
cmake .
```

Finally, we check if there is any of `GNUmakefile`/`makefile`/`Makefile` (if cmake was run previously, this will be the generated Makefile). If it exists, we run (if not, a warning message will be displayed):
```bash
make
```

After this process is complete, we will use the `code` file in the project root directory as the compilation result.

A `CMakeLists.txt` file is provided in the project. You can use or modify it as needed. The local environment has gcc-13 and g++-13 available.

### Git Configuration Requirements

**IMPORTANT**: You must create a `.gitignore` file in your project root directory to avoid OJ evaluation conflicts.

The `.gitignore` file should include at least the following entries:

```gitignore
CMakeFiles/
CMakeCache.txt
```

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The compiled executable file name must be `code`
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
