#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_  1

//公共错误码
#define  KDPLAY_NOERROR							0	//no error
#define	 KDPLAY_PARA_OVER						1	//input parameter is invalid;
#define  KDPLAY_ORDER_ERROR						2	//The order of the function to be called is error.
#define	 KDPLAY_TIMER_ERROR						3	//Create multimedia clock failed;
#define	 KDPLAY_ALLOC_MEMORY_ERROR				6	//Allocate memory failed.
#define  KDPLAY_BUF_OVER						11	//buffer is overflow

//unidecode模块错误码

#define  KDPLAY_DEC_VIDEO_ERROR					4	//Decode video data failed.
#define  KDPLAY_DEC_AUDIO_ERROR					5	//Decode audio data failed.

#define	 KDPLAY_SET_VOLUME_ERROR				7	//Set volume failed
#define  KDPLAY_CREATE_OBJ_ERROR				8	//Create thread or event failed
#define  KDPLAY_CREATE_DDRAW_ERROR				9	//Create DirectDraw object failed.
#define  KDPLAY_CREATE_OFFSCREEN_ERROR			10	//failed when creating off-screen surface.

#define  KDPLAY_CREATE_SOUND_ERROR				12	//failed when creating audio device.	
#define  KDPLAY_OPEN_FILE_ERROR					13	//Open the file failed.
#define  KDPLAY_SUPPORT_FILE_ONLY				14	//The function only support play file.
#define  KDPLAY_SUPPORT_STREAM_ONLY				15	//The function only support play stream.
#define  KDPLAY_SYS_NOT_SUPPORT					16	//System not support.
#define  KDPLAY_FILEHEADER_UNKNOWN				17	//No file header.
#define  KDPLAY_VERSION_INCORRECT				18	//The version of decoder and encoder is not adapted.  
#define  KDPLAY_INIT_DECODER_ERROR				19	//Initialize decoder failed.
#define  KDPLAY_CHECK_FILE_ERROR				20	//The file data is unknown.
#define  KDPLAY_INIT_TIMER_ERROR				21	//Initialize multimedia clock failed.
#define	 KDPLAY_BLT_ERROR						22	//Blt failed.
#define  KDPLAY_UPDATE_ERROR					23	//Update failed.
#define  KDPLAY_OPEN_FILE_ERROR_MULTI			24   //openfile error, streamtype is multi
#define  KDPLAY_OPEN_FILE_ERROR_VIDEO			25   //openfile error, streamtype is video
#define  KDPLAY_JPEG_COMPRESS_ERROR				26   //JPEG compress error
#define  KDPLAY_EXTRACT_NOT_SUPPORT				27	//Don't support the version of this file.
#define  KDPLAY_EXTRACT_DATA_ERROR				28	//extract video data failed.
#define  KDPLAY_SECRET_KEY_ERROR				29	//Secret key is error //add 20071218
#define  KDPLAY_DECODE_KEYFRAME_ERROR			30   
#define  KDPLAY_NEED_MORE_DATA					31   
#define  KDPLAY_INVALID_PORT					32	
#define  KDPLAY_NOT_FIND						33
#define	 KDPLAY_OPERTION_NOTALLOWED				34	 //this operation not support
#define	 KDPLAY_FILE_WRITE_ERR					35	 
#define	 KDPLAY_PARA_POINT_NULL					36	 
#define	 KDPLAY_NO_DECODE						37	 
#define	 KDPLAY_NO_STREAM						38 
#define	 KDPLAY_ALREADY_EXIST					39
#define  KDPLAY_BUF_EMPTY                       40
#define  KDPLAY_WAITFOR_KEYFRAME                41
#define  KDPLAY_DEVICE_LOST		                42
#define  KDPLAY_SURFACE_LOST		            43
#define  KDPLAY_LOADDLL_FAIL		            44
#define	 KDPLAY_OUTOF_VIDEO_MEMORY				45	//d3d软解下需要释放内存资源才能恢复
#define	 KDPLAY_WINDOW_SIZE_ERROR				46
#define	 KDPLAY_ERROR_MEDIA_TYPE				47
#define  KDPLAY_LOCK_ERR                        48
#define  KDPLAY_INVALID_DEVICE                  49
#define  KDPLAY_UNDEFINED_BEHAVIOR              50
#define  KDPLAY_DEVICE_FAILED                   51
#define  KDPLAY_DEVICE_RESET                    52
#define  KDPLAY_ERR_FRAME_TIME                  53
#define  KDPLAY_ERR_VERSION_CMP                 54
#define  KDPLAY_ERR_FONT_PATH                   61
#define  KDPLAY_FAIL_UNKNOWN					99   //Fail, but the reason is unknown;	



