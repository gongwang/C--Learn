/**
 * 文件名：MqKafkaReceiver.java 2017年8月24日
 * 版权：Copyright(C) 2003-2017 Suzhou Keda Technology Co., Ltd. All rights reserved.
 * 修改人：刘春光
 * 修改时间： 2017年8月24日
 * 修改内容：创建文件
 */
package com.kedacom.ismp.mqinterface.mqkafka.receiver;

import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.UUID;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRebalanceListener;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.consumer.OffsetAndMetadata;
import org.apache.kafka.clients.consumer.OffsetCommitCallback;
import org.apache.kafka.common.TopicPartition;
import org.apache.kafka.common.errors.WakeupException;
import com.kedacom.ismp.common.utils.IsmpGlobalUtil;
import com.kedacom.ismp.mqinterface.EnumMqLogLevel;
import com.kedacom.ismp.mqinterface.IMqInterface;
import com.kedacom.ismp.mqinterface.IMqOnEvent;
import com.kedacom.ismp.mqinterface.MqError;
import com.kedacom.ismp.mqinterface.receiver.IMqOnCommit;
import com.kedacom.ismp.mqinterface.receiver.IMqOnMessage;
import com.kedacom.ismp.mqinterface.receiver.IMqOnPreciseMessage;
import com.kedacom.ismp.mqinterface.receiver.IMqReceiver;
import com.kedacom.ismp.mqinterface.receiver.MqMessage;
import com.kedacom.ismp.mqinterface.receiver.MqPreciseReceiverParam;
import com.kedacom.ismp.mqinterface.receiver.MqReceiverParam;
import com.kedacom.ismp.mqinterface.mqkafka.MqKafka;

/**
 * @description 
 * @since 2017年8月24日
 * @author 刘春光
 * @version 1.0
 * @date 2017年8月24日
 */
public class MqKafkaReceiver<K,V> implements IMqReceiver<K,V>,ConsumerRebalanceListener {
	private int receiverIndex = 1;		//用于防止receiver重新连接的chientid重复
	private MqKafka server = null;
	private String[] topics = null;
	private int msgTimeOut = 0;
	/**
	 * 当前是否实际处于消息订阅状态-运行态标志位
	 */
	private boolean  onSubscribe = false;        	
	/**
	 * 当前是否应该处于消息订阅状态-用户态标志位
	 */
	private boolean  receiveFlag = false;        	
	private int     startOffset = 0;          //消息的起始偏移，用于订阅成功后设置偏移地址
	private boolean init = false;
	private boolean connected = false;          //是否成功连接服务器
	private String name;
	private String group;       //groupid，消息所属的group
//	private IMqOnMessage onMessageCb = null;   //数据回调
	private IMqOnEvent   onEventCb = null;     //事件回调
    //pthread_t        m_tSubScribeThread;    //保存订阅poll数据线程句柄
	private KafkaConsumer<K,V> consumer = null;
	
	private ConsumerPoller poller = null;
	
	private Thread pollThread = null;
	
	private String keyDeserializer;
	private String valueDeserializer;
	
	private boolean preciseReceiver = false;
	private boolean autoCommit = true;
	private IMqOnCommit onCommitCb = null;

