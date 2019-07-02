#ifndef KD_MEDIA_DEFINE_H
#define KD_MEDIA_DEFINE_H

#if defined(_WINDOWS) ||defined(WIN32)||defined(WIN64)
	#if !defined(_WINDOW)
		#define _WINDOW
	#endif
#endif

#ifndef _WINDOW
#define __stdcall
#endif

#ifdef _WINDOW
#include <winsock2.h>
#include <windows.h>
#elif	defined(_LINUX_)||defined(__LINUX__)
//#include <unistd.h>
#ifndef BOOLTYPE
#define BOOLTYPE
typedef int 				 BOOL;
#endif
#ifndef TRUE
#define  TRUE                (1)
#endif
#ifndef FALSE
#define  FALSE               (0)
#endif
#define MAX_PATH				256
#endif
//#include <time.h>

#if (defined(_WINDOW)) 
#ifdef _STATIC_LIB_
#ifdef KDMEDIA_EXPORTS
#define KDMEDIA_API __declspec(dllexport)
#else
#define KDMEDIA_API
#endif
#else
#ifdef KDMEDIA_EXPORTS
#define KDMEDIA_API __declspec(dllexport)
#else
#define KDMEDIA_API __declspec(dllimport)
#endif
#endif
#define CALLMETHOD //__stdcall
#define CALLBACK   __stdcall
#else
#define CALLMETHOD

#ifndef CALLBACK
#define CALLBACK
#endif

#define KDMEDIA_API
//#define KDMEDIA_API extern "C"
#endif

//vc6没有该头文件
#if _MSC_VER > 1200
#include <stdint.h>
#endif
#ifdef _LINUX_
#include <stdint.h>
#endif
#include "kdvdef.h"
#include "errorcode.h"

#define KD_MIN_WAVE_COEF -100
#define KD_MAX_WAVE_COEF 100

//Source buffer
#define KD_SOURCE_BUF_MAX	1024
#define KD_SOURCE_BUF_MIN	50

//Stream type
#define KD_STREAME_REALTIME 0
#define KD_STREAME_FILE	 1

#define MIN_WAVE_COEF					-100	//KD_AdjustWaveAudio使用
#define MAX_WAVE_COEF					100		//KD_AdjustWaveAudio使用
//Max display regions.
#define KD_MAX_DISPLAY_WND 16

#define KD_RUNINFO_VER 0x0001
#define KD_MAX_KDAP_DEVICENUM 3
#define TS_SYNC_RTP_ONLY 1// frame contains only RTP timestamp
#define TS_SYNC_NTP_UNIPLY 2//frame contains NTP timestamp , NTP calculated by uniplay

/* 清屏模式 */
typedef enum
{
	KD_CLEARWINDOW_MODE_FREEZON = 0,       /*图像冻结*/
	KD_CLEARWINDOW_MODE_COLORBAR,          /*显示七彩条*/
	KD_CLEARWINDOW_MODE_BLUESCREEN,	    /*蓝屏*/
	KD_CLEARWINDOW_MODE_BLACKSCREEN,       /*黑屏*/
	KD_CLEARWINDOW_MODE_NULL,       /*正常显示，不清屏*/
	KD_CLEARWINDOW_MODE_MAX
}EKDClearWindowMode;
typedef struct tagKDRECT
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} KDRECT;
#if	defined(__LINUX__)
typedef KDRECT 				 RECT;
#endif

typedef enum _KD_VIE_MODULES{
  KD_VIE_MODU_ADJ           = 0x00000001,  //图像基本调节
  KD_VIE_MODU_EHAN          = 0x00000002,  //局部增强模块
  KD_VIE_MODU_DEHAZE        = 0x00000004,  //去雾模块
  KD_VIE_MODU_DENOISE       = 0x00000008,  //去噪模块
  KD_VIE_MODU_SHARPEN       = 0x00000010,  //锐化模块
  KD_VIE_MODU_DEBLOCK       = 0x00000020,  //去块滤波模块
  KD_VIE_MODU_CRB           = 0x00000040,  //色彩平衡模块
  KD_VIE_MODU_LENS          = 0x00000080   //镜头畸变矫正模块
}KD_VIE_MODULES;

