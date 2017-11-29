function locationSuccess(position) {
   var lat = position.coords.latitude;
   var lon = position.coords.longitude;
   var dictionary = {
        "KEY_LAT": lat,
        "KEY_LONG": lon
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Location get got!");
          console.log("Location, lat: " + lat + " long: " + lon);
        },
        function(e) {
          console.log("Location not got get!");
        }
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getLataLong() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}


// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    getLataLong();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
   getLataLong();
  }                     
);
