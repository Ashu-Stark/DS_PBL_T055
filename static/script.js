// Tab switching
document.querySelectorAll('.tab-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        const tabName = btn.dataset.tab;
        
        // Update buttons
        document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        
        // Update content
        document.querySelectorAll('.tab-content').forEach(content => {
            content.classList.remove('active');
        });
        document.getElementById(`${tabName}-tab`).classList.add('active');
        
        // Clear file input when switching to paste tab
        if (tabName === 'paste') {
            document.getElementById('file-input').value = '';
            document.getElementById('file-name').classList.remove('show');
        }
    });
});

// File upload
const uploadArea = document.getElementById('upload-area');
const fileInput = document.getElementById('file-input');
const fileName = document.getElementById('file-name');

uploadArea.addEventListener('click', () => fileInput.click());

uploadArea.addEventListener('dragover', (e) => {
    e.preventDefault();
    uploadArea.style.borderColor = '#6366f1';
});

uploadArea.addEventListener('dragleave', () => {
    uploadArea.style.borderColor = '';
});

uploadArea.addEventListener('drop', (e) => {
    e.preventDefault();
    uploadArea.style.borderColor = '';
    
    const files = e.dataTransfer.files;
    if (files.length > 0) {
        handleFile(files[0]);
    }
});

fileInput.addEventListener('change', (e) => {
    if (e.target.files.length > 0) {
        handleFile(e.target.files[0]);
    }
});

function handleFile(file) {
    if (!file.name.endsWith('.c')) {
        alert('Please upload a .c file');
        return;
    }
    
    fileName.textContent = `📄 ${file.name}`;
    fileName.classList.add('show');
    
    const reader = new FileReader();
    reader.onload = (e) => {
        document.getElementById('code-input').value = e.target.result;
        // Switch to paste tab to show the code
        document.querySelector('[data-tab="paste"]').click();
    };
    reader.readAsText(file);
}

// Syntax check
const checkBtn = document.getElementById('check-btn');
const codeInput = document.getElementById('code-input');
const outputContent = document.getElementById('output-content');
const statusBadge = document.getElementById('status-badge');

checkBtn.addEventListener('click', async () => {
    const code = codeInput.value.trim();
    
    if (!code) {
        alert('Please enter or upload some C code to check');
        return;
    }
    
    // Show loading state
    checkBtn.disabled = true;
    checkBtn.querySelector('.btn-text').style.display = 'none';
    checkBtn.querySelector('.btn-loader').style.display = 'inline-block';
    
    // Clear previous results
    outputContent.innerHTML = '<div class="placeholder"><p>⏳ Checking code...</p></div>';
    statusBadge.classList.remove('show', 'success', 'error');
    
    try {
        const response = await fetch('/check', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ code: code })
        });
        
        const data = await response.json();
        
        if (data.error) {
            showError(data.error);
            return;
        }
        
        displayResults(data);
        
    } catch (error) {
        showError('Failed to connect to server: ' + error.message);
    } finally {
        // Reset button
        checkBtn.disabled = false;
        checkBtn.querySelector('.btn-text').style.display = 'inline';
        checkBtn.querySelector('.btn-loader').style.display = 'none';
    }
});

function displayResults(data) {
    outputContent.innerHTML = '';
    
    if (!data.hasErrors || data.errors.length === 0) {
        // No errors
        statusBadge.textContent = '✓ No Errors';
        statusBadge.className = 'status-badge show success';
        outputContent.innerHTML = `
            <div class="success-message">
                <p>🎉 Great! No syntax errors found.</p>
                <p style="margin-top: 10px; font-size: 0.9rem; color: var(--text-secondary);">
                    Your code passed all syntax checks.
                </p>
            </div>
        `;
    } else {
        // Has errors
        statusBadge.textContent = `✗ ${data.errors.length} Error${data.errors.length > 1 ? 's' : ''}`;
        statusBadge.className = 'status-badge show error';
        
        const errorList = document.createElement('ul');
        errorList.className = 'error-list';
        
        data.errors.forEach(error => {
            const errorItem = document.createElement('li');
            errorItem.className = 'error-item';
            errorItem.textContent = error;
            errorList.appendChild(errorItem);
        });
        
        outputContent.appendChild(errorList);
    }
}

function showError(message) {
    statusBadge.textContent = '✗ Error';
    statusBadge.className = 'status-badge show error';
    
    outputContent.innerHTML = `
        <div class="error-message">
            <strong>Error:</strong> ${message}
        </div>
    `;
}

// Allow Ctrl+Enter to run check
codeInput.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        checkBtn.click();
    }
});


