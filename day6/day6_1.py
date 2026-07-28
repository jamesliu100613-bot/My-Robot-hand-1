# 手部关键点检测
# 使用OpenCV + MediaPipe实现实时手部检测
# 导入OpenCV库
import cv2            
# 导入MediaPipe库
import mediapipe as mp  


mp_hands = mp.solutions.hands  
mp_draw = mp.solutions.drawing_utils  

# 初始化手部检测模型
# - max_num_hands: 最大检测手的数量，设置为5
hands = mp_hands.Hands(
    static_image_mode=True,
    max_num_hands=5,
    min_detection_confidence=0.6,
    min_tracking_confidence=0.6
)

FINGER_TIPS = [8, 12, 16, 20]
FINGER_PIPS = [6, 10, 14, 18]
#食指，中指，无名指，小拇指的指节点ID
def get_gesture(hand_landmarks) ->str:
    lm  = hand_landmarks.landmark
# lm[3] 是大拇指的指节点，lm[4] 是大拇指指的指尖点
# lm[8] 是食指的指尖点，lm[12] 是中指的指尖点，lm[16] 是无名指的指尖点，lm[20] 是小拇指的指尖点
# lm[6] 是食指的指节点，lm[10] 是中指的指节点，lm[14] 是无名指的指节点，lm[18] 是小拇指的指节点
    fingers_up = []
    if lm[4].x < lm[3].x:
        fingers_up.append(0)
    else:
        fingers_up.append(1)
    for tip, pip in zip(FINGER_TIPS, FINGER_PIPS):
        if lm[tip].y < lm[pip].y:
            fingers_up.append(1)
        else:
            fingers_up.append(0)
    return fingers_up

# 打开摄像头，参数0表示笔记本内置摄像头
camera = cv2.VideoCapture(0)  

# 主循环：持续读取摄像头帧并处理
while True:
    # 读取一帧图像
    # success: 布尔值，表示是否成功读取
    success, image = camera.read()  
    # 如果读取失败，退出循环
    if not success:
        break
    # 水平翻转图像
    frame = cv2.flip(image, 1)  
    # 将图像从BGR格式转换为RGB格式
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB) 
    #检测手部关键点
    results = hands.process(rgb_frame)  
    # 检查是否检测到手部
    if results.multi_hand_landmarks:
        # 获取检测到的手的数量
        hand_count = len(results.multi_hand_landmarks)
        print(f"检测到 {hand_count} 只手")
        
        # 遍历每只检测到的手
        for hand_landmarks in results.multi_hand_landmarks:  
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)
            
            # 获取图像尺寸（高度、宽度、通道数）
            height, width, channel = frame.shape
            
            # 遍历每个关键点，标注点的ID编号
            for point_id, landmark in enumerate(hand_landmarks.landmark):
                # 将归一化坐标转换为像素坐标
                pixel_x = int(landmark.x * width)
                pixel_y = int(landmark.y * height)
                cv2.putText(frame, str(point_id), (pixel_x, pixel_y), 
                            cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 5)
            # 调用get_gesture获取手指状态
            fingers_up = get_gesture(hand_landmarks)
            print(f"手指状态: {fingers_up}")
    else:
        # 未检测到手时的提示
        print("未检测到手")
    # 显示处理后的图像窗口
    cv2.imshow('Hand Detection', frame)
    
    # 检测键盘输入
    # 按 'q' 键退出程序
    if cv2.waitKey(1) & 0xFF == ord('q'):
        print("退出")
        break
camera.release()
# 关闭所有窗口
cv2.destroyAllWindows()











