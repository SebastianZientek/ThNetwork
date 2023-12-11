class DayChart {
    axeWidth = 2;
    axeColor = "#222";
    gridWidth = 0.3;
    gridColor = "#555";
    lineWidth = 1;

    #leftMargin = 80;
    #rightMargin = 10
    #topMargin = 15;
    #bottomMargin = 100;

    #bottom = 0;
    #right = 0;

    #canvas;
    #ctx;

    constructor(canvas) {
        this.#canvas = canvas;
        this.#ctx = canvas.getContext("2d");

        this.#bottom = this.#canvas.height - this.#bottomMargin
        this.#right = this.#canvas.width - this.#rightMargin;
    }

    draw(data, begin, end, valueIndex) {
        this.#clearChart();
        this.#drawFrame();

        let valRange = this.#minMaxExpanded(data, valueIndex, 0.1);
        this.#plotData(data, begin, end, valueIndex, valRange);
        this.#drawLegendY(data, valueIndex, valRange);
        this.#drawLegendX(begin, end);
    }

    #clearChart() {
        this.#ctx.fillStyle = "#999"
        this.#ctx.clearRect(0, 0, canvas.width, canvas.height);
        this.#ctx.fillRect(0, 0, canvas.width, canvas.height);
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

    #plotData(data, begin, end, valueIndex, valRange) {
        this.#ctx.strokeStyle = "red";
        this.#ctx.lineWidth = this.lineWidth;
        this.#ctx.beginPath();

        for (const [i, entry] of data.entries()) {
            let epoch = entry[1];
            let value = entry[valueIndex];

            let x = this.#mapToAxisX(epoch, begin, end);
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
        this.#drawText(value.toFixed(2), posX, posY);
    }

    #drawFrame() {
        this.#drawVLineOnChart(this.#leftMargin, this.axeColor, this.axeWidth);
        this.#drawVLineOnChart(this.#right, this.axeColor, this.axeWidth);
        this.#drawHLineOnChart(this.#bottom, this.axeColor, this.axeWidth);
        this.#drawHLineOnChart(this.#topMargin, this.axeColor, this.axeWidth);
    }

    #drawText(text, x, y, style = "#000") {
        this.#ctx.fillStyle = style;
        this.#ctx.textAlign = "center";
        this.#ctx.textBaseline = "middle"
        this.#ctx.font = "9pt sans";
        this.#ctx.fillText(text, x, y);
    }

    #drawLegendY(data, valueIndex, valRange) {
        const rangeCenter = (valRange.min + valRange.max) / 2;
        const valuesSpan = valRange.max - valRange.min;
        let pivot = 0;

        if (valRange.min < 0 && valRange.max > 0) {
            pivot = 0;
        }
        else {
            pivot = rangeCenter;
        }

        // this.#drawHLineWithValue(valRange.max, valRange);
        this.#drawHLineWithValue(pivot, valRange);

        let step = valuesSpan / 8;
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

    #drawLegendX(begin, end) {
        const minPixelsPerTimestamp = 50;
        const fiveMinutesInSec = 300;
        const minTimeToPresent = this.#mapValueToTarget(this.#leftMargin + minPixelsPerTimestamp, this.#leftMargin, this.#right, begin, end) - begin;

        const stepFactor = Math.ceil(minTimeToPresent / fiveMinutesInSec);
        const step = stepFactor * fiveMinutesInSec;

        const epochFactor = Math.ceil(begin / fiveMinutesInSec);
        const startEpoch = epochFactor * fiveMinutesInSec;

        const posY = this.#bottom + 15;
        for (let epoch = startEpoch; epoch < end; epoch += step) {
            const x = this.#mapToAxisX(epoch, begin, end);
            const timeString = this.#epochToString(epoch);
            this.#drawText(timeString, x, posY, "black");
            this.#drawVLineOnChart(x, this.gridColor, this.gridWidth);
        }
    }

    #minMax(data, index) {
        const dt = data.map((node) => node[index]);
        return {
            "min": Math.min(...dt),
            "max": Math.max(...dt),
        }
    }

    #minMaxExpanded(data, valueIndex, expandFactor = 0.1) {
        const minMax = this.#minMax(data, valueIndex);
        const valuesSpan = minMax.max - minMax.min;
        let expanMinMax = minMax;
        expanMinMax.min -= valuesSpan * expandFactor;
        expanMinMax.max += valuesSpan * expandFactor;

        return expanMinMax;
    }
};
