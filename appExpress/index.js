var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var upload = multer();
var app = express();
var request = require('superagent');

app.use(express.urlencoded())

var http = require('http');

app.get('/', function(req, res){
   res.render('form');
});

app.post("/", (req, res) => {
   console.log(req.body);

   request
   .post('https://5130-2800-e2-bf80-c44-8491-9c10-1188-cf96.ngrok.io')// .post('http://192.168.1.14/luces')
   .send(req.body)
   .set('X-API-Key', 'foobar')
   .set('Accept', 'application/json')
   .then(res => {
      alert('yay got ' + JSON.stringify(res.body));
   });

   res.redirect("/");
})

app.set('view engine', 'pug');
app.set('views', './views');

// for parsing application/json
app.use(bodyParser.json()); 

// for parsing application/xwww-
app.use(bodyParser.urlencoded({ extended: true })); 
//form-urlencoded

// for parsing multipart/form-data
app.use(upload.array()); 
app.use(express.static('public'));

app.post('/', function(req, res){
   console.log(req.body);
   res.send("recieved your request!");
});
app.listen(3000);