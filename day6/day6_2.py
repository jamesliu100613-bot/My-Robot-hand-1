# 手部关键点检测程序
# 使用OpenCV + MediaPipe实现实时手部检测与手势识别

# 导入OpenCV库（用于图像读取、处理和显示）
import cv2            
# 导入MediaPipe库（用于手部关键点检测）
import mediapipe as mp  


# 初始化手部解决方案
mp_hands = mp.solutions.hands  
# 在图像上绘制关键点和连接线
mp_draw = mp.solutions.drawing_utils  


# 初始化手部检测模型配置
hands = mp_hands.Hands(
    static_image_mode=True,       # True静态图像模式
    max_num_hands=5,              # 最大检测手的数量
    min_detection_confidence=0.6,
    min_tracking_confidence=0.6   #信度
)

# 定义手指关键点索引
FINGER_TIPS = [8, 12, 16, 20]  # 食指、中指、无名指、小拇指指尖ID
FINGER_PIPS = [6, 10, 14, 18]  # 食指、中指、无名指、小拇指中间关节ID


# 手势识别函数：根据手部关键点判断手势
def get_gesture(hand_landmarks) -> str:
    # 获取所有关键点坐标
    lm = hand_landmarks.landmark
    
    # 记录每根手指的状态（1=伸直，0=弯曲）
    fingers_up = []
    
    # 判断大拇指状态（通过x坐标比较指尖与关节位置）
    if lm[4].x < lm[3].x:
        fingers_up.append(0)  # 大拇指弯曲
    else:
        fingers_up.append(1)  # 大拇指伸直
    #注：以左手为主
    
    # 判断其他四根手指状态
    for tip, pip in zip(FINGER_TIPS, FINGER_PIPS):
        if lm[tip].y < lm[pip].y:
            fingers_up.append(1)  # 手指伸直
        else:
            fingers_up.append(0)  # 手指弯曲
    
    # 根据手指状态组合判断手势
    if fingers_up == [0, 1, 0, 0, 0]:
        return "1"              # 食指伸出（数字1）
    if fingers_up == [0, 1, 1, 0, 0]:
        return "yeah"           # 剪刀手（yeah手势）
    if fingers_up == [0, 1, 1, 1, 0]:
        return "3"              # 伸出3根手指
    if fingers_up == [0, 1, 1, 1, 1]:
        return "4"              # 伸出4根手指
    if fingers_up == [0, 0, 0, 0, 0]:
        return "quangtou"       # 拳头
    if fingers_up == [0, 0, 1, 0, 0]:
        return "zhongzhi"       # 中指
    if fingers_up == [1, 0, 0, 0, 1]:
        return "6"              # 6手势（大拇指和小拇指伸出）
    if fingers_up == [1, 1, 1, 1, 1]:
        return "zhangkai"       # 张开手掌
    if fingers_up == [0, 0, 1, 1, 1]:
        return "ok"             # OK手势
    
    # 如果未匹配到预定义手势，返回原始手指状态列表
    return str(fingers_up)


# 打开摄像头（参数0表示笔记本内置摄像头）
camera = cv2.VideoCapture(0)  


# 主循环：持续读取摄像头帧并处理
while True:
    # 读取每一帧图像
    success, image = camera.read()  
    
    # 如果读取失败，退出循环
    if not success:
        break
    
    # 水平翻转图像（镜像效果）
    frame = cv2.flip(image, 1)  
    
    # 将图像从BGR格式转换为RGB格式
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB) 
    
    # 检测手部关键点
    results = hands.process(rgb_frame)  
    
    # 检查是否检测到手部
    if results.multi_hand_landmarks:
        # 获取检测到的手的数量
        hand_count = len(results.multi_hand_landmarks)
        print(f"检测到 {hand_count} 只手")
        # 遍历每只检测到的手
        for hand_landmarks in results.multi_hand_landmarks:  
            # 在图像上绘制关键点和连接线
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            
            # 获取图像尺寸（高度、宽度、通道数）
            height, width, channel = frame.shape
            
            # 遍历每个关键点，标注点的ID编号
            for point_id, landmark in enumerate(hand_landmarks.landmark):
                # 将归一化坐标转换为像素坐标
                pixel_x = int(landmark.x * width)
                pixel_y = int(landmark.y * height)
                # 在关键点位置绘制ID编号
                cv2.putText(frame, str(point_id), (pixel_x, pixel_y), 
                            cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 5)
            
            # 调用get_gesture获取手势识别结果
            gesture_text = get_gesture(hand_landmarks)
            print(f"手指状态: {gesture_text}")
    
    # 未检测到手时的处理
    else:
        print("未检测到手")
        gesture_text = "No Hand"  # 设置默认手势文本（没有手）
    
    
    # 在图像左上角显示手势识别结果
    cv2.putText(
        frame,
        f"GESTURE: {gesture_text}",
        (20, 45),
        cv2.FONT_HERSHEY_SIMPLEX,  # 字体
        0.8,                        # 字体大小
        (0, 255, 255),              # 颜色（黄色）
        2,                          # 线条粗细
        cv2.LINE_AA                 # 抗锯齿
    )
    
    # 显示处理后的图像窗口
    cv2.imshow('Hand Detection', frame)
    
    # 检测键盘输入：按 'q' 键退出程序
    if cv2.waitKey(1) & 0xFF == ord('q'):
        print("退出")
        break


# 释放摄像头资源
camera.release()
# 关闭所有OpenCV窗口
cv2.destroyAllWindows()