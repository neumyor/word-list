# 北航软件工程 结对编程项目 单词链

开发人员：郭衍培、牛易明

### 编译的有关事项

如若需要编译Release版本的`exe`文件和`.dll`动态链接库，请在``WordChain.h`中添加宏`#define Release`

`.dll`文件请采用x64进行编译，`exe`在`x86`下进行了测试

### 异常测试的有关事项

进行异常测试时请在测试生成的unittest.dll文件同目录下添加temp.txt文件和noring.txt

异常测试时请注意在Debug模式下进行生成

```
//temp.txt//
ab bc cd de ea ab bc cd De ea ab Dc cd dd db jk kl lz
//noring.txt//
ab bc cd de
```

### GUI使用指南

将gui.exe与core.dll放在同一目录下，在64位的win10环境可以正常运行。

 如若需要编译图形界面和相应的动态链接库，请采用64位的python版本，且在x64配置下生成计算模块的动态链接库，以保证前后端能够正确衔接。

### 单元测试相关事项

请在Debug x64情况下生成单元测试

