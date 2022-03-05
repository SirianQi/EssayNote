[TOC]

参考：
[popen介绍和使用中遇到的一些问题分享](https://km.woa.com/group/20285/articles/show/182767?kmref=search&from_page=1&no=3)
[POPEN填坑总结](https://iwiki.woa.com/pages/viewpage.action?pageId=1524206516)
[popen函数无法获取错误输出](https://www.dazhuanlan.com/jtb456/topics/1715050)
[Linux C popen()函数详解](https://www.cnblogs.com/52php/p/5722238.html)
[Linux C API 参考手册](https://www.kancloud.cn/wizardforcel/linux-c-api-ref)
man手册

# 0. 太长不看版
1. 如果仅需要判断命令是否执行成功，请使用`system()`；
**1.1 如果命令中带有管道，需要在命令前追加`set -o pipefai;`，防止管道屏蔽错误；**
2. 如果需要查看命令的标准输出，请使用`popen()`和`pclose()`；
**2.1 popen()函数返回为0，仅代表子进程fork成功，且没有标准输出，不能作为命令执行成功的标志；**
**2.2 pclose()函数返回的是子进程的返回值，可以作为命令是否执行成功的标志（3除外）；**
**2.3 popen()函数执行的命令中带有管道时，需要在命令前追加`set -o pipefai;`，防止管道屏蔽错误，导致pclose()返回成功；**
**2.4 如果像通过popen()获取错误输出，需要在shell命令后追加`2>&1`重定向；**

**完整实践代码见附件或评论📎**

# 1. system()介绍
 `int system(const char *command);` 
- 功能： `system()` 函数的功能就是用来执行shell命令的，system()函数通过调用 `/bin/sh -c command` 来执行command命令，在执行完成后返回结果，执行过程中SIGCHLD信号会被阻塞，SIGINT和SIGQUIT信号会被忽略。
- 返回： `system()` 返回0代表执行成功，非0代表失败，其中返回-1时代表fork子进程失败，其他为command命令执行状态。如果需要获取执行command命令的子进程返回值，需要调用宏 `WEXITSTATUS(status)` 。
# 2. popen()、pclose()介绍
 `FILE *popen(const char *command, const char *type);` 
- 功能： `popen()` 函数通过创建一个管道，fork出一个进程来调用shell执行 `/bin/sh -c command` 来执行command命令，执行完成后返回FILE指针来保存管道获取的标准输出。这里需要注意，获取到的FILE指针必须要用 `pclose()` 函数关闭。另一个参数type有两个选项，r代表从管道读取数据，w代表写入数据（glibc2.9之后新增了e选项，这里不展开）。
- 返回： `popen()` 函数返回FILE指针来保存管道内容，返回NULL则代表fork或pipe失败或无法分配内存。如果需要获取命令返回值则需要使用 `pclose()` 函数。
 `int pclose(FILE *stream);` 
- 功能：等待 `popen()` 函数创建的子进程结束，并获取command命令执行状态。
- 返回： `pclose()` 函数返回-1代表等待子进程过程中报错，其他为command命令执行状态。

# 3. 执行shell命令的不同场景
## 3.1 执行shell命令，判断是否成功
如果我们需要在c++程序中执行shell命令，可以直接使用`system()`方法。
```
// system执行shell命令
int ExecuteCmdSys(const string& os_cmd) {
  int result = 0;
  pid_t status = system(os_cmd.c_str());

  if (status == -1) {  // 调用system失败
    result = status;
  } else {                    // 调用成功
    if (WIFEXITED(status)) {  // 正常退出
      if (WEXITSTATUS(status) == 0) {  // 正常退出 & 执行成功。如果有些代码用非0作为返回值，那就坑爹（一般不会）
        result = 0;
      } else {  //正常退出 & 执行失败
        result = status;
      }
    } else {  // 非正常退出： 一般是调用子进程异常等
      result = status;
    }
  }

  return result;
}
```
## 3.2 执行shell命令，获取标准输出
如果我们需要执行shell命令，并且获取shell命令的标准输出，可以使用`popen()`和`pclose()`函数。
```
// popen执行shell命令
int ExecuteCmd(const string& os_cmd) {
  FILE* p_out = NULL;  // shell命令执行输出流
  char res[1024] = {0}; // shell命令标准输出
  int result = 0;      // 向外返回错误码
  try {
    p_out = popen(os_cmd.c_str(), "r"); // 执行命令
    if (p_out != NULL) {
      fread(res, 1024, 1, p_out);  // 获取程序输出
      printf("stdout:%s, return%d\n", res);
    } else {
      result = -1;  // fork子进程失败
    }
		pclose(p_out);
		p_out = NULL;
  } catch (...) {
    // 捕获异常 关闭文件
    if (p_out) {
      pclose(p_out);
      p_out = NULL;
    }
    result = -1;  // 记录结果
  }
  return result;
}
```
## 3.3 执行shell命令，判断是否成功，并获取shell命令的标准输出
如果我们需要执行shell命令，判断是否成功，并获取shell命令的标准输出，可以使用`popen()`和`pclose()`函数。
```
// popen执行shell命令
int ExecuteCmd(const string& os_cmd) {
  FILE* p_out = NULL;  // shell命令执行输出流
  char res[1024] = {0}; // shell命令标准输出
  int p_ret_code = 0;  // shell命令执行结果
  int result = 0;      // 向外返回错误码
  try {
    p_out = popen(os_cmd.c_str(), "r"); // 执行命令
    if (p_out != NULL) {
      fread(res, 1024, 1, p_out);  // fork子进程成功 获取程序输出
    } else {
      result = ERR_EXEC_CMD;  // fork子进程失败
    }
    p_ret_code = pclose(p_out);  // 获取程序执行结果
    p_out = NULL;
    printf("%s; stdout:%s, return%d\n", os_cmd.c_str(), res, p_ret_code);
    if (p_ret_code != 0) {
      result = p_ret_code;  //  子进程执行失败
    }
  } catch (...) {
    // 捕获异常 关闭文件
    if (p_out) {
      p_ret_code = pclose(p_out);
      p_out = NULL;
    }
    result = ERR_EXEC_CMD;  // 记录结果
  }
  return result;
}
```
## 3.4  执行shell命令，判断是否成功，并获取shell命令的错误输出
如果我们需要执行shell命令，判断是否成功，并获取shell命令的标准输出，可以使用`popen()`和`pclose()`函数。
具体使用方法与1.4相同，不同的是**需要在shell命令后追加重定向，将错误输出重定向到标准输出上**：
```
int main() {
  int ret = 0;
  string cmd_core_dump = "tar -czvf target.tgz fake_file_1 2>&1";  // 有错误输出 进程异常结束
  ret = ExecuteCmd(cmd_core_dump);
  printf("ExecuteCmd return:%d\n", ret);
  return 0;
}
```
终端输出：
```
[siyuanqi@vm ~]$ ./test_popen
tar -czvf target.tgz fake_file_1 2>&1; stdout:tar: fake_file_1: Cannot stat: No such file or directory
tar: Exiting with failure status due to previous errors
, return512
ExecuteCmd return:512
```

## 3.5 执行含有管道命令的shell命令，并判断是否成功
执行含有管道命令的shell命令，并判断是否成功，可以使用`system()`函数或`popen()`+`pclose()`，关键在于需要在shell命令前追加`set -o pipefail`命令。
```
int main() {
  int ret = 0;
  // 有错误输出 有管道 子进程异常结束 管道不屏蔽错误
  string cmd_pipe_err2 = "set -o pipefail;tar -czvf target.tgz fake_file_1|cat ";
  ret = ExecuteCmd(cmd_pipe_err2);
  printf("ExecuteCmd return:%d\n", ret);
  ret = ExecuteCmdSys(cmd_pipe_err2);
  printf("ExecuteCmdSys return:%d\n", ret);
  return 0;
}
```
终端输出：
```
[siyuanqi@vm ~]$ ./test_popen
tar: fake_file_1: Cannot stat: No such file or directory
tar: Exiting with failure status due to previous errors
set -o pipefail;tar -czvf target.tgz fake_file_1|cat ; stdout:, return512

ExecuteCmd return:512
tar: fake_file_1: Cannot stat: No such file or directory
tar: Exiting with failure status due to previous errors
pid:512
ExecuteCmdSys return:512
```
# 4 总结分析
## 4.1 system()和popen()使用场景
`system()`是标准的执行命令函数，一般情况建议使用system()；
`popen()`是通过管道的方式fork出一个子进程，再调起/bin/sh执行shell命令，当需要获取命令的输出时可以使用，但要与`pclose()`配合使用；
## 4.2 popen()与pclose()使用
使用`popen()`时最好获取判断`pclose()`返回值，保证进程执行成功；
`popen()`返回的FILE指针只接受shell进程产生的标准输出，所以如果希望获取报错信息，需要在命令后追加`2>&1`将错误输出重定向到标准输出查看；
## 4.3 pipe带来的特殊情况
pipe命令会屏蔽整个shell命令中的部分错误，导致判断shell进程返回值不符合预期，所以需要在命令前追加`set -o pipefai;`，可以让整个命令在第一个失败的进程就返回；
## 4.4 子进程core dumped
我们部分场景是通过shell命令调用其他可执行程序，当可执行程序core dumped时，`pclose()`和`system()`获取到的返回值为子进程pid；
此时core dumped信息由系统输出，所以也无法获取；
类似如下输出：
```
[siyuanqi@vm ~]$ ./test_popen
sh: line 1: 10435 Segmentation fault      (core dumped) ./core_dump
     10436 Done                    | cat
set -o pipefail;./core_dump|cat ; stdout:, return35584

ExecuteCmd return:35584
sh: line 1: 10438 Segmentation fault      (core dumped) ./core_dump
     10439 Done                    | cat
pid:35584
ExecuteCmdSys return:35584
```