# 基于机器视觉的试剂盒瑕疵检测系统说明文档

## 1. 项目概述

本项目是一个面向试剂盒生产线的机器视觉瑕疵检测软件，主要用于在自动化设备运行过程中完成试剂盒关键部件的图像采集、缺陷识别、结果回传、数据记录和异常追溯。

系统基于 Qt/C++ 开发，使用 OpenCV 完成图像处理，使用海康机器视觉相机 SDK 完成 GigE 相机控制，通过 Modbus TCP 与 PLC 通信，并将生产统计数据写入 MySQL 数据库。软件启动后会自动初始化 PLC、相机、ROI、检测参数和本地存图目录，在生产过程中根据 PLC 指令执行拍照和识别任务。

## 2. 系统功能

- 三路相机图像采集：分别对应不同检测工位。
- 试剂盒瑕疵识别：包含试纸、底壳、上盖等检测逻辑。
- PLC 联动控制：读取 PLC 拍照/识别指令，并将识别结果写回 PLC。
- ROI 配置：支持相机 ROI 和检测子区域配置。
- 检测参数配置：通过 `config.ini` 配置相机参数、PLC 地址、瑕疵阈值和调试选项。
- UI 实时显示：显示相机图像、内外侧检测区域、系统状态、日志和生产统计。
- 缺陷图像追溯：NG 图像保存到本地，并在界面中保留最近异常记录。
- 生产数据记录：将统计数据写入 MySQL，启动时读取当天统计。
- 调试模式：支持延时、强制结果、调试存图等功能，便于现场调试和算法验证。

## 3. 技术栈与依赖

| 类型 | 使用内容 |
| --- | --- |
| 开发语言 | C++ |
| 界面框架 | Qt Widgets |
| 图像处理 | OpenCV |
| 相机 SDK | 海康 MVS / `MvCameraControl.lib` |
| PLC 通信 | Modbus TCP / `libmodbus` |
| 数据库 | MySQL |
| 工程环境 | Visual Studio + Qt VS Tools |
| 目标平台 | Windows x64 |

工程文件 `VI.vcxproj` 中配置的主要依赖包括：

- Qt 模块：`charts`、`core`、`gui`、`sql`、`widgets`
- OpenCV：`opencv_world455.lib` / `opencv_world455d.lib`
- MySQL：`libmysql.lib`
- 海康相机 SDK：`MvCameraControl.lib`
- Modbus：`modbus.lib`

## 4. 项目目录结构

```text
VI/
├── VI.cpp / VI.h                         主窗口、UI 逻辑、状态显示、数据库写入
├── VI_Task.cpp / VI_Task.h               任务调度，串联 PLC、相机和识别流程
├── VI_Camera.cpp / VI_Camera.h           相机连接、参数设置、采图、关闭
├── VI_Recog.cpp / VI_Recog.h             OpenCV 瑕疵识别算法
├── VI_Modbus.cpp / VI_Modbus.h           Modbus TCP 与 PLC 通信
├── config.ini                            相机、PLC、瑕疵阈值和调试配置
├── StateCode.h                           系统状态码、错误码、识别结果码
├── CameraParameters.h                    相机参数结构
├── TaskData.h                            单相机任务数据结构
├── RecogData.h                           识别过程图像与错误信息结构
├── INI_Config.cpp / INI_Config.h         INI 配置读写工具
├── PaintRectRoi.* / RoiPage.*            ROI 绘制和配置相关界面
├── Setting.*                             系统参数设置界面
├── HistoryDataRecord.*                   历史数据查询界面
├── show_log.*                            登录与日志相关界面
├── MarkTheError.*                        缺陷标记窗口
├── *.ui                                  Qt Designer 界面文件
├── icon/                                 软件图标资源
└── x64/                                  Debug/Release 编译产物
```

## 5. 总体架构

系统可以分为五个核心层次：

| 层次 | 模块 | 说明 |
| --- | --- | --- |
| UI 层 | `VI`、`Setting`、`RoiPage`、`HistoryDataRecord` | 负责界面显示、用户操作、参数窗口、日志和统计信息 |
| 任务调度层 | `VI_Task` | 根据 PLC 指令调度相机拍照、图像识别、结果回传和数据打包 |
| 采集层 | `VI_Camera` | 连接三台 GigE 相机，设置曝光、增益、ROI、包大小等参数 |
| 算法层 | `VI_Recog` | 按相机类型执行试纸、底壳、上盖检测 |
| 通信与数据层 | `VI_Modbus`、MySQL、本地文件 | 负责 PLC 通信、数据库记录和图像存储 |

