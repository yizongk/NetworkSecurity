var dbUtils = require('./scripts/dbUtils');

var express = require('express'),
    app = express(),
    path = require('path'),
    bodyParser = require('body-parser'),
    dbUtils = require('./scripts/dbUtils.js');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());
app.use(express.urlencoded({ extended: true} ));
app.use(express.json());
app.use(express.static(__dirname + '/views'));
app.use(express.static(__dirname + '/scripts'));
app.listen(3000);

app.post('/', (request, response) => {
    response.sendFile(path.join(__dirname + '/views/index.html'));
})

app.post('/addUser', (request, response) => {
    var params = request.body;
    dbUtils.addUser(params.username, params.password);
});

app.delete('/deleteUser', (request, response) => {
    var params = request.body;
    dbUtils.deleteUser(params.username, params.password);
});

app.put('/updateUser', (request, response) => {
    var params = request.body;
    dbUtils.updateUser(params.oldUsername, params.oldPassword, params.newUsername, params.newPassword);
});

app.get('/getAllUsers', (request, response) => {
    dbUtils.getAllUsers(response);
});

// route to create tables, to be run only once, to create tables initially on a system.
app.get('/createTables', (request, response) => {
    dbUtils.createTables();
    response.send('Tables created');
});
