$(document).ready(function() {
    function validateEmptyCredentials(credentials) {
        // removing extra spaces at the beginning or the end of the string.
        if (credentials.username.trim() == "" || credentials.password.trim() == "") {
            alert("Username/Password fields cannot be empty.");

            return false;
        }
        console.log("Username and password fields are not empty.");
        return true;
    }

    // function to check SQL Injection attack with the case of - or someValue = someValue.
    function checkSQLInjectionWithOR(str) {
        // splitting the string on the basis of space characters.
        var tokens = str.split(/\s+/);

/*         for (var i = 0; i < tokens.length; i++) {
            console.log("i : " + i + ", tokens[i] : " + tokens[i]);
        } */

        // checking if the token at the current index is '=', and it is a part of the required pattern "or someValue = someValue"
        for (var i = 0; i < tokens.length - 3; i++) {
            if (tokens[i] === "or" && tokens[i + 2] === "=" && (tokens[i + 1] === tokens[i + 3])) {
                console.log("SQL Injection attack with " + tokens[i + 1] + " = " + tokens[i + 3] + "!! Abort Login!!");

                return true;
            }
        }

        return false;
    }

    // function to convert a string of the form "or 1=1" (= without spaces) into one with spaces.
    function addSpacesWithEquals(str) {
        var temp = "";

        for (var i = 0; i < str.length; i++) {
            if (str[i] === "=") {
                temp += " = ";
            } else {
                temp += str[i];
            }
        }

        return temp;
    }

    // function to check all SQL Injection cases by converting the base input string to add spaces around '=', and remove all
    // quotes from the query to handle extra SQL Injection cases.
    function checkAllSQLInjectionCases(str) {
        var temp = "";

        str = addSpacesWithEquals(str);

        for (var i = 0; i < str.length; i++) {
            if (str[i] === "'" || str[i] === "\"") {
                continue;
            } else {
                temp += str[i];
            }
        }

        // after converting the input, check the OR condition and return the result.
        return checkSQLInjectionWithOR(temp);
    }

    $("#submitCredentials").click(function() {
        // getting the username and password values from the form.
        var username = document.getElementById("username").value;
        var password = document.getElementById("password").value;
        var credentials = {
            "username": username,
            "password": password
        }

        // check for SQL Injection in both, the username and the password.
        var areWeBeingAttacked = checkAllSQLInjectionCases(username.trim().toLowerCase()) | checkAllSQLInjectionCases(password.trim().toLowerCase());

        console.log("Are we being attacked? : " + areWeBeingAttacked);

        if (validateEmptyCredentials(credentials)) {
            // validateCredentials should be an API in the backend.
            $.post("validateCredentials", credentials, function(data, response) {
                alert("Data : " + data + "\nStatus : " + status + "\nResponse : " + reponse);
            });
        }
    });
});
