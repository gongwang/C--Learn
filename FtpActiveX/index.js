
var player = null;
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/*页面加载与页面退出部分*/
window.onload = function ()
{
            player = document.getElementById("player");
/*
            var nStartPortTemp = player.GetPlayStartPort();
            szStartPort[0].port = nStartPortTemp;
            szStartPort[1].port = nStartPortTemp + 16;
            szStartPort[2].port = nStartPortTemp + 20;
            var nInNat = getUserInnatCookie(nSessionid);
            if(1 == nInNat)
            {
                var nRecPort = getUserNatRcvPortCookie(nSessionid);
                var nNcId = getUserScidCookie(nSessionid);
                var strLocalIp = getUserIpCookie(nSessionid);
                player.StartSendNatPacket(webSeverIP, nRecPort, strLocalIp, szStartPort[0].port, nNcId, 6);   
            }
*/

}
window.onunload = function ()
{
}

function onTest()
{
	var Version = player.FTP_GetVersion();
	 
	var nRet = player.FTP_StartUploadFile("172.16.128.40", 2121, "admin", "admin123", "E:\\svr_ocx_code\\FtpActiveX\\FtpActiveX.sdf", "/usr/mp4pt/tmp/sss");
}
function onNotifyUploadFileStatus(nEvent, nErrorCode, nProgress, strLocalFile, strRemoteFile)
{
	//alert(strLocalFile);
	if(nEvent == 2) // 上传进度
	{
		document.getElementById("id_progress").innerHTML = nProgress;
	}
	if(nEvent == 3) // 上传结束
	{
		alert("上传结束");
	}
}
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/