/**
 * 文件名：MqKafka.java 2017年8月24日
 * 版权：Copyright(C) 2003-2017 Suzhou Keda Technology Co., Ltd. All rights reserved.
 * 修改人：刘春光
 * 修改时间： 2017年8月24日
 * 修改内容：创建文件
 */
package com.kedacom.ismp.mqinterface.mqkafka;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;

import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.AdminClientConfig;
import org.apache.kafka.clients.admin.TopicDescription;
import org.apache.kafka.common.KafkaException;
import org.apache.kafka.common.Node;
import org.apache.kafka.common.TopicPartitionInfo;

import com.kedacom.ismp.mqinterface.EnumMqLogLevel;
import com.kedacom.ismp.mqinterface.IMqInterface;
import com.kedacom.ismp.mqinterface.IMqOnLog;
import com.kedacom.ismp.mqinterface.IMqOnServerDown;
import com.kedacom.ismp.mqinterface.MqError;
import com.kedacom.ismp.mqinterface.MqServerInfo;
import com.kedacom.ismp.mqinterface.receiver.IMqReceiver;
import com.kedacom.ismp.mqinterface.receiver.MqPreciseReceiverParam;
import com.kedacom.ismp.mqinterface.receiver.MqReceiverParam;
import com.kedacom.ismp.mqinterface.sender.IMqSender;
import com.kedacom.ismp.mqinterface.sender.MqSenderParam;
import com.kedacom.ismp.mqinterface.mqkafka.receiver.MqKafkaReceiver;
import com.kedacom.ismp.mqinterface.mqkafka.sender.MqKafkaSender;

/**
 * @description
 * @since 2017年8月24日
 * @author 刘春光
 * @version 1.0
 * @date 2017年8月24日
 */
public class MqKafka implements IMqInterface {
	private static int serverIndex = 1;		//静态变量，用于标识程序中mqserver连接的起始序号
	
	private int curIndex = 0;		//标识当前server的唯一id，用于生成clientid
	private Timer aliveTimer; // 定时器，用于定时刷新服务的broker节点，刷新失败会抛出服务掉线事件
	private IMqOnLog onLogCb = null;
	private IMqOnServerDown downCb = null;
	private String serverName = "";
	private MqServerInfo serverInfo[];
	private String serverUserName;
	private String serverPassword;
	private int timeout = IMqInterface.MQ_DEFAULT_TIMEOUT;
	private int curState = MQ_SERVER_OFFLINE;
	private boolean inited = false;
	private AdminClient serverClient = null;
	private List<BrokerNode> serverNodes;
	private Set<MqKafkaSender> senderSet;
	private Set<MqKafkaReceiver> receiverSet;

