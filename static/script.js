const uploadBtn = document.getElementById('upload-btn');
const fileInput = document.getElementById('file-input');
const codeInput = document.getElementById('code-input');
const checkBtn = document.getElementById('check-btn');
const output = document.getElementById('output');

uploadBtn.addEventListener('click', function() {
    fileInput.click();
});

fileInput.addEventListener('change', function(e) {
    const file = e.target.files[0];
    if (file) {
        if (!file.name.endsWith('.c')) {
            alert('Please select a .c file');
            return;
        }
        const reader = new FileReader();
        reader.onload = function(event) {
            codeInput.value = event.target.result;
        };
        reader.readAsText(file);
    }
});

checkBtn.addEventListener('click', async function() {
    const code = codeInput.value.trim();

    if (!code) {
        alert('Please enter some C code to check');
        return;
    }

    checkBtn.disabled = true;
    checkBtn.textContent = 'Checking...';
    output.innerHTML = '<p class="placeholder">Checking your code...</p>';

    try {
        const response = await fetch('/check', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ code: code })
        });

        const data = await response.json();

        if (data.error) {
            output.innerHTML = '<div class="error-message">' + data.error + '</div>';
        } else if (!data.hasErrors || data.errors.length === 0) {
            output.innerHTML = '<div class="success">No syntax errors found!</div>';
        } else {
            let html = '<ul class="error-list">';
            data.errors.forEach(function(error) {
                html += '<li class="error-item">' + error + '</li>';
            });
            html += '</ul>';
            output.innerHTML = html;
        }
    } catch (error) {
        output.innerHTML = '<div class="error-message">Error: ' + error.message + '</div>';
    } finally {
        checkBtn.disabled = false;
        checkBtn.textContent = 'Check Syntax';
    }
});