#define  KDPLAY_ERR_RETURN						0xFFFFFFFF

//视频解码模块错误码
#define  KDVD_OK                  KDPLAY_NOERROR
#define  KDVD_GENERAL_ERROR       1024
#define  KDVD_ERR_PORT            1025
#define  KDVD_ERR_PARAM           KDPLAY_PARA_OVER
#define  KDVD_ERR_MALLOC          KDPLAY_ALLOC_MEMORY_ERROR
#define  KDVD_ERR_CREATE_THREAD   1028
#define  KDVD_ERR_CREATE_DECODER  1029
#define  KDVD_ERR_DEL_DECODER     1030
#define  KDVD_ERR_CREATE_TIMER    1031
#define  KDVD_ERR_IN_BUF_FULL     KDPLAY_BUF_OVER
#define  KDVD_ERR_IN_BUF_EMPTY    1033
#define  KDVD_ERR_FRAME_SIZE      1034
#define  KDVD_ERR_MODULE_UNINITED 1035
#define  KDVD_ERR_DECODE_BUFFER_FULL 1036
#define  MFX_ERROR_BASE					(s32)1050
#define  MFX_ERROR_GPU_MEDIA_TYPE		(MFX_ERROR_BASE+1)//GPU support only h264&h265
#define  MFX_ERROR_GPU_NOT_INIT			(MFX_ERROR_BASE+2)//
#define  MFX_ERROR_GPU_WND_INVALID		(MFX_ERROR_BASE+3)//
#define  MFX_ERROR_GPU_RECT_OUTREGION	(MFX_ERROR_BASE+4)//

#define  KDVD_ERR_BUF_SIZE        1100
#define  KDVD_ERROR               1101
#define  KDVD_ERR_CODEC_BUSY      1102  //codec存储不下未解码数据。
#define  KDVD_ERR_CODEC_EMPTY     1103  //CODEC中所有的数据都解码完成并都返回给上层，
#define  KDVD_ERR_CODEC_WAIT      1104  //CODEC中还有缓冲有未解码数据，并且当前没有解码完成数据。
#define  KDVD_ERR_LOOPBUF_EMPTY   1105
#define  KDVD_ERR_CODEC_NOT_SURPPORT  1106
#define  KDVD_ERR_MALLOC_CODEC    1107
#define  KDVD_ERR_NO_DISPLAY      1108
#define  KDVD_ERR_PERMISSION_DENIED 1110
#define  KDVD_ERR_CODEC_RETRY     1111
#define  KDVD_ERR_KEYFRAME_RETRY     1112

//音频解码模块错误码
#define  KDAD_OK                  KDPLAY_NOERROR
#define  KDAD_ERROR               3001
#define  KDAD_ERR_PARAM           3002
#define  KDAD_ERR_NO_ID           3003
#define  KDAD_ERR_MALLOC          3004
#define  KDAD_ERR_CREATE_THREAD   3005
#define  KDAD_ERR_CREATE_DECODER  3006
#define  KDAD_ERR_DEL_DECODER     3007
#define  KDAD_ERR_DECODE_AFRAME   3008
#define  KDAD_ERR_IN_BUF_FULL     3009
#define  KDAD_ERR_IN_BUF_EMPTY    3010
#define  KDAD_ERR_FRAME_SIZE      KDVD_ERR_FRAME_SIZE
#define  KDAD_ERR_MODULE_UNINITED 3012
#define  KDAD_ERR_MODULE_INITED   3013