核心数据流如下：

```text
PLC 指令
  ↓
VI_Modbus 读取 D901/D902/D903
  ↓
VI_Task 判断任务类型
  ↓
相机拍照 或 图像识别
  ↓
VI_Recog 输出识别结果
  ↓
结果写回 PLC
  ↓
UI 显示 / 本地存图 / MySQL 记录 / 异常追溯
```

## 6. 主程序启动流程

程序入口位于 `main.cpp`，创建并显示主窗口 `VI`。`VI` 构造函数中完成以下初始化：

1. 注册跨线程信号类型：`cv::Mat`、`string`。
2. 初始化状态栏、统计栏和子窗口。
3. 创建 `VI_Task` 工作线程和系统状态检测线程。
4. 初始化 MySQL 连接。
5. 建立 UI 与任务线程之间的 Qt 信号槽连接。
6. 创建当天图片目录，并清理过期图片。
7. 读取当天历史统计数据。
8. 自动启动视觉检测系统。

启动检测时调用 `StartVISystem(true)`，随后发出 `Init_VI_signals()`，由 `VI_Task::Init_VI()` 完成 PLC 和相机初始化。

## 7. PLC 通信流程

PLC 通信由 `VI_Modbus` 实现，使用 Modbus TCP。

### 7.1 PLC 连接

PLC 的 IP 和端口从 `config.ini` 中读取：

```ini
[PLC]
IP=192.168.1.87
Port_Number=502
```

在调试模式下，会使用 `[DeveloperOptions]` 中的 IP 和端口。

### 7.2 指令寄存器

系统默认从 `D901` 起读取三个寄存器，对应三台相机：

| 相机 | 寄存器 |
| --- | --- |
| 相机 1 | D901 |
| 相机 2 | D902 |
| 相机 3 | D903 |

代码中定义的 PLC 指令包括：

| 指令值 | 宏定义 | 含义 |
| --- | --- | --- |
| `801` | `TakePhotos` | PLC 要求相机拍照 |
| `802` | `ProcessImage` | PLC 要求处理图像 |
| `901` | `PhotoIsOver` | IPC 通知 PLC 拍照完成 |

`VI_Task::VI()` 会循环读取三个寄存器：

- 读到 `801`：启动对应相机的拍照任务。
- 读到 `802`：启动对应相机的图像识别任务。
- 其他值：忽略或作为状态处理。

### 7.3 结果回写

识别完成后，`VI_Task::TaskRecog()` 将识别结果通过 `Write_PLC_Register()` 写回对应寄存器。这样 PLC 可以根据结果控制后续剔除、报警或放行逻辑。

## 8. 相机采集流程

相机相关逻辑位于 `VI_Camera`。

### 8.1 相机初始化

`VI_Task::Init_VI()` 初始化 PLC 成功后，会并行打开三台相机：

- `OpenCamera(1, ...)`
- `OpenCamera(2, ...)`
- `OpenCamera(3, ...)`

每台相机通过 `VI_Camera::build_camera_connect()` 完成：

1. 从 `config.ini` 读取相机 IP 和参数。
2. 创建 GigE 相机句柄。
3. 打开设备。
4. 设置相机参数。
5. 开始取流。
6. 返回 `VI_CAMERA_READY` 状态。

### 8.2 相机参数

相机参数主要位于 `[Camera]` 配置段，包括：

- 相机 IP：`IP1`、`IP2`、`IP3`
- 曝光时间：`ExposureTime1`、`ExposureTime2`、`ExposureTime3`
- 增益：`Gain1`、`Gain2`、`Gain3`
- 帧率：`AcqusitionFrameRate1` 等
- 黑电平：`BlackLevel` 和 `BlackLevelEnable`
- GigE 包大小与心跳：`GevPacketSize`、`GevHeartbeatTimeout`
- 下采样比例：`Decimation_Camera1` 等
- ROI 参数：`Camera*_Roi_*`

### 8.3 图像采集

采图函数为 `VI_Camera::grab_image()`，主要步骤是：

1. 调用 `MV_CC_GetImageBuffer()` 从相机获取图像缓存。
2. 将相机缓存封装为 OpenCV `cv::Mat`。
3. 释放 SDK 图像缓存。
4. 将图像 `clone()` 到任务数据中。
5. 返回采图状态码。

采图完成后，系统向 PLC 写入 `PhotoIsOver`，表示本轮拍照完成。

