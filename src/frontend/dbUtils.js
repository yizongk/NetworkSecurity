var mysql = require('mysql');

module.exports = {
    getConnection: function() {
        // use your credentials
        var connection = mysql.createConnection({
            host     : 'localhost',
            user     : 'root',
            password : 'password', 
            database : 'sample_db'
        });
        
        connection.connect();
        return connection;
    },
    
    addUser: function(username, password) {
        var connection = this.getConnection();

        var queryString = 'INSERT INTO login (username, password) VALUES (?, ?);';
        var prepared = [connection.escape(username), connection.escape(password)];
        queryString = mysql.format(queryString, prepared);

        var doesUserExistString = 'SELECT username FROM login WHERE username = ?;';
        prepared = [connection.escape(username)];
        doesUserExistString = mysql.format(doesUserExistString, prepared);  

        connection.query(doesUserExistString, (error1, results1) => {
            if (error1) {
                throw error1;
            }

            if (results1.length > 0) {
                console.log(username + ' already exists');
            } else {
                connection.query(queryString, function(error2, result2) {
                    if (error2) {
                        throw error2;
                    }
            
                    console.log(username + ' added');
                });
            }
            
            connection.end();
        });
    },
    
    deleteUser: function(username, password) {
        var connection = this.getConnection();

        var deleteUserString = 'DELETE FROM login WHERE username = ? AND password = ?'; 
        var prepared = [connection.escape(username), connection.escape(password)];
        deleteUserString  = mysql.format(deleteUserString, prepared);

        connection.query(deleteUserString, (error, result) => {
            if (error) {
                throw error;
            }
            
            if (result.affectedRows > 0) {
                console.log(username + ' deleted');
            } else {
                console.log(username + ' does not exist');
            }
            
            connection.end();
        });
    },
    
    updateUser: function(oldUsername, oldPassword, newUsername, newPassword) {
        var connection = this.getConnection();

        var updateUserString = 'UPDATE login SET username = ?, password = ? WHERE username = ? AND password = ?;';
        var prepared = [connection.escape(newUsername), connection.escape(newPassword), 
                        connection.escape(oldUsername), connection.escape(oldPassword)];
        updateUserString = mysql.format(updateUserString, prepared);

        var doesUserExistString = 'SELECT username FROM login WHERE username = ?;';
        prepared = [connection.escape(newUsername)];
        doesUserExistString = mysql.format(doesUserExistString, prepared); 

        connection.query(doesUserExistString, (error1, results1) => {
            if (error1) {
                throw error1;
            }

            if (results1.length > 0) {
                console.log(newUsername + ' already exists');
            } else {
                connection.query(updateUserString, function(error2, results2) {
                    if (error2) {
                        throw error2;
                    }
                    
                    if (results2.affectedRows > 0) {
                        console.log(oldUsername + ' updated to ' + newUsername);
                    } else {
                        console.log(oldUsername + ' does not exist');
                    }
                });
            }
            
            connection.end();
        });
    },
    
    getAllUsers: function(response) {
        var connection = this.getConnection();
        var getAllUsersString = 'SELECT username FROM login';
        var users = [];

        connection.query(getAllUsersString, (error, results) => {
            if (error) {
                throw error;
            } else {
                response.json(JSON.stringify(results));
            }
            
            connection.end();
        });
    },

    createTables: function() {
        var connection = this.getConnection();
        var createLoginTableString = 'CREATE TABLE `login` (' +
                                     '`ID` int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,' + 
                                     '`username` varchar(50) NOT NULL,' +
                                     '`password` varchar(50) NOT NULL)';
        
        connection.query(createLoginTableString, function(error, rows, fields) {
            if (error) {
                throw error;
            }

            console.log('createTables worked');
            connection.end();
        });
    }
};
