function constraint_wind() {
    var selected = document.getElementById("mode").value;
    if(selected == 2) { // dry
        document.getElementById("wind").options[0].selected = true;
        document.getElementById("wind").options[1].hidden = true;
        document.getElementById("wind").options[2].hidden = true;
        document.getElementById("wind").options[3].hidden = true;
    } else { // warm, cool, fan
        document.getElementById("wind").options[0].hidden = false;
        document.getElementById("wind").options[1].hidden = false;
        document.getElementById("wind").options[2].hidden = false;
        document.getElementById("wind").options[3].hidden = false;
    }
}

function constraint_temp() {
    var selected = document.getElementById("mode").value;
    if(selected == 0) { // warm
        document.getElementById("temp").options[0].selected = "true";
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "initial";
    } else { // cool, dry, fan
        document.getElementById("temp").options[3].selected = "true";
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "none";
    }
}

function display_detail() {
    var selected = document.getElementById("timer").value;
    if(selected == 1) document.getElementById("detail").hidden = false;
    else document.getElementById("detail").hidden = true;
}

function send() {
    var form = document.getElementById("command");
    // 쿠키 가져와서 현재 에어컨이 켜진 상태인지 꺼진 상태인지 파악
    if(document.getElementById("timer").value == 0) {
        document.getElementById("time").value = 0;
        document.getElementById("timer_mode").value = 0;
    }
    form.submit();
}

function terminate() {
    // var form = 쿠키로부터 현재상태 가져오기
}