typedef enum _KD_RESETBUF_MODULES{
	KD_RESETBUF_AUDDEC_DAT           = 0x00000000,	//音频未解码缓存
	KD_RESETBUF_VIDDEC_DAT,							//视频未解码缓存
	KD_RESETBUF_AUDPLY_PCM,							//音频PCM缓存
	KD_RESETBUF_VIDDEC_YUV							//视频播放缓冲
}KD_RESETBUF_MODULES;


#define TARGET_NULL   1     //关闭输出
#define TARGET_PRINT  2     //输出到printf
#define TARGET_TELNET 4     //通过osp输出到telnet
#define TARGET_FILE   8      //通过osp输出到文件
#define TARGET_LOGCAT 16     //只有ANDROID下有效。

#define UNILOG_ERR     1
#define UNILOG_WARNING 2
#define UNILOG_INFO    4
#define UNILOG_DEBUG   8

typedef enum  __KD_FRAME_TYPE
{
	KD_FRAME_VIDEO = 0,
	KD_FRAME_AUDIO,
    KD_FRAME_DIRECT_VIDEO,
    KD_FRAME_MEDIA_CODEC_VIDEO,
	KD_FRAME_UNKNOWN = 255,
}KD_FRAME_TYPE;

typedef enum __KD_DATA_TYPE
{
	KD_FRAME_VIDEO_I = 0,
	KD_FRAME_VIDEO_P,
	KD_FRAME_VIDEO_B,
}KD_DATA_TYPE;


typedef enum __tEImageType
{
	KD_PicFormat_BMP32 = 0,		//BMP32类型
	KD_PicFormat_JPEG,				//JPEG类型
	KD_PicFormat_JPEG_70,			//70%质量的JPEG类型
	KD_PicFormat_JPEG_50,			//50%质量的JPEG类型
	KD_PicFormat_JPEG_30,			//30%质量的JPEG类型
	KD_PicFormat_JPEG_10,			//10%质量的JPEG类型
	KD_PicFormat_BMP24,            //BMP24类型
} EImageType;

typedef enum _tKDRenderType
{
	AudioRender_winmm = 0,
	AudioRender_Dsound,
	AudioRender_XAudio,
	AudioRender_oss,
	AudioRender_alsa,
	AudioRender_AudioCore,
	VideoRender_ddraw = 100,
	VideoRender_d3d,
	VideoRender_WOpenGl,
	VideoRender_X11,
	VideoRender_XOpenGl,
	VideoRender_AOpenGL,
	VideoRender_AppleGL,
	VideoRender_Quar2D,
	VideoRender_OpenGL_ES
}RenderType;

typedef enum	__PLAYKD_VIE_IMAGESTAB_MODE
{
	PLAYKD_IMAGESTAB_MODE_UPSCALE			= 0x00000001,
	PLAYKD_IMAGESTAB_MODE_BOXING			= 0x00000002,
}PLAYKD_VIE_IMAGESTAB_MODE;

typedef enum	__PLAYKD_VIE_IMAGEFRC_MODE
{
	PLAYKD_IMAGEFRC_MODE_CPU			= 0x00000001,
	PLAYKD_IMAGEFRC_MODE_GPU			= 0x00000002,
}PLAYKD_VIE_IMAGEFRC_MODE;

typedef struct
{
	int		denoiseLevel;			//去噪强度[0, 255] 强度上升
	int		usmAmount;				//锐化强度[0, 255] 
	int		ufrcVal;				//PLAYKD_VIE_IMAGEFRC_MODE,只实现PLAYKD_IMAGEFRC_MODE_GPU
	int		uimagestab;				//PLAYKD_VIE_IMAGESTAB_MODE
	//int		moduFlag;
	int		brightVal;				//亮度调节值，[-256, 255]	功能可以用PLAYKD_SetColor接口实现
	int		contrastVal;			//对比度调节值，[-256, 255]
	int		colorVal;				//饱和度调节值，[-256, 255]
	int		colorhue;				//色度调节值，[-256, 255]
	//int		usmRadius;				//锐化半径[1, 15]			暂时不提供
	//int		usmThreshold;			//锐化阈值[0, 255]
}KD_VIE_MODUCONFIG,*PKD_VIE_MODUCONFIG;

