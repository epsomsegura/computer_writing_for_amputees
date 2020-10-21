// MODELS
const Diccionario = sequelize.define('Diccionario', {
    // Model attributes are defined here
    palabra: {
        type: DataTypes.STRING,
    },
}, {
    tableName: 'diccionario',
    timestamps: false
});
const Personalizado = sequelize.define('Personalizado', {
    // Model attributes are defined here
    palabra: {
        type: DataTypes.STRING,
    },
}, {
    tableName: 'personalizado',
    timestamps: false
});




// CONSTANTS
// SOCKET INTERACTION
const socket = io.connect('http://localhost:5000', {
    transports: ['polling'],
    autoConnect: false,
    reconnection: true,
    reconnectionAttempts: Infinity,
});

var static_cell = 4;



// FUNCTIONS
function gestureType(gesture) {
    switch (gesture) {
        case 'up':
        case 'down':
        case 'left':
        case 'right':
            moveGesture(gesture);
            break;
        case 'backspace':
            eraseGesture();
            break;
        case 'select':
            selectGesture();
            break;
    }
}

function moveGesture(gesture) {
    $active_key = $('.active');
    // CELLS
    $elements = $active_key.parent().find('.btn');
    // ROWS
    $row_elements = $active_key.parent().parent();
    $row_container = $row_elements.parent();

    var
        active = 0,
        new_pos = 0;

    $.each($elements, function (i, val) {
        if ($(this).hasClass('active')) active = i;
    });

    switch (gesture) {
        case 'right':
        case 'left':
            if (gesture == 'right') {
                if (active == 0 && $elements.length == 1)
                    new_pos = 0;
                else if ($elements.length == 5 && active == 4)
                    new_pos = 0;
                else if (active == 9)
                    new_pos = 0;
                else if (active >= 0)
                    new_pos = active + 1;
            }
            else if (gesture == 'left') {
                if (active == 0 && $elements.length == 1)
                    new_pos = 0;
                else if (active > 0) {
                    new_pos = active - 1;
                }
                else if ($elements.length == 5 && active == 0) {
                    new_pos = 4;
                }
                else if (active == 0) {
                    new_pos = 9;
                }
            }
            static_cell = new_pos;
            $active_key.removeClass('active');
            $active_key.parent().find('.btn:eq(' + new_pos + ')').addClass('active');

            break;
        case 'up':
        case 'down':
            $id = $row_elements.attr('id');
            $rows = $row_container.find('div.row');

            if (gesture == 'up') {
                if ($row_elements.attr('id') == 'results') {
                    $('#space_row').find('div').find('.btn:eq(2)').addClass('active');
                }
                else {
                    $id_prev = $row_elements.prev().attr('id');
                    if ($id_prev == 'results') {
                        $('#' + $id_prev).find('div').find('.btn:eq(0)').addClass('active');
                    }
                    else {
                        $('#' + $id_prev).find('div').find('.btn:eq(' + static_cell + ')').addClass('active');
                    }
                }

            }
            else if (gesture == 'down') {
                if ($row_elements.attr('id') == 'space_row') {
                    $('#results').find('div').find('.btn:eq(0)').addClass('active');
                }
                else {
                    $id_next = $row_elements.next().attr('id');
                    if ($id_next == 'space_row') {
                        $('#' + $id_next).find('div').find('.btn:eq(2)').addClass('active');
                    }
                    else {
                        $('#' + $id_next).find('div').find('.btn:eq(' + static_cell + ')').addClass('active');
                    }
                }
            }

            $active_key.removeClass('active');

            break;
    }
}

