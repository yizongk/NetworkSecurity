$( document ).ready(function() {
    console.log( "ready!" );

    $("#submitCredentials").click(function() {
        var username = document.getElementById("username").value;
        var password = document.getElementById("password").value;
        var credentials = {
            "username": username,
            "password": password
        }
    
        console.log("displaying");
        console.log("credentials : " + JSON.stringify(credentials));
    
        // validateCredentials should be an API in the backend.
        $.post("validateCredentials", credentials, function(data, response) {
            alert("Data : " + data + "\nStatus : " + status + "\nResponse : " + reponse);
        });
    });
});
