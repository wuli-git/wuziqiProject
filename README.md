# -
使用SFML图形库和visualstudio c++2022写成的五子棋游戏
  使用本项目,有如下引导:
  1.创建非linux使用的空项目
  2.右击项目添加五子棋文件夹中的头文件和cpp文件,共五个
  3.在配置属性中,确保自己的C++语言标准在17或以上,选中C++17,平台工具集是visualstudio2022
  4.在配置属性中,确保调试器类型为仅限本机,而非远程linux
  5.在C/C++中,选中常规->附加包含目录:D:\SFML\SFML-2.5.1\include,即你的SFML所在文件夹可找到它
  6在链接器中->常规->附加库目录:D:\SFML\SFML-2.5.1\lib,即你的SFML所在文件夹中可找到它
  7.在链接器中->常规->附加依赖项:sfml-graphics-d.lib;sfml-system-d.lib;sfml-window-d.lib复制这三个依赖项
  8.将sfml-window-d-2.dll;sfml-graphics-d-2.dll;sfml-system-2.dll三个文件复制到项目文件夹中的Debug目录下,这三个文件在SFMLD:\SFML\SFML-2.5.1\bin下面