function selectGesture() {
    // SEND SELECTED WORD
    if ($('.active').hasClass('btn_result')) {
        sendSelectedWord($('.active').text());

        $('.btn_result').remove();
        $('.word').html('');
    }
    // SEND SELECTED WORD AND SAVE NEW WORD
    else if ($('.active').hasClass('word')) {
        saveNewWord();
        sendSelectedWord($('.active').text());

        $('.btn_result').remove();
        $('.word').html('');
    }
    // TOGGLE SYMBOLS
    else if($('.active').hasClass('btn_symbols')){
        isSymbols();
    }
    // TOGGLE UPPERCASE
    else if ($('.active').hasClass('btn_mayus')) {
        isMayus();
    }
    // ERASE TEXT
    else if ($('.active').hasClass('btn_erase')) {
        socket.emit('select', 'backbackspace');
    }
    // SEND SPACE
    else if ($('.active').hasClass('space')) {
        socket.emit('select', 'spaspace');
    }
    // SEND ENTER
    else if ($('.active').hasClass('btn_select')) {
        socket.emit('select', 'ententer');
    }
    // SEND DOT
    else if ($('.active').hasClass('btn_dot')) {
        socket.emit('select', 'dodot');
    }
    // SEND COMMA
    else if ($('.active').hasClass('btn_comma')) {
        socket.emit('select', 'cocomma');
    }
    // SEND SYMBOL
    else if ($('.active').hasClass('symbol')) {
        socket.emit('select', $('.active').text());
    }
    else {
        var char = $('.active').text();
        $('.word').append(char);
        if ($('.word').text().length > 1)
            findPossibleWords($('.word').text());
    }
}

function eraseGesture() {
    var text = $('.word').text();
    text = text.substring(0, text.length - 1);
    $('.word').text(text);
}

async function findPossibleWords(searchWord) {

    var list_words = [];
    var x = await Diccionario.findAll({
        where: {
            palabra: {
                [Op.like]: searchWord.replace('_', '') + '%',
            }
        },
        limit: 4
    });
    var y = await Personalizado.findAll({
        where: {
            palabra: {
                [Op.like]: searchWord + '%',
            }
        },
        limit: 2
    });

    $.each(y, function (i, val) { list_words.push(val.dataValues.palabra); });
    $.each(x, function (i, val) { list_words.push(val.dataValues.palabra); });

    $('.btn_result').remove();
    $.each(list_words, function (i, val) {
        div = '<div class="btn s1 green btn_result">' + val + '</div>';
        $('#result_row').append(div);
    })
}

async function saveNewWord() {
    if (isNaN($('.word').text())) {
        var new_word = Personalizado.create({ palabra: ($('.word').text().replace('_', '')).toLowerCase() });
        M.toast({ html: 'Palabra guardada en el diccionario' });
    }
}

function sendSelectedWord(selectedWord) {
    $('#results').find('div').find('.btn:eq(0)').addClass('active');
    socket.emit('select', selectedWord.replace('_', ' '));
}


function isMayus() {
    var char_keys = $('.btn_char');
    if ($('.btn_char').first().text() == 'q') {
        $.each(char_keys, function (i, val) {
            $(this).text($(this).attr('data-mayus'));
        });
        $('.btn_mayus').removeClass('green').addClass('blue');
    }
    else if ($('.btn_char').first().text() == 'Q') {
        $.each(char_keys, function (i, val) {
            $(this).text($(this).attr('data-minus'));
        });
        $('.btn_mayus').removeClass('blue').addClass('green');
    }
}

function isSymbols() {
    var char_keys = $('.btn_char');
    if ($('.btn_char').first().text() == 'Q' || $('.btn_char').first().text() == 'q') {
        $.each(char_keys, function (i, val) {
            $(this).text($(this).attr('data-symbol'));
        });
        $('.btn_symbols').removeClass('green').addClass('blue');
        $('.btn_char').addClass('symbol')
    }
    else if ($('.btn_char').first().text() == '!') {
        $.each(char_keys, function (i, val) {
            $(this).text($(this).attr('data-minus'));
        });
        $('.btn_symbols').removeClass('blue').addClass('green');
        $('.btn_char').removeClass('symbol')
    }
}







// EVENTS
$(document).ready(function () {
    socket.connect();
    if (socket.connected) socket.close();
    socket.on('connect', function () {
        console.log('Device connected');
    });

    var char_keys = $('.btn_char');
    $.each(char_keys, function (i, val) {
        $(this).html($(this).attr('data-mayus'));
    });
});

$(document).on('mouseover', '.btn', function () {
    $('.btn').removeClass('hover');
    $(this).addClass('hover');
});

// SELECT EVENT
$(document).on('click', '.btn', function () {
    $('.active').removeClass('active');
    $('.hover').addClass('active');
    selectGesture();

});
// ERASE EVENT
$(document).on('click', '.btn_erase', function () { eraseGesture(); });





// SOCKET EVENTS
socket.on('disconnect', function () {
    setTimeout(function () { socket.connect(); }, 100);
})

socket.on('interaction', function (data) {
    gestureType(data);
    socket.close();
});