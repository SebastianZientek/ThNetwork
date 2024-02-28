const MAX_ARR_ELEMS = 220; // Should be send as configuration by server
const TIME_IDX = 0;
const TEMPERATURE_IDX = 1;
const HUMIDITY_IDX = 2;

var gSensorsData = {};
var gSensorIDsToNames = {};

async function fetchSensorsMapping() {
    const response = await fetch("sensorIDsToNames");
    const sensors = await response.json();
    return sensors;
}

async function initialFetchSensorsData(sensorsData, temperatureChart, humidityChart) {
    gSensorIDsToNames = await fetchSensorsMapping();

    for (const [identifier, name] of Object.entries(gSensorIDsToNames)) {
        const dataResponse = await fetch('sensorData?' + new URLSearchParams({
            "identifier": identifier
        }))

        const readings = await dataResponse.json();

        addDataToSensor(sensorsData, readings, name);
        temperatureChart.draw(sensorsData, TEMPERATURE_IDX);
        humidityChart.draw(sensorsData, HUMIDITY_IDX);
    }
}

function removeOlderReadingsThanOneDay(data) {
    for (const [identifier, payload] of Object.entries(data)) {
        let values = payload.values;

        // Sort by time
        values.sort((a, b) => a[TIME_IDX] - b[TIME_IDX]);
        let elemsToRemove = values.length - MAX_ARR_ELEMS;
        if (elemsToRemove > 0) {
            data[identifier].values = values.slice(elemsToRemove);
        }

        const secInDay = 24 * 60 * 60;
        const currentEpoch = Math.round(Date.now() / 1000);
        const dayBeforeEpoch = currentEpoch - secInDay;

        // Filter too old readings
        data[identifier].values = data[identifier].values.filter((reading) => reading[TIME_IDX] > dayBeforeEpoch);
    }
}

function addDataToSensor(sensorsData, reading, name) {
    function setInitialData() {
        const identifier = reading["identifier"]
        const values = reading["values"];
    
        sensorsData[identifier] = {
            "name": name,
            "values": values
        };
    }

    const identifier = reading["identifier"]
    const values = reading["values"][0];

    if (sensorsData.hasOwnProperty(identifier)) {
        sensorsData[identifier]["values"] = sensorsData[identifier]["values"] || [];
        sensorsData[identifier]["values"].push(values);
    }
    else {
        setInitialData();
    }
}

window.addEventListener('load', function () {
    const temperatureCanvas = document.getElementById("temperatureCanvas");
    const temperatureChart = new MicroChart(temperatureCanvas, "Temperature");

    const humidityCanvas = document.getElementById("humidityCanvas");
    const humidityChart = new MicroChart(humidityCanvas, "Humidity");

    initialFetchSensorsData(gSensorsData, temperatureChart, humidityChart);
    temperatureChart.draw(gSensorsData, TEMPERATURE_IDX);
    humidityChart.draw(gSensorsData, HUMIDITY_IDX);

    window.addEventListener('resize', function () {
        temperatureChart.draw(gSensorsData, TEMPERATURE_IDX);
        humidityChart.draw(gSensorsData, HUMIDITY_IDX);
    })

    if (!!window.EventSource) {
        var source = new EventSource('/events');

        source.addEventListener('open', function (e) {
            console.log("Connected");
        }, false);

        source.addEventListener('error', function (e) {
            if (e.target.readyState != EventSource.OPEN) {
                console.log("Disconnected");
            }
        }, false);

        source.addEventListener('newReading', function (e) {
            const reading = JSON.parse(e.data);

            let sensorName = "No name";
            if (gSensorIDsToNames.hasOwnProperty(reading.identifier)) {
                sensorName = gSensorIDsToNames[reading.identifier];
            }

            addDataToSensor(gSensorsData, reading, sensorName);
            removeOlderReadingsThanOneDay(gSensorsData);
            temperatureChart.draw(gSensorsData, TEMPERATURE_IDX);
            humidityChart.draw(gSensorsData, HUMIDITY_IDX);
        }, false);
    }
})
