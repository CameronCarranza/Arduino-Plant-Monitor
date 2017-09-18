import express from 'express';
import Promise from 'bluebird';
import db from 'sqlite';
import bodyParser from 'body-parser';

const app = express();
const port = process.env.PORT || 3000;

// Allow POST Data.
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies

// Get all Device Data
app.get('/', async (req, res, next) => {
    try {
        const devices = await Promise.all([
            db.all('SELECT * FROM Devices')
        ]);
        res.send(devices);
    } catch (err) {
        next(err);
    }
});

// Get Single Device Data
app.get('/devices/:device_key', async (req, res, next) => {
    try {
        const device = await Promise.all([
            db.get('SELECT * FROM Devices WHERE device_key = ?', req.params.device_key)
        ]);
        res.send(device);
    } catch (err) {
        next(err);
    }
});

// Post Data to a Single Device
app.post('/devices/:device_key', async (req, res, next) => {

    // Require sensorData parameter
    if ( ! req.body.sensorData) {
        res.send({
            status: false,
            error: "No Sensor Data provided."
        })
        return false;
    }

    // Limit of sensors (current maximum is 8 with a multiplexer)
    let sensorLimit = 8;

    // Get Sensor Values
    let sensorData = Object.values(JSON.parse(req.body.sensorData));
    sensorData = sensorData.filter(Number);
    sensorData = sensorData.slice(0, sensorLimit);

    // Fill array to have all sensors based on sensorLimit.
    if ( sensorData.length < sensorLimit ) {
        let mergeArray = new Array(sensorLimit - sensorData.length).fill(0);
        sensorData = sensorData.concat(mergeArray);
    }

    // Add API Key to end of array
    sensorData.push(req.params.device_key);

    // Update Device info
    try {
        await Promise.all([
            db.run(
                'UPDATE Devices SET sensor1 = ?, sensor2 = ?, sensor3 = ?, sensor4 = ?, sensor5 = ?, sensor6 = ?, sensor7 = ?, sensor8 = ? WHERE device_key = ?',
                sensorData
            ),
        ]);
        res.send({
            status: true,
            message: 'Device data successfully updated.'
        });
    } catch (err) {
        next(err);
    }
});

// Initialization
Promise.resolve()
    .then(() => db.open('./database.sqlite', { Promise }))
    .catch(err => console.error(err.stack))
    .finally(() => app.listen(port));
