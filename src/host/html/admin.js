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
        const sensors = configuration.sensors;

        for (const [identifier, name] of Object.entries(sensors)) {
            insertSensorToTable(identifier, name);
        }

        const sensorUpdatePeriod = document.getElementById("sensorUpdatePeriod");
        const serverPort = document.getElementById("serverPort");
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
    const removeSensorDialog = document.getElementById("removeSensorDialog");
    removeSensorDialog.showModal();

    const btnConfirmSensorRemoval = document.getElementById("btnConfirmSensorRemoval");
    btnConfirmSensorRemoval.onclick = function () {
        removeSensorDialog.close(); sendSensorRemovalReq(id);
    };
}

function closeModal(modalName) {
    const dialog = document.getElementById(modalName);
    dialog.close();
}

function showInfoModal(text) {
    const dialog = document.getElementById("infoModal");
    let infoModalText = document.getElementById("infoModalText");
    infoModalText.innerText = text;

    dialog.showModal();
}

function insertSensorToTable(id, name) {
    var sensorsTable = document.getElementById("sensorsTable");
    var row = sensorsTable.insertRow();
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
    const rePassword = document.getElementById("rePassword").value;

    if (username.length === 0) {
        showInfoModal("Empty username");
        return;
    }
    else if (password.length === 0) {
        showInfoModal("Empty password");
        return;
    }
    else if (password !== rePassword) {
        showInfoModal("Password not match");
        return;
    }

    let credentials = {
        "username": username,
        "password": password,
        "rePassword": rePassword,
    }

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

    const sensorUpdatePeriod = document.getElementById("sensorUpdatePeriod").value;
    const serverPort = document.getElementById("serverPort").value;

    let properties = {
        "sensorUpdatePeriodMins": Number(sensorUpdatePeriod),
        "serverPort": Number(serverPort)
    };

    send(JSON.stringify(properties));
}

window.addEventListener('load', function () {
    fetchConfiguration();
})
