// Aguarda o carregamento completo do DOM
document.addEventListener('DOMContentLoaded', function() {
    // Obtém referências aos elementos do formulário
    const form = document.getElementById('userForm');
    const nameInput = document.getElementById('name');
    const emailInput = document.getElementById('email');
    const nameError = document.getElementById('nameError');
    const emailError = document.getElementById('emailError');
    const messageDiv = document.getElementById('message');
    
    /**
     * Valida se um campo está preenchido
     * @param {string} value - Valor do campo a ser validado
     * @returns {boolean} - Retorna true se o campo estiver preenchido
     */
    function validateRequired(value) {
        return value.trim() !== '';
    }
    
    /**
     * Valida o formato de e-mail usando expressão regular
     * @param {string} email - E-mail a ser validado
     * @returns {boolean} - Retorna true se o e-mail for válido
     */
    function validateEmail(email) {
        const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
        return emailRegex.test(email);
    }
    
    /**
     * Exibe uma mensagem de sucesso ou erro
     * @param {string} text - Texto da mensagem
     * @param {string} type - Tipo da mensagem (success ou error)
     */
    function showMessage(text, type) {
        messageDiv.textContent = text;
        messageDiv.className = `message ${type}`;
        messageDiv.classList.remove('hidden');
        
        // Oculta a mensagem após 5 segundos
        setTimeout(() => {
            messageDiv.classList.add('hidden');
        }, 5000);
    }
    
    /**
     * Limpa os campos do formulário
     */
    function clearForm() {
        form.reset();
        nameError.textContent = '';
        emailError.textContent = '';
    }
    
    /**
     * Valida todos os campos do formulário
     * @returns {boolean} - Retorna true se todos os campos obrigatórios forem válidos
     */
    function validateForm() {
        let isValid = true;
        
        // Validação do campo nome
        if (!validateRequired(nameInput.value)) {
            nameError.textContent = 'Por favor, preencha seu nome';
            isValid = false;
        } else {
            nameError.textContent = '';
        }
        
        // Validação do campo e-mail
        if (!validateRequired(emailInput.value)) {
            emailError.textContent = 'Por favor, preencha seu e-mail';
            isValid = false;
        } else if (!validateEmail(emailInput.value)) {
            emailError.textContent = 'Por favor, insira um e-mail válido';
            isValid = false;
        } else {
            emailError.textContent = '';
        }
        
        return isValid;
    }
    
    /**
     * Manipula o evento de envio do formulário
     * @param {Event} event - Evento de submissão do formulário
     */
    function handleSubmit(event) {
        // Previne o envio padrão do formulário
        event.preventDefault();
        
        // Valida o formulário
        if (validateForm()) {
            // Exibe mensagem de sucesso
            showMessage('Cadastro realizado com sucesso!', 'success');
            
            // Limpa o formulário
            clearForm();
        } else {
            // Exibe mensagem de erro
            showMessage('Por favor, corrija os erros no formulário.', 'error');
        }
    }
    
    // Adiciona o event listener para o evento de submit do formulário
    form.addEventListener('submit', handleSubmit);
});