class MicroChart {
    axeWidth = 1;
    axeColor = "#999";
    gridWidth = 0.3;
    gridColor = "#999";
    lineWidth = 2;
    legendTxtColor = "#DDD"

    #leftMargin;
    #rightMargin;
    #topMargin;
    #bottomMargin;
    #bottom;
    #right;

    #canvas;
    #ctx;
    #title;

    constructor(canvas, title = "unnamed") {
        this.#canvas = canvas;
        this.#ctx = canvas.getContext("2d");
        this.#title = title;

        this.#setSizes();
    }

    #setSizes() {
        this.#ctx = this.#canvas.getContext("2d");

        this.#leftMargin = 80;
        this.#rightMargin = 180;
        this.#topMargin = 70;
        this.#bottomMargin = 50;
        this.#bottom = this.#canvas.height - this.#bottomMargin
        this.#right = this.#canvas.width - this.#rightMargin;
    }

    draw(data, yValuesIndex) {
        const xValuesIndex = 0;
        this.#canvas.style.width = '100%';
        this.#canvas.width = this.#canvas.clientWidth;

        this.#canvas.style.height = '100%';
        this.#canvas.height = this.#canvas.clientHeight;

        this.#setSizes();
        this.#clearChart();
        this.#drawFrame();

        let yMinMax = this.#calcCommonMinMax(data, yValuesIndex);
        let yRange = this.#expandRange(yMinMax, 0.2);
        let xRange = this.#calcCommonMinMax(data, xValuesIndex);

        xRange.min = Math.min(xRange.max - 1200, xRange.min); // Minimum is 20min range

        for (const [sensor, values] of Object.entries(data)) {
            this.#plotData(values, xRange, yRange, yValuesIndex, this.#toRGB(sensor));
        }

        this.#drawLegendY(yRange);
        this.#drawLegendX(xRange);
        this.#drawLegendNames(data, yValuesIndex);
        this.#drawTitle()
    }

    #calcCommonMinMax(data, index) {
        let mins = [];
        let maxes = [];
        for (const [sensor, values] of Object.entries(data)) {
            let [min, max] = this.#minMax(values, index);
            mins.push(min);
            maxes.push(max);
        }

        return { "min": Math.min(...mins), "max": Math.max(...maxes) };
    }

    #toRGB(str) {
        var hash = 0;
        if (str.length === 0) return hash;
        for (var i = 0; i < str.length; i++) {
            hash = str.charCodeAt(i) + ((hash << 5) - hash);
            hash = hash & hash;
        }
        var rgb = [0, 0, 0];
        for (var i = 0; i < 3; i++) {
            var value = (hash >> (i * 8)) & 150;
            rgb[i] = value;
        }
        return `rgb(${rgb[0]}, ${rgb[1]}, ${rgb[2]})`;
    }

    #expandRange(range, expandFactor) {
        let exRange = {
            ...range
        }
        const valuesSpan = range.max - range.min;
        exRange.min -= valuesSpan * expandFactor;
        exRange.max += valuesSpan * expandFactor;

        exRange.min = Math.floor(exRange.min) - 1; // HACK
        exRange.max = Math.ceil( exRange.max) + 1; // HACK

        return exRange;
    }

    #drawLegendNames(data, valuesIndex) {
        let step = 0;
        for (const [sensor, values] of Object.entries(data)) {
            const top = this.#topMargin + 10;
            const y = top + (15 * step)

            let sensorName = sensor;
            if (sensorName.length > 15) {
                sensorName = sensorName.slice(0, 15) + "...";
            }

            let lastVal = values[values.length - 1][valuesIndex];
            let text = "[" + lastVal.toFixed(1) + "] " + sensorName;

            this.#ctx.fillStyle = this.#toRGB(sensor);
            this.#ctx.fillRect(this.#right + 10, y - 5, 10, 10);
            this.#drawText(text, this.#right + 25, y, this.legendTxtColor, "left")
            step++;
        }
    }

    #drawTitle() {
        this.#drawText(this.#title, this.#canvas.width / 2, 30, this.legendTxtColor, "center", "12pt sans");
    }

    #clearChart() {
        this.#ctx.fillStyle = "#171a1e"
        this.#ctx.clearRect(0, 0, this.#canvas.width, this.#canvas.height);
        this.#ctx.fillRect(0, 0, this.#canvas.width, this.#canvas.height);
    }

    #mapValueToTarget(value, srcMin, srcMax, tarMin, tarMax) {
        const srcRange = srcMax - srcMin;
        const tarRange = tarMax - tarMin;
        return (value - srcMin) * tarRange / srcRange + tarMin;
    }

    #mapToAxisX(value, min, max) {
        return this.#mapValueToTarget(value, min, max, this.#leftMargin, this.#right);
    }

    #mapToAxisY(value, min, max) {
        return this.#mapValueToTarget(value, min, max, this.#bottom, this.#topMargin);
    }

    #plotData(data, xRange, valRange, yValuesIndex, color) {
        const xValuesIndex = 0;
        this.#ctx.strokeStyle = color;
        this.#ctx.lineWidth = this.lineWidth;
        this.#ctx.beginPath();

        for (const [i, entry] of data.entries()) {
            let epoch = entry[xValuesIndex];
            let value = entry[yValuesIndex];

            let x = this.#mapToAxisX(epoch, xRange.min, xRange.max);
            let y = this.#mapToAxisY(value, valRange.min, valRange.max);
            if (i == 0) {
                this.#ctx.moveTo(x, y);
            }
            else {
                this.#ctx.lineTo(x, y);
            }
        }
        this.#ctx.stroke();
    }

    #drawLine(x1, y1, x2, y2, style = "#000", width = 1) {
        this.#ctx.strokeStyle = style;
        this.#ctx.lineWidth = width;

        this.#ctx.beginPath();
        this.#ctx.moveTo(x1, y1);
        this.#ctx.lineTo(x2, y2);
        this.#ctx.stroke();
    }

    #drawVLineOnChart(posX, style, width) {
        this.#drawLine(posX, this.#topMargin, posX, this.#bottom, style, width);
    }

    #drawHLineOnChart(posY, style, width) {
        this.#drawLine(this.#leftMargin, posY, this.#right, posY, style, width);
    }

    #drawHLineWithValue(value, range) {
        const posY = this.#mapToAxisY(value, range.min, range.max);
        const posX = this.#leftMargin - 30;

        this.#drawHLineOnChart(posY, this.gridColor, this.gridWidth);
        this.#drawText(value.toFixed(2), posX, posY, this.legendTxtColor);
    }

    #drawFrame() {
        this.#drawVLineOnChart(this.#leftMargin, this.axeColor, this.axeWidth);
        this.#drawVLineOnChart(this.#right, this.axeColor, this.axeWidth);
        this.#drawHLineOnChart(this.#bottom, this.axeColor, this.axeWidth);
        this.#drawHLineOnChart(this.#topMargin, this.axeColor, this.axeWidth);
    }

    #drawText(text, x, y, style = "#000", textAlign = "center", font = "9pt sans") {
        this.#ctx.fillStyle = style;
        this.#ctx.textAlign = textAlign;
        this.#ctx.textBaseline = "middle"
        this.#ctx.font = font;
        this.#ctx.fillText(text, x, y);
    }

    #drawLegendY(valRange) {
        let rangeCenter = (valRange.min + valRange.max) / 2;
        rangeCenter = Math.round(rangeCenter);

        let valuesSpan = valRange.max - valRange.min;
        let pivot = 0;

        if (valuesSpan == 0) valuesSpan = 1;

        if (valRange.min < 0 && valRange.max > 0) {
            pivot = 0;
        }
        else {
            pivot = rangeCenter;
        }

        this.#drawHLineWithValue(valRange.max, valRange);
        this.#drawHLineWithValue(pivot, valRange);
        this.#drawHLineWithValue(valRange.min, valRange);

        let step = valuesSpan / 8;
        let rounding = 0.5
        step = Math.round(step / rounding) * rounding;
        for (; step == 0; rounding /= 2) {
            step = Math.round(step / rounding) * rounding;
        }

        if (step == 0) {
            console.log("Too small step, set to 0.5");
            step = 0.5;
        }

        let currentValue = pivot;
        while (currentValue + step < valRange.max) {
            currentValue += step;
            this.#drawHLineWithValue(currentValue, valRange);
        }

        currentValue = pivot;
        while (currentValue - step > valRange.min) {
            currentValue -= step;
            this.#drawHLineWithValue(currentValue, valRange);
        }
    }

    #epochToString(epoch) {
        var d = new Date(0);
        d.setUTCSeconds(epoch);
        const h = String(d.getHours()).padStart(2, '0');
        const m = String(d.getMinutes()).padStart(2, '0');

        return h + ":" + m;
    }

    #drawLegendX(xRange) {
        const minPixelsPerTimestamp = 50;
        const fiveMinutesInSec = 300;
        const minTimeToPresent = this.#mapValueToTarget(this.#leftMargin + minPixelsPerTimestamp, this.#leftMargin, this.#right, xRange.min, xRange.max) - xRange.min;

        const stepFactor = Math.ceil(minTimeToPresent / fiveMinutesInSec);
        const step = stepFactor * fiveMinutesInSec;

        const epochFactor = Math.ceil(xRange.min / fiveMinutesInSec);
        const startEpoch = epochFactor * fiveMinutesInSec;

        const posY = this.#bottom + 15;
        for (let epoch = startEpoch; epoch < xRange.max; epoch += step) {
            const x = this.#mapToAxisX(epoch, xRange.min, xRange.max);
            const timeString = this.#epochToString(epoch);
            this.#drawText(timeString, x, posY, this.legendTxtColor);
            this.#drawVLineOnChart(x, this.gridColor, this.gridWidth);
        }
    }

    #minMax(data, index) {
        const dt = data.map((node) => node[index]);
        return [Math.min(...dt), Math.max(...dt)]
    }
};
