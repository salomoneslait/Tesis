var express = require('express');
var bodyParser = require('body-parser');
var multer = require('multer');
var upload = multer();
var app = express();
var request = require('superagent');

app.use(express.urlencoded())

var http = require('http');

let backend_url = process.env.BACKEND_URL || "http://192.168.1.180"

var inc = 'false';
var magB = false;
var piroB = false

app.get('/', function(req, res){
   res.render('index',{incendio:true ,puerta:magB , proximidad:piroB});
});

// sudo docker run -dit --env BACKEND_URL=https://0e3f-2800-e2-bf80-c44-b92c-bacf-1997-7f6a.ngrok.io -p 3000:3000 front:latest 
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

   res.redirect('/');
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
   res.redirect('/');
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

const SECRET_KEY = "admin";
var dados = [];
var info;


app.post('/Enviar',function(req, res){
   
   mag = req.query.mag;
   let magB = (mag.toLowerCase() === 'true');
   piro = req.query.piro;
   let piroB = (piro.toLowerCase() === 'true');
   gas = req.query.piro;
   let gasB = (piro.toLowerCase() === 'true');
   
   console.log(magB);
   console.log(piroB);
   console.log(gasB);

   //----------------------------------------------------------------------Twilio
   //Instalar mpm i twilio 
   const TWILIO_ID = '';
   const TWILIO_SK = '';
   const client=require('twilio')(TWILIO_ID,TWILIO_SK);

   if(piro==true){
      client.messages.create({
         body: 'sensor de presencia activo', 
         from: 'whatsapp:+14155238886', 
         to: 'whatsapp:+573159268068'       
         // to: 'whatsapp:+573112541022' 
      }).then(message => console.log(message.sid));
   } else if (mag==true){
      client.messages.create({
         body: 'sensor de puerta activo', 
         from: 'whatsapp:+14155238886',       
         to: 'whatsapp:+573159268068'       
         // to: 'whatsapp:+573112541022'  
      }).then(message => console.log(message.sid));
   }else if(gas==true){
      client.messages.create({
         body: 'sensor de gas activo', 
         to: 'whatsapp:+573159268068'       
         // to: 'whatsapp:+573112541022'  
      }).then(message => console.log(message.sid));
   } else {
      console.log("sensores inactivos");
   }

//------------------------------------------------------------------FIN Twilio

   res.redirect("/");
  
});
app.listen(3000);