typedef enum	__PLAYKD_VIE_MODULES
{
	PLAYKD_VIE_MODU_DENOISE			= 0x00000001,  //去噪模块
	PLAYKD_VIE_MODU_FRC				= 0x00000002,  //倍帧模块
	PLAYKD_VIE_MODU_STAB			= 0x00000004,  //防抖模块,在bsw上不支持，部分硬件可以
	PLAYKD_VIE_MODU_SHARP			= 0x00000008,  //锐化模块
	PLAYKD_VIE_MODU_ADJ				= 0x00000010,  //亮度、对比度模块，未提供，使用PLAYKD_SetColor接口代替
}PLAYKD_VIE_MODULES;

typedef struct
{
	uint32_t				dwFilePos;		//指定帧在文件中的偏移位置
	uint32_t				dwFrameLen;	//帧长度
	uint32_t				dwFrameNum;	//帧序号
	uint32_t				dwFrameTime;	//帧时间
} KD_FRAME_POS,*PKD_FRAME_POS;

typedef enum	__FRAME_DATA_FORMAT
{
	FRAME_DATA_PCM = 0,
	FRAME_DATA_YV12 = 100,
	FRAME_DATA_RGB24,
	FRAME_DATA_RGBA32,
	FRAME_DATA_ABGR,
	FRAME_DATA_ARGB4444,
	FRAME_DATA_RGB565,
	FRAME_DATA_ARGB1555,
	FRAME_DATA_BGRA,
	FRAME_DATA_YUY2,
	FRAME_DATA_UYVY,
	FRAME_DATA_NV21,
	FRAME_DATA_NV12,
	FRAME_DATA_I420,
	FRAME_DATA_END
}FRAME_DATA_FORMAT;
#define FRAME_DATA_RGB	FRAME_DATA_RGB24

typedef enum PlaySpeed
{
	PlaySpeed_1_div_8 = 1,
	PlaySpeed_1_div_4,
	PlaySpeed_1_div_2,
	PlaySpeed_1,
	PlaySpeed_2,
	PlaySpeed_4,
	PlaySpeed_8,
	PlaySpeed_16
}emPlaySpeed;

enum BufferType
{
	KD_BUF_VIDEO_SRC =1,
	KD_BUF_AUDIO_SRC ,
	KD_BUF_VIDEO_RENDER,
	KD_BUF_AUDIO_RENDER
};

enum KdDecoderMode
{
	KD_DECMODE_NOTIMER = 0,		// 无节奏模式：组一帧解一帧，立即解码显示
	KD_DECMODE_STATTIMER,		// 统计节奏模式：通过统计接收数据的速率来自动设定节奏
	KD_DECMODE_SPEEDTIMER,		// 帧率节奏模式：根据设定的播放速度及回调的帧率来调整播放节奏；当缓存<200ms时，调节速度变慢
	KD_DECMODE_STAMPTIMER,		// 时间戳节奏模式：根据时间戳来调整播放节奏
	KD_DECMODE_IPCREALTIMER,	// 实时均衡模式：相当于1到2倍数的帧信息帧率速度播放，buf>2时开启2倍数
};
/* 旋转模式 KDVP_SetRotateAngle使用 */
typedef enum
{
    ROTATE_0        = 0,
    ROTATE_90       = 90,
    ROTATE_180      = 180,
    ROTATE_270      = 270
}ERotateMode;

enum KdRunTimeInfoModule
{
	KD_RUNTIMEINFO_DEFAULT = 0,		// 
	KD_RUNTIMEINFO_VIDEODEC = 2,	// 解码模块
	KD_RUNTIMEINFO_VIDEOPLAY = 3,	// 显示模块
};

