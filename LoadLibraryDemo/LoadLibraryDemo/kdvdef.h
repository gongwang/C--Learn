/*****************************************************************************

   模块名      : KDV define 

   文件名      : kdvdef.h

   相关文件    : 

   文件实现功能: KDV宏定义

   作者        : 魏治兵

   版本        : V3.0  Copyright(C) 2001-2002 KDC, All rights reserved.

-----------------------------------------------------------------------------

   修改记录:

   日  期      版本        修改人      修改内容

   2004/02/17  3.0         魏治兵        创建

******************************************************************************/

#ifndef _KDV_DEFINE_H_

#define _KDV_DEFINE_H_

//vc6没有该头文件
#if _MSC_VER > 1200
#include <stdint.h>
#endif

#ifdef __cplusplus

extern "C" {

#endif /* __cplusplus */





/*媒体类型定义*/

#define   MEDIA_TYPE_NULL                   (uint8_t)255  /*媒体类型为空*/
    
/*音频*/
#define   MEDIA_TYPE_OPUS                 (uint8_t)117  //OPUS传输值
#define   MEDIA_TYPE_AACLD                (uint8_t)103
#define   MEDIA_TYPE_AACLC_PCM			  (uint8_t)104
#define   MEDIA_TYPE_MP2                  (uint8_t)14
#define   MEDIA_TYPE_AACLC_32K            (uint8_t)108
#define   MEDIA_TYPE_AACLC_48K            (uint8_t)109

#define   MEDIA_TYPE_MP3                    (uint8_t)99  /*mp3 mode 0-4*/

#define   MEDIA_TYPE_G7221C                 (uint8_t)98  /*G722.1.C Siren14*/

#define   MEDIA_TYPE_PCMA                   (uint8_t)8   /*G.711 Alaw  mode 5*/

#define   MEDIA_TYPE_PCMU                   (uint8_t)0   /*G.711 ulaw  mode 6*/

#define   MEDIA_TYPE_G721                   (uint8_t)2   /*G.721*/

#define   MEDIA_TYPE_G722                   (uint8_t)9   /*G.722*/

#define   MEDIA_TYPE_G7231                  (uint8_t)4   /*G.7231*/

#define   MEDIA_TYPE_ADPCM                  (uint8_t)5   /*DVI4 ADPCM*/

#define   MEDIA_TYPE_G728                   (uint8_t)15  /*G.728*/

#define   MEDIA_TYPE_G729                   (uint8_t)18  /*G.729*/
#define   MEDIA_TYPE_G7221                  (uint8_t)13  /*G.7221*/

#define   MEDIA_TYPE_AACLC                  (uint8_t)102
#define   MEDIA_TYPE_AMR                    (uint8_t)105

/*视频*/

#define   MEDIA_TYPE_PS                      (uint8_t)96  /*ps流*/

#define   MEDIA_TYPE_MP4                    (uint8_t)97  /*MPEG-4*/
#define   MEDIA_TYPE_VP8                    (uint8_t)12  /*vp8*/
#define   MEDIA_TYPE_VP9                    (uint8_t)13  /*vp8*/

#define   MEDIA_TYPE_H261                   (uint8_t)31  /*H.261*/

#define   MEDIA_TYPE_H262                   (uint8_t)33  /*H.262 (MPEG-2)*/
#define   MEDIA_TYPE_MJPEG                  (uint8_t)26  
#define   MEDIA_TYPE_SVACV                  (uint8_t)107
#define   MEDIA_TYPE_SVACA                  (uint8_t)20

#define   MEDIA_TYPE_H263                   (uint8_t)34  /*H.263*/

#define   MEDIA_TYPE_H263PLUS               (uint8_t)101 /*H.263+*/

#define   MEDIA_TYPE_H264                   (uint8_t)106 /*H.264*/
#define	  MEDIA_TYPE_FEC				    (uint8_t)107 /* fec custom define */
#define	  MEDIA_TYPE_H265				    (uint8_t)111 /* h265编解码类型 */
#define	  MEDIA_TYPE_MPEG2				    (uint8_t)95 /* mpeg2编解码类型 */

/*数据*/
#define   MEDIA_TYPE_H224                   (uint8_t)100  /*H.224 Payload 暂定100*/
#define   MEDIA_TYPE_T120                   (uint8_t)200  /*T.120媒体类型*/
#define   MEDIA_TYPE_H239                   (uint8_t)239  /*H.239数据类型 */
#define   MEDIA_TYPE_MMCU                   (uint8_t)120  /*级联数据通道 */
    

/*kdv约定的本地发送时使用的活动媒体类型 */
#define   ACTIVE_TYPE_PCMA                  (uint8_t)110   /*G.711 Alaw  mode 5*/
#define   ACTIVE_TYPE_PCMU                  (uint8_t)111   /*G.711 ulaw  mode 6*/
#define   ACTIVE_TYPE_G721                  (uint8_t)112   /*G.721*/
#define   ACTIVE_TYPE_G722                  (uint8_t)113   /*G.722*/
#define   ACTIVE_TYPE_G7231                 (uint8_t)114   /*G.7231*/
#define   ACTIVE_TYPE_G728                  (uint8_t)115   /*G.728*/
#define   ACTIVE_TYPE_G729                  (uint8_t)116   /*G.729*/
#define   ACTIVE_TYPE_G7221                 (uint8_t)117   /*G.7221*/
#define   ACTIVE_TYPE_H261                  (uint8_t)118   /*H.261*/
#define   ACTIVE_TYPE_H262                  (uint8_t)119   /*H.262 (MPEG-2)*/
#define   ACTIVE_TYPE_H263                  (uint8_t)120   /*H.263*/
#define   ACTIVE_TYPE_G7221C			          (uint8_t)121	/*G7221c*/
#define   ACTIVE_TYPE_ADPCM				          (uint8_t)122	/*ADPCM*/	
#define   ACTIVE_TYPE_AACLC				          (uint8_t)123	/*AACLC*/	
#define   ACTIVE_TYPE_AACLD           			(uint8_t)124	/*AACLD*/	

    
#define   MEDIA_RES_AUTO                    (uint8_t)0      /* 自动、默认 */

#define   MEDIA_RES_QQCIF                   (uint8_t)0x08   /* 96x80  */
#define   MEDIA_RES_SQCIF                   (uint8_t)0x10   /* 128x96  */
#define   MEDIA_RES_QCIF                    (uint8_t)0x20   /* 176x144 */

#define   MEDIA_RES_CIF                     (uint8_t)0x40   /* 352x288 */
#define   MEDIA_RES_SIF                     (uint8_t)0x41   /* 352x240 N制 CIF */
#define   MEDIA_RES_QVGA                    (uint8_t)0x42   /* 320x240 */

#define   MEDIA_RES_2CIF                    (uint8_t)0x60   /* 352x576 */

#define   MEDIA_RES_4CIF                    (uint8_t)0x80   /* 704x576 */
#define   MEDIA_RES_4SIF                    (uint8_t)0x81   /* 704x480 */
#define   MEDIA_RES_D1                      (uint8_t)0x82   /* 720x576 */
#define   MEDIA_RES_VGA                     (uint8_t)0x83   /* 640X480 */

#define   MEDIA_RES_SVGA                    (uint8_t)0xA1   /* 800x600  */
#define   MEDIA_RES_XGA                     (uint8_t)0xA2   /* 1024x768 */
#define   MEDIA_RES_XVGA                    (uint8_t)0xA3   /* 1280x960 */

#define   MEDIA_RES_WXGA                    (uint8_t)0xB1   /* 1280x800 */
#define   MEDIA_RES_WXGAP                   (uint8_t)0xB2   /* 1440x900 */

#define   MEDIA_RES_720P                    (uint8_t)0xD0   /* 1280x720 */

#define   MEDIA_RES_1080P                   (uint8_t)0xE0   /* 1920x1080 P */
#define   MEDIA_RES_1080I                   (uint8_t)0xE1   /* 1920x1080 I */

/*2004/07/15 不同类型音频码率定义*/

#define AUDIO_BITRATE_G711A 64
#define AUDIO_BITRATE_G711U 64
#define AUDIO_BITRATE_G7231 6
#define AUDIO_BITRATE_MP3   48
#define AUDIO_BITRATE_G728  16
#define AUDIO_BITRATE_G729  8/*暂不支持*/
#define AUDIO_BITRATE_G722  64
#define AUDIO_BITRATE_G7221C   24


#define MAX_VIDEO_FRAME_SIZE (512 * 1024)
#define MAX_AUDIO_FRAME_SIZE (2 * 1024)

#define MEDIA_PROFILE_AVC_BASELINE  66
#define MEDIA_PROFILE_AVC_MAIN      77
#define MEDIA_PROFILE_AVC_HIGH      100

#define MEDIA_LEVEL_AVC_1       10
#define MEDIA_LEVEL_AVC_1b      9
#define MEDIA_LEVEL_AVC_11      11
#define MEDIA_LEVEL_AVC_12      12
#define MEDIA_LEVEL_AVC_13      13
#define MEDIA_LEVEL_AVC_2       20
#define MEDIA_LEVEL_AVC_21      21
#define MEDIA_LEVEL_AVC_22      22
#define MEDIA_LEVEL_AVC_3       30
#define MEDIA_LEVEL_AVC_31      31
#define MEDIA_LEVEL_AVC_32      32
#define MEDIA_LEVEL_AVC_4       40
#define MEDIA_LEVEL_AVC_41      41
#define MEDIA_LEVEL_AVC_42      42
#define MEDIA_LEVEL_AVC_5       50
#define MEDIA_LEVEL_AVC_51      51
#define MEDIA_LEVEL_AVC_52      52

/*APP ID*/

#define AID_KDV_BASE                        (uint16_t)0 



/*网管客户端内部应用号（1-10）*/

#define AID_NMC_BGN      (AID_KDV_BASE + 1)

#define AID_NMC_END      (AID_NMC_BGN + 9)

    

/*会议控制台内部应用号（11-20）*/

#define AID_MCS_BGN      (AID_NMC_END + 1)

#define AID_MCS_END      (AID_MCS_BGN + 9)



/*终端控制台内部应用号（21-30）*/

#define AID_MTC_BGN      (AID_MCS_END + 1)

#define AID_MTC_END      (AID_MTC_BGN + 9)

    

/*网管服务器内部应用号（31-40）*/

#define AID_SVR_BGN      (AID_MTC_END + 1)

#define AID_SVR_END      (AID_SVR_BGN + 9)



/*代理内部应用号（41-50）*/

#define AID_AGT_BGN      (AID_SVR_END + 1)

#define AID_AGT_END      (AID_AGT_BGN + 9)

    

/*MCU内部应用号（51-100）*/

#define AID_MCU_BGN      (AID_AGT_END + 1)

#define AID_MCU_END      (AID_MCU_BGN + 49)

    

/*MT内部应用号（101-150）*/

#define AID_MT_BGN       (AID_MCU_END + 1)

#define AID_MT_END       (AID_MT_BGN + 49)

    

/*RECORDER内部应用号（151-160）*/

#define AID_REC_BGN      (AID_MT_END + 1)

#define AID_REC_END      (AID_REC_BGN + 9)

    

/*混音器内部应用号（161-170）*/

#define AID_MIXER_BGN    (AID_REC_END + 1)

#define AID_MIXER_END    (AID_MIXER_BGN + 9)

    

/*电视墙内部应用号（171-181）*/

#define AID_TVWALL_BGN   (AID_MIXER_END + 1)

#define AID_TVWALL_END   (AID_TVWALL_BGN + 9)



/*T.120服务器内部应用号（181-200）*/

#define AID_DCS_BGN      (AID_TVWALL_END + 1)

#define AID_DCS_END      (AID_DCS_BGN + 19)



/*T.120控制台内部应用号（201-210）*/

#define AID_DCC_BGN      (AID_DCS_END + 1)

#define AID_DCC_END      (AID_DCC_BGN + 9)



/*码率适配服务器内部应用号（211-220）*/

#define AID_BAS_BGN      (AID_DCC_END + 1)

#define AID_BAS_END      (AID_BAS_BGN + 9)



/*GK服务器内部应用号(221 - 230)*/

#define AID_GKS_BGN      (AID_BAS_END + 1)

#define AID_GKS_END      (AID_GKS_BGN + 9)



/*GK控制台内部应用号(231 - 235)*/

#define AID_GKC_BGN      (AID_GKS_END + 1)

#define AID_GKC_END      (AID_GKC_BGN + 4)



/*用户管理内部应用号(236 - 240)*/

#define AID_UM_BGN      (AID_GKC_END + 1)

#define AID_UM_END      (AID_UM_BGN + 4)



/*地址簿内部应用号(241 - 250)*/

#define AID_ADDRBOOK_BGN    (AID_UM_END + 1)

#define AID_ADDRBOOK_END    (AID_ADDRBOOK_BGN + 9)

/*数据会议终端内部应用号(251 - 260)*/

#define AID_DCMT_BGN    (AID_ADDRBOOK_END + 1)

#define AID_DCMT_END    (AID_DCMT_BGN + 9)

/*mdsc hdsc watchdog 模块(261-265) hualiang add*/
#define AID_DSC_BGN     (AID_DCMT_END + 1) 
#define AID_DSC_END     (AID_DSC_BGN + 4)

/* radius 计费 模块(266-275) guozhongjun add*/
#define  AID_RS_BGN     (AID_DSC_END + 1)
#define  AID_RS_END     (AID_RS_BGN + 9)



/*2004/07/15 不同类型音频码率定义*/

#define AUDIO_BITRATE_G711A         64
#define AUDIO_BITRATE_G711U         64
#define AUDIO_BITRATE_G7231         6
#define AUDIO_BITRATE_MP3           48
#define AUDIO_BITRATE_G728          16
#define AUDIO_BITRATE_G729          8/*暂不支持*/
#define AUDIO_BITRATE_G722          64
#define AUDIO_BITRATE_G7221C        24

enum MESSAGE_TYPE
{
	MES_INTELLSEARCH=1,			//用于智能搜索信息
	MES_GPS						//用于gps
              
};


#if 0
/*视频会议产品各模块的监听端口*/

const u16 PORT_NMS                = 60000;

const u16 PORT_NMC                = 60000;

const u16 PORT_MCU                = 60000;

const u16 PORT_MCS                = 60000;

const u16 PORT_MT                 = 60000;

const u16 PORT_MTC                = 60000;

const u16 PORT_TVWALL             = 60000;

const u16 PORT_DIGITALMIXER       = 60000;

const u16 PORT_RECORDER           = 60000;

const u16 PORT_DCS                = 61000;

const u16 PORT_GK               = 60000; 

const u16 PORT_GKC             			= 60000; 

const u16 PORT_RS                 = 61000;


#endif
#ifdef __cplusplus

}

#endif  /* __cplusplus */




#endif /* _KDV_def_H_ */



/* end of file kdvdef.h */

