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
 
    let response = fetch(`http://192.168.1.14/luces`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json;charset=utf-8'
      },
      mode: 'no-cors',
      body: JSON.stringify(estado)
    });
    

    
});