## 9. 图像识别算法

识别核心位于 `VI_Recog`。主入口为：

```cpp
void VI_Recog::VI_Image_Recog(cv::Mat image, int& result, int camera_type)
```

系统根据相机编号选择不同检测函数：

| 相机 | 检测函数 | 检测对象 |
| --- | --- | --- |
| 相机 1 | `paper_detect()` | 试纸/纸条相关检测 |
| 相机 2 | `bottom_detect()` | 底壳检测 |
| 相机 3 | `lid_detect()` | 上盖检测 |

### 9.1 ROI 读取

`GetROI()` 从 `config.ini` 读取相机联合 ROI 和两个子 ROI，并将联合 ROI 坐标转换为局部子区域坐标。

检测阈值同样从 `[Flaw]` 段读取，例如：

- 试纸宽度上下限
- 试纸角度上下限
- 污点长度、数量、灰度明显程度
- 盒内纸条 Y 坐标范围
- 缝隙面积阈值

### 9.2 相机 1：试纸检测

相机 1 执行 `paper_detect()`，主要逻辑包括：

1. 通过 ROI 切出待检测区域。
2. 对试纸区域进行阈值分割和形态学处理。
3. 查找轮廓并筛选最大区域。
4. 通过最小外接矩形判断试纸位置、宽度、角度等。
5. 检测纸槽是否有纸、是否反向、是否存在明显污点。
6. 输出相机 1 对应结果码。

常见异常包括：

- 未检测到试纸。
- 纸槽未清空或纸槽异常。
- 试纸放反。
- 试纸宽度、角度、位置超限。
- 污点超过阈值。

### 9.3 相机 2：底壳检测

相机 2 执行 `bottom_detect()`，会分别处理内侧和外侧图像区域。核心函数为 `bottom_detect_cutImage()`。

主要判断内容包括：

- 是否能找到底壳主体轮廓。
- 底壳区域面积是否在阈值范围内。
- 试纸与底壳位置关系是否符合要求。
- 缝隙、错位、倾斜等结构异常。

相机 2 的两个区域结果会合成为一个整数结果，低位表示 A 区域，高位表示 B 区域。

### 9.4 相机 3：上盖检测

相机 3 执行 `lid_detect()`，核心函数为 `lid_detect_cutImage()`。

主要判断内容包括：

- 是否能找到上盖主体。
- 上盖面积是否正常。
- 上盖与试纸/底壳区域是否匹配。
- 是否存在缺失、偏移或异常遮挡。

相机 3 同样将 A、B 两个检测区域组合为最终结果。

## 10. 识别结果码

结果码定义位于 `StateCode.h`。

| 结果码 | 宏定义 | 含义 |
| --- | --- | --- |
| `0` | `VI_RECOG_AERR_BERR` | A、B 区域均异常 |
| `1` | `VI_RECOG_AOK_BERR` | A 区域 OK，B 区域异常 |
| `2` | `VI_RECOG_AERR_BOK` | A 区域异常，B 区域 OK |
| `3` | `VI_RECOG_AOK_BOK` | A、B 区域均 OK |
| `10` | `VI_RECOG_PAPERUPSIDEDOWN` | 试纸放反 |
| `12` | `VI_RECOG_PAPERSLOTUNCLEAR` | 纸槽未清空或异常 |

其中相机 1 使用的结果逻辑与相机 2、3 略有不同：代码中相机 1 的 OK 结果为 `1`，相机 2、3 的双区域 OK 结果为 `3`。

## 11. 数据保存与追溯

### 11.1 图片保存目录

程序启动时，`VI::CreateAndDeleteImageFiles()` 会在 `D:\VI\figs` 下创建按日期组织的目录：

```text
D:\VI\figs\YYYYMMDD\
├── camera1\
│   ├── Success\
│   └── False\
├── camera2\
│   ├── Success\
│   └── False\
├── camera3\
│   ├── Success\
│   └── False\
└── debug\
```

保存规则：

- 正常模式下，OK 图像进入 `Success`，NG 图像进入 `False`。
- 调试模式下，图像保存到 `debug`。
- OK 图像可通过 `SaveFrequency` 控制保存频率。
- NG 图像会携带识别结果、时间和生产序号。
- 过期图片会按配置项自动清理。

### 11.2 MySQL 数据库

程序启动时连接本地 MySQL：

```cpp
mysql_real_connect(&DatabaseVI, "127.0.0.1", "root", "123456", "VI", 3306, NULL, 0);
```

