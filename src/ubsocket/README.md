# 编译
当前需要集成在umdk-urpc中编译，后续再做编译整改。

## 编译urpc
```
# 下载umdk源码，使用master分支即可
git clone https://gitee.com/openeuler/umdk.git

cd umdk/src
mkdir build && cd build
# 仅编译urpc
cmake -DBUILD_URPC=enable -DBUILD_ALL=disable ..
make -j32
```

## 编译adapter
在编译adapter前，需安装[libboundscheck](https://gitee.com/openeuler/libboundscheck.git)，其提供了securec能力。
Euler系统上使用`yum install libboundscheck`命令安装即可，其它系统可能要源码安装。

```
# adapter代码需要放到指定位置
cd umdk/src/urpc/
git clone https://gitee.com/fanzhaonan/brpc_adaptor.git
# 重新编译urpc
cd umdk/src/build
rm -rf *
cmake -DBUILD_URPC=enable -DBUILD_ALL=disable ..
make -j32
```
