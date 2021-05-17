wifi.setmode(wifi.STATIONAP)
wifi.sta.config("pdr_wdnmd","qwertyuiop")
--wifi.sta.connect()
wifi.sta.autoconnect(1)
tmr.alarm(2, 1000, 1, function()
     if wifi.sta.getip() == nil then
        print("WiFi Connecting...")
     else
        print("WiFi Connected, IP is ".. wifi.sta.getip())
        tmr.stop(2)
     end
end)

--uart.setup(0,9600,8,0,1,0)

MqttUserString = "Esp8266_Node";
MqttPwdString = "pass";
MqttIPString = "47.107.145.82";
MqttPort = 1883;
clientid = "GunQiu_ESP8266_Node"
SubscribeTopic = "GunQiu/Esp"
PublishTopic = "GunQiu/Web"
mqttConnectedFlage = 0;

client = mqtt.Client(clientid, 0, MqttUserString, MqttPwdString)

tmr.alarm(3, 5000, 1, function()
    client:connect(MqttIPString, MqttPort,ConnectSuccess,ConnectFailed)
    print("MQTT Connecting...");
end)

function ConnectSuccess(client)
     tmr.stop(3);
     client:subscribe(SubscribeTopic, 0, subscribeSuccess)
     print("MQTT Connected")
     mqttConnectedFlage = 1;
end

function ConnectFailed(client,reason)
   mqttConnectedFlage = 0;
   tmr.start(3)
   print("MQTT Failed Reason: " .. reason)
end

function subscribeSuccess(client)
  print("subscribeSuccess")
end

client:on("message", function(client, topic, message)
  if message ~= nil then
    uart.write(0, message)
  end
end)

client:on("offline", function(client)
    print ("offline, reconnect now")
    tmr.start(3)
end)

uart.on("data", 2,
  function(data)
    if mqttConnectedFlage == 1 then
       client:publish(PublishTopic, data, 0,0)
    end
end, 0)

--tmr.alarm(4, 5000, 1, function()
--    if mqttConnectedFlage == 1 then
--       client:publish(PublishTopic, "Hello From ESP8266", 0,0)
--    end
--end)
