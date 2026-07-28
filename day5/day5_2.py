#检测代码，检测有几只手，并打开摄像头，显示窗口，按q键退出
import cv2             # 导入OpenCV库
import mediapipe as mp  # 导入MediaPipe库

mp_hands = mp.solutions.hands # 导入手部检测模型
mp_draw = mp.solutions.drawing_utils #导入绘制工具库

hands = mp_hands.Hands(
    static_image_mode=True,max_num_hands=10,min_detection_confidence=0.6,min_tracking_confidence=0.6)# 初始化手部检测模型
    # 参数说明：
    #     static_image_mode: 是否静态图像模式，False表示连续视频流,True表示静态图像
    #     max_num_hands: 最大检测手数
    #     min_detection_confidence: 检测置信度阈值
    #     min_tracking_confidence: 追踪置信度阈值
camera = cv2.VideoCapture(0) # 打开摄像头
while True:
    success, image = camera.read() # 读取摄像头帧数
    if not success:
        break
    frame = cv2.flip(image, 1) # 水平翻转图像
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB) 
    results = hands.process(rgb_frame) # 处理图像帧
    if results.multi_hand_landmarks:
        hand_count = len(results.multi_hand_landmarks)
        print(f"检测到 {hand_count} 只手")
    else:
        print("未检测到手")
    cv2.imshow('Hand Detection', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'): # 按q键退出
        print("退出")
        break
camera.release()
cv2.destroyAllWindows()