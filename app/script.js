const formElement = document.getElementById("homeform");

formElement.addEventListener("submit", (event) => {
    event.preventDefault();
    let habitacion = document.getElementById("habitacion").value;
    let brillo = document.getElementById("customRange").value;
    let color = document.getElementById("ColorInput").value;
    let encendido = document.getElementById("flexSwitchCheckDefault").value;

    let estado = {habitacion: habitacion , brillo: brillo , color: color, encendido: encendido };
    let estadoJson = JSON.stringify(estado);

    console.log(estadoJson);

    // const url = 'https://pokeapi.co/api/v2/pokemon/1/'

    // fetch(url)
    // .then(response => response.json() )
    // .then(data => {
    //   console.log(data)
    // })
    // .catch(err=>console.log(err))

    // var url = 'http:192.168.1.180?home=`${habitacion}`$&range=16&Color=%237c0416&set=on';
    // var data = {username: 'example'};

    // fetch(`http://192.168.1.14/luces`, {
    //   method: 'POST', // or 'PUT'
    //   body: JSON.stringify(estado), // data can be `string` or {object}!
    //   headers:{
    //     'Content-Type': 'application/json'
    //   },
    //   mode: 'no-cors'
    // }).then(res => res.json())
    // .catch(error => console.error('Error:', error))
    // .then(response => console.log('Success:', response));
 
    let response = fetch(`http://192.168.1.14/luces`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      },
      mode: 'no-cors',
      body: JSON.stringify(estado)
    });
    

    
});