/*解码器统计信息*/
typedef struct tagDecStatis
{
	uint8_t	  		byVideoFrameRate;	/*视频解码帧率*/
	uint16_t		wVideoBitRate;   	/*视频解码码率*/
	uint16_t		wAudioBitRate;   	/*语音解码码率*/
	uint32_t 		dwVideoRecvFrame;	/*收到的视频帧数*/
	uint32_t		dwAudioRecvFrame;	/*收到的语音帧数*/
	uint32_t 		dwVideoLoseFrame;	/*丢失的视频帧数*/
	uint32_t 		dwAudioLoseFrame;	/*丢失的语音帧数*/
    uint16_t		wVideoLoseRatio;	//视频丢失率,单位是% 	
	uint16_t		wAudioLoseRatio;	//音频丢失率,单位是% 	
	uint32_t 		dwPackError;		/*乱帧数*/ 
	uint32_t 		dwIndexLose;		/*序号丢帧*/
	uint32_t 		dwVideoSizeLose; 	/*大小丢帧*/
	uint32_t 		dwVideoFullLose; 	/*满丢帧*/	
	uint32_t 		dwAudioSizeLose; 	/*大小丢帧*/
	uint32_t 		dwAudioFullLose; 	/*满丢帧*/	
	uint32_t 		dwAvrVideoBitRate;   /*1分钟内视频解码平均码率*/
	uint32_t	  	dwAvrAudioBitRate;   /*1分钟内语音解码平均码率*/
	BOOL 			bVidCompellingIFrm;  /*视频强制请求I帧*/								  
	uint32_t		dwDecdWidth;		/*码流的宽*/
	uint32_t 		dwDecdHeight;   	/*码流的高*/
    int             dwCodecType;        /*解码类型*/
    int             eDrawMode;          /*绘图方式*/
    int             dwVerticalSync;     /*垂直同步模式*/
}TDecStatis; 

typedef struct tagRawData
{
	uint8_t			*pData;       //数据缓冲
    uint32_t 		dwPreBufSize;//pData缓冲前预留了多少空间，用于加
	uint32_t		byMediaEncode; //在kdvdef.h 内部定义
    uint32_t		dwDataSize;  //pData指向的实际缓冲大小缓冲大小
    uint32_t 		dwFrameID;   //帧标识，用于接收端
    uint32_t 		dwTimeStamp; //时间戳, 用于接收端
	uint32_t 		dwSSRC;      //同步源, 用于接收端
	uint64_t		dwRawTimeStamp;		//绝对时间戳（视频上面时间）
    union
    {
        struct{
			//下面一个属性跟之前的版本结构体（tagFrameHdr）尤其是网络组件提供的结构体有区别，大家要注意了
			uint8_t    			byKeyFrame;    //低2位频帧类型（I or P）KD_DATA_TYPE ,第8位为码流结束标志,为1时，内部将解码所有残留帧。
			uint8_t				byFrameRate; //发送帧率,用于接收端
			uint16_t			wVideoWidth;  //视频帧宽
			uint16_t			wVideoHeight; //视频帧宽
		}tVideoParam;
		struct{
			uint16_t			wChannel;		//音频通道
			uint16_t			wSample;	//音频采样率
			uint16_t			wSamplebit; //音频采样位数
		}tAudioParam;
    };
    /*
#define KeyFrame tVideoParam.byKeyFrame
#define FrameRate tVideoParam.byFrameRate
#define VideoWidth tVideoParam.wVideoWidth
#define VideoHeight tVideoParam.wVideoHeight
#define Channels	tAudioParam.wChannel
#define KDSamples	tAudioParam.wSample
#define	SampleBits	tAudioParam.wSamplebit
    */
	uint16_t		wReverse[4]; //wReverse[0]=0x00F0表示存在音频多路流，wReverse[1]的值为流ID，必须从1累加，目前最多支持2路音频流，比如wReverse[1]=1为流1，wReverse[1]=2为流2，
}KDTRawData,*PKDTRawData;


		//帧信息