数据库主要用于记录和读取生产统计数据。启动时 `GetTodayStatistics()` 查询当天 `statistics` 表，并恢复 UI 统计栏。

> 注意：当前数据库账号、密码和库名硬编码在 `VI.cpp` 中，部署时需要保证本机 MySQL 服务、数据库和表结构已经准备好。

## 12. 配置文件说明

配置文件为 `config.ini`。

### 12.1 `[Camera]`

用于配置三台相机的 IP、曝光、增益、帧率、ROI、GigE 网络参数等。

常用字段：

| 字段 | 说明 |
| --- | --- |
| `IP1`、`IP2`、`IP3` | 三台相机 IP |
| `ExposureTime1` 等 | 曝光时间 |
| `Gain1` 等 | 增益 |
| `AcqusitionFrameRate1` 等 | 采集帧率 |
| `Camera*_Roi_OffsetX_*` | ROI X 偏移 |
| `Camera*_Roi_OffsetY_*` | ROI Y 偏移 |
| `Camera*_Roi_Width_*` | ROI 宽度 |
| `Camera*_Roi_Height_*` | ROI 高度 |
| `Decimation_Camera*` | 下采样比例 |

### 12.2 `[PLC]`

用于配置 PLC 的 Modbus TCP 地址：

| 字段 | 说明 |
| --- | --- |
| `IP` | PLC IP |
| `Port_Number` | PLC 端口，通常为 `502` |

### 12.3 `[Flaw]`

用于配置瑕疵检测阈值：

| 字段 | 说明 |
| --- | --- |
| `PaperWidthUpperLimit` | 试纸宽度上限 |
| `PaperWidthLowerLimit` | 试纸宽度下限 |
| `PaperAngleUpperLimit` | 试纸正向角度阈值 |
| `PaperAngleLowerLimit` | 试纸反向角度阈值 |
| `PaperStainLength` | 试纸污点长度阈值 |
| `PaperStainNumber` | 试纸污点数量阈值 |
| `PaperStainColorDegree` | 污点灰度明显程度阈值 |
| `BoxPaperYPositionUpperLimit` | 盒内纸条 Y 位置上限 |
| `BoxPaperYPositionLowerLimit` | 盒内纸条 Y 位置下限 |
| `BoxGapAera` | 缝隙面积阈值 |

### 12.4 `[Other]`

用于配置运行显示和保存策略：

| 字段 | 说明 |
| --- | --- |
| `CameraInfo` | 是否显示相机状态信息 |
| `ResultInfo` | 是否显示识别结果信息 |
| `SaveFrequency` | OK 图片保存频率 |
| `RetentionDay` | 数据或图片保留天数 |

### 12.5 `[DeveloperOptions]`

用于调试模式：

| 字段 | 说明 |
| --- | --- |
| `Camera1Result`、`Camera2Result`、`Camera3Result` | 强制指定相机识别结果，`-` 表示不覆盖 |
| `DelayTime` | 调试延时 |
| `SaveImage` | 调试模式是否保存图片 |
| `SaveData` | 调试模式是否保存数据 |
| `ExpiredImages` | 过期图片清理天数 |
| `IP`、`Port_Number` | 调试模式 PLC/Modbus 地址 |

## 13. UI 与操作窗口

系统使用 Qt Designer 构建界面，主要窗口包括：

| 窗口 | 文件 | 功能 |
| --- | --- | --- |
| 主窗口 | `VI.ui`、`VI.cpp` | 实时显示图像、日志、统计和状态 |
| 参数设置 | `Setting.ui` | 修改相机、PLC、识别阈值等参数 |
| ROI 设置 | `RoiPage.ui`、`MainRoiPage.ui` | 绘制和保存 ROI |
| 登录窗口 | `login.ui`、`show_log.cpp` | 权限登录，登录后开放设置和 ROI 功能 |
| 历史数据 | `HistoryDataRecord.ui` | 查询历史生产数据 |
| 缺陷标记 | `MarkTheError.ui` | 显示或标记异常图像 |
| 相机配置 | `show_camera_config_window.ui` | 查看或配置相机参数 |

## 14. 异常与状态码

系统状态码统一定义在 `StateCode.h`。

### 14.1 PLC 错误

| 状态码 | 含义 |
| --- | --- |
| `1000` | 创建 Modbus TCP 失败 |
| `1001` | 设置 Modbus debug 失败 |
| `1002` | PLC 连接失败 |
| `1003` | 写寄存器失败 |
| `1004` | 读寄存器失败 |
| `1005` | PLC 通信内存申请失败 |
| `1007` | 未知 PLC 状态 |

