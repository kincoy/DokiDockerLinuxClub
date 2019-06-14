### 运行环境
操作系统说明：
* 
>![系统相关信息](https://note.youdao.com/yws/api/personal/file/4D6B46622F1B497092A603E3B64E3B9E?method=download&shareKey=b773c271adfc5924d0ae02fa17964aec "系统相关信息")

文件系统说明：
* 考核附件的简单文件系统，做了适当修改
> 1.因为chroot时出现问题： 
"chroot: failed to run command ‘/bin/bash’: No such file or directory"
解决方法： cp /bin/bash rootfs/bin/bash
复制之后发现还是不行，查了下依赖库还有问题。
解决方法：ldd /bin/bash
然后把这些库复制到简易文件系统中相应位置即可
 >
 2.创建proc文件夹并进行挂载
 mkdir rootfs/proc
 mount -t proc proc roofs/proc
 
### 完成程度
> 1. 文件系统隔离
>
2. 进程隔离
>
3. 网络隔离


### 拓展功能
> 1. UTS隔离
>
2. IPC隔离
>

#### chroot和namespace:
![chroot 和 namespace](https://note.youdao.com/yws/api/personal/file/937D0E4FAC4D4E5AA483F8FF863DF2E5?method=download&shareKey=593bd3491d68665ae0eb1cbca9b0f48e "chroot 和 namespace")

###基础的一些东西
####  1. namespace基本概念
* namespace是LINUX内核用来隔离全局系统资源的方式。通过这种方法，使得不同namespace的进程拥有了不同的资源。并且大多数进程只能了解自己的资源而感受不到其他进程的，从而达到隔离的效果。
> ##### Linux Namespace的6大类型
>
1.Mount Namespace
2.IPC Namespace
3.Network Namespace
4.UTS Namespace
5.PID Namespace
6.User Namespace

#### 2. 创建新namespace的方法（clone）
* int clone(int (*fn)(void *), void *child_stack, int flags, void *arg);
 
 fn是函数指针，child_stack明显是为子进程分配系统堆栈空间，flags就是标志用来描述你需要从父进程继承那些资源， arg就是传给子进程的参数。
 
 这里主要是flags，通过设定不同的flag，就能够达到创建不同类型namespace的效果。
 比如UTS namespace,就需要把flag设为“CLONE_NEWUTS”，可以同时设置多个flag。


#### 3. 一些小知识
* 创建user namespace时，需要映射，如果没有设置的话，默认用户名会是nobody,id和gid都是65534.
 映射方法：添加配置到/proc/PID/uid_map和/proc/PID/gid_map中即可（这里PID指的是新创建namespace中的PID）。
* 在pid namespace中，子命名空间中的进程无法知道父命名空间中进程的存在，父命名空间中的进程能够看到子命名空间中的进程。
 而且子命名空间中的进程除了在当前命名空间中的PID（从1开始）外，还有一个在父命名空间中的PID。
* net namespace都默认有一个lo接口，但物理设备只能属于一个net namespace.而且在新的namespace，lo是默认关闭的，需要自己手动打开。
ip link set lo up
![net](https://note.youdao.com/yws/api/personal/file/E9843B82ABAF42DFBE627F100907B5FC?method=download&shareKey=36c80709ac6ce2777c61c64107f79ec0 "net")
* 当用clone函数创建mount namespace时，新创建的namespace会复制旧namespace的挂载列表，但在这之后两者不会有任何关系，无论改变其中任何一个，对另外一个都没影响。