typedef struct __tKDFrameInfo
{
	uint8_t*		pbyRawBuf;
	uint32_t		dwRawlen;
	uint32_t		dwMediaType;			//数据类型(KD_FRAME_TYPE)
	uint32_t		dwSubMediaType;			//KD_DATA_TYPE(视频有效)
	uint32_t		dwFrameIndex;			//帧序号
	uint64_t		dwNetTimeStamp;			//相对时间戳（RTP内部时间戳）
	uint64_t		dwMediaTimeStamp;		//绝对时间戳（视频上面时间）
	uint32_t		dwMediaEncode;			//在kdvdef.h 内部定义或者FRAME_DATA_FORMAT里面定义
	union FRAME
	{
		struct VideoFrame
		{
			uint16_t		dwFrameRate;
			uint16_t		dwWidth;     //画面宽
			uint16_t		dwHeight;    //画面高
			uint16_t		strike[3];
		}tVideo;

		struct AudioFrame
		{
			uint16_t		wChannels;   //声道数
			uint16_t		wBitWidth;   //位宽
			uint16_t		dwSampleRate; //采样率
		}tAudio;
	}u;
	uint16_t		wBitRate;			//比特率，单位为1028字节
	uint16_t		wReverse[3];   //采集侧，wReverse[0]保存音频采集DB值，wReverse[1]保存采集失败时的错误码,wReverse[2]参考源DB值;重编码回调时，wReverse[0]=0x00F0表示存在音频多路流，wReverse[1]的值为流ID，必须从1累加，目前最多支持2路音频流，比如wReverse[1]=1为流1，wReverse[1]=2为流2，
}KDFrameInfo,*PKDFrameInfo;

typedef struct
{
	int            iModule;				//当前运行模块，暂定2为解码模块，3为显示模块
	int            nStrVersion;     	//数据返回的结构体版本，主要是为了兼容性,第一个版本定义成0x0001
	int            nFrameID;     		//帧号
	int            nTimeStamp;     		//时间戳
	int            nErrorCode;			//错误码,参考errorcode.h
	unsigned char  reserved[12];		//保留字节
}KDRunTimeInfo;

typedef struct
{
	KDRECT	tOsdParam;
	int		nOsdBuf;        //OSD图片长度
	char	*pOsdBuf;       //OSD图片buf地址
} KDOsdParam;

typedef enum	__PLAYKD_OSD_MODES
{
    PLAYKD_OSD_MODE_RENDER			= 0x00000001,  //OSD绘制
    PLAYKD_OSD_MODE_SNAP			= 0x00000002,  //OSD抓拍
    PLAYKD_OSD_MODE_RECORD			= 0x00000004,  //OSD录像
}PLAYKD_OSD_MODES;

typedef struct
{
    int                 wStructSize;	   //sizeof(KD_OSD_MODECONFIG)
    int                 eOsdMode;		   //选择添加OSD模式,通过PLAYKD_OSD_MODES选择，可以同时开启不同模式
    BOOL                bOsd;               //是否开启OSD;默认关闭，TRUE开启
    unsigned int        uiARGB;             //字体颜色和透明度;[0xFFFFFFFF]最高2位是Alpha通道值，剩下是RGB
    unsigned int        uiFontSize;         //字体大小[16,2000]
    unsigned int        uiFontRotate;       //旋转角度[0,360)
    unsigned int        uiLayerMode;        //1：平铺，类似“井”字形均匀平铺，不会拉伸文字。文字太长，将被截断。
    char                strText[64];        //文字内容,最长60
    char*               pstrFontPath;       //字库路径，指向字符串不能超过512长度，建议填写；如果为NULL，内部自己或者默认字库，但可能导致一些字符显示错误。
}KD_OSD_MODECONFIG,*PKD_OSD_MODECONFIG;

typedef struct 
{
    uint32_t dwStreamRecvBitRate;
    uint32_t dwExpectedDelay;
    uint32_t dwIncompleteFrames;
    uint32_t dwIncompleteFirstTS;
    uint32_t dwIncompleteLastTS;
    uint32_t dwAverageCompleteTimeMs;
}TMrtcRecvState;

