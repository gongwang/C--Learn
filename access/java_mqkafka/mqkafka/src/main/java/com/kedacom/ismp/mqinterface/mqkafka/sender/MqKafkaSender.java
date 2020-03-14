/**
 * 文件名：MqKafkaSender.java 2017年8月24日
 * 版权：Copyright(C) 2003-2017 Suzhou Keda Technology Co., Ltd. All rights reserved.
 * 修改人：刘春光
 * 修改时间： 2017年8月24日
 * 修改内容：创建文件
 */
package com.kedacom.ismp.mqinterface.mqkafka.sender;

import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Properties;
import java.util.concurrent.CancellationException;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.producer.Callback;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.producer.RecordMetadata;

import com.kedacom.ismp.common.utils.IsmpGlobalUtil;
import com.kedacom.ismp.mqinterface.EnumMqLogLevel;
import com.kedacom.ismp.mqinterface.IMqInterface;
import com.kedacom.ismp.mqinterface.IMqOnEvent;
import com.kedacom.ismp.mqinterface.MqError;
import com.kedacom.ismp.mqinterface.sender.IMqSender;
import com.kedacom.ismp.mqinterface.sender.MqSenderParam;
import com.kedacom.ismp.mqinterface.mqkafka.MqKafka;

/**
 * @description 
 * @since 2017年8月24日
 * @author 刘春光
 * @version 1.0
 * @date 2017年8月24日
 */
public class MqKafkaSender<K,V> implements IMqSender<K,V>,Callback {
	private final static int SEND_SUCCESS = 9999;
	private int senderIndex = 1;	//用于防止sender重新连接的chientid重复
	private MqKafka server = null;
	private String topic;
	private String name;        //关键字 对应属性client.id 为当前的receiver或sender在服务器端的标识符，可为空
	private IMqOnEvent   eventCb = null;     //事件回调
	private int maxQueueNum = 0;               //异步发送时消息队列的大小，默认无需更改
	private int maxQueueRetainTime = 0;        //异步发送时队列的最大缓存时间，默认无需修改
	private KafkaProducer<K, V> producer;
	private boolean init = false;
	private boolean connected = false;          //是否成功连接服务器
	private int nSendState;
	private String keySerializer;
	private String valueSerializer;
	
	public MqKafkaSender(MqKafka parent, String topic, MqSenderParam<K,V> senderParam, 
			int nTimeout)throws Exception{
		this.server = parent;
		if(null == server){
			throw new Exception("server is null");
		}
		if(0 == topic.length()){
			throw new Exception("topics is empty");
		}
		this.topic = topic;
		eventCb = senderParam.IEventCb;
		maxQueueNum = senderParam.nMaxQueueNum;
		maxQueueRetainTime = senderParam.nMaxQueueRetainTime;
		name = senderParam.name;
		if(senderParam.name.length() != 0){
			name = senderParam.name;
		}else{
			//如果name为空，默认值填写kafkaJavaProducer
		    name = "";
//		    name = "kafkaJavaProducer";
		}
		init = true;
		keySerializer = getSerializerString(senderParam.getKeyClass());
		if(0 == keySerializer.length()){
			throw new Exception("generate key Serializer failed!");
		}
		valueSerializer = getSerializerString(senderParam.getValueClass());
		if(0 == valueSerializer.length()){
			throw new Exception("generate value Serializer failed!");
		}
		if(MqError.ERR_NO !=  connect()){
			throw new Exception("create kafkaProducer failed!");
		}
		connected = true;
	}
	
