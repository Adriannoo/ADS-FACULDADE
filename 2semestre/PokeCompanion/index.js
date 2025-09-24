// JSON estático (mock)
const pokedex = {
  id: 384,
  nome: "Rayquaza",
  tipos: ["Dragão", "Voador"],
  altura: 7.0,   // em metros
  peso: 206.5,   // em kg
  habilidades: ["Air Lock"],
  stats: {
    hp: 105,
    ataque: 150,
    defesa: 90,
    ataqueEspecial: 150,
    defesaEspecial: 90,
    velocidade: 95
  },
  sprites: {
    default: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/384.png",
    shiny: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/shiny/384.png",
    animado: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/versions/generation-v/black-white/animated/384.gif"
  }
};

// JSON dinâmico (fetch)
// "fetch" para fazer uma requisição HTTP
// retorna um "promise", seria os dados que estão por vir.
fetch("https://pokeapi.co/api/v2/pokemon/rayquaza")
  .then(response => response.json()) // Espera a promise e transforma em JSON
  .then(data => {   // Recebe os dados já convertidos
    // Nome
    console.log("Name", data.name);

    // Tipos
    const types = data.types.map(t => t.type.name);
    console.log("Types", types);

    // Status
    const stats = {};
    data.stats.forEach(stat => {
      stats[stat.stat.name] = stat.base_stat;
    });
    console.log("Stats:", stats);
  })
  // Caso a promise for rejeitada, joga o erro!
  .catch(error => console.error("Erro ao buscar pokémon!", error));


// Seleciona o ID do elemento <img>
const sprite_blastoise = document.getElementById("sprite-blastoise");

// Função Async/Await
// Função assíncrona, pode fazer outras coisas enquanto espera os dados.
async function getPokemon(nome) {
  try { // Vai tentar "try" executar o código
    // o "fetch" vai fazer a requisição HTTP para a API
    // "await" espera a resposta chegar antes de continuar
    const resposta = await fetch(`https://pokeapi.co/api/v2/pokemon/${nome}`);
    const dados = await resposta.json(); // Converte a resposta JSON para objeto
    console.log(dados); // Caso tudo certo, imprime os dados no console
  } catch (erro) {
    console.error("Erro:", erro); // Caso erro com os dados, joga o erro!
  }
}
getPokemon("charmander");

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