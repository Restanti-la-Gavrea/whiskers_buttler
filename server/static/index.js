
function getAvailableRobotsList(){
    return fetch('/robots').then(response => response.json());
}
async function resetRobotListOptions(){
    let robotsList = await getAvailableRobotsList();
    const selectRobot = document.getElementById('selectRobot');
    selectRobot.innerHTML = "";
    for(let robot of robotsList){
        const option = document.createElement('option');
        option.value = robot;
        option.textContent = robot;
        selectRobot.appendChild(option);
    }
}
function connectToSelectedRobot(){
    const robotName = document.getElementById('selectRobot').value;
    if(robotName != ""){
        window.location.href += '/joystick/' + robotName;
    }
}
function init(){
    resetRobotListOptions();
}
window.onload = init;
   