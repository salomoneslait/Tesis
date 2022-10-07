// Declare our dependencies
var express = require('express');
var request = require('superagent');

// Create our express app
var app = express();

// Set the view engine to use EJS as well as set the default views directory
app.set('view engine', 'ejs');
app.set('views', __dirname + '/public/views/');

// This tells Express out of which directory to serve static assets like CSS and images
app.use(express.static(__dirname + '/public'));

let backend_url = process.env.BACKEND_URL || "192.168.1.180"
let color = "123" ;
let home;
let range; 

// The homepage route of our application does not interface with the MovieAnalyst API and is always accessible. We won’t use the getAccessToken middleware here. We’ll simply render the index.ejs view.
app.get('/', function(req, res){
  res.render('index');
  // console.log("hola");
})

// For the movies route, we’ll call the getAccessToken middleware to ensure we have an access token. If we do have a valid access_token, we’ll make a request with the superagent library and we’ll be sure to add our access_token in an Authorization header before making the request to our API.
// Once the request is sent out, our API will validate that the access_token has the right scope to request the /movies resource and if it does, will return the movie data. We’ll take this movie data, and pass it alongside our movies.ejs template for rendering

// ?home=1&range=79&Color=%2304087c&set=on

app.get('/?home=1&range=50&Color=%2304087c', function(req, res){
  console.log("home");
  console.log(home);
  console.log("brillo");
  console.log(range);
  console.log("color");
  console.log(color);

  request
    .get(`http://${backend_url}?home=1&range=79&Color=%2304087c&set=on`)
    .end(function(err, data) {
      if(data.status == 403){
        res.send(403, '403 Forbidden');
      } else {
      }
    })
})

// The process will be the same for the remaining routes. We’ll make sure to get the acess_token first and then make the request to our API to get the data.
// The key difference on the authors route, is that for our client, we’re naming the route /authors, but our API endpoint is /reviewers. Our route on the client does not have to match the API endpoint route.
app.get('/off', function(req, res){
  request
    .get(`http://${backend_url}/off`)
    .end(function(err, data) {
      if(data.status == 403){
        res.send(403, '403 Forbidden');
      } else {
      }
    })
})


app.listen(process.env.PORT || 3030);