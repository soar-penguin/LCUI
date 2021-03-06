# The LCUI Project

![LCUI Logo](https://raw.github.com/lc-soft/LCUI/gh-pages/files/images/lc-soft-logo.png)

## 效果图
![screenshort 1](http://lcui.org/files/images/helloworld-01.png)
![screenshort 2](http://lcui.org/files/images/label-01.png)
![screenshort 3](http://lcui.org/files/images/devlog/test_textbox-05.gif)
![screenshort 4](http://lcui.org/files/images/devlog/2012-12-03-17-24-03.png)
![screenshort 5](http://lcui.org/files/images/mora-01.gif)
![screenshort 6](http://lcui.org/files/images/test_LCPV-3.gif)

## 说明

LCUI 是 GNU/Linux 平台上的一款自由软件，主要面向嵌入式，注重易用性和性能。

源代码基于 GNU通用公共许可协议 发布，任何人都有运行、研究、复制、发布、修改和改进LCUI的自由。

我们正努力使它成为一款成熟、稳定、高效的GUI开发工具。

请阅读 `docs/CHANGES.zh-cn` 文件了解新版本的更新内容。

阅读 `docs/FAQ.zh-cn` 文件可以找到常见问题的解决方法。

阅读 `docs/LICENSE.TXT`  文件了解 GPLv2 许可协议的具体内容。

项目主页：http://www.lcui.org

联系邮箱：lc-soft@live.cn

项目托管：https://github.com/lc-soft/LCUI

LCUI 的相关信息与资料可以在项目主页中找到。

如果遇到问题、发现BUG或者有好的建议和想法，可以在项目主页上留言或发送邮件告诉我们。

我们已经使用 GitHub 托管 LCUI 项目，网址在上面，欢迎您参与 LCUI 项目的优化和改进。


## 文档

开发文档还未编写，等到 LCUI 发展得比较成熟的时候会考虑编写，现阶段，你可以参考的头文件，
源代码，以及示例程序。

## 构建

你需要运行 `./configure` (在源码根目录中) 以引导项目的构建。

在最简单的情况下，你可以运行：

	git clone https://github.com/lc-soft/LCUI.git
	cd LCUI
	git submodule update --init
	./configure

在 LCUI 的项目主页上发布的源码包，一般是比较稳定的版本，你只需要这样做：

1. 解压LCUI的源码包。
2. 进入解压后得到的新文件夹。
3. 运行源码根目录中的configure脚本，以对系统环境进行检测。
4. 使用make命令编译 LCUI 的源代码。
5. 等待编译成功完成，如果在编译过程中遇到错误，请检查您的系统中是否有完整的编译环境。
6. 使用make install命令安装 LCUI 的函数库和头文件。

### 系统需求

* 字体处理： freetype2
* 触屏支持： tslib
* 线程支持： pthread
* 图片处理： libpng 和 libjpeg

为了你能正常使用LCUI，请先确保您的计算机已经安装了上述的函数库，并保持较新的版本。
理论上，LCUI 能在 GNU/Linux 系统环境中正常工作，LCUI 的图形输出，需要内核支持 FrameBuffer。

## 贡献

你可以通过GitHub发送pull请求，patch应符合以下要求：

1. 遵循现有代码风格。
2. 一个commit应该做完整的一件事。
3. commit信息应该从低于80个字符的摘要行后的空行开始，然后推理/分析为什么要修改（如果适
   用）。
4. 修复之前的commit（已经合并）中存在的bug，commit信息的摘要行应该以`fixup!`开头，
   表示修复了它。如果你修复的是issues列表中未解决的bug，commit信息的摘要行应该以
   `fix->`开头，并加上issues列表中对应bug信息的标题。
5. 衍合你的分支至源项目的master分支。我们不希望拉取到多余的合并提交。
6. **要清楚什么许可证适用于你的patch：** 该库中的文件基于GPLv2（或更高版本），但（原
   作者）我们仍然可以创建非自由的衍生工具。然而，如果给我们的补丁是基于GPL的，我们希望它
   以后不会进入任何非自由的衍生工具，因此，如果补丁发布于公共领域，会为我们带来方便（以及
   避免任何法律问题）。


### GitHub流程

开发补丁应遵循以下流程：

#### 初始设置

1. 在GitHub上进行Fork (点击 Fork 按钮)
2. Clone到计算机： `git clone git@github.com:«github account»/LCUI.git`
3. cd 到你的库里面： `cd LCUI`
4. 设置upstream至remote： `git remote add -f upstream git://github.com/lc-soft/LCUI.git`

#### 添加Feature

1. 为新的feature创建一个分支(branch)： `git checkout -b my_new_feature`
2. 在你的feature上工作, 像往常一样添加和提交

创建一个分支并非必须的，但是，当已经合并至源项目时可以方便删除你的分支，比较你的分支和源
项目的最终版本，然后提交拉取请求。

#### 推送到GitHub

1. 推送分支至GitHub： `git push origin my_new_feature`
2. Issue拉取请求： 在Github上点击 Pull Request 按钮

#### 有用的命令

如果源项目发生了很大的变化，最重要的是，你可以重现你的本地修改，这叫做`rebase`，例如：

	git fetch upstream
	git rebase upstream/master

这样会获取更改并重新应用你的commits。

这一般比合并更好，它会给出一个清晰的画面，以表示哪些commit是本地到了你的分支中，如果同
样的变更已经在源项目中应用，它也会“修剪”你的本地的commit。

你可以将`-i`和`rebase`一起使用，以选择“交互式”衍合，这允许你移除、重排、合并以及修改
提交信息，例如：

	git rebase -i upstream/master


--- end of README.zh-cn.md ---