	private <T> String getSerializerString(Class<T> cls){
		String serializerString = new String();
		if(cls == String.class){
			serializerString = "org.apache.kafka.common.serialization.StringSerializer";
		}else if(cls == byte[].class){
			serializerString = "org.apache.kafka.common.serialization.ByteArraySerializer";
		}else if(cls == ByteBuffer.class){
			serializerString = "org.apache.kafka.common.serialization.ByteBufferSerializer";
		}else if(cls == org.apache.kafka.common.utils.Bytes.class){
			serializerString = "org.apache.kafka.common.serialization.BytesSerializer";
		}else if(cls == Double.class){
			serializerString = "org.apache.kafka.common.serialization.DoubleSerializer";
		}else if(cls == Integer.class){
			serializerString = "org.apache.kafka.common.serialization.IntegerSerializer";
		}else if(cls == Long.class){
			serializerString = "org.apache.kafka.common.serialization.LongSerializer";
		}
		return serializerString;
	}
	
	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.sender.IMqSender#GetServer()
	 */
	public IMqInterface getServer() {
		return server;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.sender.IMqSender#SendMessage(java.lang.Object, java.lang.Object, boolean, boolean, int)
	 */
	public MqError sendMessage(K key, V value, boolean bSync, boolean bType,
			int nTimeout) {
		if(server.getServerState() != IMqInterface.MQ_SERVER_ONLINE ){
			return MqError.ERR_SERVER_DOWN;
		}
		StringBuilder logSb = new StringBuilder();
		nSendState = 0;
		ProducerRecord<K,V> sendRecord = new ProducerRecord<K,V>(topic, key, value);
		Future<RecordMetadata> future = producer.send(sendRecord, this);
		if(bSync){
			try{
				future.get(nTimeout, TimeUnit.MILLISECONDS);
			}catch(InterruptedException ie){
				//InterruptedException 线程被中断异常
				eventCb.onEvent(MqError.ERR_SDK, "producer线程中断");
			}catch(ExecutionException ee){
				//ExecutionException任务执行异常
				eventCb.onEvent(MqError.ERR_SDK, "producer发送异常");
			}catch(CancellationException ce){
				//InterruptedException 任务被取消
				eventCb.onEvent(MqError.ERR_SDK, "producer发送异常");
			}catch(TimeoutException te){
				//超时
				eventCb.onEvent(MqError.ERR_TIMEOUT, "producer 发送超时");
			}catch(Exception e){
				//
				eventCb.onEvent(MqError.ERR_SDK, "producer发送异常");
			}
			if(SEND_SUCCESS != nSendState){
				if(IsmpGlobalUtil.DEBUG_FLAG){
					//计算时间戳
					logSb.append("MqKafkaSender::sendMessage[");
					SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
					logSb.append(formatter.format(new Date())).append("]");
					logSb.append(" topic:").append(this.topic);
					logSb.append(" key:").append(key.toString());
					logSb.append(" value:").append(value.toString());
					logSb.append(" send FAILED!");
					System.out.println(logSb.toString());
				}
				return MqError.ERR_SDK;
			}
		}
		if(IsmpGlobalUtil.DEBUG_FLAG){
			//计算时间戳
			logSb.append("MqKafkaSender::sendMessage[");
			SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
			logSb.append(formatter.format(new Date())).append("]");
			logSb.append(" topic:").append(this.topic);
			logSb.append(" key:").append(key.toString());
			logSb.append(" value:").append(value.toString());
			logSb.append(" send success");
			System.out.println(logSb.toString());
		}
		return MqError.ERR_NO;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.sender.IMqSender#CloseSender()
	 */
	public MqError closeSender() {
		disConnect();
		server.removeSender(this);
		return MqError.ERR_NO;
	}
	
	public MqError connect(){
		if(!init || connected){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "connect: sender not init or has been connected!");
			return MqError.ERR_WRONG_LOGIC;
		}
		String serverAddr = server.getServerAddress();
		if(null == serverAddr || 0 == serverAddr.length()){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "reconnect:get server address failed!");
			return MqError.ERR_SERVER_DOWN;
		}
		try{
		//create kafkaproducer
		Properties props = new Properties();
		props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, serverAddr);
		if(name.isEmpty()){
			props.put(ProducerConfig.CLIENT_ID_CONFIG, "");
		}else{
			props.put(ProducerConfig.CLIENT_ID_CONFIG, name + senderIndex++);
		}
		props.put(ProducerConfig.ACKS_CONFIG, "1");	//只需要主节点确认
//		props.put("retries", 0);
		if(0 != maxQueueNum){
			props.put(ProducerConfig.BATCH_SIZE_CONFIG, maxQueueNum);	//发送缓冲区大小
		}
		//设置消息队列最大保留时间
		if(0 != maxQueueRetainTime){
			props.put(ProducerConfig.LINGER_MS_CONFIG, maxQueueRetainTime);
		}
		//设置消息阻塞或服务器不可达时的最大阻塞时间
		props.put(ProducerConfig.MAX_BLOCK_MS_CONFIG, IMqInterface.MQ_DEFAULT_TIMEOUT);//
		props.put("key.serializer", keySerializer);
		props.put("value.serializer", valueSerializer);
		//create producer
		producer = new KafkaProducer<K,V>(props); //could throw KafkaException
		}catch(Exception e){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "reconnect:KafkaProducer failed!");
			return MqError.ERR_SDK;
		}
		connected = true;
		return MqError.ERR_NO;
	}
	
	public MqError disConnect(){
		if(!init){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "disconnect: sender not init !");
			return MqError.ERR_WRONG_LOGIC;
		}
		if(connected){
			try{
				producer.close(IMqInterface.MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
			}catch(Exception e){
				//根据函数说明，超时只会导致数据发送失败，不会导致producer仍然存在
			}
			connected = false;
		}
		return MqError.ERR_NO;
	}

	/* (non-Javadoc)
	 * @see org.apache.kafka.clients.producer.Callback#onCompletion(org.apache.kafka.clients.producer.RecordMetadata, java.lang.Exception)
	 */
	public void onCompletion(RecordMetadata metadata, Exception exception) {
		if(null != exception){
//			exception - The exception thrown during processing of this record. 
//			Null if no error occurred. Possible thrown exceptions include: 
//				Non-Retriable exceptions (fatal, the message will never be sent): InvalidTopicException OffsetMetadataTooLargeException RecordBatchTooLargeException RecordTooLargeException UnknownServerException 
//				Retriable exceptions (transient, may be covered by increasing #.retries): CorruptRecordException InvalidMetadataException NotEnoughReplicasAfterAppendException NotEnoughReplicasException OffsetOutOfRangeException TimeoutException UnknownTopicOrPartitionException
			String exceptionName = exception.getClass().getName();
			if(-1 != "UnknownServerException".indexOf(exceptionName)){
				server.setCurState(IMqInterface.MQ_SERVER_OFFLINE);
				//eventCb.onEvent(MqError.ERR_SERVER_DOWN, "server is down");
			}else if(-1 != "TimeoutException".indexOf(exceptionName)){
				String lostMsg = String.format("message lost:%s, ", metadata.toString());
				eventCb.onEvent(MqError.ERR_SDK, lostMsg);
			}else {
				eventCb.onEvent(MqError.ERR_SDK, exceptionName);
			}
		}else{
			nSendState = SEND_SUCCESS;
		}
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.sender.IMqSender#getTopic()
	 */
	@Override
	public String getTopic() {
		return topic;
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.sender.IMqSender#getName()
	 */
	@Override
	public String getName() {
		return name;
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.sender.IMqSender#getMaxQueueNum()
	 */
	@Override
	public int getMaxQueueNum() {
		return maxQueueNum;
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.sender.IMqSender#getMaxQueueRetainTime()
	 */
	@Override
	public int getMaxQueueRetainTime() {
		return maxQueueRetainTime;
	}


}