#define KD_IS_DECODEOK(flag)		(flag&0x01)
#define KD_IS_UPDATEDATA(flag)		(flag&0x02)
#define KD_SET_DECODEOK_FLAG(flag)	(flag|0x01)
#define KD_SET_UPDATEDATA_FLAG(flag)	(flag|0x02)
//回调参数里面的 dwMediaEncode应该在在FRAME_DATA_FORMAT定义
typedef void (CALLBACK *PDECCALLBACK)(int32_t dwPort,PKDFrameInfo pFrame, void* pcontext);
//pcontext1 跟帧信息关联，返回PLAYKD_InputDataEx设置下去的上下文
//pcontext2：返回PLAYKD_SetYuvCallbackEx设置下去的上下文
typedef void (CALLBACK *PDECCALLBACKEXT)(int32_t dwPort,PKDFrameInfo pFrame, int flag,void* pStreamContext,void* pFrameContext);
//通过返回pFrame内部dwRawlen来判断上层应该分配多大的内存，返回出来的pbyRawBuf必须为null。
//另外需要填充宽跟高以及yuv类型
typedef void * (CALLBACK *PALLOCATEMMCALLBACK)(int32_t dwPort,PKDFrameInfo pFrame, void* pStrmContext, void*pFrameContext);
//回调参数里面的 dwMediaEncode应该在在kdvdef.h定义
//typedef void (CALLBACK *PDEMUCALLBACK)(int32_t dwPort,PKDFrameInfo pFrame, void* pcontext);

/*
pAddDrawDC 是一个结构体
win下：
typedef struct
{
    void *hdc;
    void *hwnd;
    void *rendertype;//渲染类型，D3D直接显示，D3D显示，DDRAW显示，DRAWDIB显示。
}DrawDC;

*/
typedef void (CALLBACK *PDRAWCALLBACK)(void* pAddDrawDC, void* pcontext);
//	音频采集数据回调指针,其参数含义如下:
//			pDataBuffer,回调数据指针 
//			DataLength,回调数据长度
//          pFrame.wReverse[0],代表音量DB值（-100，0）映射到(0，100),100音量最大。
//			pUserData,用户数据
typedef void (CALLBACK *PCAPTIONCALLBACK)(PKDFrameInfo pFrame, void* pUserData);
typedef int (CALLBACK *PFILEENDCALLBACK)(int32_t dwPort,void* pcontext);
//PCM数据回调函数
// ptVideoData：yuv数据结构
// pParam：使用者设置的回调参数数据
//回调参数ptVideoData里面的 dwMediaEncode应该在在FRAME_DATA_FORMAT定义
//typedef void 	(CALLBACK  	*CBF_YUVData)( PKDFrameInfo ptVideoData, void* pParam);

//PCM数据回调函数
// ptAudioData：PCM数据结构
// pParam：使用者设置的回调参数数据
//回调参数ptAudioData里面的 dwMediaEncode应该在在FRAME_DATA_FORMAT定义
//typedef void 	(CALLBACK 	*CBF_PCMData)( PKDFrameInfo ptAudioData, void* pParam);

//未解码数据回调
//回调参数pFrameHdr里面的 dwMediaEncode应该在在kdvdef.h定义
typedef void 	(CALLBACK 	*PRAWDATACALLBACK)( PKDTRawData pFrameHdr, void* pParam );

//解码播放作图回调函数
//typedef void 	(CALLBACK 	*CBF_DRAW)(void* hDC, void* pParam);

//分辨率改变回调函数
typedef void	(CALLBACK 	*PENCCALLBACK)(void* nUser);

//内部源数据回调通知函数
typedef void	(CALLBACK 	*PSOURCECALLBACK)(int32_t dwPort,void* nUser);

//aec音频参考数据回调
typedef int (CALLBACK *PFAECCB)(PKDFrameInfo pFrame,void *pUserData);

//
typedef void (CALLBACK *PRunTimeInfoCBFun)(int nPort, KDRunTimeInfo* pRunTimeInfo, void *nUser);

typedef void (CALLBACK *PFAUDIOCLOCKCB)(uint64_t u64NowTime, void * pUserData);

typedef void (CALLBACK *PFAUDIOEOSCB)(void * pUserData);
#endif
