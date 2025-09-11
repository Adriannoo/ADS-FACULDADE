// Script moderno para manipulação do formulário de cadastro

document.addEventListener('DOMContentLoaded', () => {
  // Seleciona o formulário e o campo de retorno
  const cadastroForm = document.getElementById('cadastroUsuario');
  const retorno = document.getElementById('retorno');

  cadastroForm.addEventListener('submit', (e) => {
    e.preventDefault();

    // Obtém valores dos campos
    const nome = document.getElementById('userName').value.trim();
    const email = document.getElementById('userEmail').value.trim();
    const idade = document.getElementById('userAge').value.trim();

    // Validação básica
    if (!nome || !email) {
      retorno.textContent = 'Preencha nome e e-mail para continuar.';
      retorno.style.color = '#d32f2f';
      return;
    }

    // Mensagem de sucesso
    retorno.textContent = `Usuário cadastrado!\nNome: ${nome}\nE-mail: ${email}\nIdade: ${idade ? idade : 'Não informado'}`;
    retorno.style.color = '#2563eb';

    cadastroForm.reset();
  });
});
