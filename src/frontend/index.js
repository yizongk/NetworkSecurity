$(document).ready(function() {
    $('#updateUser').click(function() {
        var htmlUpdate = "<input type='text' class='form-control' id='newUsername' placeholder='New Username' />"
                        + "<input type='password' class='form-control' id='newPassword' placeholder='New Password' />"
                        + "<button id='update' type='submit' class='btn btn-primary'>Update</button>";

        $('button').hide();
        $('.form-group').append(htmlUpdate);

        $('#update').click(function() {
            var oldUsername = $('#username').val();
            var oldPassword = $('#password').val();
            var newUsername = $('#newUsername').val();
            var newPassword = $('#newPassword').val();
            var url = '/updateUser';

            var data = {
                oldUsername: oldUsername,
                oldPassword: oldPassword,
                newUsername: newUsername,
                newPassword: newPassword
            }
    
            $.ajax({
                url: url,
                data: JSON.stringify(data),
                type: "PUT",
                contentType: "application/json",
                success: function(resp, statusText, c) {
                    // console.log("success, resp : " + resp + ", statusText : " + statusText);
                },
                error: function(err, statusText, c) {
                    alert(err.responseText);
                }
            });
        });
    });

    $('#addUser').click(function() {
        var username = $('#username').val();
        var password = $('#password').val();
        var url = '/addUser';

        var data = {
            username: username,
            password: password
        }
    
        $.ajax({
            url: url,
            data: JSON.stringify(data),
            type: "POST",
            contentType: "application/json",
            success: function(resp, statusText, c) {
                // console.log("success, resp : " + resp + ", statusText : " + statusText);
            },
            error: function(err, statusText, c) {
                alert(err.responseText);
            }
        });
    });

    $('#deleteUser').click(function() {
        var username = $('#username').val();
        var password = $('#password').val();
        var url = '/deleteUser';

        var data = {
            username: username,
            password: password
        }

        $.ajax({
            url: url,
            data: JSON.stringify(data),
            type: "DELETE",
            contentType: "application/json",
            success: function(resp, statusText, c) {
                // console.log("success, resp : " + resp + ", statusText : " + statusText);
            },
            error: function(err, statusText, c) {
                alert(err.responseText);
            }
        });
    });

    $('#getAllUsers').click(function() {
        var username = $('#username').val();
        var password = $('#password').val();
        var url = '/getAllUsers';

        var data = {
            username: username,
            password: password
        }

        $.ajax({
            url: url,
            type: "GET",
            success: function(resp, statusText, c) {
                $.each(JSON.parse(resp), function(ind, elem) {
                    $('ul.display-users').append('<li class="list-group-item">' + elem.username + '</li>');
                });
            },
            error: function(err, statusText, c) {
                alert(err.responseText);
            }
        });
    });
});