### 14.2 相机错误

| 状态码 | 含义 |
| --- | --- |
| `1010` | 创建相机句柄失败 |
| `1012` | 打开相机失败 |
| `1014` | 开始取流失败 |
| `1015` | 停止取流失败 |
| `1017` | 图像为空 |
| `1018` | 相机 ROI 校准失败 |

### 14.3 系统状态

| 状态码 | 含义 |
| --- | --- |
| `2020` | PLC 已连接 |
| `2021` | PLC 已断开 |
| `2030` | 相机准备完成 |
| `2031` | 相机拍照完成 |
| `2051` | 系统运行中 |
| `2052` | 系统停止 |
| `2053` | 系统异常 |

## 15. 编译与运行

### 15.1 编译前准备

1. 安装 Visual Studio，并启用 C++ 桌面开发环境。
2. 安装 Qt 和 Qt VS Tools。
3. 安装 OpenCV，并保证工程中的 include/lib 路径正确。
4. 安装海康 MVS SDK，并保证 `MvCameraControl.lib` 可链接。
5. 准备 `libmodbus` 源码和库文件。
6. 安装 MySQL 5.7 或兼容版本，并准备 `VI` 数据库。
7. 确认 `config.ini` 中的相机 IP、PLC IP 和 ROI 参数与现场一致。

### 15.2 工程配置

当前工程中的典型路径包括：

```text
D:\opencv\build\include
D:\opencv\build\x64\vc15\lib
D:\MVS\Development\Includes
D:\MVS\Development\Libraries\win64
D:\VI\libmodbus-master\src
C:\Program Files\MySQL\MySQL Server 5.7\include
C:\Program Files\MySQL\MySQL Server 5.7\lib
```

如果本机路径不同，需要在 Visual Studio 中修改：

- C/C++ 附加包含目录
- 链接器附加库目录
- 链接器附加依赖项

### 15.3 运行步骤

1. 打开 `VI.vcxproj`。
2. 选择 x64 Debug 或 x64 Release。
3. 确认 `config.ini` 位于程序工作目录。
4. 启动 MySQL 服务。
5. 确认 PLC 和三台相机网络可达。
6. 编译并运行程序。
7. 程序启动后会自动初始化 PLC 和相机，并进入检测流程。

## 16. 典型检测流程

```text
1. PLC 将 D901/D902/D903 写为 801
2. IPC 读取指令并调用对应相机拍照
3. IPC 将对应寄存器写为 901，通知 PLC 拍照结束
4. PLC 将寄存器写为 802
5. IPC 调用 VI_Recog 执行图像识别
6. IPC 将识别结果写回 PLC
7. UI 更新检测图像和统计结果
8. 系统保存 NG 图像，必要时保存 OK 图像
9. 系统将生产统计数据写入 MySQL
```

## 17. 调试建议

- 如果无法连接 PLC，先检查 `config.ini` 中 `[PLC]` 的 IP 和端口，再检查网卡 IP 是否与 PLC 同网段。
- 如果相机打开失败，检查 `[Camera]` 中的 IP、海康 MVS 客户端是否可识别相机，以及相机是否被其他程序占用。
- 如果图像为空，检查曝光、触发方式、取流状态和相机 ROI 是否设置正确。
- 如果识别结果异常，优先检查 ROI 是否偏移，再调整 `[Flaw]` 中的阈值。
- 如果 UI 不显示图像，检查 `VI_Task` 是否发出了 `image_msg` 信号，以及图像是否为空。
- 如果数据库无记录，检查 MySQL 服务、库名 `VI`、账号密码和表结构。
- 调试模式下可以使用 `Camera*Result` 强制指定输出结果，用于验证 PLC 后续动作。

## 18. 后续优化方向

- 将 MySQL 账号、密码和数据库名称移动到配置文件中，避免硬编码。
- 为 PLC 寄存器地址提供配置项，便于不同产线复用。
- 对相机线程和任务线程增加更严格的生命周期管理，减少线程对象泄漏风险。
- 对图像算法增加单元测试或离线批量验证工具。
- 增加日志文件输出，便于生产现场问题追踪。
- 对配置文件增加合法性校验，避免缺失字段导致程序异常。
- 对保存路径 `D:\VI\figs` 提供配置项，提高部署灵活性。

