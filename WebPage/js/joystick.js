$(function () {
    let mousedown = false;
    let img1 = document.getElementById('smallcircuit');
    let img_point = document.getElementById('numpoint');
    let mycanvas = document.getElementById('canvas');
    let ctx = mycanvas.getContext('2d');

    let POS_X = 50; //因为触控区域肯定比较大，但是真正空间区域比较小，所有要有一个位置参数
    let POS_Y = 50;
    let Canvas_Width = 300;
    let Canvas_Height = 300;
    let SAMLL_CIRCUIT_R = 8; //小圆半径
    let BIG_CIRCUIT_R = 100;//大圆半径
    let output = {}; //摇杆输出值， -1000~1000 -1000~1000 这样好统一！！！

    var x,y;
    var send_spacing_count = 0;

    var bytedata = new ArrayBuffer(5);
    var bytedata = new Uint8Array(bytedata);
    bytedata[0] = 0xFF;
    bytedata[1] = 0xFE;
    bytedata[2] = 0xFD;

    mycanvas.onmousedown = dragstart_event;
    mycanvas.onmousemove = dragging_event;
    mycanvas.onmouseup = dragstop_event;

    function init() {
        joystickdraw(Canvas_Width/2,Canvas_Height/2);   //注意该参数是不考虑小圆R
    }
    /*
    x，y，输出值：都是以大圆圆心为零点的值
    输出是限制在大圆半径内的 xy组合
     */
    function getXY(x, y) {
        let mouse2centerlength = Math.sqrt(x * x + y * y);
        let newx = x;
        let newy = y;
        if (mouse2centerlength > BIG_CIRCUIT_R) {   
            let proportion = mouse2centerlength / BIG_CIRCUIT_R;
            newx = x / proportion;
            newy = y / proportion;
        }
        return {x: newx, y: newy};
    }

    function joystickdraw(posx, posy) {
        //清除画面
        ctx.clearRect(0, 0, 600, 600);
        //保存转态
        ctx.save();
        //绘底图
        // ctx.drawImage(img2, POS_X, POS_Y, BIG_CIRCUIT_R * 2, BIG_CIRCUIT_R * 2);
        ctx.drawImage(img1, 45, 45, 30, 30);
        ctx.drawImage(img1, 135, 45, 30, 30);
        ctx.drawImage(img1, 225, 45, 30, 30);
        ctx.drawImage(img1, 45, 135, 30, 30);
        ctx.drawImage(img1, 135, 135, 30, 30);
        ctx.drawImage(img1, 225, 135, 30, 30);
        ctx.drawImage(img1, 45, 225, 30, 30);
        ctx.drawImage(img1, 135, 225, 30, 30);
        ctx.drawImage(img1, 225, 225, 30, 30);

        //根据鼠标位置平移
        //let pos = getXY(posx - POS_X - BIG_CIRCUIT_R, posy - POS_Y - BIG_CIRCUIT_R);  //这里去掉偏移值
        //console.log(pos);

        //console.log(output);
        //ctx.translate(pos.x + POS_X + BIG_CIRCUIT_R - SAMLL_CIRCUIT_R, pos.y + POS_Y + BIG_CIRCUIT_R - SAMLL_CIRCUIT_R);
        //画上球
        ctx.drawImage(img_point, posx-SAMLL_CIRCUIT_R, posy-SAMLL_CIRCUIT_R, SAMLL_CIRCUIT_R * 2, SAMLL_CIRCUIT_R * 2);
        
        $('#xvalue').text(parseInt(posx/Canvas_Width*240)+","+parseInt(posy/Canvas_Height*240));
        //还原状态
        ctx.restore();
    }

    //XY值修正
    function xyValueLimit() {
        if(x<0)
            x=-x;
        if(y<0)
            y=-y;
        if(x>Canvas_Width)
            x = Canvas_Width;
        if(y>Canvas_Height)
            y = Canvas_Height;
    }

    function dragstart_event(e) {
        mousedown = true;
        x = e.offsetX;
        y = e.offsetY;
        xyValueLimit();
        joystickdraw(x, y);
        sendPointValue(x, y);
    }

    function dragging_event(e) {
        if (mousedown) {
            x = e.offsetX;
            y = e.offsetY;
            xyValueLimit();
            if(send_spacing_count>=10) {
                send_spacing_count=0;
                sendPointValue(x, y);
            }
            joystickdraw(x, y)
            send_spacing_count+=1;
        }
    }

    function dragstop_event() {
        mousedown = false;
    }

    mycanvas.addEventListener('touchstart', function (e) {
        mousedown = true;
        console.log("touchstart");
        x = parseInt(e.changedTouches[0].clientX - mycanvas.offsetLeft);
        y = parseInt(e.changedTouches[0].clientY - mycanvas.offsetParent.offsetTop);
        xyValueLimit();
        sendPointValue(x, y);
        joystickdraw(x, y);
    }, false)
    
    mycanvas.addEventListener('touchmove', function (e) {
        if (mousedown) {
            e.preventDefault();
            console.log("touchmove");
            if(e.changedTouches[0].clientX<mycanvas.offsetLeft)
                x=0;
            else if(e.changedTouches[0].clientX>mycanvas.offsetLeft+Canvas_Width)
                x=Canvas_Width;
            else x = parseInt(e.changedTouches[0].clientX - mycanvas.offsetLeft);

            if(e.changedTouches[0].clientY<mycanvas.offsetParent.offsetTop)
                y=0;
            else if(e.changedTouches[0].clientY>mycanvas.offsetParent.offsetTop+Canvas_Height)
                y=Canvas_Height;
            else y = parseInt(e.changedTouches[0].clientY - mycanvas.offsetParent.offsetTop);

            xyValueLimit();
            if(send_spacing_count>=10) {
                send_spacing_count=0;
                sendPointValue(x, y);
            }
            joystickdraw(x, y);
            send_spacing_count+=1;
        }
    }, false)

    mycanvas.addEventListener('touchend', function (e) {
        mousedown = false;
    }, false)

    function sendPointValue(x, y) {
        bytedata[3] = parseInt(x/Canvas_Width*240);
        bytedata[4] = parseInt(y/Canvas_Height*240);
        mqttSendBytes(bytedata);
    }

    // mycanvas.ontouchstart = touchstart_event;
    // mycanvas.ontouchmove = touching_event;
    // mycanvas.ontouchend = touchstop_event;

    init();
});



