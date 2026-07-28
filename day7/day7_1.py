# 手部关键点检测程序 - 实时手势识别并通过串口发送给Arduino
import cv2            # 图像处理库
import mediapipe as mp  # 手部关键点检测库
import serial         # 串口通信库
import time           # 时间控制

# 初始化MediaPipe手部检测模块
mp_hands = mp.solutions.hands  
mp_draw = mp.solutions.drawing_utils  # 绘制工具

# 串口配置
SERIAL_PORT = "COM3"
BAUD_RATE = 115200
last_sent_gesture = ""  # 记录上次发送的手势，避免重复发送



# 手部检测模型初始化
hands = mp_hands.Hands(
    static_image_mode=True,       # 静态图像模式
    max_num_hands=5,              # 最多检测5只手
    min_detection_confidence=0.6, # 检测置信度阈值
    min_tracking_confidence=0.6   # 跟踪置信度阈值
)

# 手指关键点索引定义
FINGER_TIPS = [8, 12, 16, 20]  # 食指、中指、无名指、小指指尖
FINGER_PIPS = [6, 10, 14, 18]  # 食指、中指、无名指、小指中间关节


# 手势识别函数：根据关键点坐标判断手势（左手为主）
def get_gesture(hand_landmarks) -> str:
    lm = hand_landmarks.landmark  # 获取关键点坐标
    fingers_up = []               # 存储手指状态：1=伸直，0=弯曲
    
    # 大拇指判断（比较指尖与关节x坐标）
    fingers_up.append(0 if lm[4].x < lm[3].x else 1)
    
    # 其他四指判断（比较指尖与中间关节y坐标）
    for tip, pip in zip(FINGER_TIPS, FINGER_PIPS):
        fingers_up.append(1 if lm[tip].y < lm[pip].y else 0)
    
    # 根据手指状态匹配手势
    gesture_map = {
        (0, 1, 0, 0, 0): "1",       # 食指
        (0, 1, 1, 0, 0): "yeah",    # 剪刀手
        (0, 1, 1, 1, 0): "3",       # 三指
        (0, 1, 1, 1, 1): "4",       # 四指
        (0, 0, 0, 0, 0): "quangtou",# 拳头
        (0, 0, 1, 0, 0): "zhongzhi",# 中指
        (1, 0, 0, 0, 1): "6",       # 6手势
        (1, 1, 1, 1, 1): "zhangkai",# 张开手掌
        (0, 0, 1, 1, 1): "ok"       # OK手势
    }
    return gesture_map.get(tuple(fingers_up), str(fingers_up))


# 打开摄像头（参数0表示默认摄像头）
camera = cv2.VideoCapture(0)  

# 连接Arduino串口
try:
    arduino = serial.Serial(port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=1)
    time.sleep(2)  # 等待串口稳定
    print(f"串口连接成功: {SERIAL_PORT} @ {BAUD_RATE}")
except serial.SerialException as error:
    camera.release()
    hands.close()
    cv2.destroyAllWindows()
    print(f"串口连接失败: {error}")
    print("请检查串口连接，确保关闭其他串口监视器")
    raise SystemExit

# 主循环：实时处理摄像头图像
while True:
    success, image = camera.read()  # 读取一帧图像
    if not success: break           # 读取失败则退出
    
    frame = cv2.flip(image, 1)      # 水平翻转实现镜像效果
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # BGR转RGB
    results = hands.process(rgb_frame)  # 检测手部关键点
    
    # 检测到手部的处理
    if results.multi_hand_landmarks:
        print(f"检测到 {len(results.multi_hand_landmarks)} 只手")
        for hand_landmarks in results.multi_hand_landmarks:  
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)  # 绘制关键点
            
            # 在每个关键点上标注ID编号
            height, width = frame.shape[:2]
            for point_id, landmark in enumerate(hand_landmarks.landmark):
                pixel_x, pixel_y = int(landmark.x * width), int(landmark.y * height)
                cv2.putText(frame, str(point_id), (pixel_x, pixel_y), 
                            cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 5)
            
            gesture_text = get_gesture(hand_landmarks)  # 识别手势
            print(f"手势: {gesture_text}")
    else:
        gesture_text = "No Hand"  # 未检测到手
    
    
    # 在图像左上角显示手势结果（黄色文字）
    cv2.putText(frame, f"GESTURE: {gesture_text}", (20, 45),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2, cv2.LINE_AA)
    
    # 手势变化时发送到Arduino
    if gesture_text != last_sent_gesture:
        packet = f"GESTURE: {gesture_text}\n"
        try:
            arduino.write(packet.encode("ascii"))
            print(f"发送: {packet.strip()}")
            last_sent_gesture = gesture_text
        except serial.SerialException as error:
            print(f"发送失败: {error}")
            break
    
    cv2.imshow('Hand Detection', frame)  # 显示图像窗口
    
    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        print("退出程序")
        break

# 释放资源
camera.release()
cv2.destroyAllWindows()