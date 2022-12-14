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
                case '0':mode="??????";break;
                case '1':mode="??????";break;
                case '2':mode="?????????";break;
                case '3':mode="??????";break;
            }
            switch(wind) {
                case '0':wind="??????";break;
                case '1':wind="??????";break;
                case '2':wind="??????";break;
                case '3':wind="??????";break;
            }
            switch(power) {
                case '0':power="?????????";break;
                case '1':power="?????????";break;
                case '2':
                    if(timer_mode == 0) power = "?????????";
                    else if(timer_mode == 1) power = "?????????";
                    break;
                }

            if(power == "?????????") document.getElementById("state_power").style.color = "darkgreen";
            else if(power == "?????????") document.getElementById("state_power").style.color = "darkorange";
            else if(power == "?????????") document.getElementById("state_power").style.color = "black";
            document.getElementById("state_power").innerHTML = power;

            if(power == "?????????") {
                document.getElementById("state_detail").innerHTML = " ";
            } else {
                document.getElementById("state_detail").innerHTML = mode + " / " + wind + " / " + temp + "???";
                if(timer_on == 1) {
                    if(timer_time <= 60) document.getElementById("state_detail").innerHTML += " / " + timer_time + "??????";
                    else if(timer_time > 60) document.getElementById("state_detail").innerHTML += " / " + Math.floor(timer_time/60) + "????????? ";
                    if(timer_mode == 1) document.getElementById("state_detail").innerHTML += "??????";
                    else document.getElementById("state_detail").innerHTML += "??????";
                }
            }
        })
    });
}

load_state();