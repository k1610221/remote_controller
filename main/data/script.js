function constraint_wind() {
    mode = document.getElementById("mode").value;
    if(mode == "dry") {
        document.getElementById("wind").options[4].selected = true;
        document.getElementById("wind").disabled = true;
    } else {
        document.getElementById("wind").options[0].selected = true;
        document.getElementById("wind").disabled = false;
    }
}

function constraint_temp() {
    mode = document.getElementById("mode").value;
    if(mode != "warm") {
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "none";
        document.getElementById("temp").options[3].selected = true;
    } else {
        for(let i = 0; i < 3; i++) document.getElementById("temp").options[i].style.display = "initial";
        document.getElementById("temp").options[3].selected = true;
    }
}

function constraint_time_power() {
    timer = document.getElementById("timer").value;
    if(timer == "false") {
        document.getElementById("time").disabled = true;
        document.getElementById("time").options[12].selected = true;
        document.getElementById("power").disabled = true;
        document.getElementById("power").options[2].selected = true;
    } else {
        document.getElementById("time").disabled = false;
        document.getElementById("time").options[0].selected = true;
        document.getElementById("power").disabled = false;
        document.getElementById("power").options[0].selected = true;
    }
}