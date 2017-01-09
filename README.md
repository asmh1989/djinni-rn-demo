### 前提
安装 node.js


#### 步骤
```
#更新依赖库
git submodule update --init

#更新npm
npm install

#清理
make clean

#library工程
make library

#愉快的开发
#ios
open ios/...project

#android
import Android/.. to android-studio

```


> 完善的ide开发支持
> 
> android/ios 调试断点都支持(java/objc)
> 
> js 断点调试 react-native的方式


### ios project
`libstml` , c++ 代码, 底层处理库, 主要负责stml协议 和 数据缓存

`supoort_lib`, c++ djinni 库, 提供便捷的接口 设置

`crypto`, c++ 加密库

`RNSmobiler` react-native 工程

### android project

`react-native-stml`, stml, c++ , java代码, 编译成`react-native` 插件的形式