	/**
	 * kafka 服务连接的有参构造函数，使用此
	 * 构造函数，可以不调用接口中的init操作，构造函数自动初始化
	 * @description 构造函数
	 * @param serverName 当前连接对象的名称，用于多实例时的对象识别
	 * @param logCb
	 *            日志输出回调，不能使用cims的log库
	 * @throws 无
	 * @author 刘春光
	 * @see com.kedacom.cimsv2.mqinterface.IMqInterface#init(String,IMqOnLog,IMqOnServerDown)
	 */
	public MqKafka(String serverName, IMqOnLog logCb, IMqOnServerDown downCb) {
		curIndex = serverIndex++;
		this.serverName = serverName;
		onLogCb = logCb;
		this.downCb = downCb;
		inited = true;
		curState = MQ_SERVER_OFFLINE;
		timeout = IMqInterface.MQ_DEFAULT_TIMEOUT;
		senderSet =  new HashSet<MqKafkaSender>();
		receiverSet = new HashSet<MqKafkaReceiver>();
		serverNodes = new ArrayList<BrokerNode>();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.kedacom.cimsv2.mqinterface.IMqInterface#GetErrDesc(int)
	 */
	public String getErrDesc(MqError errorCode) {
		switch (errorCode) {
		case ERR_NO:
			return "no err";
		case ERR_UNKNOWN:
			return "unknown err";
		case ERR_SDK:
			return "sdk err";
		case ERR_HANDLE_NOTFOUND:
			return "handle not found";
		case ERR_HANDLE_ALREADY_EXISTS:
			return "handle already exists";
		case ERR_INVALID_PARAM:
			return "invalid param";
		case ERR_WRONG_LOGIC:
			return "wrong logic";
		case ERR_TIMEOUT:
			return "timeout";
		case ERR_SERVER_DOWN:
			return "server down";
		case ERR_RECEIVER_OUTDATA:
			return "receiver data timeout";
		case ERR_RECEIVER_MSG_EOF:
			return "receiver got EOF";
		default:
			return "unknown error code";
		}
	}

	/**
	 * @description 设置服务器连接状态，用于sender或 receiver检测到服务器掉线后通知服务连接类
	 * @param curState
	 *            新的服务器状态
	 * @throws 无
	 * @since 2017年9月6日
	 * @author 刘春光
	 * @date 2017年9月6日
	 */
	public void setCurState(int curState) {
		if(this.curState == curState){
			return;
		}
		this.curState = curState;
		if(curState == MQ_SERVER_OFFLINE && null != downCb){
			downCb.onServerDown(this);
		}
	}

	/**
	 * @description 获取服务器当前连接状态
	 * @return IMqInterface.MQ_SERVER_OFFLINE或MQ_SERVER_ONLINE
	 * @throws null
	 * @since 2017年9月6日
	 * @author 刘春光
	 * @see com.kedacom.cimsv2.mqinterface.IMqInterface#getServerState()
	 */
	public int getServerState() {
		return curState;
	}

	public boolean init(String serverName, IMqOnLog logCb, IMqOnServerDown downCb) {
		this.serverName = serverName;
		onLogCb = logCb;
		inited = true;
		return false;
	}

	public MqError loginServer(MqServerInfo serverInfoArr[], String username,
			String password, int nTimeout) {
		if (!inited) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR,
					"loginserver failed, mq not inited!");
			return MqError.ERR_SERVER_DOWN;
		}
		curState = MQ_SERVER_OFFLINE;
		serverInfo = serverInfoArr;
		serverUserName = username;
		serverPassword = password;
		timeout = nTimeout;
		MqError eResult = connectServer();
		return eResult;
	}
	
	private MqError connectServer(){
		MqError eResult = MqError.ERR_NO;
		try {
			if(null != serverClient){
				serverClient.close(MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
				serverClient = null;
			}
			// 使用传入的原始参数拼接地址字符串
			String serverAddr = getServerAddress();
			//设置consumer参数
			Properties props = new Properties();
			props.put(AdminClientConfig.BOOTSTRAP_SERVERS_CONFIG, serverAddr);
			props.put(AdminClientConfig.CLIENT_ID_CONFIG, "mqkafka-"+curIndex);
			serverClient = AdminClient.create(props);
			List<BrokerNode> brokers = acquireBrokers();
			if (0 == brokers.size()) {
			    serverClient.close(0, TimeUnit.MICROSECONDS);
				// 查找broker结点失败
				printLog(EnumMqLogLevel.MQ_LOG_ERROR,
						"login server ERROR: find brokers failed!");
				return MqError.ERR_SERVER_DOWN;
			}
			setServerNodes(brokers);
			// 开启broker刷新定时器
			aliveTimer = new Timer();
			TimerTask aliveTask = new KeepAliveTask(this);
			// 每隔10s刷新一下brokers数据，并借此判断服务器状态是否正常
			aliveTimer.schedule(aliveTask, 10 * 1000, 10 * 1000);
			curState = MQ_SERVER_ONLINE;
		} catch (KafkaException ek) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR,
					"loginserver ERROR,kafka error:%s", ek.getMessage());
			ek.printStackTrace();
			eResult = MqError.ERR_SDK;
		} catch (Exception e) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR,
					"LoginServer catch exceptions:%s", e.getMessage());
			e.printStackTrace();
			eResult = MqError.ERR_UNKNOWN;
			throw e;
		}
		return eResult;
	}

	public MqError logoutServer() {
		if (!inited || curState != MQ_SERVER_ONLINE) {
			return MqError.ERR_SERVER_DOWN;
		}
		// 删除创建的consumer和producer 使用迭代器会触发异常，所以转换成数组操作
		MqKafkaSender[] senderArr = senderSet.toArray(new MqKafkaSender[0]);
		for(int i = 0; i < senderArr.length; ++i) {
			senderArr[i].closeSender();
		}
		MqKafkaReceiver[] receiverArr = receiverSet.toArray(new MqKafkaReceiver[0]);
		for(int i = 0; i < receiverArr.length; ++i) {
			receiverArr[i].closeReceiver();
		}
		if(null != serverClient){
		    serverClient.close(0, TimeUnit.MICROSECONDS);
			serverClient = null;
		}
		senderSet.clear();
		receiverSet.clear();
		aliveTimer.cancel();
		curState = MQ_SERVER_OFFLINE;
		return MqError.ERR_NO;
	}
	@SuppressWarnings("rawtypes")
	public void removeReceiver(MqKafkaReceiver receiver){
		Iterator<MqKafkaReceiver> iteReceiver = receiverSet.iterator();
		while (iteReceiver.hasNext()) {
			MqKafkaReceiver curReceiver = iteReceiver.next();
			if(receiver == curReceiver){
				iteReceiver.remove();
				break;
			}
		}
	}
	@SuppressWarnings("rawtypes")
	public void removeSender(MqKafkaSender sender){
		Iterator<MqKafkaSender> iteSender = senderSet.iterator();
		while (iteSender.hasNext()) {
			MqKafkaSender curSender = iteSender.next();
			if(sender == curSender){
				iteSender.remove();
				break;
			}
		}
	}

	public void printServerData() {
		try {
			int i = 0;
			int j = 0;
			List<BrokerNode> serverNodes = acquireBrokers();
			printLog(EnumMqLogLevel.MQ_LOG_DEBUG,
					"PrintServerData brokers num:%d \n", serverNodes.size());
			/* Iterate brokers */
			for (i = 0; i < serverNodes.size(); i++) {
				printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "\tbroker:%d at %s:%d\n",
						serverNodes.get(i).id, serverNodes.get(i).host,
						serverNodes.get(i).port);
			}
			/* topics */
			try {
				Set<String> topics = serverClient.listTopics().names().get(MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
				printLog(EnumMqLogLevel.MQ_LOG_DEBUG, " topics num:%d\n",
						topics.size());
				Map<String, TopicDescription> topicsMap = serverClient.describeTopics(topics).all().get(MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
				for(Map.Entry<String,TopicDescription> topic : topicsMap.entrySet()){
					List<TopicPartitionInfo> partitions = topic.getValue().partitions();
					printLog(EnumMqLogLevel.MQ_LOG_DEBUG,
							"\ttopic:key:%s name:%s with %d partitions:\n", topic.getKey(),
							topic.getValue().name(), partitions.size());
					String topicInfo = new String();
					for(TopicPartitionInfo part: partitions){
						topicInfo = String.format("%s\t\tpartition:%d, leader:%d,replicas:", 
								topicInfo, part.partition(), part.leader().id());
						List<Node> replicationBrokers = part
								.replicas();
						for (int k = 0; k < replicationBrokers.size(); k++){
							if(k > 0)topicInfo = topicInfo.concat(",");
							topicInfo = topicInfo.concat(String.valueOf(replicationBrokers.get(k).id()));
						}
						/* Iterate partition's ISRs */
						List<Node> isrsBrokers = part.isr();
						topicInfo = topicInfo.concat(", isrs: ");
						for (int k = 0; k < isrsBrokers.size(); k++){
							if(k > 0)topicInfo = topicInfo.concat(",");
							topicInfo = topicInfo.concat(String.valueOf(isrsBrokers.get(k).id()));
						}
						topicInfo = topicInfo.concat("\n");
					}
					printLog(EnumMqLogLevel.MQ_LOG_DEBUG, topicInfo);
				}
			} catch (Exception e) {
				throw e;
			} finally {
			}
		} catch (Exception e) {
			printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "ERROR: %s\n", e.getMessage());
		}
	}

	public MqError reConnectServer(int nTimeout) {
		//先断开所有连接
		Iterator<MqKafkaSender> iteSender = senderSet.iterator();
		while (iteSender.hasNext()) {
			MqKafkaSender<?, ?> sender = iteSender.next();
			sender.disConnect();
		}
		Iterator<MqKafkaReceiver> iteReceiver = receiverSet.iterator();
		while (iteReceiver.hasNext()) {
			MqKafkaReceiver<?, ?> receiver = iteReceiver.next();
			receiver.disConnect(true);
		}
		//连接服务器
		MqError conError = connectServer();
		if(MqError.ERR_NO != conError){
			printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "mqkafka服务重连失败！err="+ conError.name());
			return conError;
		}
		// ConnectServer的consumer和producer
		iteSender = senderSet.iterator();
		while (iteSender.hasNext()) {
			MqKafkaSender<?, ?> sender = iteSender.next();
			sender.connect();
			printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "mqkafka sender["+ sender.getName()+"] 开始重连");
		}
		iteReceiver = receiverSet.iterator();
		while (iteReceiver.hasNext()) {
			MqKafkaReceiver<?, ?> receiver = iteReceiver.next();
			receiver.connect();
			printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "mqkafka receiver["+ receiver.getName()+"] 开始重连");
			if(receiver.isOnReceive()){
				printLog(EnumMqLogLevel.MQ_LOG_DEBUG, "mqkafka receiver["+ receiver.getName()+"] 启动重新接收");
				receiver.startReceive(IMqReceiver.FROM_DEFAULT);
			}
		}
		return MqError.ERR_NO;
	}

	public List<BrokerNode> acquireBrokers() {
		List<BrokerNode> retList = new ArrayList<BrokerNode>();
		try{
			Collection<Node> nodes = serverClient.describeCluster().nodes().get(MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
			for(Node node : nodes){
				BrokerNode broker = new BrokerNode();
				broker.host = node.host();
				broker.id = node.id();
				broker.port = node.port();
				broker.rack = node.rack();
				retList.add(broker);
			}
		}catch(Exception e){
			
		}
		return retList;
	}
	public void setServerNodes(List<BrokerNode> newBrokerList) {
		//策略更改，只在服务启动时获取服务器节点
		if (serverNodes.isEmpty() && null != newBrokerList && 0 != newBrokerList.size()) {
			serverNodes.addAll(newBrokerList);
		}
	}
	/** 拼接server地址串 */
	public String getServerAddress() {
		String addr = new String();
		if (serverNodes.isEmpty()) {
			// 根据服务器地址参数拼接
			for (int i = 0; i < serverInfo.length; ++i) {
				if (0 != i) {
					addr = addr.concat(",");
				}
				addr = addr.concat(serverInfo[i].getServerHost());
				addr = addr.concat(":");
				addr = addr.concat(Integer.toString(serverInfo[i]
						.getServerPort()));
			}
			return addr;
		}
		for (int i = 0; i < serverNodes.size(); ++i) {
			BrokerNode curNode = serverNodes.get(i);
			if (0 != i) {
				addr = addr.concat(",");
			}
			addr = addr.concat(curNode.host);
			addr = addr.concat(":");
			addr = addr.concat(Integer.toString(curNode.port));
		}
		return addr;
	}

	public String[] getAllTopics(int nTimeout) {
		List<String> retStrList = new ArrayList<String>();
		try {
			Set<String>topicNames = serverClient.listTopics().names().get(MQ_DEFAULT_TIMEOUT, TimeUnit.MILLISECONDS);
			// 过滤掉存储consumer offset的topic
			for (String name : topicNames) {
				if ("__consumer_offsets".equals(name)) {
					continue;
				}
				retStrList.add(name);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return retStrList.toArray(new String[0]);
	}

	@SuppressWarnings("unchecked")
	public <K, V> IMqReceiver<K, V> createReceiver(String topic[],
			MqReceiverParam<K, V> recvParam, int nTimeout) {
		MqKafkaReceiver<K, V> receiver = null;
		try {
			receiver = new MqKafkaReceiver<K, V>(this, topic, recvParam,
					nTimeout);
		} catch (Exception e) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR,
					"create receiver failed!Error:%s", e.getMessage());
			e.printStackTrace();
			return null;
		}
		// 故意劣化成object是为了能让set存储所有的receiver，而不论类型
		receiverSet.add((MqKafkaReceiver<Object, Object>) receiver);
		return receiver;
	}
	/**
	 * 
	 * @description 创建一个精密接收器(PreciseReceiver)
	 * @param topic 要订阅的topic数组
	 * @param recvParam	精密接收器参数
	 * @param nTimeout 超时时间
	 * @return IMqReceiver 创建的实际receiver对象
	 * @throws  Exception
	 * @since 2017年8月23日
	 * @author 刘春光
	 */
	public <K,V> IMqReceiver<K,V> createPreciseReceiver(String topic[], 
			MqPreciseReceiverParam<K,V> recvParam, int nTimeout) throws Exception{
		MqKafkaReceiver<K, V> receiver = null;
		try {
			receiver = new MqKafkaReceiver<K, V>(this, topic, recvParam,
					nTimeout);
		} catch (Exception e) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR,
					"create receiver failed!Error:%s", e.getMessage());
			e.printStackTrace();
			return null;
		}
		// 故意劣化成object是为了能让set存储所有的receiver，而不论类型
		receiverSet.add((MqKafkaReceiver<Object, Object>) receiver);
		return receiver;
	}
	@SuppressWarnings("unchecked")
	public <K, V> IMqSender<K, V> createSender(String topic,
			MqSenderParam<K, V> senderParam, int nTimeout) {
		MqKafkaSender<K, V> sender = null;
		try {
			sender = new MqKafkaSender<K, V>(this, topic, senderParam, nTimeout);
		} catch (Exception e) {
			printLog(EnumMqLogLevel.MQ_LOG_ERROR, "create sender failed!Error:%s",
					e.getMessage());
			return null;
		}
		// 故意劣化成object是为了能让set存储所有的sender，而不论类型
		senderSet.add((MqKafkaSender<Object, Object>) sender);
		return sender;
	}

	public void printLog(EnumMqLogLevel eLevel, String fromat, Object... args) {
		if (null != onLogCb) {
			onLogCb.onLog(eLevel, fromat, args);
		}
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.IMqInterface#getServerName()
	 */
	@Override
	public String getServerName() {
		return serverName;
	}

	/**
	 * @description 
	 * @return 
	 * @throws  
	 * @since 2017年12月13日
	 * @author 刘春光
	 * @see com.kedacom.ismp.mqinterface.IMqInterface#getServerInfo()
	 */
	@Override
	public MqServerInfo[] getServerInfo() {
		return serverInfo;
	}
}

class KeepAliveTask extends TimerTask {
	MqKafka server = null;
	int disCounter = 0;
	public KeepAliveTask(MqKafka server) {
		this.server = server;
	}

	public void run() {
		List<BrokerNode> brokers = null;
		try {
			brokers = server.acquireBrokers();
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (null != brokers && brokers.size() != 0) {
			disCounter = 0;
		} else {
			++disCounter;
			//连续3次获取server信息失败，确认断链
			if(2 < disCounter)
			{
				// 查找broker结点失败
				server.printLog(EnumMqLogLevel.MQ_LOG_ERROR,
						"KeepAliveTask: server ERROR: find brokers failed!");
				server.setCurState(IMqInterface.MQ_SERVER_OFFLINE);
				this.cancel();
			}
		}
	}
}

class BrokerNode {
	String host;
	int port;
	int id;
	String rack;
}
