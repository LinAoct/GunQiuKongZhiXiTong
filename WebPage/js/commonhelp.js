// json字符串转hex
var stringtoHex = function (str) {
    var val = "";
    var hex = "";
    var s = new ArrayBuffer(str.length/2);
    var s = new Uint8Array(s);
    var index = 0;
    for (var i = 0; i < str.length; i++) {
        if(str.charCodeAt(i)==48)
            val += "0000";
        if(str.charCodeAt(i)>48 && str.charCodeAt(i)<=57)
            val += (str.charCodeAt(i)-48).toString(2);
        if(str.charCodeAt(i)>=65 && str.charCodeAt(i)<=70)
            val += (str.charCodeAt(i)-55).toString(2);
        if(str.charCodeAt(i)>=97 && str.charCodeAt(i)<=102)
            val += (str.charCodeAt(i)-87).toString(2);
        if(val.length == 8){
            hex += bin_to_hex(val);
            s[index] = '0x'+bin_to_hex(val);
            index+=1;
            val = "";
        }
    }
    return s
}

function bin_to_hex(str) {
    let hex_array = [{key:0,val:"0000"},{key:1,val:"0001"},{key:2,val:"0010"},{key:3,val:"0011"},{key:4,val:"0100"},{key:5,val:"0101"},{key:6,val:"0110"},{key:7,val:"0111"},
        {key:8,val:"1000"},{key:9,val:"1001"},{key:'a',val:"1010"},{key:'b',val:"1011"},{key:'c',val:"1100"},{key:'d',val:"1101"},{key:'e',val:"1110"},{key:'f',val:"1111"}]
    let value = ''
    let list=[]
    if(str.length%4!==0){
        let a = "0000"
        let b=a.substring(0,4-str.length%4)
        str = b.concat(str)
    }
    while (str.length > 4) {
        list.push(str.substring(0, 4))
        str = str.substring(4);
    }
    list.push(str)
    for(let i=0;i<list.length;i++){
        for(let j=0;j<hex_array.length;j++){
            if(list[i]==hex_array[j].val){
                value = value.concat(hex_array[j].key)
                break
            }
        }
    }
    return value
}

Date.prototype.Format = function (fmt) {
    var o = {
        "M+": this.getMonth() + 1, //月份 
        "d+": this.getDate(), //日 
        "h+": this.getHours(), //小时 
        "m+": this.getMinutes(), //分 
        "s+": this.getSeconds(), //秒 
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度 
        "S": this.getMilliseconds() //毫秒 
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[
            k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}