function constraint_wind() {
    mode = document.getElementById("mode").value;
    if(mode == "dry") {
        document.getElementById("wind").innerHTML = "<option value='null'>指定不可</option>";
    } else {
        document.getElementById("wind").innerHTML = 
        "<option value='auto' >自動</option>\
        <option value='gentle'>微風</option>\
        <option value='weak'>弱風</option>\
        <option value='strong'>強風</option>";
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
        document.getElementById("time").innerHTML = "<option value='null'>指定不可</option>";
        document.getElementById("power").innerHTML = "<option value='null'>指定不可</option>";
    } else {
        document.getElementById("time").innerHTML = 
        "<option value='1'>1</option>\
        <option value='2'>2</option>\
        <option value='3'>3</option>\
        <option value='4'>4</option>\
        <option value='5'>5</option>\
        <option value='6'>6</option>\
        <option value='7'>7</option>\
        <option value='8'>8</option>\
        <option value='9'>9</option>\
        <option value='10'>10</option>\
        <option value='11'>11</option>\
        <option value='12'>12</option>";
        document.getElementById("power").innerHTML = 
        "<option value='on'>ON</option>\
        <option value='off'>OFF</option>";
    }
}