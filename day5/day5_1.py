# 图像创建与读取
import numpy as np
import cv2
from pathlib import Path

def make_sample_image(path: Path):
    # 创建一个360x640的RGB图像，包含灰色背景和简单几何图形
    image = np.zeros((360, 640, 3), dtype=np.uint8)
    
    # 将整个图像填充为灰色背景（BGR格式：(蓝, 绿, 红)）
    # (45, 45, 45) 对应灰色
    image[:] = (45, 45, 45)
    
    # 在图像上绘制一个填充矩形
    # 参数：图像、左上角坐标、右下角坐标、颜色(BGR)、线宽(-1表示填充)
    # 注意：坐标(-280, -270)和(280, 270)相对于图像中心(320, 180)
    cv2.rectangle(image, (-280, -270), (280, 270), (220, 120, 30), -1)
    
    # 创建三角形顶点坐标数组
    sanjiaoxing = np.array([[100, 100], [200, 200], [300, 100]], np.int32).reshape((-1, 1, 2))
    
    # 绘制三角形
    # thickness=1表示线宽为1像素
    cv2.polylines(image, [sanjiaoxing], isClosed=True, color=(220, 120, 30), thickness=1)
    
    # 绘制一个圆形
    # 参数：图像、圆心坐标、半径、颜色、线宽
    cv2.circle(image, (200, 200), 50, (220, 120, 30), 1)
    
    # 将图像保存到指定路径
    cv2.imwrite(str(path), image)
def main() -> None:
    image_path = Path("diyizhang.png")
    if not image_path.exists():
        make_sample_image(image_path)
    image = cv2.imread(str(image_path))
    if image is None:
        raise RuntimeError(f"无法读取图片 {image_path.resolve()}")
    height, width,channel= image.shape
    center_pixel = image[height//2, width//2]
    print("图片路径",image_path.resolve())
    print("shape",image.shape)
    print("dtype",image.dtype)
    print("高度",height)
    print("宽度",width)
    print("通道数",channel)
    print("中心像素",center_pixel.tolist())
if __name__ == "__main__":
    main()