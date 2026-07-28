## 项目介绍
本仓库记录我在七天编程课程中的学习内容，主要包含Python基础、OpenCV摄像头调用、MediaPipe手部21个关键点识别、手势判断以及Arduino硬件控制实验。

## 学习目标
- 使用Python读取电脑前置摄像头
- 使用OpenCV处理和显示实时图像
- 使用MediaPipe识别手部21个关键点
- 根据关键点坐标判断拳头、剪刀和Yeah手势
- 练习Arduino代码编写与硬件控制
- 学习使用Git和GitHub管理项目

## 项目目录
- `day1/`：调试食指
- `day2/`：移动多根手指
- `day3/`：同时移动多个手指，摆出动作
- `day4/`：调试多个动作，并编为函数
- `day5/`：手指状态判断
- `day6/`：手势识别与调试
- `day7/`：完整项目


## 运行环境
- Windows 10或Windows 11
- Python 3.10或3.11
- OpenCV
- MediaPipe
- Arduino IDE

## Python环境安装
```bash
python -m venv .venv
.venv\Scripts\activate
python -m pip install --upgrade pip
pip install opencv-python mediapipe
```

## 运行示例
```bash
python day7/final_hand_gesture.py
```

## Arduino程序
使用Arduino IDE打开`.ino`文件，选择正确的开发板和串口后上传。

## 使用说明
1. 克隆或下载本仓库。
2. 安装Python依赖。
3. 确认电脑摄像头未被其他软件占用。
4. 运行对应Python程序。
5. 按程序提示操作，通常按`Q`退出。

## 注意事项
- 不要上传密码、API密钥、访问令牌或个人隐私资料。
- 不同电脑的摄像头编号可能是0、1或2。
- MediaPipe识别效果会受到光线、背景和手部角度影响。
- Arduino程序上传前需要确认开发板型号和串口。

## 项目状态
该项目用于编程学习和课程记录，后续会尝试继续优化代码结构与识别稳定性。

## 作者
刘一夫
