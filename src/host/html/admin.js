function logout() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/logout", true);
    xhr.send();
    setTimeout(function () { window.open("/", "_self"); }, 1000);
}

async function fetchConfiguration() {
    const response = await fetch("configuration");
    if (response.status == 200) {
        const configuration = await response.json();
        console.log(configuration, configuration.sensors);

        const sensors = configuration.sensors;
        for (const [mac, name] of Object.entries(sensors)) {
            addSensorRow(mac, name);
        }

        const sensorUpdatePeriod = document.getElementById("sensor_update_time");
        const serverPort = document.getElementById("server_port");
        sensorUpdatePeriod.value = configuration.sensorUpdatePeriodMins;
        serverPort.value = configuration.serverPort;

    }
    else if (response.status == 401) {
        console.log("Unauthorized");
    }
}

function sendSensorRemovalReq(id) {
    console.log("will be removed: ", id);

    fetch("/removeSensor", {
        method: "POST",
        headers: { "Content-type": "application/json" },
        body: JSON.stringify({ "identifier": id })
    });

    location.reload();
}

function removeSensorAction(id) {
    const dialog = document.getElementById("removeSensorDialog");
    dialog.showModal();

    const btnConfirmSensorRemoval = document.getElementById("btnConfirmSensorRemoval");
    btnConfirmSensorRemoval.onclick = function () { dialog.close(); sendSensorRemovalReq(id); };
}

function closeModal(modal) {
    const dialog = document.getElementById(modal);
    dialog.close();
}

function showInfoModal(info) {
    const dialog = document.getElementById("infoModal");
    let text = document.getElementById("infoModalText");
    text.innerText = info;

    dialog.showModal();
}

function addSensorRow(id, name) {
    var table = document.getElementById("sensorsTable");
    var row = table.insertRow();
    var cellIdentifier = row.insertCell(0);
    var cellName = row.insertCell(1);
    var cellAction = row.insertCell(2);

    cellName.contentEditable = "true";
    cellName.addEventListener('keydown', (evt) => {
        if (evt.keyCode === 13) {
            evt.preventDefault();
        }
    });

    row.id = id;
    cellIdentifier.innerHTML = id;
    cellName.innerHTML = name;
    cellAction.innerHTML = "<a href='#' onclick='removeSensorAction(" + id + ")'>Remove</a>";

    return row;
}

function sendSensorsMapping() {
    async function send(sensorsMapping) {
        try {
            let results = await fetch("/updateSensorsMapping", {
                method: "POST",
                headers: { "Content-type": "application/json" },
                body: sensorsMapping
            });

            if (results.status == 200) {
                showInfoModal("Sensors updated");
            }
            else {
                showInfoModal("Error, can't update sensors");
            }
        }
        catch (error) {
            showInfoModal("Error, can't update sensors");
        }
    }


    let sensorsMapping = {};
    let table = document.getElementById("sensorsTable");
    for (let i = 1, row; row = table.rows[i]; i++) {
        let id = row.cells[0].innerText;
        let name = row.cells[1].innerText.replace(/(\r\n|\n|\r)/gm, "");
        console.log(id, name);
        sensorsMapping[id] = name;
    }

    send(JSON.stringify(sensorsMapping));
}

function sendAdminCredentials() {
    async function send(credentials) {
        try {

            let results = await fetch("/setCredentials", {
                method: "POST",
                headers: { "Content-type": "application/json" },
                body: credentials
            });

            if (results.status != 200) {
                showInfoModal("Credentials not changed, unexpected internal server error");
            }
        }
        catch (error) {
            showInfoModal("Error, can't update credentials");
        }
    }

    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const re_password = document.getElementById("re_password").value;

    if (username.length === 0) {
        showInfoModal("Empty username");
        return;
    }
    else if (password.length === 0) {
        showInfoModal("Empty password");
        return;
    }
    else if (password !== re_password) {
        showInfoModal("Password not match");
        return;
    }

    let credentials = {
        "username": username,
        "password": password,
        "re_password": re_password,
    }
    errorLabel.innerText = "";

    send(JSON.stringify(credentials));
}

function sendOtherProperties() {
    async function send(properties) {
        try {
            let results = await fetch("/setProperties", {
                method: "POST",
                headers: { "Content-type": "application/json" },
                body: properties
            });

            if (results.status == 200) {
                showInfoModal("Parameters updated");
            }
            else {
                showInfoModal("Error, can't update parameters");
            }
        }
        catch (error) {
            showInfoModal("Error, can't update parameters");
        }
    }

    const sensorUpdatePeriod = document.getElementById("sensor_update_time").value;
    const serverPort = document.getElementById("server_port").value;

    let properties = {
        "sensorUpdatePeriodMins": Number(sensorUpdatePeriod),
        "serverPort": Number(serverPort)
    };

    send(JSON.stringify(properties));
}

window.addEventListener('load', function () {
    fetchConfiguration();
})
