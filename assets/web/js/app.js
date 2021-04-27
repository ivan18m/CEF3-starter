$(document).ready(function() {
    $('.btn').click(function() {
        window.app.ChangeTextInJS('Hello World!');
    });
});

function ChangeTextInJS(text) {
    $('#text').html(text);
}
