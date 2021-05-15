var hostname = '47.107.145.82',
    port = 61614,
    clientId = 'GunQiu_Web_' + Math.random().toString().substring(3,6),
    timeout = 120,
    keepAlive = 100,
    cleanSession = false,
    ssl = false,
    userName = 'GunQiu_Web',
    password = 'pass',
    publishTopic = "GunQiu/Esp",
    subscribeTopic = "GunQiu/Web",
    isConnected = false;

client = new Paho.Client(hostname, port, clientId); //建立客户端实例 

var options = {
    invocationContext: {
        host: hostname,
        port: port,
        path: client.path,
        clientId: clientId
    },
    timeout: timeout,
    keepAliveInterval: keepAlive,
    cleanSession: cleanSession,
    useSSL: ssl,
    userName: userName,  
    password: password,  
    onSuccess: onConnect,
    onFailure: function (e) {
        console.log(e);
        s = "{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", onFailure()}";
        console.log(s);
    }
};

client.connect(options);                            //连接服务器

//连接服务器并注册连接成功处理事件  
function onConnect() {
    console.log("onConnected");
    s = "{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", onConnected()}";
    console.log(s);
    client.subscribe(subscribeTopic);
    document.getElementById("mqtt-state").style.backgroundColor = "#00D87C";
    document.getElementById("mqtt-state-label").innerText = "MQTT已连接";
    isConnected = true;
}

client.onConnectionLost = onConnectionLost; //注册连接断开处理事件  
client.onMessageArrived = onMessageArrived; //注册接收到信息处理事件

//注册消息接收处理事件  
function onConnectionLost(responseObject) {
    console.log(responseObject);
    s = "{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", onConnectionLost()}";
    console.log(s);
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:" + responseObject.errorMessage);
        console.log("连接已断开,准备重连");
        document.getElementById("mqtt-state").style.backgroundColor = "#FC8472";
        document.getElementById("mqtt-state-label").innerText = "MQTT连接断开";
        isConnected = false;
        client.connect(options);
    }
}

function onMessageArrived(message) {
    s = "{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", onMessageArrived()}";
    console.log(s);
    if(message.payloadString){
        console.log("收到消息String:" + message.payloadString);
    }
    if(message.payloadBytes){
        console.log("收到消息Bytes:" + message.payloadBytes);
    }
}

function connectManual() {
    if(!isConnected)                //判断当前是否已建立连接
        client.connect(options);    //连接服务器
}

function sendCommand() {
    var isHex = document.getElementById("checkbox-hex").checked;
    var str = document.getElementById("command").value
    if (str) {
        if(isHex)
            str = stringtoHex(str.replace(/\s*/g,""));
        message = new Paho.Message(str);
        message.destinationName = publishTopic;
        client.send(message);
        console.log("{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", content:" + (str) + ", topic:" + (publishTopic) + "}");
    }
}

function mqttSendBytes(bytes) {
    message = new Paho.Message(bytes);
    message.destinationName = publishTopic;
    client.send(message);
    console.log("{time:" + new Date().Format("yyyy-MM-dd hh:mm:ss") + ", content:" + (bytes) + ", topic:" + (publishTopic) + "}");
}

function sendFunctionValue(value) {
    var bytedata = new ArrayBuffer(3);
    var bytedata = new Uint8Array(bytedata);
    bytedata[0] = 0xFF;
    bytedata[1] = 0xFE;
    bytedata[2] = "0x"+value;
    mqttSendBytes(bytedata);
}