	public MqKafkaReceiver(MqKafka parent, String[] topic, MqReceiverParam<K,V> recvParam, int nTimeout) throws Exception{
		server = parent;
		if(null == server){
			throw new Exception("server is null");
		}
		topics = topic;
		if(0 == topic.length){
			throw new Exception("topics is empty");
		}
		preciseReceiver = false;
//		onMessageCb = recvParam.IMessageCb;
		onEventCb = recvParam.IEventCb;
		msgTimeOut = recvParam.nMsgTimeout;
		if(recvParam.name.length() != 0){
			name = recvParam.name;
		}else{
			//如果name为空，默认值填写kafkaJavaConsumer
		    name = "";
//		    name = "kafkaJavaConsumer";
		}
		if(recvParam.group.length() != 0){
			group = recvParam.group;
		}else{
			group = new String();
		}
		init = true;
		//根据传入key和value类型设置对应解码器
		keyDeserializer = getDeserializerString(recvParam.getKeyClass());
		if(0 == keyDeserializer.length()){
			throw new Exception("generate key Deserializer failed!");
		}
		valueDeserializer = getDeserializerString(recvParam.getValueClass());
		if(0 == valueDeserializer.length()){
			throw new Exception("generate value Deserializer failed!");
		}
		String serverAddr = server.getServerAddress();
		if(null == serverAddr || 0 == serverAddr.length()){
			throw new Exception("reconnect:get server address failed!");
		}
		// 生成groupid
		if(0 == group.length()){
			String groupName = generateNewGroupId(serverAddr);
	        group = groupName;
		}
		
		if(MqError.ERR_NO != connect()){
			throw new Exception("connect consumer failed!");
		}
		//设置回调
		poller = new ConsumerPoller( recvParam);
		//开始连接
		connected = true;
	}
	
	public MqKafkaReceiver(MqKafka parent, String[] topic, MqPreciseReceiverParam<K,V> recvParam, int nTimeout) throws Exception{
		server = parent;
		if(null == server){
			throw new Exception("server is null");
		}
		topics = topic;
		if(0 == topic.length){
			throw new Exception("topics is empty");
		}
		preciseReceiver = true;
		autoCommit = recvParam.autoCommit;
		onCommitCb = recvParam.ICommitCb;
//		onMessageCb = recvParam.IMessageCb;
		onEventCb = recvParam.IEventCb;
		msgTimeOut = recvParam.nMsgTimeout;
		if(recvParam.name.length() != 0){
			name = recvParam.name;
		}else{
			//如果name为空，默认值填写kafkaJavaConsumer
		    name = "";
//		    name = "kafkaJavaConsumer";
		}
		if(recvParam.group.length() != 0){
			group = recvParam.group;
		}else{
			group = new String();
		}
		init = true;
		//根据传入key和value类型设置对应解码器
		keyDeserializer = getDeserializerString(recvParam.getKeyClass());
		if(0 == keyDeserializer.length()){
			throw new Exception("generate key Deserializer failed!");
		}
		valueDeserializer = getDeserializerString(recvParam.getValueClass());
		if(0 == valueDeserializer.length()){
			throw new Exception("generate value Deserializer failed!");
		}
		String serverAddr = server.getServerAddress();
		if(null == serverAddr || 0 == serverAddr.length()){
			throw new Exception("reconnect:get server address failed!");
		}
		// 生成groupid
		if(0 == group.length()){
			String groupName = generateNewGroupId(serverAddr);
	        group = groupName;
		}
		
		if(MqError.ERR_NO != connect()){
			throw new Exception("connect consumer failed!");
		}
		//设置回调
		poller = new ConsumerPoller( recvParam);
		//开始连接
		connected = true;
	}
	
	/**
	 * @description 设置一个新的没有使用过的groupid，基础前缀为客户端name+defRecGroup，之后接groupid序号
     *  序号用服务器获取到的最后一个相同前缀的序号+1
	 * @param serverAddr 服务器地址字符串，从server对象获得
	 * @return 新的，没有被使用的groupid字符串
	 * @throws  null
	 * @since 2017年9月7日
	 * @author 刘春光
	 */
	private String generateNewGroupId(String serverAddr){
		String groupName = new String();
		groupName = groupName.concat(name).concat("def_rec_gr_");
		/**
		 * low-level api需要加载过多不需要的jar包，直接使用uuid尽量保证group.id的唯一性
		*/
		groupName = groupName.concat(UUID.randomUUID().toString().replaceAll("-", ""));
        return groupName;
	}
	
