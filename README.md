
### 前提
安装 node


#### 步骤
```
#更新依赖库
git submodule update

#更新npm
npm install

#清理
make clean

#library工程
make library

#愉快的开发

open ios/...project


```


> 暂时只支持 ios xcode开发

> xcode 可以开发 C++/Objc

> webstorm 修改js


### project
`libstml` , C++ 代码, 底层处理库, 主要负责stml协议 和 数据缓存

`supoort_lib`, C++ djinni 库, 提供便捷的接口 设置

`crypto`, C++ 加密库

`RNSmobiler` react-mative 工程
