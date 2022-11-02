var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var upload = multer();
var app = express();
var request = require('superagent');

app.use(express.urlencoded())

var http = require('http');

//----------------------------------------------------------------------Twilio
//Instalar mpm i twilio 
const TWILIO_ID = 'AC0ae00199225e55bd898190cf8e2dca57';
const TWILIO_SK = '99c848746ad595c3d9aaf533d06c553b';

const client=require('twilio')(TWILIO_ID,TWILIO_SK);

if(fire=false){
   client.messages.create({
      body: 'Hay un incendio', 
      from: 'whatsapp:+14155238886',       
      to: 'whatsapp:+573112541022' 
   }).then(message => console.log(message.sid));
   
}else{
   client.messages.create({
      body: 'No hay incendio', 
      from: 'whatsapp:+14155238886',       
      to: 'whatsapp:+573112541022' 
   }).then(message => console.log(message.sid));
}

//------------------------------------------------------------------FIN Twilio

let backend_url = process.env.BACKEND_URL || "https://12f1-2800-e2-bf80-c44-1937-f588-ebab-a577.ngrok.io"

app.get('/', function(req, res){
   res.render('index');
});
// sudo docker run -dit --env BACKEND_URL=b78a-181-206-21-114.ngrok.io -p 3030:3030 front:boton 
app.post("/", (req, res) => {
   console.log(req.body);

   request
   .post(`${backend_url}/luces`)
   .send(req.body)
   .set('X-API-Key', 'foobar')
   .set('Accept', 'application/json')
   .then(res => {
      alert('yay got ' + JSON.stringify(res.body));
   });

   res.redirect("/");
})

app.post("/tomacorrientes", (req, res) => {
   console.log(req.body);

   request
   .post(`${backend_url}/tomacorrientes`)
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