//视频播放模块错误码
#define  KDVP_OK                             KDPLAY_NOERROR//1000
#define  KDVP_ERROR                          2201  /*普通错误*/
#define  KDVP_ERR_MALLOC_MEM                 KDPLAY_ALLOC_MEMORY_ERROR  /*内存分配失败*/
#define  KDVP_ERR_FUNC_NOT_SUPPORT           KDPLAY_OPERTION_NOTALLOWED  /*该功能不提供*/
#define  KDVP_ERR_MODULE_INITED              2204  /*该模块已经被初始化*/
#define  KDVP_ERR_MODULE_UNINITED            2205  /*该模块未被初始化*/
#define  KDVP_ERR_PLYERID_INVALID            2206  /*VID PLYER ID超出最大值*/
#define  KDVP_ERR_OSDID_INVALID              2207  /*OSD ID超出最大值*/
#define  KDVP_ERR_SHOWRECTID_INVALID         2208  /*SHOWRECT ID超出最大值*/
#define  KDVP_ERR_PARAM                      KDPLAY_PARA_OVER  /*参数错误*/
#define  KDVP_ERR_SHOWRECT_CREATED           2001  /*该显示区域已经创建*/
#define  KDVP_ERR_SHOWRECT_UNCREATED         2002  /*该显示区域未创建*/
#define  KDVP_ERR_GRAPHCARD_CAPS             2003  /*显卡能力不支持*/
#define  KDVP_ERR_CREATE_TEXTURE             2004  /*创建OPENGL内存纹理失败*/
#define  KDVP_ERR_COMPILE_SHADER             2005  
#define  KDVP_ERR_LINK_SHADER                2006
#define  KDVP_ERR_FRAGMENT_LOAD              2007
#define  KDVP_ERR_CREATE_SCREEN              2008  /*创建屏幕失败，可能已经存在*/
#define  KDVP_ERR_NO_MORE_VIDPLYER_NUM       2009  /*已经创建足够多的播放器了*/
#define  KDVP_ERR_PLYER_PAUSED               2010  /*播放器已经被暂停*/
#define  KDVP_ERR_CLRMODE_SETTED             2011  /*当前已经是该清屏模式*/
#define  KDVP_ERR_SHOWRECT_DNOT_DRAW         2012  /*无效暂停或不需要重绘*/
#define  KDVP_ERR_NO_MORE_OSD_NUM            2013  /*不能再创建OSD层了*/
#define  KDVP_ERR_OSD_SET_HIDE               2014  /*Osd的HIDE状态与设置的一致*/
#define  KDVP_ERR_OPEN_DISPLAY               2015  /*XWINDOW打开DISPLAY失败*/
#define  KDVP_ERR_CHOOSE_VISUAL              2016  /*XWINDOW choose visual失败*/
#define  KDVP_ERR_CREATE_CONTEXT             2017  /*创建GL RENDER CONTEXT失败*/
#define  KDVP_ERR_MAKE_CURRENT_RC            2018  /*建立OPENGL RC链接失败*/
#define  KDVP_ERR_WND_STYLE                  2019  /*窗口风格不对*/
#define  KDVP_ERR_CHOOSE_FORMAT              2020  /*选择匹配的像素格式失败*/
#define  KDVP_ERR_SET_FORMAT                 2021  /*设置匹配的像素格式失败*/
#define  KDVP_ERR_SET_RENDERTYPE             2022  /*无法更改窗口的渲染类型*/
#define  KDVP_ERR_FOUND_VPWND_OF_VIDPLYER    2023  /*查找某个视频输入流的默认显示区域的窗口输出源失败，很可能该视频输出源被重新打开替换掉了。*/
#define  KDVP_ERR_HAVE_NO_OSDLAYER_OF_VPWND  2024  /*该窗口输出源还没设置OSD层*/
#define  KDVP_ERR_THIS_THREAD_CREATED_PLYER  2025  /*该线程已经创建一个PLYER，必须先删除！可能上层换了线程来显示原来创建的PLYER*/
#define  KDVP_ERR_NO_MORE_SHOWRECT_NUM       2026  /*无法再创建显示区域了*/
#define  KDVP_ERR_NOT_RIGHT_THREAD_ID        2027  /*该视频流不是在该线程中创建的*/ 
#define  KDVP_ERR_GET_SWSCONTEXT             2028  /*创建SWS CONTEXT失败*/
#define  KDVP_ERR_NO_MORE_WND                2029  /*没有更多的窗口可以创建*/
#define  KDVP_ERR_MAKE_CURRENT               2030  /*选择当前RC失败*/
#define  KDVP_ERR_CREATE_PROGRAM             2031  /*创建SHADER的PROGRAM失败*/
#define  KDVP_ERR_CREATE_SHADER              2032  /*创建SHADER失败*/
#define  KDVP_ERR_ATTACH_SHADER              2033
#define  KDVP_ERR_GET_PROGRAM_SHADER         2034
#define  KDVP_ERR_SHOWRECT_DEATCH_WND        2035
#define  KDVP_ERR_CREATE_OPENGL_OBJ          2036
#define  KDVP_ERR_GET_XWINDOW_ATTRI          2037
#define  KDVP_ERR_WINDOW_SIZE                KDPLAY_WINDOW_SIZE_ERROR  /*可能是因为窗口隐藏*/
#define  KDVP_ERR_SHOULD_DEL_PLYER           2039  /*模块区初始化之前必须先删除PLYER*/
#define  KDVP_ERR_CREATE_VPWND               2040
#define  KDVP_ERR_OPENGL_FUN_NOT_SUPPORT     2041
#define  KDVP_ERR_VP_CREATED                 2042
#define  KDVP_ERR_EARLY_INIT                 2100
#define  KDVP_ERR_ATTACH_THREAD_TO_JVM       2101
#define  KDVP_ERR_FOUND_VPGLSURFACEVIEW      2102
#define  KDVP_ERR_REG_JNI                    2103
#define  KDVP_DIDNOT_RENDER                  2104
#define  KDVP_ERR_DDRAW_EXCLUSIVEMODEALREADYSET  2142
#define  KDVP_ERR_SURFACE                        2143

