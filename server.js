var express = require('express');
var app = express();

app.get('/', function(req, res){
  res.send('Display JSON data');
});

app.listen(3000);