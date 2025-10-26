
// Seleciona o ID do elemento <img>
const sprite_blastoise = document.getElementById("blastoise-gif");

// // Função Async/Await
// // Função assíncrona, pode fazer outras coisas enquanto espera os dados.
// async function getPokemon(nome) {
//   try { // Vai tentar "try" executar o código
//     // o "fetch" vai fazer a requisição HTTP para a API
//     // "await" espera a resposta chegar antes de continuar
//     const resposta = await fetch(`https://pokeapi.co/api/v2/pokemon/${nome}`);
//     const dados = await resposta.json(); // Converte a resposta JSON para objeto
//     console.log(dados); // Caso tudo certo, imprime os dados no console
//   } catch (erro) {
//     console.error("Erro:", erro); // Caso erro com os dados, joga o erro!
//   }
// }
// getPokemon("charmander");

// Busca o Sprite na PokeAPI
fetch("https://pokeapi.co/api/v2/pokemon/blastoise")
  .then(response => response.json())
  .then(data => {
    // Caminho para o sprite animado
    const sprite_animado = data.sprites.versions["generation-v"]["black-white"].animated.front_default;

    // Atualiza a imagem no HTMl
    sprite_blastoise.src = sprite_animado;
  })
  .catch(error => console.error("Erro ao buscar Pokemon:", error));


// function ajustarEscala() {
//   const baseWidth = 1920;
//   const baseHeight = 1080;
//   const container = document.querySelector('.main-container');

//   const scaleX = window.innerWidth / baseWidth;
//   const scaleY = window.innerHeight / baseHeight;
//   const scale = Math.min(scaleX, scaleY);

//   container.style.transform = `scale(${scale})`;
//   container.style.transformOrigin = 'top center';

//   // Centraliza na tela
//   const offsetX = (window.innerWidth - baseWidth * scale) / 2;
//   const offsetY = (window.innerHeight - baseHeight * scale) / 2;
//   container.style.position = 'absolute';
//   container.style.left = `${offsetX}px`;
//   container.style.top = `${offsetY}px`;
// }

// window.addEventListener('resize', ajustarEscala);
// window.addEventListener('load', ajustarEscala);