// $(function () {
//     function init() {
//         joystickdraw(BIG_CIRCUIT_R + POS_X, BIG_CIRCUIT_R + POS_Y);   //注意该参数是不考虑小圆R
//     }
//     /*
//     x，y，输出值：都是以大圆圆心为零点的值
//     输出是限制在大圆半径内的 xy组合
//      */
//     function getXY(x, y) {

//         let mouse2centerlength = Math.sqrt(x * x + y * y);
//         let newx = x;
//         let newy = y;
//         if (mouse2centerlength > BIG_CIRCUIT_R) {
//             let proportion = mouse2centerlength / BIG_CIRCUIT_R;
//             newx = x / proportion;
//             newy = y / proportion;
//         }
//         return {x: newx, y: newy};
//     }

//     function joystickdraw(posx, posy) {

//         //$('#xvalue').text(posx+","+posy);

//         //这里绘图
//         //清除画面
//         ctx.clearRect(0, 0, 600, 600);
//         //保存转态
//         ctx.save();
//         //绘底图
//         ctx.drawImage(img2, POS_X, POS_Y, BIG_CIRCUIT_R * 2, BIG_CIRCUIT_R * 2);
//         //平移，根据鼠标位置
//         let pos = getXY(posx - POS_X - BIG_CIRCUIT_R, posy - POS_Y - BIG_CIRCUIT_R);  //这里去掉偏移值
//         //console.log(pos);
//         output.x = parseInt(pos.x / BIG_CIRCUIT_R * 1000);
//         output.y = parseInt(pos.y / BIG_CIRCUIT_R * 1000);
//         sendwebsocket(output);
//         var s = new ArrayBuffer(4);
//         var s = new Uint8Array(s);
//         s[0] = 0xFF;
//         s[1] = 0xFE;
//         s[2] = output.x;
//         s[3] = output.y;
//         mqttSendBytes(s);
//         $('#xvalue').text(output.x+","+output.y);

//         //console.log(output);
//         ctx.translate(pos.x + POS_X + BIG_CIRCUIT_R - SAMLL_CIRCUIT_R, pos.y + POS_Y + BIG_CIRCUIT_R - SAMLL_CIRCUIT_R);
//         //画上球
//         ctx.drawImage(img1, 0, 0, SAMLL_CIRCUIT_R * 2, SAMLL_CIRCUIT_R * 2);
//         //还原状态
//         ctx.restore();
//     }

//     function sendwebsocket(pos) {
//         if (ws) {
//             if (ws.readyState === WebSocket.OPEN)
//                 ws.send(JSON.stringify(pos))
//         }
//     }

//     function dragstart_event(e) {
//         mousedown = true;
//         joystickdraw(e.offsetX, e.offsetY)
//     }

//     function dragging_event(e) {
//         if (mousedown) {
//             joystickdraw(e.offsetX, e.offsetY)
//             console.log(e.offsetX, e.offsetY);
//         }
//     }

//     function dragstop_event() {
//         mousedown = false;
//         init();
//     }

//     function touchstart_event(e) {
//         mousedown = true;
//         joystickdraw(e.touches[0].clientX, e.touches[0].clientY)
//     }

//     function touching_event(e) {
//         if (mousedown) {
//             joystickdraw(e.touches[0].clientX, e.touches[0].clientY)
//         }
//     }

//     function touchstop_event() {
//         mousedown = false;
//         init();
//     }

//     let mousedown = false;
//     let ws;
//     let texturl = document.getElementById('url');
//     let btnconnect = document.getElementById('connect');
//     let btndisconnect = document.getElementById('disconnect');
//     let img1 = document.getElementById('smallcircuit');
//     let img2 = document.getElementById('bigcircuit');
//     let mycanvas = document.getElementById('canvas');
//     let ctx = mycanvas.getContext('2d');

//     let POS_X = 50; //因为触控区域肯定比较大，但是真正空间区域比较小，所有要有一个位置参数
//     let POS_Y = 50;
//     let SAMLL_CIRCUIT_R = 25; //小圆半径
//     let BIG_CIRCUIT_R = 100;//大圆半径
//     let output = {}; //摇杆输出值， -1000~1000 -1000~1000 这样好统一！！！

//     mycanvas.onmousedown = dragstart_event;
//     mycanvas.onmousemove = dragging_event;
//     mycanvas.onmouseup = dragstop_event;

//     mycanvas.ontouchstart = touchstart_event;
//     mycanvas.ontouchmove = touching_event;
//     mycanvas.ontouchend = touchstop_event;

//     init();
// });