//音频采集模块错误码
#define KDAC_OK                         KDPLAY_NOERROR
#define KDAC_ERROR                      4001
#define KDAC_ERR_OPEN_AC                4002
#define KDAC_ERR_CREATED                4003
#define KDAC_ERR_DEVID_INVALID          4004
#define KDAC_ERR_DEV_EXISTED            4005
#define KDAC_ERR_MALLOC                 4006
#define KDAC_ERR_NO_DEV                 4007
#define KDAC_ERR_AUD_CAP_OTHER_ERR      4008
#define KDAC_ERR_AUDDEV_PARAM           4009
#define KDAC_ERR_PARAM                  4010
#define KDAC_ERR_CREATE_DEV             4011   
#define KDAC_ERR_CREATE_THREAD          4012
#define KDAC_ERR_MODULE_UNINITED        4013
#define KDAC_ERR_MODULE_INITED          4014
#define KDAC_ERR_NEED_RESET				4015
#define KDAC_WARN_CAP_RENDER_FAILED     4100
#define KDAC_WARN_CAP_NOSURPPORT        4101 //不支持的采样率，要求用户手动设置为48K或者是16K或32K
#define KDAC_ERR_OUTBUF_EMPTY           4102

//音频播放模块错误码
#define  KDAP_OK         KDPLAY_NOERROR
#define  KDAP_ERROR                  5001
#define  KDAP_ERR_PARAM              5002
#define  KDAP_ERR_MODULE_UNINITED    5003
#define  KDAP_ERR_STREAMID_INVALID   5004
#define  KDAP_ERR_NO_STREAM          5005
#define  KDAP_ERR_STREAM_FULL        5006
#define  KDAP_ERR_MALLOC_FAIL        KDPLAY_ALLOC_MEMORY_ERROR
#define  KDAP_ERR_CREATE_DEV         5008
#define  KDAP_ERR_OPEN_SND           5009
#define  KDAP_ERR_BUF_FULL           5010
#define  KDAP_ERR_IO_RESET           5011
#define  KDAP_ERR_AP_CODEC_CONTROL   5012 
#define  KDAP_ERR_FRAME_BUF_EMPTY    5013
#define  KDAP_ERR_CREATE_THREAD      5014
#define  KDAP_ERR_AUDDEV_PARAM       5015
#define  KDAP_ERR_OPEN_MIXER         5016
#define  KDAP_ERR_ATTACH_MIXER       5017
#define  KDAP_ERR_LOAD_MIXER         5018
#define  KDAP_ERR_REGISTER_MIXER     5019
#define  KDAP_ERR_FOUND_MASTER       5020
#define  KDAP_ERR_STREAM_MUTEX       5021
#define  KDAP_ERR_FRAMELEN           5022
#define  KDAP_ERR_MODULE_INITED      5023
#define  KDAP_ERR_STOP_SOUND         5024

//音频编码模块错误码
#define  KDAE_OK                  KDPLAY_NOERROR
#define  KDAE_ERROR               6001
#define  KDAE_ERR_PARAM           6002
#define  KDAE_ERR_NO_ID           6003
#define  KDAE_ERR_MALLOC          6004
#define  KDAE_ERR_CREATE_THREAD   6005
#define  KDAE_ERR_CREATE_DECODER  6006
#define  KDAE_ERR_DEL_DECODER     6007
#define  KDAE_ERR_DECODE_AFRAME   6008
#define  KDAE_ERR_IN_BUF_FULL     6009
#define  KDAE_ERR_IN_BUF_EMPTY    6010
#define  KDAE_ERR_FRAME_SIZE      KDVD_ERR_FRAME_SIZE
#define  KDAE_ERR_MODULE_UNINITED 6012
#define  KDAE_ERR_MODULE_INITED   6013


//视频采集错误码
#define KDVC_OK                   KDPLAY_NOERROR
#define KDVC_ERROR                7001
#define KDVC_ERROR_PARAM          7002
#define KDVC_ERROR_INIT           7003
#define KDVC_ERROR_ENUM           7004
#define KDVC_ERROR_START          7005
#define KDVC_ERROR_GETPIN         7006
#define KDVC_ERROR_RENDER         7007
#define KDVC_ERROR_GETCAPBILITY   7008
#define KDVC_ERROR_SETCAPBILITY   7009
#define KDVC_ERROR_DEVICE         7010
#endif