	private <T> String getDeserializerString(Class<T> cls){
		String deserializerString = new String();
		if(cls == String.class){
			deserializerString = "org.apache.kafka.common.serialization.StringDeserializer";
		}else if(cls == byte[].class){
			deserializerString = "org.apache.kafka.common.serialization.ByteArrayDeserializer";
		}else if(cls == ByteBuffer.class){
			deserializerString = "org.apache.kafka.common.serialization.ByteBufferDeserializer";
		}else if(cls == org.apache.kafka.common.utils.Bytes.class){
			deserializerString = "org.apache.kafka.common.serialization.BytesDeserializer";
		}else if(cls == Double.class){
			deserializerString = "org.apache.kafka.common.serialization.DoubleDeserializer";
		}else if(cls == Integer.class){
			deserializerString = "org.apache.kafka.common.serialization.IntegerDeserializer";
		}else if(cls == Long.class){
			deserializerString = "org.apache.kafka.common.serialization.LongDeserializer";
		}
		return deserializerString;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#GetServer()
	 */
	public IMqInterface getServer() {
		return server;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#StartReceive(int)
	 */
	public MqError startReceive(int nOffset) {
		if(!init || !connected){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "StartReceiver ERROR:receiver not inited or connetced!");
			return MqError.ERR_HANDLE_NOTFOUND;
		}
		if(onSubscribe){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "StartReceiver ERROR:receiver has started!");
			return MqError.ERR_WRONG_LOGIC;
		}
		try{
			startOffset = nOffset;
			poller.reset();
			pollThread = new Thread(poller);
			//开始订阅
			server.printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "consumer subscribe start.");
			consumer.subscribe(Arrays.asList(topics), this);
			//开始消息拉取
			server.printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "consumer poll thread start.");
			pollThread.start();
			onSubscribe = true;
			receiveFlag = true;
		}catch(Exception e){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "StartReceive get exceptions:%s.",
					e.getMessage());
			if(null != onEventCb){
				onEventCb.onEvent(MqError.ERR_SDK, e.getMessage());
			}
		}
		return MqError.ERR_NO;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#StopReceive()
	 */
	public MqError stopReceive() {
		return stopReceive(false);
	}
	
	private MqError stopReceive(boolean reconnectFlag){
		if(null == poller || null == pollThread){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, 
					"StopReceive ERROR:poller thread is null!");
			return MqError.ERR_WRONG_LOGIC;
		}
		if(!receiveFlag){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, 
					"StopReceive ERROR:receiver has not been started!");
			return MqError.ERR_WRONG_LOGIC;
		}
		if(onSubscribe){
			poller.shutdown();
			try{
				pollThread.join(IMqInterface.MQ_DEFAULT_TIMEOUT);
			}catch(Exception e){
				//poller.reset();
			}
			onSubscribe = false;
			if(!reconnectFlag){
				receiveFlag = false;
			}
		}
		return MqError.ERR_NO;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#CloseReceiver()
	 */
	public MqError closeReceiver() {
		disConnect(false);
		server.removeReceiver(this);
		return MqError.ERR_NO;
	}
	
	public MqError connect(){
		if(!init || connected){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, 
					"connect: receiver not init or has been connected!");
			return MqError.ERR_WRONG_LOGIC;
		}
		try{
			//TODO 和c++接口一样，断链重连之后需要手动调用startreceiver函数开始接受数据
			String serverAddr = server.getServerAddress();
			if(null == serverAddr || 0 == serverAddr.length()){
				server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "reconnect:get server address failed!");
				return MqError.ERR_SERVER_DOWN;
			}
			//设置consumer参数
			Properties props = new Properties();
			props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, serverAddr);
			props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");
			props.put(ConsumerConfig.AUTO_COMMIT_INTERVAL_MS_CONFIG, "1000");
			props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, keyDeserializer);
			props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, valueDeserializer);
			props.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, autoCommit ? "true" : "false");
			if(!autoCommit){
				//非自动提交状态只有精密接收器才会有，因此不需要再判断是否是精密接收器
				//设置每次只接受一条数据
				props.put(ConsumerConfig.MAX_POLL_RECORDS_CONFIG, "1");
			}
			if(name.isEmpty()){
				props.put(ConsumerConfig.CLIENT_ID_CONFIG, "");
			}else{
				props.put(ConsumerConfig.CLIENT_ID_CONFIG, name + receiverIndex++);
			}
			props.put(ConsumerConfig.GROUP_ID_CONFIG, group);
			/* 必需设置offset为broker存储 */
			consumer = new KafkaConsumer<K,V>(props);
			connected = true;
		}catch(Exception e){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "reconnect:KafkaReceiver failed!");
			return MqError.ERR_SDK;
		}
		return MqError.ERR_NO;
	}
	
	public MqError disConnect(boolean reconnectFlag){
		if(!init ){
			server.printLog(EnumMqLogLevel.MQ_LOG_ERROR, "disconnect: receiver not init !");
			return MqError.ERR_WRONG_LOGIC;
		}
		if(connected){
			try{
				stopReceive(reconnectFlag);
				consumer.unsubscribe();
				consumer.close(IMqInterface.MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
			}catch(Exception e){
			}
			connected = false;
		}
		return MqError.ERR_NO;
	}
	/**
	 * 提交消息偏移地址至服务器
	 * 只对精密接收器(PreciseReciver)有效
	 * 如果是异步执行，结果会通过commitCb回调上来
	 * @param topic			要提交的topic名称
	 * @param partition		要提交的分区号
	 * @param offset		要提交的消息偏移地址，
	 * @param async		异步执行标志位,禁止在回调中使用同步方式commit，可能会导致死锁。异步方式可用
	 * @return 
	 * @since 2019年3月5日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.receiver.IMqReceiver#commit(java.lang.String, int, int, boolean)
	 */
	public MqError commit(String topic, int partition, long offset, boolean async) {
		if(server.getServerState() != IMqInterface.MQ_SERVER_ONLINE ){
			return MqError.ERR_SERVER_DOWN;
		}
		if(!preciseReceiver || autoCommit){
			//非精密接收器，不可使用commit接口,设置了自动提交，也不可使用此接口
			return MqError.ERR_WRONG_LOGIC;
		}
		boolean find = false;
        for (String ownTopic : topics) {
            if (ownTopic.equals(topic)){
            	find = true;
            	break;
            }
        }
        if(!find){
        	return MqError.ERR_INVALID_PARAM;
        }
        Map<TopicPartition, OffsetAndMetadata> commitMap = new HashMap<>();
        TopicPartition tp = new TopicPartition(topic, partition);
        commitMap.put(tp, new OffsetAndMetadata(offset + 1));
        MqError ret = MqError.ERR_NO;
        if(async){
        	try{
        		KafkaCommitCallBack commitCallBack = new KafkaCommitCallBack(onCommitCb);
        		consumer.commitAsync(commitMap, commitCallBack);
        	}catch(Exception e){
        		ret = MqError.ERR_SDK;
        	}
        }else{
        	try{
        		consumer.commitSync(commitMap);
        	}catch(Exception e){
        		ret = MqError.ERR_SDK;
        	}
        }
        return ret;
	}
	
	/**
	 * 修改接收器的消息偏移地址，使下次拉取消息从此位置开始，
	 * 只对精密接收器(PreciseReciver)有效,当autocommit为true时，
	 * 会和seek相互冲突，导致接收数据位置不可控。
	 * 同步接口，使用前提是autocommit为false<br>
	 * <b>警告:会导致数据重复！</b>需用户自己处理重复的数据
	 * @param topic			要提交的topic名称
	 * @param partition		要提交的分区号
	 * @param offset		要提交的消息偏移地址，
	 * @param async		异步执行标志位
	 * @return 
	 * @since 2019年3月5日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.receiver.IMqReceiver#seek(java.lang.String, int, int, boolean)
	 */
	public MqError seek(String topic, int partition, long offset) {
		if(server.getServerState() != IMqInterface.MQ_SERVER_ONLINE ){
			return MqError.ERR_SERVER_DOWN;
		}
		if(!preciseReceiver){
			//非精密接收器，不可使用commit接口,设置了自动提交，也不可使用此接口
			return MqError.ERR_WRONG_LOGIC;
		}
		boolean find = false;
        for (String ownTopic : topics) {
            if (ownTopic.equals(topic)){
            	find = true;
            	break;
            }
        }
        if(!find){
        	return MqError.ERR_INVALID_PARAM;
        }
        TopicPartition tp = new TopicPartition(topic, partition);
        MqError ret = MqError.ERR_NO;
    	try{
    		consumer.seek(tp, offset);
    	}catch(Exception e){
    		ret = MqError.ERR_SDK;
    	}
        return ret;
	}
	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#GetName()
	 */
	public String getName() {
		return name;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#GetGroup()
	 */
	public String getGroup() {
		return group;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#GetTopics()
	 */
	public String[] getTopics() {
		return topics;
	}

	/* (non-Javadoc)
	 * @see com.kedacom.cimsv2.mqinterface.receiver.IMqReceiver#GetMsgTimeout()
	 */
	public int getMsgTimeout() {
		return msgTimeOut;
	}
	/**
	 * 检测当前接收器是否应处于接收状态
	 * @return 
	 * @since 2019年11月1日
	 * @author 刘春光
	 */
	public boolean isOnReceive() {
		return receiveFlag;
	}

	/* (non-Javadoc)
	 * @see org.apache.kafka.clients.consumer.ConsumerRebalanceListener#onPartitionsRevoked(java.util.Collection)
	 */
	public void onPartitionsRevoked(Collection<TopicPartition> partitions) {

		
	}
	/* (non-Javadoc)
	 * @see org.apache.kafka.clients.consumer.ConsumerRebalanceListener#onPartitionsAssigned(java.util.Collection)
	 */
	public void onPartitionsAssigned(Collection<TopicPartition> partitions) {
		if(startOffset == IMqReceiver.FROM_BEGINING){
			consumer.seekToBeginning(partitions);
		}else if(startOffset == IMqReceiver.FROM_ENDING){
			consumer.seekToEnd(partitions);
		}else if(startOffset != IMqReceiver.FROM_DEFAULT){
			for(TopicPartition partition:partitions){
				consumer.seek(partition, startOffset);
			}
		}
		//consumer.commitSync();
		
	}
	class ConsumerPoller implements Runnable {
		private AtomicBoolean closed ;
		private IMqOnMessage<K,V> messageCb;
		private IMqOnEvent   eventCb;
		private int nMsgTimeout;                //消息拉取时的超时时间
		private boolean preciseReceiver = false;
		private IMqOnPreciseMessage<K,V> preciseMessageCb;
		public ConsumerPoller(MqReceiverParam<K,V> recvParam){
			preciseReceiver = false;
			this.closed = new AtomicBoolean(false);
			this.messageCb = recvParam.IMessageCb;
			this.eventCb = recvParam.IEventCb;
			this.nMsgTimeout = recvParam.nMsgTimeout;
			if(this.nMsgTimeout <= 0){
				this.nMsgTimeout = 1000;
			}
		}
		public ConsumerPoller(MqPreciseReceiverParam<K,V> recvParam){
			preciseReceiver = true;
			this.closed = new AtomicBoolean(false);
			this.preciseMessageCb = recvParam.IMessageCb;
			this.eventCb = recvParam.IEventCb;
			this.nMsgTimeout = recvParam.nMsgTimeout;
			if(this.nMsgTimeout <= 0){
				this.nMsgTimeout = 1000;
			}
		}
		public void run() {
			StringBuilder logSb = new StringBuilder();
			server.printLog(EnumMqLogLevel.MQ_LOG_NORMAL, "consumer["+name+"] start poll message");
			try {
				while (!closed.get()) {
					if(null == consumer){Thread.sleep(nMsgTimeout);}
					try{
						ConsumerRecords<K,V> records = consumer.poll(nMsgTimeout);
						
						// Handle new records
						for (ConsumerRecord<K, V> record : records){
							if(IsmpGlobalUtil.DEBUG_FLAG){
								logSb.delete(0, logSb.length());
								//计算时间戳
								logSb.append("MqKafkaReceiver::onMessage[");
								SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
								logSb.append(formatter.format(new Date())).append("] ");
								logSb.append(" topic:").append(record.topic());
								logSb.append(" key:").append(record.key());
								logSb.append(" value:").append(record.value());
								System.out.println(logSb.toString());
							}
							if(preciseReceiver){
								if(null != preciseMessageCb){
									boolean hasException = false;
									try{
										MqMessage<K, V> newMsg = new MqMessage<K, V>();
										newMsg.key = record.key();
										newMsg.value = record.value();
										newMsg.topic = record.topic();
										newMsg.timestamp = record.timestamp();
										preciseMessageCb.onPreciseMessage(newMsg, record.partition(), record.offset());
									}catch (Exception e){
										//防止因回掉抛异常导致接收停止
										//当回调函数抛出异常时，跳出当前循环，再次从服务器拉取数据
										hasException = true;
									}
									if(hasException){
										TopicPartition tp = new TopicPartition(record.topic(), record.partition());
								    	try{
								    		consumer.seek(tp, record.offset());
								    		//同时将重定向信息发送至事件回调
								    		if(null != eventCb){
												eventCb.onEvent(MqError.ERR_RECEIVER_MSG_EXCEPTION, "精密接收器的消息回调触发了异常，已将消息["+record.topic()+"-"+ record.partition()+"]重定向回当前位置["+record.offset()+"]");
											}
								    	}catch(Exception e){
								    		//如果seek失败，将异常打印至控制台
								    		e.printStackTrace();
								    		
								    	}
										break;
									}
								}
							}else{
								if(null != messageCb){
									try{
										MqMessage<K, V> newMsg = new MqMessage<K, V>();
										newMsg.key = record.key();
										newMsg.value = record.value();
										newMsg.topic = record.topic();
										newMsg.timestamp = record.timestamp();
										messageCb.onMessage(newMsg);
									}catch (Exception e){
										//防止因回掉抛异常导致接收停止
										e.printStackTrace();
										if(null != eventCb){
											eventCb.onEvent(MqError.ERR_RECEIVER_MSG_EXCEPTION, "接收器的消息回调触发了异常,err="+e.getMessage());
										}
									}
								}
							}
							
							//System.out.printf("offset = %d, key = %s, value = %s%n", record.offset(), record.key(), record.value());
						}
					}catch(WakeupException e){
						// Ignore exception if closing
						server.printLog(EnumMqLogLevel.MQ_LOG_NORMAL, "consumer["+name+"] recv wakeup Exception");
						//if (!closed.get()) throw e;
					}
				}
			} catch (Exception e) {
				//other events happened
				e.printStackTrace();
				if(null != eventCb){
					eventCb.onEvent(MqError.ERR_SDK, e.getMessage());
				}
			} finally {
//            consumer.close();
			}
			server.printLog(EnumMqLogLevel.MQ_LOG_NORMAL, "consumer["+name+"] stop poll message, poll thread exit.");
		}
		
		// Shutdown hook which can be called from a separate thread
		public void shutdown() {
			closed.set(true);
			consumer.wakeup();
		}
		
		public void reset() {
			if(closed.get()){
				closed.set(false);
				//关闭状态下调用wakeup会导致下次调用poll直接进入wakeupexception异常
				//consumer.wakeup();
			}
		}
	}
	class KafkaCommitCallBack implements OffsetCommitCallback{
		private IMqOnCommit onCommitCallBack = null; 
		public KafkaCommitCallBack(IMqOnCommit commitCb){
			onCommitCallBack = commitCb;
		}
		/**
		 * @param offsets
		 * @param exception 
		 * @since 2019年3月5日
		 * @author 刘春光
		 * @see org.apache.kafka.clients.consumer.OffsetCommitCallback#onComplete(java.util.Map, java.lang.Exception)
		 */
		@Override
		public void onComplete(Map<TopicPartition, OffsetAndMetadata> offsets,
				Exception exception) {
			MqError err ;
			if(null == exception){
				err = MqError.ERR_NO;
			}else{
				err = MqError.ERR_SDK;
			}
			if(null != onCommitCallBack){
				for(Map.Entry<TopicPartition, OffsetAndMetadata> offset : offsets.entrySet()){
					onCommitCallBack.onCommit(offset.getKey().topic(), offset.getKey().partition(), offset.getValue().offset(), err);
				}
			}
		}
		
	}
}
