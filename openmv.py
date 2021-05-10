import sensor, image, time  # 导入必要的模块
from pyb import UART    # 串口
from pyb import LED     # LED灯库


black_threshold = (1, 55)       # 小球灰度阈值
uart = UART(3, 115200)          # 初始化串口 波特率115200和控制器通信
windowROI =(40, 1, 239, 239)    # 图像窗口大小变量
findROI = (1, 1, 245, 238)
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)  # 设置像素格式为灰度图
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing(windowROI)         # 设置窗口大小
sensor.skip_frames(time = 500)
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(False) # 关闭白平衡
clock = time.clock()            # 实例化时钟对象
ir_led = LED(4)                 # 实例化补光灯
# ir_led.on()

while 1:
    clock.tick()                # 更新时钟
    img = sensor.snapshot().lens_corr(1.4)     # 获取一帧图像 并进行畸形矫正
    matchBlobs = None   # 存储符合要求的色块信息
    blobs = img.find_blobs([black_threshold], roi=findROI, x_stride=5, y_stride=5, pixels_threshold = 30)    # 找色块
    if blobs:       # 找到色块
        for blobsItems in blobs:    # 遍历色块
            if blobsItems.pixels() >= 60 and blobsItems.w() > 10 and blobsItems.h() > 10 :     # 筛选大于150像素的色块
                #if blobsItems.pixels() <= 200 and blobsItems.w() < 25 and blobsItems.h() < 25 :     # 筛选小于400像素的色块
                if blobsItems.pixels() <= 1200 and blobsItems.w() < 50 and blobsItems.h() < 50 :     # 筛选小于400像素的色块
                    img.draw_cross(blobsItems[5], blobsItems[6])       # 画十字
                    img.draw_rectangle(blobsItems.rect())    # 画矩形
                    matchBlobs = blobsItems
        if matchBlobs:
            output_str = bytearray([0xff,0xfe,int(matchBlobs[5]),int(matchBlobs[6])])   # 打印坐标
            print('Find:', matchBlobs[5], matchBlobs[6])#, matchBlobs)
        else:
            output_str = bytearray([0xff,0xfe,0xfd,0xfd])   # 打印坐标
            print('NoFind')
    else:
        output_str = bytearray([0xff,0xfe,0xfd,0xfd])       # 打印坐标
        print('NoFind')
    uart.write(output_str)
    # print(clock.fps())        # 打印显示 FPS
