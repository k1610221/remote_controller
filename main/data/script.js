function constraint_wind() {
    let selected = document.getElementById("mode").value;
    if(selected == 2) { // dry
        document.getElementById("wind").options[0].selected = true;
        for(let i = 1; i < 4; i++) document.getElementById("wind").options[i].hidden = true;
    } else { // warm, cool, fan
        for(let i = 0; i < 4; i++) document.getElementById("wind").options[i].hidden = false;
    }
}

function constraint_temp() {
    let selected = document.getElementById("mode").value;
    if(selected == 0) { // warm
        document.getElementById("temp").options[0].selected = "true";
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "initial";
    } else { // cool, dry, fan
        document.getElementById("temp").options[3].selected = "true";
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "none";
    }
}

function display_detail() {
    let selected = document.getElementById("timer_on").value;
    if(selected == 1) document.getElementById("detail").hidden = false;
    else document.getElementById("detail").hidden = true;
}

function run() {
    let data = "mode=" + document.getElementById("mode").value +"&wind=" + document.getElementById("wind").value +
               "&temp=" + document.getElementById("temp").value + "&timer_on=" + document.getElementById("timer_on").value + 
               "&timer_time=" + document.getElementById("timer_time").value + "&timer_mode=" + document.getElementById("timer_mode").value;
    let request = new XMLHttpRequest();
    request.open("POST", "/run", false);
    request.send(data);
    load_state();
}

function stop() {
    let request = new XMLHttpRequest();
    request.open("POST", "/stop", false);
    request.send();
    load_state();
}

function load_state() {
    fetch('/state.csv').then(function(response){
        response.text().then(function(text){
            let mode = text.split(',')[0];
            let wind = text.split(',')[1];
            let temp = text.split(',')[2];
            let timer_on = text.split(',')[3];
            let timer_time = text.split(',')[4];
            let timer_mode = text.split(',')[5];
            let power = text.split(',')[6];
            switch(mode) {
                case '0':mode="暖房";break;
                case '1':mode="冷房";break;
                case '2':mode="ドライ";break;
                case '3':mode="送風";break;
            }
            switch(wind) {
                case '0':wind="自動";break;
                case '1':wind="微風";break;
                case '2':wind="弱風";break;
                case '3':wind="強風";break;
            }
            switch(power) {
                case '0':power="休止中";break;
                case '1':power="稼働中";break;
                case '2':
                    if(timer_mode == 0) power = "稼働中";
                    else if(timer_mode == 1) power = "待機中";
                    break;
                }

            if(power == "稼働中") document.getElementById("state_power").style.color = "darkgreen";
            else if(power == "待機中") document.getElementById("state_power").style.color = "darkorange";
            else if(power == "休止中") document.getElementById("state_power").style.color = "black";
            document.getElementById("state_power").innerHTML = power;

            if(power == "休止中") {
                document.getElementById("state_detail").innerHTML = " ";
            } else {
                document.getElementById("state_detail").innerHTML = mode + " / " + wind + " / " + temp + "℃";
                if(timer_on == 1) {
                    if(timer_time <= 60) document.getElementById("state_detail").innerHTML += " / " + timer_time + "分後";
                    else if(timer_time > 60) document.getElementById("state_detail").innerHTML += " / " + Math.floor(timer_time/60) + "時間後 ";
                    if(timer_mode == 1) document.getElementById("state_detail").innerHTML += "稼働";
                    else document.getElementById("state_detail").innerHTML += "休止";
                }
            }
        })
    });
}

load_state();