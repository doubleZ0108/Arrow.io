var io = require('socket.io')(2333);
var i_map1 = 0;
var i_map2 = 0;
var i_map3 = 0;
var member_1 = 0;
var member_2 = 0;
var member_3 = 0;
var list_enemy1 = new Array();
var list_enemy2 = new Array();
var list_enemy3 = new Array();

console.log('running');
io.on('connection', function (socket) {
    var id = 0;
    console.log('connect');
    console.log(socket.handshake.address);
    socket.on("aaa", function (aaa) {
        console.log("aaa", aaa);
    });

    socket.once('mapchose', function (data) {
        console.log('join map');
        console.log(data);
        if (data == '1') {
            socket.join(data);
            member_1++;
            id = i_map1;
            socket.handshake.userAgent = id;
            io.to(socket.id).emit('IDJudge', id);
            console.log(socket.handshake.userAgent, 'id', id);
            //socket.to(data).emit('IDJudge', i_map1++);
            i_map1++;
        }
        else if (data == '2') {
            socket.join(data);
            id = i_map2;
            member_2++;
            io.to(socket.id).emit('IDJudge', id);
            //socket.to(data).emit('IDJudge', i_map2++);
            i_map2++;
        }
        else if (data == '3') {
            socket.join(data);
            id = i_map3;
            member_3++;
            io.to(socket.id).emit('IDJudge', id);
            //socket.to(data).emit('IDJudge', i_map3++);
            i_map3++;
        }
        socket.on('owner', function (uQuit) {
            socket.broadcast.to(data).emit('owner', "guest");
            console.log('room', data, 'owner has been changed');
        });
        socket.on('HP position', function (hp) {
            socket.broadcast.to(data).emit('HP position', hp);
            console.log('room', data, 'HP position recieved', hp);
        });
        socket.on('EXP position', function (exp) {
            console.log('room', data, 'EXP position recieved');
            socket.broadcast.to(data).emit('EXP position', exp);
        });
        socket.on('HP remove', function (hpd) {
            console.log('room', data, 'remove HP');
            console.log(hpd);
            socket.broadcast.to(data).emit('HP remove', hpd);
        });
        socket.on('EXP remove', function (exd) {
            console.log('room', data, 'remove EXP');
            console.log(exd);
            socket.broadcast.to(data).emit('EXP remove', exd);
        });
        socket.on('n_player keys', function (keys) {
            socket.broadcast.to(data).emit('n_player keys', keys);
        });
        socket.once('n_player keys', function (keys) {
            console.log('room', data, 'player keys recieved', keys);
        });
        socket.on('n_player run', function (run) {
            socket.broadcast.to(data).emit('n_player run', run);
        });
        socket.once('n_player run', function (run) {
            console.log('room', data, 'player run recieved', run);
        });
        socket.on('attack', function (attack) {
            socket.broadcast.to(data).emit('attack', attack);
            console.log('attack', attack);
        });
        socket.on('hurt', function (hurt) {
            socket.broadcast.to(data).emit('hurt', hurt);
            console.log(id, 'hurt', hurt);
        });
        socket.on('hero nature', function (hero) {
            socket.broadcast.to(data).emit('hero nature', hero);
            console.log('hero nature', hero);
        });
        socket.on('restart point', function (restart) {
            socket.broadcast.to(data).emit('restart point', restart);
            console.log('restart point', restart);
        });
        socket.on('red hp', function (red) {
            socket.broadcast.to(data).emit('red hp', red);
            console.log('red hp', red);
        });

        socket.on('playerchose', function (character) {
            if (data == '1') {
                list_enemy1.push(character);
                for (var i = 0; i < id; i++) {
                    if (list_enemy1[i] != "") io.to(socket.id).emit('enemy', list_enemy1[i]);
                    console.log("enemy get 1");
                }
            }
            else if (data == '2') {
                list_enemy2.push(character);
                for (var i = 0; i < id; i++)
                    if (list_enemy2[i] != "") io.to(socket.id).emit('enemy', list_enemy2[i]);
                console.log("enemy get 2");
            }
            else if (data == '3') {
                list_enemy3.push(character);
                for (var i = 0; i < id; i++)
                    if (list_enemy3[i] != "") io.to(socket.id).emit('enemy', list_enemy3[i]);
                console.log("enemy get 3");
            }
            socket.broadcast.to(data).emit('enemy', character);
            console.log('room', data, 'player character recieved');
            console.log(character);
        });
        socket.on('quit', function (quit) {
            socket.disconnect(true);
            console.log('22346');
        });
        socket.on('disconnect', function (quit) {
            console.log(id, quit);
            console.log(id, 'quitroom called');
            if (data == '1' && list_enemy1[id] != "") {
                if (member_1) member_1--;
                console.log('member in map1', member_1);
                if (!member_1) {
                    i_map1 = 0;
                    list_enemy1.splice(0, list_enemy1.length);
                }
                else {
                    list_enemy1[id] = "";
                }

            }
            if (data == '2' && list_enemy2[id] != "") {
                if (member_2) member_2--;
                console.log('member in map2', member_2);
                if (!member_2) {
                    i_map2 = 0;
                    list_enemy2.splice(0, list_enemy2.length);
                }
                else {
                    list_enemy2[id] = "";
                }
            }
            if (data == '3' && list_enemy3[id] != "") {
                if (member_3) member_3--;
                console.log('member in map3', member_3);
                if (!member_3) {
                    i_map3 = 0;
                    list_enemy3.splice(0, list_enemy3.length);
                }
                else {
                    list_enemy3[id] = "";
                }
            }
            socket.broadcast.to(data).emit('quit', id);
            console.log(socket.handshake.userAgent);
            console.log('leave', data);
            setTimeout(() => socket.disconnect(true), 5000);
            console.log(socket.handshake.address);
            console.log('lost');
        });

        console.log('mapchose OK');
    });
});