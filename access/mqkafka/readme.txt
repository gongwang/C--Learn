kafka��Ϣ���з�װ�⣬��������librdkafka�⣬pthread���zlib�⡣
librdkafka�����ʹ��c99��׼����ʹ��vs2013���ϰ汾����ɶ�̬���ٸ�vs2010ʹ��
librdkafka����libssl��libsasl��centos��ʹ���谲װopenssl-devel��cyrus-sasl-devel��
���ڴ������Ӻͷ�����/������ʱ�ĳ�ʱʱ�䣬Զ��ʱ1s����ֻ�ȡgroup��Ϣʧ�ܵ��´���receiverʧ������

librdkafka 0.11.3�������⣺
��ʹ��vs2015�����ϰ汾��ʹ��SASL�谲װ1.0.2n�汾��openssl
�����ֶ�����legacy_stdio_definitions.lib��̬�⣬
������
#if _MSC_VER>=1900
#ifndef _IOB_FUNC_INSTEAD_
#define _IOB_FUNC_INSTEAD_
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#endif // !_IOB_FUNC_INSTEAD_
#endif /* _MSC_VER>=1900 */
�������κ�cԴ���ļ���ʵ�ֺ���__iob_func
#if _MSC_VER>=1900
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */